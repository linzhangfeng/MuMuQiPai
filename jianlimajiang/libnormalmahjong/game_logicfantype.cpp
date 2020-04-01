#include "game_logic.h"



bool CGameLogic::IsQingYiSe(const HandCards* p)
{
	if(p == NULL) return false;

	_uint8 ct;
	if (p->ChangeableCardsLen == 0)
	{
		ct = CardType(p->FixedCards[0].CardData);
	}
	else
	{
		ct = CardType(p->ChangeableCards[0]);
	}
	if (ct == MJ_TYPE_FENG || ct == MJ_TYPE_JIAN)
		return false;

	for (int i=0; i<p->ChangeableCardsLen; i++)
		if (ct != CardType(p->ChangeableCards[i]))
			return false;

	if (p->FixedCards->state != TYPE_NULL)
	{
		for (int i=0; i<p->FixedCardsLen; i++)
		{
			if (ct != CardType(p->FixedCards[i].CardData))
				return false;
		}
	}

	return true;
}

bool CGameLogic::IsQingYiSe_01(const HandCards* p, _uint8 bExceptCard)
{
	HandCards tmpHc = *p;

	while (ChuPai(&tmpHc, bExceptCard));

	_uint8 ct;
	if (tmpHc.ChangeableCardsLen == 0)
	{
		if (tmpHc.FixedCards[0].CardData != bExceptCard)
			ct = CardType(tmpHc.FixedCards[0].CardData);
		else
			ct = CardType(tmpHc.FixedCards[1].CardData);
	}
	else
	{
		ct = CardType(tmpHc.ChangeableCards[0]);
	}
	if (ct == MJ_TYPE_FENG || ct == MJ_TYPE_JIAN)
		return false;

	for (int i=0; i<tmpHc.ChangeableCardsLen; i++)
		if (ct != CardType(tmpHc.ChangeableCards[i]))
			return false;

	if (tmpHc.FixedCards->state != TYPE_NULL)
	{
		for (int i=0; i<tmpHc.FixedCardsLen; i++)
		{
			if (tmpHc.FixedCards[i].CardData != bExceptCard)
			{
				if (ct != CardType(tmpHc.FixedCards[i].CardData))
					return false;
			}
		}
	}

	return true;
}

//如果进牌区有中发白的吃，排除
bool CGameLogic::IsQingYiSe_02(const HandCards* p)
{
	_uint8 ct = INVALID_MJ_CARD;
	if (p->ChangeableCardsLen == 0)
	{
		for (int i = 0; i < p->FixedCardsLen; ++i)
		{
			if ((p->FixedCards[i].state & TYPE_CHI) && (CardType(p->FixedCards[i].c[0]) > MJ_TYPE_TONG))
				continue;

			ct = CardType(p->FixedCards[i].CardData);
			break;
		}
	}
	else
	{
		ct = CardType(p->ChangeableCards[0]);
	}

	if (ct == MJ_TYPE_FENG || ct == MJ_TYPE_JIAN)
		return false;

	for (int i = 0; i < p->ChangeableCardsLen; i++)
		if (ct != CardType(p->ChangeableCards[i]))
			return false;

	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if ((p->FixedCards[i].state & TYPE_CHI) && (CardType(p->FixedCards[i].c[0]) > MJ_TYPE_TONG))
			continue;

		if (ct != CardType(p->FixedCards[i].CardData))
			return false;
	}

	return true;
}


