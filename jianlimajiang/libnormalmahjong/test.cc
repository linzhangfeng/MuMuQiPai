#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "game_logic.h"
#include <unistd.h>
#include <sys/time.h>

#include <iostream>
#include <vector>
#include <algorithm> 

static _uint8 x[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, };

//听牌的胡牌信息
struct tagHuInfos
{
	_uint8          bCard;              //胡的牌
	_uint8          bLeftCount;         //该牌剩余数
	int         	bFanShu;            //胡牌的番数
};

//听牌信息
struct tagListenInfos
{
	_uint8          bOutCard;           //需要打出的牌
	tagHuInfos      HuInfo[18];         //打出牌后可以胡牌的信息
	_uint8			bHuLen;
	tagListenInfos()
	{
		bOutCard = 0;
		memset(HuInfo, 0, sizeof(HuInfo));
		bHuLen = 0;
	}
};

int test02()
{
	HandCards hc;
	CGameLogic cgl;
	cgl.AddCard(&hc, 0x1);
	cgl.AddCard(&hc, 0x1);
	cgl.AddCard(&hc, 0x2);
	cgl.AddCard(&hc, 0x2);
	cgl.AddCard(&hc, 0x2);
	cgl.AddCard(&hc, 0x5);
	cgl.AddCard(&hc, 0x5);
	_uint8 CardDataNum[MJ_TYPE_COMMON][10] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
	cgl.GetCardCount(&hc, CardDataNum);
	if (cgl.L_IsQiDui(&hc, INVALID_MJ_CARD, CardDataNum, 1))
	{
		printf("is qidui \n");
	}

	return 0;
}
int test03()
{
	HandCards phc;
	CGameLogic cgl;
	cgl.AddCard(&phc, 0x05);
	cgl.AddCard(&phc, 0x06);
	cgl.AddCard(&phc, 0x07);

	cgl.AddCard(&phc, 0x31);
	cgl.AddCard(&phc, 0x31);
	cgl.AddCard(&phc, 0x31);
	
	cgl.AddCard(&phc, 0x01);

	if (cgl.FormatCards_Laizi_3_Group(phc.ChangeableCards, phc.ChangeableCardsLen, 1))
	{
		return -1;
	}
	return 0;
}


void test_0()
{
	HandCards hc;
	CGameLogic cgl;
	hc.ChangeableCards[0] = 0x1;
	hc.ChangeableCards[1] = 0x1;
	hc.ChangeableCards[2] = 0x1;
	hc.ChangeableCards[3] = 0x2;
	hc.ChangeableCards[4] = 0x3;
	hc.ChangeableCards[5] = 0x4;
	hc.ChangeableCards[6] = 0x5;
	hc.ChangeableCards[7] = 0x6;
	hc.ChangeableCards[8] = 0x7;

	hc.ChangeableCards[9] = 0x8;
	hc.ChangeableCards[10] = 0x9;
	hc.ChangeableCards[11] = 0x9;
	hc.ChangeableCards[12] = 0x9;
	hc.ChangeableCardsLen = 13;
	hc.FixedCardsLen = 0;

	timeval begin, end;
	bool ok;
	gettimeofday(&begin, 0);
	MJ_win_pattern pat;
	for (int i = 0; i < 10000; i++)
	{
		for (_uint8 c = 0; c < sizeof(x); c++)
		{
			cgl.AddCard(&hc, x[c]);
			ok = cgl.FormatCards(&hc, pat);
			cgl.ChuPai(&hc, x[c]);
		}
	}
	gettimeofday(&end, 0);
	long t = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
	printf("%s\ttimer:%ld.%ld\n", ok ? "succ" : "failed", t / 1000000, t % 1000000);
}

