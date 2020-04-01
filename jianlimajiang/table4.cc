#include "table.h"
#include "zjh.h"
#include "proto/proto.h"
#include "common/log.h"
#include "libnormalmahjong/common_mahjong_logic.h"
#include "redis_cache.h"

extern ZJH zjh;
extern Log log;

void Table::Proto_GameScene(_uint8 ChairID, Player* player, proto::game::AckGameScene& stGameScene)
{

	//log.info("%s begin to uid:%d chairid:%d gamestate:%d \n", __FUNCTION__, player->uid, ChairID, m_bGameState);
	stGameScene.set_outcardtime(m_Outcard_Timer_stamp);
	stGameScene.set_operatetime(m_Operate_Timer_stamp0);
	stGameScene.set_delaytime(m_bDelayTime);
	stGameScene.set_basescore(m_BaseScore);
	if (m_bDeleteTable)
	{
		stGameScene.set_scenestatus(10);
	}
	else
	{
		stGameScene.set_scenestatus(m_bGameState);
	}
	stGameScene.set_curoutcarduser(m_bOutCardChairID);
	stGameScene.set_curoutcard(m_bCurOutCard);
	//stGameScene.set_optype(m_bUserOperate[ChairID]);
	stGameScene.set_optype(0);
	stGameScene.set_bankerchairid(m_bBanker);
	_uint8 bIsMoPai = (ChairID == m_bCurChairID) ? 1 : 0;//µ±Ç°ÃþÅÆÍæ¼Ò
	stGameScene.set_iscurgetcard(bIsMoPai);
	stGameScene.set_card(m_bCurCard);//Ãþµ½µÄÅÆ
	_uint8 bAutoHu = (m_bAutoHu[ChairID] == true) ? 1 : 0;
	stGameScene.set_isautohu(bAutoHu);
	int nleftTime = 0;
	if (m_bUserOperate[ChairID] == TYPE_NULL || m_bUserOperate[ChairID] == TYPE_LISTEN)
	{
		nleftTime = get_operate_remain_time(TYPE_NULL, true);
		if (nleftTime > m_Outcard_Timer_stamp)
		{
			nleftTime = m_Outcard_Timer_stamp;
		}

	}
	else
	{
		nleftTime = get_operate_remain_time(TYPE_NULL, true);
		if (nleftTime > m_Operate_Timer_stamp0)
		{
			nleftTime = m_Operate_Timer_stamp0;
		}

	}
	stGameScene.set_lefttime(nleftTime);
	log.info("%s uid:%d chairid:%d left timer:%d m_bUserOperate:0x%x\n", __FUNCTION__, GetPlayerUid(player), ChairID, stGameScene.lefttime(), m_bUserOperate[ChairID]);

	stGameScene.set_stand_money(stand_money);
	stGameScene.set_poolcardleftnums(m_CardPool.Count());
	stGameScene.set_preready_timer(preready_timer_stamp);

	stGameScene.set_isdisbandroom(m_bIsDisbandTable);
	proto::game::AckDisbandApply *pDisInfo = stGameScene.mutable_disbandinfo();
	pDisInfo->set_applychairid(m_ApplyDisbandChairid);
	int leftchoosetime = Disband_timer_remaintime(ChairID);
	pDisInfo->set_waittime(leftchoosetime);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		pDisInfo->add_onlinechairid(i);
	}
	pDisInfo->set_totaltime(m_Disbandtable_Timer_stamp);

	proto::game::HandCards *pstHc = stGameScene.mutable_usercard();
	Fill_HandCard(ChairID, pstHc);

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stGameScene.add_handcardnums(m_HandCard[i].ChangeableCardsLen);
		stGameScene.add_totalscore(m_stHuScore[i].iTotalScore + m_stGang[i].iTotalScore);
		stGameScene.add_offlinestate(m_IsOffline[i]);

		proto::game::AckDisbandChooseBc *pDisChoose = stGameScene.add_userdisbandstate();
		pDisChoose->set_chairid(i);
		pDisChoose->set_choosestate(m_DisbandChoose[i]);
		proto::game::AckUserOnlineState *pOnlinestate = stGameScene.add_useronlinestate();
		pOnlinestate->set_chairid(i);
		pOnlinestate->set_state(m_IsOffline[i]);
		proto::game::AckUserOvertimeState *pOvertime = stGameScene.add_userovertime();
		pOvertime->set_chairid(i);
		pOvertime->set_state(m_bOverTimeState[i]);
		//´ò³öÅÆ
		proto::game::tagShowOutCard *pstShow = stGameScene.add_showcard();
		for (int j = 0; j < m_stUserOutCard[i].Size(); j++)
		{
			pstShow->add_outcard(m_stUserOutCard[i][j]);
		}
		pstShow->set_outcardlen(m_stUserOutCard[i].Size());
		//Åö¸ÜÅÆ
		proto::game::tagGroupCard *pstGr = stGameScene.add_groupcard();
		for (int j = 0; j < m_HandCard[i].FixedCardsLen; j++)
		{
			proto::game::FixedCard *pstFix = pstGr->add_fixedcard();
			Fill_FixedCard(i, j, pstFix);
			if (i != ChairID && m_conf.m_bAnGangNoShow && pstFix->state() == TYPE_ANGANG)
			{
				pstFix->set_carddata(0);
			}
		}
		//ºúÅÆÐÅÏ¢
		proto::game::tagUserHuInfo *pstUserHu = stGameScene.add_huinfo();
		for (int j = 0; j < m_stHuScore[i].stHu.Size(); j++)
		{
			proto::game::tagHu *pstHu = pstUserHu->add_hu();
			pstHu->set_hucard(m_stHuScore[i].stHu[j].bHuCard);
			pstHu->set_pashuchiarid(m_stHuScore[i].stHu[j].bPasHuChairID);
		}
	}

	Proto_GameScene_Operate(ChairID, player, stGameScene);

	proto::game::TotalRoundScore* trs = stGameScene.mutable_totalroundscore();
	for (size_t i = 0; i < m_totalrecordinfo.round_score.size(); i++)
	{
		proto::game::RoundScore* sc = trs->add_round();
		for (int j = 0; j < m_GAME_PLAYER; j++)
		{
			sc->add_score(m_totalrecordinfo.round_score[i].score[j]);
		}
	}
	trs->set_roundcount(m_totalrecordinfo.round_score.size());

	for (int i = 0; i < m_room_config.Size(); i++)
	{
		stGameScene.add_roomconfigid(m_room_config[i]);
	}

	stGameScene.set_md5(GetMd5(&m_HandCard[ChairID]));
	stGameScene.set_operateid(m_UserOperateID[ChairID]);

	//客户端以时间作为自动解散开关
	if (m_conf.m_bAutoDisband)
	{
		stGameScene.set_auto_disband_time(m_Auto_Disband_Timer_stamp);
		stGameScene.set_auto_disband_state(m_AutoDisband);
		if (m_AutoDisband != proto::game::OVER_TIME_NO)
		{
			if (m_AutoDisband == proto::game::OVER_TIME_OPERATE)
			{
				for (int i = 0; i < m_GAME_PLAYER; ++i)
				{
					if (m_bUserOperate[i] & (TYPE_CHI_PENG_GANG | TYPE_HU))
					{
						stGameScene.add_auto_disband_chairids(i);
					}
				}
			}
			else 
			{
				stGameScene.add_auto_disband_chairids(m_bCurChairID);
			}

			int nLeftTime = ev_timer_remaining(zjh.loop, &m_Auto_Disband_Timer);
			if (nLeftTime > m_Auto_Disband_Timer_stamp)
			{
				nLeftTime = m_Auto_Disband_Timer_stamp;
			}
			if (nLeftTime < 0)
			{
				nLeftTime = 0;
			}
			stGameScene.set_auto_disband_remaintime(nLeftTime);
		}
	}

	stGameScene.set_totalcount(m_nTotalCount);
	stGameScene.set_currentcount(m_nCurrentCount);

	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		stGameScene.add_userchangemode(m_UserChooseMode[i]);
	}

    //语音房信息
    proto::game::ExMsg *pexmsg = stGameScene.mutable_msg();
    pexmsg->set_voice_room(m_voice_room);
    if (m_voice_room)
    {
        pexmsg->set_public_channel_id(m_public_channel_id);
        for ( vector<string>::const_iterator it = m_group_channel_ids.begin() ; it != m_group_channel_ids.end() ; it++ )
        {
            pexmsg->add_group_channel_ids(*it);
        }
    }
    for ( int i = 0 ; i < m_GAME_PLAYER ; ++i  )
    {
        pexmsg->add_m_voice_status(m_voice_status[i]);
    } 

    proto::game::TestSvrConf* pb_conf = stGameScene.mutable_test_svr_conf();
    Proto_TestSvrConf(*pb_conf);
}