bool CGameLogic::IsHunYiSe(const HandCards* p)
{
	if (p == NULL) return false;
	_uint8 CardDataNum[MJ_TYPE_COMMON] = { 0, 0, 0, 0, 0 };	

	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{		
		CardDataNum[CardType(p->ChangeableCards[i])]++;
	}
	
	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		CardDataNum[CardType(p->FixedCards[i].CardData)]++;
	}
	
	if (CardDataNum[MJ_TYPE_FENG] + CardDataNum[MJ_TYPE_JIAN] == 0)
		return false;

	int iCount = 0;
	for (int i = 0; i < MJ_TYPE_FENG; i++)
	{
		if (CardDataNum[i] != 0)
		{
			iCount++;
		}		
	}
	
	return (iCount == 1);	
}
bool CGameLogic::IsZiYiSe(const HandCards* p)
{
	if (p == NULL)
		return false;
	
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (!(CardType(p->ChangeableCards[i]) == MJ_TYPE_FENG || CardType(p->ChangeableCards[i]) == MJ_TYPE_JIAN))
			return false;		
	}
		

	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if (!(CardType(p->FixedCards[i].CardData) == MJ_TYPE_FENG || CardType(p->FixedCards[i].CardData) == MJ_TYPE_JIAN))
			return false;
	}
	return true;
}
bool CGameLogic::IsShiBaLuoHan(const HandCards* p)
{
	int iCounts = 0;
	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if (p->FixedCards[i].state & TYPE_GANG) iCounts++;
	}
	return iCounts >= 4;
}
int CGameLogic::IsDaXiaoSanYuan(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10])
{	
	static _uint8 c[3] = { MJ_HONG_ZHONG, MJ_FA_CAI, MJ_BAI_BAN };
	int iCount = 0;
	for (int i = 0; i < (int)sizeof(c); i++)
	{
		if (CardDataNum[CardType(c[i])][CardNum(c[i])] >= 3)
		{
			iCount += 2;
		}
		else if (CardDataNum[CardType(c[1])][CardNum(c[i])] == 2)
		{
			iCount += 1;
		}
	}	
	return iCount == 6 ? MJ_FAN_TYPE_DASANYUAN : iCount == 5 ? MJ_FAN_TYPE_XIAOSANYUAN : MJ_FAN_TYPE_NULL;
}
int CGameLogic::IsDaXiaoSiXi(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10])
{	
	static _uint8 c[4] = { 0x31, 0x32, 0x33, 0x34 };
	int iCount = 0;
	for (int i = 0; i < (int)sizeof(c); i++)
	{
		if (CardDataNum[CardType(c[i])][CardNum(c[i])] >= 3)
		{
			iCount += 2;
		}
		else if (CardDataNum[CardType(c[1])][CardNum(c[i])] == 2)
		{
			iCount += 1;
		}
	}
	return iCount == 8 ? MJ_FAN_TYPE_DASIXI : iCount == 7 ? MJ_FAN_TYPE_XIAOSIXI : MJ_FAN_TYPE_NULL;
}
int CGameLogic::L_IsDaXiaoSanYuan(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10], int iLaiziNum)
{
	static _uint8 c[3] = { MJ_HONG_ZHONG, MJ_FA_CAI, MJ_BAI_BAN };
	if (iLaiziNum == 0)
		return IsDaXiaoSanYuan(p, CardDataNum);

	int iCounts = 0;
	for (int i = 0; i < (int)sizeof(c); i++)
	{
		switch (CardDataNum[CardType(c[i])][CardNum(c[i])])
		{
		case 0:
			iCounts += 3; break;
		case 1:
			iCounts += 2; break;
		case 2:
			iCounts += 1; break;			
		}
	}
	if (iCounts > iLaiziNum)
		return MJ_FAN_TYPE_NULL;

	HandCards tmp;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == c[0] || p->ChangeableCards[i] == c[1] || p->ChangeableCards[i] == c[2])
			continue;
		AddCard(&tmp, p->ChangeableCards[i]);
	}

	if (FormatCards_Laizi(tmp.ChangeableCards, tmp.ChangeableCardsLen, iLaiziNum - iCounts))
	{
		return MJ_FAN_TYPE_DASANYUAN;
	}
	return MJ_FAN_TYPE_XIAOSANYUAN;
}
int CGameLogic::L_IsDaXiaoSiXi(const HandCards* p, const _uint8 CardDataNum[MJ_TYPE_COMMON][10], int iLaiziNum)
{
	static _uint8 c[4] = { 0x31, 0x32, 0x33, 0x34 };
	if (iLaiziNum == 0)
		return IsDaXiaoSiXi(p, CardDataNum);

	int iCounts = 0;
	for (int i = 0; i < (int)sizeof(c); i++)
	{
		switch (CardDataNum[CardType(c[i])][CardNum(c[i])])
		{
		case 0:			
			iCounts += 3; break;
		case 1:
			iCounts += 2; break;
		case 2:
			iCounts += 1; break;
		}
	}
	if (iCounts > iLaiziNum)
		return MJ_FAN_TYPE_NULL;
	HandCards tmp;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == c[0] || p->ChangeableCards[i] == c[1] || p->ChangeableCards[i] == c[2] || p->ChangeableCards[i] == c[3])
			continue;
		AddCard(&tmp, p->ChangeableCards[i]);
	}
	if (FormatCards_Laizi(tmp.ChangeableCards, tmp.ChangeableCardsLen, iLaiziNum - iCounts))
	{
		return MJ_FAN_TYPE_DASIXI;
	}
	return MJ_FAN_TYPE_XIAOSIXI;
}
bool CGameLogic::IsDai19(HandCards *p, _uint8 buf[4][4], int length, _uint8 Jiang)
{
	if(p == NULL) return false;
	for (int i=0; i<p->FixedCardsLen; i++)
	{
		if (CardNum(p->FixedCards[i].CardData)!=1 && CardNum(p->FixedCards[i].CardData)!=9)
		{
			return false;
		}
	}
	if (CardNum(Jiang)!=1 && CardNum(Jiang)!=9)
	{
        return false;
	}
	bool bFind = false;
	for (int i=0; i<length; i++)
	{
		if (CardNum(buf[i][0])==CardNum(buf[i][2]))	//	如果组合是刻字
		{   
			if ((length-i) >= 3)
			{
					if (buf[i+2][0]-buf[i+1][0]==1 && buf[i+1][0]-buf[i][0]==1)
					{
                        if (CardNum(buf[i][0])==1 || CardNum(buf[i+2][0])==9)
						{
							bFind = true;
						}
					}	
					if (i == 0 && buf[3][0]-buf[i+2][0]==1 && buf[i+2][0]-buf[i][0]==1)
					{
						if (CardNum(buf[i][0])==1 || CardNum(buf[3][0])==9)
						{
							bFind = true;
						}
					}
					if ((i == 0) && (buf[3][0]-buf[i+1][0]==1 && buf[i+1][0]-buf[i][0]==1))
					{
						if (CardNum(buf[i][0])==1 || CardNum(buf[3][0])==9)
						{
							bFind = true;
						}	
					}
					
			}
			else if (CardNum(buf[i][0])!=1 && CardNum(buf[i][0])!=9 && !bFind)
			{
                return false;
			}		
		}
		else
		{
			if (CardNum(buf[i][0])!=1 && CardNum(buf[i][2])!=9)
			{
                return false;
			}
				
		}
	}
	return true;
}
bool CGameLogic::IsQuan19(const HandCards& p, const _uint8 buf[4][4], _uint8 Jiang)
{
	for (int i = 0; i < p.ChangeableCardsLen; i++)
	{
		if (CardType(p.ChangeableCards[i]) >= MJ_TYPE_FENG)
		{
			return false;
		}
		if (!(CardNum(p.ChangeableCards[i]) == 1 || CardNum(p.ChangeableCards[i]) == 9))
		{
			return false;
		}
	}
	for (int i = 0; i < p.FixedCardsLen; i++)
	{
		if (p.FixedCards[i].state & TYPE_CHI)
		{
			return false;
		}
		if (CardType(p.FixedCards[i].CardData) >= MJ_TYPE_FENG)
		{
			return false;
		}
		if (!(CardNum(p.FixedCards[i].CardData) == 1 || CardNum(p.FixedCards[i].CardData) == 9))
		{
			return false;
		}		
	}
	return true;
}
//bool CGameLogic::IsQing19(const HandCards* p, const _uint8 buf[4][4], _uint8 Jiang)
//{//不判断全风
//	static _uint8 c[MJ_TYPE_COMMON][10] = { { -1, 0, -1, -1, -1, -1, -1, -1, -1, 1, }, { -1, 2, -1, -1, -1, -1, -1, -1, -1, 3, }, { -1, 4, -1, -1, -1, -1, -1, -1, -1, 5, }, };
//	char x[6] = { 0, 0, 0, 0, 0, 0 };
//	for (int i = 0; i < p->ChangeableCardsLen; i++)
//	{
//		if (CardType(p->ChangeableCards[i]) == MJ_TYPE_FENG || CardType(p->ChangeableCards[i]) == MJ_TYPE_JIAN)
//		{
//			continue;
//		}
//		if (!(CardNum(p->ChangeableCards[i]) == 1 || CardNum(p->ChangeableCards[i]) == 9))
//		{
//			return false;
//		}		
//		x[c[CardType(p->ChangeableCards[i])][CardNum(p->ChangeableCards[i])]] = -1;
//	}
//	for (int i = 0; i < p->FixedCardsLen; i++)
//	{
//		if (p->FixedCards[i].state & TYPE_CHI)
//		{
//			return false;
//		}
//		if (CardType(p->FixedCards[i].CardData) == MJ_TYPE_FENG || CardType(p->FixedCards[i].CardData) == MJ_TYPE_JIAN)
//		{
//			continue;
//		}
//		if (!(CardNum(p->FixedCards[i].CardData) == 1 || CardNum(p->FixedCards[i].CardData) == 9))
//		{
//			return false;
//		}		
//		x[c[CardType(p->FixedCards[i].CardData)][CardNum(p->FixedCards[i].CardData)]] = -1;
//	}
//	if (x[0] ^ x[1])
//		return false;
//	if (x[2] ^ x[3])
//		return false;
//	if (x[4] ^ x[5])
//		return false;		
//	char r = x[0] + x[1] + x[2] + x[3] + x[4] + x[5];	
//	return r == -2;
//}
bool CGameLogic::IsQuan19Wind(const HandCards& p, const _uint8 buf[4][4], _uint8 Jiang)
{
	_uint8 CardDataNum[MJ_TYPE_COMMON] = { 0, 0, 0, 0, 0 };
	for (int i = 0; i < p.ChangeableCardsLen; i++)
	{
		if (!(CardNum(p.ChangeableCards[i]) == 1 || CardNum(p.ChangeableCards[i]) == 9
			|| CardType(p.ChangeableCards[i]) == MJ_TYPE_FENG || CardType(p.ChangeableCards[i]) == MJ_TYPE_JIAN))
		{
			return false;
		}
		else
		{
			CardDataNum[CardType(p.ChangeableCards[i])]++;
		}
	}
	for (int i = 0; i < p.FixedCardsLen; i++)
	{
		if (p.FixedCards[i].state & TYPE_CHI)
		{
			return false;
		}
		if (!(CardNum(p.FixedCards[i].CardData) == 1 || CardNum(p.FixedCards[i].CardData) == 9
			|| CardType(p.FixedCards[i].CardData) == MJ_TYPE_FENG || CardType(p.FixedCards[i].CardData) == MJ_TYPE_JIAN))
		{
			return false;
		}		
		else
		{
			CardDataNum[CardType(p.FixedCards[i].CardData)]++;
		}		
	}
	return (CardDataNum[MJ_TYPE_WAN] + CardDataNum[MJ_TYPE_TIAO] + CardDataNum[MJ_TYPE_TONG] != 0);	
}
bool CGameLogic::Is7Dui(const HandCards *p)
{
	if (p->ChangeableCardsLen != 14)
		return false;

	for (int i=0; i<7; i++)
	{
		if (p->ChangeableCards[i*2] != p->ChangeableCards[i*2+1])
			return false;
	}
	return true;
}

