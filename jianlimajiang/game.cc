#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <sstream>
#include <list>
#include <set>

#include "zjh.h"
#include "game.h"

#include "table.h"
#include "proto/proto.h"
#include "proto/gameproto.pb.h"
#include "common/log.h"
#include "common/client.h"
#include "common/player.h"
#include "robot_manager.h"
#include "libnormalmahjong/common.h"
#include "common/client_buffer.h"

extern ZJH zjh;
extern Log log;


static void game_del_client(Client *client, bool del_oldclient)
{
	zjh.game->del_client(client, del_oldclient);
}
static int game_del_player(Player *player)
{
	return zjh.game->del_player(player);
}
int game_dispatch_client(Client *client)
{
	return zjh.game->dispatch_client(client);
}
int game_dispatch_server(Client *client)
{
	return zjh.game->dispatch_server(client);
}
static int GameEndScoreFilter_0(Buffer* buffer)
{
	try
	{
		Client_Buffer* h = (Client_Buffer*)buffer->dpos();

		char tmp[0x10000] = "";
		strncpy(tmp, h->json_data, h->json_head.length);
		tmp[sizeof(tmp) - 1] = '\0';

		Jpacket jp;
		int iRet = jp.parse(tmp, h->json_head.length);
		if (iRet != 0)
		{
			log.error("%s parse error!\n", __FUNCTION__);
		}
		string r = jp.val["roomid"].asString();
		if (!r.empty())
		{
			int roomid = atoi(r.c_str());
			return roomid;
		}
	}	
	catch (...)
	{
	}
	return 0;	
}
static void GameEndScoreFilter(int iScore[GAME_PLAYER], Buffer* buffer)
{
	Client_Buffer* h = (Client_Buffer*)buffer->dpos();

	char tmp[0x10000] = "";
	strncpy(tmp, h->json_data, h->json_head.length);
	tmp[sizeof(tmp) - 1] = '\0';

	Jpacket jp;
	int iRet = jp.parse(tmp, h->json_head.length);
	if (iRet != 0)
	{
		log.error("%s parse error!\n", __FUNCTION__);
	}
	for (unsigned int i = 0; i < jp.val["players"].size(); i++)
	{
		iScore[i] += jp.val["players"][i]["alter_money"].asInt();
	}
}
static void GameEndScoreRepackage(int iScore[GAME_PLAYER], Buffer* buffer, Jpacket& out)
{
	Client_Buffer* h = (Client_Buffer*)buffer->dpos();

	char tmp[0x10000] = "";
	strncpy(tmp, h->json_data, h->json_head.length);
	tmp[sizeof(tmp) - 1] = '\0';

	int iRet = out.parse(tmp, h->json_head.length);
	if (iRet != 0)
	{
		log.error("%s parse error!\n", __FUNCTION__);
	}
	for (unsigned int i = 0; i < out.val["players"].size(); i++)
	{
		out.val["players"][i]["alter_money"] = iScore[i];
	}
	out.end();
}

int game_send_filter(Client* client, std::list<Buffer*>& _write_q)
{
	for (int i = 0; i < DATA_CLINET_MAX; i++)
	{	
		if (client == zjh.game->link_data_client[i])
		{
			MJ_BUFFER < Buffer*, 0x500> target;
			for (std::list<Buffer*>::iterator it = _write_q.begin(); it != _write_q.end(); it++)
			{
				JsonHeaderEx* header = (JsonHeaderEx*)(*it)->pdata();
				if (header->cmd == SERVER_DZ_UPDATE_USER_INFO || header->cmd == SERVER_DZ_UPDATE_USER_INFO_LITTLE)
				{
					target.Add(*it);
				}
			}
			if (target.Size() > 1)
			{
				std::set<int> roomid_set;
				for (int i = 0; i < target.Size(); i++)
				{
					int roomid = GameEndScoreFilter_0(target[i]);
					roomid_set.insert(roomid);
				}
				if (roomid_set.size() > 1)
					return 0;

				int iScore[GAME_PLAYER] = { 0 };
				for (int i = 0; i < target.Size(); i++)
				{
					GameEndScoreFilter(iScore, target[i]);
				}
				Jpacket out;
				GameEndScoreRepackage(iScore, target.Last(), out);
				for (int i = 0; i < target.Size(); i++)
				{
					_write_q.remove(target[i]);
					delete target[i];
				}
				zjh.game->send_to_datasvr(out.tostring());
				log.info("%s size %d, final score[%d,%d,%d,%d]!\n", __FUNCTION__, target.Size(), iScore[0], iScore[1], iScore[2], iScore[3]);
				return target.Size();
			}
		}	
	}
	return 0;
}

Game::Game()
{
	_fd = 0;
	link_data_client_index = 0;
	for (int i = 0; i < DATA_CLINET_MAX; i++)
		link_data_client[i] = NULL;	
	link_video_client = NULL;
	ev_reconnect_datasvr_tstamp = 5;
	ev_reconnect_datasvr_timer.data = this;
	ev_timer_init( &ev_reconnect_datasvr_timer, Game::reconnect_datasvr_cb, ev_reconnect_datasvr_tstamp, ev_reconnect_datasvr_tstamp );
	
	server_zid = zjh.conf["tables"]["zid"].asInt();
	server_vid = zjh.conf["tables"]["vid"].asInt();
	log.info("game svr zid[%d] vid[%d].\n", server_zid, server_vid );
	game_name = zjh.conf["game_name"].asString();
	game_number = zjh.conf["game_number"].asInt();
	
	link_video_client = NULL;
	ev_reconnect_videosvr_timer.data = this;
	ev_reconnect_videosvr_tstamp = 1;
	ev_timer_init( &ev_reconnect_videosvr_timer, Game::reconnect_videosvr_cb, ev_reconnect_videosvr_tstamp, ev_reconnect_videosvr_tstamp );
}

Game::~Game()
{
	for (int i = 0; i < DATA_CLINET_MAX; i++)
	{
		if (NULL != link_data_client[i])
		{
			delete link_data_client[i];
		}
	}
	if (NULL != link_video_client)
	{
		delete link_video_client;
	}

	ev_timer_stop( zjh.loop, &ev_reconnect_datasvr_timer );
	ev_timer_stop( zjh.loop, &ev_reconnect_videosvr_timer);
	
}

int Game::start()
{
	static InitPlayer init_player(zjh.loop, zjh.conf, game_del_player);
	static InitClient init_client(zjh.loop, game_del_client, game_dispatch_client, game_dispatch_server, game_send_filter);

	for (int i = 0; i < DATA_CLINET_MAX; i++)
		connect_datasvr(i);
	bool has_connect = false;
	for (int i = 0; i < DATA_CLINET_MAX; i++)
	{
		if (link_data_client[i] != NULL)
		{
			has_connect = true;
		}
		else
		{
			start_reconnect(i);
		}
	}
	if ( !has_connect )
	{
		log.info("%s:%d\t%s can not connect server \n", __FILE__, __LINE__, __FUNCTION__);
		exit(0);
	}
	connect_videosvr();
	
	request_data_from_redis();

	init_accept();
	return 0;
}

