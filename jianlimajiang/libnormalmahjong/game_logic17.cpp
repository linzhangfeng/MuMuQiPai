#include "game_logic.h"
#include "stdio.h"
#include <map>

#include <algorithm>
#include "../common/log.h"
using namespace std;



bool CGameLogic::FormatCards(HandCards *hcs, MJ_win_pattern& pattern)
{
	return FormatCards(vector<_uint8>(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]), pattern);
}

bool CGameLogic::FormatCards(std::vector<_uint8> vec, MJ_win_pattern& pattern)
{
	sort(vec.begin(), vec.end());
	
	vector<_uint8> vec_jiang;
	GetJiang(vec, vec_jiang);
	if (vec_jiang.empty())
	{
		return false;
	}

	vector<_uint8> vec_kan;
	vector<_uint8>::iterator it = vec_jiang.begin();
	while (it != vec_jiang.end())
	{
		vector<_uint8> tmp = vec;
		EraseCard(tmp, *it, 2);
		if (CheckSubVector(tmp , vec_kan))
		{
			GetKan(vec_kan, pattern);
			pattern.jiang = *it;
			return true;
		}
		++it;
	}
	return false;
}


void CGameLogic::GetKan(const std::vector<_uint8> &vec_kan, MJ_win_pattern& pattern)
{

	memset(pattern.kan, 0, sizeof(pattern.kan));

	if ( (vec_kan.size() % 3 ) )
	{
		
		return ;
	}

	if ( vec_kan.size()/3  > HAND_CARD_KAN_MAX )
	{
	
		return;
	}
	

	
	for ( size_t i = 0,j = 0  ; i < vec_kan.size() ; ++i  )
	{
		if (i != 0 && i % 3 == 0)
		{
			++j;
		}
		pattern.kan[j][i % 3] = vec_kan[i];

	}
}

bool CGameLogic::FormatCards_LaiZi(HandCards *hcs, MJ_win_pattern& pattern, int lai_sum)
{
	if ( (hcs->ChangeableCardsLen == 0 ) && lai_sum )
	{
		return true;
	}
	return FormatCards_LaiZi(vector<_uint8>(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]) , pattern , lai_sum);
}

bool CGameLogic::FormatCards_LaiZi(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum)
{
	if (vec.empty() && lai_sum)
	{
		return true;
	}
	
	sort(vec.begin(), vec.end());


	vector<_uint8> kan;
	if (CheckJiang_LaiZi(vec, pattern, lai_sum))
	{
		return true;
	}

	
	if (CheckShunZi_LaiZi(vec, pattern, lai_sum))
	{
		return true;
	}
	 
	return false;
}


bool CGameLogic::CheckJiang_LaiZi(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum_remain, bool bNeed258Jiang)
{
	if ( lai_sum_remain < 1 )
	{
		return false; 
	}
	vector<_uint8> kan;
	_uint8 card = INVALID_MJ_CARD;
	for (vector<_uint8>::iterator it = vec.begin() ; it != vec.end() ; ++it )
	{
		if (card != *it)
		{
			card = *it;

			if (bNeed258Jiang && !Is258Card(card)) 
			{
				continue;
			}

			vector<_uint8> tmp = vec;			
			EraseCard(tmp, card, 1);
			if (CheckSubVector_LaiZi(tmp, kan, lai_sum_remain - 1))
			{
				pattern.jiang = card;
				GetKan(kan, pattern);
				return true;
			}
		}
	}	
	
	return false;
}

bool CGameLogic::CheckShunZi_LaiZi(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum_remain, bool bNeed258Jiang)
{
	vector<_uint8> kan;
	if ( lai_sum_remain < 0 )
	{
		return false;
	}

	vector<_uint8> vec_jiang;
	GetJiang(vec, vec_jiang);
	
	for (vector<_uint8>::iterator it = vec_jiang.begin(); it != vec_jiang.end(); it++)
	{
		_uint8 card = *it;
		
		if (bNeed258Jiang && !Is258Card(card)) 
		{
			continue;
		}

		vector<_uint8> tmp = vec;
		EraseCard(tmp, card, 2);
		if (CheckSubVector_LaiZi(tmp, kan, lai_sum_remain))
		{
			pattern.jiang = card;
			GetKan(kan, pattern);
			return true;
		}
	}
	
	return false;
}

