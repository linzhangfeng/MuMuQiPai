/***********************************************************
 * File Name          :       common.h
 * Author             :       
 * Version            :       1.0
 * Date               :       2016-05-22 14:17
 * Description        :       公共接口
***********************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_



#include <time.h>
#include <map>
#include <string.h>
#include "to_char.h"
//数据类型定义
typedef          char        _tint8;     //有符号 1 字节
typedef unsigned char        _uint8;     //无符号 1 字节
typedef short                _tint16;    //有符号 2 字节
typedef unsigned short       _uint16;    //无符号 2 字节
typedef int                  _tint32;    //有符号 4 字节
typedef unsigned int         _uint32;    //无符号 4 字节
typedef long long            _tint64;    //有符号 8 字节
typedef unsigned long long   _uint64;    //无符号 8 字节

#define MINUTE	(60)
#define HOUR 	(60 * 60)
#define DAY		(24 * HOUR)

#define DEF_BUF_LEN (static_cast<int>(1024 * 128))

#define GAME_PLAYER					4				//游戏人数

#define INVALID_MJ_CARD 0xFF
#define NUM_MASK		0x0F
#define TYPE_MASK		0xF0
#define INVALID_FAN		-1

#define INVALID_CHAIRID 0xFF
#define INVALID_MJ_CARD 0xFF
#define CARD_0			(0)
//常量定义
#define MAX_WEAVE					4				//最大组合
#define MAX_COUNT					14			    //最大数目
#define MAX_REPERTORY				108			    //最大库存
#define POOL_CARD_MAX				150				//最大牌数目
#define POOL_CARD_TYPE_MAX			42				//最大牌的种类
#define MAX_LAIZI_COUNT				(8)				//最大癞子数
#define MJ_HONG_ZHONG				(0x41)
#define MJ_FA_CAI					(0x42)
#define MJ_BAI_BAN					(0x43)
#define MJ_DONG_FENG				(0x31)

//17张手牌定义  目前：平和麻将、武穴麻雀、朱河麻雀、丰城麻将
//#define MJ_HAND_CARD_17	

#ifdef MJ_HAND_CARD_17
#define HAND_CARD_SIZE_MAX			(20)
#define HAND_CARD_KAN_MAX			(6)
#else
#define HAND_CARD_SIZE_MAX			(14)
#define HAND_CARD_KAN_MAX			(4)
#endif

#define HAND_CARD_SIZE_14			(14)
#define HAND_CARD_SIZE_17			(17)
#define HAND_CARD_SIZE_20			(20)

#define MJ_YAO_JI					((_uint8)0x11)
#define MJ_BA_TONG					((_uint8)0x28)
#define MJ_1_WAN					((_uint8)0x01)
#define MJ_1_TONG					((_uint8)0x21)
#define MJ_BA_WAN					((_uint8)0x08)

#define TYPE_NULL			0x00								//没有类型
#define TYPE_PENG			0x01								//碰牌类型
#define TYPE_ZHIGANG		0x02								//直杠牌类型
#define TYPE_WANGANG        0x04                                //弯杠
#define TYPE_ANGANG         0x08                                //暗杠
#define TYPE_GANG			(TYPE_ZHIGANG | TYPE_WANGANG | TYPE_ANGANG)
#define TYPE_HU			    0x10								//吃胡类型
#define TYPE_LISTEN			0x20								//听牌类型
#define TYPE_LEFT_CHI	    0x40								//左吃类型
#define TYPE_CENTER_CHI		0x80								//中吃类型
#define TYPE_RIGHT_CHI		0x100								//右吃类型
#define TYPE_CHI			(TYPE_LEFT_CHI | TYPE_RIGHT_CHI | TYPE_CENTER_CHI)

#define TYPE_DIAO_JIANG		0x200								//吊将
#define TYPE_HUA_HU			0x200								//花胡
#define TYPE_BU_ZHANG		0x400								//补张
#define TYPE_GEN_ZHANG		0x400								//广东跟庄
#define TYPE_HAIDI_YAO		0x800								//海底牌（要）
#define TYPE_SUODING		0x1000								//锁定手牌
#define TYPE_YING_KOU		0x2000								//硬扣

#define TYPE_TAN          	0x4000                             	//摊牌
#define TYPE_LIANG          0x4000                              //亮牌
#define TYPE_SHAO			0x8000								//勺 一坎牌暗扣
#define TYPE_YX_YOUJINHU    0x8000 								//游金胡
#define TYPE_SING           0x4000 								//唱歌
#define TYPE_DANCE         	0x8000 								//跳舞

#define TYPE_LAIZI_GANG	 	0x10000								//赖子杠
#define TYPE_HONGZHONG_GANG	0x20000								//红中杠
#define TYPE_FACAI_GANG	 	0x40000								//发财杠
#define TYPE_LAIZIPI_GANG	0x80000								//赖子皮杠

#define TYPE_YOUJIN		 	0x100000							//游金
#define TYPE_SHUANGYOU		0x200000							//双游
#define TYPE_SANYOU		 	0x400000							//三游
#define TYPE_SIYOU			0x800000							//四游
#define TYPE_YOU 			(TYPE_YOUJIN | TYPE_SHUANGYOU | TYPE_SANYOU | TYPE_SIYOU)

#define TYPE_QING_WAN 		0x100000							//报清万
#define TYPE_QING_TIAO 		0x200000							//报清条
#define TYPE_QING_TONG 		0x400000							//报清筒
#define TYPE_JIANG 			0x800000							//报将
#define TYPE_FENG 			0x1000000							//报风
#define TYPE_BAO 			(TYPE_QING_WAN | TYPE_QING_TIAO | TYPE_QING_TONG | TYPE_JIANG | TYPE_FENG) 	

#define TYPE_PASS			0xFF								//过
#define TYPE_CHI_PENG		(TYPE_CHI | TYPE_PENG)
#define TYPE_CHI_PENG_GANG	(TYPE_CHI | TYPE_PENG | TYPE_GANG)
#define TYPE_PENG_GANG		(TYPE_PENG | TYPE_GANG)



const int MJ_TYPE_NUM = 3; //麻将牌型共三种:万,条,筒
enum MJ_TYPE
{
	MJ_TYPE_WAN = 0,		//万,0-8,各4张，共36张
	MJ_TYPE_TIAO = 1,		//条,0-8,各4张，共36张
	MJ_TYPE_SUO = 1,
	MJ_TYPE_TONG = 2,		//筒,0-8,各4张，共36张
	MJ_TYPE_BING = 2,
	MJ_TYPE_FENG = 3,
	MJ_TYPE_JIAN = 4,	
	MJ_TYPE_FLOWER = 5,
	MJ_TYPE_SEASION = 5,
	MJ_TYPE_COMMON = 6,		//万条筒风箭
	MJ_TYPE_INVALID = -1,
};

#define CardNum(c) ((c) & NUM_MASK)
#define CardType(c) (((c) & TYPE_MASK)>>4)
#define CardNumX(card) ((card >= MJ_HONG_ZHONG && card <= MJ_BAI_BAN) ? (CardNum(card) + 4) : CardNum(card))
#define CardValue(t,n) ( (t<<4) | (n) )

struct FixedCard
{    
    int    state;			//碰杠类型
	_uint8    chairID;		//引起操作的椅子ID(主要是为了记录碰、直杠谁的牌)
	union 
	{
		_uint8 CardData;	//碰杠的牌
		_uint8 c[4];		//吃的牌以及其他非四个一样的杠牌
	};
	
	FixedCard()
	{		
		state = TYPE_NULL;
		chairID = INVALID_CHAIRID;
		CardData = INVALID_MJ_CARD;
	}
};

/**
 * 每位玩家手中牌记录
 * 
 * @deprecated ChangeableCards 可换牌
 *             ChangeableCardsLen 可换牌个数
 *             FixedCards 固定牌
 *             FixedCardsLen 固定牌组数
 */