void Table::Proto_GameScene_Operate(_uint8 ChairID, Player* player, proto::game::AckGameScene& stGameScene)
{
	if (m_bMultiOpUserNum != 0 && m_bMultiUserDoneOp[ChairID] != TYPE_NULL)
	{
		log.info("%s user has selected uid:%d chairid:%d iOpType:0x%x!\n", __FUNCTION__, GetPlayerUid(ChairID), ChairID, m_bMultiUserDoneOp[ChairID]);
		return;
	}

	if (m_bUserOperate[ChairID] == TYPE_NULL || m_bUserOperate[ChairID] == TYPE_LISTEN)
	{
		stGameScene.set_optype(TYPE_NULL);
	}
	else
	{
		stGameScene.set_optype(m_bUserOperate[ChairID]);
	}
		
	if (m_bUserOperate[ChairID] & TYPE_ANGANG || m_bUserOperate[ChairID] & TYPE_WANGANG)
	{
		MJ_opinfo stGang;
		GetGangInfo(ChairID, stGang);
		for (int len = 0; len < stGang.Size(); len++)
		{
			proto::game::tagGangInfo *pstGang = stGameScene.add_ganginfo();
			pstGang->set_card(stGang[len].cbCard);
			pstGang->set_state(stGang[len].nState);
		}
	}

	if (m_bUserOperate[ChairID] & TYPE_LISTEN)
	{
		tagListenInfos stListenInfo[14];
		_uint8 bListenLen = 0;

		if (!(m_bUserOperate[ChairID] & TYPE_HU))
		{
			m_bUserOperate[ChairID] |= GetListeningInfo(ChairID, stListenInfo, bListenLen);
		}

		for (int j = 0; j < bListenLen; j++)
		{
			PackageTingInfo(ChairID, stListenInfo, j, stGameScene.add_listeninfo());
		}
	}
}
void Table::Proto_GameScene_lookon(_uint8 ChairID, Player* player, proto::game::AckGameScene& stGameScene)
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
	
	int nleftTime = get_operate_remain_time(TYPE_NULL, true);
	if (nleftTime > m_Operate_Timer_stamp0)
	{
		nleftTime = m_Operate_Timer_stamp0;
	}
		
	stGameScene.set_lefttime(nleftTime);
	stGameScene.set_poolcardleftnums(m_CardPool.Count());
	stGameScene.set_preready_timer(preready_timer_stamp);

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stGameScene.add_handcardnums(m_HandCard[i].ChangeableCardsLen);
		stGameScene.add_offlinestate(m_IsOffline[i]);
		//´ò³öÅÆ
		proto::game::tagShowOutCard *pstShow = stGameScene.add_showcard();
		for (int j = 0; j < m_stUserOutCard[i].Size(); j++)
		{
			pstShow->add_outcard(m_stUserOutCard[i][j]);
		}
		pstShow->set_outcardlen(m_stUserOutCard[i].Size());
		//Åö¸ÜÅÆ
		proto::game::tagGroupCard *pstGr = stGameScene.add_groupcard();
		for (int j = 0; j < m_HandCard[i].FixedCardsLen && j < 4; j++)
		{
			proto::game::FixedCard *pstFix = pstGr->add_fixedcard();
			Fill_FixedCard(i, j, pstFix);
			if (m_conf.m_bAnGangNoShow && pstFix->state() == TYPE_ANGANG)
			{
				pstFix->set_carddata(0);
			}
		}
		//ºúÅÆÐÅÏ¢
		proto::game::tagUserHuInfo *pstUserHu = stGameScene.add_huinfo();
		for (int j = 0; j < m_stHuScore[i].stHu.Size(); j++)
		{
			proto::game::tagHu *pstHu = pstUserHu->add_hu();
			pstHu->set_hucard(m_stHuScore[i].stHu[j].bHuCard);
			pstHu->set_pashuchiarid(m_stHuScore[i].stHu[j].bPasHuChairID);
		}
	}
}

