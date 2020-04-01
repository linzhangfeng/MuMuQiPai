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
#include <algorithm>
#include <assert.h>
#include <sys/time.h>

#include "game.h"
#include "common/log.h"
#include "common/library.h"
#include "table.h"
#include "common/client.h"
#include "common/player.h"
#include "proto/proto.h"
#include "common/base64.h"
#include "common/md5.h"
#include "TestSvrConfReadJson.h"

extern ZJH zjh;
extern Log log;

static Fantype_to_FanNum g_ft2fn;

Table::Table() : m_CardPool(CARD_POOL_NO_FLOWER, 0)
{
	m_voice_room = 0;
	
	preready_timer_stamp = 5;	
	m_bDelayTime = 0;

	m_Outcard_Timer_stamp = 10;
	if (!zjh.conf["timers"]["outcard_timer_stamp"].isNull())
	{
		m_Outcard_Timer_stamp = zjh.conf["timers"]["outcard_timer_stamp"].asInt();
	}
	m_Operate_Timer_stamp0 = 10;
	if (!zjh.conf["timers"]["operate_timer_stamp"].isNull())
	{
		m_Operate_Timer_stamp0 = zjh.conf["timers"]["operate_timer_stamp"].asInt();
	}
	m_Operate_Timer_stamp1 = m_Operate_Timer_stamp0;
	m_Operate_Timer_stamp2 = m_Operate_Timer_stamp0;
	m_Operate_Timer_stamp3 = m_Operate_Timer_stamp0;
	m_Disbandtable_Timer_stamp= 90;
	if (!zjh.conf["tables"]["disbandstamp"].isNull())
	{
		m_Disbandtable_Timer_stamp = zjh.conf["tables"]["disbandstamp"].asInt();
	}
	m_Disbandtable_Timer_stamp1 = 120;
	m_bDisbandCount = 2;
	m_RandomSeat = false;
	if (!zjh.conf["tables"]["RandomSeat"].isNull())
	{ 
		m_RandomSeat = zjh.conf["tables"]["RandomSeat"].asInt();
	}

	m_Outcard_Timer.data = this;
	ev_timer_init(&m_Outcard_Timer, Table::UserOutCard_Timer_cb, m_Outcard_Timer_stamp+ m_bDelayTime, m_Outcard_Timer_stamp+ m_bDelayTime);

	m_Operate_Timer0.data = this;
	ev_timer_init(&m_Operate_Timer0, Table::UserChooseOperate_Timer_cb0, m_Operate_Timer_stamp0+ m_bDelayTime, m_Operate_Timer_stamp0+ m_bDelayTime);

	m_Operate_Timer1.data = this;
	ev_timer_init(&m_Operate_Timer1, Table::UserChooseOperate_Timer_cb1, m_Operate_Timer_stamp1+ m_bDelayTime, m_Operate_Timer_stamp1+ m_bDelayTime);

	m_Operate_Timer2.data = this;
	ev_timer_init(&m_Operate_Timer2, Table::UserChooseOperate_Timer_cb2, m_Operate_Timer_stamp2+ m_bDelayTime, m_Operate_Timer_stamp2+ m_bDelayTime);

	m_Operate_Timer3.data = this;
	ev_timer_init(&m_Operate_Timer3, Table::UserChooseOperate_Timer_cb3, m_Operate_Timer_stamp3+ m_bDelayTime, m_Operate_Timer_stamp3+ m_bDelayTime);

	m_Disbandtable_Timer0.data = this;
	ev_timer_init(&m_Disbandtable_Timer0, Table::Disband_table_Timer_cb0, m_Disbandtable_Timer_stamp, m_Disbandtable_Timer_stamp);		

	send_videodata_timer.data = this;
	send_videodata_timer_stamp = 0.4;
	ev_timer_init(&send_videodata_timer, Table::Send_Video_Data_Timer_cb, send_videodata_timer_stamp, send_videodata_timer_stamp);

	m_RecvDisbandTable_Timer.data = this;
	m_RecvDisbandTable_Timer_stamp = 5;
	ev_timer_init(&m_RecvDisbandTable_Timer, Table::Recv_DisbandTable_Timer_cb, m_RecvDisbandTable_Timer_stamp, m_RecvDisbandTable_Timer_stamp);

	m_Disband_ResUser = 0;
	m_start_time = 0;
	init_member_variable();
	m_bGameState = GAME_FREE;//初始化状态
	m_bBanker = INVALID_CHAIRID;
	m_bIsDisbandTable = false;
	m_bDeleteTable = false;
	m_nPlayingTimes = 0;
	memset(m_DisbandChoose, 0, sizeof(m_DisbandChoose));
	memset(m_IsOffline, false, sizeof(m_IsOffline));
	memset(m_PreOfflineState, false, sizeof(m_PreOfflineState));

	memset(m_bUserOperate, 0, sizeof(m_bUserOperate));

	m_room_config.Clear();

	m_GAME_PLAYER = GAME_PLAYER;

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_Online_Beat[i] = false;
	}	
	m_Offline_timer.data = this;
	m_Offline_timer_stamp = NODATA_TIMEOUT;
	ev_timer_init(&m_Offline_timer, Table::Check_Offline_Timer_cb, m_Offline_timer_stamp, m_Offline_timer_stamp);
	ev_timer_start(zjh.loop, &m_Offline_timer);

	req_start_timer_stamp = 3.0f;
	req_start_timer.data = this;
	ev_timer_init(&req_start_timer, Table::RequestGameStart_Timer_cb, req_start_timer_stamp, req_start_timer_stamp);

	m_ft2fn = &g_ft2fn;

	m_bGangCalcImme = false;

	m_last_Banker = INVALID_CHAIRID;

	m_mapPlayerGPS.clear();

	m_OfflineNoOp = false;
	if (!zjh.conf["tables"]["offlinenoop"].isNull())
	{
		m_OfflineNoOp = zjh.conf["tables"]["offlinenoop"].asInt();
	}

	memset(m_UserDissState, 0, sizeof(m_UserDissState));
	m_bOutTimer = false;

	m_bIsTestSvr = false;
	if (!zjh.conf["tables"]["istestsvr"].isNull())
	{
		m_bIsTestSvr = zjh.conf["tables"]["istestsvr"].asInt();
	}

	m_Auto_Disband_Timer.data = this;
	m_Auto_Disband_Timer_stamp = 60 * 10 + 2;
	ev_timer_init(&m_Auto_Disband_Timer, Table::Auto_Disband_Timer_cb, m_Auto_Disband_Timer_stamp, m_Auto_Disband_Timer_stamp);
	
	m_AutoDisband = proto::game::OVER_TIME_NO;
	m_bIsAutoDisband = false;

	m_TableBranchId = 0;

	m_nTotalCount = 0;
	m_nCurrentCount = 0;

	memset(m_UserChooseMode, 0, sizeof(m_UserChooseMode));

	m_Offline_DelayOp_Timer.data = this;
	m_Offline_DelayOp_Timer_stamp = 5;
	ev_timer_init(&m_Offline_DelayOp_Timer, Table::Offline_DelayOp_Timer_cb, m_Offline_DelayOp_Timer_stamp, m_Offline_DelayOp_Timer_stamp);

	memset(m_bIsHu, false, sizeof(m_bIsHu));

	m_IsClubRoom = 0;
	m_voice_room = 0;
	m_group_channel_ids.clear();
	
	m_MaxHandCardSize = HAND_CARD_SIZE_14; //默认14张手牌

	m_intervene = false; 
	m_intervene_getcard = 0;
	m_intervene_badcard = 0;
	m_control_score_low = 10;
	m_control_score_high = 50;
	m_goodcard_count = 11;
    for ( int i = 0 ; i < GAME_PLAYER ; ++i  )
    {
        m_intervene_player[i] = -1;
		m_limitHu_player[i] = 0;
    }

	m_FixedBaseScore = 1.0;
}

Table::~Table()
{

	ev_timer_stop(zjh.loop, &m_Outcard_Timer);
	ev_timer_stop(zjh.loop, &m_Operate_Timer0);
	ev_timer_stop(zjh.loop, &m_Operate_Timer1);
	ev_timer_stop(zjh.loop, &m_Operate_Timer2);
	ev_timer_stop(zjh.loop, &m_Operate_Timer3);

	TIMER_STOP(send_videodata_timer);	

	TIMER_STOP(m_Disbandtable_Timer0);	
	TIMER_STOP(m_RecvDisbandTable_Timer);
	TIMER_STOP(m_Offline_timer);
	TIMER_STOP(req_start_timer);

	TIMER_STOP(m_Auto_Disband_Timer);
	TIMER_STOP(m_Offline_DelayOp_Timer);
}

void Table::init_member_variable()
{
	vid = 0;
	zid = 0;

	lose_exp = 0;
	win_exp = 0;

}

void Table::init(const Json::Value &val, const Json::Value &valCfg, Player *player)
{
	tid = 0;
	int my_vid = val["vid"].asInt();
	int my_zid = val["zid"].asInt();

	tid = atoi(player->tid_str.c_str());	
	vid = ( vid > 0 ) ? vid : my_vid;
	zid = ( zid > 0 ) ? zid : my_zid;		
	
	srand(tid);		

	ConfInit_Remote(valCfg);		

	log.info("%s player tid:%s uid:%d\n", __FUNCTION__, player->tid_str.c_str(), player->uid);
	
	if (!val["resuser"].isNull())
	{
		m_Disband_ResUser = val["resuser"].asInt();
	}

	//add 
	if (!zjh.conf["tables"]["controlcard"].isNull()) { 

		m_control_score_low = 40;
		m_intervene_getcard = 70;
		m_intervene_badcard = 70; 
		m_control_score_high = 50;
		m_intervene_setp = 2;
		m_control_card.init(player->tid_str.c_str(),
			zjh.conf["tables"]["controlcard"].asInt(), -1, 3,
			zjh.conf["tables"]["highthresholdscore"].asInt());

		if (m_control_card.m_b_senior_control) {
			m_intervene = true;
			m_intervene_setp = 0;
			m_intervene_badcard = zjh.conf["tables"]["getbadcard"].asInt();
			m_control_score_high = zjh.conf["tables"]["highthresholdscore"].asInt();
			m_intervene_getcard = zjh.conf["tables"]["getgoodcard"].asInt();
			m_control_score_low = zjh.conf["tables"]["lowthresholdscore"].asInt();
			m_goodcard_count = zjh.conf["tables"]["goodcardcount"].asInt();
		}
		if (DataStatic::instance().m_curday == 0) {
			struct timeval _tv;
			gettimeofday(&_tv, NULL);
			struct tm _tm;
			localtime_r(&_tv.tv_sec, &_tm);
			DataStatic::instance().m_curday = _tm.tm_mday;
			log.info("fun[%s] day[%d]\n", __FUNCTION__, DataStatic::instance().m_curday);
		}
		log.info("fun[%s] roommid[%d] contr[%d] contr_step[%d] high-[%d] getca[%d] low[%d] bad[%d] good-count[%d]\n",
			__FUNCTION__, tid, m_intervene,
			m_intervene_setp, m_control_score_high, m_intervene_getcard,
			m_control_score_low, m_intervene_badcard, m_goodcard_count);
	}
}

void Table::Proto_TestSvrConf(proto::game::TestSvrConf& pb_conf)
{
    if (m_bIsTestSvr)
    {
        TestSvrConfReadJson::GetInstance()->ReadJsonFile();
        pb_conf.set_client_anti_cheat(TestSvrConfReadJson::GetInstance()->GetAntiCheat());
    }
    else
    {
        pb_conf.set_client_anti_cheat(true);
    }
}

void Table::ConfInit_Remote(const Json::Value& val)
{
	m_room_config.Clear();

	if (!val["baseGold"].isNull())
	{
		m_BaseScore = val["baseGold"].asInt();
	}
	if (!val["standMoney"].isNull())
	{
		stand_money = val["standMoney"].asInt();
	}
	if (!val["baseGold"].isNull())
	{
		base_money = val["baseGold"].asInt();
	}
	if (!val["standMoney"].isNull())
	{
		stand_money = val["standMoney"].asInt();
	}
	if (!val["roomGolden"].isNull())
	{
		roomGold = val["roomGolden"].asInt();
	}
	if (!val["roomUserId"].isNull())
	{
		m_room_owner_uid = val["roomUserId"].asInt();
	}
	if (!val["roomStatus"].isNull())
	{
		m_room_status = val["roomStatus"].asInt();
	}	
	if (!val["dataMap"].isNull() && !val["dataMap"]["roomCount"].isNull())
	{
		m_nTotalCount = val["dataMap"]["roomCount"].asInt();
		m_nCurrentCount = 1;
	}
	if (!val["dataMap"].isNull())
	{
		if (!val["dataMap"]["isClubRoom"].isNull())
		{
			m_IsClubRoom = val["dataMap"]["isClubRoom"].asInt();
		}
		if (!val["dataMap"]["isVoiceRoom"].isNull())
		{
			m_voice_room = val["dataMap"]["isVoiceRoom"].asInt();
		}
	    if (m_voice_room)
	    {
	        m_public_channel_id = val["dataMap"]["publicChannelId"].asString();
	        for (_uint8 i = 0; i < val["dataMap"]["groupChannelIds"].size(); ++i)
	        {
	            m_group_channel_ids.push_back(val["dataMap"]["groupChannelIds"][i].asString());
	        }
	    }
	}
}

int Table::handler_login(Player *player)
{
	if (player == NULL)
	{
		log.error("%s tid:%d error, player is NULL \n", __FUNCTION__, tid);
		return -1;
	}
	
	if (!HasSeat(player))
	{//不让多余的人登录
		log.error("%s tid:%d uid:%d seat is full force logout!\n", __FUNCTION__, tid, GetPlayerUid(player));
		zjh.game->dz_login_error_back(player->uid,  100);
		force_logout_room_req(player);		
		//logout_room_req(player);		
		zjh.game->del_player(player);
		return -1;
	}
	log.info("%s tid:%d uid:%d client:%d usertype:%d\n", __FUNCTION__, tid, player->uid, player->client, player->usertype);
	bool bFind = false;//是否已经是桌子上的玩家
	std::map<int, Player *>::iterator iter = m_mapTable_Player.find(player->uid);
	
	if (iter != m_mapTable_Player.end())
	{
		log.error("%s tid:%d uid:%d can not sitdown because find in this table\n", __FUNCTION__, tid, player->uid);
		bFind = true;
	}		

	if (!bFind)
	{
		player->tid_str = to_char(tid);
		m_mapTable_Player.insert(pair<int, Player *>(player->uid, player));
		m_bCurPlayers = m_mapTable_Player.size();

		int iSeatCount = SeatPlayerCount();

		handler_login_succ_uc(player);
		handler_login_succ_bc(player);
		if (player->seatid < 0 || player->seatid >= GAME_PLAYER)		
		{
			handler_apply_uptable(player);
		}
		handler_table_info(player);	
		Send_Game_Scene(player);

		if (player->seatid >= 0 && player->seatid < m_GAME_PLAYER)
		{
			UserStatusNotify(player->seatid);
		}

		//改变模式
		if (iSeatCount != SeatPlayerCount())
		{
			memset(m_UserChooseMode, 0, sizeof(m_UserChooseMode));
		}
		return 0;
	}

	return -1;
}

int Table::Sit_Down(Player *player)
{
	if (m_RandomSeat)
	{
		MJ_BUFFER<_uint8, GAME_PLAYER> s;
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			if (m_SeatPlayer[i].state == Seat::E_Seat_Empty)
			{
				s.Add(i);
			}
		}
		if (s.IsNull())
		{
			return -1;
		}

		srand(time(0) + tid);
		_uint8 i = s[rand() % s.Size()];
		m_SeatPlayer[i].action = ACTION_SIDOWN;
		m_SeatPlayer[i].state = Seat::E_Seat_Occupy;
		m_SeatPlayer[i].player = player;
		m_SeatPlayer[i].seatid = i;
		m_SeatPlayer[i].uid = player->uid;
		player->status = SIT_DOWN;

		log.info("%s tid:%d player uid:%d chairid:%d success\n", __FUNCTION__, tid, player->uid, i);
		return i;

	}
	else
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			if (m_SeatPlayer[i].state == Seat::E_Seat_Empty)
			{
				m_SeatPlayer[i].action = ACTION_SIDOWN;
				m_SeatPlayer[i].state = Seat::E_Seat_Occupy;
				m_SeatPlayer[i].player = player;
				m_SeatPlayer[i].seatid = i;
				m_SeatPlayer[i].uid = player->uid;
				player->status = SIT_DOWN;

				log.info("%s tid:%d player uid:%d chairid:%d success\n", __FUNCTION__, tid, player->uid, i);
				return i;
			}
		}
	}
	return -1;
}
bool Table::Re_Sit_Down(Player* player)
{
	if (player == NULL)
		return false;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player == NULL && m_SeatPlayer[i].uid == player->uid)
		{
			m_SeatPlayer[i].action = ACTION_SIDOWN;
			m_SeatPlayer[i].state = Seat::E_Seat_Occupy;
			m_SeatPlayer[i].player = player;
			m_SeatPlayer[i].seatid = i;
			m_SeatPlayer[i].uid = player->uid;
			player->status = SIT_DOWN;
			player->seatid = i;

			log.info("%s tid:%d player uid:%d chairid:%d success\n", __FUNCTION__, tid, player->uid, i);
			return true;
		}
	}
	return false;
}
void Table::Stand_Up(Player *player)
{
	if (player->seatid >= m_GAME_PLAYER || player->seatid <0)
	{
		log.error("%s tid:%d uid:%d stand up but seatid:%d is wrong \n ", __FUNCTION__, tid, player->uid, player->seatid);
		return;
	}
	log.info("%s tid:%d uid:%d chairid:%d success\n", __FUNCTION__, tid, player->uid, player->seatid);
	player->status = STAND_UP;
	m_SeatPlayer[player->seatid].clear();
	m_bBanker = INVALID_CHAIRID;
}
void Table::Player_Leave(Player *player)
{
	if (player->seatid >= m_GAME_PLAYER || player->seatid < 0)
	{
		log.error("%s tid:%d uid:%d leave but seatid:%d is wrong \n ", __FUNCTION__, tid, player->uid, player->seatid);
		return;
	}
	log.info("%s tid:%d uid:%d chairid:%d success\n", __FUNCTION__, tid, player->uid, player->seatid);
	player->status = STAND_UP;
	m_SeatPlayer[player->seatid].leave();
	
}

