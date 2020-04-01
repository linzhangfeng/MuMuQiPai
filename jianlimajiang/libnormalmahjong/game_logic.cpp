#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <assert.h>

#include "game_logic.h"
#include <stdio.h>
#include "../common/log.h"


#define _CARD_TEST

extern Log log;
CGameLogic::CGameLogic(void)
{

}

CGameLogic::~CGameLogic(void)
{

}

void CGameLogic::AddCard(HandCards *pHc, _uint8 bCard)
{
	if (bCard == INVALID_MJ_CARD || bCard == 0)
	{
		SortCard(pHc->ChangeableCards, pHc->ChangeableCardsLen);
		return;
	}
	else
	{
		if (pHc->ChangeableCardsLen < HAND_CARD_SIZE_MAX)
		{
			pHc->ChangeableCards[pHc->ChangeableCardsLen] = bCard;
			pHc->ChangeableCardsLen++;
		}
	}

	SortCard(pHc->ChangeableCards, pHc->ChangeableCardsLen);
	
	return;
}

void CGameLogic::SortCard(_uint8 p[], _uint8 len)
{
	int i,j;
	for (i=0; i<len; i++)
	{
		for (j=i+1; j<len; j++)
		{
			if (p[i]>p[j])
			{
				_uint8 tempCard = p[i];
				p[i] = p[j];
				p[j] = tempCard;
			}
		}		
	}
}

bool CGameLogic::ChuPai(HandCards *pHc, _uint8 bCard)
{
	bool bfind = false;
	for (int i=0; i<pHc->ChangeableCardsLen; i++)
	{
		if (pHc->ChangeableCards[i] == bCard)
		{
			bfind = true;
			for (int j = i+1;j<pHc->ChangeableCardsLen;j++)
			{
				pHc->ChangeableCards[j-1] = pHc->ChangeableCards[j];        
			}
			break;
		}
	}
	if (bfind)
	{	
		pHc->ChangeableCards[pHc->ChangeableCardsLen-1] = INVALID_MJ_CARD;
		pHc->ChangeableCardsLen--;
	}	
	return bfind;
}

int CGameLogic::GetPengGang(HandCards *pHc, _uint8 bCard, MJ_opinfo& GangCardInfo)
{
	GangCardInfo.Clear();
	int count = 0;
	for (int i = 0; i < pHc->ChangeableCardsLen; i++)
	{
		if (bCard == pHc->ChangeableCards[i])
		{
			count++;
		}
	}
	int iOpType = TYPE_NULL;
	if (count >= 2)
	{		
		GangCardInfo.Add(OpState(bCard, TYPE_PENG));
		iOpType |= TYPE_PENG;
	}
	if (count >= 3)
	{		
		GangCardInfo.Add(OpState(bCard, TYPE_ZHIGANG));
		iOpType |= TYPE_ZHIGANG;
	}	
	return iOpType;
}


int CGameLogic::GetGangInfo(const HandCards*pHc, MJ_opinfo& ganginfo)
{
	_uint8 bCardCount[MJ_TYPE_COMMON][10];
	memset(bCardCount, 0, sizeof(bCardCount));
	ganginfo.Clear();
	int iOpType = TYPE_NULL;
	for (int i = 0; i < pHc->ChangeableCardsLen; i++)
	{
		_uint8 bType = CardType(pHc->ChangeableCards[i]);
		_uint8 bNum = CardNum(pHc->ChangeableCards[i]);
		
		bCardCount[bType][bNum]++;
		if (bCardCount[bType][bNum] == 4)
		{
			ganginfo.Add(OpState(pHc->ChangeableCards[i], TYPE_ANGANG));
			iOpType |= TYPE_ANGANG;
		}
	}
	for (int i = 0; i < pHc->FixedCardsLen; i++)
	{
		if (pHc->FixedCards[i].state == TYPE_PENG)
		{
			for (int j = 0; j < pHc->ChangeableCardsLen; j++)
			{
				if (pHc->FixedCards[i].CardData == pHc->ChangeableCards[j])
				{					
					ganginfo.Add(OpState(pHc->ChangeableCards[j], TYPE_WANGANG));
					iOpType |= TYPE_WANGANG;
					break;
				}
			}
		}
	}
	return iOpType;
}