void Table::Score_GameEndDraw(HuScoreSet& score, proto::game::AckGameEnd& stGameend)
{
	if (m_conf.m_bHuangGang)
	{
		memset(m_stGang, 0, sizeof(m_stGang));
	}	
}
void Table::Score_GameEndSucc(HuScoreSet& score, proto::game::AckGameEnd& stGameend)
{

	int iTotalScore[GAME_PLAYER] = { 0 };
	int iBaseScore[GAME_PLAYER][GAME_PLAYER];
	int iScore[GAME_PLAYER][GAME_PLAYER];
	memset(iBaseScore, 0, sizeof(iBaseScore));
	memset(iScore, 0, sizeof(iScore));
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		Calc_BaseScore(i, iBaseScore[i]);
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{		
		Calc_HuScore(i, iBaseScore[i], iScore[i]);		
	}
	if (m_conf.m_bCalcAllNiao)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			for (_uint8 j = 0; j < m_niao.Size(); j++)
			{
				Calc_ZhongNiao(i, m_niao[j], iBaseScore[i], iScore[i]);
			}
		}
	}
	else
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			Calc_ZhongNiao(i, INVALID_MJ_CARD, iBaseScore[i], iScore[i]);
		}
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		Calc_FengDing(i, iScore[i]);
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		Calc_Score(iTotalScore, iScore[i]);
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		score[i].iTotleScore = iTotalScore[i];
	}
}
void Table::Calc_BaseScore(_uint8 ChairID, int iBaseScore[GAME_PLAYER])
{

}
void Table::Calc_HuScore(_uint8 ChairID, int iBaseScore[GAME_PLAYER], int iScore[GAME_PLAYER])
{

}
void Table::Calc_ZhongNiao(_uint8 ChairID, _uint8 ShaiNum, int iBaseScore[GAME_PLAYER], int iScore[GAME_PLAYER])
{

}
void Table::Calc_FengDing(_uint8 ChairID, int iScore[GAME_PLAYER])
{

}
void Table::Fill_HandCard(_uint8 ChairID, proto::game::HandCards* pHandcard)
{
	pHandcard->set_changeablecardslen(m_HandCard[ChairID].ChangeableCardsLen);
	for (int j = 0; j < m_HandCard[ChairID].ChangeableCardsLen; j++)
	{
		pHandcard->add_changeablecards(m_HandCard[ChairID].ChangeableCards[j]);		
	}
	for (int j = 0; j < m_HandCard[ChairID].FixedCardsLen; j++)
	{
		proto::game::FixedCard *pFixedcard = pHandcard->add_stfixedcards();
		Fill_FixedCard(ChairID, j, pFixedcard);
	}
	pHandcard->set_fixedcardslen(m_HandCard[ChairID].FixedCardsLen);
}
void Table::Fill_FixedCard(_uint8 ChairID, _uint8 Pos, proto::game::FixedCard* pstFix)
{
	_uint8 cbCard = 0xFF;
	if (m_HandCard[ChairID].FixedCards[Pos].state == TYPE_LEFT_CHI)
	{
		cbCard = m_HandCard[ChairID].FixedCards[Pos].c[0];
	}
	else if (m_HandCard[ChairID].FixedCards[Pos].state == TYPE_CENTER_CHI)
	{
		cbCard = m_HandCard[ChairID].FixedCards[Pos].c[1];
	}
	else if (m_HandCard[ChairID].FixedCards[Pos].state == TYPE_RIGHT_CHI)
	{
		cbCard = m_HandCard[ChairID].FixedCards[Pos].c[2];
	}
	else
	{
		cbCard = m_HandCard[ChairID].FixedCards[Pos].CardData;
	}
	pstFix->set_carddata(cbCard);
	pstFix->set_state(m_HandCard[ChairID].FixedCards[Pos].state);
	pstFix->set_chairid(m_HandCard[ChairID].FixedCards[Pos].chairID);
}

int Table::MultiHu(MJ_user_buffer& id)const
{
	id.Clear();
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_bIsHu[i])
		{
			id.Add(i);
		}
	}
	return id.Size();
}

void Table::Update_Score(int iScore[GAME_PLAYER])
{
	tagUserScore userscore[GAME_PLAYER];

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player != NULL)
		{
			userscore[i].uid = m_SeatPlayer[i].player->uid;
			userscore[i].score = iScore[i];

			m_SeatPlayer[i].player->update_info(iScore[i], 0, 0, 0);

			//UpdateUserInfoToUser(m_SeatPlayer[i].player);  //客户端自己家加减
			log.fatal("%s tid:%d uid:%d chairid:%d score:%d\n", __FUNCTION__, tid, m_SeatPlayer[i].player->uid, i, iScore[i]);
		}
	}
	update_user_info_to_datasvr_dz(userscore, 1102, SERVER_DZ_UPDATE_USER_INFO_LITTLE);
	
}

void Table::Recv_DisbandTable_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table* table = (Table*)w->data;
	if (table != NULL)
	{		
		log.info("%s tid:%d, send disband message again\n", __FUNCTION__, table->tid);
		table->Send_DisbandTableMessage();
	}
}

void Table::Send_DisbandTableMessage()
{
	//·¢ËÍ½âÉ¢ÃüÁî¸øºóÌ¨
	Jpacket packet;
	packet.val["cmd"] = SERVER_DISBAND_ROOM;
	packet.val["vid"] = vid;

	char strroomid[64] = { 0 };
	sprintf(strroomid, "%d", tid);
	packet.val["roomid"] = strroomid;

	char strbuf[50] = { 0 };
	sprintf(strbuf, "%d_%04d", tid, m_nPlayingTimes);
	packet.val["innings"] = strbuf;
	packet.val["backCardInFirstCount"] = 1;
	packet.end();

	log.info("%s disband table:%d times:%s \n", __FUNCTION__, tid, strbuf);	
	zjh.game->send_to_datasvr(packet.tostring());
	
	m_bDeleteTable = true;

	TIMER_AGAIN(m_RecvDisbandTable_Timer, m_RecvDisbandTable_Timer_stamp);
}
/*
void Table::RecvClientChoosePiao(Client* client, Player *player)
{


}

void Table::StartChoosePiao()
{

}
*/
int Table::logout_room_req(Player *player)
{	
	// Èç¹ûÔÚ×ÀÉÏÏÈÈÃÆäÏÂ×À
	if (m_mapTable_Player.find(player->uid) != m_mapTable_Player.end()) {
		if (handler_downtable(player) == -1)
		{
			log.error("%s error tid:%d uid:%d seatid:%d state:%d\n", __FUNCTION__, tid, player->uid, player->seatid, player->status);			
		}
	}

	// Í¨ÖªÒµÎñÆ½Ì¨Íæ¼ÒÍË³ö·¿¼ä
	Jpacket packet;
	packet.val["cmd"] = SERVER_LOGOUT_ROOM_REQ;
	packet.val["uid"] = player->uid;
	packet.val["vid"] = zid;
	packet.val["roomid"] = tid;
	packet.end();
	zjh.game->send_to_datasvr(packet.tostring());

	log.info("%s tid:%d uid:%d seatid:%d money:%d\n", __FUNCTION__, tid, player->uid, player->seatid, player->money);

	return 0;
}
int Table::force_logout_room_req(Player *player)
{
	if (player == NULL)
	{
		return -1;
	}	
	// Í¨ÖªÒµÎñÆ½Ì¨Íæ¼ÒÍË³ö·¿¼ä
	Jpacket packet;
	packet.val["cmd"] = SERVER_FORCE_LOGOUT_ROOM_REQ;
	packet.val["uid"] = player->uid;
	packet.val["vid"] = zid;
	packet.val["roomid"] = tid;
	packet.end();
	zjh.game->send_to_datasvr(packet.tostring());

	log.info("%s tid:%d uid:%d seatid:%d money:%d\n", __FUNCTION__, tid, player->uid, player->seatid, player->money);
	return 0;
}
void Table::GameBreakOff()
{
	proto::game::AckGameEnd stGameend;
	HuScoreSet hu_score;
	// 计算中途解散的杠分
	Score_GameEndDisband(hu_score);
	Score_GameEndDraw(hu_score, stGameend);

	log.info("%s tid:%d score[%d,%d,%d,%d]\n", __FUNCTION__, tid, hu_score[0].iTotleScore, hu_score[1].iTotleScore, hu_score[2].iTotleScore, hu_score[3].iTotleScore);

	int iScore[GAME_PLAYER] = { 0 };
	for (_uint8 i = 0; i < m_GAME_PLAYER; i ++)
	{
		iScore[i] = hu_score[i].iTotleScore;
	}
	Update_Score(iScore);

	MJ_user_buffer UserID;	
	CalcRecordInfo(UserID, iScore);
	RecordScore(m_nCurrentCount);
}
bool Table::HasSeat(const Player *player)const
{	
	if (player == NULL)
	{
		return false;
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player == player)
		{
			return true;
		}
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player == NULL)
		{
			return true;
		}
	}
	return false;
}
	
