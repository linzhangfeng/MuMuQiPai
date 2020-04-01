#include "table_jianlizfb.h"

#include <time.h>

#include "../zjh.h"
#include "../common/log.h"

extern ZJH zjh;
extern Log log;

Table_jianliZFB::Table_jianliZFB()
{
    memset(m_piaoScore, -1, sizeof(m_piaoScore));
    memset(m_minglouState, -1, sizeof(m_minglouState));
    memset(m_showcardState, -1, sizeof(m_showcardState));
}

Table_jianliZFB::~Table_jianliZFB()
{
}

void Table_jianliZFB::GameStart()
{
    Table::GameStart();

    startPiao();
}

void Table_jianliZFB::Send_Game_Scene(Player *player)
{
    if (player == NULL)
    {
        log.error("Send_GameScene, player is NULL \n");
        return;
    }
    int ChairID = player->seatid;

    proto::game::jianlimj::tagCurGameScence stCurGameScence;

    proto::game::AckGameScene *stGameScene = stCurGameScence.mutable_gamesence();

    Proto_GameScene(ChairID, player, *stGameScene);

    stGameScene->set_scenestatus(m_bGameState); //客户端认为状态不为free游戏就已经开始
    for (int i = 0; i < m_GAME_PLAYER; i++)
    {
        stCurGameScence.add_userpaotou(m_piaoScore[i]);
        stCurGameScence.add_mingloustate(m_minglouState[i]);
    }

    stCurGameScence.set_ispaotouing(m_bGameState == GAME_PIAO);
    unicast(SERVER_GAME_SCENE, player, stCurGameScence);
}

void Table_jianliZFB::ConfInit_Remote(const Json::Value &val)
{
    Table::ConfInit_Remote(val);

    if (json_check_play_type_id(val))
    {
        MJ_SUB_ID SubID;
        json_get_play_type_id(val, 1000, SubID);
        if (SubID.Find(4))
        {
            m_GAME_PLAYER = 4;
        }
        else if (SubID.Find(3))
        {
            m_GAME_PLAYER = 3;
        }
        else if (SubID.Find(2))
        {
            m_GAME_PLAYER = 2;
        }

        m_canPiao.Add(0);
        m_canPiao.Add(1);
        m_canPiao.Add(2);
    }
}

void Table_jianliZFB::Proto_Game_OpNotifyAfterPengChi(_uint8 ChairID, _uint8 CurChairID, const MJ_opinfo &gang, proto::game::AckOpNotify &stNotify)
{
    Table::Proto_Game_OpNotifyAfterPengChi(ChairID, CurChairID, gang, stNotify);

    if (CurChairID == ChairID)
    {
        if ((m_bUserOperate[ChairID] & TYPE_LISTEN) && !isMingLou(ChairID))
        {
            m_bUserOperate[ChairID] |= TYPE_MINGLOU;
            stNotify.set_optype(m_bUserOperate[ChairID]);
        }
    }
}
void Table_jianliZFB::Proto_Game_UserOutCard(_uint8 ChairID, _uint8 bCard, proto::game::AckUserOutCard &stUseroutcard)
{
    Table::Proto_Game_UserOutCard(ChairID, bCard, stUseroutcard);
    stUseroutcard.set_da_type(m_showcardState[ChairID]);
}
void Table_jianliZFB::Proto_Game_UserGetCard(_uint8 ChairID, _uint8 MoChairID, _uint8 bCard, proto::game::AckUserGetCard &stUsergetcard)
{
    Table::Proto_Game_UserGetCard(ChairID, MoChairID, bCard, stUsergetcard);
    if ((m_bUserOperate[ChairID] & TYPE_LISTEN) && !isMingLou(ChairID))
    {
        m_bUserOperate[MoChairID] |= TYPE_MINGLOU;
        stUsergetcard.set_optype(m_bUserOperate[MoChairID]);
    }
}

void Table_jianliZFB::recv_client_outcard(Client *client, Player *player)
{
    proto::game::ReqOutCard stOutCard;
    stOutCard.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);

    int bOutChairID = player->seatid;
    _uint8 da_type = stOutCard.showcard();
    m_showcardState[bOutChairID] = da_type;
    if (da_type == 1)
    {
        m_minglouState[bOutChairID] = 1;
    }

    Table::recv_client_outcard(client, player);
}

