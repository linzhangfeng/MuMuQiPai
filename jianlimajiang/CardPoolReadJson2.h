#ifndef CARDPOOL_READJSON2_H
#define CARDPOOL_READJSON2_H

#include <vector>
#include <string>
#include "common.h"
#include "json/json.h"

class CardPoolReadJson2
{
public:
    static CardPoolReadJson2* GetInstance();

	bool ReadJsonFile();

	int GetTestCard(_uint8 *pout, int out_size);    //��ȡ�����ļ� 
    
	int GetMaxCount();                                  //��ȡ���ֵ
    
    int GetDataCount();                                 //��ȡʵ������

    bool IsNormalCard(_uint8 card);                     //�ж��Ƿ�Ϸ���

	bool ParseJsonValue(Json::Value &data);             //����json ����

    bool IsFixedCards();

    int GetBanker();
private:
	CardPoolReadJson2();
	~CardPoolReadJson2();

private:
    static CardPoolReadJson2* instance_;

    bool is_fixed_cards_;                                   //�Ƿ�ѡ�ù̶���
	std::string card_json_file_;	                        //�����ļ�����
	_uint8  card_data_[POOL_CARD_MAX];	                    //��ȡ��������
    int card_data_count_;                                   //��json�ж�����ʵ����Ч����
	int card_max_count_;		                            //�Ƴ�ʼ������
    int banker_;                                            //��ȡ��ׯ����Ϣ
};

#endif