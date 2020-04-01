#include "table_jianlimj.h"

#include <time.h>

#include "../zjh.h"
#include "../common/log.h"

extern ZJH zjh;
extern Log log;

extern _uint8 SortNode(SGSI_SortNode n[GAME_PLAYER], _uint8 game_player);


Table_jianlimj::Table_jianlimj()
{
	memset(m_nUserHuaCount, 0, sizeof(m_nUserHuaCount));
	memset(m_UserPiaoScore, -1, sizeof(m_UserPiaoScore));

	memset(m_bIsPassHu, false, sizeof(m_bIsPassHu));

	m_AutoPiao_Timer.data = this;
	m_AutoPiao_Timer_Stamp = 6;
	ev_timer_init(&m_AutoPiao_Timer, Table_jianlimj::AutoPiao_Timer_cb, m_AutoPiao_Timer_Stamp, m_AutoPiao_Timer_Stamp);
}

Table_jianlimj::~Table_jianlimj()
{
	TIMER_STOP(m_AutoPiao_Timer);
}

void Table_jianlimj::ConfInit_Remote(const Json::Value& val)
{
	Table::ConfInit_Remote(val);

	m_conf.m_bBankerFirst14 = true;
	m_conf.m_bSupportChi = false;
	m_conf.m_bZimoHu = false;
	m_conf.m_bYiPaoDuoXiang = true;
	m_conf.m_bQiangGangHu = false;
	m_conf.m_bGangShangPao = false;	
	m_conf.m_bHuangGang = false;
	m_conf.m_bHu7dui = false;
	m_conf.m_bSupportBuyHorse = false;
	m_conf.m_bFourceHuMutilHu = false;

	m_conf.m_AnGangPoint = 0;
	m_conf.m_ZhiGangPoint = 0;
	m_conf.m_WanGangPoint = 0;

	m_bGangCalcImme = false;

	if (json_check_play_type_id(val))
	{
		MJ_SUB_ID SubID;

		json_get_play_type_id(val, 1000, SubID);
		if (SubID.Find(4))
		{
			m_GAME_PLAYER = 4;  
		}
		else if (SubID.Find(3))
		{
			m_GAME_PLAYER = 3;
		}
		else if (SubID.Find(2))
		{
			m_GAME_PLAYER = 2;
		}
		m_room_config += SubID;

		//少人开局
		json_get_play_type_id(val, 2000, SubID);
		if (SubID.Size() > 0)
		{
			m_conf.m_bSupportChangeMode = true;    
		}
		m_room_config += SubID;

		//补花
		json_get_play_type_id(val, 1031, SubID);
		if (SubID.Find(1031001))
		{
			m_conf_jianli.m_iHuaType = 0;
		}
		else if (SubID.Find(1031002))
		{
			m_conf_jianli.m_iHuaType = 1;
		} 
		m_room_config += SubID;

		json_get_play_type_id(val, 1032, SubID);
		if (SubID.Find(1032001)) //发财
		{
			m_conf_jianli.m_iHuaType = 2;
		}
		else if (SubID.Find(1032002))	//白板
		{
			m_conf_jianli.m_iHuaType = 3;
		} 
		m_room_config += SubID;

		//底分
		json_get_play_type_id(val, 1033, SubID);
		if (SubID.Find(1033001))
		{
			m_BaseScore = 1;    
		}
		else if (SubID.Find(1033002))
		{
			m_BaseScore = 2;    
		}
		else if (SubID.Find(1033003))
		{
			m_BaseScore = 3;    
		}
		else if (SubID.Find(1033004))
		{
			m_BaseScore = 4;    
		}
		else if (SubID.Find(1033005))
		{
			m_BaseScore = 5;    
		}
		else if (SubID.Find(1033006))
		{
			m_BaseScore = 6;    
		}
		else if (SubID.Find(1033007))
		{
			m_BaseScore = 8;    
		}
		else if (SubID.Find(1033008))
		{
			m_BaseScore = 10;    
		}
		else if (SubID.Find(1033009))
		{
			m_BaseScore = 20;    
		}
		else if (SubID.Find(1033010))
		{
			m_BaseScore = 30;    
		}
		else if (SubID.Find(1033011))
		{
			m_BaseScore = 40;    
		}
		else if (SubID.Find(1033012))
		{
			m_BaseScore = 50;    
		}
		else if (SubID.Find(1033013))
		{
			m_BaseScore = 80;    
		}
		else if (SubID.Find(1033014))
		{
			m_BaseScore = 100;    
		}
		else if (SubID.Find(1033015))
		{
			m_BaseScore = 200;    
		}
		m_room_config += SubID;

		//多选择
		json_get_play_type_id(val, 1034, SubID);
		if (SubID.Find(1034001))
		{
			m_conf_jianli.m_bWeiZeng = true;
		}
		else if (SubID.Find(1034004))
		{
			m_conf.m_bZimoHu = true;
			m_conf.m_bYiPaoDuoXiang = false;
		}  
		m_room_config += SubID;

		//围增
		json_get_play_type_id(val, 1035, SubID);
		if (SubID.Find(1035001))
		{
			m_conf_jianli.m_iWeiZeng = 1;
		}
		else if (SubID.Find(1035002))
		{
			m_conf_jianli.m_iWeiZeng = 2;
		} 
		else if (SubID.Find(1035003))
		{
			m_conf_jianli.m_iWeiZeng = 3;
		}
		else if (SubID.Find(1035004))
		{
			m_conf_jianli.m_iWeiZeng = 4;
		}
		else if (SubID.Find(1035005))
		{
			m_conf_jianli.m_iWeiZeng = 5;
		}
		else if (SubID.Find(1035006))
		{
			m_conf_jianli.m_iWeiZeng = 6;
		}
		else if (SubID.Find(1035007))
		{
			m_conf_jianli.m_iWeiZeng = 8;
		}
		else if (SubID.Find(1035008))
		{
			m_conf_jianli.m_iWeiZeng = 10;
		}
		m_room_config += SubID;

		//定增
		json_get_play_type_id(val, 1036, SubID);
		if (SubID.Find(1036001))
		{
			m_conf_jianli.m_iChuZeng = 0;

			if (0 == m_conf_jianli.m_iChuZeng)
			{
				m_UserCanPiao.Clear();
				for (_uint8 i = 0; i < 3; ++i)
				{
					m_UserCanPiao.Add(i);
				}
			}
		}
		else if (SubID.Find(1036002))
		{
			m_conf_jianli.m_iChuZeng = 1;
		}
		else if (SubID.Find(1036003))
		{
			m_conf_jianli.m_iChuZeng = 2;
		}
		m_room_config += SubID;

		//用牌
		json_get_play_type_id(val, 1037, SubID);
		if (SubID.Find(1037001))
		{
			if (0 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_TTCARDS);
			}
			else if (1 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_TTHONGZHONG);
			}
			else if (2 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_TTHONGZHONG);
				m_CardPool.SetJoker(MJ_FA_CAI, 4);
			}
			else if(3 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_TTHONGZHONG);
				m_CardPool.SetJoker(MJ_BAI_BAN, 4);
			}
		}
		else
		{
			if (0 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_NO_WIND_DRAGON_FLOWER);
			}
			else if (1 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_WTTHONGZHONG);
			}
			else if (2 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_WTTHONGZHONG);
				m_CardPool.SetJoker(MJ_FA_CAI, 4);
			}
			else if(3 == m_conf_jianli.m_iHuaType)
			{
				m_CardPool.Set(CARD_POOL_WTTHONGZHONG);
				m_CardPool.SetJoker(MJ_BAI_BAN, 4);
			}
		}

		json_get_play_type_id(val, 1038, SubID);
		if (SubID.Find(1038001))
		{
			m_conf.m_HorseNum = 1;
		}
		else if (SubID.Find(1038002))
		{
			m_conf.m_HorseNum = 2;
		}
		else if (SubID.Find(1038003))
		{
			m_conf.m_HorseNum = 4;
		}
		else if (SubID.Find(1038004))
		{
			m_conf.m_HorseNum = 6;
		}
		else if (SubID.Find(1038005))
		{
			m_conf.m_HorseNum = 1;
			m_conf.m_buyHorseMojiJiangji = true;
		}
		m_room_config += SubID;

		if (m_conf.m_HorseNum > 0)
		{
			m_conf.m_bSupportBuyHorse = true;
		}

		//防作弊
		json_get_play_type_id(val, 1039, SubID);
		m_room_config += SubID;
	}

	m_CardPool.InitPool();
	m_ft2fn = &m_jianlimj_Fantype_to_FanNum;
}