bool CGameLogic::GetWanGang(const HandCards* pHc, MJ_opinfo& ganginfo, _uint8 bCard)
{
	ganginfo.Clear();
	if (bCard == INVALID_MJ_CARD)
	{
		for (int i = 0; i < pHc->FixedCardsLen; i++)
		{
			if (pHc->FixedCards[i].state == TYPE_PENG)
			{
				for (int j = 0; j < pHc->ChangeableCardsLen; j++)
				{
					if (pHc->FixedCards[i].CardData == pHc->ChangeableCards[j])
					{
						ganginfo.Add(OpState(pHc->ChangeableCards[j], TYPE_WANGANG));
					}
				}
			}
		}
		return ganginfo.NotNull();
	}
	else
	{
		for (int i = 0; i < pHc->FixedCardsLen; i++)
		{
			if (pHc->FixedCards[i].state == TYPE_PENG && pHc->FixedCards[i].CardData == bCard)
			{
				ganginfo.Add(OpState(pHc->FixedCards[i].CardData, TYPE_WANGANG));
				return true;
			}
		}
	}
	return false;
}
bool CGameLogic::GetZhiGang(const HandCards* pHc, MJ_opinfo& ganginfo, _uint8 bCard)
{	
	ganginfo.Clear();
	if (bCard == INVALID_MJ_CARD)
	{
		return false;
	}
	else
	{
		int iCounts = 0;		
		for (int i = 0; i < pHc->ChangeableCardsLen; i++)
		{
			if (pHc->ChangeableCards[i] == bCard)
			{				
				if (++iCounts == 3)
				{
					ganginfo.Add(OpState(pHc->ChangeableCards[i], TYPE_ZHIGANG));
					return true;
				}
			}
		}
		return false;
	}
}
bool CGameLogic::GetAnGang(const HandCards* pHc, MJ_opinfo& ganginfo, _uint8 bCard)
{
	ganginfo.Clear();
	if (bCard == INVALID_MJ_CARD)
	{
		_uint8 bCardCount[MJ_TYPE_COMMON][10];
		memset(bCardCount, 0, sizeof(bCardCount));				
		for (int i = 0; i < pHc->ChangeableCardsLen; i++)
		{
			_uint8 bType = CardType(pHc->ChangeableCards[i]);
			_uint8 bNum = CardNum(pHc->ChangeableCards[i]);
			if (++bCardCount[bType][bNum] == 4)
			{
				ganginfo.Add(OpState(pHc->ChangeableCards[i], TYPE_ANGANG));				
			}
		}
		return ganginfo.NotNull();
	}
	else
	{
		int iCounts = 0;
		for (int i = 0; i < pHc->ChangeableCardsLen; i++)
		{
			if (pHc->ChangeableCards[i] == bCard)
			{
				if (++iCounts == 4)
				{
					ganginfo.Add(OpState(pHc->ChangeableCards[i], TYPE_ANGANG));
					return true;
				}
			}
		}
	}
	return true;
}
Shai_Set CGameLogic::GetShaizi(_uint8 iCount)
{	
	assert(iCount < 8);
	Shai_Set set;
	srand((int)time(0));

	for (int i = 0; i < iCount; i++)
	{
		set.Shai[i] = rand() % 6 + 1;
	}
	set.Num = iCount;
	return set;
}