_uint8 Table::SeatPlayerCount()const
{
	_uint8 iSeatCount = 0;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].state != Seat::E_Seat_Empty)
		{
			iSeatCount++;
		}
	}
	return iSeatCount;
}
void Table::User_Home_Out(Player * player)
{
	log.info("%s tid:%d uid:%d\n", __FUNCTION__, tid, GetPlayerUid(player));
}

void Table::Check_All_Offline()
{
	if (check_all_table_user_offline())
	{
		send_talbe_no_user();
	}
}

void Table::Check_Offline_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table* table = (Table*)w->data;
	if (table != NULL)
	{
		table->Check_Offline_Beat();
	}
	
}
void Table::Check_Offline_Beat()
{
	//log.info("%s \n", __FUNCTION__);	
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].state == Seat::E_Seat_Empty)
		{
			continue;
		}
		if (!m_Online_Beat[i])
		{
			User_Offline(m_SeatPlayer[i].player);
		}
		m_Online_Beat[i] = false;
	}
}
void Table::Online_Beat(Player* player)
{
	if (player->seatid >= 0 && player->seatid < m_GAME_PLAYER)
	{
		if (!m_Online_Beat[player->seatid])
		{
			User_Offline(player, false);
		}		
		m_Online_Beat[player->seatid] = true;				
	}	
}

bool Table::Is_GenZhuangMa4X(_uint8 ChairID, _uint8 niao)
{
	_uint8 dis = DisChair(m_bBanker, ChairID);
	_uint8 niao_pos =  niao % GAME_PLAYER;		
	return dis == niao_pos;
}
bool Table::Is_HuPaiMa4X(_uint8 HuId, _uint8 ChairID, _uint8 niao)
{
	_uint8 dis = DisChair(HuId, ChairID);
	_uint8 niao_pos = niao % GAME_PLAYER;	
	return dis == niao_pos;
}

void Table::PackageTingInfo(_uint8 ChairID, tagListenInfos stListenInfo[14], _uint8 iPos, proto::game::tagListenInfo* listeninfo)
{
	listeninfo->set_outcard(stListenInfo[iPos].bOutCard);
	for (int k = 0; k < stListenInfo[iPos].bHuLen; k++)
	{
		proto::game::tagHuInfo *pstHuinfo = listeninfo->add_huinfo();
		_uint8 bListenCard = stListenInfo[iPos].HuInfo[k].bCard;
		pstHuinfo->set_card(bListenCard);
		pstHuinfo->set_hufannum(stListenInfo[iPos].HuInfo[k].bFanShu);
		int nCardLeftNum = Get_Card_Left_Num(ChairID, bListenCard);
		pstHuinfo->set_leftnum(nCardLeftNum);
	}
}

void Table::DoMultiForceHu(_uint8 ChairID, _uint8 bCard)
{
	m_bRecordMultiOp.iOpType = TYPE_HU;
	m_bRecordMultiOp.ChairID = ChairID;
	m_bRecordMultiOp.bCard = bCard;
	m_bMultiDoneOpNum = m_bMultiOpUserNum;

	for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
	{
		if (c == m_bCurChairID)
			continue;
		if (m_bUserOperate[c] & TYPE_HU || m_PassHuPlayer.Find(c))
		{
			m_louhu[c].Unlock();

			m_bMultiUserDoneOp[c] = TYPE_HU;
			m_bMultiUserDoneOpCard[c] = bCard;

			m_bUserOperate[c] |= TYPE_HU;
			UserOperateHu(c, bCard);
			stop_operator_timer(c);
		}
	}
}
void Table::RobGangForceHu(_uint8 ChairID, _uint8 bCard)
{
	m_RobOperateNum = m_RobHuNum;

	for (_uint8 c = NextChair(ChairID); c != ChairID; c=NextChair(c))
	{
		if (c == m_WangGangPlayer)
			continue;
		if (m_bUserOperate[c] & TYPE_HU || m_PassHuPlayer.Find(c))
		{
			m_louhu[c].Unlock();

			m_bUserOperate[c] |= TYPE_HU;
			UserOperateHu(c, bCard);
			stop_operator_timer(c);
		}		
	}	
}


void Table::CalcRecordInfo(const MJ_user_buffer& UserID, int iTotalScore[GAME_PLAYER])
{
	if (m_bBanker < m_GAME_PLAYER)
	{
		m_totalrecordinfo.banker[m_bBanker]++;
	}
	if (UserID.Size() == 1)
	{
		_uint8 ChairID = UserID[0];
		if (m_stHuScore[ChairID].stHu[0].bType == HU_ZIMO)
		{
			m_totalrecordinfo.hu[ChairID]++;
			m_totalrecordinfo.zimo[ChairID]++;
		}
		else
		{
			m_totalrecordinfo.hu[ChairID]++;
			m_totalrecordinfo.paohu[ChairID]++;

			_uint8 PaoID = m_stHuScore[ChairID].stHu[0].bPasHuChairID;
			m_totalrecordinfo.dianpao[PaoID]++;
		}

	}
	else if (UserID.Size() > 1)
	{
		for (int i = 0; i < UserID.Size(); i++)
		{
			_uint8 ChairID = UserID[i];
			if (m_stHuScore[ChairID].stHu[0].bType == HU_PAOHU)
			{
				m_totalrecordinfo.hu[ChairID]++;
				m_totalrecordinfo.paohu[ChairID]++;

				_uint8 PaoID = m_stHuScore[ChairID].stHu[0].bPasHuChairID;
				m_totalrecordinfo.dianpao[PaoID]++;
			}
		}
	}	
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player != NULL)
		{
			m_totalrecordinfo.total_score[i] = m_SeatPlayer[i].player->money - roomGold;
		}
	}	
	if (m_bGangCalcImme)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			if (iTotalScore[i] + m_stGang[i].iTotalScore > 0)
			{
				m_totalrecordinfo.win_counts[i]++;
			}
		}
	}
	else
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			if (iTotalScore[i] > 0)
			{
				m_totalrecordinfo.win_counts[i]++;
			}
		}		
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		m_totalrecordinfo.gang_times[i] += m_stGang[i].Gang.Size();
	}
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		for (int j = 0; j < m_stGang[i].Gang.Size(); j++)
		{
			if (m_stGang[i].Gang[j].iType == TYPE_ZHIGANG)
			{
				m_totalrecordinfo.dian_gang_times[m_stGang[i].Gang[j].ChairID]++;
			}
		}
	}
}

