#ifndef _GAME_H_
#define _GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ev++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "proto/loginproto.pb.h"
#include "common/jpacket.h"

class Client;
class Player;
class Table;

#define DATA_CLINET_MAX (2)
#define SWITCH_PLATFORM_SUC (200)
#define SWITCH_PLATFORM_MAIN (1)
#define SWITCH_PLATFORM_STANDBY (2)

class Game
{
public:
	Client *link_data_client[DATA_CLINET_MAX];
	int link_data_client_index;
	Client *link_video_client;
	std::string game_name;
	int game_number;

	std::map<std::string, Table*>       all_tables;		//主键roomid，通过roomid找桌子
	std::map<int, Client*>      uid_client_map;			//登陆列表
	std::map<int, Client*>      fd_client_map;
	std::map<std::string, std::vector<Client*> >		tid_client_map;//桌子未初始化时预登陆客户端

// public:
// 	std::map<std::string, time_t> m_DisbandRooms;

private:
	ev_io _ev_accept;
	int _fd;	

	ev_timer	ev_reconnect_datasvr_timer;
	ev_tstamp   ev_reconnect_datasvr_tstamp;

	ev_timer    ev_delall_client_timer;
	ev_tstamp   ev_delall_client_tstamp;

	int server_vid;
	int server_zid;

public:
	Game();
	virtual ~Game();
	int start();
	int connect_datasvr(int index);
	static void reconnect_datasvr_cb(struct ev_loop *loop, struct ev_timer *w, int revents);

	void register_server(int index);
	void start_reconnect(int index);
	int reconnect_datasvr(int index);
	void send_to_datasvr(const std::string& res, Client* client = NULL);	

	static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	void del_client(Client *client, bool del_oldclient = false);
	//处理客户端和平台发送的消息
	int dispatch_client(Client *client);
	int dispatch_server(Client *client);
	//登陆桌子
	int handler_login_table(int uid, Client* client, Json::Value& val);
	//同步平台uinfo
	int SyncUserInfo(std::string roomid, Json::Value &val);
	//删除玩家
	int del_player(Player *player);
	//开始游戏
	void game_start_res(Jpacket& packet);
	//玩家请求进入对战桌面
	int dz_request_login(Client *client);
	// 对战模式登录成功返回 
	int dz_login_succ_back(Json::Value& val);
	int dz_login_error_back(int uid, int code);
	int dz_login_reconnect(Client* client, Player* player);
	//将玩家请求信息发送data_server
	int send_request_login(Client* client);
	//玩家离线
	void game_user_offline(Player *player);
	//是否可以删除房间查询
	bool DeleteRoomReq(const std::string& roomid);
	//发送登出房间协议给客户端
	void SendLogout(Client *client, int uid, int code);
	//发送登录失败
	void SendLoginFail(Client *client, int uid, int code);
	int request_room_conf(Client *client, const std::string& roomid);
	int handler_room_conf_res(Json::Value &val);
	void player_offline_timeout(Player *player);
	//登出 服务器
	void logout_room_res(const Json::Value& val);
	//获取所有连接客户端的数量
	void sys_get_user_num(Client *client);

	//踢人
	void strip_user(const Json::Value& val);

	/*
	* 平台切服
	*/	
	void switch_platform(Client* client);
	void reply_switch_platform(int index, int org_index, int new_index);
public:
	ev_timer	ev_reconnect_videosvr_timer;
	ev_tstamp   ev_reconnect_videosvr_tstamp;
	int connect_videosvr();
	void start_videoreconnect();
	int reconnect_videosvr();		
	void send_to_videosvr(const std::string &res);
	static void reconnect_videosvr_cb(struct ev_loop *loop, struct ev_timer *w, int revents);

	/*数据恢复接口*/
public:
	int parse_ids(std::string orstr, std::vector<int> &ids);
	int save_data_to_redis(std::string tid, std::string data_str);
	int request_data_from_redis();
	int del_data_from_redis(std::string tid);

private:
	int init_accept();

};

#endif