void test_1(int lai_sum)
{
	HandCards hc;
	CGameLogic cgl;
	hc.ChangeableCards[0] = 0x1;
	hc.ChangeableCards[1] = 0x1;
	hc.ChangeableCards[2] = 0x1;
	hc.ChangeableCards[3] = 0x2;
	hc.ChangeableCards[4] = 0x3;
	hc.ChangeableCards[5] = 0x4;
	hc.ChangeableCards[6] = 0x5;
	hc.ChangeableCards[7] = 0x6;
	hc.ChangeableCards[8] = 0x7;

	hc.ChangeableCards[9] = 0x8;
	hc.ChangeableCards[10] = 0x9;
	hc.ChangeableCards[11] = 0x9;
	hc.ChangeableCards[12] = 0x9;
	hc.ChangeableCardsLen = 13;
	hc.FixedCardsLen = 0;
	
	for( int i= 0 ; i < lai_sum ; ++i  )
	{
		hc.ChangeableCardsLen--;
		hc.ChangeableCards[12 - 1 - i] = INVALID_MJ_CARD;
	}


	timeval begin, end;
	bool ok;
	gettimeofday(&begin, 0);
	for (int i = 0;	i < 10000 ; i++)
	{
		for (_uint8 c = 0; c < sizeof(x); c++)
		{
			cgl.AddCard(&hc, x[c]);
			MJ_win_pattern pat;
			ok = cgl.FormatCards_LaiZi(&hc, pat, lai_sum);
			cgl.ChuPai(&hc, x[c]);
		}
	}
	gettimeofday(&end, 0);
	long t = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
	printf("%s\ttimer:%ld.%ld\n", ok ? "succ" : "failed", t / 1000000, t % 1000000);

}

void SetIntersection(std::vector<int> v1, std::vector<int> v2, std::vector<int> &iv)
{
	iv.clear();
	iv.assign(v1.size()+v2.size(), 0);
	std::vector<int>::iterator it;

	it = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), iv.begin());
	iv.resize(it-iv.begin());
}

void SetUnion(std::vector<int> v1, std::vector<int> v2, std::vector<int> &uv)
{
	uv.clear();
	uv.assign(v1.size()+v2.size(), 0);
	std::vector<int>::iterator it;

	it = set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), uv.begin());
	uv.resize(it-uv.begin());
}

bool ExsitArray(_uint8 a, tagHuInfos l[18], int len)
{
	for (int j = 0; j < len; ++j)
	{
		if (a == l[j].bCard)
		{
			return true;
		}
	}

	return false;
}


bool ExsitVectorMember(std::vector<int> v, _uint8 member)
{
	std::vector<int>::iterator iter = v.begin();
	for( ; iter != v.end(); )
	{
		if ((*iter) == member)
		{
			return true;
		}
		else iter++;
	}

	return false;
}


void EraseVectorMember(std::vector<int> &v, _uint8 member, int num)
{
	for (int i = 0; i < num; ++i)
	{
		std::vector<int>::iterator iter = v.begin();
		for( ; iter != v.end(); )
		{
			if ((*iter) == member)
			{
				iter = v.erase(iter);
				break;
			}
			else iter++;
		}
	}
}

void EstimateCardWeave(HandCards tmpHc, _uint8 cbCard, std::vector<int> &v)
{
	CGameLogic m_GameLogic;

	if (m_GameLogic.Is7Dui(&tmpHc))
	{
		v.push_back(cbCard);
	}
	else
	{
		MJ_win_pattern pattern;
		if (m_GameLogic.FormatCards(&tmpHc, pattern.kan, pattern.jiang))
		{
			if (pattern.jiang == cbCard)
			{
				v.push_back(cbCard);
				v.push_back(cbCard);
			}

			for (int k = 0; k < 4; k++)
			{
				for (int n = 0; n < 4; n++)
				{
					if (pattern.kan[k][n] == cbCard)
					{
						for (int m = 0; m < 4; m++)
						{
							if (pattern.kan[k][m] == INVALID_MJ_CARD) break;
							v.push_back(pattern.kan[k][m]);
						}
						break;
					}
				}
			}

			EraseVectorMember(v, cbCard, 1);
		}
	}
}

