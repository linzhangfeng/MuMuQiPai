/*------------------------------------------
File Name		:   SCMahJongLogic.h
Author			:	chenyonghua
Version			:	1.0
Date			:	2016.05.23
Descripition	:	牌型判断逻辑
-------------------------------------------*/

#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__
#include "common.h"
#include "mjcardtype.h"
#include "mj_common2.h"
#include "mj_common4.h"
#include <vector>
#include <algorithm>

//筛子
struct Shai_Set
{
	_uint8 Num;
	_uint8 Shai[7];	
	Shai_Set() 
	{		
		Num = 0;
		memset(Shai, 0, sizeof(Shai));
	}
};
struct OpState
{	
	_uint8 cbCard;
	int    nState;
	_uint8 Cards[4];  //用于获取四个不一样的杠牌或者三个不一样的碰牌或者非规律的吃牌
	OpState()
	{
		cbCard = INVALID_MJ_CARD;
		nState = 0;
		Cards[0] = 0; Cards[1] = 0; Cards[2] = 0; Cards[3] = 0;
	}
	OpState(_uint8 card, int state) : cbCard(card), nState(state)
	{
	}
	OpState(_uint8 card, int state, _uint8 c[4])
	{
		cbCard = card;
		nState = state;
		Cards[0] = c[0]; Cards[1] = c[1]; Cards[2] = c[2]; Cards[3] = c[3];
	}	
	bool operator==(const OpState& other)const
	{
		return cbCard == other.cbCard && nState == other.nState;
	}
};

typedef MJ_BUFFER<OpState, 100> MJ_opinfo;

class CGameLogic
{
public:
	CGameLogic(void);
	~CGameLogic(void);
	
public:
    /**
     *  @function: RandomCard
     *  @brief: 打乱牌
     *  @cbCard: 要打乱的牌
     *  @cbLength: 牌的个数
     *  @return: 无
    **/
	void RandomCard(_uint8 cbCard[], _uint8 cbLength, int iTableID);
    /**
     *  @function: AddCard
     *  @brief: 把牌加到手牌种
     *  @pHc: 手牌
     *  @bCard: 要加入手牌的牌
     *  @return: 无
    **/
	void AddCard(HandCards *pHc, _uint8 bCard);
    /**
     *  @function: ChuPai
     *  @brief: 出牌
     *  @pHc: 玩家手牌
     *  @bCard: 要打出的牌
     *  @return: 是否能成功打出该牌
    **/
	bool ChuPai(HandCards *pHc, _uint8 bCard);

	void ExecuteRobGang(HandCards *pHc, _uint8 bOpCardChairID, _uint8 bOperateCard, int iOpType);

	void RecoverRobGang(HandCards *pHc, _uint8 bOpCardChairID, _uint8 bOperateCard, int iOpType);
    
	void ExecuteGang(HandCards *pHc, _uint8 bOpCardChairID, _uint8 bOperateCard, int iOpType);
    /**
     *  @function: ExecutePeng
     *  @brief: 玩家执行碰操作
     *  @bOpCardChairID: 引起操作的玩家椅子ID(纪录碰了谁的牌)
     *  @pHc: 玩家的手牌
     *  @bOperateCard: 操作的牌
     *  @return: 无
    **/
	void ExecutePeng(HandCards *pHc, _uint8 bOpCardChairID, _uint8 bOperateCard);
    /**
     *  @function: ChangeCard
     *  @brief: 传进四个玩家的牌，开始换牌
     *  @stUserCard: 四个玩家选择换的牌
     *  @return: 无
    **/
    void ExecuteLeftChi_Feng(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard);
    void ExecuteCenterChi_Feng(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard);
    void ExecuteRightChi_Feng(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard);
	void ExecuteLeftChi(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard);
	void ExecuteCenterChi(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard);
	void ExecuteRightChi(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard);
	int GetGangInfo(const HandCards*pHc, MJ_opinfo& GangCardInfo);
	int GetPengGang(HandCards *pHc, _uint8 bCard, MJ_opinfo& GangCardInfo);