bool CGameLogic::IsQingDui(HandCards* p)
{
	_uint8 bt = CardType(p->ChangeableCards[0]);
	for (int i=0; i<p->ChangeableCardsLen; i++)
	{
		if (bt != CardType(p->ChangeableCards[i]))
			return false;
	}

	for (int i=0; i<p->FixedCardsLen; i++)
	{
		if (bt != CardType(p->FixedCards[i].CardData))
			return false;
	}

	return true;
}

bool CGameLogic::IsJiangJiangHu(const HandCards* p, const _uint8 buf[4][4], const _uint8 Jiang)
{
	if(p == NULL) return false;
	for (int i=0; i<p->FixedCardsLen; i++)
	{
		if (CardNum(p->FixedCards[i].CardData)!=2 && CardNum(p->FixedCards[i].CardData)!=5 && CardNum(p->FixedCards[i].CardData)!=8)
			return false;
	}
	if (CardNum(Jiang)!=2 && CardNum(Jiang)!=5 && CardNum(Jiang)!=8)
		return false;

	//int x = 4 - p->FixedCardsLen;
	for (int i=0; i< 4; i++)
	{
		if (CardNum(buf[i][0])!=2 && CardNum(buf[i][0])!=5 && CardNum(buf[i][0])!=8)
			return false;
	}
	return true;
}