int Table::del_player(Player *player)
{
	if (player == NULL)
	{
		log.error("%s tid:%d but player is NULL \n", __FUNCTION__, tid);
		return -1;
	}
	log.info("%s tid:%d uid:%d player nums:%d \n", __FUNCTION__, tid, player->uid, m_mapTable_Player.size());
	std::map<int, Player *>::iterator iter = m_mapTable_Player.find(player->uid);
	if (iter == m_mapTable_Player.end())
	{
		log.error("%s tid:%d can not find uid:%d \n", __FUNCTION__, tid, player->uid);
		return -1;
	}
	if (m_bGameState != GAME_FREE)
	{
		if (player->seatid >= 0 && player->seatid < m_GAME_PLAYER)
		{
			m_PreOfflineState[player->seatid] = m_IsOffline[player->seatid];
            m_IsOffline[player->seatid] = true;//记录断线情况
        }	
    }
    else
    {
    	if (player->seatid >= 0 && player->seatid < m_GAME_PLAYER)
    	{
    		m_PreOfflineState[player->seatid] = m_IsOffline[player->seatid];
			m_IsOffline[player->seatid] = false;//记录断线情况
			if (m_bIsDisbandTable)
			{   
				//游戏没开始时玩家掉线或者退出时 默认不解散
				RecvDisbandTableChoose(player->seatid, proto::game::DISBAND_NO);
			}
		}	
	}

	m_mapTable_Player.erase(player->uid);
	m_bCurPlayers = m_mapTable_Player.size();	
	if (m_room_status == 0)
	{
		Stand_Up(player);
	}
	else
	{
		Player_Leave(player);
	}
	
	if (m_bCurPlayers == 0)
	{		
		return 1;
	}

	return 0;	
}

int Table::handler_login_succ_uc(Player *player)
{  
	if (NULL == player)
	{
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return -1;
	}
	
	proto::login::AckLoginSuccessUc stLoginuc;
	stLoginuc.set_seatid(player->seatid);
	stLoginuc.set_uid(player->uid);
	stLoginuc.set_money(player->money);
	if (player->name != "")
	{
		stLoginuc.set_name(player->name.c_str());
	}
	if (player->avatar != "")
	{
		stLoginuc.set_avatar(player->avatar.c_str());
	}
	stLoginuc.set_sex(player->sex);
	stLoginuc.set_coin(player->coin);
	stLoginuc.set_exp(player->exp);
	stLoginuc.set_rmb(player->rmb);
	stLoginuc.set_vlevel(player->vlevel);

	unicast(SERVER_LOGIN_SUCC_UC, player, stLoginuc);

	log.info("%s tid:%d send to client uid:%d \n", __FUNCTION__, tid, player->uid);
	return 0;	
}

// SERVER_LOGIN_SUCC_BC
int Table::handler_login_succ_bc(Player *player)
{
	proto::login::AckLoginSuccessBc stLoginbc;
	stLoginbc.set_seatid(player->seatid);
	stLoginbc.set_uid(player->uid);
	stLoginbc.set_money(player->money);
	if (player->name != "")
	{
		stLoginbc.set_name(player->name.c_str());
	}
	if (player->avatar.size() != 0)
	{
		stLoginbc.set_avatar(player->avatar.c_str());
	}
	
	stLoginbc.set_sex(player->sex);

	broadcast(SERVER_LOGIN_SUCC_BC, stLoginbc);
	return 0;
}

int Table::handler_table_info(Player *player)
{
	if (player == NULL)
	{
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return - 1;
	}
	
	proto::login::AckTableInfo stTableinfo;
	stTableinfo.set_seatid(player->seatid);
	stTableinfo.set_roomgolden(roomGold);

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player)
		{
			proto::login::tagPlayers *paddPlayer = stTableinfo.add_players();
			paddPlayer->set_uid(m_SeatPlayer[i].player->uid);
			paddPlayer->set_seatid(m_SeatPlayer[i].player->seatid);
			_uint8 bReady = (m_SeatPlayer[i].action == ACTION_READY ? 1:0);
			paddPlayer->set_ready(bReady);
			paddPlayer->set_money(m_SeatPlayer[i].player->money);
			if (m_SeatPlayer[i].player->name != "")
			{
				paddPlayer->set_name(m_SeatPlayer[i].player->name.c_str());
			}
			if (m_SeatPlayer[i].player->avatar != "")
			{
				paddPlayer->set_avatar(m_SeatPlayer[i].player->avatar.c_str());
			}
			paddPlayer->set_sex(m_SeatPlayer[i].player->sex);

			std::map<int, tagPlayerGPS>::iterator iter = m_mapPlayerGPS.find(m_SeatPlayer[i].player->uid);
			if (iter != m_mapPlayerGPS.end())
			{
				proto::login::AckGPS* pgps = paddPlayer->mutable_gps();
				pgps->set_uid(m_SeatPlayer[i].player->uid);
				pgps->set_latitude(iter->second.latitude);
				pgps->set_longitude(iter->second.longitude);
				pgps->set_city(iter->second.city);
			}
		}	
	}

	stTableinfo.set_cardpoolnums(m_CardPool.MaxCount());
	if (m_Start_Shai.Num > 0)
	{
		stTableinfo.add_touziinfo(m_Start_Shai.Shai[0]);
		stTableinfo.add_touziinfo(m_Start_Shai.Shai[1]);
	}
	stTableinfo.set_gcrdfrmlstnum(m_CardPool.GetCardCount_FromBack());
	stTableinfo.set_game_player(m_GAME_PLAYER);
 	stTableinfo.set_isclubroom(m_IsClubRoom);  	

	unicast(SERVER_TABLE_INFO_UC, player, stTableinfo);
	
	log.info("%s tid:%d send to client uid:%d m_GAME_PLAYER:%d \n", __FUNCTION__, tid, player->uid, m_GAME_PLAYER);

	return 0;
}

int Table::handler_ready(Player *player)
{  
	log.info("%s \n", __FUNCTION__);
	//clean_table_user();
	return 0;
}


int Table::handler_logout(Player *player)
{   
	if (m_bGameState != GAME_FREE || m_room_status != 0)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			if (m_SeatPlayer[i].player!=NULL && m_SeatPlayer[i].player->uid == player->uid)
			{
				log.info("%s tid:%d error uid:%d m_bGameState:%d m_room_status:%d is in playing\n", __FUNCTION__, tid, player->uid, m_bGameState, m_room_status);				
				if (player->seatid >= 0 && player->seatid < m_GAME_PLAYER)
				{
					m_PreOfflineState[player->seatid] = m_IsOffline[player->seatid];
					m_IsOffline[player->seatid] = true;//记录断线情况
				}	
				return -1;
			}	
		}	
	}

	int iSeatCount = SeatPlayerCount();

	logout_room_req(player);

	//改变模式
	if (iSeatCount != SeatPlayerCount())
	{
		memset(m_UserChooseMode, 0, sizeof(m_UserChooseMode));
	}

	log.info("%s tid:%d uid:%d seatid:%d money:%d\n", __FUNCTION__, tid, player->uid, player->seatid, player->money);

	//handler_downtable( player );
	//广播退出房间协议
	proto::login::AckLogoutBc stLogout;
	stLogout.set_seatid(player->seatid);
	stLogout.set_uid(player->uid);
	stLogout.set_type(player->logout_type);	

	broadcast(SERVER_LOGOUT_SUCC_BC, stLogout);
	return 0;
}

int Table::handler_chat(Client *client)
{	

	proto::login::AckChatBC  chardata;

	if (chardata.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length))
	{
		broadcast(SERVER_CHAT_BC, chardata);
	}
	else
	{
		log.error("%s tid:%d error ", __FUNCTION__, tid);
	}
	return 0;
}

int Table::handler_interaction_emotion(Client *client, Player *player)
{
	if (client == NULL || player == NULL)
	{
		return -1;
	}

	
	proto::login::ReqEmotion stEmotion;
	stEmotion.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
	int target_seatid = stEmotion.target_seatid();
	int target_seat_num = stEmotion.target_seat_num();
	int itype = stEmotion.type();
	log.info("%s tid:%d target uid:%d chairid:%d seatnum:%d type:%d \n", __FUNCTION__, tid, GetPlayerUid(target_seatid), target_seatid, target_seat_num, itype);
	proto::login::AckEmotionBC stEmotionBC;
	stEmotionBC.Clear();
	stEmotionBC.set_seatid(player->seatid);
	stEmotionBC.set_money(player->money);
	stEmotionBC.set_target_seatid(target_seatid);
	stEmotionBC.set_target_seat_num(target_seat_num);
	stEmotionBC.set_type(itype);

	broadcast(SERVER_EMOTION_BC, stEmotionBC);
	return 0;
	
}

void Table::handler_daemonize( Player *player )
{
	
}

int Table::handler_info(Player *player)
{
	UpdateUserInfoToUser(player);
	return 0;
}

void Table::update_user_info_to_datasvr_dz(tagUserScore userscore[GAME_PLAYER], int tally_type, int cmd_type, int alter_type)
{
	double base = 1;
	if (m_FixedBaseScore * m_BaseScore > 0)
		base = m_FixedBaseScore * m_BaseScore;
	std::map<int, int> uid_win_lost;
	for (int i = 0; i < m_GAME_PLAYER; ++i) {
		int uid = userscore[i].uid;
		double temp_win_lost = userscore[i].score;
		temp_win_lost /= base;
		int win_lost = int(temp_win_lost); 
		uid_win_lost[uid] = win_lost;
		DataStatic::instance().add_score(uid, win_lost, i == (m_GAME_PLAYER - 1));
	}
	m_control_card.record_alter_money(uid_win_lost);

	Jpacket packet_data;
	packet_data.val["cmd"] = cmd_type;

	for (int i=0; i<m_GAME_PLAYER; i++)
	{
		packet_data.val["players"][i]["uid"] = userscore[i].uid;
		packet_data.val["players"][i]["alter_money"] = userscore[i].score;
		packet_data.val["players"][i]["al_board"] = 1;
		packet_data.val["players"][i]["basemoney"] = base;
		if( userscore[i].score > 0 ){
			packet_data.val["players"][i]["al_win"] = 1;
			packet_data.val["players"][i]["alter_exp"] = win_exp;
		}else{
			packet_data.val["players"][i]["al_win"] = 0;
			packet_data.val["players"][i]["alter_exp"] = lose_exp;
		}

	}	
	
	packet_data.val["vid"] = vid;
	packet_data.val["roomid"] = to_char(tid);
	
	char strbuf[50] = {0};
	sprintf(strbuf,"%d_%04d",tid, m_nPlayingTimes);
	std::string strTimes = strbuf;
	
	packet_data.val["innings"]= strTimes; 
	packet_data.end();
	
	zjh.game->send_to_datasvr(packet_data.tostring());

	while ((int)m_totalrecordinfo.round_score.size() < m_nPlayingTimes)
	{
		m_totalrecordinfo.round_score.push_back(::RoundScore(0, 0, 0, 0));
	}

	if (m_nPlayingTimes > 0)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			m_totalrecordinfo.round_score[m_nPlayingTimes - 1].score[i] += userscore[i].score;
		}
	}
	
}

void Table::InitData()
{
	m_bGameState = GAME_FREE;
	m_bCurChairID = INVALID_CHAIRID;//当前出牌或摸牌的椅子ID
	m_bMultiOpUserNum = 0;//引起有操作提示的人数	
	
	m_bMultiDoneOpNum = 0;//已经选择操作的玩家数
	m_bMulitFirstOperate = false;//优先级最高的玩家是否执行还是放弃，放弃为false	
	
	m_bCurCard = INVALID_MJ_CARD;	
	m_bMulitHuOpeCount = 0;	
	m_RobWanGangCard = INVALID_MJ_CARD;
	m_RobOperateNum = 0;
	m_bRobHu = false;
	m_RobHuNum = 0;
	m_bRobWanGang = false;
	m_bRobGangHuJudge = false;
	m_WangGangPlayerOpType = TYPE_NULL;
	m_bOutCardChairID = INVALID_CHAIRID;
	m_bCurOutCard = INVALID_MJ_CARD;
	m_bMultiOpHuNum = 0;

	m_videostr.clear();


	memset(m_bMultiUserDoneOp, 0, sizeof(m_bMultiUserDoneOp));//用户选择的操作
	memset(m_bMultiUserDoneOpCard, 0, sizeof(m_bMultiUserDoneOpCard));

	memset(m_HandCard, 0, sizeof(m_HandCard));
	memset(m_stUserOutCard, 0, sizeof(m_stUserOutCard));
	
	memset(m_bIsHu, false, sizeof(m_bIsHu));
	
	memset(m_HuFanNums, 0, sizeof(m_HuFanNums));
	
	memset(m_bGetCardCount, 0, sizeof(m_bGetCardCount));
	memset(m_bAddFanFlag, 0, sizeof(m_bAddFanFlag));
	
	memset(m_bAutoHu, 0, sizeof(m_bAutoHu));
	memset(m_RobHuDoneOp, 0, sizeof(m_RobHuDoneOp));
	memset(m_bIsTrustee, false, sizeof(m_bIsTrustee));

	//初始化杠的椅子id
	
	memset(m_stGang, 0, sizeof(m_stGang));
	memset(m_stHuScore, 0, sizeof(m_stHuScore));
	memset(m_oFan, 0, sizeof(m_oFan));

	reset_disbandcount();

	memset(m_UserOperateID, 0, sizeof(m_UserOperateID));

	memset(m_voice_status,-1,sizeof(m_voice_status));

	m_vecOpCards.clear();

	InitInterveneInfo();
}

void Table::Send_Game_Scene(Player *player)
{
	if (player == NULL)
	{
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return;
	}
	int ChairID = player->seatid;	    
	if (ChairID < 0 || ChairID >= m_GAME_PLAYER)
	{
		Send_lookon_GameScene(player);
		return;
	}

	proto::game::AckGameScene stGameScene;	
	
	Proto_GameScene(ChairID, player, stGameScene);	

	unicast(SERVER_GAME_SCENE, player, stGameScene);
	
	log.info("%s tid:%d over to uid:%d chairid:%d \n", __FUNCTION__, tid, player->uid, player->seatid);	
}