_uint8 SortNode(SGSI_SortNode n[GAME_PLAYER], _uint8 game_player)
{
	SGSI_SortNode tmp;
	for (int i = 0; i < (game_player - 1); i++)
	{
		for (int j = 0; j < (game_player - 1); j++)
		{
			if (n[j].iTotalScore < n[j + 1].iTotalScore)
			{
				tmp = n[j];
				n[j] = n[j + 1];
				n[j + 1] = tmp;
			}
		}
	}
	if (n[0].iTotalScore == 0)
		return 0;
	int iMax = n[0].iTotalScore;
	_uint8 iCounts = 1;
	for (int i = 1; i < game_player; i++)
	{
		if (iMax == n[i].iTotalScore)
			iCounts++;
	}
	return iCounts;
}

/*
void Table::Start_Game_End_Timer()
{
	TIMER_AGAIN(end_timer, end_timer_stamp);
}
void Table::end_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	//ev_timer_stop(zjh.loop, &table->end_timer);
	TIMER_STOP(table->end_timer);
	table->GameEnd();
}
*/

void Table::RedisUpdateScoreToServer(int iTotalScore[GAME_PLAYER])
{
	if (!zjh.conf["open_redis"].asInt())
	{
		return;
	}
	for ( int i = 0 ;i < GAME_PLAYER ; ++i  )
	{
		m_account_info.SetScore(i , iTotalScore[i]);
		m_account_info.SetTotalScore(i, m_totalrecordinfo.total_score[i]);
	}

	log.info("%s:%d\t%s roomid:%d  total_score[0]:%d    total_score[1]:%d    total_score[2]:%d    total_score[3]:%d \n",
		__FILE__,
		__LINE__,
		__FUNCTION__ , 			
		tid,
		m_totalrecordinfo.total_score[0],
		m_totalrecordinfo.total_score[1],
		m_totalrecordinfo.total_score[2],
		m_totalrecordinfo.total_score[3]
		);

	CRedisCache::GetInstance().UpdateGameEndScore(m_account_info);
}
void Table::Send_GameRound_ScoreInfo(Player* player)
{
	proto::game::TotalRoundScore trs;
	for (size_t i = 0; i < m_totalrecordinfo.round_score.size(); i++)
	{
		proto::game::RoundScore* sc = trs.add_round();
		for (int j = 0; j < m_GAME_PLAYER; j++)
		{
			sc->add_score(m_totalrecordinfo.round_score[i].score[j]);
		}
	}
	trs.set_roundcount(m_totalrecordinfo.round_score.size());

	unicast(SERVER_ACK_ROUND_SCORE, player, trs);
}
void Table::RedisUpdateScoreToLocal()
{
	if (!zjh.conf["open_redis"].asInt())
	{
		return;
	}
	if (CRedisCache::GetInstance().GetGameScore(m_account_info))
	{
		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			if (!m_account_info.GetPlayerScore( GetPlayerUid(i) , m_totalrecordinfo.total_score[i] ))
			{
				log.error("%s:%d\t%s roomid:%d get playerscore error  uid:%d\n",
					__FILE__,
					__LINE__,
					__FUNCTION__ , 			
					tid,
					GetPlayerUid(i)
					);
			}
		}
	}
	else
	{
		log.error("%s:%d\t%s roomid:%d get roominfo error \n",
			__FILE__,
			__LINE__,
			__FUNCTION__ , 			
			tid
			);
	}	
}

void Table::InitPlayAccont()
{
	m_account_info.Reset();
	m_account_info.SetRoomid(tid);
	m_account_info.SetRound(m_nPlayingTimes);

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_SeatPlayer[i].player == NULL)
		{
			m_account_info.SetPlayerUid(i, 0);
		}
		else
		{
			m_account_info.SetPlayerUid(i, m_SeatPlayer[i].player->uid);
		}
	}

}
void Table::RequestGameStart_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table* table = (Table*)w->data;
	if (table != NULL)
	{
		table->game_start_req();
	}
}

void Table::Proto_GameEnd_ScoreInfo(Player* player, proto::game::GameEndRecord &ger)
{ 		
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::ReordInfo* info = ger.add_userrecoreinfo();
		info->set_banker_num(m_totalrecordinfo.banker[i]);
		info->set_hu_num(m_totalrecordinfo.hu[i]);
		info->set_outtime_num(m_totalrecordinfo.timerout_op[i]);
		info->set_totalscore(m_totalrecordinfo.total_score[i]);
		info->set_wincounts(m_totalrecordinfo.win_counts[i]);
		info->set_gangtimes(m_totalrecordinfo.gang_times[i]);
		info->set_diangangtimes(m_totalrecordinfo.dian_gang_times[i]);
		info->set_ex1(m_totalrecordinfo.dianpao[i]);
		info->set_dissstate(m_UserDissState[i]);
		info->set_mobaotimes(m_totalrecordinfo.mobao_count[i]);
	}

	_uint8 iCounts;
	SGSI_SortNode dianpao[GAME_PLAYER];
	SGSI_SortNode winner[GAME_PLAYER];
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		dianpao[i].ChairID = i;
		dianpao[i].iTotalScore = m_totalrecordinfo.dianpao[i];
		winner[i].ChairID = i; 
		winner[i].iTotalScore = m_totalrecordinfo.total_score[i];
	}
	iCounts = SortNode(dianpao, m_GAME_PLAYER);
	for (int i = 0; i < iCounts; i++)
	{
		ger.add_mostdianpaouser(dianpao[i].ChairID);
	}
	iCounts = SortNode(winner, m_GAME_PLAYER);
	for (int i = 0; i < iCounts; i++)
	{
		ger.add_mostwinscoreuser(winner[i].ChairID);
	}

	ger.set_datatime(time(NULL));
	if (0 == m_start_time)
	{
		ger.set_starttime(time(NULL));
	}
	else
	{
		ger.set_starttime(m_start_time);
	}

	//log.debug("%s tid:%d ger:%s .\n", __FUNCTION__, tid, ger.DebugString().c_str());
}

