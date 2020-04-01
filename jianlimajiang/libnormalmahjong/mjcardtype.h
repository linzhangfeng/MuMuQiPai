/********************************************************************
    created:	2013/5/02
    filename: 	MJCardType.h
    author:		
    
    purpose:	麻将类型定义
*********************************************************************/
#ifndef __MAHJONG_DEF_H__
#define __MAHJONG_DEF_H__
#include "common.h"
#include "mj_common2.h"

#define  BIAN_DAN	3
#define  DUI_ZI		2
#define  BIAN_KAN	1
#define  SINGLE		0
//游戏定时器
#define TIMER_WAIT_READY        10                  //等待准备定时器
#define TIMER_DICE              20                  //骰子时器
#define TIMER_LACKCARD          30                  //定缺牌定时器
#define TIMER_OUT_CARD          40                  //出牌定时器
#define TIMER_OPERATE           50                  //操作定时器
#define TIMER_DELAY_OUT_CARD    60                  //延迟出牌定时器
#define TIMER_GET_CARD          70                  //发第14张牌定时器
#define TIMER_MULTIOPERATE      80                  //多操作时钟
#define TIMER_DELAY_GET_CARD    90                  //自摸延迟定时器
#define TIMER_DELAY_WRITE_SCORE 100                 //延迟写分
#define TIMER_CHANGE_TIME       110                 //换牌时间
#define	TIMER_AUTO_OUT_CARD     120                 //系统自动出牌定时器

#define LOOPNUM   5


/**
 * 番类型ID定义
 */