void CGameLogic::ExecuteRobGang(HandCards *pHc, _uint8 bOpCardChairID, _uint8 bOperateCard, int iOpType)
{
	if (iOpType == TYPE_WANGANG)
	{
		for (int i = 0; i < pHc->FixedCardsLen; i++)
		{
			if (pHc->FixedCards[i].state == TYPE_PENG && pHc->FixedCards[i].CardData == bOperateCard)
			{
				pHc->FixedCards[i].state = iOpType;
				RemoveCard(pHc, bOperateCard, 1);
				break;
			}
		}
	}
	else if (iOpType == TYPE_ANGANG || iOpType == TYPE_ZHIGANG)
	{
		_uint8 bLen = (iOpType == TYPE_ANGANG ? 4 : 3);
		RemoveCard(pHc, bOperateCard, bLen);
		pHc->FixedCards[pHc->FixedCardsLen].CardData = bOperateCard;
		pHc->FixedCards[pHc->FixedCardsLen].state = iOpType;
		pHc->FixedCards[pHc->FixedCardsLen].chairID = bOpCardChairID;
		pHc->FixedCardsLen++;
	}
}

void CGameLogic::RecoverRobGang(HandCards *pHc, _uint8 bOpCardChairID, _uint8 bOperateCard, int iOpType)
{
	if (iOpType == TYPE_WANGANG)
	{
		for (int i = 0; i < pHc->FixedCardsLen; i++)
		{
			if (pHc->FixedCards[i].state == iOpType && pHc->FixedCards[i].CardData == bOperateCard)
			{
				pHc->FixedCards[i].state = TYPE_PENG;
				AddCard(pHc, bOperateCard);
				break;
			}
		}
	}
	else if (iOpType == TYPE_ANGANG || iOpType == TYPE_ZHIGANG)
	{
		if (pHc->FixedCardsLen > 0)
		{
			int num = iOpType == TYPE_ANGANG ? 4 : 3;
			int tmp = pHc->FixedCardsLen - 1;
			if (pHc->FixedCards[tmp].state == iOpType && pHc->FixedCards[tmp].CardData == bOperateCard)
			{
				pHc->FixedCardsLen--;
				for (int i = 0; i < num; ++i)
					AddCard(pHc, bOperateCard);
			}
			else
			{
				for (int i = 0; i < tmp; i++)
				{
					if (pHc->FixedCards[i].state == iOpType && pHc->FixedCards[i].CardData == bOperateCard)
					{
						memmove(&pHc->FixedCards[i], &pHc->FixedCards[i + 1], (tmp - i) * sizeof(FixedCard));
						pHc->FixedCardsLen--;
						for (int i = 0; i < num; ++i)
							AddCard(pHc, bOperateCard);
						break;					
					}
				}
			}
		}
	}
}

//操作函数
void CGameLogic::ExecuteGang(HandCards *pHc, _uint8 bOpCardChairID, _uint8 cbOperateCard, int cbType)
{

	if (cbType == TYPE_WANGANG)
	{
		RemoveCard(pHc, cbOperateCard, 1);
		for (int i=0; i<pHc->FixedCardsLen; i++)
		{
			if (pHc->FixedCards[i].CardData == cbOperateCard)
			{
				pHc->FixedCards[i].state = cbType;
			}
		}
	}
	else
	{
		_uint8 bLen = (cbType == TYPE_ANGANG ? 4 : 3);
		RemoveCard(pHc, cbOperateCard, bLen);
		pHc->FixedCards[pHc->FixedCardsLen].CardData = cbOperateCard;
		pHc->FixedCards[pHc->FixedCardsLen].state = cbType;
		pHc->FixedCards[pHc->FixedCardsLen].chairID = bOpCardChairID;
		pHc->FixedCardsLen++;
	}
}

void CGameLogic::ExecutePeng(HandCards *pHc, _uint8 bOpCardChairID, _uint8 cbOperateCard)
{
	RemoveCard(pHc, cbOperateCard, 2);
	pHc->FixedCards[pHc->FixedCardsLen].CardData = cbOperateCard;
	pHc->FixedCards[pHc->FixedCardsLen].state = TYPE_PENG;
	pHc->FixedCards[pHc->FixedCardsLen].chairID = bOpCardChairID;
	pHc->FixedCardsLen++;
}


