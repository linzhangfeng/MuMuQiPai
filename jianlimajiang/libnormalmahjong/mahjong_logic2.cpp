#include <assert.h>
#include "game_logic.h"

bool CGameLogic::Is13Yao(const HandCards* p, _uint8 c, _uint8 CardsFormatBuf[4][4], _uint8 Jiang)
{
	static _uint8 YaoPai[13] = { 0x01, 0x09, 0x11, 0x19, 0x21, 0x29, 0x31, 0x32, 0x33, 0x34, 0x41, 0x42, 0x43 };
	HandCards thc;
	int  DuiCount = 0;

	thc = *p;
	AddCard(&thc, c);

	//±ØÐëÃÅÇå
	if (thc.ChangeableCardsLen != 14)
	{
		return false;
	}
	for (int i = 0, j = 0; i < MAX_COUNT; i++, j++)
	{
		if (thc.ChangeableCards[i] == YaoPai[j])
		{
			continue;
		}
		else if (thc.ChangeableCards[i] == thc.ChangeableCards[i - 1] && thc.ChangeableCards[i] == YaoPai[j - 1])
		{
			j--;
			DuiCount++;
		}
		else
		{
			return false;
		}
	}
	ChuPai(&thc, c);

	return (DuiCount == 1);
}


bool CGameLogic::IsQuanqiuren(const HandCards* pHc)
{

	if (pHc->ChangeableCardsLen - 2 == 0)
	{
		if (pHc->ChangeableCards[0] != pHc->ChangeableCards[1])
		{
			return false;
		}
		for (int i = 0; i < pHc->FixedCardsLen; i++)
		{
			if (pHc->FixedCards[i].state == TYPE_ANGANG)
			{
				return false;
			}
		}		
		return true;
	}

	return false;
}

bool CGameLogic::IsQuanQiuRen(const HandCards* pHc)
{
	if (pHc->ChangeableCardsLen == 2 && pHc->ChangeableCards[0] == pHc->ChangeableCards[1] 
		&& Is258Card(pHc->ChangeableCards[0])) 
	{
		return true;
	}

	return false;
}

bool CGameLogic::IsQuanQiuRen_01(const HandCards* pHc)
{
	for (int i = 0; i < pHc->FixedCardsLen; ++i)
	{
		if (pHc->FixedCards[i].state == TYPE_ANGANG)
			return false;
	}
	
	if (pHc->ChangeableCardsLen == 2 && pHc->ChangeableCards[0] == pHc->ChangeableCards[1] 
		&& Is258Card(pHc->ChangeableCards[0])) 
	{
		return true;
	}

	return false;
}

bool CGameLogic::IsQuanQiuRen_02(const HandCards* pHc, int iLaiziNum)
{
	if ( (iLaiziNum == 0 && pHc->ChangeableCardsLen == 2 && pHc->ChangeableCards[0] == pHc->ChangeableCards[1] && Is258Card(pHc->ChangeableCards[0]))
		|| (iLaiziNum == 1 && pHc->ChangeableCardsLen == 1 && Is258Card(pHc->ChangeableCards[0])) ) 
	{
		return true;
	}

	return false;
}

bool CGameLogic::IsQuanQiuRen_03(const HandCards* pHc, int iLaiziNum)
{
	for (int i = 0; i < pHc->FixedCardsLen; ++i)
	{
		if (pHc->FixedCards[i].state == TYPE_ANGANG)
			return false;
	}

	if ( (iLaiziNum == 0 && pHc->ChangeableCardsLen == 2 && pHc->ChangeableCards[0] == pHc->ChangeableCards[1] && Is258Card(pHc->ChangeableCards[0]))
		|| (iLaiziNum == 1 && pHc->ChangeableCardsLen == 1 && Is258Card(pHc->ChangeableCards[0])) ) 
	{
		return true;
	}

	return false;
}

bool CGameLogic::IsQuanQiuRen_04(const HandCards* pHc, int iLaiziNum)
{
	for (int i = 0; i < pHc->FixedCardsLen; ++i)
	{
		if (pHc->FixedCards[i].state == TYPE_ANGANG)
			return false;
	}

	if ( (iLaiziNum == 0 && pHc->ChangeableCardsLen == 2 && pHc->ChangeableCards[0] == pHc->ChangeableCards[1])
		|| (iLaiziNum == 1 && pHc->ChangeableCardsLen == 1) )  
	{
		return true;
	}

	return false;
}


bool CGameLogic::Is258Card(_uint8 bCard) 
{
	if (CardType(bCard) > MJ_TYPE_TONG)
		return false;

	if (CardNum(bCard) != 2 && CardNum(bCard) != 5 && CardNum(bCard) != 8)
		return false;

	return true;
}

int CGameLogic::GetChi(const HandCards *p, _uint8 cbCard, MJ_opinfo& ChiInfo)
{

	if (CardType(cbCard) >= MJ_TYPE_FENG) return TYPE_NULL;

	int cbMask = TYPE_NULL;
	bool bFind[4] = { false, false, false, false };
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] + 2 == cbCard)
		{
			bFind[0] = true;
		}
		else if (p->ChangeableCards[i] + 1 == cbCard)
		{
			bFind[1] = true;
		}
		else if (p->ChangeableCards[i] == cbCard + 1)
		{
			bFind[2] = true;
		}
		else if (p->ChangeableCards[i] == cbCard + 2)
		{
			bFind[3] = true;
		}
	}
	if (bFind[0] && bFind[1])
	{
		cbMask |= TYPE_RIGHT_CHI;
		ChiInfo.Add(OpState(cbCard, TYPE_RIGHT_CHI));
	}
	if (bFind[1] && bFind[2])
	{
		cbMask |= TYPE_CENTER_CHI;
		ChiInfo.Add(OpState(cbCard, TYPE_CENTER_CHI));
	}
	if (bFind[2] && bFind[3])
	{
		cbMask |= TYPE_LEFT_CHI;
		ChiInfo.Add(OpState(cbCard, TYPE_LEFT_CHI));
	}
	return cbMask;
}