bool CGameLogic::CheckSubVector_LaiZi(std::vector<_uint8> vec, std::vector<_uint8> &kan , int lai_sum_remain)
{
	if ( vec.empty() )
	{
		return true;
	}

	if ( lai_sum_remain < 0  )
	{
		return false;
	}

	//癞子数量为0  直接使用非癞子算法
	if ( lai_sum_remain == 0  )
	{
		return CheckSubVector(vec, kan);
	}
	
	_uint8 first_card = vec[0];
	_uint8 need_lai_sum = 0; 

	//添加字牌判断
	if ((CardType(first_card) == MJ_TYPE_WAN) || (CardType(first_card) == MJ_TYPE_TIAO) || (CardType(first_card) == MJ_TYPE_TONG))
	{
		//万条筒才可以进行 顺子检测

		need_lai_sum =  GetShunZi_LaiZi(vec, first_card);

		vector<_uint8> tmp = vec;
		//提取出顺子
		EraseCard(tmp, first_card);
		EraseCard(tmp, first_card + 1);
		EraseCard(tmp, first_card + 2);
		if (CheckSubVector_LaiZi(tmp, kan, lai_sum_remain - need_lai_sum))
		{
			if (CardNum(first_card) == 8)
			{
				kan.push_back(first_card - 1);
				kan.push_back(first_card);
				kan.push_back(first_card + 1);
			}
			else if (CardNum(first_card) == 9)
			{
				kan.push_back(first_card - 2);
				kan.push_back(first_card - 1);
				kan.push_back(first_card);
			}
			else
			{
				kan.push_back(first_card);
				kan.push_back(first_card + 1);
				kan.push_back(first_card + 2);
			}
			return true;
		}
	}

	//将第一张牌组成刻字
	int cnt = count(vec.begin(), vec.end(), first_card);
	if ( cnt > 3 )
	{
		EraseCard(vec, first_card, 3);
		need_lai_sum = 0;
	}
	else
	{
		EraseCard(vec, first_card, cnt);
		need_lai_sum = 3 - cnt;
	}
	
	if ( CheckSubVector_LaiZi(vec , kan , lai_sum_remain - need_lai_sum  ))
	{
		kan.push_back(first_card);
		kan.push_back(first_card);
		kan.push_back(first_card);
		return true;
	}
	
	return false;
}

_uint8 CGameLogic::GetShunZi_LaiZi(const std::vector<_uint8> &vec, _uint8 card )
{
	if ( vec.empty() )
	{
		return 0;
	}

	if ( card == INVALID_MJ_CARD )
	{
		card = vec[0];
	}
	_uint8 single = 0;
	if ( find(vec.begin() , vec.end() , card + 1) == vec.end() )
	{
		single++;
	}
	if ( find(vec.begin() , vec.end() , card + 2 ) == vec.end() )
	{
		single++;
	}
	return single;
}