void Table_jianlimj::GameStart()
{
	if (!m_conf_jianli.m_iChuZeng)
	{
		Table::GameStart();
		StartChoosePiao();
		return;	
	}

	Table::GameStart();

	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i) 
	{
		m_UserPiaoScore[i] = m_conf_jianli.m_iChuZeng;
	}

	Table::Send_Hand_Card();

	Jl_HuaInfo HuaInfo;
	for (int i = 0; i < m_HandCard[m_bBanker].ChangeableCardsLen; ++i)
	{
		if (IsHuaPai(m_HandCard[m_bBanker].ChangeableCards[i]))
		{
			HuaInfo.vecHua.push_back(m_HandCard[m_bBanker].ChangeableCards[i]);
		}
	}

	if (!HuaInfo.vecHua.empty())
	{
		for (size_t j = 0; j < HuaInfo.vecHua.size(); ++j)
		{
			_uint8 tmp = HuaInfo.vecHua[j];
			m_GameLogic.ChuPai(&m_HandCard[m_bBanker], tmp);
			m_stUserOutCard[m_bBanker].Add(tmp);
			m_Hua[m_bBanker].Add(tmp);

			tmp = GetOneCard();
			HuaInfo.vecGets.push_back(tmp);
			m_GameLogic.AddCard(&m_HandCard[m_bBanker], tmp);
		}
	}

	if (!HuaInfo.vecHua.empty())
	{
		proto::game::jianlimj::AckBuHua stAckBuHua;
		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			stAckBuHua.Clear();
			stAckBuHua.set_leftcount(m_CardPool.Count());
			proto::game::jianlimj::Hua* pstHua = stAckBuHua.mutable_buhua();
			pstHua->set_chairid(m_bBanker);
			for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
			{
				pstHua->add_hua(HuaInfo.vecHua[n]);
			}

			if (i == m_bBanker)
			{
				for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
				{
					pstHua->add_getcard(HuaInfo.vecGets[n]);
				}
			}
			else
			{
				for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
				{
					pstHua->add_getcard(0);
				}
			}
			
			unicast(SERVER_CHANGE_CARD, i, stAckBuHua);
		}

		//保存录像
		proto::game::jianlimj::AckBuHua stAckBuHuaVideo;
		stAckBuHuaVideo.set_leftcount(m_CardPool.Count());

		proto::game::jianlimj::Hua* pstHua = stAckBuHuaVideo.mutable_buhua();	
		
		pstHua->set_chairid(m_bBanker);
		for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
		{
			pstHua->add_hua(HuaInfo.vecHua[n]);
		}
	
		SaveVideoData(SERVER_CHANGE_CARD, stAckBuHuaVideo);
	}

	m_bFirstCard = true;
	OnUserGetCard(m_bBanker);
	m_bFirstCard = false;
}

void Table_jianlimj::TurnBanker()
{
	MJ_user_buffer UserID;
	_uint8 bDraw = (MultiHu(UserID) == 0) ? 1 : 0;	
	if (bDraw)
	{
		if (m_iLastMoPaiID != INVALID_CHAIRID)
		{
			m_bBanker = m_iLastMoPaiID;
		}
		else
		{
			m_bBanker = PreChair(m_bCurChairID);
		}
	}
	else
	{
		if (UserID.Size() == 1)
		{
			m_bBanker = UserID[0];
		}
		else
		{
			m_bBanker = m_stHuScore[UserID[0]].stHu[0].bPasHuChairID;
		}
	}
}

void Table_jianlimj::GameEndReset()
{
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		m_Peng[i].Clear();
		m_Hua[i].Clear();
	}

	Table::GameEndReset();

	memset(m_nUserHuaCount, 0, sizeof(m_nUserHuaCount));

	m_iLastMoPaiID = INVALID_CHAIRID;

//	memset(m_UserPiaoScore, -1, sizeof(m_UserPiaoScore));
}

void Table_jianlimj::GameEnd()
{
	MJ_user_buffer UserID;
	_uint8 bDraw = (MultiHu(UserID) == 0) ? 1 : 0;//结束状态，1表示流局，0表示胡牌结束	
	
	//单发结算协议	
	proto::game::jianlimj::AckCurGameEnd stCurGameEnd;
	proto::game::AckGameEnd *stGameend = stCurGameEnd.mutable_gameend();

	HuScoreSet hu_score;
	
	if (bDraw)
	{
		Score_GameEndDraw(hu_score, *stGameend);
	}
	else
	{
		Score_GameEndSucc(hu_score, stCurGameEnd);
	}
	stGameend->set_endstate(bDraw);

/*	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i) 
	{
		proto::game::jianlimj::HuaCard *pHua_list = stCurGameEnd.add_hualist();
		for (int j = 0; j < m_Hua[i].Size(); ++j) 
		{
			pHua_list->add_card(m_Hua[i][j]);
		}
	}*/

	for (int j = 0; j < m_niao.Size(); ++j) 
	{
		stCurGameEnd.add_malist(m_niao[j]);
	}

	for (int j = 0; j < m_zhong_niao.Size(); ++j) 
	{
		stCurGameEnd.add_zhongmalist(m_zhong_niao[j]);
	}

	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i) 
	{
		stCurGameEnd.add_num(m_Hua[i].Size());
	}

	Proto_GameEndInfo_AddFan(UserID, *stGameend);

	int UserScore[GAME_PLAYER] = {0};
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::HandCards *pHandcard = stGameend->add_usercard();
		Fill_HandCard(i, pHandcard);

		Proto_GameEndInfo_GangInfo(i, *stGameend);

		UserScore[i] = hu_score[i].iTotleScore;

		stGameend->add_score(UserScore[i]);	
	}

	log.fatal("%s tid:%d bDraw:%d score[%d,%d,%d,%d]\n", __FUNCTION__, tid, bDraw, UserScore[0], UserScore[1], UserScore[2], UserScore[3]);

	tagUserScore userscore[GAME_PLAYER];
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player != NULL)
		{  
			userscore[i].uid = m_SeatPlayer[i].player->uid;
			userscore[i].score = UserScore[i] ;
			if (UserScore[i]  > 0)
			{
				m_SeatPlayer[i].player->update_info(UserScore[i], 1, 1, win_exp);
			}
			else
			{
				m_SeatPlayer[i].player->update_info(UserScore[i], 1, 0, lose_exp);
			}

			//UpdateUserInfoToUser(m_SeatPlayer[i].player); //客户端动画会有问题
			stGameend->add_money(m_SeatPlayer[i].player->money); 
		}
	}

	broadcast(SERVER_GAME_END, stCurGameEnd);

	log.debug("%s stGameEnd:%s .\n", __FUNCTION__, stCurGameEnd.DebugString().c_str());

 	//发送录像
	save_video_game_end(stCurGameEnd);

	update_user_info_to_datasvr_dz(userscore, 1102);
	
	ev_timer_again(zjh.loop, &send_videodata_timer);

	CalcRecordInfo(UserID, UserScore);

	RedisUpdateScoreToServer(UserScore);

	m_last_Banker = m_bBanker;
	//轮庄
	TurnBanker();
	//初始化数据
	GameEndReset();
}

/*int Table_jianlimj::Calc_Fan(_uint8 ChairID, MJ_fan_buffer& oFan)
{
	int iTotalFan = 0;
	for (int i = 0; i < oFan.Size(); i++)
	{
		iTotalFan += m_ft2fn->GetFan(oFan[i]);
	}
	return iTotalFan;
}*/

void Table_jianlimj::Calc_BaseScore(_uint8 ChairID, int iBaseScore[GAME_PLAYER])
{
	if (!m_bIsHu[ChairID])
	{
		return;
	}		

	iBaseScore[ChairID] = m_BaseScore;
	for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
	{
		iBaseScore[c] = m_BaseScore;
	}

	log.info("%s tid:%d ChairID:%d uid:%d m_BaseScore:%d iBaseScore:[%d,%d,%d,%d] .\n", 
		__FUNCTION__, tid, ChairID, GetPlayerUid(ChairID), m_BaseScore, iBaseScore[0], iBaseScore[1], iBaseScore[2], iBaseScore[3]);
}

void Table_jianlimj::Calc_HuScore(_uint8 ChairID, int iBaseScore[GAME_PLAYER], int iScore[GAME_PLAYER])
{
	if (m_bIsHu[ChairID])
	{
		if (m_stHuScore[ChairID].stHu[0].bType == HU_ZIMO)
		{
			for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
			{
				iScore[ChairID] += iBaseScore[c] * Calc_UserScore(ChairID, c) * 2;
				iScore[c] -= iBaseScore[c] * Calc_UserScore(ChairID, c) * 2;
			}
		}
		else
		{
			_uint8 iPaoId = m_stHuScore[ChairID].stHu[0].bPasHuChairID;
			iScore[ChairID] += iBaseScore[iPaoId] * Calc_UserScore(ChairID, iPaoId);
			iScore[iPaoId] -= iBaseScore[iPaoId] * Calc_UserScore(ChairID, iPaoId);	
		}

	}
}

void Table_jianlimj::UserGetLastCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{
	//最后一张牌不能杠
	iOpType &= ~(TYPE_ANGANG | TYPE_WANGANG);
}

void Table_jianlimj::UserGangHouPao(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan)
{
}

void Table_jianlimj::UserGetFirstCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{

}

void Table_jianlimj::UserFirstOutCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{

}

void Table_jianlimj::UserQiangGangHu(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan)
{
	
}

void Table_jianlimj::UserAfterOperate(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	m_bCurChairID = ChairID;
	log.info("%s uid:%d chairid:%d operate:0x%x card:0x%02x\n", __FUNCTION__, GetPlayerUid(ChairID), ChairID, iOpType, bCard);

	m_bUserOperate[ChairID] = TYPE_NULL;
}