int Table::game_start_req()
{   	
	m_room_status = 1;

	char strbuf[50] = {0};
	sprintf(strbuf,"%d_%04d",tid, m_nPlayingTimes);
	std::string strTimes = strbuf;
	
	char strroomid[64] = {0};
	sprintf(strroomid, "%d", tid);
	
	Jpacket req_packet;
	req_packet.val["cmd"] = SERVER_DZ_GAME_START_REQ; 
	req_packet.val["vid"] = zid;
	req_packet.val["roomid"] = strroomid;
	req_packet.val["innings"] = strTimes;
	req_packet.end();
	
	zjh.game->send_to_datasvr(req_packet.tostring());
	log.info( "%s tid:%d innings:%s .\n", __FUNCTION__, tid, strTimes.c_str());
	return 0;
}
int Table::game_start_res( Jpacket &packet )
{
	TIMER_STOP(req_start_timer);

	int can_start = packet.val["can_start"].asInt();
	InitPlayAccont();

	m_account_info.SetRound(m_nPlayingTimes);
	RedisUpdateScoreToLocal();

	char strbuf[50] = {0};
	sprintf(strbuf,"%d_%04d",tid, m_nPlayingTimes);
	std::string strTimes = strbuf;
	if( 0 == can_start )
	{
		int code = packet.val["code"].asInt();
		proto::game::AckGameStartFail stGameStartFail;
		stGameStartFail.set_vid(vid);
		char strroomid[128] = {0};
		sprintf(strroomid, "%d", tid);
		stGameStartFail.set_roomid(strroomid);
		stGameStartFail.set_can_start(0);
		stGameStartFail.set_code(code);
		int ntime = (int)time( NULL );
		stGameStartFail.set_ts(ntime);

		broadcast(SERVER_DZ_GAME_START_RES, stGameStartFail);

		log.error( "%s fail tid:%d can_start[%d] code[%d] nTimes:%s.\n", __FUNCTION__, tid, can_start, code ,strTimes.c_str());
		m_bDeleteTable = true;
		return -1;
	}

    if ((!packet.val["isVoiceRoom"].isNull()) && packet.val["isVoiceRoom"].isInt())
    {
        m_voice_room = packet.val["isVoiceRoom"].asInt();
    }
    if (m_voice_room)
    {
        m_public_channel_id = packet.val["publicChannelId"].asString();
        m_group_channel_ids.clear();
        for (_uint8 i = 0; i < packet.val["groupChannelIds"].size(); ++i)
        {
            m_group_channel_ids.push_back(packet.val["groupChannelIds"][i].asString());
        }
    }

	//ev_timer_again(zjh.loop, &ready_timer);
	int nReadNum = 0;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].action == ACTION_READY)
		{
			nReadNum++;
		}
	}
	//log.info( "%s succ tid:%d can_start[%d] ready:%d.\n", __FUNCTION__, tid, can_start, nReadNum );
	if (nReadNum == m_GAME_PLAYER && m_bGameState == GAME_FREE)
	{
		if ((!packet.val["count"].isNull()) && packet.val["count"].isNumeric())
			m_nTotalCount = packet.val["count"].asInt();
		if ((!packet.val["currentCount"].isNull()) && packet.val["currentCount"].isNumeric())
			m_nCurrentCount = packet.val["currentCount"].asInt();

		log.info("%s tid:%d m_nTotalCount:%d m_nCurrentCount:%d .\n", __FUNCTION__, tid, m_nTotalCount, m_nCurrentCount);

		if (m_nPlayingTimes == 1)
        {
            RecordTakeInScore();
        }

		GameStart();
	}
	return 0;
}
int Table::handler_apply_uptable(Player *player)
{   
	if (NULL == player)
	{
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return  -1;
	}
	handler_uptable(player);
	return 0;
}

void Table::uptable_error_uc(Player *player, int code)
{  
	if (NULL == player)
	{   
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return ;
	}
	proto::login::AckUpTableErrorUc stUpTable;
	stUpTable.set_uid(player->uid);
	stUpTable.set_money(player->money);
	stUpTable.set_code(code);

	unicast(SERVER_UPTABLE_ERR_UC, player, stUpTable);
}

int Table::handler_uptable(Player *player)
{  
	if (NULL == player)
	{
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return  -1;
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (NULL != m_SeatPlayer[i].player)
		{
			if (m_SeatPlayer[i].player->uid == player->uid)
			{
				log.error("%s error tid:%d uid:%d already in seat\n", __FUNCTION__, tid, player->uid);
				uptable_error_uc(player, 102);
				return -1;
			}		
		}    
	}
	if (player->money < stand_money) {
		log.error("%s tid:%d error uid:%d money:%d too less stand_money:%d.\n", __FUNCTION__, tid, player->uid, player->money, stand_money);
		uptable_error_uc(player, 104);
		return -1;
	}

	_uint8 iSeatCount = SeatPlayerCount();
	int seatid = -1 ;
	if (iSeatCount < m_GAME_PLAYER) {
		seatid = Sit_Down(player);
	}
	else if (Re_Sit_Down(player))
	{
		seatid = player->seatid;
	}
	else
	{   
		log.error("%s tid:%d error uid:%d seatnum:%d\n", __FUNCTION__, tid, player->uid, iSeatCount);
		uptable_error_uc(player, 105);
		return -1;
	}
	
	player->seatid = seatid;
	proto::login::AckUpTableSuccess stUpTable;
	stUpTable.set_uid(player->uid);
	stUpTable.set_seatid(player->seatid);
	if (player->name.size() !=0)
	{
		stUpTable.set_name(player->name.c_str());
	}
	
	stUpTable.set_sex(player->sex);
	if (player->avatar.size() != 0)
	{
		stUpTable.set_avatar(player->avatar.c_str());
	}
	
	stUpTable.set_money(player->money);
	stUpTable.set_coin(player->coin);
	stUpTable.set_exp(player->exp);
	stUpTable.set_pcount(player->pcount);
	stUpTable.set_vlevel(player->vlevel);
	
	broadcast(SERVER_UPTABLE_SUCC_BC, stUpTable, player);

	log.info("%s tid:%d succ uid:%d chairid:%d money:%d\n", __FUNCTION__, tid, player->uid, player->seatid, player->money);
	return 0;
}
int Table::handler_downtable( Player *player )
{  
	if (player == NULL)
	{
		log.error("%s tid:%d error player is null", __FUNCTION__, tid);
		return -1;
	}
	bool bFindUser = false;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player != NULL)
		{
			if (m_SeatPlayer[i].player->uid == player->uid)
			{
				bFindUser = true;
			}           
		}      
	}
	if (!bFindUser)
	{
		log.error("%s tid:%d error uid:%d not in seat.\n", __FUNCTION__, tid, player->uid);
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			if (m_SeatPlayer[i].player != NULL)
			{
				log.info("%s tid:%d uid:%d, seatid:%d, roomid:%d\n", __FUNCTION__, tid, m_SeatPlayer[i].player ? m_SeatPlayer[i].player->uid : 0, m_SeatPlayer[i].seatid, tid);
			}
		}
		return -1;
	}
	if (m_bGameState != GAME_FREE || m_room_status != 0)
	{
         //player->down_tag = (player->down_tag == 0) ? 1:0;
		proto::login::AckAheadDownTable stHeadDownTable;
		stHeadDownTable.set_uid(player->uid);
		stHeadDownTable.set_seatid(player->seatid);
		stHeadDownTable.set_down_tag(player->down_tag);

		unicast(SERVER_AHEAD_DOWNTABLE_UC, player, stHeadDownTable);
		log.info("%s round down tid:%d uid:%d money:%d state:%d\n", __FUNCTION__, tid, player->uid, player->money, m_bGameState);
		return 0;
	}
	proto::login::AckDownTableSuccess stDownTableSuccess;
	stDownTableSuccess.set_uid(player->uid);
	stDownTableSuccess.set_seatid(player->seatid);
	stDownTableSuccess.set_money(player->money);
	player->down_tag = 0;

	broadcast(SERVER_DOWNTABLE_SUCC_BC, stDownTableSuccess);

	if (m_room_status == 0)
	{
		Stand_Up(player);
	}
	else
	{
		Player_Leave(player);
	}

	player->clear();
	log.info("%s succ tid:%d uid:%d money:%d players:%d \n", __FUNCTION__, tid, player->uid, player->money, m_mapTable_Player.size());
	
	return 0;
} 

int Table::handler_player_GPS(Client *client, Player *player)
{
	if (client == NULL || player == NULL)
	{
		return -1;
	}

	proto::login::AckGPS stGPS;
	stGPS.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);

	stGPS.set_uid(player->uid);

	broadcast(SERVER_PLAYER_GPS, stGPS);

	tagPlayerGPS stPlayerGPS;
	stPlayerGPS.latitude = stGPS.latitude();
	stPlayerGPS.longitude = stGPS.longitude();
	stPlayerGPS.city = stGPS.city();

	std::map<int, tagPlayerGPS>::iterator iter = m_mapPlayerGPS.find(player->uid);
	if (iter != m_mapPlayerGPS.end())
	{
		iter->second = stPlayerGPS;
	}
	else
	{
		m_mapPlayerGPS.insert(std::pair<int, tagPlayerGPS>(player->uid, stPlayerGPS));
	}

	return 0;
}

void Table::GameStart()
{  
	if (m_bGameState != GAME_FREE)
	{
		log.error("game start error, now tid:%d gamestate is playing... \n", tid);
		return;
	}	

	//初始化数据，庄家椅子id不能再被初始化
	InitData();	
	InitBanker();

	// 记录第一局开始时间
	if (0 == m_start_time)
	{
		m_start_time = time(NULL);
	}

	log.fatal("%s tid:%d round:%d Banker uid:%d chairid:%d player[%d, %d, %d, %d]\n", __FUNCTION__, tid, m_nPlayingTimes, GetPlayerUid(m_bBanker), m_bBanker,
		GetPlayerUid(0), GetPlayerUid(1), GetPlayerUid(2), GetPlayerUid(3));

	save_video_palyer_info();
	save_game_scene_info();

    //获得牌池牌
	m_bGameState = GAME_PLAYING;
    
    if (!m_bIsTestSvr || !LoadFixCards())
	{	
		m_CardPool.InitPool();		
	}

	m_Start_Shai = 	m_GameLogic.GetShaizi(2);

    //发送游戏开始协议
	proto::game::AckGameStart stGamestart;
	stGamestart.set_chairid(m_bBanker);
	stGamestart.set_ischangbanker(m_last_Banker == INVALID_CHAIRID ? 0 :
		m_last_Banker == m_bBanker ? 0 : 1);
	if (m_Start_Shai.Num > 0)
	{
		stGamestart.add_touziinfo(m_Start_Shai.Shai[0]);
		stGamestart.add_touziinfo(m_Start_Shai.Shai[1]);
	}
	stGamestart.set_totalcount(m_nTotalCount);
	stGamestart.set_currentcount(m_nCurrentCount);
	
	broadcast(SERVER_GAME_START, stGamestart);
	
	save_video_gamestar();

	// 广播圈数信息给玩家
	BroadQuanInfo();

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player != NULL)
		{
			m_SeatPlayer[i].player->status = GAME_PLAYING;
		}
	}
	
	ChangeBetterHandleCard();
	
	Send_Hand_Card();
}

bool Table::LoadFixCards()
{
    CardPoolReadJson2* json_card = CardPoolReadJson2::GetInstance();
    if (json_card->ReadJsonFile() && json_card->IsFixedCards())
    {
        m_CardPool.FixCard_ReadJson2();
        if (json_card->GetBanker() >= 0)
        {
            m_bBanker = json_card->GetBanker() % m_GAME_PLAYER;
        }

        return true;
    }
    else
    {
        return false;
    }
}

void Table::OnUserGetCard(_uint8 ChairID, bool bGetLast)
{
	if (m_bDeleteTable)
	{
		return;
	}

	if (CardPoolIsEmpty())
	{
		GameEnd();
		return;
	}
	memset(&m_bUserOperate, 0, sizeof(m_bUserOperate));
	m_bCurChairID = ChairID;

	if (!bGetLast)
    {
        CheckGetGoodCard(ChairID);
		CheckGetBadCard(ChairID);
    }

	_uint8 bGetCard = bGetLast? m_CardPool.GetLast() : m_CardPool.GetOneCard();

	m_bGetCardCount[ChairID]++;

	m_bCurCard = bGetCard;
	MJ_major major;
	//摸牌判断
	m_bUserOperate[ChairID] = GetCardEstimate(ChairID, bGetCard,  major, m_HuFanNums[ChairID]);

	m_GameLogic.AddCard(&m_HandCard[ChairID], bGetCard);

	//广播玩家摸牌
	
	proto::game::AckUserGetCard stUsergetcard;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player == NULL)
		{
			log.error("OnUserGetCard tid:%d chairid:%d  error, one player is NULL\n", tid, ChairID);
			continue;
		}

		Proto_Game_UserGetCard(i, ChairID, bGetCard, stUsergetcard);
		stUsergetcard.set_getcarddir(bGetLast ? 1 : 0);
		unicast(SERVER_USER_GETCARD, i, stUsergetcard);	
	}

	log.info("%s tid:%d uid:%d chairid:%d getcard:0x%02x optype:0x%x cardpoollen:%d \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bGetCard, m_bUserOperate[ChairID], m_CardPool.Count());

	save_user_get_card(ChairID, bGetCard, m_CardPool.Count(), bGetLast);

	if (m_bUserOperate[ChairID] == TYPE_NULL || m_bUserOperate[ChairID] == TYPE_LISTEN)
	{
		//log.info("start outcard timer, uid:%d chairid:%d\n", GetPlayerUid(ChairID), ChairID);
		//出牌时钟
		set_out_timer(ChairID);
	}
	else
	{
		//log.info("start operate timer, uid:%d chairid:%d have operate:0x%x \n", GetPlayerUid(ChairID), ChairID, m_bUserOperate[ChairID]);
		//操作时钟
		set_operator_timer(ChairID);

	}
}

void Table::UserOutCardErr(_uint8 ChairID, _uint8 bCard, int code)
{
	proto::game::AckUserOutCardErr stUseroutcardErr;
	stUseroutcardErr.set_card(bCard);
	stUseroutcardErr.set_code(code);
	for (int i = 0; i < m_HandCard[ChairID].ChangeableCardsLen; ++i)
	{
		stUseroutcardErr.add_usercard(m_HandCard[ChairID].ChangeableCards[i]);
	}	
	for (int i = 0; i < m_stUserOutCard[ChairID].Size(); ++i)
	{
		stUseroutcardErr.add_outcards(m_stUserOutCard[ChairID][i]);
	}
	unicast(SERVER_USER_OUTCARD_ERR, ChairID, stUseroutcardErr);
}

void Table::OnUserOutCard(_uint8 ChairID, _uint8 bCard)
{
	Stop_Auto_Disband();

	ev_timer_stop(zjh.loop, &m_Outcard_Timer);

	log.info("%s tid:%d uid:%d chairid:%d outcard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bCard);

	m_bCurChairID = ChairID;
	m_bOutCardChairID = ChairID;
	m_GameLogic.ChuPai(&m_HandCard[ChairID], bCard);
	m_bCurCard = bCard;
	m_bCurOutCard = bCard;
	m_bOverTimeState[ChairID] = 0;
	m_stUserOutCard[ChairID].Add(bCard);
	
    //快速模式，不广播，弹按钮时一同弹出，但是保存录像
    if (!m_conf.m_bFastMode)
	{
		proto::game::AckUserOutCard stUseroutcard;

		Proto_Game_UserOutCard(ChairID, bCard, stUseroutcard);

		broadcast(SERVER_USER_OUTCARD, stUseroutcard);	
	}

	save_user_out_card(ChairID, bCard);
}


bool Table::CheckUserOutCard(_uint8 ChairID, _uint8 cbCard)
{
	bool bIsHave = false;
	if(ChairID != m_bCurChairID || cbCard == INVALID_MJ_CARD) 
	{
		log.error("%s tid:%d error cur ChairID:%d, recv out uid:%d chairID:%d card:0x%02x \n", __FUNCTION__, tid, m_bCurChairID, GetPlayerUid(ChairID), ChairID, cbCard);
		return bIsHave;
	}

	if (m_HandCard[ChairID].ChangeableCardsLen % 3 != 2)
	{
		log.info("%s tid:%d chairid:%d ChangeableCardsLen:%d is can not out card \n", __FUNCTION__, tid, ChairID, m_HandCard[ChairID].ChangeableCardsLen);
		return bIsHave;
	}

	// 检查此张牌是否在用户手牌里
	_uint8 bChangeLens = m_HandCard[ChairID].ChangeableCardsLen;
	for (_uint8 i = 0; i < bChangeLens; i++)
	{
		if (m_HandCard[ChairID].ChangeableCards[i] == cbCard)
		{
			bIsHave = true;
			break;
		}
	}
	if (!bIsHave)
	{
		log.error("%s tid:%d uid:%d seatid:%d outcard error has no card:0x%02x  \n",  __FUNCTION__, tid, ChairID, GetPlayerUid(ChairID), cbCard);
		for (_uint8 i = 0; i < bChangeLens; i++)
		{
			log.info("card[%d]:0x%02x \n", i, m_HandCard[ChairID].ChangeableCards[i]);
		}
	}

	return bIsHave;
}

//用户打牌后的判断
bool Table::EstimateUserRespond(_uint8 bOutChairID, _uint8 bOutCard)
{
	m_bMultiOpUserNum = 0;
	m_bMultiOpHuNum = 0;
	bool bIsChangeUser = true;//是否轮到下一玩家摸牌

	memset(m_bUserOperate, TYPE_NULL, sizeof(m_bUserOperate));
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (i == bOutChairID) 
		{
			m_bUserOperate[i] = TYPE_NULL;
			continue;
		}
		//牌型判断			
		MJ_major major;		
		m_bUserOperate[i] = OutCardEstimate(i, bOutCard, major, m_HuFanNums[i]);

		if (m_bUserOperate[i] != TYPE_NULL)
		{
			if (m_bUserOperate[i] & TYPE_HU)
			{
				//m_bAddFanFlag[i] = true;
				m_bMultiOpHuNum++;				
			}
			m_bMultiOpUserNum++;
			bIsChangeUser = false;

			//快速模式，后面弹按钮
			if (!m_conf.m_bFastMode)
			{
				for (int j = 0; j < m_GAME_PLAYER; j++)
				{
					proto::game::AckOpNotify stNotify;		
					Proto_Game_OpNotify(i, j, bOutCard, stNotify);
					unicast(SERVER_OPERATE_NOTIFY, j, stNotify);	
					if (i == j) save_operate_notify(stNotify);		
				}
			}

			log.info("%s tid:%d uid:%d chairid:%d Operate:0x%x, bOutCard:0x%x totalop:%d \n", __FUNCTION__, tid, GetPlayerUid(i), i, m_bUserOperate[i], bOutCard, m_bMultiOpUserNum);	
		}
	}	

	//快速模式，打牌和按钮一起弹出
	if (m_conf.m_bFastMode)
	{
		for (int j = 0; j < m_GAME_PLAYER; j++)
		{
			proto::game::AckOpNotify stNotify;		
			stNotify.set_chairid(bOutChairID);
			stNotify.set_card(bOutCard);
			stNotify.set_optype(m_bUserOperate[j]);	
			stNotify.set_operateid(++m_UserOperateID[j]);	
			unicast(SERVER_OPERATE_RESULT2, j, stNotify);	

			if (m_bUserOperate[j] != TYPE_NULL)
			{
				stNotify.set_chairid(j);
				SaveVideoData(SERVER_OPERATE_NOTIFY, stNotify);	
			}		
		}
	}
	
	if (bIsChangeUser)
	{		
		m_bCurChairID = NextChair(bOutChairID);
		memset(m_bAddFanFlag, 0, sizeof(m_bAddFanFlag));
		OnUserGetCard(m_bCurChairID);
	}
	else 
	{
		//开启操作时钟 TODO		
		for (_uint8 c = NextChair(bOutChairID); c != bOutChairID; c = NextChair(c))
		{			
			if (m_bUserOperate[c] != TYPE_NULL)
			{
				set_operator_timer(c);
			}			
		}
	}

	return bIsChangeUser;
}

