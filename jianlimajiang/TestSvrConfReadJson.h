#ifndef __TESTSVRCONFREADJSON_H__
#define __TESTSVRCONFREADJSON_H__

#include <string>
#include "./include/json/json.h"

class TestSvrConfReadJson
{
public:
    static TestSvrConfReadJson* GetInstance();

    bool ReadJsonFile();

    bool GetAntiCheat();                                //��ȡ�Ƿ���������

private:
    TestSvrConfReadJson();

    ~TestSvrConfReadJson();

    bool ParseJsonValue(Json::Value& data);             //����json ����

private:
    static TestSvrConfReadJson* instance_;

    std::string conf_json_file_;	                        //�����ļ�����
    bool anti_cheat_;                                       //�ͻ��˷����׿���
};

#endif // !__TESTSVRCONFREADJSON_H__
