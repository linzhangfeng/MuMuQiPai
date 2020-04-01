#ifndef __TABLE_JIAN_LI_ZFB_H__
#define __TABLE_JIAN_LI_ZFB_H__

#include "../table.h"
#include "../proto/jianligameproto.pb.h"

//游戏自定义ID 5100-5200
#define SERVER_BROAD_START_PIAO 5101  //开始选择飘
#define SERVER_BROAD_CHOOSE_PIAO 5102 //玩家选择飘
#define SERVER_BROAD_RESULT_PIAO 5103 //广播飘的结果

//游戏自定义ID 2000-2100
#define CLIENT_CHOOSE_PIAO 2001 //开始选择飘

#define TYPE_MINGLOU 0x4000 //明楼

class Table_jianliZFB : public Table
{
public:
    Table_jianliZFB();
    ~Table_jianliZFB();

public:
    void GameStart();
    void GameEnd();
    void Send_Game_Scene(Player *player);
    void ConfInit_Remote(const Json::Value &val);
    int dispatch_client(int cmd, Client *client, Player *player);
    void GameEndReset();
    void Proto_Game_OpNotifyAfterPengChi(_uint8 ChairID, _uint8 CurChairID, const MJ_opinfo &gang, proto::game::AckOpNotify &stNotify);
    void Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard &stUsergetcard);
    void recv_client_outcard(Client *client, Player *player);
    void Proto_Game_UserOutCard(_uint8 ChairID, _uint8 bCard, proto::game::AckUserOutCard &stUseroutcard);

private:
    void startPiao();
    void recv_client_piao(Client *client, Player *player);
    bool isMingLou(_uint8 Chaird);
protected:
    MJ_BUFFER<_uint8, 10> m_canPiao;
    int m_piaoScore[GAME_PLAYER];
    int m_minglouState[GAME_PLAYER];
    int m_showcardState[GAME_PLAYER];
};
#endif