void Table_jianlimj::UserOperateGang(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	memset(m_bAddFanFlag, 0, sizeof(m_bAddFanFlag));
	memset(m_bUserOperate, 0, sizeof(m_bUserOperate));

	log.info("%s tid:%d uid:%d ChairID:%d, iOpType:0x%x, bCard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, bCard);

	int iGangScore[GAME_PLAYER] = {0};
	_uint8 bTempChairID = INVALID_CHAIRID;
	m_bAddFanFlag[ChairID] = true;
	switch (iOpType)
	{
		case TYPE_ANGANG:
		{
			m_stGang[ChairID].Gang.Add(tagGangScore(bCard, ChairID, TYPE_ANGANG));				
			bTempChairID = ChairID;
		}
		break;
		case TYPE_WANGANG:
		{	
	/*		_uint8 cbProvideUser = INVALID_CHAIRID;
			for (_uint8 i=0; i<m_HandCard[ChairID].FixedCardsLen; i++)
			{
				if (m_HandCard[ChairID].FixedCards[i].state == TYPE_PENG)
				{
					if (m_HandCard[ChairID].FixedCards[i].CardData == bCard)
					{					
						cbProvideUser = m_HandCard[ChairID].FixedCards[i].chairID;
					}
				}
			}	*/
			m_stGang[ChairID].Gang.Add(tagGangScore(bCard, ChairID, iOpType));
			bTempChairID = ChairID;						
		}
		break;
		case TYPE_ZHIGANG:
		{
			m_stGang[ChairID].Gang.Add(tagGangScore(bCard, m_bOutCardChairID, TYPE_ZHIGANG));

			bTempChairID = m_bOutCardChairID;

			m_bGetCardCount[ChairID]++;
				//从打出牌删掉这张牌
			m_stUserOutCard[m_bOutCardChairID].RemoveLast(bCard);
		}
		break;
		default:
		log.error("UserOperateGang, wrong optype:%d \n", iOpType);
		break;
	}
	

	m_GameLogic.ExecuteGang(&m_HandCard[ChairID], bTempChairID, bCard, iOpType);

	Calc_Score(m_stGang, iGangScore);

	proto::game::AckBcOpResult stOpresult;

	Proto_Game_OperateGang(ChairID, bCard, iOpType, iGangScore, stOpresult);

	broadcast(SERVER_OPERATE_RESULT, stOpresult);

	m_bUserOperate[ChairID] = TYPE_NULL;

	SaveVideoData(SERVER_OPERATE_RESULT, stOpresult);
}



int Table_jianlimj::OutCardEstimate(_uint8 ChairID, _uint8 bCard, MJ_major& major, int& fan_num)
{	
	assert(major.m_oFan.Size() == 0);

	int iOpType = TYPE_NULL;
	
	HandCards* pHc = &m_HandCard[ChairID];
		
	iOpType |= GetChi(ChairID, bCard, major.m_Chi);
		
	iOpType |= GetPengGang(ChairID, bCard, major.m_GetPengGang);

	
	assert(pHc->ChangeableCardsLen < HAND_CARD_SIZE_MAX);

	if (!m_conf.m_bZimoHu)
	{
		m_GameLogic.AddCard(pHc, bCard);
		
		iOpType |= Check_Hu(ChairID, *pHc, major.m_oFan);		
		if (CardPoolIsEmpty())
		{
			UserLastOutCard(ChairID, iOpType, major.m_oFan);
		}		
		
		if (IsQuanqiuren(*pHc, major.m_oFan, iOpType))
		{
		}		
		
		if (m_bGetCardCount[m_bBanker] == 1)
		{//庄家打出第一张牌
			for (_uint8 c = NextChair(m_bBanker); c != m_bBanker; c = NextChair(c))
			{
				if (m_bGetCardCount[c] != 0)
					goto BANKER_FIRST_OUTCARD;
			}
			UserFirstOutCard(ChairID, iOpType, major.m_oFan);			
		}
BANKER_FIRST_OUTCARD:
		if (iOpType & TYPE_HU)
		{
			if (m_conf.m_bGangShangPao)
			{
				if (!m_bRobWanGang && m_bAddFanFlag[m_bCurChairID]) //杠上炮
				{
					UserGangHouPao(ChairID, bCard, major.m_oFan);					
				}
			}
			if (m_conf.m_bQiangGangHu || m_conf.m_bQiangZhiGang || m_conf.m_bQiangAnGang)
			{
				if (m_bRobWanGang && m_bAddFanFlag[m_WangGangPlayer])  //抢杠胡
				{
					UserQiangGangHu(ChairID, bCard, major.m_oFan);
				}
				else if (m_bRobGangHuJudge)
				{
					UserQiangGangHu(ChairID, bCard, major.m_oFan);
				}
			}
			
			fan_num = Calc_Fan(ChairID, major.m_oFan);
		}
		m_GameLogic.ChuPai(pHc, bCard);
	}	

	return iOpType;	
}

int Table_jianlimj::GetCardEstimate(_uint8 ChairID, _uint8 bCard, MJ_major& major, int& fan_num)
{
	assert(major.m_oFan.Size() == 0);

	int iOpType = TYPE_NULL;

	HandCards* pHc = &m_HandCard[ChairID];	
	m_GameLogic.AddCard(pHc, bCard);

	//assert(pHc->ChangeableCardsLen % 3 == 2);
	if (pHc->ChangeableCardsLen % 3 != 2)
	{
		log.error("%s tid:%d fatal paishu error uid:%d chairid:%d!\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID);
		m_GameLogic.ChuPai(pHc, bCard);
		return TYPE_NULL;
	}
	assert(pHc->ChangeableCardsLen <= HAND_CARD_SIZE_MAX);
		
	iOpType |= GetGangInfo(ChairID, major.m_GetGangInfo);

	iOpType |= Check_Hu(ChairID, *pHc, major.m_oFan);
	if (m_bGetCardCount[ChairID] == 1)
	{
		UserGetFirstCard(ChairID, iOpType, major.m_oFan);
	}		
	if (CardPoolIsEmpty())
	{
		UserGetLastCard(ChairID, iOpType, major.m_oFan);	
	}		
	if (iOpType & TYPE_HU)
	{
/* 		if (m_bAddFanFlag[ChairID])   //杠上花
 		{
 			UserGangShangHua(ChairID, bCard, major.m_oFan); 

 			if (major.m_oFan.Find(MJ_FAN_TYPE_GANGSHANGHUA) && major.m_oFan.Find(MJ_FAN_TYPE_PINGHU))
 			{
 				major.m_oFan.Remove(MJ_FAN_TYPE_PINGHU);
 			}		
 		}*/

		fan_num = Calc_Fan(ChairID, major.m_oFan);
	}

	m_GameLogic.ChuPai(pHc, bCard);

	return iOpType;
}

int Table_jianlimj::GetGangInfo(_uint8 ChairID, MJ_opinfo& ganginfo)
{
	int iOpType = m_GameLogic.GetGangInfo(&m_HandCard[ChairID], ganginfo);

	//癞子不能吃碰杠
	MJ_opinfo temp_del;
	for (_uint8 i = 0; i < ganginfo.Size(); ++i)
	{
		if (m_Peng[ChairID].Find(ganginfo[i].cbCard) || IsHuaPai(ganginfo[i].cbCard))
		{
			iOpType &= ~ganginfo[i].nState;
			temp_del.Add(ganginfo[i]);
		}
	}
	for (_uint8 i = 0; i < temp_del.Size(); ++i)
	{
		ganginfo.Remove(temp_del[i]);
	}
	//如果存在两个相同的杠，上面的方法会把两个杠都移除掉,下面在重新加回去
	for (_uint8 i = 0; i < ganginfo.Size(); ++i)
	{
		iOpType |= ganginfo[i].nState;
	}

	return iOpType;
}


// 胡牌牌型计算并添加响应番
int Table_jianlimj::Check_Hu_Pattern(_uint8 ChairID, const HandCards& pHc, MJ_fan_buffer& oFan, const MJ_win_pattern& oPattern)
{
	HandCards tmpHc = pHc;

	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
	m_GameLogic.GetCardCount(&tmpHc, CardDataNum);

	if (oFan.IsNull())
	{
		oFan.Add(MJ_FAN_TYPE_PINGHU);
	}

	return TYPE_HU;
}

int Table_jianlimj::Check_Hu(_uint8 ChairID, HandCards& pHc, MJ_fan_buffer& oFan)
{
	if (pHc.ChangeableCardsLen > 14)
	{
		return TYPE_NULL;
	}

	if (pHc.ChangeableCardsLen != 0 && pHc.ChangeableCardsLen % 3 != 2)
	{
		return TYPE_NULL;
	}

/*	if (Is7Dui(ChairID, pHc, oFan))
	{
		return TYPE_HU;
	}	*/

	MJ_win_pattern pattern;
	if (FormatCards(&pHc, pattern))
	{
		return Check_Hu_Pattern(ChairID, pHc, oFan, pattern);
	}


	return TYPE_NULL;
}

void Table_jianlimj::InitData()
{
	Table::InitData();

	memset(m_bIsPassHu, false, sizeof(m_bIsPassHu));

	m_iLastMoPaiID = INVALID_CHAIRID;

	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i)
	{
		m_Peng[i].Clear();
		m_Hua[i].Clear();
	}
}

void Table_jianlimj::OnUserOutCard(_uint8 ChairID, _uint8 bCard)
{
    // 删出牌时钟	
	ev_timer_stop(zjh.loop, &m_Outcard_Timer);
	log.info("%s tid:%d uid:%d chairid:%d outcard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bCard);
	m_bCurChairID = ChairID;
	m_bOutCardChairID = ChairID;
	m_GameLogic.ChuPai(&m_HandCard[ChairID], bCard);
	m_bCurCard = bCard;
	m_bCurOutCard = bCard;
	m_bOverTimeState[ChairID] = 0;
	
	m_stUserOutCard[ChairID].Add(bCard);

	if (IsHuaPai(bCard))
	{
		m_Hua[ChairID].Add(bCard);
	}

    //广播玩家出牌	
	proto::game::AckUserOutCard stUseroutcard;
	stUseroutcard.Clear();
	stUseroutcard.set_chairid(ChairID);
	stUseroutcard.set_card(bCard);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::tagShowOutCard *pstShow = stUseroutcard.add_showcard();
		for (int j=0; j<m_stUserOutCard[i].Size(); j++)
		{
			pstShow->add_outcard(m_stUserOutCard[i][j]);
		}
		pstShow->set_outcardlen(m_stUserOutCard[i].Size());
	}

	broadcast(SERVER_USER_OUTCARD, stUseroutcard);

	save_user_out_card(ChairID, bCard);

}

void Table_jianlimj::OnUserGetCard(_uint8 ChairID, bool bGetLast)
{
	if (m_bDeleteTable)
	{
		return;
	}

	if (IsLastGetCard())
	{
		m_iLastMoPaiID = ChairID;
	}

	if (CardPoolIsEmpty())
	{
		GameEnd();
		return;
	}
	memset(&m_bUserOperate, 0, sizeof(m_bUserOperate));
	m_bCurChairID = ChairID;

	_uint8 bGetCard = INVALID_MJ_CARD;
	if (m_bFirstCard)
	{
		int i = 0; 
		for (; i < m_HandCard[ChairID].ChangeableCardsLen; ++i)
		{
			if (m_bCurCard == m_HandCard[ChairID].ChangeableCards[i])
				break;
		}
		if (i < m_HandCard[ChairID].ChangeableCardsLen)
		{
			bGetCard = m_bCurCard;
		}
		else
		{
			bGetCard = m_HandCard[ChairID].ChangeableCards[m_HandCard[ChairID].ChangeableCardsLen - 1];
		}

		m_GameLogic.ChuPai(&m_HandCard[ChairID], bGetCard);
	}
	else
	{
		bGetCard = bGetLast ? m_CardPool.GetLast() : m_CardPool.GetOneCard();
	}

	m_bGetCardCount[ChairID]++;
	m_bIsPassHu[ChairID] = false;
	m_bCurCard = bGetCard;

	bool buguohua = false;
	m_GameLogic.AddCard(&m_HandCard[ChairID], bGetCard);

	Jl_HuaInfo HuaInfo;
	for (int i = 0; i < m_HandCard[ChairID].ChangeableCardsLen; ++i)
	{
		if (IsHuaPai(m_HandCard[ChairID].ChangeableCards[i]))
		{
			HuaInfo.vecHua.push_back(m_HandCard[ChairID].ChangeableCards[i]);
		}
	}

	bool flag = false;
	while (!HuaInfo.vecHua.empty())
	{
		m_bAddFanFlag[ChairID] = false;
		if(flag == false)
		{
			for(int i=0; i<m_GAME_PLAYER; i++)
			{
				proto::game::AckUserGetCard stUsergetcard;
				stUsergetcard.Clear();
				stUsergetcard.set_chairid(ChairID);
				stUsergetcard.set_leftcardnum(m_CardPool.Count());
				if(i != ChairID)
				{

				}
				else
				{
					stUsergetcard.set_card(bGetCard);
				}
				stUsergetcard.set_optype2(0);
				unicast(SERVER_USER_GETCARD,i,stUsergetcard);

			}
			save_user_get_card(ChairID, bGetCard, m_CardPool.Count(), false);
			m_bCurChairID  = ChairID;
			m_bCurCard = bGetCard;
		}

		for (size_t j = 0; j < HuaInfo.vecHua.size(); ++j)
		{
			if(CardPoolIsEmpty())
			{
				GameEnd();
				return;
			}

			_uint8 tmp = HuaInfo.vecHua[j];
			m_GameLogic.ChuPai(&m_HandCard[ChairID], tmp);
			m_stUserOutCard[ChairID].Add(tmp);
			m_Hua[ChairID].Add(tmp);
			m_bAddFanFlag[ChairID] = true;
			m_bCurOutCard = tmp;

			bGetCard = m_CardPool.GetOneCard();
			HuaInfo.vecGets.push_back(bGetCard);
			m_GameLogic.AddCard(&m_HandCard[ChairID], bGetCard);
			m_bCurCard = bGetCard;

			m_bGetCardCount[ChairID]++;

			m_bCurChairID = ChairID;
			m_bOutCardChairID = ChairID;
		}
	
		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			proto::game::jianlimj::AckBuHua stAckBuHua;

			stAckBuHua.set_leftcount(m_CardPool.Count());	

			proto::game::jianlimj::Hua* pstHua = stAckBuHua.mutable_buhua();
			pstHua->set_chairid(ChairID);
			for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
			{
				pstHua->add_hua(HuaInfo.vecHua[n]);
			}

			if (i == ChairID)
			{
				for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
				{
					pstHua->add_getcard(HuaInfo.vecGets[n]);
				}
			}
			else
			{
				for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
				{
					pstHua->add_getcard(0);
				}
			}
			
			unicast(SERVER_CHANGE_CARD, i, stAckBuHua);
		}

		//保存录像
		proto::game::jianlimj::AckBuHua stAckBuHuaVideo;
		stAckBuHuaVideo.set_leftcount(m_CardPool.Count());

		proto::game::jianlimj::Hua* pstHua = stAckBuHuaVideo.mutable_buhua();
		
		pstHua->set_chairid(ChairID);
		for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
		{
			pstHua->add_hua(HuaInfo.vecHua[n]);
		}
	
		SaveVideoData(SERVER_CHANGE_CARD, stAckBuHuaVideo);
		
		buguohua = true;

		HuaInfo.vecHua.clear();
		HuaInfo.vecGets.clear();

		for (int i = 0; i < m_HandCard[ChairID].ChangeableCardsLen; ++i)
		{
			if (IsHuaPai(m_HandCard[ChairID].ChangeableCards[i]))
			{
				HuaInfo.vecHua.push_back(m_HandCard[ChairID].ChangeableCards[i]);
			}
		}

		flag = true;
	}

	m_GameLogic.ChuPai(&m_HandCard[ChairID], bGetCard);
	

	MJ_major major;

	m_bUserOperate[ChairID] = GetCardEstimate(ChairID, bGetCard, major, m_HuFanNums[ChairID]);
	m_GameLogic.AddCard(&m_HandCard[ChairID], bGetCard);
	
	
	log.info("%s tid:%d uid:%d chairid:%d getcard:0x%02x optype:0x%x cardpoollen:%d \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bGetCard, m_bUserOperate[ChairID], m_CardPool.Count());
	
	if ((!m_bFirstCard) && (!buguohua))
	{
		save_user_get_card(ChairID, bGetCard, m_CardPool.Count(), bGetLast);
	}

	//广播玩家摸牌
	proto::game::AckUserGetCard stUsergetcard;
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (m_SeatPlayer[i].player == NULL)
		{
			log.error("OnUserGetCard chairid:%d  error, one player is NULL\n", ChairID);
			continue;
		}

//		Proto_Game_UserGetCard(i, ChairID, bGetCard, stUsergetcard);
		Proto_Game_UserGetCard(i, ChairID, bGetCard, stUsergetcard, major, buguohua);
		stUsergetcard.set_optype2(bGetLast ? 1 : 0);
		unicast(SERVER_USER_GETCARD, i, stUsergetcard);		
	}

	//补花
	if (IsHuaPai(bGetCard))
	{
		OnUserOutCard(ChairID, bGetCard);
		OnUserGetCard(ChairID, true);
	}
	else
	{
		//补花
		if (m_bUserOperate[ChairID] == TYPE_NULL || m_bUserOperate[ChairID] == TYPE_LISTEN)
		{
			set_out_timer_1(ChairID,false,m_bFirstCard);
		}
		else
		{
			set_operator_timer_1(ChairID,false,m_bFirstCard);
		}
	}
}