	//_uint8 GetPengGang(HandCards *pHc, _uint8 bCard, bool bMo = false);
	//void ChangeCard(tagChangeCard stUserCard[4]);

	//_uint8 GetPeng(HandCards *pHc, _uint8 bCard);
	//_uint8 GetGang(HandCards *pHc, _uint8 bCard, bool bIsGetCard);	
	bool GetWanGang(const HandCards* pHc, MJ_opinfo& ganginfo, _uint8 bCard = INVALID_MJ_CARD);
	bool GetZhiGang(const HandCards* pHc, MJ_opinfo& ganginfo, _uint8 bCard = INVALID_MJ_CARD);
	bool GetAnGang(const HandCards* pHc, MJ_opinfo& ganginfo, _uint8 bCard = INVALID_MJ_CARD);

	_uint8 NextCard(_uint8 card);
	_uint8 PrevCard(_uint8 card);
public:
	Shai_Set GetShaizi(_uint8 iCount);
public:
	bool Has4SameCards(HandCards& pHc, _uint8 bCard)const;
	bool Has4SameCardsX(HandCards& pHc, _uint8 bCard)const;

	bool IsXiaoSanYuan(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang);
	bool IsDaSanYuan(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang);
	bool IsMingSiGui(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang, _uint8 cbCard, bool bQuanPinDao = false);
	bool IsAnSiGui(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang, _uint8 cbCard, bool bQuanPinDao = false);
	bool IsKa5Xing(const HandCards* p, _uint8 cbCard, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang);

	bool IsQingDui(HandCards* p);												//清对
	bool IsJiangJiangHu(const HandCards* p, const _uint8 buf[4][4], const _uint8 Jiang);		//将将胡 全是258 的碰碰胡
	
	bool IsQingYiSe(const HandCards* p);	
	bool IsQingYiSe_01(const HandCards* p, _uint8 bExceptCard = INVALID_MJ_CARD);		
	bool IsQingYiSe_02(const HandCards* p);										//清一色
	bool IsDai19(HandCards* p, _uint8 buf[4][4], int length, _uint8 Jiang);			//带幺9	
//	bool IsQing19(const HandCards* p, const _uint8 buf[4][4], _uint8 Jiang);			//混一色19 加风牌	//不判断全风
	bool IsQuan19(const HandCards& p, const _uint8 buf[4][4], _uint8 Jiang);			//全19
	bool IsQuan19Wind(const HandCards& p, const _uint8 buf[4][4], _uint8 Jiang);			//全19
	bool IsHunYiSe(const HandCards* p);												//混一色
	bool IsZiYiSe(const HandCards* p);											//字一色
	bool IsShiBaLuoHan(const HandCards* p);		//十八罗汉
	int IsDaXiaoSanYuan(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10]);
	int IsDaXiaoSiXi(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10]);
	int L_IsDaXiaoSanYuan(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10], int iLaiziNum);
	int L_IsDaXiaoSiXi(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10], int iLaiziNum);
	//////////////////////////////////////////////////////////////////////////
	//非正规则型判断
	bool Is7Dui(const HandCards* p);													//7对, 3番
	bool IsQing7Dui(const HandCards* p);												//清7对， 5番
	bool IsLong7Dui(const HandCards* p, _uint8 &cbGenNum);								//龙7对， 5番
	bool IsQingLong7Dui(HandCards* p);											//清龙7对,6番
	bool IsDanDiao(const HandCards& p);
	bool IsJiangYiSe(const HandCards* p);	
	bool IsJiangYiSe_01(const HandCards* p);	
					