int CGameLogic::GetZiChi(const HandCards *p, _uint8 cbCard, MJ_opinfo& ChiInfo)
{
	/*一定要是字牌才进来判断*/
	if (CardType(cbCard) < MJ_TYPE_FENG) return TYPE_NULL;
	if (CardType(cbCard) > MJ_TYPE_JIAN) return TYPE_NULL;

	int cbMask = TYPE_NULL;
	if (cbCard >= 0x31 && cbCard <= 0x34)
	{
		bool bFind[4] = { false, false, false, false };
		for (int i = 0; i < p->ChangeableCardsLen; i++)
		{
			if (p->ChangeableCards[i] < 0x31 || p->ChangeableCards[i] > 0x34)
			{
				continue;
			}
			if (p->ChangeableCards[i] == 0x31)
			{
				bFind[0] = true;
			}
			else if (p->ChangeableCards[i] == 0x32)
			{
				bFind[1] = true;
			}
			else if (p->ChangeableCards[i] == 0x33)
			{
				bFind[2] = true;
			}
			else if (p->ChangeableCards[i] == 0x34)
			{
				bFind[3] = true;
			}
		}
		int i = cbCard - 0x31;

		if (bFind[(i + 1) % 4] && bFind[(i + 2) % 4])
		{
			cbMask |= TYPE_LEFT_CHI;
			ChiInfo.Add(OpState(cbCard, TYPE_LEFT_CHI));
		}
		if (bFind[(i + 2) % 4] && bFind[(i + 3) % 4])
		{
			cbMask |= TYPE_RIGHT_CHI;
			ChiInfo.Add(OpState(cbCard, TYPE_RIGHT_CHI));
		}
		if (bFind[(i + 1) % 4] && bFind[(i + 3) % 4])
		{
			cbMask |= TYPE_CENTER_CHI;
			ChiInfo.Add(OpState(cbCard, TYPE_CENTER_CHI));
		}
	}
	else if (cbCard >= 0x41 && cbCard <= 0x43)
	{
		bool bFind[3] = { false, false, false};
		for (int i = 0; i < p->ChangeableCardsLen; i++)
		{
			if (p->ChangeableCards[i] < 0x41 || p->ChangeableCards[i] > 0x43)
			{
				continue;
			}
			if (p->ChangeableCards[i] == 0x41)
			{
				bFind[0] = true;
			} 
			else if(p->ChangeableCards[i] == 0x42)
			{
				bFind[1] = true;
			}
			else if (p->ChangeableCards[i] == 0x43)
			{
				bFind[2] = true;
			}
		}
		if (cbCard == 0x41 && bFind[1] && bFind[2])
		{
			cbMask |= TYPE_LEFT_CHI;
			ChiInfo.Add(OpState(cbCard, TYPE_LEFT_CHI));
		}
		else if (cbCard == 0x42 && bFind[0] && bFind[2])
		{
			cbMask |= TYPE_CENTER_CHI;
			ChiInfo.Add(OpState(cbCard, TYPE_CENTER_CHI));
		}
		else if (cbCard == 0x43 && bFind[0] && bFind[1])
		{
			cbMask |= TYPE_RIGHT_CHI;
			ChiInfo.Add(OpState(cbCard, TYPE_RIGHT_CHI));
		}
	}
	
	return cbMask;
}

void CGameLogic::ExecuteLeftChi_Feng(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard)
{
	_uint8 removeCard1 = 0;
	_uint8 removeCard2 = 0;
	removeCard1 = ((cbOperateCard - 0x31) + 1) % 4 + 0x31;
	removeCard2 = ((cbOperateCard - 0x31) + 2) % 4 + 0x31;
	//删除扑克
	_uint8 cbRemoveCard[] = { removeCard1, removeCard2 };
	RemoveCard(handcard, (_uint8*)cbRemoveCard, 2);

	handcard->FixedCards[handcard->FixedCardsLen].c[0] = cbOperateCard;
	handcard->FixedCards[handcard->FixedCardsLen].c[1] = removeCard1;
	handcard->FixedCards[handcard->FixedCardsLen].c[2] = removeCard2;
	handcard->FixedCards[handcard->FixedCardsLen].state = TYPE_LEFT_CHI;
	handcard->FixedCards[handcard->FixedCardsLen].chairID = chairID;
	handcard->FixedCardsLen++;
}

void CGameLogic::ExecuteCenterChi_Feng(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard)
{
	_uint8 removeCard1 = 0;
	_uint8 removeCard2 = 0;
	removeCard1 = cbOperateCard > 0x31 ? cbOperateCard - 1 : 0x34;
	removeCard2 = ((cbOperateCard - 0x31) + 1) % 4 + 0x31;
	//删除扑克
	_uint8 cbRemoveCard[] = { removeCard1, removeCard2 };
	RemoveCard(handcard, (_uint8*)cbRemoveCard, 2);

	handcard->FixedCards[handcard->FixedCardsLen].c[0] = removeCard1;
	handcard->FixedCards[handcard->FixedCardsLen].c[1] = cbOperateCard;
	handcard->FixedCards[handcard->FixedCardsLen].c[2] = removeCard2;
	handcard->FixedCards[handcard->FixedCardsLen].state = TYPE_CENTER_CHI;
	handcard->FixedCards[handcard->FixedCardsLen].chairID = chairID;
	handcard->FixedCardsLen++;
}

void CGameLogic::ExecuteRightChi_Feng(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard)
{
	_uint8 removeCard1 = 0;
	_uint8 removeCard2 = 0;
	removeCard1 = cbOperateCard > 0x32 ? cbOperateCard - 2 : (cbOperateCard > 0x31 ? 0x34 : 0x33);
	removeCard2 = cbOperateCard > 0x31 ? cbOperateCard - 1 : 0x34;
	//删除扑克
	_uint8 cbRemoveCard[] = { removeCard1, removeCard2 };
	RemoveCard(handcard, (_uint8*)cbRemoveCard, 2);

	handcard->FixedCards[handcard->FixedCardsLen].c[0] = removeCard1;
	handcard->FixedCards[handcard->FixedCardsLen].c[1] = removeCard2;
	handcard->FixedCards[handcard->FixedCardsLen].c[2] = cbOperateCard;
	handcard->FixedCards[handcard->FixedCardsLen].state = TYPE_RIGHT_CHI;
	handcard->FixedCards[handcard->FixedCardsLen].chairID = chairID;
	handcard->FixedCardsLen++;
}

void CGameLogic::ExecuteLeftChi(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard)
{
	if (CardType(cbOperateCard) == MJ_TYPE_FENG)
	{
		ExecuteLeftChi_Feng(handcard, chairID, cbOperateCard);
		return;
	}

	_uint8 cbRemoveCard[] = { _uint8(cbOperateCard + 1), _uint8(cbOperateCard + 2) };
	RemoveCard(handcard, (_uint8*)cbRemoveCard, 2);

	handcard->FixedCards[handcard->FixedCardsLen].c[0] = cbOperateCard;
	handcard->FixedCards[handcard->FixedCardsLen].c[1] = _uint8(cbOperateCard + 1);
	handcard->FixedCards[handcard->FixedCardsLen].c[2] = _uint8(cbOperateCard + 2);
	handcard->FixedCards[handcard->FixedCardsLen].state = TYPE_LEFT_CHI;
	handcard->FixedCards[handcard->FixedCardsLen].chairID = chairID;
	handcard->FixedCardsLen++;
}

void CGameLogic::ExecuteCenterChi(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard)
{
	if (CardType(cbOperateCard) == MJ_TYPE_FENG)
	{
		ExecuteCenterChi_Feng(handcard, chairID, cbOperateCard);
		return;
	}

	_uint8 cbRemoveCard[] = { _uint8(cbOperateCard - 1), _uint8(cbOperateCard + 1) };
	RemoveCard(handcard, (_uint8*)cbRemoveCard, 2);

	handcard->FixedCards[handcard->FixedCardsLen].c[0] = _uint8(cbOperateCard - 1);
	handcard->FixedCards[handcard->FixedCardsLen].c[1] = cbOperateCard;
	handcard->FixedCards[handcard->FixedCardsLen].c[2] = _uint8(cbOperateCard + 1);
	handcard->FixedCards[handcard->FixedCardsLen].state = TYPE_CENTER_CHI;
	handcard->FixedCards[handcard->FixedCardsLen].chairID = chairID;
	handcard->FixedCardsLen++;
}

