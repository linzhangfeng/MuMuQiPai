#ifndef __TABLE_HELPER_H__
#define __TABLE_HELPER_H__
#include <vector>
#include <json/json.h>
#include <assert.h>
#include "common.h"
#include "mj_common2.h"
#include "CardPoolReadJson2.h"

typedef MJ_BUFFER<int, 40> MJ_SUB_ID;
bool json_check_play_type_id(const Json::Value &val);
int json_get_play_type_id(const Json::Value &val, int iPlayID, MJ_SUB_ID& SubID);

bool Is_ChiPengGang(int iOpType);
bool Is_HuOperator(int iOpType);

/*
 *	漏胡规则
 *	记录弃胡的牌， 以及番数
 *	m_Available true启用漏胡规则
 *	Lock 锁住不能胡牌
 *	Evalute 判断是否可以解锁胡牌, true 可以解锁
 *	Unlock	解锁可以胡牌
 */
#define FW_LOCK_ALL_CARD	INVALID_MJ_CARD
#define FW_LOCK_ANY_POINT	(0xff)

class ForbidWin
{
	//_uint8 m_LockCard;
	std::vector<_uint8> m_LockCard;
	_uint8 m_LockPoints;
	bool m_Lock;		
	bool Evalute(_uint8 card, _uint8 point)const;
public:
	ForbidWin();
	void Lock(_uint8 card, _uint8 point);	
	bool IsLock(_uint8 card, _uint8 point = 0);	
	void Unlock();	
};


class HuScore
{
public:
	int iTotleScore;
	int iHuScore;
	int iGangScore;
	int iAddtionScore;
	
};
class HuScoreSet
{
	HuScore set[GAME_PLAYER];
public:
	HuScoreSet()
	{
		memset(set, 0, sizeof(set));
	}
	HuScore& operator[](_uint32 pos)
	{
		assert(pos < GAME_PLAYER);
		return set[pos];
	}	
};

enum E_POOL_MODE
{
	CARD_POOL_ALL,			//所有牌 包括八个花
	CARD_POOL_NO_FLOWER,	//万筒条风箭
	CARD_POOL_NO_WIND_DRAGON_FLOWER,//万筒条
	CARD_POOL_2_PEOPLE,
	CARD_POOL_3_PEOPLE,
	CARD_POOL_100CARDS,		//100张没有万
	CARD_POOL_TTF,			//筒条风
	CARD_POOL_TTJ,			//筒条箭
	CARD_POOL_TTFJNoDong,	//筒条风箭不带东

	CARD_POOL_4_FLOWER,		//万筒条风箭 + 春夏秋冬
	CARD_POOL_NO_WIND_4_FLOWER, //无风牌4花   万筒条+ 春夏秋冬
	CARD_POOL_NO_WIND_8_FLOWER, //无风牌8花  万筒条+8花
	CARD_POOL_2_FLOWER,		//万筒条风箭 + 春夏
	CARD_POOL_6_FLOWER,		//万筒条风箭 + 春夏秋冬梅兰

	CARD_POOL_TTFJNoBai,	//同条风箭没有白板

	CARD_POOL_72CARDS,		//72张牌
	CARD_POOL_WTTHONGZHONG, //万条筒和红中
	CARD_POOL_TTHONGZHONG, //条筒和红中
	CARD_POOL_TTCARDS,		//条筒
	CARD_POOL_WTTJ_CARDS,	//玩条筒箭

	CARD_POOL_WAN_TONG_FENG_JIAN,	//万筒风箭
	CARD_POOL_WAN_TONG,		//万筒

	CARD_POOL_WTTHZFC,		//万条同红中发财
	CARD_POOL_NO_WAN,		//条筒风箭
	CARD_POOL_WTZ2TCARDS,	//万筒字加2个一条

	CARD_POOL_WTTHONGZHONGBAIBAN, //万条筒和红中白板	
	CARD_POOL_TTW_DNXB_ZFB_2HUA,        //万条筒+2个花（春夏）
};

class CCardTypePool
{
public:
	CCardTypePool();
	~CCardTypePool();

	_uint8 operator[](_uint8 pos)const
	{
		assert(pos < m_MaxCount);
		return m_Pool[pos];
	}
	_uint8 Size()const { return m_MaxCount; }
	void Set(E_POOL_MODE eMode);
	void SetJoker(_uint8 card);
public:
	void Clear() { m_MaxCount = 0; }
	void AddPool(const _uint8* card, _uint8 size);
private:
	_uint8 m_JokerCard;
private:
	_uint8 m_Pool[POOL_CARD_TYPE_MAX];
	_uint8 m_MaxCount;
	E_POOL_MODE m_Mode;
};

class CCardPool
{
public:	
	CCardPool(E_POOL_MODE iMode, int iSeedID = 0);
	~CCardPool();