void Table_jianlimj::OnRecvUserOperate(_uint8 ChairID, int iOpType, int iCard)
{
	if ((m_bUserOperate[ChairID] & TYPE_HU) && iOpType != TYPE_HU)
	{
		m_bIsPassHu[ChairID] = true;
	}

	if (((m_bUserOperate[ChairID] & TYPE_ZHIGANG) && iOpType != TYPE_ZHIGANG) 
		|| ((m_bUserOperate[ChairID] & TYPE_WANGANG) && iOpType != TYPE_WANGANG) )
	{
		if (!m_Peng[ChairID].Find(iCard))
		{
			m_Peng[ChairID].Add(iCard);
		}
		
	}

	Table::OnRecvUserOperate(ChairID, iOpType, iCard);
}

void Table_jianlimj::Proto_Game_OperateGang(_uint8 ChairID, _uint8 bCard, int iOpType, int iGangScore[GAME_PLAYER], proto::game::AckBcOpResult& stOpresult)
{
	stOpresult.set_chairid(ChairID);
	stOpresult.set_outcardchairid(m_bOutCardChairID);
	stOpresult.set_optype(iOpType);
	stOpresult.set_card(bCard);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		stOpresult.add_score(0);
	}

	if (m_bGangCalcImme)
	{
		Update_Score(iGangScore);
	}
}


int Table_jianlimj::dispatch_client(int cmd, Client *client, Player *player)
{
	int iRet = Table::dispatch_client(cmd, client, player);
	if (iRet >= 0)
	{
		return 0;
	}

	switch (cmd)
	{
		case CLIENT_GAME_END_RECORD_REG:
		{
			proto::game::GameEndRecord ger;		
			Proto_GameEnd_ScoreInfo(player, ger);
			unicast(SERVER_GAME_RECORD, player, ger);
			return 0;
		}
		case CLIENT_REQ_ROUND_SCORE:
		Send_GameRound_ScoreInfo(player);
		return 0;
		case CLIENT_MJ_CHOOSE_PIAO:
		RecvClientChoosePiao(client, player);
		return 0;	
		default:
		break;
	}
	log.error("%s tid:%d invalid command[%d] uid:%d\n", __FUNCTION__, tid, cmd, player->uid);
	return -1;
}