void CGameLogic::ExecuteRightChi(HandCards *handcard, _uint8 chairID, _uint8 cbOperateCard)
{
	if (CardType(cbOperateCard) == MJ_TYPE_FENG)
	{
		ExecuteRightChi_Feng(handcard, chairID, cbOperateCard);
		return;
	}
	
	_uint8 cbRemoveCard[] = { _uint8(cbOperateCard - 2), _uint8(cbOperateCard - 1) };
	RemoveCard(handcard, (_uint8*)cbRemoveCard, 2);

	handcard->FixedCards[handcard->FixedCardsLen].c[0] = _uint8(cbOperateCard - 2);
	handcard->FixedCards[handcard->FixedCardsLen].c[1] = _uint8(cbOperateCard - 1);
	handcard->FixedCards[handcard->FixedCardsLen].c[2] = cbOperateCard;
	handcard->FixedCards[handcard->FixedCardsLen].state = TYPE_RIGHT_CHI;
	handcard->FixedCards[handcard->FixedCardsLen].chairID = chairID;
	handcard->FixedCardsLen++;
}

bool CGameLogic::IsPengPengHu(const HandCards* p, const _uint8 CardsFormatBuf[4][4], _uint8 Jiang)
{	
	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = { { 0 } };
	GetCardCount(p, CardDataNum);
	return L_IsPengPengHu(p, INVALID_MJ_CARD, CardDataNum, 0);	
}

bool CGameLogic::IsMenQing(const HandCards* p)
{
	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if (p->FixedCards[i].state != TYPE_ANGANG)
			return false;
	}
	return true;
}
bool CGameLogic::IsMenQing_02(const HandCards* p)
{
	if (p->FixedCardsLen == 0)
	{
		return true;
	}
	return false;
}

bool CGameLogic::IsMenQing_03(const HandCards* p)
{
	for (int i = 0; i < p->FixedCardsLen; i++)
	{
		if ((p->FixedCards[i].state & TYPE_CHI) && (CardType(p->FixedCards[i].c[0]) > MJ_TYPE_TONG))
			continue;

		if (p->FixedCards[i].state != TYPE_ANGANG)
			return false;
	}
	return true;
}

bool CGameLogic::IsYiTiaoLong(const HandCards* p)
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
	}
	for (int i = 0; i < MJ_TYPE_FENG; i++)
	{
		int iCount = 0;
		for (int j = 1; j < 10; j++)
		{
			if (tmpBuf[i][j] >0)
			{
				iCount++;
			}
		}
		if (iCount == 9)
			return true;
	}
	return false;
}

bool CGameLogic::IsYiTiaoLong(const HandCards* p, int cardNum[MJ_TYPE_COMMON][10], _uint8 laizicard, MJ_ten TiCards)
{
	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = {{0},};

	GetCardCount(p, CardDataNum);
	GetCardCountExt(p, CardDataNum);

	for (int i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (TiCards.Find(CardValue(i, j)))
			{
				CardDataNum[CardType(laizicard)][CardNum(laizicard)] += CardDataNum[i][j];
				CardDataNum[i][j] = 0;
			}
		}
	}

	int maxCount = 0;
	for (int i = 0; i < MJ_TYPE_COMMON; i++)
	{
		int iCount = 0;
		for (int j = 1; j < 10; j++)
		{
			if (CardDataNum[i][j] > 0)
			{
				iCount++;
			}
		}
		if (iCount > maxCount) 
		{
			maxCount = iCount;
			for (int j = 1; j < 10; j++)
			{
				cardNum[i][j] = CardDataNum[i][j];
			}
		}
		if (iCount == 9) 
		{
			HandCards tmp = *p;
			for(int j = 1; j < 10; j++)
			{
				RemoveCard(&tmp, CardValue(i, j), 1);
			}
			MJ_win_pattern pattern;
			if(FormatCards(&tmp, pattern.kan, pattern.jiang))
			{
				return true;
			}	
		}
	}
	return false;
}

bool CGameLogic::IsYiTiaoLong_02(const HandCards* p)
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
	}
	for (int i = 0; i < MJ_TYPE_FENG; i++)
	{
		int iCount = 0;
		for (int j = 1; j < 10; j++)
		{
			if (tmpBuf[i][j] >0)
			{
				iCount++;  
			}
		}
		if (iCount == 9)
		{
			HandCards handcard1 = *p;
			for(int j=1;j<10;j++)
			{
				ChuPai(&handcard1, CardValue(i,j));
			}
			MJ_win_pattern pattern;
			if(FormatCards(&handcard1, pattern.kan,pattern.jiang))
			{
				return true;

			}
		}
	}
	return false;
}

bool CGameLogic::IsYiTiaoLong_03(const HandCards* p)
{
	_uint8 holeCards[MJ_TYPE_COMMON][10];
	memset(holeCards, 0, sizeof(holeCards));

	for (int i = 0; i < p->ChangeableCardsLen; ++i){
		_uint8 card = p->ChangeableCards[i];
		holeCards[CardType(card)][CardNum(card)]++;
	}

	bool bYiTiaoLong = true;
	for (int i = 0; i < MJ_TYPE_FENG; ++i){	
		bYiTiaoLong = true;
		for (int j = 1; j < 10; ++j){
			if (0 == holeCards[i][j]){
				bYiTiaoLong = false;
				break;
			}
		}
		if (bYiTiaoLong){
			return true;
		}
	}

	return false;
}

bool CGameLogic::FormatCards_Laizi(_uint8 card[14], int nLen, int nLaiZiNum)
{
	//MjCardData PaiCard[4];
	memset(m_PaiCard, 0, sizeof(m_PaiCard));
	PyCard(card, nLen, m_PaiCard);

	int needHunNum = 0;
	int needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[0], 0, needMinHunNum);
	int wan_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[1], 0, needMinHunNum);
	int tiao_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[2], 0, needMinHunNum);
	int tong_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[3], 0, needMinHunNum);
	int feng_need = needMinHunNum;

	needHunNum = tiao_need + tong_need + feng_need;
	if (needHunNum <= nLaiZiNum)
	{
		int hasNum = nLaiZiNum - needHunNum;
		bool ishu = LaiZi_CanHu(m_PaiCard[0], hasNum);
		if (ishu)
		{
			return true;
		}
	}
	needHunNum = wan_need + tong_need + feng_need;
	if (needHunNum <= nLaiZiNum)
	{
		int hasNum = nLaiZiNum - needHunNum;
		bool ishu = LaiZi_CanHu(m_PaiCard[1], hasNum);
		if (ishu)
		{
			return true;
		}
	}
	needHunNum = wan_need + tiao_need + feng_need;
	if (needHunNum <= nLaiZiNum)
	{
		int hasNum = nLaiZiNum - needHunNum;
		bool ishu = LaiZi_CanHu(m_PaiCard[2], hasNum);
		if (ishu)
		{
			return true;
		}
	}
	needHunNum = wan_need + tiao_need + tong_need;
	if (needHunNum <= nLaiZiNum)
	{
		int hasNum = nLaiZiNum - needHunNum;
		bool ishu = LaiZi_CanHu(m_PaiCard[3], hasNum);
		if (ishu)
		{
			return true;
		}
	}
	return false;
}
void CGameLogic::PyCard(const _uint8 handcard[14], int nLen, MJ_hand_buffer PaiCard[4])
{
	for (int i = 0; i < nLen; i++)
	{
		if (CardType(handcard[i]) == MJ_TYPE_WAN)
		{			
			PaiCard[MJ_TYPE_WAN].Add(handcard[i]);
		}
		else if (CardType(handcard[i]) == MJ_TYPE_TIAO)
		{			
			PaiCard[MJ_TYPE_TIAO].Add(handcard[i]);
		}
		else if (CardType(handcard[i]) == MJ_TYPE_BING)
		{			
			PaiCard[MJ_TYPE_BING].Add(handcard[i]);
		}
		else if (CardType(handcard[i]) == MJ_TYPE_FENG || CardType(handcard[i]) == MJ_TYPE_JIAN)
		{			
			PaiCard[MJ_TYPE_FENG].Add(handcard[i]);
		}

	}

	for (int i = 0; i < 4; ++i)
	{
		PaiCard[i].SortAscend();
	}

}