bool CGameLogic::CheckSubVector(std::vector<_uint8> vec, std::vector<_uint8> &kan)
{
	if ( vec.empty() )
	{
		return true;
	}
	if ( vec.size() < 3 )
	{
		return false;
	}

	_uint8 first_card = vec[0];
	int sum = count(vec.begin(), vec.end(), first_card);

	if (  sum == 1 || sum == 2 )
	{
		//风牌不存在顺子
		if ( (CardType(first_card) == MJ_TYPE_FENG) || ( CardType(first_card) == MJ_TYPE_JIAN )   )
		{
			return false;
		}
		//只能是 abc
		if ( CheckShunZi(vec , first_card , sum) )
		{
			EraseCard(vec, first_card, sum);
			EraseCard(vec, first_card + 1, sum);
			EraseCard(vec, first_card + 2, sum);
			if ( CheckSubVector(vec , kan) )
			{
				for ( int i = 0 ; i < sum ; ++i  )
				{
					kan.push_back(first_card);
					kan.push_back(first_card + 1);
					kan.push_back(first_card + 2);
				}
				return true;
			}
			
		}
	}
	else if ( sum == 3 )
	{
		//aaa (abc abc abc)
		//aaa
		vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);
		if ( CheckSubVector(tmp, kan))
		{
			for (int i = 0; i < sum; ++i)
			{
				kan.push_back(first_card);
			}
			return true;
		}
		//风牌不存在顺子
		if ((CardType(first_card) == MJ_TYPE_FENG) || (CardType(first_card) == MJ_TYPE_JIAN))
		{
			return false;
		}
		//abc abc abc
		if (  CheckShunZi(vec , first_card , 3))
		{
			EraseCard(vec, first_card, sum);
			EraseCard(vec, first_card + 1, sum);
			EraseCard(vec, first_card + 2, sum);
			if (CheckSubVector(vec, kan))
			{
				for (int i = 0; i < sum; ++i)
				{
					kan.push_back(first_card);
					kan.push_back(first_card + 1);
					kan.push_back(first_card + 2);
				}
				return true;
			}

		}
		return false;

	}
	else if ( sum >= 4 )
	{
		//风牌不存在顺子
		if ((CardType(first_card) == MJ_TYPE_FENG) || (CardType(first_card) == MJ_TYPE_JIAN))
		{
			return false;
		}

		//aaa+abc  (abc abc abc abc)

		//aaa+abc  
		vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);
		if ( CheckShunZi(tmp , first_card ) )
		{
			EraseCard(tmp, first_card);
			EraseCard(tmp, first_card + 1);
			EraseCard(tmp, first_card + 2);
		}
		if ( CheckSubVector(tmp , kan ) )
		{
			kan.push_back(first_card);
			kan.push_back(first_card);
			kan.push_back(first_card);

			kan.push_back(first_card);
			kan.push_back(first_card + 1);
			kan.push_back(first_card + 2);


			
			return true;
		}

		//abc abc abc abc
		if ( CheckShunZi(vec , first_card , sum) )
		{
			EraseCard(vec, first_card, sum);
			EraseCard(vec, first_card + 1, sum);
			EraseCard(vec, first_card + 2, sum);
			if (CheckSubVector(vec, kan))
			{
				for (int i = 0; i < sum; ++i)
				{
					kan.push_back(first_card);
					kan.push_back(first_card + 1);
					kan.push_back(first_card + 2);
				}
				return true;
			}
		}

	}
	return false;
}

void CGameLogic::GetJiang(std::vector<_uint8> &vec, std::vector<_uint8> &out_jiang)
{
	map<_uint8, int> tmp;
	for (vector<_uint8>::iterator it = vec.begin() ; it != vec.end() ; ++it)
	{
		if ( tmp.find(*it) == tmp.end() )
		{
			tmp[*it] = 0;
		}
		else
		{
			tmp[*it]++;
		}
	}

	map<_uint8, int>::iterator it = tmp.begin();
	while ( it != tmp.end() )
	{
		if ( it->second >= 1 )
		{
			out_jiang.push_back(it->first);
		}
		++it;
	}
}

bool CGameLogic::EraseCard(std::vector<_uint8> &vec , _uint8 card, _uint8 sum)
{
	if ( count(vec.begin() , vec.end() , card) < sum )
	{
		return false;
	}

	for ( int i = 0 ; i < sum ; ++i  )
	{
		vector<_uint8>::iterator it = find(vec.begin(), vec.end(), card);
		vec.erase(it);
	}
	return true;

}

bool CGameLogic::CheckShunZi(std::vector<_uint8> &vec, _uint8 card, _uint8 sum)
{
	if ( vec.size() < sum*3 )
	{
		return false;
	}
	if ( count(vec.begin() , vec.end() , card) < sum )
	{
		return false;
	}
	if (count(vec.begin(), vec.end(), card+1) < sum)
	{
		return false;
	}
	if (count(vec.begin(), vec.end(), card + 2) < sum)
	{
		return false;
	}
	return true;
}