//用户打牌后的判断
bool Table_jianlimj::EstimateUserRespond(_uint8 bOutChairID, _uint8 bOutCard)
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

		if ((m_bUserOperate[i] & TYPE_HU) && m_bIsPassHu[i])
		{
			m_bUserOperate[i] &=~ TYPE_HU;
		}

		if (m_bUserOperate[i] != TYPE_NULL)
		{
			if (m_bUserOperate[i] & TYPE_HU)
			{
				//m_bAddFanFlag[i] = true;
				m_bMultiOpHuNum++;				
			}
			m_bMultiOpUserNum++;
			bIsChangeUser = false;
			//准备向四个玩家发送操作提示

			for (int j = 0; j < m_GAME_PLAYER; j++)
			{
				proto::game::AckOpNotify stNotify;		

				Proto_Game_OpNotify(i, j, bOutCard, stNotify);
				unicast(SERVER_OPERATE_NOTIFY, j, stNotify);	

				if (i == j) save_operate_notify(stNotify);		
			}
		}
		if (m_bUserOperate[i] != 0)
		{
			log.info("%s uid:%d chairid:%d Operate:0x%x, totalop:%d \n", __FUNCTION__, GetPlayerUid(i), i, m_bUserOperate[i], m_bMultiOpUserNum);
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

void Table_jianlimj::Score_GameEndSucc(HuScoreSet& score, proto::game::jianlimj::AckCurGameEnd &stCurGameEnd)
{
	if (m_conf.m_bSupportBuyHorse)
	{
		CalZhongMaNum();
	}

	int iTotalScore[GAME_PLAYER] = { 0 };
	int iBaseScore[GAME_PLAYER][GAME_PLAYER];
	int iHuScore[GAME_PLAYER][GAME_PLAYER];
	int iGangScore[GAME_PLAYER] = {0};

	memset(iBaseScore, 0, sizeof(iBaseScore));
	memset(iHuScore, 0, sizeof(iHuScore));

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		Calc_BaseScore(i, iBaseScore[i]);
	}

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{		
		Calc_HuScore(i, iBaseScore[i], iHuScore[i]);		
	}

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		Calc_Score(iTotalScore, iHuScore[i]);
	}

	int iTotalWin = 0;

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		if (iTotalScore[i] > 0)
		{
			iTotalWin += iTotalScore[i];
		}
	}

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{		
	 	Calc_GangScore(i, iGangScore, iTotalWin);		
	}

	Calc_Score(m_stGang, iGangScore);

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		score[i].iTotleScore = iTotalScore[i] + iGangScore[i];
	}

	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		stCurGameEnd.add_userpaotou(m_UserPiaoScore[i]);
	}

}

void Table_jianlimj::StartChoosePiao()
{
	m_bGameState = GAME_PIAO;

	bool bHavePiao = true;
	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		if (m_UserPiaoScore[i] <= 0) bHavePiao = false;
	}

	if (bHavePiao)
	{
		proto::game::jianlimj::AckChooseOver stAck;
		for (int i=0; i<m_GAME_PLAYER; i++)
		{
			stAck.add_piaonum(m_UserPiaoScore[i]);
		}

		broadcast(SERVER_CHOOSE_PIAO_OVER, stAck);

		std::string str = stAck.SerializeAsString();
		save_video_data(SERVER_CHOOSE_PIAO_OVER, str);

		m_bGameState = GAME_PLAYING;
		Table::Send_Hand_Card();
		
		Jl_HuaInfo HuaInfo;
		for (int i = 0; i < m_HandCard[m_bBanker].ChangeableCardsLen; ++i)
		{
			if (IsHuaPai(m_HandCard[m_bBanker].ChangeableCards[i]))
			{
				HuaInfo.vecHua.push_back(m_HandCard[m_bBanker].ChangeableCards[i]);
			}
		}

		if (!HuaInfo.vecHua.empty())
		{
			for (size_t j = 0; j < HuaInfo.vecHua.size(); ++j)
			{
				_uint8 tmp = HuaInfo.vecHua[j];
				m_GameLogic.ChuPai(&m_HandCard[m_bBanker], tmp);
				m_stUserOutCard[m_bBanker].Add(tmp);
				m_Hua[m_bBanker].Add(tmp);

				tmp = GetOneCard();
				HuaInfo.vecGets.push_back(tmp);
				m_GameLogic.AddCard(&m_HandCard[m_bBanker], tmp);
			}
		}

		if (!HuaInfo.vecHua.empty())
		{
			proto::game::jianlimj::AckBuHua stAckBuHua;
			for (int i = 0; i < m_GAME_PLAYER; ++i)
			{
				stAckBuHua.Clear();
				stAckBuHua.set_leftcount(m_CardPool.Count());
				proto::game::jianlimj::Hua* pstHua = stAckBuHua.mutable_buhua();
				pstHua->set_chairid(m_bBanker);
				for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
				{
					pstHua->add_hua(HuaInfo.vecHua[n]);
				}

				if (i == m_bBanker)
				{
					for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
					{
						pstHua->add_getcard(HuaInfo.vecGets[n]);
					}
				}
				else
				{
					for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
					{
						pstHua->add_getcard(0);
					}
				}
				
				unicast(SERVER_CHANGE_CARD, i, stAckBuHua);
			}

			//保存录像
			proto::game::jianlimj::AckBuHua stAckBuHuaVideo;
			stAckBuHuaVideo.set_leftcount(m_CardPool.Count());

			proto::game::jianlimj::Hua* pstHua = stAckBuHuaVideo.mutable_buhua();	
			
			pstHua->set_chairid(m_bBanker);
			for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
			{
				pstHua->add_hua(HuaInfo.vecHua[n]);
			}
		
			SaveVideoData(SERVER_CHANGE_CARD, stAckBuHuaVideo);
		}

		m_bFirstCard = true;
		OnUserGetCard(m_bBanker);
		m_bFirstCard = false;

		return;
	}

	proto::game::jianlimj::AckStartPiao stAck;

	for (_uint8 i = 0; i < m_UserCanPiao.Size(); ++i)
	{
		stAck.add_piaonum(m_UserCanPiao[i]);
	}

	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		if (m_UserPiaoScore[i] <= 0)
		{
			unicast(SERVER_START_PIAO, i, stAck);
		}
	}


	/*std::string str = stAck.SerializeAsString();
	save_video_data(SERVER_START_PIAO, str);*/

	TIMER_AGAIN_s(m_AutoPiao_Timer);
}

void Table_jianlimj::RecvClientChoosePiao(Client *client, Player * player)
{
	_uint8 chairid = player->seatid;

	if (m_UserPiaoScore[chairid] > 0)
	{
		log.error("%s tid:%d error, uid:%d chairid:%d already piao \n", __FUNCTION__, tid, player->uid, chairid);
		return;
	}

	proto::game::jianlimj::ReqChoosePiao stReq;
	stReq.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
	int piaonum = stReq.piaonum();

	if (!m_UserCanPiao.Find(piaonum))
	{
		log.error("%s tid:%d error, piaonum:%d not found \n", __FUNCTION__, tid, piaonum);
		return;
	}

	m_UserPiaoScore[chairid] = piaonum;

	//广播玩家选择的跑头
	proto::game::jianlimj::AckBroadPiao stAck;
	stAck.set_chairid(chairid);;
	broadcast(SERVER_BROAD_PIAO, stAck);

	std::string str = stAck.SerializeAsString();
	save_video_data(SERVER_BROAD_PIAO, str);

	log.info("%s tid:%d uid:%d chairid:%d piaonum:%d  \n", __FUNCTION__, tid, player->uid, chairid, m_UserPiaoScore[chairid]);

	int count = 0;
	for (int i=0; i<m_GAME_PLAYER; i++)
	{
		if (m_UserPiaoScore[i] >= 0)
			count++;
	}
	
	if (count == m_GAME_PLAYER)
	{	
		proto::game::jianlimj::AckChooseOver stAck;
		for (int i=0; i<m_GAME_PLAYER; i++)
		{
			stAck.add_piaonum(m_UserPiaoScore[i]);
		}

		broadcast(SERVER_CHOOSE_PIAO_OVER, stAck);

		std::string str = stAck.SerializeAsString();
		save_video_data(SERVER_CHOOSE_PIAO_OVER, str);

		GameStartAfterPiao();
	}
}

void Table_jianlimj::GameStartAfterPiao()
{
	TIMER_STOP(m_AutoPiao_Timer);
	m_bGameState = GAME_PLAYING;

	Table::Send_Hand_Card();

	Jl_HuaInfo HuaInfo;
	for (int i = 0; i < m_HandCard[m_bBanker].ChangeableCardsLen; ++i)
	{
		if (IsHuaPai(m_HandCard[m_bBanker].ChangeableCards[i]))
		{
			HuaInfo.vecHua.push_back(m_HandCard[m_bBanker].ChangeableCards[i]);
		}
	}

	if (!HuaInfo.vecHua.empty())
	{
		for (size_t j = 0; j < HuaInfo.vecHua.size(); ++j)
		{
			_uint8 tmp = HuaInfo.vecHua[j];
			m_GameLogic.ChuPai(&m_HandCard[m_bBanker], tmp);
			m_stUserOutCard[m_bBanker].Add(tmp);
			m_Hua[m_bBanker].Add(tmp);

			tmp = GetOneCard();
			HuaInfo.vecGets.push_back(tmp);
			m_GameLogic.AddCard(&m_HandCard[m_bBanker], tmp);
		}
	}

	if (!HuaInfo.vecHua.empty())
	{
		proto::game::jianlimj::AckBuHua stAckBuHua;
		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			stAckBuHua.Clear();
			stAckBuHua.set_leftcount(m_CardPool.Count());
			proto::game::jianlimj::Hua* pstHua = stAckBuHua.mutable_buhua();
			pstHua->set_chairid(m_bBanker);
			for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
			{
				pstHua->add_hua(HuaInfo.vecHua[n]);
			}

			if (i == m_bBanker)
			{
				for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
				{
					pstHua->add_getcard(HuaInfo.vecGets[n]);
				}
			}
			else
			{
				for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
				{
					pstHua->add_getcard(0);
				}
			}
			
			unicast(SERVER_CHANGE_CARD, i, stAckBuHua);
		}

		//保存录像
		proto::game::jianlimj::AckBuHua stAckBuHuaVideo;
		stAckBuHuaVideo.set_leftcount(m_CardPool.Count());

		proto::game::jianlimj::Hua* pstHua = stAckBuHuaVideo.mutable_buhua();	
		
		pstHua->set_chairid(m_bBanker);
		for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
		{
			pstHua->add_hua(HuaInfo.vecHua[n]);
		}
	
		SaveVideoData(SERVER_CHANGE_CARD, stAckBuHuaVideo);
	}

	m_bFirstCard = true;
	OnUserGetCard(m_bBanker);
	m_bFirstCard = false;
}