void Table::RecordTakeInScore()
{
    for (int i = 0; i < m_GAME_PLAYER; ++i)
    {
		if (m_SeatPlayer[i].player != NULL)
		{				
            mScoreRecord.mScore[0][i] = m_SeatPlayer[i].player->money;
		}
    }
}


void Table::RecordScore(int jushu)
{
    if (jushu < 1 || jushu >= MAX_JU)
        return;

    mScoreRecord.mCount++;

    for (int i = 0; i < m_GAME_PLAYER; ++i)
    {
		if (m_SeatPlayer[i].player != NULL)
		{				
            mScoreRecord.mScore[jushu][i] = m_SeatPlayer[i].player->money;
		}
    }
}

//璁＄畻杩炶耽娆℃暟
void Table::CalcContinueWinTimes()
{
    log.info("tid:%d TotalPlayTimes: %d", tid, m_nPlayingTimes);

    if (mScoreRecord.mCount < 2)
        return;

    for (int j = 0; j <= mScoreRecord.mCount; ++j)
    {
        log.info("tid:%d index:%d mScoreRecord.mScore[%d, %d, %d, %d]",
        	tid, j, mScoreRecord.mScore[j][0],
        	mScoreRecord.mScore[j][1],
        	mScoreRecord.mScore[j][2],
        	mScoreRecord.mScore[j][3]);
    }

    for (int i = 0; i < m_GAME_PLAYER; ++i)
    {
        std::map<int, int> mapContinueWinTimes;
        int sum = 0;
        
        for (int j = 1; j <= mScoreRecord.mCount; ++j)
        {
            if (mScoreRecord.mScore[j][i] > mScoreRecord.mScore[j-1][i])
                sum++;
            else
            {
                if (sum >= 2)
                {
                    if (mapContinueWinTimes.find(sum) != mapContinueWinTimes.end())
                        mapContinueWinTimes[sum]++;
                    else
                        mapContinueWinTimes[sum] = 1;
                }
                
                sum = 0;
            }
        }

        if (sum >= 2)
        {
            if (mapContinueWinTimes.find(sum) != mapContinueWinTimes.end())
                mapContinueWinTimes[sum]++;
            else
                mapContinueWinTimes[sum] = 1;
        }

        LogContinueWinTimes(i, mapContinueWinTimes);
        
    }
}

void Table::LogContinueWinTimes(_uint8 ChairID, std::map<int, int> &mapContinueWinTimes)
{
    if (ChairID >= m_GAME_PLAYER)
        return;

    if (mapContinueWinTimes.size() == 0)
    {
        log.info("tid:%d uid:%d, chairid:%d have no continue win record.",
            	tid, GetPlayerUid(ChairID), ChairID);
        return;
    }

    for (std::map<int, int>::iterator it = mapContinueWinTimes.begin();
        it != mapContinueWinTimes.end(); ++it)
    {
        for (int j=0; j<it->second; ++j)
        {
            log.info("tid:%d uid:%d, chairid:%d continue win record: [cw_%d]",
                tid, GetPlayerUid(ChairID), ChairID, it->first);
        }
    }
}

