#ifndef __COMMON_MAHJONG_LOGIC_H__
#define __COMMON_MAHJONG_LOGIC_H__
#include "mj_common2.h"
#include "mjcardtype.h"



/*
 * ������ һ�����ƣ�����������
 * uiCounts = 3, ��ʾ������3�ŵ���
 * uiCounts = 4, ��ʾ������4�ŵ���
 */
int HandCardsHasMultiCard(HandCards *pHc, _uint8 uiCounts, MJ_cards_type_buffer& pCardsType);
/*
 * ������pCardsType �����������Ʒ��� true������������
 */
bool HandCardsHasAnyCard(HandCards* pHc, const MJ_cards_type_buffer& pCardsType, MJ_cards_type_buffer& pOut);
/*
 * �������ض����ֵ��ƣ� ����������
 */
bool HandCardsHasSpecialNumber(HandCards* pHc, _uint8 uiNumber);
/*
 * ����ÿ�ֻ�ɫ�м���
 */
void HandCardsSuitCounts(HandCards* pHc, _uint8 uiSuitCount[5]);

#endif