int Game::init_accept()
{
	log.info("Listening on %s:%d\n",
			zjh.conf["game"]["host"].asString().c_str(),
			zjh.conf["game"]["port"].asInt());

	struct sockaddr_in addr;

	_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (_fd < 0) {
		log.error("File[%s] Line[%d]: socket failed: %s\n", __FILE__, __LINE__, strerror(errno));
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(zjh.conf["game"]["port"].asInt());
	addr.sin_addr.s_addr = inet_addr(zjh.conf["game"]["host"].asString().c_str());
	if (addr.sin_addr.s_addr == INADDR_NONE) {
		log.error("game::init_accept Incorrect ip address!");
		close(_fd);
		_fd = -1;
		exit(1);
	}

	int on = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		log.error("File[%s] Line[%d]: setsockopt failed: %s\n", __FILE__, __LINE__, strerror(errno));
		close(_fd);
		exit(1);
		return -1;
	}

	if (bind(_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		log.error("File[%s] Line[%d]: bind failed: %s\n", __FILE__, __LINE__, strerror(errno));
		close(_fd);
		exit(1);
		return -1;
	}

	fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL, 0) | O_NONBLOCK);
	listen(_fd, 10000);

	_ev_accept.data = this;
	ev_io_init(&_ev_accept, Game::accept_cb, _fd, EV_READ);
	
	ev_io_start(zjh.loop, &_ev_accept);

	log.info("listen ok\n");
	return 0;
}