void EstimateCardWeaveOne(HandCards tmpHc, _uint8 cbCard, std::vector<int> &v)
{
	CGameLogic m_GameLogic;

	if (m_GameLogic.Is7Dui(&tmpHc))
	{
		v.push_back(cbCard);
		return;
	}

	MJ_win_pattern pattern;
	if (m_GameLogic.FormatCards(&tmpHc, pattern.kan, pattern.jiang))
	{
		if (pattern.jiang == cbCard)
		{
			v.push_back(cbCard);
			return;
		}

		for (int k = 0; k < 4; k++)
		{
			for (int n = 0; n < 4; n++)
			{
				if (pattern.kan[k][n] == cbCard)
				{
					for (int m = 0; m < 4; m++)
					{
						if (pattern.kan[k][m] == INVALID_MJ_CARD) break;
						v.push_back(pattern.kan[k][m]);
					}

					EraseVectorMember(v, cbCard, 1);

					return;
				}
			}
		}
	}	
}

void EstimateShowCards(_uint8 ChairID, std::vector<int> &sv)
{
	sv.clear();
	//HandCards tmpHc = m_HandCard[ChairID];
	//
	//
	HandCards tmpHc;	
	CGameLogic m_GameLogic;
	tmpHc.ChangeableCards[0] = 0x02;
	tmpHc.ChangeableCards[1] = 0x02;	
	tmpHc.ChangeableCards[2] = 0x02;
	tmpHc.ChangeableCards[3] = 0x04;
	tmpHc.ChangeableCards[4] = 0x05;
	tmpHc.ChangeableCards[5] = 0x06;
	tmpHc.ChangeableCards[6] = 0x07;
	tmpHc.ChangeableCardsLen = 7;

	tagListenInfos stListenInfo[14];
	//_uint8 bListenLen = 0;

	stListenInfo[0].HuInfo[0].bCard = 0x03;
	stListenInfo[0].HuInfo[1].bCard = 0x04;
	stListenInfo[0].HuInfo[2].bCard = 0x07;
	stListenInfo[0].bHuLen = 3;

	// if (!IsListening(ChairID, stListenInfo, bListenLen))
	// {
	// 	return;
	// }

	std::vector<int> v1;
	std::vector<int> v2;
	std::vector<int> v3;
	_uint8 cbCard;




	printf("-------------------1------------------ \n");

	//只听一张牌，找到谁算谁
	if (stListenInfo[0].bHuLen == 1)   
	{
		cbCard = stListenInfo[0].HuInfo[0].bCard;
		m_GameLogic.AddCard(&tmpHc, cbCard);
		EstimateCardWeaveOne(tmpHc, cbCard, v3);
		m_GameLogic.ChuPai(&tmpHc, cbCard);

		sv = v3;
		return;
	}

	printf("-------------------2------------------ \n");

	//听多张牌时，求多张牌的交集
	cbCard = stListenInfo[0].HuInfo[0].bCard;
	m_GameLogic.AddCard(&tmpHc, cbCard);
	EstimateCardWeave(tmpHc, cbCard, v1);
	m_GameLogic.ChuPai(&tmpHc, cbCard);

	for (int i = 1; i < stListenInfo[0].bHuLen; ++i)
	{
		v2.clear();
		v3.clear();
		cbCard = stListenInfo[0].HuInfo[i].bCard;
		m_GameLogic.AddCard(&tmpHc, cbCard);
		EstimateCardWeave(tmpHc, cbCard, v2);
		m_GameLogic.ChuPai(&tmpHc, cbCard);

		SetIntersection(v1, v2, v3);

		if (v3.size() == 0) break;  

		v1 = v3;
	}

	//找到交集返回
	if (v3.size() > 0) 
	{
		sv = v3;
		return;
	}

	printf("-------------------3------------------ \n");

	//没有交集，求并集
	v1.clear();
	v2.clear();
	v3.clear();
	bool bSameType = true; //是否同花色
	_uint8 ct = CardType(stListenInfo[0].HuInfo[0].bCard);
	for (int i = 0; i < stListenInfo[0].bHuLen; ++i)
	{
		if (ct != CardType(stListenInfo[0].HuInfo[i].bCard))
		{
			bSameType = false;
			break;
		}
	}
	if (!bSameType) // 不同花色
	{
		cbCard = stListenInfo[0].HuInfo[0].bCard;
		m_GameLogic.AddCard(&tmpHc, cbCard);
		EstimateCardWeave(tmpHc, cbCard, v1);
		m_GameLogic.ChuPai(&tmpHc, cbCard);

		for (int i = 1; i < stListenInfo[0].bHuLen; ++i)
		{
			v2.clear();
			v3.clear();
			_uint8 cbCard = stListenInfo[0].HuInfo[i].bCard;
			m_GameLogic.AddCard(&tmpHc, cbCard);
			EstimateCardWeave(tmpHc, cbCard, v2);
			m_GameLogic.ChuPai(&tmpHc, cbCard);

			SetUnion(v1, v2, v3);

			v1 = v3;
		}

		sv = v3;
		return;
	}

	printf("-------------------4------------------ \n");


	//相同花色的并集，拿到听牌前后相关的牌求并集
	
	printf("handcard\n");
	for (int i = 0; i < tmpHc.ChangeableCardsLen; ++i)
	{
		printf("0x%02x, \n", tmpHc.ChangeableCards[i]);
	}
	printf("\n");
	
	std::vector<int> tmpV;

	cbCard = stListenInfo[0].HuInfo[0].bCard;
	m_GameLogic.AddCard(&tmpHc, cbCard);
	MJ_win_pattern pattern;
	if (m_GameLogic.FormatCards(&tmpHc, pattern.kan, pattern.jiang))
	{
		if (pattern.jiang == cbCard)
		{
			if (std::find(tmpV.begin(), tmpV.end(), cbCard) == tmpV.end())
			{
				tmpV.push_back(cbCard);
			}
		}

		for (int k = 0; k < 4; k++)
		{
			bool bFind = false;
			for (int n = 0; n < 4; ++n)
			{
				if (ExsitArray(pattern.kan[k][n], stListenInfo[0].HuInfo, stListenInfo[0].bHuLen))
				{
					bFind = true;
					break;
				}
			}
			if (bFind)
			{
				for (int m = 0; m < 4; m++)
				{
					if (pattern.kan[k][m] == INVALID_MJ_CARD) break;
					if (std::find(tmpV.begin(), tmpV.end(), pattern.kan[k][m]) == tmpV.end())
					{
						tmpV.push_back(pattern.kan[k][m]);
					}
				}	
			}
		}
		m_GameLogic.ChuPai(&tmpHc, cbCard);
	}

	for (int i = 0; i < tmpHc.ChangeableCardsLen; ++i)
	{
		if (ExsitVectorMember(tmpV, tmpHc.ChangeableCards[i]))
		{
			v3.push_back(tmpHc.ChangeableCards[i]);
		}
	}

	sv = v3;
	return;
}