	void Set(E_POOL_MODE iMode, int iSeedID = 0);
	void SetJoker(_uint8 card, _uint8 count);
public:		
	_uint8 operator[](_uint8 pos)const;
	_uint8& operator[](_uint8 pos);	
	
	void InitPool();
	_uint8 Find(_uint8 card)const;

public:
	_uint8 GetOneCard();
	_uint8 GetLast();
	int GetCard(_uint8* pool, _uint8 size);	
	int GetLastCard(_uint8* pool, _uint8 size);

	_uint8 MaxCount()const { return m_MaxCount; }		//最大牌数
	_uint8 LastCard()const { return m_Pool[m_Last - 1];	}	//查看最后一张牌
	_uint8 CurCard()const { return m_Pool[m_Current]; }	//查看第一张牌
	_uint8 Count()const { return m_Count; }				//当前剩余牌数
	bool IsEmpty()const { return m_Count == 0; }		//是否没有牌了
	bool IsLastCard()const { return m_Count == 1; }		//是否最后一张牌
	_uint8 GetCardCount_FromBack()const{ return m_MaxCount - m_Last; }
	_uint8 GetCardByIndex(_uint8 index) const;			//查看牌池第几张牌(索引从0开始，跟m_Current无关)
	_uint8 LastIndexCard(_uint8 index) const { return m_Pool[m_Last - index]; } //查看最后第几张
    int SomeLastCard(_uint8* pool, _uint8 size);
    bool Exchange_Last_2_Card();
    _uint8 GangRefillCard();
public:
	bool Exchange_Cur_Next(_uint8 pos = -1);
	void ChangeCard(int first, int second);
    _uint8 GetLastSub1();
    _uint8 LastSub1Card();
    _uint8 GetRandCardFromLeft();
public:
	void FixCard(int );//测试用固定牌
	void FixCard_ReadJson();
	void FixCard_ReadJson2();
public:
	_uint8 Current() const { return m_Current; }
	_uint8 Last() const { return m_Last; }
	bool SwapCard(_uint8 index1, _uint8 index2);
private:
	void AddPool(const _uint8* card, _uint8 size, _uint8 count = 4);
	void AddJoker();
private:
	void InitTTFJNoDong();
	void Init72Cards();
	void InitWTTHongZhong();
	void InitWTTHzFc();
	void InitTTHongZhong();
	void InitTTFJNoBaiCards();
	void InitTTJCards();	
	void InitTTFCards();	
	void InitNoFlower();	
	void InitNoWindDragon();
	void Init2People();
	void Init3People();
	void Init100Cards();
	void InitAllCard();
	void Init4FlowerCard();
	void InitNoWind4FlowerCard();
	void InitNoWind8FlowerCard();
	void Init2FlowerCard();
	void Init6FlowerCard();
	void InitTTCards();
	void InitWTTJCards();
	void InitWTZ2TCards();
	void InitWTTHongZhongBaiBan();	
    void InitTtwDnxbZfb2Hua();

public:
	void GetInfo(int& SeedID, E_POOL_MODE& Mode, _uint8 Pool[POOL_CARD_MAX], _uint8& MaxCount, 
		_uint8& Count, _uint8& Current, _uint8& Last, _uint8& JokerCard, _uint8& JokerCount)
	{
		SeedID = m_SeedID;
		Mode = m_Mode;
		memcpy(Pool, m_Pool, sizeof(_uint8) * POOL_CARD_MAX);
		MaxCount = m_MaxCount;
		Count = m_Count;
		Current = m_Current;
		Last = m_Last;
		JokerCard = m_JokerCard;
		JokerCount = m_JokerCount;
	}
	void SetInfo(int SeedID, E_POOL_MODE Mode, _uint8 Pool[POOL_CARD_MAX], _uint8 MaxCount, 
		_uint8 Count, _uint8 Current, _uint8 Last, _uint8 JokerCard, _uint8 JokerCount)
	{
		m_SeedID = SeedID;
		m_Mode = Mode;
		memcpy(m_Pool, Pool, sizeof(_uint8) * POOL_CARD_MAX);
		m_MaxCount = MaxCount;
		m_Count = Count;
		m_Current = Current;
		m_Last = Last;
		m_JokerCard = JokerCard;
		m_JokerCount = JokerCount;
	}

private:
	CCardTypePool m_CardTypePool;
public:
	const CCardTypePool& Type()const
	{
		return m_CardTypePool;
	}

private:
	int m_SeedID;
	E_POOL_MODE m_Mode;
	_uint8 m_Pool[POOL_CARD_MAX];		
	_uint8 m_MaxCount;
	_uint8 m_Count;
	_uint8 m_Current;	//当前牌位置
	_uint8 m_Last;		//最后一张牌的下一张
	
private:
	struct
	{
		_uint8 m_JokerCard;
		_uint8 m_JokerCount;
	};
};


#endif