void Table_jianlimj::Send_Game_Scene(Player *player)
{
	if (player == NULL)
	{
		log.error("Send_GameScene, player is NULL \n");
		return;
	}
	int ChairID = player->seatid;	    
	if (ChairID < 0 || ChairID >= m_GAME_PLAYER)
	{
		Send_lookon_GameScene(player);
		return;
	}

	proto::game::jianlimj::tagCurGameScence stCurGameScence;

	proto::game::AckGameScene *stGameScene = stCurGameScence.mutable_gamesence();	
	
	Proto_GameScene(ChairID, player, *stGameScene);

	stGameScene->set_scenestatus(m_bGameState);	//客户端认为状态不为free游戏就已经开始

	stCurGameScence.set_ispaotouing(m_bGameState == GAME_PIAO ? true : false);

	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i)
	{
		if (m_UserPiaoScore[i] >= 0)
		{
			stCurGameScence.add_ispaotouok(1);
		}
		else
		{
			stCurGameScence.add_ispaotouok(0);
		}
	}

	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i)
	{
		stCurGameScence.add_userpaotou(m_UserPiaoScore[i]);
	}
	
	log.debug("%s stCurGameScence:%s .\n", __FUNCTION__, stCurGameScence.DebugString().c_str());

	unicast(SERVER_GAME_SCENE, player, stCurGameScence);
}

/*bool Table_jianlimj::Is7Dui(_uint8 ChairID, HandCards& pHc, MJ_fan_buffer& oFan)
{
	if (m_conf.m_bHu7dui)
	{
		if (m_GameLogic.Is7Dui(&pHc))
		{
			oFan.Add(MJ_FAN_TYPE_QIDUI);

			return true;
		}
	}

	return false;
}*/

void Table_jianlimj::save_video_game_end(const proto::game::jianlimj::AckCurGameEnd stCurGameEnd)
{
	std::string str = stCurGameEnd.SerializeAsString();
	save_video_data(SERVER_GAME_END, str);
}

void Table_jianlimj::save_game_scene_info()
{
	proto::game::jianlimj::tagCurGameScence stCurGameScence;
	proto::game::AckGameScene *stGameScene = stCurGameScence.mutable_gamesence();	
	
	stGameScene->set_outcardtime(m_Outcard_Timer_stamp);
	stGameScene->set_operatetime(m_Operate_Timer_stamp0);
	stGameScene->set_delaytime(m_bDelayTime);
	stGameScene->set_basescore(m_BaseScore);
	stGameScene->set_scenestatus(m_bGameState);
	stGameScene->set_curoutcarduser(m_bOutCardChairID);
	stGameScene->set_curoutcard(m_bCurOutCard);
	stGameScene->set_bankerchairid(m_bBanker);
	stGameScene->set_stand_money(stand_money);

	stGameScene->set_scenestatus(m_bGameState == GAME_PIAO ? GAME_FREE : m_bGameState);	//客户端认为状态不为free游戏就已经开始

	for (int i = 0; i < m_room_config.Size(); i++)
	{
		stGameScene->add_roomconfigid(m_room_config[i]);
	}

	for (_uint8 i = 0; i < m_GAME_PLAYER; ++i)
	{
		stCurGameScence.add_userpaotou(m_UserPiaoScore[i]);
	}

	std::string str = stCurGameScence.SerializeAsString();
	save_video_data(SERVER_GAME_SCENE, str);
}

bool Table_jianlimj::IsQuanqiuren(const HandCards& pHc, MJ_fan_buffer& oFan, int& iOpType)
{
	return false;
}

void Table_jianlimj::UserLastOutCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{
	
	iOpType &= ~(TYPE_CHI | TYPE_GANG);
}

void Table_jianlimj::Calc_GangScore(_uint8 ChairID, int iScore[GAME_PLAYER], int iTotalWinScore)
{
	for (int i = 0; i < m_stGang[ChairID].Gang.Size(); i++)
	{
		int iType = m_stGang[ChairID].Gang[i].iType;
		_uint8 cbProvideUser = m_stGang[ChairID].Gang[i].ChairID;

		if (TYPE_ANGANG == iType)
		{
			for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
			{
				iScore[ChairID] += iTotalWinScore;
				iScore[c] -= iTotalWinScore;
			}
		}

		if (TYPE_WANGANG == iType)
		{
			for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
			{
				iScore[ChairID] += MyRound(iTotalWinScore / 2.0);
				iScore[c] -= MyRound(iTotalWinScore / 2.0);
			}
		}				

		if (TYPE_ZHIGANG == iType)
		{
			iScore[ChairID] += iTotalWinScore;
			iScore[cbProvideUser] -= iTotalWinScore;
		}		
	}

	MJ_BUFFER<_uint8, 10> tmpHua;
	tmpHua.Clear();
	for (int j = 0; j < m_Hua[ChairID].Size(); j++)
	{	
		_uint8 cbCard = m_Hua[ChairID][j];
		if ((m_Hua[ChairID].Counts(cbCard) >= 4) && !tmpHua.Find(cbCard))
		{
			tmpHua.Add(cbCard);
		}
	}

	for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
	{
		iScore[ChairID] += iTotalWinScore * tmpHua.Size();
		iScore[c] -= iTotalWinScore * tmpHua.Size();
	}

}

/*int Table_jianlimj::Calc_KengZhuang(_uint8 ChairID)
{
	if (m_conf_jianli.m_bKengZhuang)
	{
		if (m_bBanker == ChairID)
		{
			return 1;
		}
	}

	return 0;
}*/

int Table_jianlimj::Calc_WeiZeng()
{
	if (m_conf_jianli.m_bWeiZeng)
	{
		return m_conf_jianli.m_iWeiZeng;
	}

	return 0;
}

int Table_jianlimj::Calc_ChuZeng(_uint8 ChairID, _uint8 OtherChairID)
{
	if (m_UserPiaoScore[ChairID] >= 0 && m_UserPiaoScore[OtherChairID] >= 0)
	{
		return m_UserPiaoScore[ChairID] + m_UserPiaoScore[OtherChairID];
	}

	return 0;
}


int Table_jianlimj::Calc_SelfScore(_uint8 ChairID)
{
	return (1 + Calc_WeiZeng()/* + Calc_ChuZeng(ChairID) + Calc_KengZhuang(ChairID)*/ + m_nUserHuaCount[ChairID]);
}

/*void Table_jianlimj::Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard& stUsergetcard)
{
	//_uint8 tempChairid = m_SeatPlayer[ChairID].player->seatid;
	stUsergetcard.Clear();

	stUsergetcard.set_chairid(MoChairID);
	stUsergetcard.set_leftcardnum(m_CardPool.Count());
	if (m_bFirstCard)
	{
		stUsergetcard.set_card(INVALID_MJ_CARD);
	}

	//如果是摸牌用户，则打包摸牌、听牌等数据，非摸牌用户则不发送这些数据
	if (ChairID == MoChairID)
	{
		if (m_bFirstCard)
		{
			stUsergetcard.set_card(INVALID_MJ_CARD);
		}
		else
		{
			stUsergetcard.set_card(bCard);
		}

		if (m_bUserOperate[MoChairID] & TYPE_ANGANG || m_bUserOperate[MoChairID] & TYPE_WANGANG)
		{
			MJ_opinfo stGang;			
			GetGangInfo(MoChairID, stGang);

			for (int i = 0; i < stGang.Size(); i++)
			{
				//log.info("玩家还没胡牌,此时暗杠牌有:0x%02x \n", stGang[i].cbCard);
				log.info("%s uid:%d chairID:%d angang:0x%02x\n", __FUNCTION__, GetPlayerUid(ChairID), ChairID, stGang[i].cbCard);
				proto::game::tagGangInfo *pstGang = stUsergetcard.add_ganginfo();
				pstGang->set_card(stGang[i].cbCard);
				pstGang->set_state(stGang[i].nState);
			}			
		}
		//听牌判断 TODO
		tagListenInfos stListenInfo[14];
		_uint8 bListenLen = 0;		
		//胡牌不用判断听牌，自动胡牌不用判断听牌
		if (!(m_bUserOperate[ChairID] & TYPE_HU))
		{
			m_bUserOperate[ChairID] |= GetListeningInfo(ChairID, stListenInfo, bListenLen);
		}
		stUsergetcard.set_optype(m_bUserOperate[MoChairID]);
		if (m_bUserOperate[MoChairID] & TYPE_LISTEN)
		{
			//打包听牌信息
			for (int j = 0; j < bListenLen; j++)
			{
				PackageTingInfo(ChairID, stListenInfo, j, stUsergetcard.add_listeninfo());		

			}
		}
	}
}*/

