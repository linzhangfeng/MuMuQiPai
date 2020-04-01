#include "proto/proto.h"
#include "table.h"
#include "zjh.h"
#include "common/log.h"
#include "libnormalmahjong/common_mahjong_logic.h"
#include "game.h"


extern ZJH zjh;
extern Log log;

#define MultiOpOr(X,Y)	(((X) & (Y) ? (Y) : 0) & (m_bMultiUserDoneOp[c] & (Y) ? (Y) :0))

void Table::UserOperateChi(_uint8 ChairID, int iOpType, _uint8 bCard)
{

	memset(m_bAddFanFlag, 0, sizeof(m_bAddFanFlag));
	memset(m_bUserOperate, 0, sizeof(m_bUserOperate));

	MJ_opinfo stChi;
	switch (iOpType)
	{
	case TYPE_LEFT_CHI:
	{
		//吃牌检查
		if (!(GetChi(ChairID, bCard, stChi) & TYPE_LEFT_CHI))
		{
			log.info("%s tid:%d ChairID:%d operate:0x%x，card: 0x%02x,but handcard has no cur operate \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
			return;
		}
		m_GameLogic.ExecuteLeftChi(&m_HandCard[ChairID], m_bOutCardChairID, bCard);		
	}
	break;
	case TYPE_CENTER_CHI:
	{
		//吃牌检查
		if (!(GetChi(ChairID, bCard, stChi) & TYPE_CENTER_CHI))
		{
			log.info("%s tid:%d ChairID:%d operate:0x%x，card: 0x%02x,but handcard has no cur operate \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
			return;
		}
		m_GameLogic.ExecuteCenterChi(&m_HandCard[ChairID], m_bOutCardChairID, bCard);
	}
	break;
	case TYPE_RIGHT_CHI:
	{
		//吃牌检查
		if (!(GetChi(ChairID, bCard, stChi) & TYPE_RIGHT_CHI))
		{
			log.info("%s tid:%d ChairID:%d operate:0x%x，card: 0x%02x,but handcard has no cur operate \n", __FUNCTION__, tid, ChairID, iOpType, bCard);
			return;
		}
		m_GameLogic.ExecuteRightChi(&m_HandCard[ChairID], m_bOutCardChairID, bCard);
	}
	break;
	default:
		break;
	}
	m_bGetCardCount[ChairID]++;

	m_stUserOutCard[m_bOutCardChairID].RemoveLast(bCard);	
	
	//广播操作结果的消息
	BroadcastOperateResult(ChairID, iOpType, bCard);
	log.info("%s tid:%d uid:%d ChairID:%d, bCard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bCard);

	SendTingInfo(ChairID);

	int score[GAME_PLAYER] = { 0 };
	save_video_operate(iOpType, ChairID, m_bOutCardChairID, bCard, score);

	UserAfterOperate(ChairID, iOpType, bCard);	

}

void Table::UserOperatePeng(_uint8 ChairID, _uint8 bCard)
{	
	log.info("%s tid:%d uid:%d ChairID:%d, bCard:0x%02x \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bCard);
	
	memset(m_bAddFanFlag, 0, sizeof(m_bAddFanFlag));	
	memset(m_bUserOperate, 0, sizeof(m_bUserOperate));

	BroadcastOperateResult(ChairID, TYPE_PENG, bCard);

	m_GameLogic.ExecutePeng(&m_HandCard[ChairID], m_bOutCardChairID, bCard);
	m_bGetCardCount[ChairID]++;

	//从打出牌删掉这张牌	
	m_stUserOutCard[m_bOutCardChairID].RemoveLast(bCard);

	//m_bOutCardCount[CardType(bCard)][CardNum(bCard)] += 2;

	//TODO 检查是否听牌
	SendTingInfo(ChairID);

	int score[GAME_PLAYER] = { 0 };
	save_video_operate(TYPE_PENG, ChairID, m_bOutCardChairID, bCard, score);
	
	UserAfterOperate(ChairID, TYPE_PENG, bCard);
}

int Table::OutCardEstimate(_uint8 ChairID, _uint8 bCard, MJ_major& major, int& fan_num)
{	
	int iOpType = TYPE_NULL;
	
	HandCards* pHc = &m_HandCard[ChairID];
		
	iOpType |= GetChi(ChairID, bCard, major.m_Chi);
		
	iOpType |= GetPengGang(ChairID, bCard, major.m_GetPengGang);

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
	else if (m_bRobGangHuJudge || (m_bRobWanGang  && m_bAddFanFlag[m_WangGangPlayer]))
	{
		m_GameLogic.AddCard(pHc, bCard);

		iOpType |= Check_Hu(ChairID, *pHc, major.m_oFan);

		if (iOpType & TYPE_HU)
		{
			UserQiangGangHu(ChairID, bCard, major.m_oFan);
			fan_num = Calc_Fan(ChairID, major.m_oFan);
		}
		m_GameLogic.ChuPai(pHc, bCard);
	}
	return iOpType;	
}

int Table::GetCardEstimate(_uint8 ChairID, _uint8 bCard, MJ_major& major, int& fan_num)
{
	int iOpType = TYPE_NULL;

	HandCards* pHc = &m_HandCard[ChairID];	
	m_GameLogic.AddCard(pHc, bCard);

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
 		if (m_bAddFanFlag[ChairID])   //杠上花
 		{
			UserGangShangHua(ChairID, bCard, major.m_oFan); 			
 		}
		fan_num = Calc_Fan(ChairID, major.m_oFan);
	}

	m_GameLogic.ChuPai(pHc, bCard);

	return iOpType;
}
void Table::InitBanker()
{
	if (m_bBanker >= m_GAME_PLAYER)
	{
		if (m_nPlayingTimes == 1)
		{
			for (int i = 0; i < m_GAME_PLAYER; i++)
			{
				if (m_SeatPlayer[i].player != NULL)
				{
					log.info("%s tid %d player uid:%d, room owner:%d\n", __FUNCTION__, tid, GetPlayerUid(i), m_room_owner_uid);
					if (m_SeatPlayer[i].player->uid == m_room_owner_uid)
					{
						m_bBanker = m_SeatPlayer[i].seatid;
						break;
					}
				}
			}
		}
	}
	if (m_bBanker >= m_GAME_PLAYER)
	{
		if (m_RandomSeat)
		{
			m_bBanker = rand() % m_GAME_PLAYER;
		}
		else
		{
			m_bBanker = 0;
		}
	}
}
void Table::TurnBanker()
{
	
	m_bBanker = NextChair(m_bBanker);

}

int Table::Check_Hu(_uint8 ChairID, HandCards& pHc, MJ_fan_buffer& oFan)
{
	//取余3不等于2的手牌不做胡牌判断
	if (pHc.ChangeableCardsLen != 0 && pHc.ChangeableCardsLen % 3 != 2)
	{
		return TYPE_NULL;
	}		
	MJ_win_pattern pattern;
	if (Is7Dui(ChairID, pHc, oFan))
	{
		return TYPE_HU;
	}	
	else if (Is13Yao(pHc, oFan, pattern))
	{		
		return TYPE_HU;
	}
	if (FormatCards(&pHc, pattern))
	{
		return Check_Hu_Pattern(ChairID, pHc, oFan, pattern);
	}
	
	return TYPE_NULL;
}
int Table::Check_Hu_Pattern(_uint8 ChairID, const HandCards& pHc, MJ_fan_buffer& oFan, const MJ_win_pattern& oPattern)
{
	oFan.Add(MJ_FAN_TYPE_PINGHU);
	return TYPE_HU;
}

int Table::Calc_Fan(_uint8 ChairID, MJ_fan_buffer& oFan)
{
	
	return 0;
}
int Table::Calc_Hu(_uint8 ChairID, _uint8 bCard, int Score[GAME_PLAYER])
{
	m_oFan[ChairID].Clear();	
	m_HuFanNums[ChairID] = 0;

	MJ_major major;
	if (m_bRobHu)
	{
		int iOpType = OutCardEstimate(ChairID, bCard, major, m_HuFanNums[ChairID]);
		if (iOpType & TYPE_HU)
		{
			m_oFan[ChairID] = major.m_oFan;
			Score[ChairID] += m_HuFanNums[ChairID] * m_BaseScore;
			Score[m_WangGangPlayer] -= m_HuFanNums[ChairID] * m_BaseScore;

			m_stHuScore[ChairID].stHu.Add(tagHuRecord(m_WangGangPlayer, bCard, HU_PAOHU, m_HuFanNums[ChairID]));
			return 0;
		}
	}
	else if (ChairID == m_bCurChairID)
	{//自摸
		int iOpType = GetCardEstimate(ChairID, INVALID_MJ_CARD, major, m_HuFanNums[ChairID]);
		if (iOpType & TYPE_HU)
		{			
			m_oFan[ChairID] = major.m_oFan;
			for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
			{
				Score[ChairID] += m_HuFanNums[ChairID] * m_BaseScore;
				Score[c] -= m_HuFanNums[ChairID] * m_BaseScore;
			}
			m_stHuScore[ChairID].stHu.Add(tagHuRecord(ChairID, bCard, HU_ZIMO, m_HuFanNums[ChairID]));
			return 0;
		}		
	}
	else
	{//放炮
		int iOpType = OutCardEstimate(ChairID, bCard, major, m_HuFanNums[ChairID]);
		if (iOpType & TYPE_HU)
		{
			m_oFan[ChairID] = major.m_oFan;
			Score[ChairID] += m_HuFanNums[ChairID] * m_BaseScore;			
			Score[m_bCurChairID] -= m_HuFanNums[ChairID] * m_BaseScore;

			m_stHuScore[ChairID].stHu.Add(tagHuRecord(m_bCurChairID, bCard, HU_PAOHU, m_HuFanNums[ChairID] ));
			return 0;
		}		
	}
	
	log.error("%s tid:%d ChairID:%d bCard:0x%x estimate error .\n", __FUNCTION__, tid, ChairID, bCard);
	return -1;
}
void Table::UserGetFirstCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{
	if (iOpType & TYPE_HU)
	{
		if (ChairID == m_bBanker)
		{
			oFan.Add(MJ_FAN_TYPE_TIANHU);
		}		
	}	
}
void Table::UserFirstOutCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{
	if (iOpType & TYPE_HU && ChairID != m_bBanker)
	{
		oFan.Add(MJ_FAN_TYPE_DIHU);
	}
}

void Table::UserGetLastCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{
	if (iOpType & TYPE_HU)
	{
		oFan.Add(MJ_FAN_TYPE_HAIDILAOYUE);
	}

	//最后一张牌不能杠
	iOpType &= ~(TYPE_ANGANG | TYPE_WANGANG);	

}
void Table::UserLastOutCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan)
{
	
	iOpType &= ~(TYPE_PENG | TYPE_CHI | TYPE_GANG);
}
void Table::UserGangShangHua(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan)
{
	oFan.Add(MJ_FAN_TYPE_GANGSHANGHUA);
}
void Table::UserGangHouPao(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan)
{
	oFan.Add(MJ_FAN_TYPE_GANGHOUPAO);
}
void Table::UserQiangGangHu(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan)
{
	oFan.Add(MJ_FAN_TYPE_QIANGGANGHU);
}

void Table::Proto_Game_OpNotify(_uint8 ChairID, _uint8 CurChairID, _uint8 bCard, proto::game::AckOpNotify& stNotify)
{
	
	stNotify.Clear();
	stNotify.set_chairid(ChairID);
	//非提示玩家,下面的协议字段为空
	if (CurChairID == ChairID)
	{
		stNotify.set_optype(m_bUserOperate[ChairID]);
		stNotify.set_card(bCard);
		stNotify.set_operateid(++m_UserOperateID[ChairID]);		
	}

}
void Table::Proto_Game_OpNotifyAfterPengChi(_uint8 ChairID, _uint8 CurChairID, const MJ_opinfo& gang, proto::game::AckOpNotify& stNotify)
{
	stNotify.Clear();
	stNotify.set_chairid(ChairID);
	//非提示玩家,下面的协议字段为空
	if (CurChairID == ChairID)
	{
		stNotify.set_optype(m_bUserOperate[ChairID]);
		stNotify.set_card(0);
		for (int i = 0; i < gang.Size(); i++)
		{
			proto::game::tagGangInfo* ganginfo = stNotify.add_gang();
			ganginfo->set_card(gang[i].cbCard);
			ganginfo->set_state(gang[i].nState);
		}
		stNotify.set_operateid(++m_UserOperateID[ChairID]);	
	}
}
void Table::Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard& stUsergetcard)
{
	//_uint8 tempChairid = m_SeatPlayer[ChairID].player->seatid;
	stUsergetcard.Clear();

	stUsergetcard.set_chairid(MoChairID);
	stUsergetcard.set_leftcardnum(m_CardPool.Count());

	//如果是摸牌用户，则打包摸牌、听牌等数据，非摸牌用户则不发送这些数据
	if (ChairID == MoChairID)
	{
		stUsergetcard.set_card(bCard);

		if (m_bUserOperate[MoChairID] & TYPE_ANGANG || m_bUserOperate[MoChairID] & TYPE_WANGANG)
		{
			MJ_opinfo stGang;			
			GetGangInfo(MoChairID, stGang);

			for (int i = 0; i < stGang.Size(); i++)
			{
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

		stUsergetcard.set_md5(GetMd5(&m_HandCard[ChairID]));
		stUsergetcard.set_operateid(++m_UserOperateID[ChairID]);
	}
}

void Table::Proto_Game_UserOutCard(_uint8 ChairID, _uint8 bCard, proto::game::AckUserOutCard& stUseroutcard)
{
	stUseroutcard.Clear();
	stUseroutcard.set_chairid(ChairID);
	stUseroutcard.set_card(bCard);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::tagShowOutCard *pstShow = stUseroutcard.add_showcard();
		for (int j = 0; j < m_stUserOutCard[i].Size(); j++)
		{
			pstShow->add_outcard(m_stUserOutCard[i][j]);
		}
		pstShow->set_outcardlen(m_stUserOutCard[i].Size());
	}
}

void Table::Proto_Game_OperateGang(_uint8 ChairID, _uint8 bCard, int iOpType, int iGangScore[GAME_PLAYER], proto::game::AckBcOpResult& stOpresult)
{
	stOpresult.set_chairid(ChairID);
	stOpresult.set_outcardchairid(m_bOutCardChairID);
	stOpresult.set_optype(iOpType);
	stOpresult.set_card(bCard);

	if (m_bGangCalcImme || m_conf.m_bShowGangScore)
	{
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			stOpresult.add_score(iGangScore[i]);
		}
	}

	if (m_bGangCalcImme)
	{
		Update_Score(iGangScore);
	}
}

void Table::SendTingInfo(_uint8 ChairID)
{
	tagListenInfos stListenInfo[14];
	_uint8 bListenLen = 0;	
	//胡牌不用判断听牌，自动胡牌不用判断听牌
	if (GetListeningInfo(ChairID, stListenInfo, bListenLen))
	{
		m_bUserOperate[ChairID] |= TYPE_LISTEN;
	}
	
	// 发送听牌协议
	proto::game::AckListenNotify stListen;
	if (m_bUserOperate[ChairID] & TYPE_LISTEN)
	{
		log.info("%s tid:%d uid:%d chairid:%d ListenLen:%d \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bListenLen);
		stListen.set_optype(TYPE_LISTEN);
		//打包听牌信息
		for (int j = 0; j < bListenLen; j++)
		{
			PackageTingInfo(ChairID, stListenInfo, j, stListen.add_listeninfo());
		}
	}
	else
	{
		stListen.set_optype(TYPE_NULL);
		log.info("%s tid:%d uid:%d chairid:%d no listen\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID);
	}
	unicast(SERVER_LISTEN_NOTIFY, ChairID, stListen);
	
}

void Table::SendTingInfo_Simple(_uint8 ChairID)
{
	tagListenInfos stListenInfo[14];
	_uint8 bListenLen = 0;
	//胡牌不用判断听牌，自动胡牌不用判断听牌
	if (IsListening(ChairID, stListenInfo, bListenLen))
	{
		m_bUserOperate[ChairID] |= TYPE_LISTEN;
	}

	// 发送听牌协议
	proto::game::AckListenNotify stListen;
	if (m_bUserOperate[ChairID] & TYPE_LISTEN)
	{
		log.info("%s tid:%d uid:%d chairid:%d ListenLen:%d \n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, bListenLen);
		stListen.set_optype(TYPE_LISTEN);

		PackageTingInfo(ChairID, stListenInfo, 0, stListen.add_listeninfo());
		
	}	
	unicast(SERVER_LISTEN_NOTIFY, ChairID, stListen);
}
void Table::UserAfterOperate(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	m_bCurChairID = ChairID;
	m_bUserOperate[ChairID] &= TYPE_LISTEN;

	MJ_opinfo gang;
	m_bUserOperate[ChairID] |= GetGangInfo(m_bCurChairID, gang);
	
	log.info("%s tid:%d uid:%d chairid:%d iOpType:0x%02x card:0x%02x m_bUserOperate:0x%02x \n", 
		__FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, iOpType, bCard, m_bUserOperate[ChairID]);

	if (gang.Size() == 0)
	{
		set_out_timer(m_bCurChairID);
	}
	else
	{
		proto::game::AckOpNotify stNotify;		
		for (int i = 0; i < m_GAME_PLAYER; i++)
		{
			Proto_Game_OpNotifyAfterPengChi(ChairID, i, gang, stNotify);		
			unicast(SERVER_OPERATE_NOTIFY, i, stNotify);

			if (i == ChairID) save_operate_notify(stNotify);
		}
		set_operator_timer(ChairID);
	}	
}

bool Table::MulitFirstOpIsOver()
{
	if (!m_conf.m_bMulitFirstOpIsOver)
		return false;

	//非一炮多响优先级高的胡牌了直接结束
	if (!m_conf.m_bYiPaoDuoXiang)
	{
		for (_uint8 c = NextChair(m_bCurChairID); c != m_bCurChairID; c = NextChair(c))
		{
			if (m_bUserOperate[c] & TYPE_HU)
			{
				if (m_bMultiUserDoneOp[c] == TYPE_NULL)
					break;
				if (m_bMultiUserDoneOp[c] == TYPE_HU)
				{
					UserOperateHu(c, m_bCurCard);
					init_multi_data();
					GameEnd();	
					return true;
				}
			}		
		}	
	}

	return false;
}

void Table::DoMultiOperate(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	m_bMultiDoneOpNum++;
	m_bMultiUserDoneOp[ChairID] = iOpType;
	m_bMultiUserDoneOpCard[ChairID] = bCard;
	
	log.info("%s tid:%d uid:%d chaird:%d, curoptype:0x%x, optype:0x%x, bCard:0x%x now done OpNum:%d, totalOpnum:%d \n", __FUNCTION__,
		tid, GetPlayerUid(ChairID), ChairID, m_bUserOperate[ChairID], iOpType, bCard, m_bMultiDoneOpNum, m_bMultiOpUserNum);
	
	if (iOpType == TYPE_HU)
	{
		m_bMulitFirstOperate = true;		
		m_bMulitHuOpeCount++;
		
		for (_uint8 c = NextChair(ChairID); c != ChairID; c = NextChair(c))
		{
			if (c == m_bCurChairID)
				continue;
			if (m_bUserOperate[c] & TYPE_HU)
				continue;
			if (m_bUserOperate[c] == TYPE_NULL)
				continue;
			if (m_bMultiUserDoneOp[c] == TYPE_NULL)
			{
				m_bMultiDoneOpNum++;
				m_bMultiUserDoneOp[c] = TYPE_PASS;
				UserOperatePass(c, TYPE_PASS, bCard);
			}
		}
		if (m_conf.m_bYiPaoDuoXiang)
		{
			UserOperateHu(ChairID, m_bCurCard);

			if (m_conf.m_bFourceHuMutilHu)
			{
				DoMultiForceHu(ChairID, m_bCurCard);
			}
		}
	}
	else
	{
		if (iOpType == TYPE_PASS)
		{
			if (m_bUserOperate[ChairID] & TYPE_HU)   //胡牌玩家选择了过牌
			{
				m_bMulitHuOpeCount++;
				m_PassHuPlayer.Add(ChairID);
			}
			UserOperatePass(ChairID, iOpType, bCard);
		}
		else
		{
			if (iOpType != TYPE_NULL)
			{				
				if (
					(m_bRecordMultiOp.iOpType == TYPE_NULL && (iOpType & (TYPE_CHI | TYPE_PENG | TYPE_GANG | TYPE_HU))) ||
					((iOpType & TYPE_GANG) && (m_bRecordMultiOp.iOpType & (TYPE_CHI | TYPE_PENG))) ||
					((iOpType & TYPE_PENG) && (m_bRecordMultiOp.iOpType & (TYPE_CHI))) 					
					)					
				{
					m_bRecordMultiOp.iOpType = iOpType;
					m_bRecordMultiOp.ChairID = ChairID;
					m_bRecordMultiOp.bCard = bCard;
				}			
			}
		}
	}
	if (m_bMultiDoneOpNum >= m_bMultiOpUserNum)
	{
		if (m_bMulitFirstOperate)
		{			
			if (!m_conf.m_bYiPaoDuoXiang)
			{
				for (_uint8 c = NextChair(m_bCurChairID); c != m_bCurChairID; c = NextChair(c))
				{
					if (m_bMultiUserDoneOp[c] == TYPE_HU)
					{
						UserOperateHu(c, m_bCurCard);
						break;
					}					
				}				
			}
			init_multi_data();
			GameEnd();
		}
		else
		{
			if (m_bRecordMultiOp.iOpType != TYPE_NULL)
			{				
				MultiOperateOver(m_bRecordMultiOp.ChairID, m_bRecordMultiOp.iOpType, m_bRecordMultiOp.bCard);	
				init_multi_data();							
			}
			else
			{
				MultiOperateOver(ChairID, iOpType, bCard);	
				init_multi_data();			
				
				OnUserGetCard(NextChair(m_bCurChairID));
			}
		}
	}
	else//优先级操作了，优先级低的就忽略了
	{
		if (m_bMulitFirstOperate)
		{
			if (m_bMulitHuOpeCount != m_bMultiOpHuNum)
			{
				MulitFirstOpIsOver();
				return;
			}
			/*未到达*/
			if (m_conf.m_bYiPaoDuoXiang)
			{//一炮多响,其他玩家自动胡
				
			}
			else
			{
				for (_uint8 c = NextChair(m_bCurChairID); c != m_bCurChairID; c = NextChair(c))
				{
					if (m_bMultiUserDoneOp[c] == TYPE_HU)
					{
						UserOperateHu(c, m_bCurCard);
						break;
					}
				}
				
			}
			init_multi_data();
			GameEnd();
		}
		else
		{
			bool bOver = true;			
			for (_uint8 c = NextChair(m_bCurChairID); c != m_bCurChairID; c = NextChair(c))
			{
				if (m_bMultiUserDoneOp[c] != TYPE_NULL)
					continue;
				int tmpOpType = m_bUserOperate[c];
				if (tmpOpType == TYPE_NULL)
					continue;
				if (
					(m_bRecordMultiOp.iOpType == TYPE_NULL && (tmpOpType & (TYPE_CHI | TYPE_PENG | TYPE_GANG | TYPE_HU))) ||
					(tmpOpType & TYPE_HU) ||		
					(tmpOpType & TYPE_GANG && (m_bRecordMultiOp.iOpType & (TYPE_CHI | TYPE_PENG | TYPE_GANG))) ||
					(tmpOpType & TYPE_PENG && (m_bRecordMultiOp.iOpType & (TYPE_CHI | TYPE_PENG))) 					
					)
				{
					bOver = false;
					break;
				}
			}
			if (bOver)   //优先级高的玩家操作后，立即停止多操作
			{
				for (int i = 0; i < m_GAME_PLAYER; i++)  //其他低操作玩家服务器主动发过操作
				{
					if (i == m_bRecordMultiOp.ChairID)
					{
						continue;
					}
					if (m_bUserOperate[i] != TYPE_NULL && m_bMultiUserDoneOp[i] == TYPE_NULL)
					{
						UserOperatePass(i, TYPE_PASS, bCard);
					}

				}
				MultiOperateOver(m_bRecordMultiOp.ChairID, m_bRecordMultiOp.iOpType, m_bRecordMultiOp.bCard);
				init_multi_data();
			}

		}
	}
}

void Table::init_multi_data()
{
	m_bMulitFirstOperate = false;//数据初始化
	m_bRecordMultiOp.iOpType = TYPE_NULL;
	m_bRecordMultiOp.ChairID = 0xFF;
	m_bRecordMultiOp.bCard = 0xFF;
	m_bMultiDoneOpNum = 0;
	m_bMultiOpUserNum = 0;
	m_bMulitHuOpeCount = 0;
	m_bMultiOpHuNum = 0;
	memset(m_bMultiUserDoneOp, 0, sizeof(m_bMultiUserDoneOp));
	memset(m_bMultiUserDoneOpCard, 0, sizeof(m_bMultiUserDoneOpCard));
	m_PassHuPlayer.Clear();
}

void Table::MultiOperateOver(_uint8 ChairID, int iOpType, _uint8 bCard)
{
	if (iOpType == TYPE_PASS)
	{
		m_bAddFanFlag[m_bOutCardChairID] = false;
		return;
	}

	for (_uint8 c = NextChair(m_bCurChairID); c != ChairID;c = NextChair(c))
	{
		if (m_bMultiUserDoneOp[c] == TYPE_PASS)
			continue;
		if (MultiOpOr(iOpType, TYPE_GANG))		
		{
			ChairID = c;
			iOpType = m_bMultiUserDoneOp[c];
			bCard = m_bMultiUserDoneOpCard[c];
			break;
		}
		else if (MultiOpOr(iOpType, TYPE_PENG))
		{
			ChairID = c;
			iOpType = m_bMultiUserDoneOp[c];
			bCard = m_bMultiUserDoneOpCard[c];
			break;
		}
		else if (MultiOpOr(iOpType, TYPE_CHI))
		{
			ChairID = c;
			iOpType = m_bMultiUserDoneOp[c];
			bCard = m_bMultiUserDoneOpCard[c];
			break;
		}
	}
	switch (iOpType)
	{
	case TYPE_LEFT_CHI:
	case TYPE_CENTER_CHI:
	case TYPE_RIGHT_CHI:	
		UserOperateChi(ChairID, iOpType, bCard);
		break;
	case TYPE_PENG:	
		UserOperatePeng(ChairID, bCard);
		break;	
	case TYPE_WANGANG:
	case TYPE_ANGANG:		
	case TYPE_ZHIGANG:
		if (m_conf.m_bQiangZhiGang && RobGangHu(ChairID, bCard, iOpType))
			return;
		UserOperateGang(ChairID, iOpType, bCard);
		init_multi_data();	
		OnUserGetCard(ChairID, true);
		break;	
	default:
		break;
	}
}
int Table::GetChi(_uint8 ChairID, _uint8 bCard, MJ_opinfo& ChiInfo)
{
	if (m_conf.m_bSupportChi)
	{
		if (ChairID != NextChair(m_bOutCardChairID)) return TYPE_NULL;

		return m_GameLogic.GetChi(&m_HandCard[ChairID], bCard, ChiInfo);
	}
	return TYPE_NULL;	
}
int Table::GetGangInfo(_uint8 ChairID, MJ_opinfo& GangCardInfo)
{
	return m_GameLogic.GetGangInfo(&m_HandCard[ChairID], GangCardInfo);
}
int Table::GetPengGang(_uint8 ChairID, _uint8 cbCard, MJ_opinfo& GangCardInfo)
{
	return m_GameLogic.GetPengGang(&m_HandCard[ChairID], cbCard, GangCardInfo);	
}

bool Table::FormatCards(HandCards *pHc, MJ_win_pattern& pattern)
{
	return m_GameLogic.FormatCards(pHc, pattern);
}

bool Table::Is7Dui(_uint8 ChairID, HandCards& pHc, MJ_fan_buffer& oFan)
{
	if (m_conf.m_bHu7dui && m_GameLogic.Is7Dui(&pHc))
	{		
		oFan.Add(MJ_FAN_TYPE_QIDUI);
		return true;
	}
	return false;
}

bool Table::Is13Yao(HandCards& pHc, MJ_fan_buffer& oFan, MJ_win_pattern& pattern)
{
	if (m_conf.m_bHu13yao && m_GameLogic.Is13Yao(&pHc, INVALID_MJ_CARD, pattern.kan, pattern.jiang))
	{
		oFan.Add(MJ_FAN_TYPE_13YAO);
		return true;
	}
	return false;
}

bool Table::IsListening(_uint8 ChairID, tagListenInfos stListenInfo[14], _uint8 &bListenLen)
{	
	HandCards tmp = m_HandCard[ChairID];
	memset(stListenInfo, 0, sizeof(tagListenInfos) * 14);
	bListenLen = 0;
	Check_Listening(&tmp, ChairID, 0, stListenInfo, bListenLen);

	return bListenLen != 0;
}

void Table::SetRobGangHu_Flag(_uint8 ChairID, int iOpType, proto::game::AckBcOpResult& stOpresult)
{
	if (Is_HuOperator(iOpType) && m_bRobWanGang && m_bAddFanFlag[m_WangGangPlayer])
	{
		stOpresult.set_isqiangganghu(1);
	}
}

bool Table::CardPoolIsEmpty()
{
	return m_CardPool.IsEmpty();
}

bool Table::CheckPlayerSkey(const Json::Value& val, const Client* client)
{
	std::map<int, Player*>::iterator it;
	if ((it = m_mapTable_Player.find(client->uid)) != m_mapTable_Player.end())
	{
		Player* player = it->second;
		if (player == NULL)
			return false;
		if (!val["skey"].isNull() && val["skey"].asString() == client->skey)
		{
			log.info("%s tid:%d reconnect skey %s success!\n", __FUNCTION__, tid, client->skey.c_str());
			player->skey = val["skey"].asString();
			return true;
		}
	}
	return false;
}

int Table::dispatch_client(int cmd, Client *client, Player *player)
{
	if (client == NULL || player == NULL)
	{
		log.error("%s:%d\t%s tid:%d client has disbanded \n",__FILE__,__LINE__,__FUNCTION__ , tid);
		return -1;
	}
	switch (cmd)
	{
		case CLIENT_GAME_END_RECORD_REG:
		{
			if (m_bDeleteTable || (m_bGameState == GAME_FREE && m_nCurrentCount >= m_nTotalCount))
			{
				proto::game::GameEndRecord ger;		
				Proto_GameEnd_ScoreInfo(player, ger);
				unicast(SERVER_GAME_RECORD, player, ger);
			}
			return 0;
		}
		case CLIENT_REQ_ROUND_SCORE:
			Send_GameRound_ScoreInfo(player);
			return 0;	
		case CLIENT_REQ_CHANGE_MODE:
			RecvClientChangeMode(client, player);
			return 0;
        case CLIENT_REAL_TIME_SPEECH:
            recv_client_realtime_speech(client, player);
            return 0;
		default:
			break;
	}
	//log.error("%s tid:%d invalid command[%d] uid:%d\n", __FUNCTION__, tid, cmd, player->uid);
	return -1;
}

int Table::recv_client_realtime_speech(Client *client, Player *player)
{
    if (player->seatid >= m_GAME_PLAYER || player->seatid < 0)
    {
        log.error("%s, tid:%d but chaird:%d is wrong \n", __FUNCTION__, tid, player->seatid);
        return -1;
    }

    proto::game::RealTimeSpeech  msg;
    if (msg.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length))
    {
        m_voice_status[player->seatid] = msg.code();

        proto::game::RealTimeSpeechAck ack;
        for ( int i = 0 ; i < m_GAME_PLAYER ; ++i  )
        {
            ack.add_code(m_voice_status[i]);
        }
        log.info("%s tid:%d m_voice_status[%d,%d,%d,%d]\n", __FUNCTION__, tid , m_voice_status[0] , m_voice_status[1], m_voice_status[2], m_voice_status[3]);
        broadcast(SERVER_REAL_TIME_SPEECH, ack);
    }
    else
    {
        log.error("%s tid:%d err!\n", __FUNCTION__, tid);
        return -1;
    }
    return 0;
}

void Table::SendPlayerFirstCardOp(_uint8 ChairID)
{
	MJ_major major;
	m_bUserOperate[ChairID] = GetCardEstimate(ChairID, INVALID_MJ_CARD, major, m_HuFanNums[ChairID]);
	for (int i = 0; i < m_GAME_PLAYER; i++)
	{
		proto::game::AckOpNotify stNotify;
		stNotify.Clear();

		stNotify.set_chairid(ChairID);
		if (ChairID == i)
		{//非打牌玩家
			int iOpType = m_bUserOperate[ChairID];
			if (m_bUserOperate[ChairID] & TYPE_GANG)
			{
				for (int i = 0; i < major.m_GetGangInfo.Size(); i++)
				{
					log.info("%s tid:%d uid:%d chairID:%d angang:0x%02x\n", __FUNCTION__, tid, GetPlayerUid(ChairID), ChairID, major.m_GetGangInfo[i].cbCard);
					proto::game::tagGangInfo *pstGang = stNotify.add_gang();
					pstGang->set_card(major.m_GetGangInfo[i].cbCard);
					pstGang->set_state(major.m_GetGangInfo[i].nState);
				}

			}
			stNotify.set_card(m_bCurCard);
			stNotify.set_optype(iOpType);
		}
		unicast(SERVER_OPERATE_NOTIFY, i, stNotify);

		save_operate_notify(stNotify);
	}
}

void Table::GetFirstHandCard_NoOp(_uint8 ChairdID)
{
	_uint8 bCard = m_CardPool.GetOneCard();

	m_bCurChairID = ChairdID;
	m_bGetCardCount[ChairdID]++;
	m_bCurCard = bCard;

	m_GameLogic.AddCard(&m_HandCard[ChairdID], bCard);

	save_user_get_card(ChairdID, bCard, m_CardPool.Count());

	//广播庄家获得手牌
	proto::game::AckUserGetCard stUsergetcard;
	stUsergetcard.set_chairid(ChairdID);
	stUsergetcard.set_leftcardnum(m_CardPool.Count());
	for (_uint8 c = NextChair(ChairdID); c != ChairdID; c = NextChair(c))
	{
		unicast(SERVER_USER_GETCARD, c, stUsergetcard);
	}
	stUsergetcard.set_card(bCard);
	stUsergetcard.set_optype(TYPE_NULL);

	unicast(SERVER_USER_GETCARD, ChairdID, stUsergetcard);
}

int Table::Check_DaJiao(_uint8 ChairID)
{
	HandCards tempHandCard = m_HandCard[ChairID];
	
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

	int iMaxFan = 0;
	for (_uint8 i = 0; i < _relativity.Size(); ++i)
	{
		m_GameLogic.AddCard(&tempHandCard, _relativity[i]);
		MJ_fan_buffer fan;
		
		if (Check_Hu(ChairID, tempHandCard, fan) == TYPE_HU)
		{
			int tmpFan = Calc_Fan(ChairID, fan);
			if (iMaxFan < tmpFan)
			{
				iMaxFan = tmpFan;
				m_oFan[ChairID] = fan;
			}
		}
		m_GameLogic.ChuPai(&tempHandCard, _relativity[i]);
	}
	m_HuFanNums[ChairID] = iMaxFan;
	return iMaxFan;
}
bool Table::IsQuanqiuren(const HandCards& pHc, MJ_fan_buffer& oFan, int& iOpType)
{
	if (!m_conf.m_bNoSupport_Quanqiuren &&	m_GameLogic.IsQuanqiuren(&pHc))
	{
		oFan.Add(MJ_FAN_TYPE_QUANQIUREN);
		iOpType |= TYPE_HU;
		return true;
	}
	return false;
}

bool Table::VectorFind(std::vector<_uint8> v, _uint8 c)
{
	if (std::find(v.begin(), v.end(), c) != v.end())
		return true;

	return false;
}