bool CGameLogic::IsLong7Dui(const HandCards* p, _uint8 &cbGenNum)
{
	if (p->ChangeableCardsLen != 14)
		return false;
	for (int i=0; i<6; i++)
	{
		if (p->ChangeableCards[i*2] != p->ChangeableCards[i*2+1])
			return false;
		if ((p->ChangeableCards[(i+1)*2] - p->ChangeableCards[i*2]) == 0)
			cbGenNum++;
	}
	if (cbGenNum >= 1)
		return(p->ChangeableCards[12] == p->ChangeableCards[13]);
	else
		return false;
}

bool CGameLogic::IsQing7Dui(const HandCards *p)
{
	if(p == NULL) return false;
	_uint8 ct = CardType(p->ChangeableCards[0]);
	for (int i=0; i<p->ChangeableCardsLen; i++)
		if (ct != CardType(p->ChangeableCards[i]))
			return false;

	return true;
}

// bool CGameLogic::IsQing19(HandCards* p)
// {
// 	if(p == NULL) return false;
// 	_uint8 bt = CardType(p->ChangeableCards[0]);
// 	for (int i=0; i<p->ChangeableCardsLen; i++)
// 	{
// 		if (bt != CardType(p->ChangeableCards[i]))
// 		{
// 			return false;
// 		}
// 	}
// 
// 	for (int i=0; i<p->FixedCardsLen; i++)
// 	{
// 		if (bt != CardType(p->FixedCards[i].CardData))
// 		{
// 			return false;
// 		}
// 	}
// 	
// 	return true;
// 
// }

bool CGameLogic::IsQingLong7Dui(HandCards* p)
{
	_uint8 ct = CardType(p->ChangeableCards[0]);

	for (int i=0; i<p->ChangeableCardsLen; i++)
		if (ct != CardType(p->ChangeableCards[i]))
			return false;

	return true;
}
bool CGameLogic::IsDanDiao(const HandCards& pHc)
{
	return (pHc.ChangeableCardsLen == 2) && (pHc.ChangeableCards[0] == pHc.ChangeableCards[1]);
}
bool CGameLogic::Has4SameCards(HandCards& pHc, _uint8 bCard)const
{
	int iCount = 0;
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] == bCard)
		{
			iCount++;
		}
	}
	return (iCount == 4);
}
bool CGameLogic::Has4SameCardsX(HandCards& pHc, _uint8 bCard)const
{
	int iCount = 0;
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] == bCard)
		{
			iCount++;
		}
	}
	for (int i = 0; i < pHc.FixedCardsLen; i++)
	{
		if (pHc.FixedCards[i].state & TYPE_CHI)
		{
			if (pHc.FixedCards[i].c[0] == bCard || 
				pHc.FixedCards[i].c[1] == bCard || 
				pHc.FixedCards[i].c[2] == bCard)
			{
				iCount++;
			}
		}
		else if (pHc.FixedCards[i].state & TYPE_PENG )
		{
			if (pHc.FixedCards[i].CardData == bCard)
			{
				iCount += 3;
			}
		}
		else if (pHc.FixedCards[i].state & TYPE_GANG)
		{
			if (pHc.FixedCards[i].CardData == bCard)
			{
				iCount += 4;
			}
		}
		
	}
	return (iCount == 4);
}

bool CGameLogic::IsXiaoSanYuan(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang)
{
    int jianCount = 0;
    for (int i=0; i<HAND_CARD_KAN_MAX; i++)
    {
        if (CardType(CardsFormatBuf[i][0]) == MJ_TYPE_JIAN)
		{
		    jianCount ++;
		}  
    }
    return (jianCount == 2) && (CardType(Jiang) == MJ_TYPE_JIAN);
}

bool CGameLogic::IsDaSanYuan(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang)
{
    int jianCount = 0;
    for (int i=0; i<HAND_CARD_KAN_MAX; i++)
    {
        if (CardType(CardsFormatBuf[i][0]) == MJ_TYPE_JIAN)
		{
            jianCount ++;
		}
    }
    return (jianCount == 3);
}