// 玩家摸牌广播
void Table_jianlimj::Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard& stUsergetcard, MJ_major & major,bool buguohua)
{
	stUsergetcard.Clear();

	stUsergetcard.set_chairid(MoChairID);
	stUsergetcard.set_leftcardnum(m_CardPool.Count());
	if (m_bFirstCard || buguohua)
	{
		stUsergetcard.set_card(INVALID_MJ_CARD);
	}

	//如果是摸牌用户，则打包摸牌、听牌等数据，非摸牌用户则不发送这些数据
	if (ChairID == MoChairID)
	{
		if (m_bFirstCard || buguohua)
		{
			stUsergetcard.set_card(INVALID_MJ_CARD);
		}
		else
		{
			stUsergetcard.set_card(bCard);
		}

		if (m_bUserOperate[MoChairID] & TYPE_ANGANG || m_bUserOperate[MoChairID] & TYPE_WANGANG)
		{
			MJ_opinfo stGang;			
			for(int i=0;i<major.m_GetGangInfo.Size();i++)
			{
				stGang.Add(OpState(major.m_GetGangInfo[i].cbCard,major.m_GetGangInfo[i].nState));
			}

			for (int i = 0; i < stGang.Size(); i++)
			{
				//log.info("玩家还没胡牌,此时暗杠牌有:0x%02x \n", stGang[i].cbCard);
				log.info("%s tid:%d uid:%d chairID:%d angang:0x%02x\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, stGang[i].cbCard);
				proto::game::tagGangInfo *pstGang = stUsergetcard.add_ganginfo();
				pstGang->set_card(stGang[i].cbCard);
				pstGang->set_state(stGang[i].nState);
			}			
		}
		//听牌判断 TODO
		tagListenInfos stListenInfo[14];
		_uint8 bListenLen = 0;		
		//胡牌不用判断听牌，自动胡牌不用判断听牌
		if (!(m_bUserOperate[ChairID] & TYPE_HU))
		{
			m_bUserOperate[ChairID] |= GetListeningInfo(ChairID, stListenInfo, bListenLen);
		}
		stUsergetcard.set_optype(m_bUserOperate[MoChairID]);
		if (m_bUserOperate[MoChairID] & TYPE_LISTEN)
		{
			//打包听牌信息
			for (int j = 0; j < bListenLen; j++)
			{
				PackageTingInfo(ChairID, stListenInfo, j, stUsergetcard.add_listeninfo());
			}
		}
	}
}

bool Table_jianlimj::IsHuaPai(_uint8 cbCard)
{
	if (1 == m_conf_jianli.m_iHuaType)
	{
		if (0x41 == cbCard)
		{
			return true;
		}
	}
	else if (2 == m_conf_jianli.m_iHuaType)
	{
		if ((0x41 == cbCard) || (0x42 == cbCard))
		{
			return true;
		}
	}
	else if (3 == m_conf_jianli.m_iHuaType)
	{
		if ((0x41 == cbCard) || (0x43 == cbCard))
		{
			return true;
		}
	}

	return false;
}

void Table_jianlimj::Score_GameEndDraw(HuScoreSet& score, proto::game::AckGameEnd& stGameend)
{
	int iBaseScore[GAME_PLAYER][GAME_PLAYER];
	int iGangScore[GAME_PLAYER] = {0};

	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		Calc_BaseScore(i, iBaseScore[i]);
	}

	int iTotalWinScore = 0;
	//杠牌分数
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{		
		Calc_GangScore(i, iGangScore, iTotalWinScore);		
	}


	if (!m_conf.m_bHuangGang)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{	
			score[i].iTotleScore = iGangScore[i];
		}
	}	
}

/*void Table_jianlimj::UserOperateBuHua()
{
	bool r = false;

	Jl_HuaInfo HuaInfo[GAME_PLAYER];
	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		for (int j = 0; j < m_HandCard[i].ChangeableCardsLen; ++j)
		{
			if (IsHuaPai(m_HandCard[i].ChangeableCards[j]))
				HuaInfo[i].vecHua.push_back(m_HandCard[i].ChangeableCards[j]);
		}

		if (!HuaInfo[i].vecHua.empty())
		{
			for (size_t j = 0; j < HuaInfo[i].vecHua.size(); ++j)
			{
				_uint8 tmp = HuaInfo[i].vecHua[j];
				m_GameLogic.ChuPai(&m_HandCard[i], tmp);
				m_stUserOutCard[i].Add(tmp);

				tmp = GetOneCard();
				HuaInfo[i].vecGets.push_back(tmp);
				m_GameLogic.AddCard(&m_HandCard[i], tmp);
			}

			r = true;
		}
	}

	if (r)
	{
		for (int i = 0; i < m_GAME_PLAYER; ++i)
		{
			proto::game::jianlimj::AckBuHua stAckBuHua;
	
			stAckBuHua.set_leftcount(m_CardPool.Count());	

			for (int j = 0; j < m_GAME_PLAYER; ++j)
			{
				proto::game::jianlimj::Hua* pstHua = stAckBuHua.add_buhua();
				pstHua->set_chairid(j);
				for (size_t n = 0; n < HuaInfo[j].vecHua.size(); ++n)
				{
					pstHua->add_hua(HuaInfo[j].vecHua[n]);
				}

				if (j == i)
				{
					for (size_t n = 0; n < HuaInfo[j].vecGets.size(); ++n)
					{
						pstHua->add_getcard(HuaInfo[j].vecGets[n]);
					}
				}
			}

			unicast(SERVER_CHANGE_CARD, i, stAckBuHua);
		}

		//保存录像
		proto::game::jianlimj::AckBuHua stAckBuHuaVideo;
		stAckBuHuaVideo.set_leftcount(m_CardPool.Count());	
		for (int j = 0; j < m_GAME_PLAYER; ++j)
		{
			proto::game::jianlimj::Hua* pstHua = stAckBuHuaVideo.add_buhua();
			pstHua->set_chairid(j);
			for (size_t n = 0; n < HuaInfo[j].vecHua.size(); ++n)
			{
				pstHua->add_hua(HuaInfo[j].vecHua[n]);
			}
			for (size_t n = 0; n < HuaInfo[j].vecGets.size(); ++n)
			{
				pstHua->add_getcard(HuaInfo[j].vecGets[n]);
			}
		}	
		SaveVideoData(SERVER_CHANGE_CARD, stAckBuHuaVideo);
	}

	log.info("%s tid:%d HuaInfo.size:[%d, %d, %d, %d] .\n", __FUNCTION__, tid, 
		HuaInfo[0].vecHua.size(), HuaInfo[1].vecHua.size(), HuaInfo[2].vecHua.size(), HuaInfo[3].vecHua.size());
}*/

/*_uint8 Table_jianlimj::GetOneCard()
{
	int iCount = 1;
	while(true)
	{
		if (!IsHuaPai(m_CardPool[0])) 
			break;

		_uint8 tmp = m_CardPool[0];
		m_CardPool[0] = m_CardPool[iCount];
		m_CardPool[iCount] = tmp;

		iCount++;
	}

	return m_CardPool.GetOneCard();
}*/

void Table_jianlimj::set_out_timer_1(_uint8 ChairID, bool bDelayTimer,bool first)
{
	//log.info("start player uid:%d chairid:%d out card timer  \n", GetPlayerUid(ChairID), ChairID);
	if (!bDelayTimer)
	{
		if(first)
		{
			ev_timer_stop(zjh.loop, &m_Outcard_Timer);
			ev_timer_set(&m_Outcard_Timer, m_Outcard_Timer_stamp+8 , m_Outcard_Timer_stamp+8);
			ev_timer_again(zjh.loop, &m_Outcard_Timer);

		}
		else
		{
			ev_timer_stop(zjh.loop, &m_Outcard_Timer);
			ev_timer_set(&m_Outcard_Timer, m_Outcard_Timer_stamp+2 , m_Outcard_Timer_stamp+2);
			ev_timer_again(zjh.loop, &m_Outcard_Timer);

		}
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


void Table_jianlimj::set_operator_timer_1(_uint8 ChairID, bool bDelayTimer,bool first)
{
	//操作时钟
	switch (ChairID)
	{
		case 0:
		{			
			if (!bDelayTimer)
			{
				if(first)
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer0);
					ev_timer_set(&m_Operate_Timer0, m_Operate_Timer_stamp0+8, m_Operate_Timer_stamp0+8);

					ev_timer_again(zjh.loop, &m_Operate_Timer0);

				}
				else
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer0);
					ev_timer_set(&m_Operate_Timer0, m_Operate_Timer_stamp0+2, m_Operate_Timer_stamp0+2);

					ev_timer_again(zjh.loop, &m_Operate_Timer0);

				}
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
				if(first)
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer1);
					ev_timer_set(&m_Operate_Timer1, m_Operate_Timer_stamp1+8, m_Operate_Timer_stamp1+8);

					ev_timer_again(zjh.loop, &m_Operate_Timer1);

				}
				else
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer1);
					ev_timer_set(&m_Operate_Timer1, m_Operate_Timer_stamp1+2, m_Operate_Timer_stamp1+2);

					ev_timer_again(zjh.loop, &m_Operate_Timer1);

				}
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
				if(first)
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer2);
					ev_timer_set(&m_Operate_Timer2, m_Operate_Timer_stamp2+8, m_Operate_Timer_stamp2+8);

					ev_timer_again(zjh.loop, &m_Operate_Timer2);

				}
				else
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer2);
					ev_timer_set(&m_Operate_Timer2, m_Operate_Timer_stamp2+2, m_Operate_Timer_stamp2+2);

					ev_timer_again(zjh.loop, &m_Operate_Timer2);

				}
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
				if(first)
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer3);
					ev_timer_set(&m_Operate_Timer3, m_Operate_Timer_stamp3+8, m_Operate_Timer_stamp3+8);

					ev_timer_again(zjh.loop, &m_Operate_Timer3);

				}
				else
				{
					ev_timer_stop(zjh.loop, &m_Operate_Timer3);
					ev_timer_set(&m_Operate_Timer3, m_Operate_Timer_stamp3+2, m_Operate_Timer_stamp3+2);

					ev_timer_again(zjh.loop, &m_Operate_Timer3);

				}
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