enum MJFanType
{	
	MJ_FAN_TYPE_NULL = 0,
	MJ_FAN_TYPE_JIULIANBAODENG = 1,				//九莲宝灯
	MJ_FAN_TYPE_DASIXI = 2,						//大四喜
	MJ_FAN_TYPE_XIAOSIXI = 3,					//小四喜
	MJ_FAN_TYPE_DASANYUAN = 4,					//大三元 
	MJ_FAN_TYPE_SIANKE = 5,						//四暗刻
	MJ_FAN_TYPE_SHIBALUOHAN = 6,				//十八罗汉
	MJ_FAN_TYPE_QUANYAOJIU = 7,					//全幺九	
	MJ_FAN_TYPE_ZIYISE = 8,						//字一色
	MJ_FAN_TYPE_XIAOSANYUAN = 9,				//小三元
	MJ_FAN_TYPE_QINGYAOJIU = 10,				//混幺九,清幺九
	MJ_FAN_TYPE_QINGDUI = 11,					//清对 清一色 + 碰碰胡	
	MJ_FAN_TYPE_QINGPENG = 12,					//清碰， 清一色 + 碰碰胡
	MJ_FAN_TYPE_HUNPENG = 13,					//混碰, 混一色 + 碰碰胡
	MJ_FAN_TYPE_QINGYISE = 14,					//清一色
	MJ_FAN_TYPE_HUNYISE = 15,					//混一色
	MJ_FAN_TYPE_PENGPENGHU = 16,				//碰碰胡		
	MJ_FAN_TYPE_PINGHU = 17,					//平胡	
	MJ_FAN_TYPE_JIHU = 18,						//鸡胡	
	MJ_FAN_TYPE_TIANHU = 19,					//天胡
	MJ_FAN_TYPE_DIHU = 20,						//地胡
	MJ_FAN_TYPE_RENHU = 21,						//人胡
	MJ_FAN_TYPE_QIANGGANGHU = 22,				//抢杠胡
	MJ_FAN_TYPE_HAIDILAOYUE = 23,				//海底捞月
	MJ_FAN_TYPE_GANGSHANGHUA = 24,				//杠上开花
	MJ_FAN_TYPE_QINGQIDUI = 25,					//清七对	
	MJ_FAN_TYPE_QINGLONGQIDUI = 26,				//清龙七对 
	MJ_FAN_TYPE_LONGQIDUI = 27,					//龙七对
	MJ_FAN_TYPE_QUANQIUREN = 28,				//全求人
	MJ_FAN_TYPE_GANGHOUPAO = 29,				//杠上炮
	MJ_FAN_TYPE_MENQIANQING = 30,				//门前清
	MJ_FAN_TYPE_FENGYISE = 31,					//风一色
	MJ_FAN_TYPE_MENQINGQINGYISE = 32,			//门前清 + 清一色
	MJ_FAN_TYPE_QINGSIANKE = 33,				//清一色 + 四暗刻
	MJ_FAN_TYPE_HAIDIPAO = 34,					//海底炮
	MJ_FAN_TYPE_13YAO = 35,						//十三幺	
	MJ_FAN_TYPE_QIDUI = 36,						//七对		
	MJ_FAN_TYPE_DAIYAOJIU = 37,					//带幺九	
	MJ_FAN_TYPE_JIANGDUI = 38,					//将对, 258做将
	MJ_FAN_TYPE_YITIAOLONG = 39,				//一条龙
	MJ_FAN_TYPE_QINGYITIAOLONG = 40,			//清一条龙
	MJ_FAN_TYPE_4JOKER = 41,					//4鬼胡
	MJ_FAN_TYPE_NOJOKER = 42,					//无鬼
	MJ_FAN_TYPE_QINGQUANQIUREN = 43,			//清全求人
	MJ_FAN_TYPE_DANDIAO = 44,					//单吊
	MJ_FAN_TYPE_MINGSIGUI = 45,					//明四归
	MJ_FAN_TYPE_ANSIGUI = 46,					//暗四归
	MJ_FAN_TYPE_JIANGYISE = 47,					//将一色
	MJ_FAN_TYPE_KAERTIAO = 48,					//卡二条
	MJ_FAN_TYPE_LONGZHUABEI = 49,				//龙抓背
	MJ_FAN_TYPE_SIGUIYI = 50,					//四归一 兼容： 门清
	MJ_FAN_TYPE_QINGPENGDIAO = 51,				//清碰钓 清一色 + 碰碰胡 + 单调
	MJ_FAN_TYPE_JINGOUDIAO = 52,				//金钩钓	
	MJ_FAN_TYPE_JINGOUPAO = 53,					//金钩炮  
	MJ_FAN_TYPE_KAXINWU = 54,					//卡心五
	MJ_FAN_TYPE_QUEMEN = 55,					//缺门
	MJ_FAN_TYPE_YIBANGAO = 56,					//一般高
	MJ_FAN_TYPE_JIAHU = 57,						//夹胡
	MJ_FAN_TYPE_YINGHU = 58,					//硬胡
	MJ_FAN_TYPE_MINGGANG = 59,					//明杠
	MJ_FAN_TYPE_ANGANG = 60,					//暗杠
	MJ_FAN_TYPE_SHUANGLONGQIDUI = 61,			//双龙七对
	MJ_FAN_TYPE_QINGSHUANGLONGQIDUI = 62,		//清双龙七对
	MJ_FAN_TYPE_SANLONGQIDUI = 63,				//三龙七对
	MJ_FAN_TYPE_QINGSANLONGQIDUI = 64,			//清三龙七对
	MJ_FAN_TYPE_SIHONGZHONG = 65,				//四红中
	MJ_FAN_TYPE_XIAODIAOCHE = 65,				//小吊车
	MJ_FAN_TYPE_DADIAOCHE = 66,					//大吊车
	MJ_FAN_TYPE_PENGPENGQUANQIUREN = 67,		//碰碰全球人
	MJ_FAN_TYPE_QINGPENGQUANQIUREN = 68,		//清一色碰碰胡全球人
	MJ_FAN_TYPE_ZIPENGHU = 69,					//字一色碰碰胡
	MJ_FAN_TYPE_ZIPENGQUANQIUREN = 70,			//字一色碰碰全球人
	MJ_FAN_TYPE_ZIQIDUI = 71,					//字一色七对
	MJ_FAN_TYPE_ZILONGQIDUI = 72,				//字一色龙七对
	MJ_FAN_TYPE_ZISHUANGLONGQIDUI = 73,			//字一色双龙七对
	MJ_FAN_TYPE_ZISANLONGQIDUI = 74,			//字一色三龙七对
	MJ_FAN_TYPE_SHISANLAN = 75,					//十三烂
	MJ_FAN_TYPE_QIXINGSHISANLAN = 76,			//七星十三烂
	MJ_FAN_TYPE_ZIQUANQIUREN = 77,				//字一色全球人
	//黄梅
	MJ_FAN_TYPE_HUBIAN = 77, 					//胡边
	MJ_FAN_TYPE_HUKA = 78, 						//胡卡
	MJ_FAN_TYPE_HONGZHONGPENG = 79,				//红中碰
	MJ_FAN_TYPE_BAIBANPENG = 80, 				//白板碰
	MJ_FAN_TYPE_MINGGANGHONGZHONG = 81, 		//明杠红中
	MJ_FAN_TYPE_MINGGANGBAIBAN = 82, 			//明杠白板	
	MJ_FAN_TYPE_HANDHONGZHONG = 83, 			//手中3个红中 
	MJ_FAN_TYPE_HANDBAIBAN = 84, 				//手中3个白板
	MJ_FAN_TYPE_ANGANGHONGZHONG = 85, 			//暗杠红中
	MJ_FAN_TYPE_ANGANGBAIBAN = 86, 				//暗杠白板
	MJ_FAN_TYPE_FACAI = 87, 					//发财
	MJ_FAN_TYPE_MINGGANG_1 = 88, 				//明杠
	MJ_FAN_TYPE_ANGANG_1 = 89, 					//暗杠
	MJ_FAN_TYPE_4LAIZI = 90, 					//4个癞子
	MJ_FAN_TYPE_GANGSHANGPAO = 91, 				//杠上炮
	MJ_FAN_TYPE_HUDIAO = 92,					//千丘
	MJ_FAN_TYPE_PINGHU_1 = 93,
	MJ_FAN_TYPE_PINGHU_2 = 94,
	MJ_FAN_TYPE_PENGPENGHU_1 = 95,
	MJ_FAN_TYPE_PENGPENGHU_2 = 96,
	MJ_FAN_TYPE_QINGYISE_1 = 97,
	MJ_FAN_TYPE_QINGYISE_2 = 98,
	MJ_FAN_TYPE_QIDUI_1 = 99,
	MJ_FAN_TYPE_QIDUI_2 = 100,
	MJ_FAN_TYPE_HUBIAN_1 = 101,
	MJ_FAN_TYPE_HUBIAN_2 = 102,
	MJ_FAN_TYPE_HUKA_1 = 103,
	MJ_FAN_TYPE_HUKA_2 = 104,
	MJ_FAN_TYPE_HUDIAO_1 = 105,
	MJ_FAN_TYPE_HUDIAO_2 = 106,
	MJ_FAN_TYPE_ZIMO = 107,
	MJ_FAN_TYPE_DANDIAO_1 = 108,
	MJ_FAN_TYPE_DANDIAO_2 = 109,
	MJ_FAN_TYPE_ANGANGHUA = 110,				//暗杠发财
	MJ_FAN_TYPE_HUHONGZHONG = 111,				//胡红中
	MJ_FAN_TYPE_HUBAIBAN = 112,					//胡白板
	//扬州
	MJ_FAN_TYPE_PIHU = 121,						//屁胡
	MJ_FAN_TYPE_HUNDUIDUI = 122,				//混对对
	MJ_FAN_TYPE_QINGDUIDUI = 123,				//清对对
	MJ_FAN_TYPE_FENGQING = 124,					//风清	
	MJ_FAN_TYPE_4LAIHUPAI = 125,				//四赖子
	MJ_FAN_TYPE_HUNQIDUI = 126,					//混七对
	MJ_FAN_TYPE_ZHAI_1 = 127,					//炸、龙七对叫法
	MJ_FAN_TYPE_ZHAI_2 = 128,
	MJ_FAN_TYPE_ZHAI_3 = 129,
	MJ_FAN_TYPE_BANZIFANBEI = 130,				//板子翻倍