/*
void CGameLogic::ChangeCard(tagChangeCard stUserCard[4])
{
    //换牌，优先换到不是自己的牌
	_uint8 bCard[12] = {0};
	_uint8 bLen = 0;
    for (int i=0; i<4; i++)
    {
        if (stUserCard[i].bLen == 0)
        {
            continue;
        }
		memcpy(bCard + bLen, stUserCard[i].bCard, sizeof(_uint8)*stUserCard[i].bLen);
        bLen += stUserCard[i].bLen;
    }

	_uint8 btemplen = 0;
    for (int i=3; i>=0; i--)
    {
        memcpy(stUserCard[i].bCard, bCard+btemplen, sizeof(_uint8)*stUserCard[i].bLen);
        btemplen += stUserCard[i].bLen;
    }
}
*/
//输入：手牌，当前牌，返回值：不能碰则返回false
bool CGameLogic::EstimateFixCard(HandCards *pHC, _uint8 cbCardData)
{
	if (pHC->FixedCardsLen < 2)
	{
		return true;
	}
	_uint8 cbNum[MJ_TYPE_NUM] = {0};
	_uint8 cbType = CardType(cbCardData);
	for (int i=0; i<pHC->FixedCardsLen; i++)
	{
		cbNum[CardType(pHC->FixedCards[i].CardData)]++;
	}
	if (cbNum[cbType] > 0)
	{
		return true;
	}

	_uint8 cbTypeNum = 0;
	for (int i = 0; i < MJ_TYPE_NUM; ++i)
	{
		if (cbNum[i] > 0)++cbTypeNum;
	}
	return (cbTypeNum < 2);//碰了两种花色，第三种不能碰,
}

void CGameLogic::RemoveCard(HandCards *hcs, _uint8 c, int length)
{
	for (int i=0;i<length;i++)
	{
		ChuPai(hcs, c);
	}

}


void CGameLogic::RemoveCard(HandCards *pHc, _uint8 c)
{	
	for (int i = 0; i < pHc->ChangeableCardsLen;)
	{
		if (pHc->ChangeableCards[i] == c && (0 != --pHc->ChangeableCardsLen - i))
		{
			memmove(&pHc->ChangeableCards[i], &pHc->ChangeableCards[i + 1], pHc->ChangeableCardsLen - i);
			continue;
		}
		i++;
	}	
}


void CGameLogic::RemoveCard(_uint8 sCard[], _uint8 cblen, _uint8 c)
{
	bool bfind=false;
	for (int i=0;i < cblen; i++)
	{
		if (sCard[i] != c)continue;
		bfind = true;
		for (int j = i+1;j < cblen; j++)
		{
			sCard[j-1] = sCard[j];
		}
		break;
	}
	if (bfind)
	{	
		sCard[cblen-1]=INVALID_MJ_CARD;
	}
}

//功能函数 添加 wTableID 参数 防止同一时间内两桌发牌相同
void CGameLogic::RandomCard(_uint8 cbCard[], _uint8 cbLength, int iTableID)
{
	srand(_uint32(time(0)) + iTableID);

	int index;
	for (int i=0; i<cbLength; i++)
	{
		index = i + rand()%(cbLength-i);/*rand()%cbLength;*/
		if (i != index)
		{   
			_uint8 tempCard = cbCard[i];
			cbCard[i] = cbCard[index];
			cbCard[index] = tempCard;
		}
	}
}

void CGameLogic::RemoveCard(HandCards *hcs, _uint8 *c, int length)
{
	for (int i = 0; i < length; i++)
	{
		ChuPai(hcs, *(c + i));
	}
}