void CGameLogic::get_need_hun_num(MJ_hand_buffer& stData, int nNeedNum, int &nNeedMinNum)
{
	if (nNeedMinNum == 0)
	{
		return;
	}
	if (nNeedNum >= nNeedMinNum)
	{
		return;
	}

	if (stData.Size() == 0)
	{
		nNeedMinNum = MIN(nNeedNum, nNeedMinNum);;
		return;
	}
	else if (stData.Size() == 1)
	{
		nNeedMinNum = MIN(nNeedNum + 2, nNeedMinNum);
		return;
	}
	else if (stData.Size() == 2)
	{
		_uint8 cbCard1 = stData[0];
		_uint8 cbCard2 = stData[1];
		if (CardType(cbCard2) == MJ_TYPE_JIAN || CardType(cbCard2) == MJ_TYPE_FENG)
		{
			if (cbCard1 == cbCard2)
			{
				nNeedMinNum = MIN(nNeedNum + 1, nNeedMinNum);
				return;
			}
			else
			{
				stData.Remove(cbCard1);
				get_need_hun_num(stData, nNeedNum + 2, nNeedMinNum);
				stData.Add(cbCard1);
				stData.SortAscend();
			}
		}
		else if ((cbCard2 - cbCard1) < 3)
		{
			nNeedMinNum = MIN(nNeedNum + 1, nNeedMinNum);
		}
		else
		{
			stData.Remove(cbCard1);
			get_need_hun_num(stData, nNeedNum + 2, nNeedMinNum);
			stData.Add(cbCard1);
			stData.SortAscend();
		}
		return;
	}

	_uint8 bCard1 = stData[0];
	_uint8 bCard2 = stData[1];
	_uint8 bCard3 = stData[2];
	if ((nNeedNum + 2) < nNeedMinNum)
	{
		stData.Remove(bCard1);
		get_need_hun_num(stData, nNeedNum + 2, nNeedMinNum);
		stData.Add(bCard1);
		stData.SortAscend();
		
	}
	if (nNeedNum + 1 < nNeedMinNum)
	{
		if (CardType(bCard1) == MJ_TYPE_JIAN || CardType(bCard1) == MJ_TYPE_FENG)
		{
			if (bCard1 == bCard2)
			{
				stData.Remove(bCard1);
				stData.Remove(bCard2);
				get_need_hun_num(stData, nNeedNum + 1, nNeedMinNum);
				stData.Add(bCard1);
				stData.Add(bCard2);
				stData.SortAscend();
			}

		}
		else
		{
			for (int i = 1; i < stData.Size(); i++)
			{
				if ((nNeedNum + 1) >= nNeedMinNum)
				{
					break;;
				}
				bCard2 = stData[i];
				if ((i + 1) != stData.Size())
				{
					bCard3 = stData[i + 1];
					if (bCard3 == bCard2)
					{
						continue;
					}

				}
				if ((bCard2 - bCard1) < 3)
				{
					stData.Remove(bCard1);
					stData.Remove(bCard2);
					get_need_hun_num(stData, nNeedNum + 1, nNeedMinNum);
					stData.Add(bCard1);
					stData.Add(bCard2);
					stData.SortAscend();
				}
				else
				{
					break;
				}
			}
		}
	}

	for (int i = 1; i < stData.Size(); i++)
	{
		if (nNeedNum >= nNeedMinNum)
		{
			break;
		}
		bCard2 = stData[i];
		if ((i + 2) < stData.Size())
		{
			if (stData[i + 2] == bCard2)
			{
				continue;
			}
		}
		for (int j = i + 1; j < stData.Size(); j++)
		{
			if (nNeedNum >= nNeedMinNum)
			{
				break;;
			}
			bCard3 = stData[j];

			if ((j + 1) < stData.Size())
			{
				if (bCard3 == stData[j + 1])
				{
					continue;
				}
			}
			if (checke_combine(bCard1, bCard2, bCard3))
			{			
				stData.Remove(bCard1);
				stData.Remove(bCard2);
				stData.Remove(bCard3);
				get_need_hun_num(stData, nNeedNum, nNeedMinNum);
				stData.Add(bCard1);
				stData.Add(bCard2);
				stData.Add(bCard3);
				stData.SortAscend();
			}
		}
	}
}