void GetSanPai(const HandCards*pHc, MJ_opinfo& ganginfo)
{
	_uint8 bCardCount[MJ_TYPE_COMMON][10];
	memset(bCardCount, 0, sizeof(bCardCount));
	ganginfo.Clear();
	for (int i = 0; i < pHc->ChangeableCardsLen; i++)
	{
		_uint8 bType = CardType(pHc->ChangeableCards[i]);
		_uint8 bNum = CardNum(pHc->ChangeableCards[i]);
		
		bCardCount[bType][bNum]++;
	}

	for (_uint8 i = 0; i < MJ_TYPE_COMMON; ++i)
	{
		for (_uint8 j = 0; j < 10; ++j)
		{
			if (bCardCount[i][j] == 3)
			{
				ganginfo.Add(OpState(CardValue(i, j), TYPE_ANGANG));
			}
		}
	}
}

bool CompareListenInfo(tagListenInfos stListenInfo1, tagListenInfos stListenInfo2)
{
	if (stListenInfo1.bHuLen != stListenInfo2.bHuLen)
	{
		return false;
	}

	for (_uint8 i = 0; i < stListenInfo1.bHuLen; ++i)
	{
		if (stListenInfo1.HuInfo[i].bCard != stListenInfo2.HuInfo[i].bCard)
		{
			return false;
		}
	}

	return true;
}