	MJ_FAN_TYPE_LUANSANFENG = 131,				//乱三风
	MJ_FAN_TYPE_LUANZHONGFABAI = 132,			//乱中发白
	MJ_FAN_TYPE_SING = 133,						//唱歌
	MJ_FAN_TYPE_DANCE = 134,					//跳舞
	MJ_FAN_TYPE_GANGSHANGGANG = 135,			//杠上杠
	MJ_FAN_TYPE_ZHONGZHANG = 136,				//中张
	MJ_FAN_TYPE_HEIQIDUI = 137,					//黑七对
	MJ_FAN_TYPE_HEIQUANQIUREN = 138,			//黑全球人

	MJ_FAN_TYPE_MAX = 200,

	MJ_FAN_TYPE_SPECIAL = 512,
	MJ_FAN_CHANGSHA_SIXIHU = 513,				//长沙四喜胡
	MJ_FAN_CHANGSHA_BANBANHU = 514,				//长沙板板胡
	MJ_FAN_CHANGSHA_QUEYIMEN = 515,				//长沙缺一门
	MJ_FAN_CHANGSHA_LIULIUSHUN = 516,			//长沙六六顺	
	MJ_FAN_CHANGSHA_JIANGJIANGHU = 517,			//长沙将将胡 全是258 的碰碰胡
	MJ_FAN_CHENZHOU_SIZHONGHU = 518,	
	MJ_FAN_GUANGDONG_QIANGGANGQUANBAO = 519,
	MJ_FAN_GUANGDONG_GANGBAOQUANBAO = 520,
	MJ_FAN_GUIYANG_RUANBAO = 521,
	MJ_FAN_GUIYANG_YINGBAO = 522,
	MJ_FAN_GUIYANG_SHABAO_10 = 523,
	MJ_FAN_GUIYANG_SHABAO_20 = 524,
	MJ_FAN_HUA_DIAO = 525,				//花单钓
	MJ_FAN_HUA_DIAO_HUA = 526,			//花钓花
	MJ_FAN_HUA_2_HUA_DIAO_HUA = 527,	//双花钓花
	MJ_FAN_HUA_3_DIAO_HUA = 528,		//三花钓花	