void Table::DoSimpleOperate(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	log.info("%s tid:%d uid:%d chairid:%d, iOptype:0x%x m_bUserOperate:0x%x bCard:0x%x \n", 
		__FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, m_bUserOperate[ChairID], bCard);
	
	switch (iOpType)
	{
		case TYPE_PENG:
		UserOperatePeng(ChairID, bCard);
		break;
		case TYPE_WANGANG:
		if (m_conf.m_bQiangGangHu && RobGangHu(ChairID, bCard, iOpType))			
			return;
		case TYPE_ANGANG:		
		if (m_conf.m_bQiangAnGang && RobGangHu(ChairID, bCard, iOpType))
			return;
		case TYPE_ZHIGANG:
		if (m_conf.m_bQiangZhiGang && RobGangHu(ChairID, bCard, iOpType))
			return;
		do
		{ 
			UserOperateGang(ChairID, iOpType, bCard);
			OnUserGetCard(ChairID, true);
		} while (0);
		break;
		case TYPE_LEFT_CHI:
		case TYPE_CENTER_CHI:
		case TYPE_RIGHT_CHI:
		UserOperateChi(ChairID, iOpType, bCard);

		break;
		case TYPE_PASS:
		UserOperatePass(ChairID, iOpType, bCard);
			//如果自摸，则轮到他出牌
		if (m_bCurChairID == ChairID)
		{
				//开启出牌时钟
			set_out_timer(ChairID);

		}
		else
		{				
			m_bAddFanFlag[m_bOutCardChairID] = false;
			if ( m_bCurChairID == INVALID_CHAIRID)
			{
				OnUserGetCard(m_bBanker);
			}
			else
			{
				m_bCurChairID = NextChair(m_bCurChairID);
				OnUserGetCard(m_bCurChairID);
			}
		}
		break;
		case TYPE_HU:
		UserOperateHu(ChairID);

		GameEnd();
		break;
		default:
		break;
	}	
}

bool Table::CheckSubOperate(_uint8 ChairID, int iOpType, _uint8& bCard)
{
	if (Is_ChiPengGang(iOpType))
	{
		if (iOpType & TYPE_CHI)
		{
			MJ_opinfo stChi;
			GetChi(ChairID, m_bCurOutCard, stChi);
			if (!stChi.Find(OpState(bCard, iOpType)))
			{
				log.error("%s tid:%d ChairID:%d iOpType:0x%x bCard: 0x%02x not exsit \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
				return false;
			}
		}
		if (iOpType == TYPE_PENG || iOpType == TYPE_ZHIGANG)
		{
			MJ_opinfo ganginfo;
			GetPengGang(ChairID, m_bCurOutCard, ganginfo);
			if (!ganginfo.Find(OpState(bCard, iOpType)))
			{
				log.error("%s tid:%d ChairID:%d iOpType:0x%x bCard: 0x%02x not exsit \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
				return false;
			}
		}
		if (iOpType == TYPE_WANGANG || iOpType == TYPE_ANGANG)
		{
			MJ_opinfo ganginfo;
			GetGangInfo(ChairID, ganginfo);
			if (!ganginfo.Find(OpState(bCard, iOpType)))
			{
				log.error("%s tid:%d ChairID:%d iOpType:0x%x bCard: 0x%02x not exsit \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
				return false;
			}
		}
	}	
	if (iOpType == TYPE_HU)
	{
		_uint8 tmpCard = m_bRobWanGang ? m_RobWanGangCard : m_bCurCard;
		if (bCard != INVALID_MJ_CARD && bCard != tmpCard)
		{
			bCard = tmpCard;
			log.error("%s tid:%d ChairID:%d iOpType:0x%x bCard: 0x%02x card wrong \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
		}
	}
	
	return true;							
}

bool Table::CheckUserOperator(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	if (iOpType == TYPE_NULL)
	{
		log.error("%s tid:%d error ChairID:%d iOpType:0x%x bCard:0x%x is NULL\n", __FUNCTION__, tid, ChairID, iOpType, bCard);
		return false;
	}
	if (ChairID >= m_GAME_PLAYER)
	{
		log.error("%s tid:%d error ChairID:%d iOpType:0x%x bCard:0x%x \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
		return false;
	}
	if ((iOpType != TYPE_NULL && !(m_bUserOperate[ChairID] & iOpType) && iOpType != TYPE_PASS)
		|| (iOpType == TYPE_PASS && m_bUserOperate[ChairID] == TYPE_NULL))
	{
		log.error("%s tid:%d error uid:%d chairid:%d iOpType:0x%x m_bUserOperate:0x%x\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, m_bUserOperate[ChairID]);
		return false;
	}
	if (Is_ChiPengGang(iOpType) && (bCard == 0x0 || bCard == 0xff))
	{
		log.error("%s tid:%d error uid:%d chairid:%d iOpType:0x%x card 0x%02x!\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, bCard);
		return false;
	}
	if ((iOpType != TYPE_PASS) && ( iOpType & (TYPE_CHI | TYPE_PENG)) && (bCard != m_bCurOutCard))
	{
		log.error("%s tid:%d error uid:%d chairid:%d iOpType:0x%x card 0x%02x outcard:0x%02x!\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, bCard, m_bCurOutCard);
		return false;
	}
	if (m_bUserOperate[ChairID] == TYPE_NULL || m_bUserOperate[ChairID] == TYPE_LISTEN)
	{
		log.error("%s tid:%d error uid:%d chairid:%d iOpType:0x%x m_bUserOperate:0x%x\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, m_bUserOperate[ChairID]);
		return false;
	}
	if (m_bRobWanGang && ChairID == m_WangGangPlayer)
	{
		log.error("%s tid:%d rob gang error uid:%d chairid:%d iOpType:0x%x m_bUserOperate:0x%x\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, m_bUserOperate[ChairID]);
		return false;
	}
	//验证吃碰杠胡
	if (!CheckSubOperate(ChairID, iOpType, bCard))
	{
		return false;
	}	
	//验证多操作是否已经操作
	if (m_bMultiOpUserNum > 1 && m_bMultiUserDoneOp[ChairID] != TYPE_NULL)
	{
		log.error("%s tid:%d ChairID:%d iOpType:0x%x bCard:0x%x m_bMultiUserDoneOp:0x%x already op .\n", __FUNCTION__, tid, ChairID, iOpType, bCard, m_bMultiUserDoneOp[ChairID]);
		return false;
	}

	return true;
}

void Table::OnRecvUserOperate(_uint8 ChairID, int iOpType, int iCard)
{
	Stop_Auto_Disband();

	_uint8 bCard = (_uint8)iCard;
	
	stop_operator_timer(ChairID);
	m_bOverTimeState[ChairID] = 0;
    //如果是多操作
	if (m_bMultiOpUserNum > 1)
	{
		DoMultiOperate(ChairID, iOpType, bCard);
	}
	else 
	{   
		if (m_bRobWanGang)  //抢杠胡操作优先级
		{
			RobGangPriority(ChairID, bCard, iOpType);
			return ;
		}
		DoSimpleOperate(ChairID, iOpType, bCard);
		//判断是下一玩家摸牌，或者当前玩家出牌		
	}
}

void Table::BroadcastOperateResult(_uint8 bOpChairID, int iOpType, _uint8 bOpCard)
{
    //广播发送操作结果	
	proto::game::AckBcOpResult stOpresult;
	stOpresult.set_chairid(bOpChairID);
	stOpresult.set_outcardchairid(m_bCurChairID);
	stOpresult.set_optype(iOpType);
	stOpresult.set_card(bOpCard);
	SetRobGangHu_Flag(bOpChairID, iOpType, stOpresult);
	stOpresult.set_passhu(m_PassHuOperate);
	broadcast(SERVER_OPERATE_RESULT, stOpresult);
}

void Table::SaveOperateResultVideo(_uint8 bOpChairID, int iOpType, _uint8 bOpCard)
{	
	proto::game::AckBcOpResult stOpresult;
	stOpresult.set_chairid(bOpChairID);
	stOpresult.set_outcardchairid(m_bCurChairID);
	stOpresult.set_optype(iOpType);
	stOpresult.set_card(bOpCard);
	SetRobGangHu_Flag(bOpChairID, iOpType, stOpresult);
	for (size_t i = 0; i < m_vecOpCards.size(); ++i)
	{
		stOpresult.add_opcards(m_vecOpCards[i]);
	}
	stOpresult.set_passhu(m_PassHuOperate);		
	SaveVideoData(SERVER_OPERATE_RESULT, stOpresult);
}

void Table::CalcGangScore(_uint8 ChairID, int iOpType, _uint8 bCard, int iGangScore[GAME_PLAYER], _uint8 opChairID)
{
	switch (iOpType)
	{
		case TYPE_ANGANG:
		{
			for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
			{
				iGangScore[ChairID] += m_BaseScore * m_conf.m_AnGangPoint;
				iGangScore[c] -= m_BaseScore * m_conf.m_AnGangPoint;
			}			
		}
		break;
		case TYPE_WANGANG:
		{	
			for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
			{
				iGangScore[ChairID] += m_BaseScore * m_conf.m_WanGangPoint;
				iGangScore[c] -=  m_BaseScore * m_conf.m_WanGangPoint;					
			}	
		}
		break;
		case TYPE_ZHIGANG:
		{
			iGangScore[ChairID] +=  m_BaseScore * m_conf.m_ZhiGangPoint;
			iGangScore[opChairID] -=  m_BaseScore * m_conf.m_ZhiGangPoint;
		}
		break;
		default:
		break;
	}
}

void Table::UserOperateGang(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	memset(m_bAddFanFlag, 0, sizeof(m_bAddFanFlag));
	memset(m_bUserOperate, 0, sizeof(m_bUserOperate));

	m_bAddFanFlag[ChairID] = true;
	_uint8 OpChairID = INVALID_CHAIRID;
	switch (iOpType)
	{
		case TYPE_ANGANG:
		case TYPE_WANGANG:
			OpChairID = ChairID;
			m_stGang[ChairID].Gang.Add(tagGangScore(bCard, ChairID, iOpType));
			break;
		case TYPE_ZHIGANG:
			OpChairID = m_bOutCardChairID;
			m_stGang[ChairID].Gang.Add(tagGangScore(bCard, OpChairID, iOpType));
			m_bGetCardCount[ChairID]++;
			m_stUserOutCard[OpChairID].RemoveLast(bCard);
			break;
		default:
			break;
	}

	int iGangScore[GAME_PLAYER] = {0};
	CalcGangScore(ChairID, iOpType, bCard, iGangScore, OpChairID);
	Calc_Score(m_stGang, iGangScore);	
	
	log.info("%s tid:%d uid:%d ChairID:%d, iOpType:0x%x, bCard:0x%02x OpChairID:%d iGangScore:[%d %d %d %d] \n", 
		__FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, bCard, OpChairID, iGangScore[0], iGangScore[1], iGangScore[2], iGangScore[3]);

	m_GameLogic.ExecuteGang(&m_HandCard[ChairID], OpChairID, bCard, iOpType);

	proto::game::AckBcOpResult stOpresult;
	Proto_Game_OperateGang(ChairID, bCard, iOpType, iGangScore, stOpresult);

	//暗杠不亮牌
	if (m_conf.m_bAnGangNoShow && iOpType == TYPE_ANGANG)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			stOpresult.set_card(i == ChairID ? bCard : 0);
			unicast(SERVER_OPERATE_RESULT, i, stOpresult);		
		}
	}
	else
	{
		broadcast(SERVER_OPERATE_RESULT, stOpresult);
	}

	stOpresult.set_card(bCard);
	for (size_t i = 0; i < m_vecOpCards.size(); ++i)
	{
		stOpresult.add_opcards(m_vecOpCards[i]);
	}	
	stOpresult.set_passhu(m_PassHuOperate);	
	SaveVideoData(SERVER_OPERATE_RESULT, stOpresult);

	m_bUserOperate[ChairID] = TYPE_NULL;
}

void Table::UserOperatePass(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	log.info("%s tid:%d uid:%d ChairID:%d, iOpType:0x%x, m_bUserOperate:0x%02x bCard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, m_bUserOperate[ChairID], bCard);
	BroadcastOperateResult(ChairID, iOpType, bCard);
	//保存点过录像
	SaveOperateResultVideo(ChairID, iOpType, bCard);

	if (ChairID != m_bCurChairID)
	{
		m_bAddFanFlag[ChairID] = false;
		m_bUserOperate[ChairID] = TYPE_NULL;
	}
	if (ChairID == m_bCurChairID)
	{
		m_bUserOperate[ChairID] &= ~(TYPE_CHI_PENG_GANG | TYPE_HU );
	}
}

void Table::UserOperateHu(_uint8 ChairID, _uint8 bHuCard)
{
	_uint8 bCard = INVALID_MJ_CARD;
	if (bHuCard == INVALID_MJ_CARD)
	{
		bCard = m_bCurCard;
	}
	else
	{
		bCard = bHuCard;
	}
	
	if (!(m_bUserOperate[ChairID] & TYPE_HU))
	{
		log.error("%s tid:%d chairid:%d bHuCard:0x%x m_bUserOperate:0x%x:%d \n", __FUNCTION__, tid, ChairID, bHuCard, m_bUserOperate[ChairID]);
		return ;
	}	

	log.info("%s tid:%d uid:%d ChairID:%d hu FanNum:%d bCard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, m_HuFanNums[ChairID], bCard);	
	
	m_bIsHu[ChairID] = true;	
	int userscore[GAME_PLAYER] = { 0 };
	Calc_Hu(ChairID, bCard, userscore);	
	Calc_Score(m_stHuScore, userscore);

	//广播操作结果
	proto::game::AckBcOpResult stOpresult;
	stOpresult.set_chairid(ChairID);
	stOpresult.set_outcardchairid(m_bRobWanGang ? m_WangGangPlayer : m_bCurChairID);	
	stOpresult.set_optype(TYPE_HU);
	stOpresult.set_card(bCard);
	int type = TYPE_NULL;
	if (m_oFan[ChairID].Find(MJ_FAN_TYPE_GANGSHANGHUA)){
		type = HU_GANG_SHANG_HUA;
	}else if (m_oFan[ChairID].Find(MJ_FAN_TYPE_GANGHOUPAO)){
		type = HU_GANG_HOU_PAO;
	}else if (m_bRobWanGang){
		type = (m_WangGangPlayerOpType == TYPE_ZHIGANG ? HU_QIANG_ZHI_GANG : HU_QIANG_GANG);
	}
	stOpresult.set_optype2(type);
	broadcast(SERVER_OPERATE_RESULT, stOpresult);
	SaveVideoData(SERVER_OPERATE_RESULT, stOpresult);
	
	//广播胡
	proto::game::AckUserHu stHu;
	stHu.set_huchairid(ChairID);
	stHu.set_pashuchairid(m_bCurChairID);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stHu.add_score(userscore[i]);		
	}
	// 大胡动画
	Proto_Game_HuFan(stHu, ChairID);
	broadcast(SERVER_USER_HU, stHu);
	SaveVideoData(SERVER_USER_HU, stHu);

	m_bUserOperate[ChairID] = TYPE_NULL;
}

void Table::GameEnd()
{
	
	//摸完最后一张牌才算是结束
	MJ_user_buffer UserID;
	_uint8 bDraw = (MultiHu(UserID) == 0) ? 1 : 0;//结束状态，1表示流局，0表示胡牌结束	
	
	//单发结算协议	
	proto::game::AckGameEnd stGameend;
	HuScoreSet hu_score;
	
	if (bDraw)
	{
		Score_GameEndDraw(hu_score, stGameend);
	}
	else
	{
		Score_GameEndSucc(hu_score, stGameend);
	}
	stGameend.set_endstate(bDraw);

	Proto_GameEndInfo_AddFan(UserID, stGameend);

	int UserScore[GAME_PLAYER] = {0};
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::HandCards *pHandcard = stGameend.add_usercard();
		Fill_HandCard(i, pHandcard);

		Proto_GameEndInfo_GangInfo(i, stGameend);

		UserScore[i] = hu_score[i].iTotleScore;

		stGameend.add_score(UserScore[i]);	
	}

	log.fatal("%s tid:%d bDraw:%d score[%d,%d,%d,%d]\n", __FUNCTION__, tid, bDraw, UserScore[0], UserScore[1], UserScore[2], UserScore[3]);

	tagUserScore userscore[GAME_PLAYER];
	UpdatePlayerInfo(userscore, UserScore, stGameend);
	
	stGameend.set_ts((int)time(NULL));

	broadcast(SERVER_GAME_END, stGameend);
 	//发送录像
	save_video_game_end(stGameend);

	update_user_info_to_datasvr_dz(userscore, 1102);

	//数据统计
	CalcRecordInfo(UserID, UserScore);

	RedisUpdateScoreToServer(UserScore);

	RecordScore(m_nCurrentCount);

	if (m_nCurrentCount >= m_nTotalCount)
	{
		CalcContinueWinTimes();
		SaveEndRecordVideo();
	}		

	ev_timer_again(zjh.loop, &send_videodata_timer);	

	m_last_Banker = m_bBanker;
	//轮庄
	TurnBanker();
	//初始化数据
	GameEndReset();
}

void Table::UpdatePlayerInfo(tagUserScore userscore[GAME_PLAYER], int UserScore[GAME_PLAYER], proto::game::AckGameEnd& stGameend)
{
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player != NULL)
		{  
			userscore[i].uid = m_SeatPlayer[i].player->uid;
			userscore[i].score = UserScore[i] ;
			if (UserScore[i] > 0)
			{
				m_SeatPlayer[i].player->update_info(UserScore[i], 1, 1, win_exp);
			}
			else
			{
				m_SeatPlayer[i].player->update_info(UserScore[i], 1, 0, lose_exp);
			}

			//UpdateUserInfoToUser(m_SeatPlayer[i].player); //客户端动画会有问题
			stGameend.add_money(m_SeatPlayer[i].player->money); 
		}
	}
}

void Table::GameEndReset()
{  

	memset(m_stGang, 0, sizeof(m_stGang));
	memset(m_stHuScore, 0, sizeof(m_stHuScore));
	memset(m_bIsHu, 0, sizeof(m_bIsHu));
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		m_louhu[i].Unlock();
	}

	m_niao.Clear();
	m_zhong_niao.Clear();
	m_DaNiaoID = INVALID_CHAIRID;
	m_bGameState = GAME_FREE;

	m_bRobWanGang = false;
	m_bRobGangHuJudge = false;
	m_WangGangPlayerOpType = TYPE_NULL;
	m_PassHuPlayer.Clear();
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player == NULL)
		{
			continue;
		}
		m_SeatPlayer[i].player->status =  SIT_DOWN;		
		{
			m_SeatPlayer[i].action = ACTION_SIDOWN;
		}
	}

	log.info("%s tid:%d player uid:[%d,%d,%d,%d] game reset\n", __FUNCTION__, tid,
		GetPlayerUid(0), GetPlayerUid(1), GetPlayerUid(2), GetPlayerUid(3));
	
	memset(&m_HandCard, 0, sizeof(m_HandCard));
	memset(m_stUserOutCard, 0, sizeof(m_stUserOutCard));
	
	ev_timer_stop(zjh.loop, &m_Outcard_Timer);
	ev_timer_stop(zjh.loop, &m_Operate_Timer0);
	ev_timer_stop(zjh.loop, &m_Operate_Timer1);
	ev_timer_stop(zjh.loop, &m_Operate_Timer2);
	ev_timer_stop(zjh.loop, &m_Operate_Timer3);

	memset(m_bUserOperate, 0, sizeof(m_bUserOperate));

}

void Table::UserOutCard_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	if (table)
	{		
		ev_timer_stop(zjh.loop, &table->m_Outcard_Timer);
		table->m_bOverTimeState[table->m_bCurChairID] = proto::game::OVER_TIME_OUTCARD;
		table->UserOvertime_State(table->m_bCurChairID);		
		table->m_totalrecordinfo.timerout_op[table->m_bCurChairID]++;
		table->UserOutCardTimeOut(table->m_bCurChairID);
	}
}

void Table::UserChooseOperate_Timer_cb0(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	if (table)
	{
		log.info("%s tid:%d uid:%d Chairid:0 choose timer out...\n", __FUNCTION__, table->tid, table->GetPlayerUid(static_cast<_uint8>(0)));
		ev_timer_stop(zjh.loop, &table->m_Operate_Timer0);
		if (table->m_bUserOperate[0] == TYPE_NULL)
		{
			log.info("chairid 0 operate timer is on, but it have not operate \n");
			return;
		}
		table->m_bOverTimeState[0] = proto::game::OVER_TIME_OPERATE;
		table->UserOvertime_State(0);		
		table->m_totalrecordinfo.timerout_op[0]++;
		table->UserChooseOperateTimeOut(0);
	}
}

void Table::UserChooseOperate_Timer_cb1(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	if (table)
	{
		log.info("%s tid:%d uid:%d Chairid:1 choose timer out...\n", __FUNCTION__, table->tid, table->GetPlayerUid(static_cast<_uint8>(1)));
		ev_timer_stop(zjh.loop, &table->m_Operate_Timer1);
		if (table->m_bUserOperate[1] == TYPE_NULL)
		{
			log.info("chairid 1 operate timer is on, but it have not operate \n");
			return;
		}
		table->m_bOverTimeState[1] = proto::game::OVER_TIME_OPERATE;
		table->UserOvertime_State(1);		
		table->m_totalrecordinfo.timerout_op[1]++;
		table->UserChooseOperateTimeOut(1);
	}
}

void Table::UserChooseOperate_Timer_cb2(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	if (table)
	{
		log.info("%s tid:%d uid:%d Chairid:2 choose timer out...\n", __FUNCTION__, table->tid, table->GetPlayerUid(static_cast<_uint8>(2)));
		ev_timer_stop(zjh.loop, &table->m_Operate_Timer2);
		if (table->m_bUserOperate[2] == TYPE_NULL)
		{
			log.info("chairid 2 operate timer is on, but it have not operate \n");
			return;
		}
		table->m_bOverTimeState[2] = proto::game::OVER_TIME_OPERATE;
		table->UserOvertime_State(2);		
		table->m_totalrecordinfo.timerout_op[2]++;
		table->UserChooseOperateTimeOut(2);
	}
}

void Table::UserChooseOperate_Timer_cb3(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	if (table)
	{
		log.info("%s tid:%d uid:%d Chairid:3 choose timer out...\n", __FUNCTION__, table->tid, table->GetPlayerUid(static_cast<_uint8>(3)));
		ev_timer_stop(zjh.loop, &table->m_Operate_Timer3);
		if (table->m_bUserOperate[3] == TYPE_NULL)
		{
			log.info("chairid 3 operate timer is on, but it have not operate \n");
			return;
		}
		table->m_bOverTimeState[3] = proto::game::OVER_TIME_OPERATE;
		table->UserOvertime_State(3);		
		table->m_totalrecordinfo.timerout_op[3]++;
		table->UserChooseOperateTimeOut(3);
	}	
}

void Table::User_Ready(_uint8 ChairID, int action)
{
	if (m_SeatPlayer[ChairID].player == NULL)
	{
		return;
	}
	if (m_SeatPlayer[ChairID].action == proto::game::ACTION_READY)
	{
		return;
	}
	m_SeatPlayer[ChairID].action = proto::game::ACTION_READY;
	m_SeatPlayer[ChairID].player->status = READY;
	_uint8 bReadyNum = 0;

	proto::game::AckUserReady stReady;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		int nready = m_SeatPlayer[i].action == proto::game::ACTION_READY ? 1 : 0;
		stReady.add_ready(nready);
		if (nready)
		{
			bReadyNum++;
		}
	}
	log.info("%s tid:%d uid:%d chairid:%d ready. ready num:%d \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bReadyNum);

	broadcast(SERVER_USER_READY, stReady);

	UserStatusNotify(ChairID);

	if (bReadyNum == m_GAME_PLAYER)
	{
		m_nPlayingTimes++;
		game_start_req();
		TIMER_AGAIN(req_start_timer, req_start_timer_stamp);
	}
}

void Table::recv_client_action(Client *client, Player *player)
{
	if (client == NULL)
	{
		log.info("RecvClientAction *client is NULL \n");
		return;
	}
	if (player == NULL)
	{
		log.info("RecvClientAction  player is NULL \n");
		return;
	}
	_uint8 ChairID = player->seatid;
	if (ChairID >= m_GAME_PLAYER || ChairID < 0)
	{
		log.info("RecvClientAction, but chaird:%d is wrong \n", player->seatid);
		return;
	}
	proto::game::ReqAction stAction;
	stAction.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
	int action = stAction.action();
	switch (action)
	{
		case proto::game::ACTION_READY:
		{
			User_Ready(ChairID, action);
		}
		break;
		default:
		break;
	}
}

void Table::recv_client_outcard(Client *client, Player *player)
{
	if (player->seatid >= m_GAME_PLAYER || player->seatid < 0)
	{
		log.error("%s tid:%d but uid:%d chaird:%d is wrong \n", __FUNCTION__, tid, player->uid, player->seatid);
		return;
	}
	
	int bOutChairID = player->seatid;
	proto::game::ReqOutCard stOutCard;
	stOutCard.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);	
	_uint8 bOutCard = stOutCard.card();

	//log.info("%s tid:%d uid:%d chairid:%d outcard:0x%02x \n", __FUNCTION__, tid, player->uid, bOutChairID, bOutCard);

	recv_client_outcard(bOutChairID, bOutCard);
}

void Table::recv_client_outcard(_uint8 bOutChairID, _uint8 bOutCard)
{
	if (m_bDeleteTable || m_bIsDisbandTable)
	{
		log.error("%s tid:%d table delete \n", __FUNCTION__, tid);
		return;
	}

	//检查玩家打出的牌
	if (CheckUserOutCard(bOutChairID, bOutCard))
	{
		OnUserOutCard(bOutChairID, bOutCard);
		//出牌后其他玩家操作判断
		EstimateUserRespond(bOutChairID, bOutCard);		
	}	
}

void Table::recv_client_opeatereuslt(Client *client, Player *player)
{
	if (m_bDeleteTable || m_bIsDisbandTable)
	{
		log.error("%s tid:%d table delete \n", __FUNCTION__, tid);
		return;
	}

	if (player->seatid >= m_GAME_PLAYER || player->seatid < 0)
	{
		log.error("%s tid:%d but chaird:%d is wrong \n", __FUNCTION__, tid, player->seatid);
		return;
	}

	int chairid = player->seatid;
	proto::game::ReqOperateResult stOperate;
	stOperate.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
	int iOpType = stOperate.type();
	int iCard = stOperate.card();
	int iOpId = stOperate.operateid();
	m_vecOpCards.clear();
	for (int i = 0; i < stOperate.opcards_size(); ++i)
	{
		m_vecOpCards.push_back(stOperate.opcards(i));
	}
	m_PassHuOperate = stOperate.passhu();

	//log.info("%s tid:%d chairid:%d iOpType:0x%x iCard:0x%x iOpId:%d m_vecOpCards:%s m_PassHuOperate:%d.\n", 
	//	__FUNCTION__, tid, chairid, iOpType, iCard, iOpId, CatVectorCard(m_vecOpCards), m_PassHuOperate);

	if (!Check_OperateInfo(chairid, iOpType, iCard, iOpId, m_vecOpCards))
	{
		return;
	}
	if (!CheckUserOperator(chairid, iOpType, iCard))
	{
		return;
	}

	OnRecvUserOperate(chairid, iOpType, iCard);
}

void Table::RecvClientAutoHu(Client *client, Player *player)
{
	if (client == NULL)
	{
		log.error("%s tid:%d error client is null", __FUNCTION__, tid);
		return;
	}

	proto::game::ReqIsAuto stReq;
	stReq.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);

	if (player == NULL)
	{
		log.error("%s tid:%d player client is null", __FUNCTION__, tid);
		return;
	}

	_uint8 ChairID = player->seatid;
	m_bAutoHu[ChairID] = stReq.isauto() == 0 ? false : true;
	log.info("%s tid:%d uid:%d chairid:%d autoHu:%d \n", __FUNCTION__, tid, player->uid, player->seatid, m_bAutoHu[ChairID]);
}