int CGameLogic::GetNoLaiZiHandCard(const HandCards &card_in, HandCards &card_out, _uint8 m_laizi)
{
	int _sum = 0;
	for ( _uint8 i = 0 , j = 0 ; i < card_in.ChangeableCardsLen ; ++i  )
	{
		if ( card_in.ChangeableCards[i] != m_laizi )
		{
			++_sum;
			card_out.ChangeableCards[j++] = card_in.ChangeableCards[i];
		}
	}
	card_out.ChangeableCardsLen = _sum;

	card_out.FixedCardsLen = card_in.FixedCardsLen;
	memcpy(card_out.FixedCards, card_in.FixedCards, sizeof(card_in.FixedCards));
	return card_in.ChangeableCardsLen - card_out.ChangeableCardsLen;
}

int CGameLogic::GetNoLaiZiHandCard(const HandCards &card_in, HandCards &card_out, const std::vector<_uint8>& vec_laizi)
{
	HandCards tmp = card_in;
	vector<_uint8>::const_iterator it = vec_laizi.begin();
	int iCounts = 0;
	while ( it != vec_laizi.end() )
	{
		iCounts += GetNoLaiZiHandCard(tmp, card_out , *it);
		tmp = card_out;
		it++;
	}
	return iCounts;
}


int CGameLogic::GetLaiZiSum(const HandCards &phc, _uint8 m_laizi)
{
	return (int)count(&phc.ChangeableCards[0], &phc.ChangeableCards[0] + phc.ChangeableCardsLen, m_laizi);
}

int CGameLogic::GetLaiZiSum(const HandCards &phc, const std::vector<_uint8>& vec_laizi)
{
	int single = 0; 
	for ( int i = 0 ; i < phc.ChangeableCardsLen ; ++i  )
	{
		vector<_uint8>::const_iterator it = find(vec_laizi.begin(), vec_laizi.end(), phc.ChangeableCards[i]);
		if ( it != vec_laizi.end() )
		{
			++single;
		}
	}
	return single;
}

bool CGameLogic::CheckSubVectorPeng(HandCards &card_in, int lai_sum_remain)
{
	vector<_uint8> vec(&card_in.ChangeableCards[0], &card_in.ChangeableCards[card_in.ChangeableCardsLen]);
	return CheckSubVectorPeng(vec, lai_sum_remain);
}

bool CGameLogic::CheckSubVectorPeng(std::vector<_uint8> &vec, int lai_sum_remain)
{
	if ( vec.empty() )
	{
		return true;
	}

	if ( lai_sum_remain < 0  )
	{
		return false;
	}
	_uint8 first_card = vec[0];
	_uint8 need_lai_sum = 0;

	int cnt = count(vec.begin(), vec.end(), first_card);
	if ( cnt > 3 )
	{
		EraseCard(vec, first_card, 3);
		need_lai_sum = 0;
	}
	else
	{
		EraseCard(vec, first_card, cnt);
		need_lai_sum = 3 - cnt;
	}

	return CheckSubVectorPeng(vec, lai_sum_remain - need_lai_sum);
}





bool CGameLogic::CheckQing19(HandCards &no_laizi_handcard)
{
	_uint8 _card_type = INVALID_MJ_CARD;
	for ( int i = 0 ; i < no_laizi_handcard.FixedCardsLen ; ++i  )
	{
		if (no_laizi_handcard.FixedCards[i].state & TYPE_CHI)
		{
			return false;
		}
		if (CardType(no_laizi_handcard.FixedCards[i].CardData) == MJ_TYPE_FENG || CardType(no_laizi_handcard.FixedCards[i].CardData) == MJ_TYPE_JIAN)
		{
			continue;
		}
		if (!(CardNum(no_laizi_handcard.FixedCards[i].CardData) == 1 || CardNum(no_laizi_handcard.FixedCards[i].CardData) == 9))
		{
			return false;
		}
		else
		{
			_uint8 types = CardType(no_laizi_handcard.FixedCards[i].CardData);
			if ( _card_type != INVALID_MJ_CARD )
			{
				_card_type = types;
			}
			else if ( _card_type != types )
			{
				return false;
			}			
		}
	}
	for ( int i = 0 ; i < no_laizi_handcard.ChangeableCardsLen ; ++i  )
	{

		_uint8 types = CardType(no_laizi_handcard.ChangeableCards[i]);
		if ( types == MJ_TYPE_FENG || types == MJ_TYPE_JIAN )
		{
			continue;
		}
		if ( ! ( CardNum(no_laizi_handcard.ChangeableCards[i])==1 || CardNum(no_laizi_handcard.ChangeableCards[i]) == 9  )  )
		{
			return false;
		}
		else
		{			
			if ( _card_type == INVALID_MJ_CARD )
			{
				_card_type = types;
			}
			else if ( _card_type != types )
			{
				return false;
			}
		}
	}
	return true;
}