	MJ_FAN_QIDUI_SHUANGLONG = 529,		//双龙七对(超豪华七对)
	MJ_FAN_QIDUI_SANLONG = 530,			//三龙七对（非凡七对）
	MJ_FAN_QIDUI_QINGSHUANGLONG = 531,	//清双龙七对
	MJ_FAN_QIDUI_QINGSANLONG = 532,		//清三龙七对
	MJ_FAN_HUIZHOU_BAOJIUQUANBAO = 533,	//惠州：报九全包
	MJ_FAN_TYPE_KAWUXING = 534,			//襄阳卡五星
	MJ_FAN_TYPE_SHOWCARDS = 535,		//襄阳亮牌

	MJ_FAN_XIANNING_258JIANGYISE = 564,	//将一色
	MJ_FAN_XIANNING_HEIHU = 565,		//黑胡
	MJ_FAN_XIANNING_HONGZHONGGANG = 567,
	MJ_FAN_XIANNING_LAIZIGANG = 568,
	MJ_FAN_XIANNING_SIGUIJIAYIFAN = 569,//四赖加蕃
	MJ_FAN_XIANNING_SIHONGZHONG = 570,	//四红中

	MJ_FAN_TYPE_SPECIAL_MAX = 571,
};

//换牌结构体
struct tagChangeCard
{
	_uint8 bCard[3];    //换的牌，每个人最多换三张
	_uint8 bLen;        //换牌张数
	tagChangeCard()
	{
		memset(bCard, 0, sizeof(bCard));
		bLen = 0;
	}
};

//操作状态
enum OPERATE_STAUS
{ 
	OPERATE_NULL               = 0,                  
	OPERATE_CHANGE_CARD        = 1,                  //换牌状态
	OPERATE_LACK_CARD          = 2,                  //定缺牌状态
	OPERATE_CARD               = 3,                  //出牌和操作牌状态
};

enum GANG_TYPE
{
	ZHI_GANG_INDEX = 0,
	WAN_GANG_INDEX = 1,
	AN_GANG_INDEX  = 2
};

struct GangScore
{  
	int         GangType;           //杠类型
	_tint64		llScore;		      //总分
};
struct GangInfo
{
	int			chairID;	          //玩家ID
	GangScore   sGangScore[3];    
};
struct ScoreInfo
{
	int chairID;
	_tint64 llScore;
};
/**
*刮风
*/
struct WindInfo 
{
	_uint8			cbWindNum;	      //刮风了几个玩家
	GangInfo		sScore[3];	      //被刮风玩家扣分信息
};

