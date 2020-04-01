#include "CardPoolReadJson2.h"
#include "common/log.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

extern Log log;
using namespace std;

CardPoolReadJson2* CardPoolReadJson2::instance_ = NULL;

CardPoolReadJson2* CardPoolReadJson2::GetInstance() {
    if (instance_ == NULL) {
        instance_ = new CardPoolReadJson2();
    }
    return instance_;
}

CardPoolReadJson2::CardPoolReadJson2() :
    is_fixed_cards_(false),
    card_json_file_("./conf/mj_card.json"), 
    card_data_count_(0),
    card_max_count_(0),
    banker_(0) {
    memset(&card_data_, 0, sizeof(card_data_));
}


CardPoolReadJson2::~CardPoolReadJson2() {
}


bool CardPoolReadJson2::ReadJsonFile() {
    ifstream ifs;
    ifs.open(card_json_file_.data(), std::ios::binary);
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

bool CardPoolReadJson2::ParseJsonValue(Json::Value &data) {
    if (!data.isMember("FixedCards") || !data.isMember("MaxCount") ||
        !data.isMember("Banker") || !data.isMember("Cards")) {
        log.error("%s json is error.\n", __FUNCTION__);
        return false;
    }

    if (data["FixedCards"].asInt()) {
        is_fixed_cards_ = true;
    } else {
        is_fixed_cards_ = false;
    }
    card_max_count_ = data["MaxCount"].asInt();
    banker_ = data["Banker"].asInt();
    card_data_count_ = 0;
    memset(&card_data_, 0, sizeof(card_data_));

    Json::Value arryobj = data["Cards"];
    int read_num = static_cast<size_t>(card_max_count_) > arryobj.size() ? arryobj.size() : card_max_count_;
    read_num = read_num > POOL_CARD_MAX ? POOL_CARD_MAX : read_num;
    for (int i = 0; i < read_num; ++i) {
        int card = arryobj[i].asInt();
        if (IsNormalCard(card)) {
            card_data_[i] = card;
            ++card_data_count_;
        }
    }
    log.info("%s load success. MaxCount:%d  actually count:%d\n", __FUNCTION__, card_max_count_, card_data_count_);

    return true;
}

bool CardPoolReadJson2::IsFixedCards() {
    return is_fixed_cards_;
}

int CardPoolReadJson2::GetBanker() {
    return banker_;
}

int CardPoolReadJson2::GetTestCard(_uint8 *pout, int out_size) {
    if (card_data_count_ == 0) {
        return 0;
    }

    out_size = out_size < card_data_count_ ? out_size : card_data_count_;
    for (int i = 0; i < out_size; ++i) {
        pout[i] = card_data_[i];
    }
    return out_size;
}



int CardPoolReadJson2::GetMaxCount()
{
    return card_max_count_;
}

int CardPoolReadJson2::GetDataCount() {
    return card_data_count_;
}

bool CardPoolReadJson2::IsNormalCard(_uint8 card) {
    //int type = CardType(card);
    //int num = CardNum(card);
    //if (type >= 0 && type <= 6 && num > 0 && num <= 9) {
    //    return true;
    //}

    //return false;
    return true;
}