public:
	void SortCard(_uint8 p[], _uint8 len);
	bool GetJiang(_uint8* p, int len, int &JiangIndex, _uint8 &jiang, _uint8 *pRestCardBuf);
	//已经碰了两种花色，第三种不能碰,不能碰则返回false
	bool EstimateFixCard(HandCards *pHC, _uint8 cbCardData);
	bool FormatCards(HandCards *hcs, _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], _uint8 &Jiang);	
	/*风牌可以组顺子*/
	bool FormatCards_FengShun(HandCards *hcs, _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], _uint8 &Jiang);
	/*风牌可以组顺子*/
	bool FengPaiZuKan(const std::vector<_uint8> &fengPai, std::vector<std::vector<_uint8> >&kan);

	void RemoveCard(HandCards *hcs, _uint8 c, int length);
	void RemoveCard(HandCards *pHC, _uint8 c);
	void RemoveCard(_uint8 sCard[], _uint8 cblen, _uint8 c);
	void RemoveCard(HandCards *hcs, _uint8 *c, int length);	
	bool HasCard(const HandCards *hc, _uint8 c);	
	
public:
	bool IsQuanqiuren(const HandCards* pHc);
	bool IsQuanQiuRen(const HandCards* pHc);
	bool IsQuanQiuRen_01(const HandCards* pHc);
	bool IsQuanQiuRen_02(const HandCards* pHc, int iLaiziNum);
	bool IsQuanQiuRen_03(const HandCards* pHc, int iLaiziNum);
	bool IsQuanQiuRen_04(const HandCards* pHc, int iLaiziNum);
	bool Is258Card(_uint8 bCard);

	int GetChi(const HandCards *p, _uint8 cbCard, MJ_opinfo& ChiInfo);
	int GetZiChi(const HandCards *p, _uint8 cbCard, MJ_opinfo& ChiInfo);
	bool IsPengPengHu(const HandCards* p, const _uint8 CardsFormatBuf[4][4], _uint8 Jiang);
	bool Is13Yao(const HandCards* p, _uint8 c, _uint8 CardsFormatBuf[4][4], _uint8 Jiang);
	bool IsMenQing(const HandCards* p);		//算暗杠
	bool IsMenQing_02(const HandCards* p);	//不算暗杠
	bool IsMenQing_03(const HandCards* p);	//不算仰牌
	bool IsYiTiaoLong(const HandCards* p);
	bool IsYiTiaoLong(const HandCards* p, int cardNum[MJ_TYPE_COMMON][10], _uint8 laizicard, MJ_ten TiCards);
	bool IsYiTiaoLong_02(const HandCards* p);
	bool IsYiTiaoLong_03(const HandCards* p);

public:
	
	bool FormatCards_Laizi(_uint8 card[14], int nLen, int nLaiZiNum);	
	bool LaiZi_CanHu(MJ_hand_buffer stData, int LaiZiNum, bool bAllShunZi = false);
	void PyCard(const _uint8 handcard[14], int nLen, MJ_hand_buffer PaiCard[MJ_TYPE_JIAN]);
	void get_need_hun_num(MJ_hand_buffer& stData, int nNeedNum, int &nNeedMinNum);
	void get_need_hun_num_by_shun(MJ_hand_buffer stData, int nNeedNum, int &nNeedMinNum);
	bool checke_combine(_uint8 cbCard1, _uint8 cbCard2, _uint8 cbCard3);
	bool checke_combine_shun(_uint8 cbCard1, _uint8 cbCard2, _uint8 cbCard3);
	bool checke_combine_dui_zi(_uint8 cbCard1, _uint8 cbCard2);
	
public:
	bool L_Is13Yao(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum);                  //13幺
	bool L_IsQiDui(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum);                  //七对
	int L_IsQiDui_02(const HandCards* p, _uint8 c, const _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum);  
	bool L_IsQiDui_03(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNumint, int& iLongQiDui);                  //七对
	bool L_IsQiDui_04(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNumint, int& iLongQiDui, std::vector<_uint8> &L_Cards); //七对
	bool L_IsQiDui_05(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum, int& iLongQiDui);               //七对
	bool L_IsPengPengHu(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum);             //碰碰胡	
	bool L_IsPengPengHu_01(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum);
	int L_IsPengPengHu_02(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum);
	bool L_IsPengPengHu_04(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum, std::vector<_uint8>& L_Cards);
	
