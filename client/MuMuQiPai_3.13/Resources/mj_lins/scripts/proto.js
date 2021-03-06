var CMD =
    {
        //服务端
        SERVER_LOGIN_SUCC_UC:           4000,//登录成功
        SERVER_LOGOUT_SUCC_BC:          4001,//登出成功
        SERVER_READY_SUCC_BC:           4002,//准备成功
        SERVER_CONNECT_SUCC_BC:         4003,//准备成功
        SERVER_DISBAND_ROOM_SUCC_BC:    4004,//解散房间成功
        SERVER_USER_OFFLINE_BC:         4005,//广播玩家掉线
        SERVER_UPTABLE_SUCCC_BC:        4006,//上桌成功
        SERVER_DOWNTABLE_SUCCC_BC:      4007,//下桌成功
        SERVER_TABLE_INFO_UC:           4008,//桌面信息
        SERVER_GAME_SCENE:              4009,//游戏场景

        //客户端请求
        CLIENT_LOGIN_REQ:           1001, //登录请求
        CLIENT_LOGOUT_REQ:          1002, //登出请求
        CLIENT_READY_REQ:           1003, //准备请求
        CLIENT_DISBAND_ROOM_REQ:    1004, //解散请求
        CLINET_HEART_BEAT_REQ:      1005, //心跳请求
        CLIENT_DISBAND_RESULT_REQ:  1004, //解散请求
    };