bool CGameLogic::Check7Dui(HandCards &no_laizi_handcard, int lai_sum)
{
	if ( no_laizi_handcard.FixedCardsLen )
	{
		return false;
	}
	if ( no_laizi_handcard.ChangeableCardsLen == 0 )
	{
		return true;
	}
	return CheckSubVectorDui(vector<_uint8>(&no_laizi_handcard.ChangeableCards[0], &no_laizi_handcard.ChangeableCards[0] + no_laizi_handcard.ChangeableCardsLen), lai_sum);
}

bool CGameLogic::CheckSubVectorDui(std::vector<_uint8> vec, int lai_sum)
{
	sort(vec.begin(), vec.end());

	while (!vec.empty())
	{
		_uint8 first_card = vec[0];
		int sum = count(vec.begin(), vec.end(), first_card);

		if (sum % 2)
		{
			--lai_sum;
		}

		if (lai_sum < 0)
		{
			return false;
		}

		EraseCard(vec, first_card, sum);
	}
	return true;
}

/*字牌牌组成顺子*/
bool CGameLogic::FormatCards_01(HandCards *hcs, MJ_win_pattern& pattern)
{
	return FormatCards_01(vector<_uint8>(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]), pattern);
}

/*字牌牌组成顺子*/
bool CGameLogic::FormatCards_01(std::vector<_uint8> vec, MJ_win_pattern& pattern)
{
	sort(vec.begin(), vec.end());
	
	vector<_uint8> vec_jiang;
	GetJiang(vec, vec_jiang);
	if (vec_jiang.empty())
	{
		return false;
	}

	std::vector<_uint8> vec_kan;
	std::vector<_uint8>::iterator it = vec_jiang.begin();
	while (it != vec_jiang.end())
	{
		std::vector<_uint8> tmp = vec;
		EraseCard(tmp, *it, 2);

		std::vector<_uint8> vecZi;
		GetZi(tmp, vecZi);

		if ((CheckSubVector(tmp, vec_kan)) 
			&& (CheckSubVector_01(vecZi, vec_kan)))
		{
			GetKan(vec_kan, pattern);
			pattern.jiang = *it;
			return true;
		}
		++it;
	}
	return false;
}

void CGameLogic::GetZi(std::vector<_uint8> &vec, std::vector<_uint8> &vecZi)
{
	vecZi.clear();

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (CardType(vec[i]) == MJ_TYPE_FENG || CardType(vec[i]) == MJ_TYPE_JIAN)
			vecZi.push_back(vec[i]);
	}

	for (size_t i = 0; i < vecZi.size(); ++i)
	{
		EraseCard(vec, vecZi[i]);
	}
}

/*风牌组成顺子*/
bool CGameLogic::CheckShunZi_01(std::vector<_uint8> &vec, _uint8 card, _uint8 sum, std::vector<std::vector<_uint8> >& vecShun)
{
	vecShun.clear();

	int count0 = count(vec.begin(), vec.end(), card);
	int count1 = count(vec.begin(), vec.end(), card + 1);
	int count2 = count(vec.begin(), vec.end(), card + 2);
	int count3 = count(vec.begin(), vec.end(), card + 3);

	if (count0 > 0 && count1 > 0 && count2 > 0)
	{
		std::vector<_uint8> tmp;
		tmp.push_back(card);
		tmp.push_back(card + 1);
		tmp.push_back(card + 2);
		vecShun.push_back(tmp);
	}
	
	if (count0 > 0 && count1 > 0 && count3 > 0)
	{
		std::vector<_uint8> tmp;
		tmp.push_back(card);
		tmp.push_back(card + 1);
		tmp.push_back(card + 3);
		vecShun.push_back(tmp);
	}
	
	if (count0 > 0 && count2 > 0 && count3 > 0)
	{
		std::vector<_uint8> tmp;
		tmp.push_back(card);
		tmp.push_back(card + 2);
		tmp.push_back(card + 3);
		vecShun.push_back(tmp);
	}

	return vecShun.size() > 0;
}

