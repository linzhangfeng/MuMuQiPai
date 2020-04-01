#ifndef CARDPOOL_READJSON_H
#define CARDPOOL_READJSON_H

#include <vector>
#include <string>
#include "common.h"
#include "json/json.h"
#include "libnormalmahjong/mj_common2.h"

class CardPool_ReadJson
{
public:
	CardPool_ReadJson();
	virtual ~CardPool_ReadJson();
	
	void ReadJsonFile(std::string str_file_name);

	_tint32 GetTestCard(_uint8 *pout, _uint32 out_size); //��ȡ�����ļ� 

	_uint32 GetMaxCount();//��ȡ���ֵ
	
private:



	void ParseJsonValue(Json::Value  &data);  //����json ����
	void ParseOneRecord(std::string rcord);//����һ������
	_uint32 Atoi(const std::string num, int base);

	std::string m_TestFileName;	//�����ļ�����
	//std::vector< std::vector<_uint8> > m_TestCardData;	//��������
	MJ_BUFFER<_uint8, POOL_CARD_MAX> m_CardData;	//��ȡ��������
	_uint32 m_MaxCount;		//�Ƴ�ʼ������
};

#endif