bool CGameLogic::LaiZi_CanHu(MJ_hand_buffer stData, int LaiZiNum, bool bAllShunZi)
{
	if (stData.Size() == 0)
	{
		if (LaiZiNum >= 2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	MJ_hand_buffer tempData;
	//memcpy(&tempData, &stData, sizeof(MjCardData));
	tempData = stData;
	for (int i = 0; i < tempData.Size(); i++)
	{
		if (i == tempData.Size() - 1)
		{
			if (LaiZiNum > 0)
			{
				LaiZiNum = LaiZiNum - 1;				
				stData.Remove(tempData[i]);
				int nNeedMinLaiZiNum = 8;
				if (bAllShunZi)
				{
					get_need_hun_num_by_shun(stData, 0, nNeedMinLaiZiNum);
				}
				else
				{
					get_need_hun_num(stData, 0, nNeedMinLaiZiNum);
				}

				if (nNeedMinLaiZiNum <= LaiZiNum)
				{
					return true;
				}
			}
		}
		else
		{
			if (((i + 2) == tempData.Size())
				|| tempData[i] != tempData[i + 2])
			{
				if (checke_combine_dui_zi(tempData[i], tempData[i + 1]))
				{					
					stData.Remove(tempData[i]);
					stData.Remove(tempData[i + 1]);
					int nNeedMinLaiZiNum = 8;
					if (bAllShunZi)
					{
						get_need_hun_num_by_shun(stData, 0, nNeedMinLaiZiNum);
					}
					else
					{
						get_need_hun_num(stData, 0, nNeedMinLaiZiNum);
					}
					if (nNeedMinLaiZiNum <= LaiZiNum)
					{
						return true;
					}					
					stData.Add(tempData[i]);
					stData.Add(tempData[i + 1]);
					stData.SortAscend();
				}
			}
			if (LaiZiNum > 0 && tempData[i] != tempData[i + 1])
			{
				LaiZiNum = LaiZiNum - 1;				
				stData.Remove(tempData[i]);
				int nNeedMinLaiZiNum = 8;
				if (bAllShunZi)
				{
					get_need_hun_num_by_shun(stData, 0, nNeedMinLaiZiNum);
				}
				else
				{
					get_need_hun_num(stData, 0, nNeedMinLaiZiNum);
				}
				if (nNeedMinLaiZiNum <= LaiZiNum)
				{
					return true;
				}
				LaiZiNum = LaiZiNum + 1;				
				stData.Add(tempData[i]);
				stData.SortAscend();
			}
		}
	}
	return false;
}
bool CGameLogic::checke_combine(_uint8 cbCard1, _uint8 cbCard2, _uint8 cbCard3)
{
	bool bComBine = false;
	if ((cbCard1 == cbCard2) && (cbCard2 == cbCard3))
	{
		bComBine = true;
	}
	if (((cbCard1 + 1) == cbCard2) && ((cbCard2 + 1) == cbCard3))
	{
		if (CardType(cbCard1) != MJ_TYPE_JIAN && CardType(cbCard1) != MJ_TYPE_FENG)
		{
			bComBine = true;
		}
	}

	return bComBine;
}
bool CGameLogic::checke_combine_shun(_uint8 cbCard1, _uint8 cbCard2, _uint8 cbCard3)
{
	bool bComBine = false;
	if (((cbCard1 + 1) == cbCard2) && ((cbCard2 + 1) == cbCard3))
	{
		if (CardType(cbCard1) < MJ_TYPE_FENG)
		{
			bComBine = true;
		}
	}

	return bComBine;
}

bool CGameLogic::checke_combine_dui_zi(_uint8 cbCard1, _uint8 cbCard2)
{
	if (cbCard1 == cbCard2)
	{
		return true;
	}
	return false;
}

void CGameLogic::get_need_hun_num_by_shun(MJ_hand_buffer stData, int nNeedNum, int &nNeedMinNum)
{
	if (nNeedMinNum == 0)
	{
		return;
	}
	if (nNeedNum >= nNeedMinNum)
	{
		return;
	}

	if (stData.Size() == 0)
	{
		nNeedMinNum = MIN(nNeedNum, nNeedMinNum);;
		return;
	}
	else if (stData.Size() == 1)
	{
		nNeedMinNum = MIN(nNeedNum + 2, nNeedMinNum);
		return;
	}
	else if (stData.Size() == 2)
	{
		_uint8 cbCard1 = stData[0];
		_uint8 cbCard2 = stData[1];
		if (CardType(cbCard2) == MJ_TYPE_JIAN || CardType(cbCard2) == MJ_TYPE_FENG)
		{
			if (cbCard1 == cbCard2)
			{
				nNeedMinNum = MIN(nNeedNum + 1, nNeedMinNum);
				return;
			}

		}
		else if ((cbCard2 - cbCard1) < 3)
		{
			nNeedMinNum = MIN(nNeedNum + 1, nNeedMinNum);
		}
		else
		{
			nNeedMinNum = 8;
		}
		return;
	}

	_uint8 bCard1 = stData[0];
	_uint8 bCard2 = stData[1];
	_uint8 bCard3 = stData[2];
	if ((nNeedNum + 2) < nNeedMinNum)
	{		
		stData.Remove(bCard1);
		get_need_hun_num(stData, nNeedNum + 2, nNeedMinNum);		
		stData.Add(bCard1);
		stData.SortAscend();
	}
	if (nNeedNum + 1 < nNeedMinNum)
	{
		if (CardType(bCard1) == MJ_TYPE_JIAN || CardType(bCard1) == MJ_TYPE_FENG)
		{
			if (bCard1 == bCard2)
			{				
				stData.Remove(bCard1);
				stData.Remove(bCard2);
				get_need_hun_num(stData, nNeedNum + 1, nNeedMinNum);				
				stData.Add(bCard1);
				stData.Add(bCard2);
				stData.SortAscend();
			}
		}
		else
		{
			for (int i = 1; i < stData.Size(); i++)
			{
				if ((nNeedNum + 1) >= nNeedMinNum)
				{
					break;;
				}
				bCard2 = stData[i];
				if ((i + 1) != stData.Size())
				{
					bCard3 = stData[i + 1];
					if (bCard3 == bCard2)
					{
						continue;
					}
				}
				if ((bCard2 - bCard1) < 3)
				{					
					stData.Remove(bCard1);
					stData.Remove(bCard2);
					get_need_hun_num(stData, nNeedNum + 1, nNeedMinNum);					
					stData.Add(bCard1);
					stData.Add(bCard2);
					stData.SortAscend();
				}
				else
				{
					break;
				}
			}
		}
	}

	for (int i = 1; i < stData.Size(); i++)
	{
		if (nNeedNum >= nNeedMinNum)
		{
			break;
		}
		bCard2 = stData[i];
		if ((i + 2) < stData.Size())
		{
			if (stData[i + 2] == bCard2)
			{
				continue;
			}
		}
		for (int j = i + 1; j < stData.Size(); j++)
		{
			if (nNeedNum >= nNeedMinNum)
			{
				break;;
			}
			bCard3 = stData[j];

			if ((j + 1) < stData.Size())
			{
				if (bCard3 == stData[j + 1])
				{
					continue;
				}
			}
			if (checke_combine_shun(bCard1, bCard2, bCard3))
			{				
				stData.Remove(bCard1);
				stData.Remove(bCard2);
				stData.Remove(bCard3);
				get_need_hun_num(stData, nNeedNum, nNeedMinNum);				
				stData.Add(bCard1);
				stData.Add(bCard2);
				stData.Add(bCard3);
				stData.SortAscend();
			}
		}
	}
}

bool CGameLogic::L_Is13Yao(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum)
{
	//static _uint8 YaoPai[13] = {0x01, 0x09, 0x11, 0x19, 0x21, 0x29,0x31,0x32,0x33,0x34,0x41,0x42,0x43};
	HandCards thc;
	thc = *p;
	int OneNum = 0;
	int TwoNum = 0;
	//_uint8 CardDataNum[5][10] = {0};
	//±ØÐëÃÅÇå
	if ((thc.ChangeableCardsLen + nHunNum) != 14)
	{
		return false;
	}
	for (int i = 1; i < 5; i++)
	{
		if (CardDataNum[3][i] == 1)
		{
			OneNum++;
		}
		if (CardDataNum[3][i] == 2)
		{
			TwoNum++;
		}
	}

	for (int i = 1; i < 4; i++)
	{
		if (CardDataNum[4][i] == 1)
		{
			OneNum++;
		}
		if (CardDataNum[4][i] == 2)
		{
			TwoNum++;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (CardDataNum[i][1] == 1)
		{
			OneNum++;
		}
		if (CardDataNum[i][1] == 2)
		{
			TwoNum++;
		}
		if (CardDataNum[i][9] == 1)
		{
			OneNum++;
		}
		if (CardDataNum[i][9] == 2)
		{
			TwoNum++;
		}
	}
	if (OneNum == (14 - nHunNum - TwoNum * 2))
	{
		if (TwoNum > 1)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool CGameLogic::L_IsQiDui(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum)
{
	if (p->FixedCardsLen != 0)
	{
		return false;
	}
	int nNeedHun = 0;
	_uint8 last = INVALID_MJ_CARD;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == last) continue;
		_uint8 m = CardType(p->ChangeableCards[i]);
		_uint8 n = CardNum(p->ChangeableCards[i]);
		if (CardDataNum[m][n] == 1)
		{
			nNeedHun++;
		}
		if (CardDataNum[m][n] == 3)
		{
			nNeedHun++;
		}
		last = p->ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum)
	{
		return true;
	}
	return false;
}

int CGameLogic::L_IsQiDui_02(const HandCards* p, _uint8 c, const _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum)
{	
	if (p->FixedCardsLen != 0)
	{
		return -1;
	}
	int nNeedHun = 0;
	_uint8 last = INVALID_MJ_CARD;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == last) continue;
		_uint8 m = CardType(p->ChangeableCards[i]);
		_uint8 n = CardNum(p->ChangeableCards[i]);
		if (CardDataNum[m][n] == 1)
		{
			nNeedHun++;
		}
		if (CardDataNum[m][n] == 3)
		{
			nNeedHun++;
		}
		last = p->ChangeableCards[i];
	}
	return nNeedHun;
}

bool CGameLogic::L_IsQiDui_03(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum, int& iLongQiDui)
{
	if (p->FixedCardsLen != 0)
	{
		return false;
	}
	int nNeedHun = 0;
	_uint8 last = INVALID_MJ_CARD;
	iLongQiDui = 0;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == last) continue;
		_uint8 m = CardType(p->ChangeableCards[i]);
		_uint8 n = CardNum(p->ChangeableCards[i]);
		if (CardDataNum[m][n] == 1)
		{
			nNeedHun++;
		}
		else if (CardDataNum[m][n] == 3)
		{
			nNeedHun++;
			iLongQiDui++;
		}
		else if (CardDataNum[m][n] == 4)
		{
			iLongQiDui++;
		}
		last = p->ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum)
	{
		return true;
	}
	return false;
}