struct HandCards
{
	_uint8      ChangeableCardsLen;
    _uint8      ChangeableCards[HAND_CARD_SIZE_MAX];
    _uint8      FixedCardsLen;
	FixedCard   FixedCards[HAND_CARD_KAN_MAX];
	HandCards()
	{
		ChangeableCardsLen = 0;
		FixedCardsLen = 0;
		memset(ChangeableCards, INVALID_MJ_CARD, sizeof(ChangeableCards));
	}
};

enum ACTION
{
	ACTION_SIDOWN = 1,//坐下
	ACTION_READY = 2,//准备
	ACTION_LEAVE = 3,//离开
	ACTION_CHANGE_TABLE = 4,//请求换桌
};
enum USER_STATUS
{
	STAND_UP = 0,        //站起状态
	SIT_DOWN = 1,        //坐下状态
	READY = 2,        //准备状态
	PLAYING = 3,        //游戏状态
};

enum HU_MODE
{
	HU_INVALID,
	HU_ZIMO = 1,			//自摸
	HU_PAOHU = 2,			//炮胡
	HU_DIANPAO = 3,			//点炮
	HU_BEIZIMO = 4,			//被自摸
	HU_QIANG_GANG = 5,		//抢杠胡
	HU_BEI_QIANG_GANG = 6,	//被抢杠
	HU_YI_PAO_DUO_XIANG = 7,//一炮多响
	HU_GANG_SHANG_HUA = 8,	//杠上花
	HU_GANG_HOU_PAO = 9,	//杠后炮
	HU_SHUA_KAI = 10,		//杠癞子开花
	HU_QIANG_ZHI_GANG = 11,	//抢直杠
	HU_LIANG_DAO = 12,      //亮倒
	HU_JIAO_PAI = 13,       //叫牌
	HU_BAO_HU = 14,         //包胡
	HU_ROB_KING_HU = 15,	//抢金胡
	HU_QIANG_AN_GANG = 16,	//抢暗杠
};

enum E_BANKER_TYPE
{
	TURN_HU_BANKER,		//胡牌为庄
	TURN_SHUN_BANKER,	//轮流坐庄
	TURN_SHUN,			//顺庄
};

//解散房间状态
enum E_DISS_STATE
{
	DISS_STATE_NONE = 0,		//无
	DISS_STATE_APPLYER = 1,		//发起者
	DISS_STATE_YESER = 2,		//同意者
	DISS_STATE_OUTTIMER = 3,	//超时者
	DISS_STATE_OFFLINER = 4,	//离线者
	DISS_STATE_AUTODISS = 5,	//自动解散
	DISS_STATE_FORCE_DIAN = 6,	//店小二强制解散
	DISS_STATE_FORCE_QUAN = 7,	//圈主强制解散
	DISS_STATE_USER_TIMEOUT = 8,//记录谁引起的自动解散
};

#endif //_COMMON_H_