bool CGameLogic::IsMingSiGui(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang, _uint8 cbCard, bool bQuanPinDao)
{
	if (p->FixedCardsLen > HAND_CARD_KAN_MAX) return false;

	for (int i = (HAND_CARD_KAN_MAX - p->FixedCardsLen); i < HAND_CARD_KAN_MAX; ++i)
	{
		if (CardsFormatBuf[i][0] == CardsFormatBuf[i][1])
		{
			_uint8 cbCardData = CardsFormatBuf[i][0];
			for (int j = 0; j < (HAND_CARD_KAN_MAX - p->FixedCardsLen); ++j)
			{
				if(CardsFormatBuf[j][0] == CardsFormatBuf[j][1])
					continue;
				
				for (int k = 0; k < 4; ++k)
				{
					if(CardsFormatBuf[j][k] == cbCardData)
					{
						if (bQuanPinDao)
						{
							return true;
						}
						else
						{
							if (cbCardData == cbCard)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
    return false;
}

bool CGameLogic::IsAnSiGui(const HandCards* p, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang, _uint8 cbCard, bool bQuanPinDao)
{
	if (p->FixedCardsLen > HAND_CARD_KAN_MAX) return false;

	_uint8 bCardCount[MJ_TYPE_COMMON][10];
	memset(bCardCount, 0, sizeof(bCardCount));

	bCardCount[CardType(Jiang)][CardNum(Jiang)] += 2;

	for (int i = 0; i < (HAND_CARD_KAN_MAX - p->FixedCardsLen); ++i)
	{
		if (bQuanPinDao)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (CardsFormatBuf[i][j] == INVALID_MJ_CARD) continue;

				_uint8 iType = CardType(CardsFormatBuf[i][j]);
				_uint8 iNum = CardNum(CardsFormatBuf[i][j]);
				if (++bCardCount[iType][iNum] >= 4)
				{
					return true;
				}
			}
		}
		else
		{
			if (CardsFormatBuf[i][0] == CardsFormatBuf[i][1])
			{
				_uint8 cbCardData = CardsFormatBuf[i][0];
				for (int j = 0; j < (HAND_CARD_KAN_MAX - p->FixedCardsLen); ++j)
				{
					if(CardsFormatBuf[j][0] == CardsFormatBuf[j][1])
						continue;
					
					for (int k = 0; k < 4; ++k)
					{
						if(CardsFormatBuf[j][k] == cbCardData && cbCardData == cbCard)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
bool CGameLogic::IsKa5Xing(const HandCards* p, _uint8 cbCard, const _uint8 CardsFormatBuf[HAND_CARD_KAN_MAX][4], const _uint8 Jiang)
{
	if (p->FixedCardsLen > HAND_CARD_KAN_MAX) return false;

	for (int i = 0; i < (HAND_CARD_KAN_MAX - p->FixedCardsLen); ++i)
	{
		if (CardsFormatBuf[i][0] == CardsFormatBuf[i][1])
			continue;
		if (CardNum(cbCard) == 5 && CardsFormatBuf[i][1] == cbCard)
			return true;
	}
	return false;
}

bool CGameLogic::IsJiangYiSe(const HandCards* p)
{
	if(p == NULL) return false;

	for (int i=0; i<p->FixedCardsLen; i++)
	{
		if (CardType(p->FixedCards[i].CardData) > MJ_TYPE_TONG)
			return false;

		if (p->FixedCards[i].state & TYPE_CHI)
		{
			if ((CardNum(p->FixedCards[i].c[0])!=2 && CardNum(p->FixedCards[i].c[0])!=5 && CardNum(p->FixedCards[i].c[0])!=8)
				|| (CardNum(p->FixedCards[i].c[1])!=2 && CardNum(p->FixedCards[i].c[1])!=5 && CardNum(p->FixedCards[i].c[1])!=8)
				|| (CardNum(p->FixedCards[i].c[2])!=2 && CardNum(p->FixedCards[i].c[2])!=5 && CardNum(p->FixedCards[i].c[2])!=8))
				return false;
		}
		else
		{
			if (CardNum(p->FixedCards[i].CardData)!=2 && CardNum(p->FixedCards[i].CardData)!=5 && CardNum(p->FixedCards[i].CardData)!=8)
				return false;
		}
	}

	for (int i = 0; i < p->ChangeableCardsLen; ++i)
	{
		if (CardType(p->ChangeableCards[i]) > MJ_TYPE_TONG)
			return false;

		if (CardNum(p->ChangeableCards[i])!=2 && CardNum(p->ChangeableCards[i])!=5 && CardNum(p->ChangeableCards[i])!=8)
			return false;
	}
	
	return true;
}

//进牌区有仰牌，排除
bool CGameLogic::IsJiangYiSe_01(const HandCards* p)
{
	if(p == NULL) return false;

	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if ((p->FixedCards[i].state & TYPE_CHI) && (CardType(p->FixedCards[i].CardData) > MJ_TYPE_TONG))
			continue;

		if (CardType(p->FixedCards[i].CardData) > MJ_TYPE_TONG)
			return false;

		if (p->FixedCards[i].state & TYPE_CHI)
		{
			if ((CardNum(p->FixedCards[i].c[0])!=2 && CardNum(p->FixedCards[i].c[0])!=5 && CardNum(p->FixedCards[i].c[0])!=8)
				|| (CardNum(p->FixedCards[i].c[1])!=2 && CardNum(p->FixedCards[i].c[1])!=5 && CardNum(p->FixedCards[i].c[1])!=8)
				|| (CardNum(p->FixedCards[i].c[2])!=2 && CardNum(p->FixedCards[i].c[2])!=5 && CardNum(p->FixedCards[i].c[2])!=8))
				return false;
		}
		else
		{
			if (CardNum(p->FixedCards[i].CardData)!=2 && CardNum(p->FixedCards[i].CardData)!=5 && CardNum(p->FixedCards[i].CardData)!=8)
				return false;
		}
	}

	for (int i = 0; i < p->ChangeableCardsLen; ++i)
	{
		if (CardType(p->ChangeableCards[i]) > MJ_TYPE_TONG)
			return false;

		if (CardNum(p->ChangeableCards[i])!=2 && CardNum(p->ChangeableCards[i])!=5 && CardNum(p->ChangeableCards[i])!=8)
			return false;
	}
	
	return true;
}

bool CGameLogic::IsKa2Tiao(const HandCards* hcs, _uint8 bCard)
{
	if (bCard != 0x12) return false;

	if (hcs->ChangeableCardsLen == 0) return false;

	std::vector<_uint8> vecHc(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]);

	std::vector<_uint8>::iterator iter = std::find(vecHc.begin(), vecHc.end(), bCard);
	if (iter == vecHc.end())
	{
		return false;
	}
	vecHc.erase(iter);

	iter = std::find(vecHc.begin(), vecHc.end(), bCard - 1);
	if (iter == vecHc.end())
	{
		return false;
	}
	vecHc.erase(iter);

	iter = std::find(vecHc.begin(), vecHc.end(), bCard + 1);
	if (iter == vecHc.end())
	{
		return false;
	}
	vecHc.erase(iter);

	HandCards tmpHc;
	for (_uint8 i = 0; i < vecHc.size(); ++i)
	{
		tmpHc.ChangeableCards[tmpHc.ChangeableCardsLen++] = vecHc[i];
	}

	MJ_win_pattern tmppattern;
	if (FormatCards(&tmpHc, tmppattern.kan, tmppattern.jiang))
	{
		return true;
	}

	return false;
}

bool CGameLogic::IsKa5Xin(const HandCards* hcs, _uint8 bCard)
{
	if (CardNum(bCard) != 0x05) return false;

	if (hcs->ChangeableCardsLen == 0) return false;

	std::vector<_uint8> vecHc(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]);

	std::vector<_uint8>::iterator iter = std::find(vecHc.begin(), vecHc.end(), bCard);
	if (iter == vecHc.end())
	{
		return false;
	}
	vecHc.erase(iter);

	iter = std::find(vecHc.begin(), vecHc.end(), bCard - 1);
	if (iter == vecHc.end())
	{
		return false;
	}
	vecHc.erase(iter);

	iter = std::find(vecHc.begin(), vecHc.end(), bCard + 1);
	if (iter == vecHc.end())
	{
		return false;
	}
	vecHc.erase(iter);

	MJ_win_pattern tmppattern;
	if (FormatCards(vecHc, tmppattern))
	{
		return true;
	}

	return false;
}

bool CGameLogic::IsJiaHu(const HandCards* hcs, _uint8 bCard)
{
	if ((CardNum(bCard) == 1 || CardNum(bCard) == 9))
	{
		return false;
	}

	if (CardType(bCard) > MJ_TYPE_TONG)
	{
		return false;
	}

	if (hcs->ChangeableCardsLen == 0) return false;

	std::vector<_uint8> vec(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]);

	if (EraseCard(vec, bCard))
	{
		if (EraseCard(vec, bCard - 1))
		{
			if (EraseCard(vec, bCard + 1))
			{	
				MJ_win_pattern tmppattern;
				if (FormatCards(vec, tmppattern))
				{
					return true;
				}		
			}
		}
	}
	return false;
}

bool CGameLogic::IsBianHu(const HandCards* hcs, _uint8 bCard)
{
	if (!(CardNum(bCard) == 3 || CardNum(bCard) == 7))
	{
		return false;
	}
	if (CardType(bCard) > MJ_TYPE_TONG)
	{
		return false;
	}

	if (hcs->ChangeableCardsLen == 0) return false;

	std::vector<_uint8> vec(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]);

	_uint8 bCard1 = INVALID_MJ_CARD;
	_uint8 bCard2 = INVALID_MJ_CARD;
	if (CardNum(bCard) == 3)
	{
		bCard1 = bCard - 1;
		bCard2 = bCard - 2;
	}
	else if (CardNum(bCard) == 7)
	{
		bCard1 = bCard + 1;
		bCard2 = bCard + 2;
	}

	if (EraseCard(vec, bCard))
	{
		if (EraseCard(vec, bCard1))
		{
			if (EraseCard(vec, bCard2))
			{	
				MJ_win_pattern tmppattern;
				if (FormatCards(vec, tmppattern))
				{
					return true;
				}		
			}
		}
	}

	return false;

}

bool CGameLogic::IsDiaoHu(const HandCards* hcs, _uint8 bCard)
{
	if (hcs->ChangeableCardsLen == 0) return false;

	std::vector<_uint8> vec(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]);

	if (std::count(vec.begin(), vec.end(), bCard) < 2) return false;

	if (EraseCard(vec, bCard, 2))
	{
		std::vector<_uint8> vec_kan;
		if (CheckSubVector(vec, vec_kan))
		{
			return true;
		}	
	}

	return false;

}

bool CGameLogic::IsSiGuiYi(const HandCards* hcs, int &iCounts)
{
	iCounts = 0;

	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = {{0}, {0}, {0}, {0}, {0}};
	GetCardCount(hcs, CardDataNum);

	for (int i = 0; i < hcs->FixedCardsLen; ++i)
	{
		if (hcs->FixedCards[i].state == TYPE_PENG)
		{
			_uint8 bCard = hcs->FixedCards[i].CardData;
			CardDataNum[CardType(bCard)][CardNum(bCard)] += 3;
		}
		else if (hcs->FixedCards[i].state & TYPE_CHI)
		{
			for (int j = 0; j < 3; ++j)
			{
				_uint8 bCard = hcs->FixedCards[i].c[j];
				CardDataNum[CardType(bCard)][CardNum(bCard)]++;
			}
		}
	}

	for (int i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (CardDataNum[i][j] >= 4) iCounts++;
		}
	}

	return iCounts > 0;
}

int CGameLogic::GetSiGuiYi(const HandCards* hcs)
{
	int iCounts = 0;

	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = {{0}, {0}, {0}, {0}, {0}};
	GetCardCount(hcs, CardDataNum);

	for (int i = 0; i < hcs->FixedCardsLen; ++i)
	{
		if (hcs->FixedCards[i].state & TYPE_CHI)
		{
			for (int j = 0; j < 3; ++j)
			{
				_uint8 bCard = hcs->FixedCards[i].c[j];
				CardDataNum[CardType(bCard)][CardNum(bCard)]++;
			}
		}
		else
		{
			_uint8 bCard = hcs->FixedCards[i].CardData;
			CardDataNum[CardType(bCard)][CardNum(bCard)] += hcs->FixedCards[i].state == TYPE_PENG ? 3 : 4;
		}
	}

	for (int i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (CardDataNum[i][j] >= 4) iCounts++;
		}
	}

	return iCounts;
}

int CGameLogic::GetTypeCount(const HandCards* hcs)
{
	int iCounts = 0;

	_uint8 CardTypes[MJ_TYPE_COMMON] = {0};

	for (int i = 0; i < hcs->ChangeableCardsLen; ++i)
	{
		CardTypes[CardType(hcs->ChangeableCards[i])]++;
	}

	for (int i = 0; i < hcs->FixedCardsLen; ++i)
	{
		if (hcs->FixedCards[i].state & TYPE_CHI)
		{
			CardTypes[CardType(hcs->FixedCards[i].c[0])] += 3;
		}
		else if (hcs->FixedCards[i].state & TYPE_PENG)
		{
			CardTypes[CardType(hcs->FixedCards[i].CardData)] += 3;			
		}
		else
		{
			CardTypes[CardType(hcs->FixedCards[i].CardData)] += 4;
		}
	}

	for (int i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		if (CardTypes[i] > 0) iCounts++;
	}

	return iCounts;
}

int CGameLogic::GetTypeCount_01(const HandCards* hcs)
{
	int iCounts = 0;

	_uint8 CardTypes[MJ_TYPE_COMMON] = {0};

	for (int i = 0; i < hcs->ChangeableCardsLen; ++i)
	{
		int iType = CardType(hcs->ChangeableCards[i]);

		if (iType > MJ_TYPE_TONG) continue;

		CardTypes[iType]++;
	}

	for (int i = 0; i < hcs->FixedCardsLen; ++i)
	{
		if (hcs->FixedCards[i].state & TYPE_CHI)
		{
			CardTypes[CardType(hcs->FixedCards[i].c[0])] += 3;
		}
		else if (hcs->FixedCards[i].state & TYPE_PENG)
		{
			int iType = CardType(hcs->FixedCards[i].CardData);

			if (iType > MJ_TYPE_TONG) continue;

			CardTypes[iType] += 3;			
		}
		else
		{
			int iType = CardType(hcs->FixedCards[i].CardData);

			if (iType > MJ_TYPE_TONG) continue;

			CardTypes[CardType(hcs->FixedCards[i].CardData)] += 4;
		}
	}

	for (int i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		if (CardTypes[i] > 0) iCounts++;
	}

	return iCounts;
}

int CGameLogic::GetYiBanGaoCount(const HandCards* p)
{
	_uint8 tmpBuf[MJ_TYPE_FENG][10];
	memset(tmpBuf, 0, sizeof(tmpBuf));
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		_uint8 type = CardType(p->ChangeableCards[i]);
		_uint8 num = CardNum(p->ChangeableCards[i]);
		if (type < MJ_TYPE_FENG)
		{
			tmpBuf[type][num]++;
		}
	}

	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if (p->FixedCards[i].state & TYPE_CHI)
		{
			tmpBuf[CardType(p->FixedCards[i].c[0])][CardNum(p->FixedCards[i].c[0])]++;
			tmpBuf[CardType(p->FixedCards[i].c[1])][CardNum(p->FixedCards[i].c[1])]++;
			tmpBuf[CardType(p->FixedCards[i].c[2])][CardNum(p->FixedCards[i].c[2])]++;
		}
		else if(p->FixedCards[i].state & TYPE_PENG)
		{
			tmpBuf[CardType(p->FixedCards[i].CardData)][CardNum(p->FixedCards[i].CardData)] += 3;
		}
		else if (p->FixedCards[i].state & TYPE_GANG)
		{
			tmpBuf[CardType(p->FixedCards[i].CardData)][CardNum(p->FixedCards[i].CardData)] += 4;
		}
	}

	int iCount = 0;
	for (int i = 0; i < MJ_TYPE_FENG; i++)
	{
		_uint8 cardBuf[10] = { 0 };
		for (int j = 1; j < 10; j++)
		{
			if (tmpBuf[i][j] >= 2)
			{
				cardBuf[j] = j;
			}
		}
		for (int ci = 1; ci < 8;)
		{
			if (cardBuf[ci] > 0 && cardBuf[ci + 1] > 0 && cardBuf[ci + 2] > 0)
			{
				iCount++;
				ci += 3;
			}
			else
			{
				ci++;
			}
		}
	}

	return iCount;
}