/*只用于风牌的检测*/
bool CGameLogic::CheckSubVector_01(std::vector<_uint8> vec, std::vector<_uint8> &kan)
{
	if ( vec.empty() )
	{
		return true;
	}
	if ( vec.size() < 3 )
	{
		return false;
	}

	_uint8 first_card = vec[0];
	int sum = count(vec.begin(), vec.end(), first_card);

	if (sum == 1 || sum == 2)
	{
		//全部组成顺子
		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_01(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp, vecShun[i][j]);
				}
				if (CheckSubVector_01(tmp , kan))
				{
					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}
	}
	else if (sum == 3)
	{
		//组成坎
		std::vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);
		if (CheckSubVector_01(tmp, kan))
		{
			for (int i = 0; i < sum; ++i)
			{
				kan.push_back(first_card);
			}
			return true;
		}

		//全部组成顺子
		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_01(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp, vecShun[i][j]);
				}
				if (CheckSubVector_01(tmp , kan))
				{
					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}
		return false;

	}
	else if (sum == 4)
	{
		//组成坎后组成顺子
		vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);

		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_01(tmp, first_card, 1, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp1 = tmp;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp1, vecShun[i][j]);
				}
				if (CheckSubVector_01(tmp1 , kan))
				{
					kan.push_back(first_card);
					kan.push_back(first_card);
					kan.push_back(first_card);

					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}

		//全部组成顺子
		if (CheckShunZi_01(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp, vecShun[i][j]);
				}
				if (CheckSubVector_01(tmp , kan))
				{
					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}
	}
	return false;
}

/*只用于风牌的检测 , 风牌优先组成顺子*/
bool CGameLogic::CheckSubVector_02(std::vector<_uint8> vec, std::vector<_uint8> &kan)
{
	if ( vec.empty() )
	{
		return true;
	}
	if ( vec.size() < 3 )
	{
		return false;
	}

	_uint8 first_card = vec[0];
	int sum = count(vec.begin(), vec.end(), first_card);

	if (sum == 1 || sum == 2)
	{
		//全部组成顺子
		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_01(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp, vecShun[i][j]);
				}
				if (CheckSubVector_02(tmp , kan))
				{
					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}
	}
	else if (sum == 3)
	{
		//全部组成顺子
		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_01(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp, vecShun[i][j]);
				}
				if (CheckSubVector_02(tmp , kan))
				{
					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}
		//组成坎
		std::vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);
		if (CheckSubVector_02(tmp, kan))
		{
			for (int i = 0; i < sum; ++i)
			{
				kan.push_back(first_card);
			}
			return true;
		}
		return false;
	}
	else if (sum == 4)
	{
		std::vector<std::vector<_uint8> > vecShun;
		//全部组成顺子
		if (CheckShunZi_01(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp, vecShun[i][j]);
				}
				if (CheckSubVector_02(tmp , kan))
				{
					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}

		//组成坎后组成顺子
		vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);

		if (CheckShunZi_01(tmp, first_card, 1, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp1 = tmp;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard(tmp1, vecShun[i][j]);
				}
				if (CheckSubVector_02(tmp1 , kan))
				{
					kan.push_back(first_card);
					kan.push_back(first_card);
					kan.push_back(first_card);

					for (size_t j = 0; j < vecShun[i].size(); ++j)
					{
						kan.push_back(vecShun[i][j]);
					}
					return true;
				}
			}	
		}
	}
	return false;
}

/*只用于风牌的检测 , 风牌只组成顺子*/
bool CGameLogic::CheckSubVector_03(std::vector<_uint8> vec, std::vector<_uint8> &kan)
{
	if ( vec.empty() )
	{
		return true;
	}
	if ( vec.size() < 3 )
	{
		return false;
	}

	_uint8 first_card = vec[0];
	int sum = count(vec.begin(), vec.end(), first_card);

	//全部组成顺子
	std::vector<std::vector<_uint8> > vecShun;
	if (CheckShunZi_01(vec, first_card, sum, vecShun))
	{
		for (size_t i = 0; i < vecShun.size(); ++i)
		{
			std::vector<_uint8> tmp = vec;
			for (size_t j = 0; j < vecShun[i].size(); ++j)
			{
				EraseCard(tmp, vecShun[i][j]);
			}
			if (CheckSubVector_03(tmp , kan))
			{
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					kan.push_back(vecShun[i][j]);
				}
				return true;
			}
		}	
	}
	
	return false;
}