bool CGameLogic::FormatCards(HandCards *hcs, _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], _uint8 &Jiang)
{
	if (!hcs) return false;

	memset((char *)CardsFormatBuf, INVALID_MJ_CARD, HAND_CARD_KAN_MAX * 4);

	//-------------------
	_uint8 RestCardBuf[14] = { 0 };
	int count[4] = { 0 };
	int i, j, k;
	bool bJiang;
	bool b = false;
	int  SerialNum = 0;
	int JiangIndex = 0;
	//-------------------

	bJiang = GetJiang(hcs->ChangeableCards, hcs->ChangeableCardsLen, JiangIndex, Jiang, RestCardBuf);
	SerialNum = (hcs->ChangeableCardsLen - 2) / 3;

	if (SerialNum > 4) return false;
	if (hcs->ChangeableCardsLen - 2 > 13) return false;

	while (bJiang)
	{
		for (i = 0; i < 4; i++)
			count[i] = 0;
		for (i = 0; i < hcs->ChangeableCardsLen - 2; i++)
		{
			b = false;
			for (j = 0; j<SerialNum; j++)
			{
				if (count[j] == 0)
				{
					CardsFormatBuf[j][0] = RestCardBuf[i];
					count[j] ++;
					b = true;
					break;
				}
				else if (count[j] == 3)
					continue;
				else if (CardType(RestCardBuf[i]) > MJ_TYPE_BING)//字牌处理
				{
					if (CardsFormatBuf[j][count[j] - 1] == RestCardBuf[i])
					{
						CardsFormatBuf[j][count[j]] = RestCardBuf[i];
						count[j] ++;
						b = true;
						break;
					}
				}
				else//数牌处理
				{					
					//数牌刻子
					if (RestCardBuf[i] == CardsFormatBuf[j][count[j] - 1])
					{
						if (count[j] == 1)
						{
							if (RestCardBuf[i] == RestCardBuf[i + 1])
							{
								CardsFormatBuf[j][count[j]] = RestCardBuf[i];
								count[j] ++;
								b = true;
								break;
							}
						}
						else if (count[j] == 2)
						{
							if (CardsFormatBuf[j][0] == CardsFormatBuf[j][1])
							{
								CardsFormatBuf[j][count[j]] = RestCardBuf[i];
								count[j] ++;
								b = true;
								break;
							}
						}
					}
					//数牌顺子
					else if ((RestCardBuf[i] - CardsFormatBuf[j][count[j] - 1]) == 1)
					{
						CardsFormatBuf[j][count[j]] = RestCardBuf[i];
						count[j] ++;
						b = true;
						break;
					}
					
				}
			}
			if (!b)
				break;
		}
		if (b || SerialNum == 0)
		{
			if (hcs->FixedCardsLen > 4) return false;

			j = (hcs->ChangeableCardsLen - 2) / 3;

			for (i = 0; i < hcs->FixedCardsLen; i++)
			{
				if (j >= 4) return false;

				switch (hcs->FixedCards[i].state)
				{
				case TYPE_ZHIGANG:
				case TYPE_WANGANG:
				case TYPE_ANGANG:
				{
					for (k = 0; k < 4; k++)
					{
						CardsFormatBuf[j][k] = hcs->FixedCards[i].c[0];
					}
				}
				break;
				case TYPE_PENG:
				{
					for (k = 0; k < 3; k++)
					{
						CardsFormatBuf[j][k] = hcs->FixedCards[i].c[0];
					}
				}
				break;
				default:
					for (k = 0; k < 3; k++)
					{
						CardsFormatBuf[j][k] = hcs->FixedCards[i].c[k];
					}
					break;
				}
				j++;
			}
			return true;
		}
		JiangIndex += 2;
		bJiang = GetJiang(hcs->ChangeableCards, hcs->ChangeableCardsLen, JiangIndex, Jiang, RestCardBuf);
	}
	return false;
}