public:
	void GetCardCount(const HandCards* pHc, _uint8 CardDataNum[MJ_TYPE_COMMON][10]);
	void GetCardCountExt(const HandCards* pHc, _uint8 CardDataNum[MJ_TYPE_COMMON][10]);
	int GetCardCountX(const HandCards* pHc, _uint8 card);
	int GetFixedCardCount(const HandCards* pHc, _uint8 bCard);
	bool FormatCards_Laizi_3_Group(const _uint8 card[HAND_CARD_SIZE_MAX], int nLen, int nLaiZiNum);
	bool GetCardTypeCount(const HandCards* pHc, _uint8 CardTypeCount[MJ_TYPE_COMMON]);
private:
	MJ_hand_buffer m_PaiCard[MJ_TYPE_JIAN];        //各种花式的牌数据



public:
	//game_logic 移植过来的
	/*
	*	无赖子胡牌算
	*/
	bool FormatCards(HandCards *hcs, MJ_win_pattern& pattern);
	bool FormatCards(std::vector<_uint8> vec, MJ_win_pattern& pattern);
	/*
	*	癞子胡牌算
	*/
	bool FormatCards_LaiZi(HandCards *hcs, MJ_win_pattern& pattern, int lai_sum);
	bool FormatCards_LaiZi(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum);
	//癞子混将
	bool CheckJiang_LaiZi(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum_remain, bool bNeed258Jiang = false);
	//癞子混三个
	bool CheckShunZi_LaiZi(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum_remain, bool bNeed258Jiang = false);
	//查找子序列 
	bool CheckSubVector_LaiZi(std::vector<_uint8> vec, std::vector<_uint8> &kan, int lai_sum_remain);
	//获取card成顺子 所需要的癞子数量
	_uint8 GetShunZi_LaiZi(const std::vector<_uint8> &vec, _uint8 card = INVALID_MJ_CARD);


	void GetKan(const std::vector<_uint8> &vec_kan, MJ_win_pattern& pattern);

	//检测子序列
	bool CheckSubVector(std::vector<_uint8> vec, std::vector<_uint8> &kan);

	// 获取将
	void GetJiang(std::vector<_uint8> &vec, std::vector<_uint8> &out_jiang);

	//删除制定 牌 多少个
	bool EraseCard(std::vector<_uint8> &vec, _uint8 card, _uint8 sum = 1);

	//检测顺子
	bool CheckShunZi(std::vector<_uint8> &vec, _uint8 card, _uint8 sum = 1);

	//获取没有癞子的牌
	int GetNoLaiZiHandCard(const HandCards &card_in, HandCards &card_out , _uint8 m_laizi);
	int GetNoLaiZiHandCard(const HandCards &card_in, HandCards &card_out,const std::vector<_uint8>& vec_laizi);

	//获取癞子的数量
	int GetLaiZiSum(const HandCards &phc, _uint8 m_laizi);
	int GetLaiZiSum(const HandCards &phc, const std::vector<_uint8>& vec_laizi);

	//常用胡牌算法
	//检测子序列是否能够全部成为坎
	bool CheckSubVectorPeng(HandCards &no_laizi_handcard, int lai_sum_remain);
	bool CheckSubVectorPeng(std::vector<_uint8> &vec, int lai_sum_remain);

	//下面判断需要之前判断已经胡牌
	//检测清幺九
	bool CheckQing19(HandCards &no_laizi_handcard);

	//检测是否是七对
	bool Check7Dui(HandCards &no_laizi_handcard, int lai_sum);
	//检测子序列 是都能组成对子
	bool CheckSubVectorDui(std::vector<_uint8> vec, int lai_sum);
	//判断边、卡、胡牌
	bool Is_BianZhang(const HandCards* pHc, _uint8 bHuCard, const MJ_win_pattern& oPattern);
	bool Is_KaZhang(const HandCards* pHc, _uint8 bHuCard, const MJ_win_pattern& oPattern);
	bool Is_DaKuanZhang(const HandCards* pHc, _uint8 bHuCard, const MJ_win_pattern& oPattern);

	//见字胡
	bool IsJianZiHu(const _uint8 card[14], int nLen, int nLaiZiNum);
	bool IsJianZiHu(HandCards hcs, int lai_sum, bool bDai7Dui = false);
	bool IsJianZiHu(std::vector<_uint8> vec, int lai_sum, bool bDai7Dui = false);

	//卡二条、卡心五
	bool IsKa2Tiao(const HandCards* hcs, _uint8 bCard);
	bool IsKa5Xin(const HandCards* hcs, _uint8 bCard);
	bool IsJiaHu(const HandCards* hcs, _uint8 bCard);
	bool IsBianHu(const HandCards* hcs, _uint8 bCard);
	bool IsDiaoHu(const HandCards* hcs, _uint8 bCard);

	//四归一
	bool IsSiGuiYi(const HandCards* hcs, int &iCounts);
	int GetSiGuiYi(const HandCards* hcs);
	int GetTypeCount(const HandCards* hcs);
	int GetTypeCount_01(const HandCards* hcs);

	//一般高
	int GetYiBanGaoCount(const HandCards* p);

	//13不靠，或者13乱，13烂
	bool Is13BuKao(const HandCards *pHc);

	//烂牌
	bool IsLanPai(HandCards pHc, bool &qixing);
	bool IsJiangDui(const HandCards* p);
	bool Is19(_uint8 bCard);
	bool IsDai19(HandCards *p, MJ_win_pattern pattern);
	bool IsWu19(HandCards *p);
	int GetMingSiGui(HandCards *p);
	int GetAnSiGui(HandCards *p);
	bool IsDuiZiHu(HandCards *p, _uint8 bCard);
	bool Is14Yao(HandCards hcs);

	//带字牌的顺子检测
	bool FormatCards_01(HandCards *hcs, MJ_win_pattern& pattern);
	bool FormatCards_01(std::vector<_uint8> vec, MJ_win_pattern& pattern);
	void GetZi(std::vector<_uint8> &vec, std::vector<_uint8> &vecFeng);
	bool CheckShunZi_01(std::vector<_uint8> &vec, _uint8 card, _uint8 sum, std::vector<std::vector<_uint8> >& vecShun);
	bool CheckSubVector_01(std::vector<_uint8> vec, std::vector<_uint8> &kan);
	bool CheckSubVector_02(std::vector<_uint8> vec, std::vector<_uint8> &kan);
	bool CheckSubVector_03(std::vector<_uint8> vec, std::vector<_uint8> &kan);

	//19组顺子
	bool CheckShunZi_02(std::vector<_uint8> &vec, _uint8 card, _uint8 sum, std::vector<std::vector<_uint8> >& vecShun);
	bool EraseCard_02(std::vector<_uint8> &vec , _uint8 card, std::vector<_uint8> &res, _uint8 sum = 1);
	bool CheckSubVector_04(std::vector<_uint8> vec, std::vector<_uint8> &kan);
	bool CheckSubVector_05(std::vector<_uint8> vec, std::vector<_uint8> &kan);

	//带癞子258做将，可纯癞子做将需传入癞子牌
	bool FormatCards_LaiZi_258Jiang(const HandCards *hcs, MJ_win_pattern& pattern, int lai_sum, _uint8 laizicard = INVALID_MJ_CARD);
	bool FormatCards_LaiZi_258Jiang(std::vector<_uint8> vec, MJ_win_pattern & pattern, int lai_sum, _uint8 laizicard = INVALID_MJ_CARD);
};

#endif