bool Table::IsListening(_uint8 ChairID, _uint8 bRemoveCard)
{	
	HandCards tempHandCard = m_HandCard[ChairID];
	if (bRemoveCard != INVALID_MJ_CARD)
	{
		m_GameLogic.RemoveCard(&tempHandCard, bRemoveCard);
	}

	_uint8 last_check = INVALID_MJ_CARD;
	MJ_BUFFER<_uint8, POOL_CARD_TYPE_MAX> _relativity;
	for (int i = 0; i < tempHandCard.ChangeableCardsLen; ++i)
	{
		if (last_check == tempHandCard.ChangeableCards[i])
		{
			continue;
		}
		last_check = tempHandCard.ChangeableCards[i];
		GetHandCardRelativity(last_check, _relativity);
	}

	for (_uint8 i = 0; i < _relativity.Size(); ++i)
	{
		m_GameLogic.AddCard(&tempHandCard, _relativity[i]);
		MJ_fan_buffer fan;
		if (Check_Hu(ChairID, tempHandCard, fan) == TYPE_HU)
		{
			log.info("%s uid:%d chairdid:%d Remove:0x%02x\n", __FUNCTION__, GetPlayerUid(ChairID), ChairID, bRemoveCard);
			return true;
		}
		m_GameLogic.ChuPai(&tempHandCard, _relativity[i]);
	}
	return false;
}

void Table::Reconnect(Player *player)
{
	if (player == NULL)
	{
		return;
	}
	int uid = player->uid;
	log.info("%s tid:%d uid:%d chairid:%d\n", __FUNCTION__, tid, uid, player->seatid);
	std::map<int, Player *>::iterator iter = m_mapTable_Player.find(uid);
	if (iter != m_mapTable_Player.end())
	{		
		iter->second = player;
		
	}
	if (player->seatid < m_GAME_PLAYER && player->seatid >= 0)
	{
		m_SeatPlayer[player->seatid].player = player;
	}
	User_Offline(player, false);
	
}

void Table::UpdateUserInfoToUser(Player *player)
{
	if (player == NULL)
	{
		log.error("%s tid:%d player client is null", __FUNCTION__, tid);
		return;
	}
	log.info("%s tid:%d uid:%d chairid:%d money:%d\n", __FUNCTION__, tid, player->uid, player->seatid, player->money);	
	proto::game::AckUpdateUserInfo stAck; 
	stAck.set_chairid(player->seatid);
	stAck.set_uid(player->uid);
	stAck.set_money(player->money);
	stAck.set_rmb(player->rmb);

	broadcast(SERVER_UPDATE_USERINFO, stAck);

	SaveVideoData(SERVER_UPDATE_USERINFO, stAck);
}

void Table::RecvUserTrustee(Client *client)
{
	if (client == NULL)
	{
		log.error("%s tid:%d client is null", __FUNCTION__, tid);
		return;
	}
	
	proto::game::ReqIsAuto stReq;
	stReq.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
	
	if (client->player == NULL)
	{
		log.error("%s tid:%d player is null", __FUNCTION__, tid);
		return;
	}
	
	_uint8 ChairID = client->player->seatid;
	m_bIsTrustee[ChairID] = stReq.isauto() == 0 ? false : true;

	proto::game::AckIsAuto stAck;
	stAck.set_chairid(client->player->seatid);
	stAck.set_isauto(m_bIsTrustee[ChairID]);

	broadcast(SERVER_TRUSTEE, stAck);
	log.info("%s tid:%d uid:%d chairid:%d trustee:%d \n", __FUNCTION__, tid, client->uid, client->player->seatid, m_bIsTrustee[ChairID]);
}

int  Table::Get_Card_Left_Num(_uint8 ChairID, _uint8 cbCard)
{
	int nNum = 0;

	nNum = m_CardPool.Find(cbCard);

	for (int k = 0; k < m_GAME_PLAYER; k++)
	{   
		if (k == ChairID)
		{
			continue;
		}

		for (int j = 0; j < m_HandCard[k].ChangeableCardsLen; j++)
		{
			if (cbCard == m_HandCard[k].ChangeableCards[j])
			{
				nNum++;
			}
		}

		//暗杠不亮牌
		if (m_conf.m_bAnGangNoShow)
		{
			for (int j = 0; j < m_HandCard[k].FixedCardsLen; ++j)
			{
				if (m_HandCard[k].FixedCards[j].state == TYPE_ANGANG && cbCard == m_HandCard[k].FixedCards[j].CardData)
				{
					nNum += 4;
				}
			}
		}
	}
	return nNum;
}