bool CGameLogic::FormatCards_FengShun(HandCards *hcs, _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], _uint8 &Jiang)
{
	if (!hcs) return false;

	memset((char *)CardsFormatBuf, INVALID_MJ_CARD, HAND_CARD_KAN_MAX * 4);

	//-------------------
	_uint8 RestCardBuf[14] = { 0 };
	int count[4] = { 0 };
	int i, j, k;
	bool bJiang;
	bool b = false;
	int  SerialNum = 0;
	int JiangIndex = 0;
	//-------------------

	bJiang = GetJiang(hcs->ChangeableCards, hcs->ChangeableCardsLen, JiangIndex, Jiang, RestCardBuf);
	SerialNum = (hcs->ChangeableCardsLen - 2) / 3;

	/*bool printInfo = false;
	if (hcs->ChangeableCards[0] == 0x11 && hcs->ChangeableCards[1] == 0x12 && hcs->ChangeableCards[2] == 0x13)
	{
		printInfo = true;
	}*/

	/*if (Jiang == 0x18 && printInfo)
	{
		log_debug("======================================");
		for (uint32_t jjj = 0; jjj < hcs->ChangeableCardsLen; jjj++)
		{
			log_debug("handCard = %#x", hcs->ChangeableCards[jjj]);
		}
		log_debug("======================================");

	}*/

	if (SerialNum > 4) return false;
	if (hcs->ChangeableCardsLen - 2 > 13) return false;

	while (bJiang)
	{
		for (i = 0; i < 4; i++)
			count[i] = 0;
		/*把风牌都记录起来*/
		std::vector<_uint8> fengPai;
		for (i = 0; i < hcs->ChangeableCardsLen - 2; i++)
		{
			b = false;
			for (j = 0; j < SerialNum; j++)
			{
				/*数算和中发白都在这里处理*/
				if (CardType(RestCardBuf[i]) != MJ_TYPE_FENG)
				{
					if (count[j] == 0)
					{
						CardsFormatBuf[j][0] = RestCardBuf[i];
						count[j] ++;
						b = true;
						break;
					}
					else if (count[j] == 3)
						continue;
					else//数牌处理
					{
						//数牌刻子
						if (RestCardBuf[i] == CardsFormatBuf[j][count[j] - 1])
						{
							if (count[j] == 1)
							{
								if (RestCardBuf[i] == RestCardBuf[i + 1])
								{
									CardsFormatBuf[j][count[j]] = RestCardBuf[i];
									count[j] ++;
									b = true;
									break;
								}
							}
							else if (count[j] == 2)
							{
								if (CardsFormatBuf[j][0] == CardsFormatBuf[j][1])
								{
									CardsFormatBuf[j][count[j]] = RestCardBuf[i];
									count[j] ++;
									b = true;
									break;
								}
							}
						}
						//数牌顺子
						else if ((RestCardBuf[i] - CardsFormatBuf[j][count[j] - 1]) == 1)
						{
							CardsFormatBuf[j][count[j]] = RestCardBuf[i];
							count[j] ++;
							b = true;
							break;
						}

					}
				}
				else {
					fengPai.push_back(RestCardBuf[i]);
					b = true;
					break;
				}
			}
			if (!b)
			{
				break;
			}
				
		}
		/*if (Jiang == 0x18 && printInfo)
		{
			log_debug("b = %d", b);
			log_debug("fengPai.size() = %d", fengPai.size());
		}*/

		if ((b || SerialNum == 0) && (fengPai.size() % 3 == 0))
		{
			if (hcs->FixedCardsLen > 4) return false;

			
			/*if (Jiang == 0x18 && printInfo)
			{
				log_debug("======================================");
				for (uint32_t jjj = 0; jjj < hcs->ChangeableCardsLen; jjj++)
				{
					log_debug("handCard = %#x", hcs->ChangeableCards[jjj]);
				}

				for (uint32_t zz = 0; zz < fengPai.size(); zz++)
				{
					log_debug("card = %#x", fengPai[zz]);
				}
				log_debug("======================================");
				for (uint32_t z = 0; z < 4; z++)
				{
					log_debug("count[z] = %d", count[z]);
				}
				log_debug("****************************************");
			}*/
			
			/*前面的顺子是否完整*/
			bool wanzheng = true;
			for (uint32_t z = 0; z < 4; z++)
			{
				if (count[z] != 0 && count[z] != 3)
				{
					wanzheng = false;
				}
			}
			/*风牌能组成的坎*/
			std::vector<std::vector<_uint8> > kan;
			if (FengPaiZuKan(fengPai, kan) && wanzheng)
			{
				/*log_debug("FengPaiZuKan");*/
				j = (hcs->ChangeableCardsLen - 2) / 3;

				/*把风牌组的坎加入CardsFormatBuf*/
				uint32_t insertPos = j - fengPai.size() / 3;
				for (uint32_t index = 0; index < kan.size(); index++)
				{
					for (uint32_t cardIndex = 0; cardIndex < kan[index].size(); cardIndex++)
					{
						CardsFormatBuf[insertPos][cardIndex] = kan[index][cardIndex];
					}
				}

				for (i = 0; i < hcs->FixedCardsLen; i++)
				{
					if (j >= 4) return false;

					switch (hcs->FixedCards[i].state)
					{
					case TYPE_ZHIGANG:
					case TYPE_WANGANG:
					case TYPE_ANGANG:
					{
						for (k = 0; k < 4; k++)
						{
							CardsFormatBuf[j][k] = hcs->FixedCards[i].c[0];
						}
					}
					break;
					case TYPE_PENG:
					{
						for (k = 0; k < 3; k++)
						{
							CardsFormatBuf[j][k] = hcs->FixedCards[i].c[0];
						}
					}
					break;
					default:
						for (k = 0; k < 3; k++)
						{
							CardsFormatBuf[j][k] = hcs->FixedCards[i].c[k];
						}
						break;
					}
					j++;
				}
				return true;
			}
		}
		JiangIndex += 2;
		bJiang = GetJiang(hcs->ChangeableCards, hcs->ChangeableCardsLen, JiangIndex, Jiang, RestCardBuf);
	}
	return false;
}