bool CGameLogic::L_IsQiDui_04(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum, int& iLongQiDui, std::vector<_uint8> &L_Cards)
{
	if (p->FixedCardsLen != 0)
	{
		return false;
	}

	iLongQiDui = 0;
	L_Cards.clear();

	int nNeedHun = 0;
	_uint8 last = INVALID_MJ_CARD;
	iLongQiDui = 0;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == last) continue;
		_uint8 m = CardType(p->ChangeableCards[i]);
		_uint8 n = CardNum(p->ChangeableCards[i]);
		if (CardDataNum[m][n] == 1)
		{
			nNeedHun++;
			L_Cards.push_back(p->ChangeableCards[i]);
		}
		else if (CardDataNum[m][n] == 3)
		{
			nNeedHun++;
			L_Cards.push_back(p->ChangeableCards[i]);
		}
		else if (CardDataNum[m][n] == 4)
		{
			iLongQiDui++;
		}
		last = p->ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum)
	{
		return true;
	}
	return false;
}

bool CGameLogic::L_IsQiDui_05(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum, int& iLongQiDui)
{
	if (p->FixedCardsLen != 0)
	{
		return false;
	}
	int nNeedHun = 0;
	_uint8 last = INVALID_MJ_CARD;
	iLongQiDui = 0;
	for (int i = 0; i < p->ChangeableCardsLen; i++)
	{
		if (p->ChangeableCards[i] == last) continue;
		_uint8 m = CardType(p->ChangeableCards[i]);
		_uint8 n = CardNum(p->ChangeableCards[i]);
		if (CardDataNum[m][n] == 1)
		{
			nNeedHun++;
		}
		else if (CardDataNum[m][n] == 3)
		{
			nNeedHun++;
			iLongQiDui++;
		}
		else if (CardDataNum[m][n] == 4)
		{
			iLongQiDui++;
		}
		last = p->ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum)
	{
		_uint8 last= INVALID_MJ_CARD;
		int remainHun=nHunNum-nNeedHun;
		for(int i=0;i<p->ChangeableCardsLen;i++)
		{
			if(p->ChangeableCards[i]==last) continue;
			_uint8 m=CardType(p->ChangeableCards[i]);
			_uint8 n=CardNum(p->ChangeableCards[i]);
			if(CardDataNum[m][n]==2)
			{
				if(remainHun/2 != 0)
				{
					iLongQiDui++;
					remainHun-=2;
				}
			}
			last = p->ChangeableCards[i];
		}
		iLongQiDui += remainHun /4;
		return true;
	}
	return false;
}

bool CGameLogic::L_IsPengPengHu(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum)
{
	int nNeedHun = 0;
	const HandCards& pHc = *p;
	for (int i = 0; i < pHc.FixedCardsLen; i++)
	{
		if (pHc.FixedCards[i].state & TYPE_CHI)			
		{
			return false;
		}
	}
	_uint8 last = INVALID_MJ_CARD;
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] == last)continue;
		_uint8 m = CardType(pHc.ChangeableCards[i]);
		_uint8 n = CardNum(pHc.ChangeableCards[i]);	
		if ( (CardDataNum[m][n] == 1 ) || (CardDataNum[m][n] == 4) )
		{
			nNeedHun += 2;
		}
		if (CardDataNum[m][n] == 2)
		{
			nNeedHun += 1;
		}
		last = pHc.ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum + 1)
	{
		return true;
	}
	return false;
}

//如果进牌区有仰牌，排除
bool CGameLogic::L_IsPengPengHu_01(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum)
{
	int nNeedHun = 0;
	const HandCards& pHc = *p;
	for (int i = 0; i < pHc.FixedCardsLen; i++)
	{
		if ( (pHc.FixedCards[i].state & TYPE_CHI) && (CardType(pHc.FixedCards[i].CardData) <= MJ_TYPE_TONG))
		{
			return false;
		}
	}

	_uint8 last = INVALID_MJ_CARD;
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] == last)continue;
		_uint8 m = CardType(pHc.ChangeableCards[i]);
		_uint8 n = CardNum(pHc.ChangeableCards[i]);	
		if ( (CardDataNum[m][n] == 1 ) || (CardDataNum[m][n] == 4) )
		{
			nNeedHun += 2;
		}
		if (CardDataNum[m][n] == 2)
		{
			nNeedHun += 1;
		}
		last = pHc.ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum + 1)
	{
		return true;
	}
	return false;
}

int CGameLogic::L_IsPengPengHu_02(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum)
{
	int nNeedHun = 0;
	const HandCards& pHc = *p;
	for (int i = 0; i < pHc.FixedCardsLen; i++)
	{
		if (pHc.FixedCards[i].state & TYPE_CHI)
		{
			return 1000;
		}
	}

	_uint8 last = INVALID_MJ_CARD;
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] == last)continue;
		_uint8 m = CardType(pHc.ChangeableCards[i]);
		_uint8 n = CardNum(pHc.ChangeableCards[i]);
		if ((CardDataNum[m][n] == 1) || (CardDataNum[m][n] == 4))
		{
			nNeedHun += 2;
		}
		if (CardDataNum[m][n] == 2)
		{
			nNeedHun += 1;
		}	
		last = pHc.ChangeableCards[i];
	}
	return nNeedHun;	
}

bool CGameLogic::L_IsPengPengHu_04(const HandCards* p, _uint8 c, _uint8 CardDataNum[MJ_TYPE_COMMON][10], int nHunNum, std::vector<_uint8>& L_Cards)
{
	int nNeedHun = 0;
	const HandCards& pHc = *p;
	for (int i = 0; i < pHc.FixedCardsLen; i++)
	{
		if (pHc.FixedCards[i].state & TYPE_CHI)			
		{
			return false;
		}
	}
	_uint8 last = INVALID_MJ_CARD;
	for (int i = 0; i < pHc.ChangeableCardsLen; i++)
	{
		if (pHc.ChangeableCards[i] == last)continue;
		_uint8 m = CardType(pHc.ChangeableCards[i]);
		_uint8 n = CardNum(pHc.ChangeableCards[i]);	
		if ( (CardDataNum[m][n] == 1 ) || (CardDataNum[m][n] == 4) )
		{
			nNeedHun += 2;
			L_Cards.push_back(pHc.ChangeableCards[i]);
			L_Cards.push_back(pHc.ChangeableCards[i]);
		}
		if (CardDataNum[m][n] == 2)
		{
			nNeedHun += 1;
			L_Cards.push_back(pHc.ChangeableCards[i]);
		}
		last = pHc.ChangeableCards[i];
	}
	if (nNeedHun <= nHunNum + 1)
	{
		return true;
	}
	return false;
}