/*是否是烂牌*/
bool CGameLogic::IsLanPai(HandCards pHc, bool &qixing)
{
	if (pHc.ChangeableCardsLen != 14)
		return false;

	/*对手牌排序*/
	SortCard(pHc.ChangeableCards, pHc.ChangeableCardsLen);
	std::vector<_uint8> fenPai;
	fenPai.clear();
	std::vector<_uint8> color[3];
	for (int i = 0; i < 3; i++)
	{
		color[i].clear();
	}
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] > 0x30)  //是风牌
		{
			fenPai.push_back(pHc.ChangeableCards[i]);
			continue;
		}
		_uint8 type = CardType(pHc.ChangeableCards[i]);
		if (type >= 3)
		{
			return false;
		}
		color[type].push_back(pHc.ChangeableCards[i]);  //万筒条
	}
	if (fenPai.size() < 5 || fenPai.size() > 7)   //风牌少于5张或风牌大于七张
		return false;
	//风牌是否有重复
	for (size_t i = 1; i < fenPai.size(); i++)
	{
		if (fenPai[i] == fenPai[i - 1])
			return false;
	}
	/*判断筒条万的规则*/
	for (int i = 0; i < 3; i++)
	{
		for (size_t j = 1; j < color[i].size(); j++)
		{
			if (color[i][j] - color[i][j - 1] <= 2)
			{
				return false;
			}
		}
	}
	/*8.七星十三烂*/
	if (fenPai.size() == 7)
		qixing = true;

	return true;
}