void Table_jianlimj::DoSimpleOperate(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	log.info("DoSimpleOperate uid:%d chairid:%d, iOptype:0x%x \n", GetPlayerUid(ChairID), ChairID, iOpType);
	
	switch (iOpType)
	{
		case TYPE_PENG:
		UserOperatePeng(ChairID, bCard);
		{
			Jl_HuaInfo HuaInfo;
			for (int i = 0; i < m_HandCard[ChairID].ChangeableCardsLen; ++i)
			{
				if (IsHuaPai(m_HandCard[ChairID].ChangeableCards[i]))
				{
					HuaInfo.vecHua.push_back(m_HandCard[ChairID].ChangeableCards[i]);
				}
			}

			_uint8 cbHuaNum = HuaInfo.vecHua.size();

			bool buguohua = false;

			if (cbHuaNum > 0)
			{
				while (cbHuaNum != 0)
				{
					_uint8 bGetCard = INVALID_MJ_CARD;
					for (size_t j = 0; j < HuaInfo.vecHua.size(); j++)
					{
						if(CardPoolIsEmpty())
						{
							GameEnd();
							return;
						}

						_uint8 tmp = HuaInfo.vecHua[j];
						m_GameLogic.ChuPai(&m_HandCard[ChairID], tmp);
						m_stUserOutCard[ChairID].Add(tmp);
						m_Hua[ChairID].Add(tmp);
						m_bCurOutCard = tmp;


						bGetCard = m_CardPool.GetLast();
						HuaInfo.vecGets.push_back(bGetCard);
						m_GameLogic.AddCard(&m_HandCard[ChairID], bGetCard);
						m_bCurCard = bGetCard;

						m_bGetCardCount[ChairID]++;

						m_bCurChairID = ChairID;
						m_bOutCardChairID = ChairID;
					}
				
					for (int i = 0; i < m_GAME_PLAYER; ++i)
					{
						proto::game::jianlimj::AckBuHua stAckBuHua;

						stAckBuHua.set_leftcount(m_CardPool.Count());	

						proto::game::jianlimj::Hua* pstHua = stAckBuHua.mutable_buhua();
						pstHua->set_chairid(ChairID);
						for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
						{
							pstHua->add_hua(HuaInfo.vecHua[n]);
						}

						if (i == ChairID)
						{
							for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
							{
								pstHua->add_getcard(HuaInfo.vecGets[n]);
							}
						}
						else
						{
							for (size_t n = 0; n < HuaInfo.vecGets.size(); ++n)
							{
								pstHua->add_getcard(0);
							}
						}
						
						unicast(SERVER_CHANGE_CARD, i, stAckBuHua);
					}

					//保存录像
					proto::game::jianlimj::AckBuHua stAckBuHuaVideo;
					stAckBuHuaVideo.set_leftcount(m_CardPool.Count());

					proto::game::jianlimj::Hua* pstHua = stAckBuHuaVideo.mutable_buhua();
					
					pstHua->set_chairid(ChairID);
					for (size_t n = 0; n < HuaInfo.vecHua.size(); ++n)
					{
						pstHua->add_hua(HuaInfo.vecHua[n]);
					}
				
					SaveVideoData(SERVER_CHANGE_CARD, stAckBuHuaVideo);
					
					buguohua = true;

					HuaInfo.vecHua.clear();
					HuaInfo.vecGets.clear();

					cbHuaNum = 0;

					for (int i = 0; i < m_HandCard[ChairID].ChangeableCardsLen; ++i)
					{
						if (IsHuaPai(m_HandCard[ChairID].ChangeableCards[i]))
						{
							HuaInfo.vecHua.push_back(m_HandCard[ChairID].ChangeableCards[i]);
						}
					}

					cbHuaNum = HuaInfo.vecHua.size();


					if (cbHuaNum == 0)
					{
						MJ_major major;
						m_GameLogic.ChuPai(&m_HandCard[ChairID],bGetCard);
						m_bUserOperate[ChairID] = GetCardEstimate(ChairID, bGetCard, major, m_HuFanNums[ChairID]);
						m_GameLogic.AddCard(&m_HandCard[ChairID],bGetCard);

						proto::game::AckUserGetCard stUsergetcard;
						for (int i = 0; i < m_GAME_PLAYER; i++)
						{
							Proto_Game_UserGetCard((_uint8)i, ChairID, bGetCard, stUsergetcard,major,buguohua);
							stUsergetcard.set_optype2(1);
							unicast(SERVER_USER_GETCARD, i, stUsergetcard);	
										
							if (i == ChairID)	
							{
								SaveVideoData(SERVER_USER_GETCARD, stUsergetcard);
							}	
						}

						if (m_bUserOperate[ChairID] == TYPE_NULL || m_bUserOperate[ChairID] == TYPE_LISTEN)
						{
							set_out_timer(ChairID);
						}
						else
						{
							set_operator_timer(ChairID);
						}
					}
				}
			}
		}
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
		break;
		default:
		break;
	}	
}

void Table_jianlimj::Proto_GameEndInfo_GangInfo(_uint8 ChairID, proto::game::AckGameEnd& stGameend)
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

void Table_jianlimj::Send_Hand_Card()
{

}

void Table_jianlimj::AutoPiao_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table_jianlimj *table = (Table_jianlimj*)w->data;
	if (table)
	{		
		TIMER_STOP(table->m_AutoPiao_Timer);
		table->AutoPiaoTimeOut();
	}
}

void Table_jianlimj::AutoPiaoTimeOut()
{
	for (int i = 0; i < m_GAME_PLAYER; ++i)
	{
		if (m_UserPiaoScore[i] < 0)
		{
			m_UserPiaoScore[i] = 0;

			proto::game::jianlimj::AckBroadPiao stAck;
			stAck.set_chairid(i);
//			stAck.set_piaonum(m_UserPiaoScore[i]);
			broadcast(SERVER_BROAD_PIAO, stAck);
			SaveVideoData(SERVER_BROAD_PIAO, stAck);

			LogInfo(i, "m_UserPiaoScore:%d time out", m_UserPiaoScore[i]);
		}
	}

	int count = 0;
	for (int i=0; i<m_GAME_PLAYER; i++)
	{
		if (m_UserPiaoScore[i] >= 0)
			count++;
	}
	
	if (count == m_GAME_PLAYER)
	{	
		proto::game::jianlimj::AckChooseOver stAck;
		for (int i=0; i<m_GAME_PLAYER; i++)
		{
			stAck.add_piaonum(m_UserPiaoScore[i]);
		}

		broadcast(SERVER_CHOOSE_PIAO_OVER, stAck);

		std::string str = stAck.SerializeAsString();
		save_video_data(SERVER_CHOOSE_PIAO_OVER, str);

		GameStartAfterPiao();
	}
}


int Table_jianlimj::Calc_UserScore(_uint8 HuChairID, _uint8 OtherChairID)
{
	int iMaNum = 0;
	if (m_conf.m_buyHorseMojiJiangji)
	{
		for (_uint8 i = 0; i < m_zhong_niao.Size(); i++)
		{
			if (CardType(m_zhong_niao[i]) > MJ_TYPE_FENG)
			{
				iMaNum += 10;
			}
			else
			{
				iMaNum += CardNum(m_zhong_niao[i]);
			}
		}
	}
	else
	{
		iMaNum += m_zhong_niao.Size();
	}
	return (1 + Calc_WeiZeng() * 2 + Calc_ChuZeng(HuChairID, OtherChairID)
			 + m_Hua[HuChairID].Size() + m_Hua[OtherChairID].Size() + iMaNum);
}

bool Table_jianlimj::CardPoolIsEmpty()
{
	if (m_conf.m_bSupportBuyHorse)
	{
		if (m_CardPool.Count() <= m_conf.m_HorseNum)
		{
			return true;
		}
		return false;
	}
	else
	{
		return m_CardPool.IsEmpty();
	}
}

bool Table_jianlimj::IsLastGetCard()
{
	if (m_conf.m_bSupportBuyHorse)
	{
		if (m_CardPool.Count() == (m_conf.m_HorseNum + 1))
		{
			return true;
		}
		return false;
	}
	else
	{
		return (m_CardPool.Count() == 1);
	}
}


void Table_jianlimj::CalZhongMaNum()
{
	m_niao.Clear();
	m_zhong_niao.Clear();

	for (int i = 0; i < m_conf.m_HorseNum; i++)
	{
		_uint8 nTempCard = m_CardPool.GetOneCard();
		m_niao.Add(nTempCard);
	}

	if (m_conf.m_buyHorseMojiJiangji)
	{
		for (_uint8 i = 0; i < m_niao.Size(); i++)
		{
			m_zhong_niao.Add(m_niao[i]);
		}
	}
	else
	{
		for (_uint8 i = 0; i < m_niao.Size(); i++)
		{
			_uint8 num = CardNum(m_niao[i]);
			if (num == 1 || num == 5 || num == 9)
			{
				m_zhong_niao.Add(m_niao[i]);
			}
		}
	}

}

_uint8 Table_jianlimj::GetOneCard()
{
	int iCount = 1;
	while(true)
	{
		if (!IsHuaPai(m_CardPool[0])) 
			break;

		_uint8 tmp = m_CardPool[0];
		m_CardPool[0] = m_CardPool[iCount];
		m_CardPool[iCount] = tmp;

		iCount++;
	}

	return m_CardPool.GetOneCard();
}

int Table_jianlimj::MyRound(float f)
{ 
	return f >= 0 ? int (f+0.5) : int (f-0.5);
}