void CGameLogic::GetCardCount(const HandCards* pHc, _uint8 CardDataNum[MJ_TYPE_COMMON][10])
{
	memset(CardDataNum, 0, sizeof(_uint8) * MJ_TYPE_COMMON * 10);
	for (int i = 0; i < pHc->ChangeableCardsLen; i++)
	{
		int nCardType = CardType(pHc->ChangeableCards[i]);
		if (nCardType < MJ_TYPE_COMMON)
		{
			int nCardNum = CardNum(pHc->ChangeableCards[i]);
			if (nCardNum > 0 && nCardNum < 10)
			{
				CardDataNum[nCardType][nCardNum]++;
			}
		}
	}
}
void CGameLogic::GetCardCountExt(const HandCards* pHc, _uint8 CardDataNum[MJ_TYPE_COMMON][10])
{	
	for (int i = 0; i < pHc->FixedCardsLen; i++)
	{
		if (pHc->FixedCards[i].state & TYPE_CHI)
		{
			CardDataNum[CardType(pHc->FixedCards[i].c[0])][CardNum(pHc->FixedCards[i].c[0])]++;
			CardDataNum[CardType(pHc->FixedCards[i].c[1])][CardNum(pHc->FixedCards[i].c[1])]++;
			CardDataNum[CardType(pHc->FixedCards[i].c[2])][CardNum(pHc->FixedCards[i].c[2])]++;
		}
		else if (pHc->FixedCards[i].state & TYPE_PENG)
		{
			CardDataNum[CardType(pHc->FixedCards[i].c[0])][CardNum(pHc->FixedCards[i].c[0])] += 3;
		}
		else if (pHc->FixedCards[i].state & TYPE_GANG)
		{
			CardDataNum[CardType(pHc->FixedCards[i].c[0])][CardNum(pHc->FixedCards[i].c[0])] += 4;
		}
	}
}
int CGameLogic::GetCardCountX(const HandCards* pHc, _uint8 bCard)
{
	int iCounts = 0;
	for (int i = 0; i < pHc->ChangeableCardsLen; i++)
	{
		
		if (pHc->ChangeableCards[i] == bCard)
		{
			iCounts++;
		}
	}
	for (int i = 0; i < pHc->FixedCardsLen; i++)
	{
		if (pHc->FixedCards[i].state & TYPE_CHI)
		{
			if (pHc->FixedCards[i].c[0] == bCard ||
				pHc->FixedCards[i].c[1] == bCard ||
				pHc->FixedCards[i].c[2] == bCard)
			{
				iCounts++;
			}
		}
		else if (pHc->FixedCards[i].state & TYPE_PENG)
		{
			if (pHc->FixedCards[i].CardData == bCard)
			{
				iCounts += 3;
			}
		}
		else if (pHc->FixedCards[i].state & TYPE_GANG)
		{
			if (pHc->FixedCards[i].CardData == bCard)
			{
				iCounts += 4;
			}
		}
	}
	return iCounts;
}

int CGameLogic::GetFixedCardCount(const HandCards* pHc, _uint8 bCard)
{
	int iCounts = 0;
	for (int i = 0; i < pHc->FixedCardsLen; i++)
	{
		if (pHc->FixedCards[i].state & TYPE_CHI)
		{
			if (pHc->FixedCards[i].c[0] == bCard ||
				pHc->FixedCards[i].c[1] == bCard ||
				pHc->FixedCards[i].c[2] == bCard)
			{
				iCounts++;
			}
		}
		else if (pHc->FixedCards[i].state & TYPE_PENG)
		{
			if (pHc->FixedCards[i].CardData == bCard)
			{
				iCounts += 3;
			}
		}
		else if (pHc->FixedCards[i].state & TYPE_GANG)
		{
			if (pHc->FixedCards[i].CardData == bCard)
			{
				iCounts += 4;
			}
		}
	}
	return iCounts;
}
bool CGameLogic::FormatCards_Laizi_3_Group(const _uint8 card[HAND_CARD_SIZE_MAX], int nLen, int nLaiZiNum)
{
	//MjCardData PaiCard[4];
	memset(m_PaiCard, 0, sizeof(m_PaiCard));
	PyCard(card, nLen, m_PaiCard);

	int needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[0], 0, needMinHunNum);
	int wan_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[1], 0, needMinHunNum);
	int tiao_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[2], 0, needMinHunNum);
	int tong_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[3], 0, needMinHunNum);
	int feng_need = needMinHunNum;

	if ((wan_need + tiao_need + tong_need + feng_need) <= nLaiZiNum)
	{
		return true;
	}
	return false;
}

bool CGameLogic::GetCardTypeCount(const HandCards* pHc, _uint8 CardTypeCount[MJ_TYPE_COMMON])
{
	memset(CardTypeCount, 0, sizeof(_uint8) * MJ_TYPE_COMMON);
	for (int i = 0; i < pHc->ChangeableCardsLen; i++)
	{
		_uint8 type = CardType(pHc->ChangeableCards[i]);
		if (type >= MJ_TYPE_COMMON)
			return false;
		CardTypeCount[type]++;
	}
	for (int i = 0; i < pHc->FixedCardsLen; i++)
	{
		if (pHc->FixedCards[i].state & TYPE_CHI)
		{			
			_uint8 type = CardType(pHc->FixedCards[i].CardData);
			CardTypeCount[type] += 3;
			
		}
		else if (pHc->FixedCards[i].state & TYPE_PENG)
		{
			_uint8 type = CardType(pHc->FixedCards[i].CardData);
			CardTypeCount[type] += 3;				
		}
		else if (pHc->FixedCards[i].state & TYPE_GANG)
		{
			_uint8 type = CardType(pHc->FixedCards[i].CardData);
			CardTypeCount[type] += 4;				
		}
	}
	return true;
}