bool CGameLogic::IsJiangDui(const HandCards* p)
{
	if(p == NULL) return false;

	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if (p->FixedCards[i].state & TYPE_CHI) return false;
		if (!Is258Card(p->FixedCards[i].CardData)) return false;
	}

	for (int i = 0; i < p->ChangeableCardsLen; ++i)
	{
		if (!Is258Card(p->ChangeableCards[i])) return false;
	}

	return true;
}

bool CGameLogic::Is19(_uint8 bCard)
{
	return (CardType(bCard) <= MJ_TYPE_TONG && (CardNum(bCard) == 1 || CardNum(bCard) == 9));
}

bool CGameLogic::IsDai19(HandCards *p, MJ_win_pattern pattern)
{
	for (int i = 0; i < p->FixedCardsLen; ++i)
	{
		if (p->FixedCards[i].state & TYPE_CHI)
		{
			if (!Is19(p->FixedCards[i].c[0]) && !Is19(p->FixedCards[i].c[1]) && !Is19(p->FixedCards[i].c[2])) return false;
		}
		else
		{
			if (!Is19(p->FixedCards[i].CardData)) return false;
		}
	}

	if (!Is19(pattern.jiang)) return false;

	for (int i = 0; i < HAND_CARD_KAN_MAX; ++i)
	{
		if (pattern.kan[i][0] == 0 || pattern.kan[i][0] == INVALID_MJ_CARD) continue;
		if (!Is19(pattern.kan[i][0]) && !Is19(pattern.kan[i][1]) && !Is19(pattern.kan[i][2])) return false;
	}

	return true;
}