void Table_jianliZFB::GameEnd()
{
    //摸完最后一张牌才算是结束
    MJ_user_buffer UserID;
    _uint8 bDraw = (MultiHu(UserID) == 0) ? 1 : 0; //结束状态，1表示流局，0表示胡牌结束

    proto::game::jianlimj::AckCurGameEnd stCurGameEnd;
    proto::game::AckGameEnd *stGameend = stCurGameEnd.mutable_gameend();
    HuScoreSet hu_score;

    if (bDraw)
    {
        Score_GameEndDraw(hu_score, *stGameend);
    }
    else
    {
        Score_GameEndSucc(hu_score, *stGameend);
    }
    stGameend->set_endstate(bDraw);

    Proto_GameEndInfo_AddFan(UserID, *stGameend);

    int UserScore[GAME_PLAYER] = {0};
    for (int i = 0; i < m_GAME_PLAYER; i++)
    {
        proto::game::HandCards *pHandcard = stGameend->add_usercard();
        Fill_HandCard(i, pHandcard);

        Proto_GameEndInfo_GangInfo(i, *stGameend);

        UserScore[i] = hu_score[i].iTotleScore;

        stGameend->add_score(UserScore[i]);
    }

    log.fatal("%s tid:%d bDraw:%d score[%d,%d,%d,%d]\n", __FUNCTION__, tid, bDraw, UserScore[0], UserScore[1], UserScore[2], UserScore[3]);

    tagUserScore userscore[GAME_PLAYER];
    UpdatePlayerInfo(userscore, UserScore, *stGameend);

    stGameend->set_ts((int)time(NULL));

    broadcast(SERVER_GAME_END, stCurGameEnd);
    //发送录像
    save_video_data(SERVER_GAME_END, stCurGameEnd.SerializeAsString());

    update_user_info_to_datasvr_dz(userscore, 1102);

    //数据统计
    CalcRecordInfo(UserID, UserScore);

    RedisUpdateScoreToServer(UserScore);

    RecordScore(m_nCurrentCount);

    if (m_nCurrentCount >= m_nTotalCount)
    {
        CalcContinueWinTimes();
        SaveEndRecordVideo();
    }

    ev_timer_again(zjh.loop, &send_videodata_timer);

    m_last_Banker = m_bBanker;
    //轮庄
    TurnBanker();
    //初始化数据
    GameEndReset();
}

void Table_jianliZFB::GameEndReset()
{
    Table::GameEndReset();

    memset(m_minglouState, -1, sizeof(m_minglouState));
    memset(m_showcardState, -1, sizeof(m_showcardState));

    for (int i = 0; i < m_GAME_PLAYER; i++)
    {
        if (m_piaoScore[i] <= 0)
        {
            m_piaoScore[i] = -1;
        }
    }
}

int Table_jianliZFB::dispatch_client(int cmd, Client *client, Player *player)
{
    int ret = Table::dispatch_client(cmd, client, player);
    if (ret > 0)
        return 0;

    switch (cmd)
    {
    case CLIENT_CHOOSE_PIAO:
    {
        recv_client_piao(client, player);
        return 0;
    }
    default:
        break;
    }
    return 0;
}

void Table_jianliZFB::recv_client_piao(Client *client, Player *player)
{
    _uint8 chairid = player->seatid;

    if (m_piaoScore[chairid] > 0)
    {
        log.error("%s tid:%d error, uid:%d chairid:%d already piao \n", __FUNCTION__, tid, player->uid, chairid);
        return;
    }

    proto::game::jianlimj::ReqChoosePiao stReqChoosePiao;
    stReqChoosePiao.ParseFromArray(client->m_Buffer->proto_data, client->m_Buffer->proto_head.length);
    int piaoNum = stReqChoosePiao.piaonum();

    if (!m_canPiao.Find(piaoNum))
    {
        log.error("%s roomId:%d error, piaonum:%d not found \n", __FUNCTION__, tid, piaoNum);
        return;
    }

    m_piaoScore[chairid] = piaoNum;

    proto::game::jianlimj::AckBroadPiao stAckBroadPiao;
    stAckBroadPiao.set_chairid(chairid);

    broadcast(SERVER_BROAD_CHOOSE_PIAO, stAckBroadPiao);
    save_video_data(SERVER_BROAD_CHOOSE_PIAO, stAckBroadPiao.SerializeAsString());

    //判断是否全部选飘
    int count = 0;
    for (int i = 0; i < m_GAME_PLAYER; i++)
    {
        if (m_piaoScore[i] >= 0)
            count++;
    }

    if (count == m_GAME_PLAYER)
    {
        proto::game::jianlimj::AckChooseOver stAckChooseOver;
        for (int i = 0; i < m_GAME_PLAYER; i++)
        {
            stAckChooseOver.add_piaonum(m_piaoScore[i]);
        }
        broadcast(SERVER_BROAD_RESULT_PIAO, stAckChooseOver);
        save_video_data(SERVER_BROAD_RESULT_PIAO, stAckChooseOver.SerializeAsString());

        //庄家开始摸排
        OnUserGetCard(m_bBanker);
        m_bGameState = GAME_PLAYING;
    }
}
bool Table_jianliZFB::isMingLou(_uint8 Chaird)
{
    if (m_minglouState[Chaird] == 1)
    {
        return true;
    }
    return false;
}

void Table_jianliZFB::startPiao()
{
    m_bGameState = GAME_PIAO;
    //广播开始选票
    proto::game::jianlimj::AckStartPiao stAckStartPiao;

    for (int i = 0; i < m_canPiao.Size(); i++)
    {
        stAckStartPiao.add_piaonum(m_canPiao[i]);
    }

    broadcast(SERVER_BROAD_START_PIAO, stAckStartPiao);
    save_video_data(SERVER_BROAD_START_PIAO, stAckStartPiao.SerializeAsString());
}