struct RainInfo
{
	_uint8			cbRainNum;	      //下雨了几个玩家
	GangInfo		sScore[3];	      //被下雨玩家扣分信息
};

struct UserPasGangInfo
{
	int      chairID;
	int      GangType;
	_uint8   cbCardData;
	_tint64  llScore;
};

struct GangUserInfo
{  
	int GangUserNum;
	UserPasGangInfo userPasGang[3];
	GangUserInfo()
	{
		GangUserNum = 0;
		memset(userPasGang, 0, sizeof(userPasGang));
	}
};

//杠牌记录
struct GangRecord
{   
	int GangNum;                     //杠个数
	GangUserInfo sGangInfo[3];
	GangRecord()
	{
		GangNum = 0;
		memset(sGangInfo, 0, sizeof(sGangInfo));
	}
};

struct OpTimeRecord
{
	_uint64 ullTime;
	int     nOpTeype;
	OpTimeRecord()
	{
		ullTime = 0;
		nOpTeype = 0;
	}
};

struct info     
{ 
	int chairID;                     //椅子ID
	int nFanType;                    //番种
	int nFanNum;                     //番数
	int nGenNum;                     //根的个数
};
//非花猪玩家信息
struct tagFlower 
{
	_uint8  cbNum;			  //花猪的人数
	info    flower[4];	      //花猪信息
};

//非大叫玩家信息
struct tagListen 
{
	_uint8  cbNum;					//大叫玩家人数
	info    Listen[4];	//大叫玩家的信息
};

struct tagFLScore
{
	int chairID;
	_tint64 llScore;      //查花猪，查大叫赢分数
	_tint64 llBakcScore;  //刮风下雨归还分数
	int nFanType;         //番种
	int nFanNum;          //番数
	int nGenNum;          //根个数
};

struct tagUserFlower      //玩家查谁花猪信息
{   
	_uint8 cbNum;
	tagFLScore sScore[GAME_PLAYER + 1];  //被查玩家分数信息 
};

struct tagUserListen      //玩家查谁大叫信息
{
	_uint8 cbNum;      
	tagFLScore sScore[GAME_PLAYER + 1];  
};

//结算胡牌信息
struct HuInfo 
{  
	_uint8  cbZiMo;                     //是否自摸
	_uint8	cbHuNum;					//玩家胡了几个人
	int	    ChairID[3];				    //玩家胡了谁
	_uint8	cbFanType;					//胡牌番种
	_uint8	cbFanNum;					//总番数(番数+杠数+根数+另加番)
	_uint8  cbCombType;                 //组合翻型
	int		nHuScore;					//分数
	_uint8	cbGangNum;					//杠的个数
	_uint8	cbGenNum;					//根的个数
	_uint8	cbAddFan;					//另加番
	_uint8  cbPasChairID;               //引起胡的玩家(自摸时为自己的ID)
	_uint8  cbHuCard;                   //引起胡的牌
	_uint8  HuType;                     //胡牌类型
	int     OutNum;                     //出牌手数
	int     TaskDoubleTimes;            //牌局任务倍数
};

//被胡的信息记录
struct HuRecord
{
	int	   ChairID;					//玩家椅子ID
	int	   nScore;						//分数
};
//被胡牌信息
struct PassiveHuInfo
{
	_uint8		cbPasHuNum;					//被几个玩家胡
	HuRecord	sPassiveHu[3];				//被那个玩家胡的信息记录
};

struct LengthInfo 
{
	_uint8 cbLength;
};

struct ChanceInfo
{
	_uint8	cbNomal0;
	_uint8	cbPingHu1;
	_uint8	cbDuiDuihu2;
	_uint8	cbQingyise3;
	_uint8	cbJiangdui4;
	_uint8	cbLongQidui5;
	_uint8	cbQLongQidui6;
	_uint8	cbDiHu7;
	_uint8	cbTianHu8;
};