void Table::InitInterveneInfo()
{
    if (m_intervene) {
        for (int i = 0; i < GAME_PLAYER; ++i){
            m_intervene_player[i] = -1;
			m_limitHu_player[i] =0 ; 
        }
		if(m_nPlayingTimes < 2)  return ;  
		int score[GAME_PLAYER] = { 0 }; 
		double base_score =1;
		if ((m_FixedBaseScore * m_BaseScore) > 0)
		{
			base_score = m_FixedBaseScore * m_BaseScore;
		}
		for (size_t i = 0; i < m_totalrecordinfo.round_score.size(); ++i) {
			for (int j = 0; j < GAME_PLAYER; ++j) {
				double temp_score = (1.0 * m_totalrecordinfo.round_score[i].score[j]) / base_score;
				score[j] +=  int(temp_score);
			}
		}
		_uint8 min_charid = INVALID_CHAIRID, max_charid = INVALID_CHAIRID;
		int min_score = 0, max_score = 0;
		for (int i = 0; i < GAME_PLAYER; ++i) {
			if (min_score > score[i]){
				min_score = score[i];
				min_charid = i;
			}
			if (max_score < score[i]) { 
				max_score = score[i];
				max_charid = i;
			}
		}
		int total_win_lost_money=0 ; 
		set<int> all_handle_seat_set; 

		for(int i = 0 ; i< m_GAME_PLAYER;++i) { 
			for(int day  =1; day  <= 3; ++day ){ 
				total_win_lost_money = DataStatic::instance().get_total_moeny(m_SeatPlayer[i].uid , day ) ; 
				if(m_SeatPlayer[i].player) {
					log.info("%s tid:%d uid:%d chairid:%d day[%d] total[%d] name[%s]\n" ,__FUNCTION__ , tid, GetPlayerUid(i), i, day , total_win_lost_money, m_SeatPlayer[i].player->name.c_str() ); 
				}				
				int day_ctrl_win_lost = 300;
				int Accumulate_ctrl_persent = 40;
				
				if (total_win_lost_money > 0) { 	
					if (zjh.conf["tables"]["day_max_ctrl"].isNull())  continue; 
					bool isArray = zjh.conf["tables"]["day_max_ctrl"].isArray();
					if (isArray==0) continue;
					day_ctrl_win_lost  = zjh.conf["tables"]["day_max_ctrl"][day-1][0u].asInt();					
					if ( abs(total_win_lost_money) < day_ctrl_win_lost) continue;
					
					Accumulate_ctrl_persent = zjh.conf["tables"]["day_max_ctrl"][day-1][2].asInt();
				}
				else { 
					if(zjh.conf["tables"]["day_min_ctrl"].isNull())  continue;
					bool isArray = zjh.conf["tables"]["day_min_ctrl"].isArray();
					if (isArray==0) continue;
					day_ctrl_win_lost = zjh.conf["tables"]["day_min_ctrl"][day-1][0u].asInt();  
					if ( abs(total_win_lost_money) <  abs(day_ctrl_win_lost) ) continue;
					
					Accumulate_ctrl_persent = zjh.conf["tables"]["day_min_ctrl"][day-1][2].asInt();
				}						
				int random_val=  rand()% 100+1; 
				
				Accumulate_ctrl_persent += (abs(total_win_lost_money) - abs(day_ctrl_win_lost)) / 5 ;
				

				log.info("%s tid:%d uid:%d chairid:%d day max day [%d] acc[%d] \n", __FUNCTION__ , tid, GetPlayerUid(i), i, day_ctrl_win_lost, Accumulate_ctrl_persent);  
				if(random_val >= Accumulate_ctrl_persent )  continue;

				// 大于500 ==0 控制输， 小于500 控制赢 ==1 
				if(total_win_lost_money> 0)
					m_intervene_player[i] = 0 ; 
				else m_intervene_player[i] = 1 ;  

				all_handle_seat_set.insert(i) ; 
				if(m_SeatPlayer[i].player ){
					log.info("%s tid:%d uid:%d chairid:%d total[%d]  name[%s]140\n" , __FUNCTION__ , tid, GetPlayerUid(i), i, total_win_lost_money ,m_SeatPlayer[i].player->name.c_str()) ;
				}					
			}			
		}
		if(m_nTotalCount - m_nPlayingTimes==0 && abs(max_score) + abs(min_score) > m_control_score_low && abs(max_score) + abs(min_score) <= m_control_score_high) {
			log.info("%s tid:%d no need control \n " , __FUNCTION__ , tid);			
		}
		else if(m_nTotalCount<= 2*m_nPlayingTimes ) { 
			if(abs(max_score)+ abs(min_score)<= m_control_score_low ){
				if(max_charid != INVALID_CHAIRID  && all_handle_seat_set.find(max_charid) == all_handle_seat_set.end() ){
					m_intervene_player[max_charid] = 1 ;
				} 		
				if(min_charid != INVALID_CHAIRID && all_handle_seat_set.find(min_charid) == all_handle_seat_set.end()) 
					m_intervene_player[min_charid] = 0 ; 

			} else {  
				if(m_control_card.is_control(m_nPlayingTimes)){
					log.info("%s tid:%d need control  \n",__FUNCTION__, tid ); 
					if (min_charid != INVALID_CHAIRID && max_charid != INVALID_CHAIRID) {
						if(all_handle_seat_set.find(min_charid) == all_handle_seat_set.end())
							m_intervene_player[min_charid] = 1;
						m_intervene_player[max_charid] = 0;
					}					
				}				
			}

		} else{			
			if( m_control_card.is_control(m_nPlayingTimes)){
				log.info("%s tid:%d need control \n" ,__FUNCTION__, tid );  
				if (min_charid != INVALID_CHAIRID && max_charid != INVALID_CHAIRID) {
					if(all_handle_seat_set.find(min_charid) == all_handle_seat_set.end())
						m_intervene_player[min_charid] = 1;
					m_intervene_player[max_charid] = 0;
				}				
			}			
		}     

		for(int i = 0 ;i< GAME_PLAYER ;i++){
			if(m_intervene_player[i] == 0) {
				m_limitHu_player[i] = zjh.conf["tables"]["limithucount"].asInt() ;
				if(m_SeatPlayer[i].player) {
					log.info("%s tid:%d uid:%d chairid:%d name[%s] limit_hu[%d] inter[%d]\n" ,__FUNCTION__ , tid, GetPlayerUid(i), i, m_SeatPlayer[i].player->name.c_str(), m_limitHu_player[i], m_intervene_player[i]); 
				}
			}
		}		
        log.info("%s tid:%d m_nPlayingTimes[%d] m_intervene_player[%d, %d, %d, %d] .\n" , 
            __FUNCTION__ , tid, m_nPlayingTimes,m_intervene_player[0], m_intervene_player[1], 
            m_intervene_player[2], m_intervene_player[3]);
		log.info("%s tid:%d limit-hu[%d %d %d %d] \n" , __FUNCTION__ , tid,
			m_limitHu_player[0],m_limitHu_player[1] , m_limitHu_player[2] , m_limitHu_player[3]) ; 
		
		log.info(" %s tid:%d score[%d %d %d %d] \n " , __FUNCTION__ , tid,  score[0] , score[1] , score[2] , score[3]) ; 
    }
}

void Table::CheckGetBadCard(_uint8 charid)
{ 
	if(charid >= m_GAME_PLAYER) return ; 
	if (!m_intervene ) { 
		return;
	}
	if( m_intervene_player[charid]!= 0) {
		return;
	} 	
	if(m_CardPool.Count()< 10) return ; 
	HandCards pc = m_HandCard[charid];
	HandCards src_hc = pc;
	_uint8 cur_card = m_CardPool.CurCard();
	_uint8 src_card = cur_card;
	m_GameLogic.AddCard( &pc , cur_card ); 
	
	MJ_fan_buffer  ofan;  
	ofan.Clear() ; 
	int ret=  Check_Hu(charid , pc, ofan) ;

	if(m_limitHu_player[charid]> 0 &&  ret> 0 ) {  
		if(ret> 0){
			_uint8 current = m_CardPool.Current();
			_uint8 last = m_CardPool.Last();
			for(_uint8 i = current; i < last; ++i){
				if (!IsCanSwapCard(i)) continue;
				cur_card = m_CardPool.GetCardByIndex(i);
				HandCards temp_hc = src_hc;
				m_GameLogic.AddCard( &temp_hc , cur_card ); 
				ofan.Clear() ; 
				if (!Check_Hu(charid ,temp_hc, ofan))
				{
					m_CardPool.SwapCard(current,i); 
					log.info("%s tid:%d uid:%d chairid:%d limit-hu[%d] current[%u] i[%u]  0x%02x changto 0x%02x\n" , 
						__FUNCTION__, tid, GetPlayerUid(charid), charid, m_limitHu_player[charid], current, i, src_card, cur_card);
					break;
				}
			}		
		}
		m_limitHu_player[charid]--; 
	} else { 
		int val = rand()%100+1; 
		if(val > m_intervene_badcard ) return ; 
		set<_uint8> realative_set ;
		_uint8  *pMyArray= m_HandCard[charid].ChangeableCards ;
		_uint8  mylen    = m_HandCard[charid].ChangeableCardsLen ; 
		int face = 0 ;
		int suit = 0 ;
		for(_uint8 st = 0; st< mylen; ++st){
			face = pMyArray[st] & 0xf; 
			suit = pMyArray[st] >> 4 ;
			if(suit< MJ_TYPE_FENG){
				realative_set.insert(pMyArray[st]) ; 
				if(face>=2 && face<=8 ){
					realative_set.insert(pMyArray[st]-1) ;
					realative_set.insert(pMyArray[st]+1) ;
				} else if(face ==9){
					realative_set.insert(pMyArray[st]-1) ;
				} else if(face==1){
					realative_set.insert(pMyArray[st]+1) ;
				}
			} else   realative_set.insert(pMyArray[st]) ; 
		}
		_uint8 current = m_CardPool.Current();
		_uint8 last = m_CardPool.Last();
		for(_uint8 i = current; i < last; ++i  ){
			if (!IsCanSwapCard(i)) continue;
			val = m_CardPool.GetCardByIndex(i); 
			if (CheckIsLaiZiCard(charid, val)) continue;
			if(realative_set.find(val)!= realative_set.end()) continue; 
			if (i > current)
			{
				m_CardPool.SwapCard(current,i) ; 
				log.info("%s tid:%d uid:%d chairid:%d current[%u] i[%u]  0x%02x changto 0x%02x\n" , 
						__FUNCTION__,  tid, GetPlayerUid(charid), charid, current, i, src_card, val);
			}
			break;
		}
	} 	
}