bool CGameLogic::IsWu19(HandCards *p)
{
	for (int i = 0; i < p->FixedCardsLen; ++i)
	{
		if (p->FixedCards[i].state & TYPE_CHI)
		{
			if (Is19(p->FixedCards[i].c[0]) || Is19(p->FixedCards[i].c[1]) || Is19(p->FixedCards[i].c[2])) return false;
		}
		else
		{
			if (Is19(p->FixedCards[i].CardData)) return false;
		}
	}

	for (int i = 0; i < p->ChangeableCardsLen; ++i)
	{
		if (Is19(p->ChangeableCards[i])) return false;
	}

	return true;
}

int CGameLogic::GetMingSiGui(HandCards *p)
{
	int iCounts = 0;
	for (int i = 0; i < p->FixedCardsLen; ++i)
	{
		if (p->FixedCards[i].state == TYPE_PENG)
		{
			for (int j = 0; j < p->ChangeableCardsLen; ++j)
			{
				if (p->ChangeableCards[j] == p->FixedCards[i].CardData)
				{
					iCounts++;
				}
			}
		}
	}

	return iCounts;
}

int CGameLogic::GetAnSiGui(HandCards *p)
{
	int iCounts = 0;

	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = {{0}, {0}, {0}, {0}, {0}};
	GetCardCount(p, CardDataNum);

	for (int i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (CardDataNum[i][j] >= 4) iCounts++;
		}
	}

	return iCounts;
}

bool CGameLogic::IsDuiZiHu(HandCards *hcs, _uint8 bCard)
{
	std::vector<_uint8> vecHc(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]);

	if (std::count(vecHc.begin(), vecHc.end(), bCard) < 3) return false;

	EraseCard(vecHc, bCard, 3);

	MJ_win_pattern pattern;
	if (FormatCards(vecHc, pattern))
	{
		return true;
	}

	return false;
}

//手上全是1或者9或者字牌
bool CGameLogic::Is14Yao(HandCards hcs)
{
	for (int i = 0; i < hcs.FixedCardsLen; ++i)
	{
		if (CardType(hcs.FixedCards[i].CardData) <= MJ_TYPE_TONG)
		{
			if (hcs.FixedCards[i].state & TYPE_CHI) return false;
			if (CardNum(hcs.FixedCards[i].CardData) != 1 && CardNum(hcs.FixedCards[i].CardData) != 9) return false;
		}
	}

	for (int i = 0; i < hcs.ChangeableCardsLen; ++i)
	{
		if (CardType(hcs.ChangeableCards[i]) <= MJ_TYPE_TONG)
		{
			if (CardNum(hcs.ChangeableCards[i]) != 1 && CardNum(hcs.ChangeableCards[i]) != 9)
				return false;
		} 
	}

	return true;
}