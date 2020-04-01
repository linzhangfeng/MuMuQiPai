#include "jpacket.h"
#include "../proto/proto.h"
#include "log.h"

extern Log log;

void xorfunc(std::string &nString)
{
	const int KEY = 13;
	int strLen = (nString.length());
	char *cString = (char*)(nString.c_str());
	
	for (int i = 0; i < strLen; i++)
	{
		*(cString + i) = (*(cString + i) ^ KEY);
	}
}
void xorfunc(char* cString, int strLen)
{
	const int KEY = 13;
	for (int i = 0; i < strLen; i++)
	{
		cString[i] ^= KEY;
	}
}
Jpacket::Jpacket()
{
	useless = 0;
	protected_bound = 0;
}

Jpacket::~Jpacket()
{
}

std::string& Jpacket::tostring()
{
    return str;
}
const std::string& Jpacket::tostring()const
{
	return str;
}
Json::Value& Jpacket::tojson()
{
    return val;
}

void Jpacket::end()
{    
	char out[0x20000] = "";
	
	strncpy(out, val.toStyledString().c_str(), sizeof(out));
	out[sizeof(out) - 1] = 0;
	int strLen = strlen(out);
			
	JsonHeaderEx header;
	header.cmd = val["cmd"].asInt();
	header.d.length = strLen;
	
	log.debug("SendDataStyled:%s", out);
	xorfunc(out, strLen);

    str.clear();
    str.append((const char *)&header, sizeof(struct JsonHeaderEx));
    str.append(out);
}


int Jpacket::parse(char* buf, int len)
{
	Json::Reader reader;
	
	xorfunc(buf, len);
	if (reader.parse(buf, val) < 0)
	{
		return -1;
	}
	log.debug("RecvDataStyled:%s", val.toStyledString().c_str());
	return 0;
}

int Jpacket::sefe_check()
{
	if (val.type() != Json::objectValue)
	{
		log.error("%s parse error!\n", __FUNCTION__);
		return -2;
	}
	if (val["cmd"].isNull())
	{
		log.error("%s cmd is null!\n", __FUNCTION__);
		return -2;
	}	

	if (!val["cmd"].isNumeric())
	{
		log.error("%s cmd is not number!\n", __FUNCTION__);
		return -2;
	}	
	int cmd = val["cmd"].asInt();
	return  cmd;
}