void Table::CheckGetGoodCard(_uint8 charid)
{
	if(charid>= m_GAME_PLAYER) return ; 
    if (!m_intervene ) {
        return;
    }
	if ( m_intervene_player[charid] != 1 ) {
		return;
	}
	if ( (rand() % 100 + 1) > m_intervene_getcard ) {
        return;
    }

	_uint8 src_card = m_CardPool.CurCard();

    vector<_uint8> vec(&m_HandCard[charid].ChangeableCards[0], 
        &m_HandCard[charid].ChangeableCards[m_HandCard[charid].ChangeableCardsLen]);

	_uint8 current = m_CardPool.Current();
	_uint8 last = m_CardPool.Last();

    for ( _uint8 i = current; i < last; ++i  )
    {
		if (!IsCanSwapCard(i)) continue;
		_uint8 bCard = m_CardPool.GetCardByIndex(i);
        if (CheckIsLaiZiCard(charid, bCard)
			|| (find(vec.begin(), vec.end(), bCard) != vec.end())
			|| ( CardType(bCard) < MJ_TYPE_FENG &&
				(find(vec.begin() , vec.end() , bCard - 2)  != vec.end() ||
				find(vec.begin(), vec.end(), bCard - 1) != vec.end()	||
				find(vec.begin(), vec.end(), bCard + 1) != vec.end() ||
				find(vec.begin(), vec.end(), bCard + 2) != vec.end()))
			)
        {
            if ( i != current )
            {
                m_CardPool.SwapCard(current, i);
				log.info("%s tid:%d uid:%d chairid:%d current[%u] i[%u]  0x%02x changto 0x%02x\n" , 
					__FUNCTION__,  tid, GetPlayerUid(charid), charid, current, i, src_card, bCard);
            }
            break;
        }
    }
}

void Table::ChangeBetterHandleCard()
{
    _uint8 start_pos = 0;
	_uint8 card_count = 0;
	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i)
    {
		if (m_conf.m_bBankerFirst14 && i == m_bBanker)
		{
			card_count = m_MaxHandCardSize;
		}
		else
		{
			card_count = m_MaxHandCardSize - 1;
		}

        if ( m_intervene_player[i] != 1 )
        {
			start_pos += card_count;
            continue;
        }
        map<_uint8, int> map_count;
        vector<_uint8> vec_card;

        AnalysisCard(i, start_pos, start_pos+card_count, map_count, vec_card);

        vector<_uint8> vec_change_card = GetChangeCard(map_count, vec_card);

        for (vector<_uint8>::const_iterator it = vec_change_card.begin(); it != vec_change_card.end(); it++)
        {
            ChangeCard(i, start_pos, start_pos+card_count, *it, map_count, vec_card);
        }
    }
}

void Table::AnalysisCard(_uint8 charid, _uint8 start_pos, _uint8 end_pos, std::map<_uint8 , int> &map_count, std::vector<_uint8> &vec_card)
{
    for (_uint8 j = start_pos; j < end_pos; ++j)
    {
        _uint8 card = m_CardPool.GetCardByIndex(j);
        if (find(vec_card.begin(), vec_card.end(), card) == vec_card.end())
        {
            vec_card.push_back(card);
        }
        if (map_count.find(card) == map_count.end())
        {
            map_count[card] = 1;
        }
        else
        {
            map_count[card]++;
        }
    }
    sort(vec_card.begin(), vec_card.end());
}

vector<_uint8> Table::GetChangeCard(std::map<_uint8 , int> &map_count, std::vector<_uint8> &vec_card)
{
    vector<_uint8> vec_change_card;
	_uint8 hao_pai_count = 0 ;   
	// 好牌张数 
	vector<_uint8> swap_vet; 
	for(size_t  n = 0 ; n< vec_card.size() ; ++n){
		bool _haopai = false; 
		if(hao_pai_count>= m_goodcard_count) break; 
		_uint8 card = vec_card[n];
		if (CardType(card) < MJ_TYPE_FENG) {
			if( CheckBetterCard(card , map_count , vec_card))
			{
				_haopai = true;
			}
		}
		else{
			_haopai = true;
		}
		if(!_haopai){
			swap_vet.push_back(card) ; 
		} else hao_pai_count++ ;
	}
	if(hao_pai_count< m_goodcard_count){
		vector<_uint8>::iterator  it = swap_vet.begin() ;
		for(; it!= swap_vet.end() ; ++it){ 
			if(hao_pai_count>=m_goodcard_count) break; 
			hao_pai_count++ ; 
			m_GameLogic.EraseCard(vec_card, *it) ;
			vec_change_card.push_back(*it) ;
		}
	}
	return  vec_change_card ; 
}

bool Table::CheckBetterCard(_uint8 card, std::map<_uint8, int> &map_count, const std::vector<_uint8> &vec_card)
{
	if (map_count[card] >= 2) {
		return true;
	}
	//int face = card >> 4 ; 
    if ( find(  vec_card.begin() , vec_card.end() , card - 2) != vec_card.end()  ) {
        return true;
    }
    if ( find(vec_card.begin(), vec_card.end(), card - 1) != vec_card.end()) {
        return true;
    }
    if ( find(vec_card.begin(), vec_card.end(), card + 1) != vec_card.end()) {
        return true;
    }
    if ( find(vec_card.begin(), vec_card.end(), card + 2) != vec_card.end()) {
        return true;
    }

    return false;
}

void Table::ChangeCard(_uint8 charid, _uint8 start_pos, _uint8 end_pos, _uint8 card,std::map<_uint8, int> &_out_map, const std::vector<_uint8> &vec_card)
{
    _uint8 current = charid * 13 + 13 + 2 ;
	_uint8 last = m_CardPool.Last();

    for (_uint8 k = current; k < last; k++)
    {
		if (!IsCanSwapCard(k)) continue;
        int cur_card = m_CardPool.GetCardByIndex(k);
        bool _haopai = false;
        if (CardType(cur_card) < MJ_TYPE_FENG)
        {
            if ( CheckBetterCard(cur_card , _out_map , vec_card))
            {
                _haopai = true;
            }
        }
        if (_haopai)
        {
            for (_uint8 m = start_pos; m < end_pos; m++)
            {
                if (m_CardPool.GetCardByIndex(m) == card)
                {
                    m_CardPool.SwapCard(m, k);
                }
            }
        }
    }
}
