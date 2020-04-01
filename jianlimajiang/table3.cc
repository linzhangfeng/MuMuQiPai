#include "table.h"
#include "common/log.h"

//#include "location/table_jianlimj.h"
#include "location/table_jianlizfb.h"
extern Log log;

Table* Table::CreateTable(const Json::Value& table_val, const Json::Value& val)
{
	const string& loc = table_val["location"].asString();	

	log.info("%s local:%s val:%s\n", __FUNCTION__, loc.c_str(), val.toStyledString().c_str());

	return new Table_jianliZFB();
}