int Table::GetEncryptCode() 
{
    srand(time(0));
    return (rand() % 255 + 1);          //牌的数据占8位
}

int Table::EncryptCard(_uint8 card, int encrypt_code) 
{
    return (card ^ encrypt_code);
}

bool Table::RobGangHu(_uint8 ChairID, _uint8 CardData, int iOpType)
{  
	if (iOpType & TYPE_WANGANG && !m_conf.m_bQiangGangHu)
		return false;
	if (iOpType & TYPE_ZHIGANG && !m_conf.m_bQiangZhiGang)
		return false;
	if (iOpType & TYPE_ANGANG && !m_conf.m_bQiangAnGang)
		return false;	

	_uint8 cbNum = 0;
	bool bOperated = false;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{    
		if (i == ChairID)
		{
			continue;
		}		
		MJ_major major;
		m_bRobGangHuJudge = true;
		m_WangGangPlayerOpType = iOpType;
		m_bUserOperate[i] = OutCardEstimate(i, CardData, major, m_HuFanNums[i]);		
		m_bRobGangHuJudge = false;
		m_WangGangPlayerOpType = TYPE_NULL;
		m_bUserOperate[i] &= TYPE_HU;
		if (m_bUserOperate[i] & TYPE_HU)
		{   
			const HandCards& hc = m_HandCard[ChairID];
			MJ_opinfo opinfo;
			
			if (iOpType & TYPE_WANGANG && m_GameLogic.GetWanGang(&hc, opinfo, CardData))
			{
				if (!bOperated)
				{
					bOperated = true;
					BroadcastOperateResult(ChairID, iOpType, CardData);
					SaveOperateResultVideo(ChairID, iOpType, CardData);
				} 

				for (int c = 0; c < m_GAME_PLAYER; c++)
				{
					proto::game::AckOpNotify stNotify;
					Proto_Game_OpNotify(i, c, CardData, stNotify);
					stNotify.set_optype2(HU_QIANG_GANG);
					unicast(SERVER_OPERATE_NOTIFY, c, stNotify);

					if (i == c) save_operate_notify(stNotify);		
				}
				cbNum++;
			}
			else if (iOpType & TYPE_ZHIGANG && m_GameLogic.GetZhiGang(&hc, opinfo, CardData))
			{
				if (!bOperated)
				{
					bOperated = true;
					BroadcastOperateResult(ChairID, iOpType, CardData);
					SaveOperateResultVideo(ChairID, iOpType, CardData);
				} 

				for (int c = 0; c < m_GAME_PLAYER; c++)
				{
					proto::game::AckOpNotify stNotify;
					Proto_Game_OpNotify(i, c, CardData, stNotify);
					stNotify.set_optype2(HU_QIANG_ZHI_GANG);
					unicast(SERVER_OPERATE_NOTIFY, c, stNotify);

					if (i == c) save_operate_notify(stNotify);	
				}
				cbNum++;
			}
			else if (iOpType & TYPE_ANGANG && m_GameLogic.GetAnGang(&hc, opinfo, CardData))
			{
				if (!bOperated)
				{
					bOperated = true;
					BroadcastOperateResult(ChairID, iOpType, CardData);
					SaveOperateResultVideo(ChairID, iOpType, CardData);
				} 

				for (int c = 0; c < m_GAME_PLAYER; c++)
				{
					proto::game::AckOpNotify stNotify;
					Proto_Game_OpNotify(i, c, CardData, stNotify);
					stNotify.set_optype2(HU_QIANG_GANG);
					unicast(SERVER_OPERATE_NOTIFY, c, stNotify);

					if (i == c) save_operate_notify(stNotify);
				}
				cbNum++;
			}					
		}
	}
	//开启操作时钟 TODO
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (i == ChairID)
		{
			continue;
		}
		log.info("%s tid:%d uid:%d chaird:%d out card, so that chairid:%d have operate:0x%x\n", __FUNCTION__, tid, GetPlayerUid(ChairID),ChairID, i, m_bUserOperate[i]);			
		if (m_bUserOperate[i] & TYPE_HU)
		{
			set_operator_timer(i);
		}

	}
	if (cbNum > 0)
	{
		m_RobHuNum = cbNum;
		m_bRobWanGang = true;
		m_WangGangPlayer = ChairID;
		m_WangGangPlayerOpType = iOpType;
		m_RobWanGangCard = CardData;

		//执行杠牌
		OperateRobGang(ChairID, CardData, iOpType);
	}

	return (cbNum > 0);
}

//抢杠胡操作优先级
void Table::RobGangPriority(_uint8 ChairID, _uint8 CardData, int OperateType)
{
	stop_operator_timer(ChairID);

	m_RobOperateNum++;
	m_RobHuDoneOp[ChairID] = OperateType;
	if (TYPE_HU == OperateType)
	{
		m_bRobHu = true;		
		m_bAddFanFlag[m_WangGangPlayer] = true;	
		
		if (m_conf.m_bYiPaoDuoXiang)
		{
			m_bIsHu[ChairID] = true;
			UserOperateHu(ChairID, m_RobWanGangCard);

			if ( m_conf.m_bForceHuQiangGangHu )
			{
				RobGangForceHu(ChairID, m_RobWanGangCard);
			}
			
		}
	}
	else if (OperateType == TYPE_PASS)
	{
		UserOperatePass(ChairID, OperateType, CardData);
		m_PassHuPlayer.Add(ChairID);
	}
	
	log.info("%s tid:%d uid:%d chairid:%d OperateNum:%d RobHuNum:%d \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, m_RobOperateNum, m_RobHuNum);
	if (m_RobOperateNum == m_RobHuNum)  //所有胡玩家都已经操作完毕
	{
		//恢复抢杠
		RecoverRobGang(m_WangGangPlayer, m_RobWanGangCard, m_WangGangPlayerOpType);
		
		if (m_bRobHu)   //如果有人胡牌
		{
			if (!m_conf.m_bYiPaoDuoXiang)
			{
				for (_uint8 c = NextChair(m_WangGangPlayer); c != m_WangGangPlayer; c = NextChair(c))
				{
					if (m_RobHuDoneOp[c] == TYPE_PASS)
						continue;
					if (m_bUserOperate[c] & TYPE_HU)
					{
						m_bIsHu[c] = true;
						UserOperateHu(c, m_RobWanGangCard);
						break;
					}
				}
			}
			if (m_WangGangPlayerOpType & (TYPE_ANGANG | TYPE_WANGANG))
				m_GameLogic.RemoveCard(&m_HandCard[m_WangGangPlayer], m_RobWanGangCard, 1);			
			GameEnd();
		}
		else
		{
			UserOperateGang(m_WangGangPlayer, m_WangGangPlayerOpType, m_RobWanGangCard);
			OnUserGetCard(m_WangGangPlayer, true);
		}

	}
	if (m_RobOperateNum == m_RobHuNum)
	{
		m_bRobWanGang = false;
		m_bRobHu = false;
		m_RobOperateNum = 0;
		m_RobHuNum = 0;
		
		m_WangGangPlayer = INVALID_CHAIRID;
		m_WangGangPlayerOpType = TYPE_NULL;
		m_RobWanGangCard = INVALID_MJ_CARD;
		memset(&m_RobHuDoneOp, 0, sizeof(m_RobHuDoneOp));
		m_PassHuPlayer.Clear();

	}
}



bool Table::is_all_ready()
{
	for (int i = 0; i<m_GAME_PLAYER; i++)
	{
		//判断已经坐下的玩家是否都已经准备
		if (m_SeatPlayer[i].state == Seat::E_Seat_Occupy)
		{
			if (m_SeatPlayer[i].action != ACTION_READY)
			{
				return false;
			}			
		}
	}
	return true;
}

void Table::clean_table_user()
{
	std::vector<Player*> temp_del_vec;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{   
		Player *player = m_SeatPlayer[i].player;
		if(player != NULL &&(NULL == player->client || 
			player->money < stand_money || 1 == player->down_tag))
		{  
			log.info("%s uid:%d client:%p money:%d down_tag:%d \n", __FUNCTION__, player->uid, player->client, player->money, player->down_tag);
			temp_del_vec.push_back( player );
		}
	}
	
	for( std::vector<Player*>::iterator iter = temp_del_vec.begin(); iter != temp_del_vec.end(); iter++ )
	{
		handler_downtable( *iter );
		//m_bBanker = INVALID_CHAIR;
	}
	log.info("%s clean user num:%d\n", __FUNCTION__, temp_del_vec.size());
	temp_del_vec.clear();
	for( std::map<int, Player*>::iterator iter = m_mapTable_Player.begin(); iter != m_mapTable_Player.end(); iter++ )
	{
		if( NULL == iter->second->client )
		{   
			log.info("%s uid:%d client is null \n", __FUNCTION__, iter->second->uid);
			temp_del_vec.push_back( iter->second );
		}
		iter->second->reset();
	}
	for( std::vector<Player*>::iterator iter = temp_del_vec.begin(); iter != temp_del_vec.end(); iter++ )
	{
		log.warn("%s del_player %d\n", __FUNCTION__, (*iter)->uid);
		zjh.game->del_player( *iter );
	}
}

void Table::set_operator_timer(_uint8 ChairID, bool bDelayTimer)
{
	//操作时钟
	switch (ChairID)
	{
		case 0:
		{			
			if (!bDelayTimer)
			{
				ev_timer_stop(zjh.loop, &m_Operate_Timer0);
				ev_timer_set(&m_Operate_Timer0, m_Operate_Timer_stamp0+2, m_Operate_Timer_stamp0+2);
				ev_timer_again(zjh.loop, &m_Operate_Timer0);
			}
			else
			{   
				ev_timer_stop(zjh.loop, &m_Operate_Timer0);
				ev_timer_set(&m_Operate_Timer0, m_bDelayTime, m_bDelayTime);
				ev_timer_again(zjh.loop, &m_Operate_Timer0);
				log.info("operate timer m_bDelayTime:%d \n", m_bDelayTime);
			}

			m_CurTimer = m_Operate_Timer0;
		}
		break;
		case 1:
		{			
			if (!bDelayTimer)
			{
				ev_timer_stop(zjh.loop, &m_Operate_Timer1);
				ev_timer_set(&m_Operate_Timer1, m_Operate_Timer_stamp1+2, m_Operate_Timer_stamp1+2);
				ev_timer_again(zjh.loop, &m_Operate_Timer1);
			}
			else
			{   
				ev_timer_stop(zjh.loop, &m_Operate_Timer1);
				ev_timer_set(&m_Operate_Timer1, m_bDelayTime, m_bDelayTime);
				ev_timer_again(zjh.loop, &m_Operate_Timer1);
				log.info("operate timer m_bDelayTime:%d \n", m_bDelayTime);
			}
			m_CurTimer = m_Operate_Timer1;
		}
		break;
		case 2:
		{			
			if (!bDelayTimer)
			{
				ev_timer_stop(zjh.loop, &m_Operate_Timer2);
				ev_timer_set(&m_Operate_Timer2, m_Operate_Timer_stamp2+2, m_Operate_Timer_stamp2+2);
				ev_timer_again(zjh.loop, &m_Operate_Timer2);
			}
			else
			{   
				ev_timer_stop(zjh.loop, &m_Operate_Timer2);
				ev_timer_set(&m_Operate_Timer2, m_bDelayTime, m_bDelayTime);
				ev_timer_again(zjh.loop, &m_Operate_Timer2);
				log.info("operate timer m_bDelayTime:%d \n", m_bDelayTime);
			}
			m_CurTimer = m_Operate_Timer2;
		}
		break;
		case 3:
		{			
			if (!bDelayTimer)
			{
				ev_timer_stop(zjh.loop, &m_Operate_Timer3);
				ev_timer_set(&m_Operate_Timer3, m_Operate_Timer_stamp3+2, m_Operate_Timer_stamp3+2);
				ev_timer_again(zjh.loop, &m_Operate_Timer3);
			}
			else
			{   
				ev_timer_stop(zjh.loop, &m_Operate_Timer3);
				ev_timer_set(&m_Operate_Timer3, m_bDelayTime, m_bDelayTime);
				ev_timer_again(zjh.loop, &m_Operate_Timer3);
				log.info("operate timer m_bDelayTime:%d \n", m_bDelayTime);
			}
			m_CurTimer = m_Operate_Timer3;
		}
		break;
		default:
		break;
	}
}


void Table::stop_operator_timer(_uint8 ChairID)
{
	switch (ChairID)
	{
		case 0:
		ev_timer_stop(zjh.loop, &m_Operate_Timer0);
		break;
		case 1:
		ev_timer_stop(zjh.loop, &m_Operate_Timer1);
		break;
		case 2:
		ev_timer_stop(zjh.loop, &m_Operate_Timer2);
		break;
		case 3:
		ev_timer_stop(zjh.loop, &m_Operate_Timer3);
		break;
		default:
		break;
	}
}

void Table::set_out_timer(_uint8 ChairID, bool bDelayTimer)
{
	//log.info("start player uid:%d chairid:%d out card timer  \n", GetPlayerUid(ChairID), ChairID);
	if (!bDelayTimer)
	{
		ev_timer_stop(zjh.loop, &m_Outcard_Timer);
		ev_timer_set(&m_Outcard_Timer, m_Outcard_Timer_stamp+2 , m_Outcard_Timer_stamp+2);
		ev_timer_again(zjh.loop, &m_Outcard_Timer);
	}
	else
	{   
		ev_timer_stop(zjh.loop, &m_Outcard_Timer);
		ev_timer_set(&m_Outcard_Timer, m_bDelayTime, m_bDelayTime);
		ev_timer_again(zjh.loop, &m_Outcard_Timer);
		log.info("outcard timer m_bDelayTime:%d \n", m_bDelayTime);
	}
	m_CurTimer = m_Outcard_Timer;
}

void Table::Broadcast_UpdateTimer(_tint8 ChairID)
{
	//广播给客户端超时后的时钟的还剩余多少秒数
	proto::game::AckUpdateTimer stUpdateTimer;
	stUpdateTimer.set_chairid(ChairID);
	int blefttime = ev_timer_remaining(zjh.loop, &m_CurTimer);
	stUpdateTimer.set_lefttime(blefttime);

	broadcast(SERVER_UPDATE_TIMER, stUpdateTimer);
	log.info("%s uid:%d chairid:%d lefttime:%d \n", __FUNCTION__, GetPlayerUid(ChairID), stUpdateTimer.chairid(), stUpdateTimer.lefttime());
	
}
int Table::get_operate_remain_time(int iOpType, bool bReconnnect)
{   
	int nLeftTime = m_Outcard_Timer_stamp;
	if (iOpType == TYPE_NULL)
	{
		if (!bReconnnect)
		{
			nLeftTime = m_Outcard_Timer_stamp;
		}
		else
		{
			nLeftTime = ev_timer_remaining(zjh.loop, &m_CurTimer);
			if (nLeftTime >= m_Outcard_Timer_stamp)
			{
				nLeftTime = m_Outcard_Timer_stamp;
			}
			if (nLeftTime < 0)
			{
				nLeftTime = 0;
			}
			
		}
	}
	else
	{
		if (!bReconnnect)
		{
			nLeftTime = m_Operate_Timer_stamp0;
		}
		else
		{
			nLeftTime = ev_timer_remaining(zjh.loop, &m_CurTimer);
			if (nLeftTime >= m_Operate_Timer_stamp0)
			{
				nLeftTime = m_Operate_Timer_stamp0;
			}
			if (nLeftTime < 0)
			{
				nLeftTime = 0;
			}	
		}
	}
	return nLeftTime;
}
//手牌为14张牌的时候的判断听牌
int Table::GetListeningInfo(_uint8 ChairID, tagListenInfos stListenInfo[14], _uint8& bListenLen)
{
	HandCards tempHandCard = m_HandCard[ChairID];

	_uint8 bCard = 0xFF;
	memset(stListenInfo, 0, sizeof(tagListenInfos) * 14);
	bListenLen = 0;	
	
	for (int i=0; i<tempHandCard.ChangeableCardsLen; i++)
	{   
		//防止手牌有有对、刻字时，连续对相同的牌做多次的判断胡牌
		if (bCard == tempHandCard.ChangeableCards[i])
		{
			continue;
		} 
		bCard = tempHandCard.ChangeableCards[i];
		Check_Listening(&tempHandCard, ChairID, bCard, stListenInfo, bListenLen);
	}

	return (bListenLen != 0 ? TYPE_LISTEN : TYPE_NULL);
}

void Table::GetHandCardRelativity(_uint8 bCard, MJ_BUFFER<_uint8, POOL_CARD_TYPE_MAX>& _relativity)
{
	_uint8 _start, _end;
	if ((CardType(bCard) == MJ_TYPE_FENG) || (CardType(bCard) == MJ_TYPE_JIAN))
	{
		//字牌
		_start = _end = bCard;
	}
	else
	{
		//玩条同
		if (CardNum(bCard) == 1)
		{
			_start = bCard;
			_end = bCard + 1;
		}
		else if (CardNum(bCard) == 9)
		{
			_start = bCard - 1;
			_end = bCard;
		}
		else
		{
			_start = bCard - 1;
			_end = bCard + 1;
		}
	}
	
	for (_uint8 i = _start; i <= _end; ++i)
	{
		if (_relativity.Size() >= 2)
		{
			if ((_relativity[_relativity.Size() - 1] != i) && (_relativity[_relativity.Size() - 2] != i))
			{
				_relativity.Add(i);
			}
		}
		else if (_relativity.Size() == 1)
		{
			if (_relativity[0] != i)
			{
				_relativity.Add(i);
			}
		}
		else
		{
			_relativity.Add(i);
		}

	}
}

void Table::GetAllHandCardRelativity(HandCards *pHc, MJ_BUFFER<_uint8, POOL_CARD_TYPE_MAX>& _relativity)
{
	_uint8 last_check = INVALID_MJ_CARD;
	
	for (int i = 0; i < pHc->ChangeableCardsLen; ++i)
	{
		if (last_check == pHc->ChangeableCards[i])
		{
			continue;
		}
		last_check = pHc->ChangeableCards[i];
		GetHandCardRelativity(last_check, _relativity);
	}
}

void Table::Check_Listening(HandCards *pHc, _uint8 ChairID, _uint8 bCard, tagListenInfos stListenInfo[14], _uint8 &bListenLen)
{
	HandCards tempHandCard = *pHc;

	m_GameLogic.ChuPai(&tempHandCard, bCard);
	bool bTrue = false;

	MJ_BUFFER<_uint8, POOL_CARD_TYPE_MAX> _relativity;
	GetAllHandCardRelativity(&tempHandCard, _relativity);

	for (_uint8 i = 0; i< _relativity.Size(); i++)
	{
		m_GameLogic.AddCard(&tempHandCard, _relativity[i]);
		MJ_fan_buffer fan_buffer;
		if (Check_Hu(ChairID, tempHandCard, fan_buffer) == TYPE_HU)
		{			
			if (stListenInfo[bListenLen].bHuLen < POOL_CARD_TYPE_MAX)
			{				
				stListenInfo[bListenLen].bOutCard = bCard;//要打出的牌       
				stListenInfo[bListenLen].HuInfo[stListenInfo[bListenLen].bHuLen].bCard = _relativity[i];//听的牌
				stListenInfo[bListenLen].HuInfo[stListenInfo[bListenLen].bHuLen].bFanShu = Calc_Fan(ChairID, fan_buffer);
				bTrue = true;
				stListenInfo[bListenLen].bHuLen++;
			}	
			//log.info("Check_Listening chairid:%d OutCard:0x%02x, listenLen:%d hulen:%d \n", ChairID, bCard, bListenLen+ 1, stListenInfo[bListenLen].bHuLen);
		}
		m_GameLogic.ChuPai(&tempHandCard, _relativity[i]);
	}
	m_GameLogic.AddCard(&tempHandCard, bCard);
	if (bTrue)
	{ 	
		//log.info("%s uid:%d chairid:%d outcard:0x%02x hulen:%d\n", __FUNCTION__, GetPlayerUid(ChairID), ChairID, bCard, stListenInfo[bListenLen].bHuLen);
		if (bListenLen < 14)
		{
			bListenLen++;
		}		
	}
	
}
void Table::Send_lookon_GameScene(Player *player)
{
	if (player == NULL)
	{
		log.error("%s tid:%d player is null", __FUNCTION__, tid);
		return;
	}
	int ChairID = player->seatid;	
	//log.info("%s begin uid %d chairid:%d now gamestate:%d \n", __FUNCTION__, player->uid, ChairID, m_bGameState);

	proto::game::AckGameScene stGameScene;

	Proto_GameScene_lookon(ChairID, player, stGameScene);

	unicast(SERVER_GAME_SCENE, player, stGameScene);
	//log.info("%s over uid:%d chairid:%d \n", __FUNCTION__, player->uid, player->seatid);
}

void Table::User_Offline(Player * player, bool offline)
{   
	if (NULL == player)
	{
		return;
	}	
	if (player->seatid >= 0 && player->seatid < m_GAME_PLAYER)
	{
		m_IsOffline[player->seatid] = offline;
		UserOnlineState_bc(player->seatid);
		if (m_PreOfflineState[player->seatid] != m_IsOffline[player->seatid])
		{
			UserStatusNotify(player->seatid);
		}
		m_PreOfflineState[player->seatid] = m_IsOffline[player->seatid];
	}
}
//玩家在线状态的改变
void Table::UserOnlineState_bc(_uint8 ChairID)
{
	proto::game::AckUserOnlineState stAck;
	stAck.Clear();
	stAck.set_chairid(ChairID);
	_uint8 state = m_IsOffline[ChairID] ? 1 : 0;
	stAck.set_state(state);

	broadcast(SERVER_USER_OFFLINE_BC, stAck);
}
void Table::UserOvertime_State(_uint8 ChairID)
{
	proto::game::AckUserOvertimeState stAck;
	stAck.Clear();
	stAck.set_chairid(ChairID);
	stAck.set_state(m_bOverTimeState[ChairID]);
	
	broadcast(SERVER_OVERTIME_USER, stAck);
}
char* Table::CatHandCard(const HandCards& pHc, _uint8 ChairID)
{
	static char retBuf[0x100];
	retBuf[0] = '\0';
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		char tmp[0x10] = { 0 };
		sprintf(tmp, "%02x,", pHc.ChangeableCards[i]);
		strcat(retBuf, tmp);
	}
	return retBuf;
}

