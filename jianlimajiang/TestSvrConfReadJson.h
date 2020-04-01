#ifndef __TESTSVRCONFREADJSON_H__
#define __TESTSVRCONFREADJSON_H__

#include <string>
#include "./include/json/json.h"

class TestSvrConfReadJson
{
public:
    static TestSvrConfReadJson* GetInstance();

    bool ReadJsonFile();

    bool GetAntiCheat();                                //获取是否开启防作弊

private:
    TestSvrConfReadJson();

    ~TestSvrConfReadJson();

    bool ParseJsonValue(Json::Value& data);             //解析json 数组

private:
    static TestSvrConfReadJson* instance_;

    std::string conf_json_file_;	                        //测试文件名称
    bool anti_cheat_;                                       //客户端防作弊开关
};

#endif // !__TESTSVRCONFREADJSON_H__