//19组成顺子

/*19组成顺子*/
bool CGameLogic::CheckShunZi_02(std::vector<_uint8> &vec, _uint8 card, _uint8 sum, std::vector<std::vector<_uint8> >& vecShun)
{
	vecShun.clear();

	_uint8 CardDataNum[10] = {0};
	for (size_t i = 0; i < vec.size(); ++i)
	{
		CardDataNum[CardNum(vec[i])]++;
	}

	if (CardDataNum[1] > 0 && CardDataNum[9] > 1)
	{
		std::vector<_uint8> tmp;
		tmp.push_back(1);
		tmp.push_back(9);
		tmp.push_back(9);
		vecShun.push_back(tmp);
	}

	if (CardDataNum[1] > 1 && CardDataNum[9] > 0)
	{
		std::vector<_uint8> tmp;
		tmp.push_back(1);
		tmp.push_back(1);
		tmp.push_back(9);
		vecShun.push_back(tmp);
	}

	return vecShun.size() > 0;
}

bool CGameLogic::EraseCard_02(std::vector<_uint8> &vec , _uint8 card, std::vector<_uint8> &res, _uint8 sum)
{
	std::vector<_uint8> tmp;
	for (size_t j = 0; j < vec.size(); ++j)
	{
		if (CardNum(vec[j]) == card)
		{
			if ((int)tmp.size() < sum)
			{
				tmp.push_back(vec[j]);
			}
		}  
	}

	for (size_t j = 0; j < tmp.size(); ++j)
	{
		vector<_uint8>::iterator it = find(vec.begin(), vec.end(), tmp[j]);
		vec.erase(it);
		res.push_back(tmp[j]);
	}

  return true;
}

/*只用于19的检测 , 19优先组成顺子*/
bool CGameLogic::CheckSubVector_04(std::vector<_uint8> vec, std::vector<_uint8> &kan)
{
	if ( vec.empty() )
	{
		return true;
	}
	if ( vec.size() < 3 )
	{
		return false;
	}

	_uint8 first_card = vec[0];
	int sum = count(vec.begin(), vec.end(), first_card);

	if (sum == 1 || sum == 2)
	{
		//全部组成顺子
		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_02(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				std::vector<_uint8> res;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard_02(tmp, vecShun[i][j], res);
				}
				if (CheckSubVector_04(tmp , kan))
				{
					for (size_t j = 0; j < res.size(); ++j)
					{
						kan.push_back(res[j]);
					}
					return true;
				}
			}	
		}
	}
	else if (sum == 3)
	{
		//全部组成顺子
		std::vector<std::vector<_uint8> > vecShun;
		if (CheckShunZi_02(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				std::vector<_uint8> res;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard_02(tmp, vecShun[i][j], res);
				}
				if (CheckSubVector_04(tmp , kan))
				{
					for (size_t j = 0; j <res.size(); ++j)
					{
						kan.push_back(res[j]);
					}
					return true;
				}
			}	
		}
		//组成坎
		std::vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);
		if (CheckSubVector_04(tmp, kan))
		{
			for (int i = 0; i < sum; ++i)
			{
				kan.push_back(first_card);
			}
			return true;
		}
		return false;
	}
	else if (sum == 4)
	{
		std::vector<std::vector<_uint8> > vecShun;
		//全部组成顺子
		if (CheckShunZi_02(vec, first_card, sum, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp = vec;
				std::vector<_uint8> res;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard_02(tmp, vecShun[i][j], res);
				}
				if (CheckSubVector_04(tmp , kan))
				{
					for (size_t j = 0; j < res.size(); ++j)
					{
						kan.push_back(res[j]);
					}
					return true;
				}
			}	
		}

		//组成坎后组成顺子
		vector<_uint8> tmp = vec;
		EraseCard(tmp, first_card, 3);

		if (CheckShunZi_02(tmp, first_card, 1, vecShun))
		{
			for (size_t i = 0; i < vecShun.size(); ++i)
			{
				std::vector<_uint8> tmp1 = tmp;
				std::vector<_uint8> res;
				for (size_t j = 0; j < vecShun[i].size(); ++j)
				{
					EraseCard_02(tmp1, vecShun[i][j], res);
				}
				if (CheckSubVector_04(tmp1 , kan))
				{
					kan.push_back(first_card);
					kan.push_back(first_card);
					kan.push_back(first_card);

					for (size_t j = 0; j < res.size(); ++j)
					{
						kan.push_back(res[j]);
					}
					return true;
				}
			}	
		}
	}
	return false;
}

