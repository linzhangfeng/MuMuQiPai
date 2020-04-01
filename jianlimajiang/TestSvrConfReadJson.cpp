#include "TestSvrConfReadJson.h"
#include "common/log.h"
#include "./include/json/json.h"

#include <fstream>

extern Log log;

TestSvrConfReadJson* TestSvrConfReadJson::instance_ = NULL;

TestSvrConfReadJson* TestSvrConfReadJson::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new TestSvrConfReadJson();
    }
    return instance_;
}

bool TestSvrConfReadJson::ReadJsonFile()
{
    std::ifstream ifs;
    ifs.open(conf_json_file_.data(), std::ios::binary);
    if (!ifs) {
        log.error("%s open file fail.\n", __FUNCTION__);
        return false;
    }
    Json::Reader json_read;
    Json::Value rootValue;
    if (!json_read.parse(ifs, rootValue)) {
        log.error("%s json is error.\n", __FUNCTION__);
        return false;
    }
    ifs.close();
    return ParseJsonValue(rootValue);
}

bool TestSvrConfReadJson::GetAntiCheat()
{
    return anti_cheat_;
}

TestSvrConfReadJson::TestSvrConfReadJson() :
    conf_json_file_("./conf/test_svr_conf.json"),
    anti_cheat_(true)
{

}

TestSvrConfReadJson::~TestSvrConfReadJson()
{

}

bool TestSvrConfReadJson::ParseJsonValue(Json::Value& data)
{
    if (data.isMember("AntiCheat") && data["AntiCheat"].asInt() == 0)
    {
        anti_cheat_ = false;
    }
    else
    {
        anti_cheat_ = true;
    }

    return true;
}
