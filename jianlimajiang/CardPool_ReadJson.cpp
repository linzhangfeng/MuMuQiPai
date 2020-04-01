#include "CardPool_ReadJson.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

CardPool_ReadJson::CardPool_ReadJson():m_MaxCount(0)
{
}


CardPool_ReadJson::~CardPool_ReadJson()
{
}


void CardPool_ReadJson::ReadJsonFile(std::string str_file_name)
{
	m_TestFileName = str_file_name;
	ifstream ifs;
	ifs.open(m_TestFileName.data() , std::ios::binary);
	if ( !ifs )
	{
		cout << "open failed" << endl;
	}
	Json::Reader json_read;
	Json::Value rootValue;
	if ( !json_read.parse(ifs , rootValue ) )
	{
		return;
	}

	ParseJsonValue(rootValue);
	ifs.close();
}

void CardPool_ReadJson::ParseJsonValue(Json::Value  &data)
{
	Json::Value _max_count = data["MaxCount"];
	if (_max_count.isInt() )
	{
		m_MaxCount = _max_count.asInt();
	}
	Json::Value arryobj = data["CardPool"];
	for ( size_t i = 0 ; i < arryobj.size() ; ++i  )
	{
		if (arryobj[i].isString())
		{
			ParseOneRecord(arryobj[i].asString());
		}
	}
	
}

void CardPool_ReadJson::ParseOneRecord(std::string record)
{
	string str_tmp;
	while (record.length() )
	{
		size_t pos = record.find(',');
		str_tmp = record.substr(0, pos );

		while (str_tmp[0] == ' ' || str_tmp[0] == '\n' || str_tmp[0] == '\t' || str_tmp[0] == '\r')
		{
			str_tmp = str_tmp.substr(1);
		}
		str_tmp = str_tmp.substr(2);	

		m_CardData.Add(Atoi(str_tmp,16));
		if ( record.length() == 1 )
		{
			
			break;
		}
		else
		{
			record = record.substr(pos + 1);
		}
	}
}

_tint32 CardPool_ReadJson::GetTestCard(_uint8 *pout, _uint32 out_size)
{
	if ( m_CardData.Size() == 0 )
	{
		return 0;
	}

	if (_uint32( m_CardData.Size()) < out_size )
	{
		out_size = _uint32(m_CardData.Size());
	}
	for (_uint32 i = 0 ; i < out_size ; ++i  )
	{
		pout[i] = m_CardData[i];
	}
	return out_size;
}

_uint32 CardPool_ReadJson::GetMaxCount()
{
	return m_MaxCount;
}

_uint32 CardPool_ReadJson::Atoi(const std::string num, int base)
{
	_uint32 ret = 0;
	_uint32 tbase = 1;
	_uint32 rmove = 0;
	switch (base)
	{
	case 2:
		rmove = 1;
		break;
	case 8:
		rmove = 3;
		break;
	case 16:
		rmove = 4;
		break;
	default:
		return 0;
	}
	for (int i = num.length() - 1; i >= 0; --i)
	{
		if (num[i] <= '9' && num[i] >= '0')
		{
			ret = ret + (num[i] - '0')* tbase;
			tbase = tbase << rmove;
		}
		else
		{
			return 0;
		}
	}
	return ret;
}

