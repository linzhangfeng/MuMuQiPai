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

	int GetTestCard(_uint8 *pout, int out_size);    //获取测试文件 
    
	int GetMaxCount();                                  //获取最大值
    
    int GetDataCount();                                 //获取实际牌数

    bool IsNormalCard(_uint8 card);                     //判断是否合法牌

	bool ParseJsonValue(Json::Value &data);             //解析json 数组

    bool IsFixedCards();

    int GetBanker();
private:
	CardPoolReadJson2();
	~CardPoolReadJson2();

private:
    static CardPoolReadJson2* instance_;

    bool is_fixed_cards_;                                   //是否选用固定牌
	std::string card_json_file_;	                        //测试文件名称
	_uint8  card_data_[POOL_CARD_MAX];	                    //读取的牌数据
    int card_data_count_;                                   //从json中读到的实际有效牌数
	int card_max_count_;		                            //牌初始化数量
    int banker_;                                            //读取的庄家信息
};

#endif