/*19只组成顺子*/
bool CGameLogic::CheckSubVector_05(std::vector<_uint8> vec, std::vector<_uint8> &kan)
{
	if ( vec.empty() )
	{
		return true;
	}
	if ( vec.size() < 3 )
	{
		return false;
	}

	_uint8 first_card = vec[0];
	int sum = count(vec.begin(), vec.end(), first_card);

	//全部组成顺子
	std::vector<std::vector<_uint8> > vecShun;
	if (CheckShunZi_02(vec, first_card, sum, vecShun))
	{
		for (size_t i = 0; i < vecShun.size(); ++i)
		{
			std::vector<_uint8> tmp = vec;
			std::vector<_uint8> res;
			for (size_t j = 0; j < vecShun[i].size(); ++j)
			{
				EraseCard_02(tmp, vecShun[i][j], res);
			}
			if (CheckSubVector_05(tmp, kan))
			{
				for (size_t j = 0; j < res.size(); ++j)
				{
					kan.push_back(res[j]);
				}
				return true;
			}
		}	
	}
	
	return false;
}

bool CGameLogic::IsJianZiHu(HandCards hcs, int lai_sum, bool bDai7Dui)
{
	if (bDai7Dui)
	{
		if (hcs.FixedCardsLen > 0) 
		{
			bDai7Dui = false;
		}
	}

	return IsJianZiHu(vector<_uint8>(&hcs.ChangeableCards[0], &hcs.ChangeableCards[hcs.ChangeableCardsLen]), lai_sum, bDai7Dui);
}

bool CGameLogic::IsJianZiHu(std::vector<_uint8> vec, int lai_sum, bool bDai7Dui)
{
	if (lai_sum < 1) 
	{
		return false;
	}

	if (bDai7Dui)
	{
		if (CheckSubVectorDui(vec, lai_sum - 1)) 
		{
			return true;
		}
	}

	std::vector<_uint8> kan;
	return CheckSubVector_LaiZi(vec, kan, lai_sum - 1);
}

bool CGameLogic::FormatCards_LaiZi_258Jiang(const HandCards *hcs, MJ_win_pattern& pattern, int lai_sum, _uint8 laizicard)
{
	return FormatCards_LaiZi_258Jiang(vector<_uint8>(&hcs->ChangeableCards[0], &hcs->ChangeableCards[hcs->ChangeableCardsLen]), pattern, lai_sum, laizicard);
}

bool CGameLogic::FormatCards_LaiZi_258Jiang(std::vector<_uint8> vec, MJ_win_pattern& pattern, int lai_sum, _uint8 laizicard) 
{
	sort(vec.begin(), vec.end());

	//纯癞子做将，需传入参数癞子
	if (laizicard != INVALID_MJ_CARD && lai_sum >= 2)
	{
		vector<_uint8> kan;
		vector<_uint8> tmp = vec;
		if (CheckSubVector_LaiZi(tmp, kan, lai_sum - 2))
		{
			pattern.jiang = laizicard;
			GetKan(kan, pattern);
			return true;
		}
	}

	vector<_uint8> kan;
	if (CheckJiang_LaiZi(vec, pattern, lai_sum, true)) 
	{
		return true;
	}

	if (CheckShunZi_LaiZi(vec, pattern, lai_sum, true)) 
	{
		return true;
	}

	return false;
}