struct TypeProbInfo
{
	int iProb;
	int extractIndex;
};
struct SuperChanceInfo
{
	TypeProbInfo	tagNomal0;
	TypeProbInfo	tagPingHu1;
	TypeProbInfo	tagDuiDuiHu2;
	TypeProbInfo	tagQingYiSe3;
	TypeProbInfo	tagDaiYaoJiu4;
	TypeProbInfo	tagQiDui5;
	TypeProbInfo	tagQingDaDui6;
	TypeProbInfo	tagJiangDui7;
	TypeProbInfo	tagLongQiDui8;
	TypeProbInfo	tagQingQiDui9;
	TypeProbInfo	tagQingYaoJiu10;
	TypeProbInfo	tagQLongQiDui11;
};

struct tagExtractInfo
{
	int iNum;
	int iProb;
	tagExtractInfo()
	{
		iNum = 0;
		iProb = 0;
	}
};

struct tagExtractList
{
	int index;
	std::map<_uint32,tagExtractInfo> Extract_list;
	tagExtractList()
	{
		index = 0;
	}
};
struct tagCardType
{
	_uint8 cbCardType;
	_uint8 cbNum;
	tagCardType()
	{
		cbCardType = 0;
		cbNum = 0;
	}
};

struct tagCardAnalysis
{
	_uint8 cbCardType;                //花色类型
	_uint8 cbTotalNum;                //本种花色总张数
	_uint8 cbSingleNum;               //单张个数
	_uint8 cbDuiziNum;                //对子个数
	_uint8 cbThreeNum;                //三张个数
	_uint8 cbFourNum;                 //四张个数
	_uint8 cbSingle[13];
	_uint8 cbDuizi[13];
	_uint8 cbThree[13];
	_uint8 cbFour[13];
	tagCardAnalysis()
	{
		cbCardType = 0;
		cbTotalNum = 0;
		cbSingleNum = 0;
		cbDuiziNum = 0;
		cbThreeNum = 0;
		cbFourNum = 0;
	}
};

struct tagTaskType
{   
	int TaskType;             //任务类型
	int RewardType;           //奖励类型
	int PropID;               //奖励的道具ID
	int RewardNum;            //奖励数量
	char TaskDesc[256];      //奖励描述
	tagTaskType()
	{  
		RewardType  = 0;
		RewardNum   = 0;
		PropID      = 0;
		memset(TaskDesc, 0, sizeof(TaskDesc));
	}
};

struct tagCardTask
{
	tagTaskType sTaskType[50];
	int nCount;
	tagCardTask()
	{
		nCount = 0;
		memset(sTaskType, 0, sizeof(sTaskType));
	};
};


class Fantype_to_FanNum
{
protected:
	int fan_num[MJ_FAN_TYPE_MAX];
	int special_fan_num[MJ_FAN_TYPE_SPECIAL_MAX - MJ_FAN_TYPE_SPECIAL];
public:
	Fantype_to_FanNum()
	{
		memset(fan_num, 0, sizeof(fan_num));
		memset(special_fan_num, 0, sizeof(special_fan_num));
	}
	virtual ~Fantype_to_FanNum()
	{

	}
	virtual bool FanFilter(const MJ_fan_buffer& oFan, MJ_fan_buffer& oOutFan)
	{
		return false;
	}
	virtual int operator[](int fan)const
	{
		return GetFan(fan);
	}
	virtual int GetFan(int fan)const
	{
		if (fan < MJ_FAN_TYPE_MAX)
		{
			return fan_num[fan];
		}
		else if (fan < MJ_FAN_TYPE_SPECIAL_MAX && fan >= MJ_FAN_TYPE_SPECIAL)
		{
			return special_fan_num[fan - MJ_FAN_TYPE_SPECIAL];
		}
		throw("out of range!\n");
		return 0;
	}
	virtual bool SetFan(int iFanType, _uint32 iFanNum)
	{		
		if (iFanType < MJ_FAN_TYPE_MAX)
		{
			fan_num[iFanType] = iFanNum;
			return true;
		}
		else if (iFanType >= MJ_FAN_TYPE_SPECIAL && iFanType < MJ_FAN_TYPE_SPECIAL_MAX)
		{
			special_fan_num[iFanType - MJ_FAN_TYPE_SPECIAL] = iFanNum;
			return true;
		}
		return false;
	}
};
#endif