void EstimatePreGang(_uint8 ChairID, _uint8 bOutCard, MJ_BUFFER<_uint8, 4>& PreGangInfo)
{
	PreGangInfo.Clear();

	MJ_opinfo stsanpai;			
	GetSanPai(&stOutHc, stsanpai);

	for (_uint8 i = 0; i < stsanpai.Size(); ++i)
	{
		HandCards stTempHc = stOutHc;
		stTempHc.ChangeableCards[stTempHc.ChangeableCardsLen++] = stsanpai[i].cbCard;
		m_GameLogic.ExecuteGang(&stTempHc, INVALID_CHAIRID, stsanpai[i].cbCard, stsanpai[i].nState);
		tagListenInfos stListenInfo[14];
		_uint8 bListenLen = 0;	

		//对比杠完之后听牌信息和之前保存的是否一致，不一致则不能杠
		if (IsListening(&stTempHc, stListenInfo, bListenLen) && CompareListenInfo(stListenInfo[0], m_stListenInfo)) 
		{
			PreGangInfo.Add(stsanpai[i].cbCard);
		}
	}
}

int main(int argc, char *argv[])
{	
	
// 	test_0();
//  	for ( int i = 0 ; i < 4 ; ++i  )
//  	{
//  		test_1(i);
//  	}

// 	HandCards hc;	
// 	CGameLogic cgl;
// 	hc.ChangeableCards[0] = 0x1;
// 	hc.ChangeableCards[1] = 0x1;	
// 	hc.ChangeableCards[2] = 0x1;
// 	hc.ChangeableCards[3] = 0x2;
// 	hc.ChangeableCards[4] = 0x3;
// 	hc.ChangeableCards[5] = 0x4;
// 	hc.ChangeableCards[6] = 0x5;
// 	hc.ChangeableCards[7] = 0x6;
// 	hc.ChangeableCards[8] = 0x7;

// 	hc.ChangeableCards[9] = 0x8;
// 	hc.ChangeableCards[10] = 0x9;
// 	hc.ChangeableCards[11] = 0x9;
// 	hc.ChangeableCards[12] = 0x9;
// 	hc.ChangeableCardsLen = 13;

// 	hc.FixedCardsLen = 0;

// 	bool ok;
// 	timeval begin, end;

// 	gettimeofday(&begin, 0);
// 	for (int i = 0;
// #ifdef DEBUG		
// 		i < 1;
// #else
// 		i < 10000;
// #endif
// 		i++)
// 	{			
// 		for (_uint8 c = 0; c < sizeof(x); c++)
// 		{
// 			cgl.AddCard(&hc, x[c]);		
// 			ok = cgl.FormatCards_Laizi(hc.ChangeableCards, hc.ChangeableCardsLen, 1);	
// 			cgl.ChuPai(&hc, x[c]);
// 		}
// 	}
// 	gettimeofday(&end, 0);	
// 	long t = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
// 	printf("%s\ttimer:%ld.%ld\n", ok ? "succ" : "failed", t / 1000000, t % 1000000);
 
	_uint8 ChairID = 0;
	std::vector<int> v;
	EstimateShowCards(ChairID, v);

	for (_uint8 i = 0; i < v.size(); ++i)
	{
		printf("v:0x%02x \n", v[i]);
	}

	return 0;
}