bool CGameLogic::FengPaiZuKan(const std::vector<_uint8> &fengPai, std::vector<std::vector<_uint8> >&kan)
{
	if (fengPai.size() == 0)
	{
		return true;
	} else if (fengPai.size() == 3)
	{
		if ((fengPai[0] != fengPai[1] && fengPai[1] != fengPai[2] && fengPai[0] != fengPai[2]) ||
			(fengPai[0] == fengPai[1] && fengPai[1] == fengPai[2]))
		{
			std::vector<_uint8> tmpKan;
			tmpKan.push_back(fengPai[0]);
			tmpKan.push_back(fengPai[1]);
			tmpKan.push_back(fengPai[2]);
			kan.push_back(tmpKan);
			return true;
		}
	}
	else if (fengPai.size() > 3)
	{
		/*先找顺子*/
		_uint8 searchGroup[4][3] = { { 0x31, 0x32, 0x33 }, /*东南西*/
									{ 0x31, 0x32, 0x34 }, /*东南北*/
									{ 0x31, 0x33, 0x34 }, /*东西北*/
									{ 0x32, 0x33, 0x34 }}; /*南西北*/
		for (_uint8 group = 0; group < 4; group++)
		{
			bool kanFind[3] = { 0 };
			for (_uint8 index = 0; index < fengPai.size(); index++)
			{
				if (fengPai[index] == searchGroup[group][0])
				{
					kanFind[0] = true;
				}
				else if (fengPai[index] == searchGroup[group][1])
				{
					kanFind[1] = true;
				}
				else if (fengPai[index] == searchGroup[group][2])
				{
					kanFind[2] = true;
				}
			}
			if (kanFind[0] && kanFind[1] && kanFind[2])
			{
				/*取出三张牌，将其它的牌放入tmp, 递归*/
				vector<_uint8> subset;
				bool eraseSign[3] = { 0 };
				for (_uint8 index = 0; index < fengPai.size(); index++)
				{
					if (eraseSign[0] == false && fengPai[index] == searchGroup[group][0])
					{
						eraseSign[0] = true;
					}
					else if (eraseSign[1] == false && fengPai[index] == searchGroup[group][1])
					{
						eraseSign[1] = true;
					}
					else if (eraseSign[2] == false && fengPai[index] == searchGroup[group][2])
					{
						eraseSign[2] = true;
					}
					else {
						subset.push_back(fengPai[index]);
					}
				}
				if (FengPaiZuKan(subset, kan))
				{
					std::vector<_uint8> tmpKan;
					tmpKan.push_back(searchGroup[group][0]);
					tmpKan.push_back(searchGroup[group][1]);
					tmpKan.push_back(searchGroup[group][2]);
					kan.push_back(tmpKan);
					return true;
				}
			}
		}

		for (_uint8 group = 0; group < 4; group++)
		{
			/*找不出任何顺子，则找一个刻子，往下递归*/
			int count = 0;
			for (_uint8 index = 0; index < fengPai.size(); index++)
			{
				if (fengPai[index] == 0x31 + group)
					count++;
			}
			if (count >= 3)
			{
				/*取出三张牌，递归*/
				std::vector<_uint8> subset;
				int eraseSign = 0;
				for (_uint8 index = 0; index < fengPai.size(); index++)
				{
					if (fengPai[index] == (0x31 + group) && eraseSign < 3)
					{
						eraseSign++;
					}
					else {
						subset.push_back(fengPai[index]);
					}
				}
				if (FengPaiZuKan(subset, kan))
				{
					std::vector<_uint8> tmpKan;
					tmpKan.push_back(0x31 + group);
					tmpKan.push_back(0x31 + group);
					tmpKan.push_back(0x31 + group);
					kan.push_back(tmpKan);
					return true;
				}
			}
		}
	}
	return false;
}