int Game::connect_datasvr(int index)
{
	std::string data_svr = "data-svr";
	if (index != 0)
		data_svr += to_char(index);

	log.info("datasvr management ip[%s] port[%d].\n",
				zjh.conf[data_svr]["host"].asString().c_str(),
				zjh.conf[data_svr]["port"].asInt() );
	
	struct sockaddr_in addr;
	int fd = socket(PF_INET, SOCK_STREAM, 0);	
	if (fd < 0) 
	{
		log.error("File[%s] Line[%d]: socket failed: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(zjh.conf[data_svr]["port"].asInt());
	addr.sin_addr.s_addr = inet_addr(zjh.conf[data_svr]["host"].asString().c_str());
	if (addr.sin_addr.s_addr == INADDR_NONE) 
	{
		log.error("game::connect_datasvr Incorrect ip address!");
		close(fd);
		fd = -1;
		exit(1);
	}
	if( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(fd);
		log.error("connect data svr error: %s(errno: %d)\n", strerror(errno), errno);
		fd = -1;
		return -1;
	}
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	link_data_client[index] = new(std::nothrow) Client(fd, false, 1);
	if( NULL != link_data_client[index])
	{
		log.info("connect data svr fd[%d].\n", fd );
		register_server(index);
	}
	else
	{
		close(fd);
		log.error("new data svr client error.\n");
		exit(1);
	}
	return 0;

}
void Game::register_server(int index)
{
	Jpacket packet;
	packet.val["cmd"] = SERVER_REGISTER_REQ;
	packet.val["vid"] = server_zid;
	packet.val["host"] = zjh.conf["game"]["host"].asString();
	packet.val["port"] = zjh.conf["game"]["port"].asInt();
	packet.val["index"] = index;
	packet.val["new_index"] = link_data_client_index;
	packet.end();
	send_to_datasvr(packet.tostring(), link_data_client[index]);
}

void Game::start_reconnect(int index)
{
	if (!ev_is_active(&ev_reconnect_datasvr_timer))
	{
		ev_timer_again(zjh.loop, &ev_reconnect_datasvr_timer);
	}
	log.info("%s to datasvr index:%d!\n", __FUNCTION__, index);
}

void Game::reconnect_datasvr_cb( struct ev_loop *loop, struct ev_timer *w, int revents )
{
 	Game *p_game = (Game*)w->data;
	for (int i = 0; i < DATA_CLINET_MAX; i++)	
		if (p_game->link_data_client[i] == NULL)
 			p_game->reconnect_datasvr(i);
	
	for (int i = 0; i < DATA_CLINET_MAX; i++)
		if (p_game->link_data_client[i] == NULL)
			return;

	ev_timer_stop(zjh.loop, &p_game->ev_reconnect_datasvr_timer );
}

int Game::reconnect_datasvr(int index)
{
	std::string data_svr = "data-svr";
	if (index != 0)
		data_svr += to_char(index);

	log.info("reconnect_datasvr ip[%s] port[%d].\n",
				zjh.conf[data_svr]["host"].asString().c_str(),
				zjh.conf[data_svr]["port"].asInt() );

	struct sockaddr_in addr;
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd < 0) 
	{
		log.error("File[%s] Line[%d]: socket failed: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(zjh.conf[data_svr]["port"].asInt());
	addr.sin_addr.s_addr = inet_addr(zjh.conf[data_svr]["host"].asString().c_str());
	if (addr.sin_addr.s_addr == INADDR_NONE) 
	{
		log.error("game::reconnect_datasvr Incorrect ip address!");
		close(fd);
		fd = -1;
		return -1;
	}
	if( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(fd);
		log.error("reconnect datasvr error: %s(errno: %d)\n", strerror(errno), errno);
		return -1;
	}
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	link_data_client[index] = new (std::nothrow) Client( fd, false, 1);
	if( link_data_client[index])
	{
		log.info("reconnect datasvr link fd[%d].\n", fd );
		register_server(index);
	}
	return 0;
}

void Game::send_to_datasvr(const std::string &res, Client* client)
{	
	if (client != NULL)
	{
		client->send_as_json(res);
	}
	else if( NULL != link_data_client[link_data_client_index])
	{
		link_data_client[link_data_client_index]->send_as_json(res);
	}
	else
	{		
		int org = link_data_client_index;
        ++link_data_client_index;
		link_data_client_index %= DATA_CLINET_MAX;
		if (NULL != link_data_client[link_data_client_index])
		{				
			log.info("%s:%d cur:%d %d to %d\n", __FUNCTION__, __LINE__, org, org, link_data_client_index);

			Jpacket packet;
			packet.val["cmd"] = SERVER_REGISTER_REQ;
			packet.val["vid"] = server_zid;
			packet.val["host"] = zjh.conf["game"]["host"].asString();
			packet.val["port"] = zjh.conf["game"]["port"].asInt();
			packet.val["index"] = link_data_client_index;
			packet.val["new_index"] = link_data_client_index;
			packet.end();
			link_data_client[link_data_client_index]->send_as_json(packet.tostring());			
			link_data_client[link_data_client_index]->send_as_json(res);			
		}
		else
		{
			log.error("link_data client is NULL send to datasvr failed \n");
		}
	}
}

void Game::accept_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	if (EV_ERROR & revents) {
		log.error("got invalid event\n");
		return;
	}
	
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int fd = accept(w->fd, (struct sockaddr *) &client_addr, &client_len);
	if (fd < 0) {
		log.error("accept error[%s]\n", strerror(errno));
		return;
	}

	Game *game = (Game*) (w->data);

	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

	Client *client = new (std::nothrow) Client(fd, true, 0);
	if (client) 
	{		
		client->string_ip = inet_ntoa( client_addr.sin_addr );
		game->fd_client_map[fd] = client;
		
		log.info("%s Client connect ip:%s port:%d fd:%d\n", __FUNCTION__, client->string_ip.c_str(), client_addr.sin_port, fd);
	} 
	else
	{
		close(fd);
	}
}

void Game::del_client(Client *client, bool del_oldclient)
{
	if (client == NULL)
	{
		log.error("%s client NULL .\n", __FUNCTION__);
		return;
	}
	for (int i = 0; i < DATA_CLINET_MAX; i++)
	{
		if (client == link_data_client[i])
		{		
			link_data_client[i] = NULL;
			int standby_data_client = (i + 1) % DATA_CLINET_MAX;
			if (NULL != link_data_client[standby_data_client])
			{
				link_data_client_index = standby_data_client;
				log.info("%s:%d register_server cur:%d %d to %d\n", __FUNCTION__, __LINE__, link_data_client_index, i, link_data_client_index);				
				register_server(link_data_client_index);
			}
			if (!client->_write_q.empty())
			{				
				if (link_data_client[standby_data_client] != NULL)
				{
					link_data_client[standby_data_client]->_write_q.splice(
						link_data_client[standby_data_client]->_write_q.end(),
						client->_write_q);
				}				
			}			
			delete client;			
			start_reconnect(i);					
			return;
		}
	}
	if (client == link_video_client)
	{
		delete link_video_client;
		link_video_client = NULL;
		start_videoreconnect();
		return; 
	}
	if (del_oldclient)
	{
		log.info("%s delete old client fd:%d uid:%d 0x%x\n", __FUNCTION__, client->fd, client->uid, client);
		fd_client_map.erase(client->fd);
		if (client->uid != -1 )
			uid_client_map.erase(client->uid);
		delete client;
		client = NULL;
		return;
	}

	if (fd_client_map.find(client->fd) == fd_client_map.end())
	{
		log.error("%s del client free fd[%d].\n", __FUNCTION__, client->fd);
		return;
	}	
	fd_client_map.erase(client->fd);	

	if (client->uid != -1)
	{
		if (uid_client_map.find(client->uid) != uid_client_map.end())
		{
			log.info("%s uid:%d erase uid_client_map\n", __FUNCTION__, client->uid);
			uid_client_map.erase(client->uid);
		}
	}
	
	if (client->player)
	{
		Player *player = client->player;		
		player->start_offline_timer();
		if (client->player->status !=  PLAYING && (player->seatid < 0 || player->seatid >= GAME_PLAYER))
		{  	
			del_player(player);
		}
		else
		{   
			player->client = NULL;	
			game_user_offline(player);
		}
	}
	else
	{
		int uid = client->uid;
		if (all_tables.find(client->tid_str) != all_tables.end())
		{
			Table* table = all_tables[client->tid_str];
			if (table != NULL && uid != -1)
			{
				if (table->m_mapTable_Player.find(uid) != table->m_mapTable_Player.end())
				{
					Player* player = table->m_mapTable_Player[uid];
					if (player != NULL)
					{
						player->start_offline_timer();
						if (player->status != PLAYING && (player->seatid < 0 || player->seatid >= GAME_PLAYER))
						{
							del_player(player);
						}
						else
						{
							player->client = NULL;
							game_user_offline(player);
							log.info("%s player is null offline tid:%s uid:%d \n", __FUNCTION__, client->tid_str.c_str(), uid);
						}						
					}
				}
			}
		}
		log.info("%s player is null end tid:%s uid:%d\n", __FUNCTION__, client->tid_str.c_str(), client->uid);
	}

	if (client)
	{
		log.info("%s uid:%d del client fd:%d 0x%x\n", __FUNCTION__, client->uid, client->fd, client);
		delete client;
		client = NULL;	
	}
}

int Game::dispatch_client(Client *client)
{
	//用protobuf 解包
	int cmd = client->m_Buffer->proto_head.cmdID;

	if (cmd == CLIENT_DZ_LOGIN_REQ)
	{		
		return dz_request_login(client);
	}
	else if (cmd == CLINET_HEART_BEAT_REQ)
	{		
		proto::login::AckHeatBeat stAck;
		stAck.set_state(0);		
		client->unicast(SERVER_HEART_BEAT_RESP, stAck);
		client->Heart_Beat();
		if (client->player != NULL)
		{
			if (all_tables.find(client->tid_str) != all_tables.end())
			{
				all_tables[client->tid_str]->Online_Beat(client->player);
			}
		}
		return 0;
	}
	else
	{
		for (int i = 0; i < DATA_CLINET_MAX; i++)
		{
			if (link_data_client[i] && client->fd == link_data_client[i]->fd)
			{
				log.error("%s error cmd:%d link_data_clinet is null, client->fd == link_data_client->fd\n", __FUNCTION__, cmd);
				return 0;
			}
		}
	}
   
	Player *player = client->player;
	if (NULL == player)
	{		
		return -1;
	}
	if (all_tables.find(player->tid_str) == all_tables.end())
	{
		log.error("%s tid:%s uid:%d is not in table\n", __FUNCTION__, player->tid_str.c_str(), player->uid);
		return -1;
	}
	//log.info("%s cmd:%d player tid:%s uid:%d\n", __FUNCTION__, cmd, player->tid_str.c_str(), player->uid);
	switch (cmd)
   	{
		case CLIENT_READY_REQ:
			all_tables[player->tid_str]->handler_ready(player);
			break;
		case CLIENT_INFO_REQ:
			all_tables[player->tid_str]->handler_info(player);
			break;
		case CLIENT_LOGOUT_REQ:			
			del_player(player);
			break;
		case CLIENT_CHAT_REQ:
			all_tables[player->tid_str]->handler_chat(client);
			break;
		case CLIENT_EMOTION_REQ:
			all_tables[player->tid_str]->handler_interaction_emotion(client, player);
			break;		
		case CLIENT_TABLE_INFO_REQ:
			all_tables[player->tid_str]->handler_table_info(player);
			break;	
		case CLIENT_UPTABLE_APPLY_REQ:
			all_tables[player->tid_str]->handler_apply_uptable(player);
			break;		
		case CLIENT_DOWNTABLE_REQ:
			all_tables[player->tid_str]->handler_downtable(player);
			break;
		case CLIENT_DAEMONIZE_REQ:
			all_tables[player->tid_str]->handler_daemonize( player );
			break;
		case CLIENT_PLAYER_GPS:
			all_tables[player->tid_str]->handler_player_GPS(client, player);
			break;
		case CLIENT_MJ_OUT_CARD:
			all_tables[player->tid_str]->recv_client_outcard(client, player);
			break;
		case CLIENT_MJ_OPERATE_CARD:
			all_tables[player->tid_str]->recv_client_opeatereuslt(client, player);
			break;
		case CLIENT_MJ_USER_ACTION:
			all_tables[player->tid_str]->recv_client_action(client, player);
			break;		
		case CLIENT_MJ_HOME_RETURN:
			{
				log.info("Recv_Home_Return req tid:%s uid:%d\n", player->tid_str.c_str(), player->uid);
				all_tables[player->tid_str]->handler_table_info(player);
				all_tables[player->tid_str]->Send_Game_Scene(player);
			}break;
		case CLIENT_DISBAND_ROOM:
			all_tables[player->tid_str]->RecvDisbandTableApply(player);
			break;
		case CLIENT_DISBAND_CHOOSE:
			{
				proto::game::ReqChooseDisband stChoose;
				stChoose.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
				int choosestype = stChoose.choosestate();
				all_tables[player->tid_str]->RecvDisbandTableChoose(player->seatid, choosestype);
			}
			break;
		case CLIENT_HOME_OUT:
			all_tables[player->tid_str]->User_Home_Out(player);
			break;	
		default:
			return all_tables[player->tid_str]->dispatch_client(cmd, client, player);			
	}	
	return 0;
}


int Game::dispatch_server(Client *client)
{	
	int cmd = client->packet.sefe_check();
	if (cmd < 0) {
		log.error("the cmd format is error.\n");
		return -1;
	}

	switch (cmd)
	{	
		case SERVER_DISBAND_ROOM_RESP:
		{
			std::string roomid = client->packet.val["roomid"].asString();
			if (all_tables.find(roomid) != all_tables.end())
			{
				all_tables[roomid]->Disband_table_resp(client->packet);
			}
			log.info("%s roomid:%s disband resp\n", __FUNCTION__, roomid.c_str());
		}break;
		case SERVER_ROOM_CONFIG_RES:
		{
			handler_room_conf_res(client->packet.tojson());
		}break;
		case SERVER_LOGOUT_ROOM_RESP:
		{
			logout_room_res(client->packet.tojson());
		}break;
		case SYS_GET_USER_NUM:	
		{
			sys_get_user_num(client);
		}break;
		case SERVER_DZ_LOGIN_SUCC_RES:
		{
			dz_login_succ_back(client->packet.val);
		}break;
		case SERVER_DZ_LOGIN_ERR_RES:
		{
			int val = client->packet.val["uid"].asInt();
			int code = client->packet.val["code"].asInt();
			client->packet.end();
			dz_login_error_back(val, code);
		}break;
		case SERVER_DZ_GAME_START_RES:
		{
			game_start_res(client->packet);
		}break;
		case SERVER_DZ_UPDATE_USER_INFO:
		case SERVER_DZ_UPDATE_USER_INFO_LITTLE:
		{
			std::string roomid;
			if (!client->packet.val["roomid"].isNull() && client->packet.val["roomid"].isString())
			{
				roomid = client->packet.val["roomid"].asString();
				if (all_tables.find(roomid) != all_tables.end())
				{
					all_tables[roomid]->ForceDisbandInfo(client->packet.val);
				}
			}
			log.info("%s cmd:%d roomid:%s update info resp .\n", __FUNCTION__, cmd, roomid.c_str());
		}break;
		case SERVER_DELETE_ROOM_REQ:
		{
			std::string roomid;
			if (!client->packet.val["roomid"].isNull() && client->packet.val["roomid"].isString())
			{
				roomid = client->packet.val["roomid"].asString();
				DeleteRoomReq(roomid);
			}
		}break;
		case SERVER_VIDEO_DATA_ERR_RESP:
		case SERVER_ALL_PLAYER_DOWN:
		case SERVER_DELETE_ROOM_RESP:
		case SERVER_FORCE_LOGOUT_ROOM_RESP:
		case SERVER_USER_STATUS_NOTIFY_RESP:
		{
			std::string roomid;
			if (!client->packet.val["roomid"].isNull())
			{
				if (client->packet.val["roomid"].isString()) roomid = client->packet.val["roomid"].asString();
				else if (client->packet.val["roomid"].isInt()) roomid = to_char(client->packet.val["roomid"].asInt());
			}
			log.info("%s roomid:%s cmd:%d resp .\n", __FUNCTION__, roomid.c_str(), cmd);
		}break;
		case SERVER_DISBAND_ROOM_NOTIFY:
		{
			std::string roomid;
			if (!client->packet.val["roomid"].isNull() && client->packet.val["roomid"].isString())
			{
				roomid = client->packet.val["roomid"].asString();
				if (all_tables.find(roomid) != all_tables.end())
				{
					all_tables[roomid]->ForceDisbandTable(client->packet.val);
				}
			}
			log.info("%s roomid:%s force disband room .\n", __FUNCTION__, roomid.c_str());
		}break;
		case SERVER_DZ_STRIP:
		{
			strip_user(client->packet.val);
		}break;
	case SERVER_DZ_SWITCH_PLATFORM:
		switch_platform(client);
		break;
		default:
			log.error("%s invalid command[%d]\n", __FUNCTION__, cmd);
			return 0;
	}

	return 0;
}

int Game::handler_login_table(int uid, Client* client, Json::Value &val)
{
	if (client == NULL)
	{
		log.error("%s client is null \n", __FUNCTION__);
		return -1;
	}
	if (client->position == POSITION_TABLE) 
	{
		log.error("%s uid:%d have been in table\n", __FUNCTION__, client->uid);
		return -1;
	}
	Player* player = client->player;
	if (player == NULL)
	{
		log.error("%s player is null \n", __FUNCTION__);
		return -1;
	}
	std::string roomid = client->tid_str;
	if(!roomid.empty())
	{
		if(all_tables.find(roomid) == all_tables.end())
		{			
			Table* table = Table::CreateTable(zjh.conf["tables"], val);
			table->init(zjh.conf["tables"], val, player);
			all_tables[roomid] = table;
		}
		client->set_positon(POSITION_TABLE);
		all_tables[roomid]->handler_login(player);

		log.info("%s tid:%s uid:%d succ\n", __FUNCTION__, roomid.c_str(), player->uid);

		/*同步平台uinfo信息*/
		SyncUserInfo(roomid, val);

		return 0;
	}

	log.error("%s uid:%d roomid is NULL\n", __FUNCTION__, player->uid);
	return -1;
}

int Game::SyncUserInfo(std::string roomid, Json::Value &val)
{
	if ((!val["dataMap"].isNull()) && (!val["dataMap"]["uinfo"].isNull()) && (val["dataMap"]["uinfo"].isArray()))
	{
		for (size_t i = 0; i < val["dataMap"]["uinfo"].size(); ++i)
		{
			int uid = val["dataMap"]["uinfo"][i]["uid"].asInt();
			Table* t_table = all_tables[roomid];
			if (t_table)
			{
				if (t_table->m_mapTable_Player.find(uid) == t_table->m_mapTable_Player.end()
					&& uid_client_map.find(uid) == uid_client_map.end())  //防止此玩家正在登陆时，同步此玩家的uinfo，会出现一个房间两个player，删除player出现崩溃
				{
					Player *t_player = new (std::nothrow) Player();
					if (t_player)
					{
						log.info("%s roomid:%s uid:%d new player .\n", __FUNCTION__, roomid.c_str(), uid);

						Json::Value t_val = val["dataMap"]["uinfo"][i];
						t_player->init(t_val);
						t_table->handler_login(t_player);
						if (t_player->seatid >= 0 && t_player->seatid < t_table->m_GAME_PLAYER)
						{
							t_table->m_PreOfflineState[t_player->seatid] = t_table->m_IsOffline[t_player->seatid];
							t_table->m_IsOffline[t_player->seatid] = true;
							t_table->UserOnlineState_bc(t_player->seatid);
							t_table->UserStatusNotify(t_player->seatid);
						}

						log.info("%s roomid:%s uid:%d seatid:%d success .\n", __FUNCTION__, roomid.c_str(), t_player->uid, t_player->seatid);
					}
				}
			}	
		}
	}

	return 0;
}

int Game::del_player(Player *player)
{   
	log.info("Game::del_player\n");
	if( NULL == player )
	{
		log.info("player is NULL ,return -1...\n");
		return -1;
	}
	int ret = 0;
	int uid = player->uid;
	if (all_tables.find(player->tid_str) != all_tables.end()) 
	{
		ret = all_tables[player->tid_str]->handler_logout(player);
		if (ret < 0) {
			log.error("%s table handler logout\n", __FUNCTION__);
			return -1;
		}
		ret = all_tables[player->tid_str]->del_player(player);
		if (ret < 0) {
			log.error("%s table del player\n", __FUNCTION__);			
		}
		if (1 == ret)
		{   
			//all_tables[player->tid]->send_talbe_no_user();
			std::map<std::string, Table*>::iterator iter2 = all_tables.find(player->tid_str);
			if( iter2 != all_tables.end() )
			{				
				Table *table = iter2->second;
				all_tables.erase( iter2 );
				if (table)
				{
					table->DeleteDataFromRedis(player->tid_str);
					delete table;
					table = NULL;
				}				
				log.info("%s delete table! tid:%s left tables:%d \n", __FUNCTION__, player->tid_str.c_str(), all_tables.size());
				Jpacket packet;
				packet.val["cmd"] = SERVER_ALL_PLAYER_DOWN;
				packet.val["vid"] = server_vid;
				packet.val["roomid"] = player->tid_str;
				packet.end();
				send_to_datasvr( packet.tostring() );

			}
		}
	}

	if (player)
	{

		// if (player->client != NULL)
		// {
		// 	std::map<std::string, std::vector<Client*> >::iterator it = tid_client_map.find(player->tid_str);
		// 	if (it != tid_client_map.end()) {
		// 		for (std::vector<Client*>::iterator iter = it->second.begin(); iter != it->second.end();)
		// 		{
		// 			if ((*iter) == player->client)
		// 			{    
		// 				iter = it->second.erase(iter);
		// 				break;
		// 			}
		// 			else
		// 			{
		// 				iter++;
		// 			}
		// 		}
		// 	}
		// }
		log.info("%s delete player! tid:%s uid:%d player:%p\n", __FUNCTION__, player->tid_str.c_str(), uid, player);
		delete player;
		player = NULL;
	}

	return 0;
}

void Game::game_start_res( Jpacket &packet )
{
	std::string roomid = packet.val["roomid"].asString();
	if( all_tables.find( roomid ) != all_tables.end() )
	{
		all_tables[roomid]->game_start_res( packet );
	}
}

int Game::dz_request_login( Client *client )
{
	proto::login::ReqLogin stLogin;
	stLogin.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);

	int uid = stLogin.uid();
	client->uid = uid;
	if (stLogin.has_skey())	
		client->skey = stLogin.skey();
	
	client->tid_str = to_char(stLogin.roomid());
	if (stLogin.has_deviceid())	
		client->deviceid = stLogin.deviceid();
	
	if (uid == 0 || uid == -1)
	{
		log.error("%s tid:%s uid:%d zid:%d skey:%s\n", __FUNCTION__, client->tid_str.c_str(), uid, server_zid, stLogin.skey().c_str());
		return -1;
	}
	log.info("%s tid:%s uid:%d zid:%d skey:%s\n", __FUNCTION__, client->tid_str.c_str(), uid, server_zid, stLogin.skey().c_str());
	
	//断线重连
	if (all_tables.find(client->tid_str) != all_tables.end())
	{
		map<std::string, Table*>::iterator iter = all_tables.find(client->tid_str);
		Table *table = iter->second;
		if (table)
		{
			if (table->m_mapTable_Player.find(client->uid) != table->m_mapTable_Player.end())
			{
				Player* player = table->m_mapTable_Player[client->uid];
				if (player)
				{  
					Client *oldClient = player->client;
					if (oldClient == client)
					{
						log.info("%s old client %x = client %x\n", __FUNCTION__, oldClient, client);						
						client->Heart_Beat();
						return 0;
					}
					else if (oldClient && oldClient->fd != client->fd)
					{
						log.error("%s tid:%s uid:%d pre_destroy old_client:%d\n", __FUNCTION__, client->tid_str.c_str(), client->uid, oldClient->fd);
						SendLogout(oldClient, client->uid, -1);
						oldClient->player = NULL;
						Client::pre_destroy(oldClient, true);
					}
					player->set_client(client);
					client->set_positon(POSITION_TABLE);
					uid_client_map[uid] = client;

					if (player->skey != client->skey)
					{						
						return send_request_login(client);
					}				
					return dz_login_reconnect(client, player);
				}     
			}	
		}	
	}

	if( uid_client_map.find( uid ) != uid_client_map.end() )
	{
		if( uid_client_map[uid]->fd != client->fd )
		{
			log.info("%s tid:%s uid:%d client player delete uid_client_map\n", __FUNCTION__, client->tid_str.c_str(), client->uid);
			del_client( uid_client_map[ uid ] );			
		}
	}
	
	client->player = NULL;
	uid_client_map[uid] = client;

	return send_request_login(client);
}
int Game::send_request_login(Client* client)
{
	int uid = client->uid;

	Jpacket packet;

	packet.val["cmd"] = CLIENT_DZ_LOGIN_REQ;
	packet.val["uid"] = uid;
	packet.val["skey"] = client->skey;
	packet.val["deviceId"] = client->deviceid;
	packet.val["vid"] = server_vid;
	packet.val["roomId"] = client->tid_str;
	if (all_tables.find(client->tid_str) == all_tables.end())
	{
		packet.val["isGetRoomInfo"] = 1;
	}
	packet.end();
	send_to_datasvr(packet.tostring());

	return 0;
}
int Game::dz_login_reconnect(Client* client, Player* player)
{
	int uid = client->uid;	
	all_tables[client->tid_str]->Reconnect(player);//断线重连，更新桌子保存player指针	  
	all_tables[client->tid_str]->handler_login_succ_uc(player);
	if (player->seatid < 0 || player->seatid >= GAME_PLAYER)
	{
		all_tables[client->tid_str]->handler_apply_uptable(player);
	}
	all_tables[client->tid_str]->handler_table_info(player);
	all_tables[client->tid_str]->Send_Game_Scene(player);
	
	player->stop_offline_timer();

	client->Set_Beat_Time(NODATA_TIMEOUT);

	if (uid_client_map.find(uid) != uid_client_map.end())
	{
		log.error("%s uid_client_map has uid:%d already!\n", __FUNCTION__, uid);
	}
	//uid_client_map[uid] = client;
	log.info("%s tid:%s uid:%d success\n", __FUNCTION__, client->tid_str.c_str(), client->uid);
	return 0;
}
int Game::dz_login_succ_back( Json::Value &val )
{
	int uid = val["uid"].asInt();
	int usertype = val["usertype"].asInt();
	log.info("%s login success, uid:%d usertype:%d \n", __FUNCTION__, uid, usertype);

	std::map<int,Client*>::iterator iter = uid_client_map.find(uid);
	if( iter == uid_client_map.end() )
	{
		log.error("%s the player uid:%d socket is not in map.\n", __FUNCTION__, uid );
		return 0;
	}
	Client *client = iter->second;
	//uid_client_map.erase( uid );
	if( all_tables.find( client->tid_str ) != all_tables.end() )
	{
		Table* table = all_tables[client->tid_str];
		if (table->m_mapTable_Player.find(client->uid) != table->m_mapTable_Player.end())
		{			
			if (table->CheckPlayerSkey(val, client))
			{
				Player* player = table->m_mapTable_Player[client->uid];
				if (player != NULL)
				{
					dz_login_reconnect(client, player);
					return 0;
				}
			}
			log.error("%s rebind tid:%s uid:%d get info ok\n", __FUNCTION__, client->tid_str.c_str(), uid);
			del_client(client);
			return 0;
		}		
	}
	if (client->player != NULL)
	{
        log.error("%s player is ready new uid:%d", __FUNCTION__, client->uid);
		return 0;
	}

	//登陆返回配置
	if (all_tables.find(client->tid_str) == all_tables.end())
	{
		int status = val["status"].asInt();
		if (status != 200)
		{
			SendLoginFail(client, client->uid, 100);
			SendLogout(client, client->uid, -1);
			//(*it_c)->player = NULL;			
			//del_client(client);					
			Client::pre_destroy(client);			
			log.info("%s tid:%s uid:%d  login error status:%d  \n", __FUNCTION__, client->tid_str.c_str(), client->uid, status);
			return 0;
		}
		else
		{
			if (val["dataMap"]["innerWayList"].isNull())
			{
				send_request_login(client);
				return 0;
			}
		}
	}
	
	Player *player = new (std::nothrow) Player();
	if (player == NULL) 
	{
		log.error("new player err");
		return 0;
	}
	int ret = player->init(val);
	player->tid_str = client->tid_str;
	player->set_client(client);
	if(0 != ret)
	{
		proto::login::AckLoginError stLoginEr;
		stLoginEr.set_uid(uid);
		stLoginEr.set_code(508);	
		client->unicast(SERVER_LOGIN_ERR_UC, stLoginEr);
		Client::pre_destroy(client);
		return 0;
	}

	log.info("%s new success tid:%s uid:%d  player:%p\n", __FUNCTION__, player->tid_str.c_str(), player->uid, player);	

    client->Set_Beat_Time(NODATA_TIMEOUT);
	
	// if(all_tables.find(client->tid_str) != all_tables.end())
	// {
		handler_login_table(uid, client, val);
	// }
	// else
	// {    
	// 	request_room_conf(client, client->tid_str);
	// }
	return 0;
}

int Game::dz_login_error_back(int uid, int code)
{	
	log.debug("%s uid:%d\n", __FUNCTION__, uid);
	std::map<int,Client*>::iterator iter = uid_client_map.find( uid );
	if( iter == uid_client_map.end() )
	{
		log.error("%s the player uid:%d socket is not in map.\n", __FUNCTION__, uid);
		return 0;
	}
	Client *user_client = iter->second;	
	proto::login::AckLoginError stLoginEr;
	stLoginEr.set_uid(uid);
	stLoginEr.set_code(code);	
	user_client->unicast(SERVER_LOGIN_ERR_UC, stLoginEr);
	//广播退出房间协议,防止客户端断线后又发登录请求，一直死循环
	SendLogout(user_client, uid, code);
	Client::pre_destroy( user_client );
	return 0;
}

int Game::connect_videosvr()
{   
	if (zjh.conf["video-svr"].isNull())
	{
		return -1;
	}
	log.info("videosvr management ip[%s] port[%d].\n",
		zjh.conf["video-svr"]["host"].asString().c_str(),
		zjh.conf["video-svr"]["port"].asInt() );
	struct sockaddr_in addr;
	int fd = socket(PF_INET, SOCK_STREAM, 0);	
	if (fd < 0) 
	{
		log.error("File[%s] Line[%d]: socket failed: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(zjh.conf["video-svr"]["port"].asInt());
	addr.sin_addr.s_addr = inet_addr(zjh.conf["video-svr"]["host"].asString().c_str());
	if (addr.sin_addr.s_addr == INADDR_NONE) 
	{
		log.error("game::connect_videosvr Incorrect ip address!");
		close(fd);
		fd = -1;
		exit(1);
	}
	if( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(fd);
		log.error("connect video svr error: %s(errno: %d)\n", strerror(errno), errno);
		fd = -1;
		exit(1);
	}
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	link_video_client = new(std::nothrow) Client( fd, false, 1);
	if( NULL != link_video_client )
	{
		log.info("connect video svr fd[%d].\n", fd );
	}
	else
	{
		close(fd);
		log.error("new video svr client error.\n");
		//exit(1);
	}
	return 0;
}

int Game::reconnect_videosvr()
{
	struct sockaddr_in addr;
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd < 0) 
	{
		log.error("File[%s] Line[%d]: socket failed: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(zjh.conf["video-svr"]["port"].asInt());
	addr.sin_addr.s_addr = inet_addr(zjh.conf["video-svr"]["host"].asString().c_str());
	if (addr.sin_addr.s_addr == INADDR_NONE) 
	{
		log.error("game::reconnect_videosvr Incorrect ip address!");
		close(fd);
		fd = -1;
		return -1;
	}
	if( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(fd);
		log.error("reconnect videosvr error: %s(errno: %d)\n", strerror(errno), errno);
		return -1;
	}
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	link_video_client = new (std::nothrow) Client( fd, false, 1);
	if( link_video_client )
	{
		log.info("%s videosvr link fd[%d].\n", __FUNCTION__, fd);
		ev_timer_stop(zjh.loop, &ev_reconnect_videosvr_timer );
	}
	log.info("%s reconnect!\n", __FUNCTION__);
	return 0;
}

void Game::send_to_videosvr(const std::string &res)
{
	if( NULL != link_video_client )
	{
		link_video_client->send_as_json(res);
	}
	else
	{
		log.error("link_data client is NULL, send to videosvr failed \n");
	}
}

void Game::start_videoreconnect()
{
	ev_timer_again( zjh.loop, &ev_reconnect_videosvr_timer );
}
void Game::reconnect_videosvr_cb( struct ev_loop *loop, struct ev_timer *w, int revents )
{
	Game *p_game = (Game*)w->data;
	p_game->reconnect_videosvr();
}

void Game::game_user_offline(Player *player)
{
	if (player == NULL)
	{
		return;
	}
	if (all_tables.find(player->tid_str) != all_tables.end()) 
	{		
		//玩家一分钟后掉线		 
		//all_tables[player->tid_str]->User_Offline(player);
		Table* table = all_tables[player->tid_str];
		if (player->seatid >= 0 && player->seatid < table->m_GAME_PLAYER)
		{
			table->m_PreOfflineState[player->seatid] = table->m_IsOffline[player->seatid];
			table->m_IsOffline[player->seatid] = true;
		}
		table->Check_All_Offline();
	}
}

bool Game::DeleteRoomReq(const string& roomid)
{
	bool bIsAll = true;
	map<string, Table *>::iterator iter = all_tables.find(roomid);
	std::vector<Player*> temp_del_vec;
	if (iter != all_tables.end())
	{
		Table *table = iter->second;
		if (table)
		{
			//检测是否所有玩家都已经断线
			map<int, Player *>::iterator iter = table->m_mapTable_Player.begin();
			for (; iter != table->m_mapTable_Player.end(); iter++)
			{
				Player *player = iter->second;
				if (player)
				{
					if (player->client != NULL)
					{
						bIsAll = false;
						break;
					}
					temp_del_vec.push_back(player);
				}
			}
		}
	}
	log.info("%s delete tid:%s user num:%d\n", __FUNCTION__, roomid.c_str(), temp_del_vec.size());

	Jpacket packet;
	packet.val["cmd"] = SERVER_DELETE_ROOM_RESP;
	packet.val["vid"] = zjh.game->server_vid;
	packet.val["roomid"] = roomid;
	if (bIsAll)
	{
		packet.val["type"] = 1;
		if (iter != all_tables.end())
		{
			Table *table = iter->second;
			all_tables.erase(roomid);
			if (table)
			{	
				table->DeleteDataFromRedis(roomid);		
				delete table;
				table = NULL;
			}
			log.info("%s delete table! tid:%s. left tables:%d \n", __FUNCTION__, roomid.c_str(), all_tables.size());
			for( std::vector<Player*>::iterator iter2 = temp_del_vec.begin(); iter2 != temp_del_vec.end(); iter2++ )
			{
				Player *player = *iter2;
				if (player)
				{
					log.info("%s delete player tid:%s. uid:%d %p \n", __FUNCTION__, roomid.c_str(), player->uid, player);
					delete player;
					player = NULL;
				}
			}
		}
	}
	else
	{
		packet.val["type"] = 0;
	}
	packet.end();
	zjh.game->send_to_datasvr(packet.tostring());
	return true;
}

void Game::sys_get_user_num(Client *client)
{
	int iUserCount = fd_client_map.size();
	Jpacket packet;
	packet.val["cmd"] = SYS_GET_USER_NUM;
	packet.val["usernum"] = iUserCount;
	packet.val["gameid"] = zjh.conf["sever-id"]["src_svr_id"].asInt();
	packet.end();

	send_to_datasvr(packet.tostring());    
	log.info("%s num:%d\n", __FUNCTION__, iUserCount);
}

void Game::strip_user(const Json::Value& val)
{
	if ( val["roomid"].isNull() ||  !val["roomid"].isString())
	{
		log.info("%s json roomid is wrong\n", __FUNCTION__ );
		return;
	}
	if ( val["uid"].isNull() || !val["uid"].isInt() )
	{
		log.info("%s json uid is wrong\n",__FUNCTION__);
		return;
	}

	string roomid = val["roomid"].asString();
	int user = val["uid"].asInt();

	log.info("%s roomid:%s uid:%d\n", __FUNCTION__ ,roomid.data() , user);

	if (all_tables.find( roomid ) == all_tables.end())
	{
		log.info("%s no room\n", __FUNCTION__ );
		return;
	}

	Table *ptable = all_tables[roomid];

	if ( ptable->m_mapTable_Player.find(user) == ptable->m_mapTable_Player.end() )
	{
		log.info("%s no user\n", __FUNCTION__);
		return;
	}

	if (ptable->get_room_status() != 0)
	{
		log.info("%s game is running \n", __FUNCTION__);
		return;
	}

	Player *pplayer = ptable->m_mapTable_Player[user];
	if ( pplayer == NULL )
	{
		log.info("%s  pplayer == null\n", __FUNCTION__);
		return;
	}
	 
	proto::login::AckStrsip msg;
	msg.set_uid(pplayer->uid);
	msg.set_chairid(pplayer->seatid);
    if (val["handleType"].isNull() || !val["handleType"].isInt())
    {
        msg.set_handle_type(1);
    }
    else
    {
        msg.set_handle_type(val["handleType"].asInt());
    }
	if ( pplayer->client  )
	{
		pplayer->client->unicast(CLIENT_STRSIP, msg);
	}

	del_player(pplayer);
}

int Game::request_room_conf(Client *client, const std::string& roomid)
{
	// if (tid_client_map.find(roomid) == tid_client_map.end()) {
	// 	Jpacket pac;
	// 	pac.val["cmd"] = SERVER_ROOM_CONFIG_REQ;
	// 	pac.val["roomId"] = roomid;
	// 	pac.end();
	// 	send_to_datasvr(pac.tostring());
	// }
	// std::vector<Client*> temp_client = tid_client_map[roomid];
	// bool bFind = false;
	// for (std::vector<Client*>::iterator it_c = temp_client.begin(); it_c != temp_client.end(); it_c++) 
	// {
	// 	if ((*it_c) == client)
	// 	{
	// 		bFind = true;
	// 	}
	// }
	// if (!bFind)
	// {
	// 	tid_client_map[roomid].push_back(client);
	// }
	return 0;
}

int Game::handler_room_conf_res(Json::Value &val)
{
	// std::string roomid = val["roomId"].asString();
	// int status = val["status"].asInt();
	// std::map<std::string, std::vector<Client*> >::iterator it = tid_client_map.find(roomid);
	// if (it != tid_client_map.end()) {
	// 	//获取配置信息失败
	// 	if (status != 200) {
	// 		log.error("%s room[%s] conf res error\n", __FUNCTION__, roomid.c_str());
	// 		std::vector<Client*> temp_del_client = tid_client_map[roomid];
	// 		for (std::vector<Client*>::iterator it_c = temp_del_client.begin(); it_c != temp_del_client.end(); it_c++) {
	// 			SendLoginFail(*it_c, (*it_c)->uid, 2);
	// 			SendLogout(*it_c, (*it_c)->uid, -1);
	// 			//(*it_c)->player = NULL;
	// 			del_client(*it_c);
	// 		}
	// 		tid_client_map.erase(roomid);
	// 		return -1;
	// 	}
	// 	std::vector<Client*>::iterator it_v = it->second.begin();
	// 	while (it_v != it->second.end()) {
	// 		int ret = handler_login_table( (*it_v)->uid, *it_v, val);
	// 		if( 0 == ret ){
	// 			(*it_v)->Set_Beat_Time(NODATA_TIMEOUT);
	// 		}
	// 		else {
	// 			//(*it_v)->player = NULL;
	// 			del_client(*it_v);
	// 		}
	// 		it_v = it->second.erase(it_v);
	// 	}
	// 	tid_client_map.erase(roomid);
	// 	return 0;
	// }
	// return -1;
	
	return 0;
}
void Game::SendLoginFail(Client *client, int uid, int code)
{
	proto::login::AckLoginError stLoginEr;
	stLoginEr.set_uid(uid);
	stLoginEr.set_code(code);
	client->unicast(SERVER_LOGIN_ERR_UC, stLoginEr);
	Client::pre_destroy( client );
}

void Game::SendLogout(Client *client, int uid, int code)
{
	//广播退出房间协议,防止客户端断线后又发登录请求，一直死循环	
	proto::login::AckLogoutBc stLogout;
	stLogout.set_seatid(-1);
	stLogout.set_uid(uid);
	stLogout.set_type(code);
	log.info("%s uid:%d loging error code:%d logout out room\n", __FUNCTION__, uid, code);	
	client->unicast(SERVER_LOGOUT_SUCC_BC, stLogout);
}



void Game::player_offline_timeout(Player *player)
{
	if (NULL == player)
	{   
		log.error("player_offline_timeout is error");
		return;
	}
	log.info("%s tid:%d player is timeout uid:%d \n", __FUNCTION__, player->tid_str.c_str(), player->uid);
	if (all_tables.find(player->tid_str) != all_tables.end())
	{
		map<std::string, Table*>::iterator iter = all_tables.find(player->tid_str);
		Table *table = iter->second;
		if (table)
		{
			if (table->m_bDeleteTable || (player->seatid < 0 || player->seatid >= GAME_PLAYER))  //如果桌子已经解散
			{
				del_player(player);
			}
		}
	}
	else
	{
		log.info("%s del_player %d\n", __FUNCTION__, player ? player->uid : 0);
	    del_player(player);
	}
		
}

void Game::logout_room_res(const Json::Value& val)
{
	std::string roomid = val["roomid"].asString();
	if (roomid.empty())
	{
		log.error("logout_room_res: roomid is null.\n");
		return;
	}
		
	if (all_tables.find(roomid) != all_tables.end())
	{
		int uid = val["uid"].asInt();
		int status = val["status"].asInt();
		int is_success = val["is_success"].asInt();
		log.info("%s tid:%s uid:%d status:%d is_success:%d\n", __FUNCTION__, roomid.c_str(), uid, status, is_success);

		if (status == 200 && is_success == 1)
		{
			Table* table = all_tables[roomid];
			if (table->m_mapTable_Player.find(uid) != table->m_mapTable_Player.end())
			{
				Player* player = table->m_mapTable_Player[uid];
				del_player(player);
			}			
		}
	}
}


void Game::switch_platform(Client* client)
{
	for (int i = 0; i < DATA_CLINET_MAX; i++)
	{
		if (link_data_client[i] == client)
		{
			int org = link_data_client_index;
			link_data_client_index = i;
			log.info("%s:%d cur:%d %d to %d\n", __FUNCTION__, __LINE__, i, org, link_data_client_index);
			reply_switch_platform(i, org, link_data_client_index);
			break;
		}
	}
}

void last_push_front(std::list<Buffer*>& l)
{
	if (!l.empty())
	{
		Buffer* buffer = l.back();
		if (buffer != NULL)
		{
			l.pop_back();
			l.push_front(buffer);
		}
	}
}

void Game::reply_switch_platform(int index, int org_index, int new_index)
{
	Jpacket packet;
	packet.val["cmd"] = SERVER_DZ_SWITCH_PLATFORM_ACK;
	packet.val["org_index"] = org_index;
	packet.val["new_index"] = new_index;
	packet.val["index"] = index;
	packet.val["type"] = new_index == index ? SWITCH_PLATFORM_MAIN : SWITCH_PLATFORM_STANDBY;
	packet.val["status"] = org_index == new_index ? 0 : SWITCH_PLATFORM_SUC;
	packet.end();

	if (link_data_client[index] != NULL)
	{		
		link_data_client[index]->send_as_json(packet.tostring());
		last_push_front(link_data_client[index]->_write_q);		
	}

	int standby = (index + 1) % DATA_CLINET_MAX;
	packet.val["index"] = standby;
	packet.val["type"] = new_index == standby ? SWITCH_PLATFORM_MAIN : SWITCH_PLATFORM_STANDBY;
	packet.end();
	if (link_data_client[standby] != NULL)
	{
		link_data_client[standby]->send_as_json(packet.tostring());		
	}
}

/**
 * 数据恢复接口
 */

int Game::parse_ids(std::string orstr, std::vector<int> &ids)
{
	char tmp[0x1000];		
	const char* tmp_str = orstr.c_str();
	const char* tmp_str2 = NULL;
	int len = 0;
	while ((tmp_str2 = strchr(tmp_str, ',')) != NULL)
	{
		len = tmp_str2++ - tmp_str;
		strncpy(tmp, tmp_str, len);
		tmp[len] = '\0';
		ids.push_back(atoi(tmp));
		tmp_str = tmp_str2;
	}

	if ((len = strlen(tmp_str)) != 0)
	{
		strncpy(tmp, tmp_str, len);
		tmp[len] = '\0';
		ids.push_back(atoi(tmp));			
	}

	return 0;
}

int Game::save_data_to_redis(std::string tid, std::string data_str)
{
	if (!zjh.conf["tables"]["open_datasave"].asInt())
		return 0;

	std::string loc = zjh.conf["tables"]["location"].asString();	

	int ret = zjh.main_rc[0]->command("hset %s %s %s", loc.c_str(), tid.c_str(), data_str.c_str());
	if (ret < 0) 
	{
		log.error("%s tid:%s error \n", __FUNCTION__, tid.c_str());
		return -1;
	}

	//log.info("%s tid:%s Succ \n", __FUNCTION__, tid.c_str());
	
	return 0;
}

int Game::request_data_from_redis()
{
	if (!zjh.conf["tables"]["open_datasave"].asInt())
		return 0;

	std::string loc = zjh.conf["tables"]["location"].asString();	

	//删除redis数据
	if (zjh.conf["tables"]["open_datasave"].asInt() == 2)
	{
		int ret = zjh.main_rc[0]->command("del %s", loc.c_str());
		if (ret < 0) 
		{
			log.error("%s delete data error.\n", __FUNCTION__);
			return -1;
		}
		log.info("%s delete data success .\n", __FUNCTION__);
		return 0;
	}

	//获取redis数据
	int ret = zjh.main_rc[0]->command("hgetall %s", loc.c_str());
	if (ret < 0) 
	{
		log.error("%s getall error.\n", __FUNCTION__);
		return -1;
	}

	if (zjh.main_rc[0]->is_array_return_ok() < 0) 
	{
		log.info("%s return_ok is null .\n", __FUNCTION__);
		return 0;
	}

	size_t i = 1;
	while (i < zjh.main_rc[0]->reply->elements) 
	{
		std::string ids_str = zjh.main_rc[0]->reply->element[i-1]->str;
		string data_str = zjh.main_rc[0]->reply->element[i]->str;
		/**解析桌子号和分流id**/
		std::vector<int> ids;
		parse_ids(ids_str, ids);

		if (ids.size() > 1)
		{
			log.info("%s ids:[%d, %d] .\n", __FUNCTION__, ids[0], ids[1]);

			std::string tid = to_char(ids[0]);
			if(all_tables.find(tid) == all_tables.end())
			{
				/**构造分流配置json**/
				Json::Value val;
				val["dataMap"]["innerWayList"][to_char(ids[1] / 1000)] = to_char(ids[1]);
				
				Table* table = Table::CreateTable(zjh.conf["tables"], val);
				if (table)
				{
					ret = table->ParseTableInfoFromRedis(data_str);
					if(ret < 0)
					{
						delete table;
						table = NULL;
					}
					else
					{
						all_tables[tid] = table;
					}
				}
			}
		}

		i += 2;
	}
	return 0;
}

int Game::del_data_from_redis(string tid)
{
	if (!zjh.conf["tables"]["open_datasave"].asInt())
		return 0;

	std::string loc = zjh.conf["tables"]["location"].asString();	

	int ret = zjh.main_rc[0]->command("hdel %s %s ", loc.c_str(), tid.c_str());
	if (ret < 0) 
	{
		log.error("%s error.\n", __FUNCTION__);
		return -1;
	}

	//log.info("%s tid:%s Succ \n", __FUNCTION__, tid.c_str());
	
	return 0;
}
