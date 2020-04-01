#ifndef __TABLE_JIAN_LI_MJ_H__
#define __TABLE_JIAN_LI_MJ_H__

#include "../table.h"
#include "../proto/jianligameproto.pb.h"

struct MJ_conf_jianli
{
	int m_iHuaType;				//0:无花 1:红中 2:红中发财 3:红中白板
	bool m_bKengZhuang;			//坑庄

	bool m_bWeiZeng;
	int m_iWeiZeng;				//围增
	int m_iChuZeng;				//出增/定增

	
	MJ_conf_jianli()
	{
		m_iHuaType = 0;
		m_bKengZhuang = false;

		m_bWeiZeng = false;
		m_iWeiZeng = 0;
		m_iChuZeng = 0;
	}
};

//补花信息
struct Jl_HuaInfo
{
	std::vector<_uint8> vecHua;
	std::vector<_uint8> vecGets;

	Jl_HuaInfo()
	{
		vecHua.clear();
		vecGets.clear();
	}
};

class jianli_Fantype_to_FanNum : public Fantype_to_FanNum
{
public:
	jianli_Fantype_to_FanNum()
	{
		fan_num[MJ_FAN_TYPE_PINGHU] = 1;
	}
	bool SetFan(int iFanType, _uint32 iFanNum)
	{
		if (iFanType < MJ_FAN_TYPE_MAX)
		{
			fan_num[iFanType] = iFanNum;
			return true;
		}
		return false;
	}
};


class Table_jianlimj : public Table
{
public:
	Table_jianlimj();
	~Table_jianlimj();
public:
	void ConfInit_Remote(const Json::Value& val);	

	void GameStart();

	void TurnBanker();

	void GameEndReset();

	void GameEnd();
	
//	int Calc_Fan(_uint8 ChairID, MJ_fan_buffer& oFan);
	
	void Calc_BaseScore(_uint8 ChairID, int iBaseScore[GAME_PLAYER]);

	void Calc_HuScore(_uint8 ChairID, int iBaseScore[GAME_PLAYER], int iScore[GAME_PLAYER]);

	void UserGetLastCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan);

	int Check_Hu_Pattern(_uint8 ChairID, const HandCards& pHc, MJ_fan_buffer& oFan, const MJ_win_pattern& oPattern);

	void UserGangHouPao(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan);
	
	void UserGetFirstCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan);

	void UserFirstOutCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan);

	void UserAfterOperate(_uint8 ChairID, int iOpType, _uint8 bCard);

	void UserOperateGang(_uint8 ChairID, int iOpType, _uint8 bCard);

	int OutCardEstimate(_uint8 ChairID, _uint8 bCard, MJ_major& major, int& fan_num);

	int GetCardEstimate(_uint8 ChairID, _uint8 bGetCard, MJ_major& major, int& fan_num);

	int GetGangInfo(_uint8 ChairID, MJ_opinfo& ganginfo);

	int Check_Hu(_uint8 ChairID, HandCards& pHc, MJ_fan_buffer& oFan);

	void InitData();

	void OnUserOutCard(_uint8 ChairID, _uint8 bCard);

	void OnUserGetCard(_uint8 ChairID, bool bGetLast = false);

	void OnRecvUserOperate(_uint8 ChairID, int iOpType, int iCard);

	void Send_Hand_Card();

	void Proto_Game_OperateGang(_uint8 ChairID, _uint8 bCard, int iOpType, int iGangScore[GAME_PLAYER], proto::game::AckBcOpResult& stOpresult);

	int dispatch_client(int cmd, Client *client, Player *player);

	bool EstimateUserRespond(_uint8 bOutChairID, _uint8 bOutCard);

	void Score_GameEndSucc(HuScoreSet& score, proto::game::jianlimj::AckCurGameEnd &stCurGameEnd);

	void save_video_game_end(const proto::game::jianlimj::AckCurGameEnd stCurGameEnd);

	void StartChoosePiao();

	void RecvClientChoosePiao(Client *client, Player * player);

	void GameStartAfterPiao();

//	bool Is7Dui(_uint8 ChairID, HandCards& pHc, MJ_fan_buffer& oFan);

	void Send_Game_Scene(Player *player);

	void save_game_scene_info();

	void UserQiangGangHu(_uint8 ChairID, _uint8 bCard, MJ_fan_buffer& oFan);

	bool IsQuanqiuren(const HandCards& pHc, MJ_fan_buffer& oFan, int& iOpType);

	void UserLastOutCard(_uint8 ChairID, int& iOpType, MJ_fan_buffer& oFan);

	void Calc_GangScore(_uint8 ChairID, int iScore[GAME_PLAYER], int iTotalWinScore);

//	int Calc_KengZhuang(_uint8 ChairID);

//	void Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard& stUsergetcard);

	void Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard& stUsergetcard, MJ_major& major, bool buguohua);

	bool IsHuaPai(_uint8 cbCard);

	void Score_GameEndDraw(HuScoreSet& score, proto::game::AckGameEnd& stGameend);

	int Calc_WeiZeng();

	int Calc_ChuZeng(_uint8 ChairID, _uint8 OtherChairID);

	int Calc_SelfScore(_uint8 ChairID);

//	void UserOperateBuHua();

//	_uint8 GetOneCard();

	void set_out_timer_1(_uint8 ChairID, bool bDelayTimer,bool first);

	void set_operator_timer_1(_uint8 ChairID, bool bDelayTimer,bool first);

	void DoSimpleOperate(_uint8 ChairID, int iOpType, _uint8 bCard);

	void Proto_GameEndInfo_GangInfo(_uint8 ChairID, proto::game::AckGameEnd& stGameend);

	bool CardPoolIsEmpty();

	bool IsLastGetCard();

	void CalZhongMaNum();

	int Calc_UserScore(_uint8 HuChairID, _uint8 OtherChairID);

	_uint8 GetOneCard();

	int MyRound(float f);

protected:
	MJ_conf_jianli m_conf_jianli;	

protected:	
	jianli_Fantype_to_FanNum m_jianlimj_Fantype_to_FanNum;

	MJ_BUFFER<_uint8, 5> m_Peng[GAME_PLAYER];//玩家碰后牌(记录玩家弃弯杠信息)

	//记录是否是庄家首张
	bool m_bFirstCard;

	bool m_bIsPassHu[GAME_PLAYER];			//是否过胡

	_uint8 m_nUserHuaCount[GAME_PLAYER]; 	//补花数

	MJ_BUFFER<_uint8, 12> m_Hua[GAME_PLAYER];//补花的牌

	MJ_BUFFER<_uint8, 10> m_UserCanPiao;		//玩家可出增分数
	int m_UserPiaoScore[GAME_PLAYER];		//玩家出增分数

	int m_iLastMoPaiID;						//最后摸牌

private:
	ev_timer                    m_AutoPiao_Timer;
	ev_tstamp                   m_AutoPiao_Timer_Stamp;
	static void AutoPiao_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);	
	void AutoPiaoTimeOut();

};
#endif