bool CGameLogic::GetJiang(_uint8* p, int len, int& JiangIndex, _uint8& jiang, _uint8* pRestCardBuf)
{
	SortCard(p,len);
	int i,j;
	for (i=0; i<JiangIndex; i++)
	{
        pRestCardBuf[i] = p[i]; 
	}
		
	for (i=JiangIndex; i<len-1; i++)
	{
		if (p[i] == p[i+1])
		{
			JiangIndex =i;
			jiang = p[i];
			for (j=i+2; j<len; j++)
			{
                pRestCardBuf[j-2]=p[j];
			}
			return true;
		}
		pRestCardBuf[i] = p[i];
	}

	JiangIndex=len-1;
	return false;
}
bool CGameLogic::HasCard(const HandCards *hc, _uint8 c)
{
	for (int i=0; i<hc->ChangeableCardsLen; i++)
	{
		if (hc->ChangeableCards[i]==c)
		{
			return true;
		}
	}
	return false;
}

_uint8 CGameLogic::NextCard(_uint8 bCard)
{
	_uint8 bCardNext = INVALID_MJ_CARD;
	_uint8 type = CardType(bCard);
	_uint8 num = CardNum(bCard);
	if (type == MJ_TYPE_WAN || type == MJ_TYPE_TIAO || type == MJ_TYPE_TONG)
	{
		if (num == 9)
		{
			num = 0;
		}
		bCardNext = (type << 4) | (num + 1);
	}
	else
	{
		switch (bCard)
		{
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x41:
		case 0x42:
			bCardNext = bCard + 1;
			break;
		case 0x34:
			bCardNext = 0x41;
			break;
		case 0x43:
			bCardNext = 0x31;
			break;
		default:
			assert(false);
			break;
		}
	}
	return bCardNext;
}

_uint8 CGameLogic::PrevCard(_uint8 bCard)
{
	_uint8 bCardPrev = INVALID_MJ_CARD;
	_uint8 type = CardType(bCard);
	_uint8 num = CardNum(bCard);
	if (type == MJ_TYPE_WAN || type == MJ_TYPE_TIAO || type == MJ_TYPE_TONG)
	{
		if (num == 1)
		{
			num = 10;
		}
		bCardPrev = (type << 4) | (num - 1);
	}
	else
	{
		switch (bCard)
		{
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x42:
		case 0x43:
			bCardPrev = bCard - 1;
			break;
		case 0x31:
			bCardPrev = 0x43;
			break;
		case 0x41:
			bCardPrev = 0x34;
			break;
		default:
			assert(false);
			break;
		}
	}
	return bCardPrev;
}