void Table::Send_Hand_Card()
{  
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		//庄家起手14张
		if (m_conf.m_bBankerFirst14 && i == m_bBanker)
		{
			m_HandCard[i].ChangeableCardsLen = m_MaxHandCardSize;
			m_CardPool.GetCard(m_HandCard[i].ChangeableCards, m_HandCard[i].ChangeableCardsLen);
			m_bCurCard = m_HandCard[i].ChangeableCards[m_HandCard[i].ChangeableCardsLen - 1];
		}
		else
		{
			m_HandCard[i].ChangeableCardsLen = m_MaxHandCardSize - 1;
			m_CardPool.GetCard(m_HandCard[i].ChangeableCards, m_HandCard[i].ChangeableCardsLen);
		}
	}

	//给客户端发牌
	proto::game::AckFirstHandCard stFirsthandcard;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stFirsthandcard.Clear();
		proto::game::HandCards *pstHc = stFirsthandcard.mutable_usercard();
		pstHc->set_changeablecardslen(m_HandCard[i].ChangeableCardsLen);
		for (int j=0; j<m_HandCard[i].ChangeableCardsLen; j++)
		{
			pstHc->add_changeablecards(m_HandCard[i].ChangeableCards[j]);		
		}
		for (int k = 0; k < m_GAME_PLAYER; k++)
		{
			stFirsthandcard.add_handcardnums(m_HandCard[k].ChangeableCardsLen);
		}
		//庄家起手14张
		if (m_conf.m_bBankerFirst14 && i == m_bBanker)
		{
			stFirsthandcard.set_first_get_card(m_bCurCard);
		}
        stFirsthandcard.set_voice_room(m_voice_room);
        if (m_voice_room)
        {
            stFirsthandcard.set_public_channel_id(m_public_channel_id);
            for (std::vector<string>::const_iterator it = m_group_channel_ids.begin(); 
            	it != m_group_channel_ids.end(); it++)
            {
                stFirsthandcard.add_group_channel_ids(*it);
            }
        }
        stFirsthandcard.set_cardnum(m_CardPool.Count());
		stFirsthandcard.set_md5(GetMd5(&m_HandCard[i]));		
		unicast(SERVER_FIRST_HANDCARD, i, stFirsthandcard);

		log.info("%s tid:%d first uid:%d chairid:%d [%s]\n", __FUNCTION__, tid, GetPlayerUid(i), i, CatHandCard(m_HandCard[i], i));
	}

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		m_GameLogic.SortCard(m_HandCard[i].ChangeableCards, m_HandCard[i].ChangeableCardsLen);
	}
	save_video_firsthand_card();
	
}

void Table::Proto_GameEndInfo(const MJ_user_buffer& UserID, proto::game::AckGameEnd& stGameend)
{

}

void Table::Proto_GameEndInfo_AddFan(const MJ_user_buffer& UserID, proto::game::AckGameEnd& stGameend)
{
	proto::game::FanInfo* info[GAME_PLAYER];
	for (_uint8 i = 0; i < m_GAME_PLAYER; i++)
	{
		info[i] = stGameend.add_faninfo();
	}	

	proto::game::AddFanInfo* pfan = NULL;	
	if (UserID.Size() == 1)
	{//一个人胡
		_uint8 win_seat = UserID[0];

		if (m_stHuScore[win_seat].stHu[0].bType == HU_ZIMO)
		{//自摸胡
			info[win_seat]->set_chairid(win_seat);
			info[win_seat]->set_specialtype(m_oFan[win_seat].Find(MJ_FAN_TYPE_GANGSHANGHUA) ? HU_GANG_SHANG_HUA : HU_ZIMO);
			info[win_seat]->set_fannum(m_HuFanNums[win_seat]);
			info[win_seat]->set_hucard(m_stHuScore[win_seat].stHu[0].bHuCard);

			for (int i = 0; i < m_oFan[win_seat].Size(); i++)
			{
				pfan = info[win_seat]->add_addfan();
				pfan->set_addtype(m_oFan[win_seat][i]);
				pfan->set_addnum(m_ft2fn->GetFan(m_oFan[win_seat][i]));
			}
			for (_uint8 c = NextChair(win_seat); c != win_seat; c = NextChair(c))
			{
				info[c]->set_chairid(c);
				info[c]->set_specialtype(HU_BEIZIMO);
				info[c]->set_fannum(0);
				info[c]->set_hucard(0);
				//info[c]->set_fantype(0);				
			}
		}
		else if (m_stHuScore[win_seat].stHu[0].bType == HU_PAOHU)
		{//点炮 或者抢杠胡
			info[win_seat]->set_chairid(win_seat);
			info[win_seat]->set_specialtype(m_bRobHu ? HU_QIANG_GANG : HU_PAOHU);
			info[win_seat]->set_fannum(m_HuFanNums[win_seat]);
			info[win_seat]->set_hucard(m_stHuScore[win_seat].stHu[0].bHuCard);
			//info[win_seat]->set_fantype(m_HuFanType[win_seat]);

			for (int i = 0; i < m_oFan[win_seat].Size(); i++)
			{
				pfan = info[win_seat]->add_addfan();
				pfan->set_addtype(m_oFan[win_seat][i]);
				pfan->set_addnum(m_ft2fn->GetFan(m_oFan[win_seat][i]));
			}

			for (_uint8 c = NextChair(win_seat); c != win_seat; c = NextChair(c))
			{
				info[c]->set_chairid(c);
				info[c]->set_specialtype(HU_INVALID);
				info[c]->set_fannum(0);
				info[c]->set_hucard(0);
				//info[c]->set_fantype(0);

			}
			info[m_stHuScore[win_seat].stHu[0].bPasHuChairID]->set_specialtype(m_bRobHu ? HU_BEI_QIANG_GANG : HU_DIANPAO);
		}
	}
	else if (UserID.Size() > 1)
	{//一炮多响
		for (_uint8 i = 0; i < m_GAME_PLAYER; i++)
		{
			if (UserID.Find(i))
			{

				info[i]->set_chairid(i);
				info[i]->set_specialtype(m_bRobHu ? HU_QIANG_GANG : HU_PAOHU);
				info[i]->set_fannum(m_HuFanNums[i]);
				info[i]->set_hucard(m_stHuScore[i].stHu[0].bHuCard);

				for (int j = 0; j < m_oFan[i].Size(); j++)
				{
					pfan = info[i]->add_addfan();
					pfan->set_addtype(m_oFan[i][j]);
					pfan->set_addnum(m_ft2fn->GetFan(m_oFan[i][j]));
				}
			}
			else
			{
				info[i]->set_chairid(i);
				info[i]->set_specialtype(HU_INVALID);
				info[i]->set_fannum(0);
				info[i]->set_hucard(0);
				//info[i]->set_fantype(0);
			}

		}
		_uint8 dianpaoID = m_stHuScore[UserID[0]].stHu[0].bPasHuChairID;
		info[dianpaoID]->set_specialtype(m_bRobHu ? HU_BEI_QIANG_GANG : HU_DIANPAO);
	}
	else
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			info[i]->set_chairid(i);
			info[i]->set_specialtype(HU_INVALID);
			info[i]->set_fannum(0);
			info[i]->set_hucard(0);
		}
	}
}

void Table::Proto_GameEndInfo_GangInfo(_uint8 ChairID, proto::game::AckGameEnd& stGameend)
{
	proto::game::tagEndGangInfo* pEndGanginfo = stGameend.add_endganginfo();

	for (int i = 0; i < m_stGang[ChairID].Gang.Size(); i++)
	{
		const tagGangScore& t = m_stGang[ChairID].Gang[i];

		proto::game::tagEndGang *pGang = pEndGanginfo->add_gang();
		pGang->set_gangtype(t.iType);
		pGang->set_gangcard(t.Card);

	}
	for (int i = 0; i < m_GAME_PLAYER; i++)//添加点杠信息
	{
		for (int j = 0; j < m_stGang[i].Gang.Size(); j++)
		{
			const tagGangScore& t = m_stGang[i].Gang[j];
			if (t.iType == TYPE_ZHIGANG && t.ChairID == ChairID)
			{
				proto::game::tagEndGang* pGang = pEndGanginfo->add_gang();
				pGang->set_gangtype(64);//点杠
				pGang->set_gangcard(t.Card);

			}
		}
	}

	pEndGanginfo->set_totalgangscore(m_stGang[ChairID].iTotalScore);
}

void Table::save_video_palyer_info()
{
	proto::login::AckTableInfo stTableinfo;
	stTableinfo.Clear();
	stTableinfo.set_seatid(-1);
	stTableinfo.set_roomgolden(roomGold);

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player)
		{   
			proto::login::tagPlayers *paddPlayer = stTableinfo.add_players();
			paddPlayer->set_uid(m_SeatPlayer[i].player->uid);
			paddPlayer->set_seatid(m_SeatPlayer[i].player->seatid);
			_uint8 bReady = (m_SeatPlayer[i].action == proto::game::ACTION_READY ? 1:0);
			paddPlayer->set_ready(bReady);
			paddPlayer->set_money(m_SeatPlayer[i].player->money);
			if (m_SeatPlayer[i].player->name != "")
			{
				paddPlayer->set_name(m_SeatPlayer[i].player->name.c_str());
			}
			if (m_SeatPlayer[i].player->avatar != "")
			{
				paddPlayer->set_avatar(m_SeatPlayer[i].player->avatar.c_str());
			}			
			paddPlayer->set_sex(m_SeatPlayer[i].player->sex);

			std::map<int, tagPlayerGPS>::iterator iter = m_mapPlayerGPS.find(m_SeatPlayer[i].player->uid);
			if (iter != m_mapPlayerGPS.end())
			{
				proto::login::AckGPS* pgps = paddPlayer->mutable_gps();
				pgps->set_uid(m_SeatPlayer[i].player->uid);
				pgps->set_latitude(iter->second.latitude);
				pgps->set_longitude(iter->second.longitude);
				pgps->set_city(iter->second.city);
			}
		}	
	}
	stTableinfo.set_cardpoolnums(m_CardPool.MaxCount());
	if (m_Start_Shai.Num > 0)
	{
		stTableinfo.add_touziinfo(m_Start_Shai.Shai[0]);
		stTableinfo.add_touziinfo(m_Start_Shai.Shai[1]);
	}
	stTableinfo.set_gcrdfrmlstnum(m_CardPool.GetCardCount_FromBack());
	stTableinfo.set_game_player(m_GAME_PLAYER);

	std::string str = stTableinfo.SerializeAsString();
	save_video_data(SERVER_TABLE_INFO_UC, str);
}

void Table::Proto_GameScene_Info(proto::game::AckGameScene& stGameScene)
{
	stGameScene.set_outcardtime(m_Outcard_Timer_stamp);
	stGameScene.set_operatetime(m_Operate_Timer_stamp0);
	stGameScene.set_delaytime(m_bDelayTime);
	stGameScene.set_basescore(m_BaseScore);
	stGameScene.set_scenestatus(m_bGameState);
	stGameScene.set_curoutcarduser(m_bOutCardChairID);
	stGameScene.set_curoutcard(m_bCurOutCard);
	stGameScene.set_bankerchairid(m_bBanker);
	stGameScene.set_stand_money(stand_money);

	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		stGameScene.add_userchangemode(m_UserChooseMode[i]);
	}

	Proto_GameScene_lookon(INVALID_CHAIRID, NULL, stGameScene);

	for (int i = 0; i < m_room_config.Size(); i++)
	{
		stGameScene.add_roomconfigid(m_room_config[i]);
	}
}