bool CGameLogic::Is_BianZhang(const HandCards* pHc, _uint8 bHuCard, const MJ_win_pattern& oPattern)
{
	if (!(CardNum(bHuCard) == 3 || CardNum(bHuCard) == 7))
	{
		return false;
	}
	if (CardType(bHuCard) > MJ_TYPE_TONG)
	{
		return false;
	}
	bool bBianZhang = false;
	for (int i = 0; i < HAND_CARD_KAN_MAX; i++)
	{
		if (oPattern.kan[i][0] == 0)
		{
			break;
		}
		int iCount = 0;
		for (int j = 0; j < 3; j++)
		{
			if (oPattern.kan[i][j] == bHuCard)
				iCount++;
		}
		if (iCount == 0) continue;
		if (iCount > 1) continue;
		if (iCount == 1)
		{
			if (CardNum(bHuCard) == 3 )
			{				
				if (CardNum(oPattern.kan[i][0]) == 3  &&
					CardNum(oPattern.kan[i][1]) == 4 &&
					CardNum(oPattern.kan[i][2]) == 5)
					return false;					
				if (CardNum(oPattern.kan[i][0]) == 1 &&
					CardNum(oPattern.kan[i][1]) == 2 &&
					CardNum(oPattern.kan[i][2]) == 3)
					bBianZhang = true;
			}
			else if (CardNum(bHuCard) == 7)
			{
				if (CardNum(oPattern.kan[i][0]) == 5 &&
					CardNum(oPattern.kan[i][1]) == 6 &&
					CardNum(oPattern.kan[i][2]) == 7)
					return false;
				if (CardNum(oPattern.kan[i][0]) == 7 &&
					CardNum(oPattern.kan[i][1]) == 8 &&
					CardNum(oPattern.kan[i][2]) == 9)
					bBianZhang = true;
			}			
		}
	}	
	return bBianZhang;
}
bool CGameLogic::Is_KaZhang(const HandCards* pHc, _uint8 bHuCard, const MJ_win_pattern& oPattern)
{
	if ((CardNum(bHuCard) == 1 || CardNum(bHuCard) == 9))
	{
		return false;
	}
	if (CardType(bHuCard) > MJ_TYPE_TONG)
	{
		return false;
	}
	bool bKanZhang = false;
	for (int i = 0; i < HAND_CARD_KAN_MAX; i++)
	{
		if (oPattern.kan[i][0] == 0)
		{
			break;
		}
		int iCount = 0;
		for (int j = 0; j < 3; j++)
		{
			if (oPattern.kan[i][j] == bHuCard)
				iCount++;
		}
		if (iCount == 0) continue;
		if (iCount > 1) continue;
		if (iCount == 1)
		{
			if (oPattern.kan[i][0] == bHuCard &&
				oPattern.kan[i][1] == (bHuCard + 1) &&
				oPattern.kan[i][2] == (bHuCard + 2))
				return false;
			if (oPattern.kan[i][0] == bHuCard &&
				oPattern.kan[i][1] == (bHuCard - 1) &&
				oPattern.kan[i][2] == (bHuCard - 2))
				return false;
			if (oPattern.kan[i][0] == (bHuCard - 1) &&
				oPattern.kan[i][1] == (bHuCard) &&
				oPattern.kan[i][2] == (bHuCard + 1))
				bKanZhang = true;
		}
	}
	return bKanZhang;
}
bool CGameLogic::Is_DaKuanZhang(const HandCards* pHc, _uint8 bHuCard, const MJ_win_pattern& oPattern)
{
	if (CardType(bHuCard) > MJ_TYPE_TONG)
	{
		return false;
	}
	static _uint8 dakuanzhang[10][2] = {
		{ 0, 0 },
		{ 4, 7 }, { 5, 8 }, { 6, 9 }, { 1, 7 }, { 2, 8 }, { 3, 9 }, { 1, 4 }, { 2, 5 }, { 3, 6 },
	};
	HandCards tmp = *pHc;
	ChuPai(&tmp, bHuCard);
	_uint8 num = CardNum(bHuCard);
	_uint8 zhang_0 = CardType(bHuCard) << 4 | dakuanzhang[num][0];
	AddCard(&tmp, zhang_0);

	MJ_win_pattern pattern;
	if (!FormatCards(&tmp, pattern))
		return false;

	_uint8 zhang_1 = CardType(bHuCard) << 4 | dakuanzhang[num][1];
	ChuPai(&tmp, zhang_0);
	AddCard(&tmp, zhang_1);
	if (!FormatCards(&tmp, pattern))
		return false;

	return true;
}

bool CGameLogic::IsJianZiHu(const _uint8 card[14], int nLen, int nLaiZiNum)
{
	memset(m_PaiCard, 0, sizeof(m_PaiCard));
	PyCard(card, nLen, m_PaiCard);

	int needHunNum = 0;
	int needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[0], 0, needMinHunNum);
	int wan_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[1], 0, needMinHunNum);
	int tiao_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[2], 0, needMinHunNum);
	int tong_need = needMinHunNum;
	needMinHunNum = 8;
	get_need_hun_num(m_PaiCard[3], 0, needMinHunNum);
	int feng_need = needMinHunNum;

	needHunNum = wan_need + tiao_need + tong_need + feng_need;
	if (needHunNum <= nLaiZiNum - 1)
	{
		return true;
	}
	return false;
}

bool CGameLogic::Is13BuKao(const HandCards *pHc)
{
	if (pHc->ChangeableCardsLen != 14) return false;

	std::vector<_uint8> vec(&pHc->ChangeableCards[0], &pHc->ChangeableCards[pHc->ChangeableCardsLen]);

	bool b147 = false;
	bool b258 = false;
	bool b369 = false;
	_uint8 t147 = INVALID_MJ_CARD;
	_uint8 t258 = INVALID_MJ_CARD; 
	_uint8 t369 = INVALID_MJ_CARD;  
	std::vector<_uint8> vecdel;

	for (size_t i = 0; i < vec.size(); ++i)
	{
		_uint8 Card1 = vec[i];
		if (!b147 && CardNum(Card1) == 0x01)
		{
			_uint8 Card2 = CardValue(CardType(Card1), 0x04);
			_uint8 Card3 = CardValue(CardType(Card1), 0x07);

			if (std::find(vec.begin(), vec.end(), Card2) == vec.end()) return false;
			if (std::find(vec.begin(), vec.end(), Card3) == vec.end()) return false;

			vecdel.push_back(Card1);
			vecdel.push_back(Card2);
			vecdel.push_back(Card3);

			b147 = true;

			t147 = CardType(Card1);
		}

		if (!b258 && CardNum(Card1) == 0x02)
		{
			_uint8 Card2 = CardValue(CardType(Card1), 0x05);
			_uint8 Card3 = CardValue(CardType(Card1), 0x08);

			if (std::find(vec.begin(), vec.end(), Card2) == vec.end()) return false;
			if (std::find(vec.begin(), vec.end(), Card3) == vec.end()) return false;

			vecdel.push_back(Card1);
			vecdel.push_back(Card2);
			vecdel.push_back(Card3);

			b258 = true;

			t258 = CardType(Card1);
		}

		if (!b369 && CardNum(Card1) == 0x03)
		{
			_uint8 Card2 = CardValue(CardType(Card1), 0x06);
			_uint8 Card3 = CardValue(CardType(Card1), 0x09);

			if (std::find(vec.begin(), vec.end(), Card2) == vec.end()) return false;
			if (std::find(vec.begin(), vec.end(), Card3) == vec.end()) return false;

			vecdel.push_back(Card1);
			vecdel.push_back(Card2);
			vecdel.push_back(Card3);

			b369 = true;

			t369 = CardType(Card1);
		}
	}

	if (!b147 || !b258 || !b369) return false;

	if (t147 == t258 || t147 == t369 || t258 == t369) return false;

	for (size_t i = 0; i < vecdel.size(); ++i)
	{
		EraseCard(vec, vecdel[i]);
	}

	_uint8 CardDataNum[MJ_TYPE_COMMON][5];
	memset(CardDataNum, 0, sizeof(CardDataNum));

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (CardType(vec[i]) <= MJ_TYPE_TONG) return false;

		if (++CardDataNum[CardType(vec[i])][CardNum(vec[i])] >= 2) return false;
	}

	return true;
}