void Table::save_game_scene_info()
{
	proto::game::AckGameScene stGameScene;

	Proto_GameScene_Info(stGameScene);

	SaveVideoData(SERVER_GAME_SCENE, stGameScene);
}

void Table::save_video_gamestar()
{
	proto::game::AckGameStart stGamestart;
	stGamestart.set_chairid(m_bBanker);
	if (m_Start_Shai.Num > 0)
	{
		stGamestart.add_touziinfo(m_Start_Shai.Shai[0]);
		stGamestart.add_touziinfo(m_Start_Shai.Shai[1]);
	}
	stGamestart.set_totalcount(m_nTotalCount);
	stGamestart.set_currentcount(m_nCurrentCount);
	std::string str = stGamestart.SerializeAsString();
	save_video_data(SERVER_GAME_START, str);
}

void Table::save_video_firsthand_card()
{  
	proto::game::VideoFirstHandCard FirstHandCard;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::HandCards *pHandCard = FirstHandCard.add_usercard();
		if (pHandCard)
		{
			pHandCard->set_changeablecardslen(m_HandCard[i].ChangeableCardsLen);
			for (int j = 0; j < m_HandCard[i].ChangeableCardsLen; j++)
			{
				pHandCard->add_changeablecards(m_HandCard[i].ChangeableCards[j]);
			}
		}
	}
	
	std::string str = FirstHandCard.SerializeAsString();
	save_video_data(SERVER_VIDEO_FIRST_HANDCARD, str);
}

void Table::save_user_get_card(_uint8 chairID, _uint8 cbCard, int nLeftCardNum, bool bGetLast)
{
	proto::game::AckUserGetCard getCard;
	getCard.Clear();
	getCard.set_card(cbCard);
	getCard.set_chairid(chairID);
	getCard.set_leftcardnum(nLeftCardNum);
	getCard.set_optype(m_bUserOperate[chairID]);
	getCard.set_getcarddir(bGetLast ? 1 : 0);
	std::string str = getCard.SerializeAsString();
	save_video_data(SERVER_USER_GETCARD, str);
}

void Table::save_user_out_card(_uint8 chairID, _uint8 cbCard)
{
	proto::game::AckUserOutCard stUseroutcard;
	stUseroutcard.Clear();
	stUseroutcard.set_chairid(chairID);
	stUseroutcard.set_card(cbCard);

	std::string str = stUseroutcard.SerializeAsString();
	save_video_data(SERVER_USER_OUTCARD, str);
}

void Table::save_video_hu(_uint8 chairID, _uint8 paschairID, int score[GAME_PLAYER])
{
	proto::game::AckUserHu stHu;
	stHu.set_huchairid(chairID);
	stHu.set_pashuchairid(paschairID);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stHu.add_score(score[i]);
	}
	std::string str = stHu.SerializeAsString();
	save_video_data(SERVER_USER_HU, str);
}

void Table::save_video_operate(int iOpType, _uint8 chairID, _uint8 paschairID, _uint8 cbCard, int score[GAME_PLAYER])
{  
	proto::game::AckBcOpResult stOpresult;
	stOpresult.set_chairid(chairID);
	stOpresult.set_outcardchairid(paschairID);
	stOpresult.set_optype(iOpType);
	stOpresult.set_card(cbCard);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stOpresult.add_score(score[i]);
	}
	for (size_t i = 0; i < m_vecOpCards.size(); ++i)
	{
		stOpresult.add_opcards(m_vecOpCards[i]);
	}
	stOpresult.set_passhu(m_PassHuOperate);		
	std::string str = stOpresult.SerializeAsString();
	save_video_data(SERVER_OPERATE_RESULT, str);
}

void Table::save_video_operate2(const proto::game::AckBcOpResult& stOpresult)
{
	std::string str = stOpresult.SerializeAsString();
	save_video_data(SERVER_OPERATE_RESULT, str);
}

void Table::save_operate_notify(const proto::game::AckOpNotify& stNotify)
{
	std::string str = stNotify.SerializeAsString();
	save_video_data(SERVER_OPERATE_NOTIFY, str);		
}

void Table::save_video_game_end(const proto::game::AckGameEnd& stGameEnd)
{
	std::string str = stGameEnd.SerializeAsString();
	save_video_data(SERVER_GAME_END, str);
}

void Table::SaveEndRecordVideo()
{
	proto::game::GameEndRecord ger;		
	Proto_GameEnd_ScoreInfo(NULL, ger);
	SaveVideoData(SERVER_GAME_RECORD, ger);
}

void Table::save_video_data(int cmd, const std::string& str, int nlen)
{   
	std::string strcmd;
	char b[64] = {0};
	sprintf(b, "%d:S:", cmd);
	std::string aaa(b,strlen(b));
	strcmd = b;
	m_videostr += strcmd;
	m_videostr += str;
	m_videostr.append(":E:");
	
}

void Table::Send_Video_Data_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{   
	Table *table = (Table*) w->data;
	ev_timer_stop(zjh.loop, &table->send_videodata_timer);
	table->send_video_to_svr();
}

void Table::send_video_to_svr()
{
	if (m_videostr.size() == 0)
	{
		return;
	}

	unsigned char buf[MaxVideoBufLen] ={0};
	int noutlen = MaxVideoBufLen;	
	int iRet = base64_encode((const unsigned char *)m_videostr.c_str(),m_videostr.size(),(unsigned char *)buf, &noutlen);
	if (iRet != CRYPT_OK)
	{
		log.error("send_video_to_svr tid:%d base64_encode is error code:%d", tid, iRet);
		return;
	}
	
	Jpacket packet_data;
	packet_data.val["cmd"] = SERVER_VIDEO_DATA_SAVE;
	packet_data.val["roomid"] = tid;
	std::string packStr((char *)buf, noutlen);
	packet_data.val["content"] = packStr;

	char strbuf[50] = { 0 };
	sprintf(strbuf, "%d_%04d", tid, m_nPlayingTimes);
	std::string strTimes = strbuf;

	packet_data.val["innings"] = strTimes;
	packet_data.end();
	zjh.game->send_to_videosvr(packet_data.tostring());	

	m_videostr.clear();
}

void Table::reset_disbandcount()
{
	for( std::map<int, Player*>::iterator iter = m_mapTable_Player.begin(); iter != m_mapTable_Player.end(); iter++ )
	{
		if( iter->second )
		{   
			iter->second->DisBandCount = 0;
		}
	}
}

bool Table::check_roomowner_disband_table(int uid, int roomstatus)
{
	if (m_bGameState == GAME_FREE && roomstatus == 0)
	{
		proto::game::AckDisbandResult stAckResult;		
		stAckResult.set_result(1);
		broadcast(SERVER_DISBAND_RESULT, stAckResult);

		log.info("%s tid:%d uid:%d roomstatus:%d .\n", __FUNCTION__, tid, uid, roomstatus);

		m_bIsDisbandTable = false;

		//发送解散命令给后台
		Send_DisbandTableMessage();

		memset(m_DisbandChoose, proto::game::DISBAND_WAIT, sizeof(m_DisbandChoose));//初始化玩家选择
		return true;
	}
	return false;
}

void Table::send_talbe_no_user()
{
	Jpacket req_packet;
	req_packet.val["cmd"] = SERVER_ALL_PLAYER_DOWN;
	req_packet.val["vid"] = vid;

	char strroomid[64] = { 0 };
	sprintf(strroomid, "%d", tid);

	req_packet.val["roomid"] = strroomid;
	req_packet.end();
	
	zjh.game->send_to_datasvr(req_packet.tostring());
}

bool Table::check_all_table_user_offline()
{
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (!m_IsOffline[i])
		{
			return false;
		}
	}
	return true;
}

void Table::UserOutCardTimeOut(_uint8 ChairID)
{
	if (m_conf.m_bAutoDisband)
	{
		TIMER_STOP(m_Auto_Disband_Timer);
		TIMER_AGAIN_s(m_Auto_Disband_Timer);
		m_AutoDisband = proto::game::OVER_TIME_OUTCARD;
	}
}

void Table::UserChooseOperateTimeOut(_uint8 ChairID)
{
	if (m_conf.m_bAutoDisband)
	{
		TIMER_STOP(m_Auto_Disband_Timer);
		TIMER_AGAIN_s(m_Auto_Disband_Timer);
		m_AutoDisband = proto::game::OVER_TIME_OPERATE;
	}
}

std::string Table::GetMd5(const HandCards* pHc)
{
	HandCards tmp = *pHc;

	m_GameLogic.SortCard(tmp.ChangeableCards, tmp.ChangeableCardsLen);

	std::ostringstream oss;
	oss << "__beauty__";
	static char retBuf[0x100];
	retBuf[0] = '\0';
	for (int i = 0; i < tmp.ChangeableCardsLen; i++)
	{
		char tmpbuf[0x10] = { 0 };
		sprintf(tmpbuf, "%d", tmp.ChangeableCards[i]);
		strcat(retBuf, tmpbuf);
	}
	oss << retBuf;
	oss << "??wangsiying??";

	std::string md5;
	MD5::MD5Str(oss.str(), md5);

	//log.debug("%s oss:%s md5:%s .\n", __FUNCTION__, oss.str().c_str(), md5.c_str());

	return md5;
}

bool Table::Check_OperateInfo(_uint8 ChairID, int iOpType, int iCard, int iOpId, std::vector<int> vecOpCards)
{
	if (iOpId != 0 && iOpId != m_UserOperateID[ChairID])
	{
		log.error("%s tid:%d ChairID:%d iOpId:%d m_UserOperateID:%d .\n", __FUNCTION__, tid, ChairID, iOpId, m_UserOperateID[ChairID]);
		if (m_SeatPlayer[ChairID].player != NULL)
		{
			handler_table_info(m_SeatPlayer[ChairID].player);	
			Send_Game_Scene(m_SeatPlayer[ChairID].player);
		}
		return false;
	}

	return true;
}

void Table::Stop_Auto_Disband()
{
	if (m_conf.m_bAutoDisband)
	{
		if (m_AutoDisband == proto::game::OVER_TIME_OPERATE)
		{
			int iCounts = 0;
			for (int i = 0; i < m_GAME_PLAYER; ++i)
			{
				if (m_bUserOperate[i] & (TYPE_CHI_PENG_GANG | TYPE_HU))
					iCounts++;
			}

			if (iCounts == 0)
			{
				TIMER_STOP(m_Auto_Disband_Timer);
				m_AutoDisband = proto::game::OVER_TIME_NO;	
			}
		}
		else
		{
			TIMER_STOP(m_Auto_Disband_Timer);
			m_AutoDisband = proto::game::OVER_TIME_NO;	
		}
	}
}

void Table::Auto_Disband_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	log.info("%s .\n", __FUNCTION__);
	Table *table = (Table*)w->data;
	if (table)
	{		
		TIMER_STOP(table->m_Auto_Disband_Timer);
		table->Auto_Disband_Table();
	}
}

void Table::Auto_Disband_Table()
{
	m_bIsAutoDisband = true;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_DisbandChoose[i] == proto::game::DISBAND_WAIT)
		{
			RecvDisbandTableChoose(i, proto::game::DISBAND_YES);				
		}
	}
	m_bIsAutoDisband = false;

	log.info("%s tid:%d Auto DisBand .\n", __FUNCTION__, tid);
}

void Table::OperateRobGang(_uint8 ChairID, _uint8 CardData, int iOpType)
{
	//执行抢杠
	m_GameLogic.ExecuteRobGang(&m_HandCard[ChairID], m_bCurChairID, CardData, iOpType);
	m_bUserOperate[ChairID] = TYPE_NULL;
	if (iOpType == TYPE_ZHIGANG)
	{
		m_stUserOutCard[m_bOutCardChairID].RemoveLast(CardData);
	}
}

void Table::RecoverRobGang(_uint8 ChairID, _uint8 CardData, int iOpType)
{
	//恢复抢杠
	m_GameLogic.RecoverRobGang(&m_HandCard[ChairID], m_bCurChairID, CardData, iOpType);
	if (m_WangGangPlayerOpType == TYPE_ZHIGANG)
	{
		m_stUserOutCard[m_bOutCardChairID].Add(m_RobWanGangCard);
	}
}

void Table::RecvClientChangeMode(Client *client, Player *player)
{
	if (!m_conf.m_bSupportChangeMode)
		return;

	_uint8 ChairID = player->seatid;

	if (ChairID >= m_GAME_PLAYER || m_room_status != 0)
	{
		log.error("%s tid:%d ChairID:%d m_room_status:%d wrong .\n", __FUNCTION__, tid, ChairID, m_room_status);
		return;
	}

	proto::game::ReqChangeMode stReq;
	stReq.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
	int num = stReq.num();
	int iSeatCount = SeatPlayerCount();

	if (num == 0) //取消
	{
		if (m_UserChooseMode[ChairID] == 0)
		{
			log.error("%s tid:%d ChairID:%d num:%d m_UserChooseMode:%d not choose.\n", __FUNCTION__, tid, ChairID, num, m_UserChooseMode[ChairID]);
			return;
		}
	}
	else
	{
		if (num < 2 || num >= m_GAME_PLAYER || num != iSeatCount || m_UserChooseMode[ChairID] != 0)
		{
			log.error("%s tid:%d ChairID:%d num:%d wrong iSeatCount:%d m_UserChooseMode:%d .\n", __FUNCTION__, tid, ChairID, num, iSeatCount, m_UserChooseMode[ChairID]);
			return;
		}

		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			if (m_UserChooseMode[i] != 0)
			{
				if (num != m_UserChooseMode[i])
				{
					log.error("%s tid:%d ChairID:%d num:%d m_UserChooseMode[%d]:%d not same.\n", __FUNCTION__, tid, ChairID, num, i, m_UserChooseMode[i]);
					return;
				}
			}
		}
	}

	m_UserChooseMode[ChairID] = num;

	proto::game::AckChangeMode stAck;
	stAck.set_chairid(ChairID);
	stAck.set_num(m_UserChooseMode[ChairID]);
	broadcast(SERVER_ACK_CHANGE_MODE, stAck);

	//勾选改变模式，则自动准备
	if (m_SeatPlayer[ChairID].action != proto::game::ACTION_READY)
	{
		User_Ready(ChairID, proto::game::ACTION_READY);
	}

	log.info("%s tid:%d ChairID:%d uid:%d m_UserChooseMode:%d iSeatCount:%d .\n", __FUNCTION__, tid, ChairID, GetPlayerUid(ChairID), m_UserChooseMode[ChairID], iSeatCount);

	int iChooseCount = 0;
	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		if (m_UserChooseMode[i] != 0)
			iChooseCount++;
	}

	if (iChooseCount == iSeatCount)
	{
		m_GAME_PLAYER = iSeatCount;

		//修改房间配置
		ChangeRoomConf();

		//重新排座位
		ResetSeats();

		log.info("%s tid:%d m_GAME_PLAYER:%d Mode Change .\n", __FUNCTION__, tid, m_GAME_PLAYER);

		int iReadyCount = 0; 
		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			if (m_SeatPlayer[i].state != Seat::E_Seat_Empty && m_SeatPlayer[i].action == ACTION_READY)
				iReadyCount++;
		}
		if (iReadyCount == m_GAME_PLAYER)
		{
			m_nPlayingTimes++;
			game_start_req();
			TIMER_AGAIN(req_start_timer, req_start_timer_stamp);
		}
	}
}

void Table::ChangeRoomConf()
{
	
}

void Table::ResetSeats()
{
	MJ_BUFFER<Seat, GAME_PLAYER> Mj_Seats;
	std::vector<int> vecModes;

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_SeatPlayer[i].state != Seat::E_Seat_Empty)
		{
			Mj_Seats.Add(m_SeatPlayer[i]);
			vecModes.push_back(m_UserChooseMode[i]);
		}
	}

	for (int i = 0; i < Mj_Seats.Size(); ++i)
	{
		m_SeatPlayer[i].clear();

		m_SeatPlayer[i] = Mj_Seats[i];

		if (m_SeatPlayer[i].player != NULL)
		{
			m_SeatPlayer[i].player->seatid = i;
		}

		m_UserChooseMode[i] = vecModes[i]; 			//位置变化，模式变化
	}

	for (int i = 0; i < Mj_Seats.Size(); ++i)
	{
		if (m_SeatPlayer[i].player != NULL)
		{
			handler_table_info(m_SeatPlayer[i].player);	
			Send_Game_Scene(m_SeatPlayer[i].player);
		}
	}
}

void Table::UserStatusNotify(_uint8 ChairID)
{
	Jpacket req_packet;
	req_packet.val["cmd"] = SERVER_USER_STATUS_NOTIFY; 
	req_packet.val["uid"] = GetPlayerUid(ChairID);
	req_packet.val["roomid"] = tid;
	req_packet.val["prepared"] = m_SeatPlayer[ChairID].action == ACTION_READY ? 1 : 0;
	req_packet.val["online"] = m_IsOffline[ChairID] ? 0 : 1;
	req_packet.end();
	zjh.game->send_to_datasvr(req_packet.tostring());
}
