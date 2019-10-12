var MJTable = cc.Layer.extend({
    tag: "RoomMJTable",
    rootNode: null,
    roomPublic: null,
    roomCard: null,
    roomAction: null,
    players: {},
    ctor: function () {
        this._super();
        MJModel.table = this;

        this.init();
    },

    init: function () {
        this.updateBg();
        this.initRoom();
        this.reflashSetting(true);
        this.initData();
    },
    initRoom: function () {
        this.roomPublic = new MJPublic();
        this.addChild(this.roomPublic, CommonOrder.BEST_TOP);

        this.roomAction = new MJAction();
        this.addChild(this.roomAction, CommonOrder.BEST_TOP);

        MJModel.initData();
        //this.schedule(this.test, 1.0);
    },
    onEnter: function () {
        this._super();
    },
    onExit: function () {
        this.release();
        this._super();
    },

    test: function (dt) {
        MJModel.testCount++;
        var tempCount = MJModel.testCount;
        if (tempCount == 5 && true) {
            //测试上桌消息
            var data = {
                seatId:1,
                uid:10173,
                name:"我是一颗小虎牙",
                money:1,
                ready:1,
                sex:1,
                avatar:""
            };
            this.handler_server_up_table_succ_bc(JSON.stringify(data),false);
        }
    },

    initData: function () {
        var self = this;
        var size = cc.director.getWinSize();
        var uiJson = ccs.load(getResPath("RoomMJ/StudioUI/GameTable/MJTable.json"));
        var rootNode = uiJson.node;
        rootNode.setPosition(0, 0);
        this.addChild(rootNode);
        this.rootNode = rootNode;

        var btn_test_back = Utils.findNode(this.rootNode, "btn_test_back");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        var btn_test_back = Utils.findNode(this.rootNode, "btn_connect");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        var btn_test_back = Utils.findNode(this.rootNode, "btn_send");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        var btn_test_back = Utils.findNode(this.rootNode, "btn_recv");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        var btn_test_back = Utils.findNode(this.rootNode, "btn_disband");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        var btn_test_back = Utils.findNode(this.rootNode, "btn_exitRoom");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        //版本号
        var banbenhao = new cc.LabelTTF("版本号：1", "Arial", 20);
        banbenhao.setPosition(cc.p(size.width * 0.5, size.height * 0.5));
        this.addChild(banbenhao, 100);

        this.schedule(this.updateGame, 0.0);
        this.schedule(this.gameHeartBeat, 3.0);

        this.initPlayers();

    },
    initPlayers: function () {
        //桌上玩家初始化
        var self = this;
        for (var i = 0; i < MJModel.playNum; i++) {
            var player = new MJPlayer();
            player.setVisible(false);
            player.setId(i);
            player.setPosition(MJConfig.getPlayerPos(i));
            this.addChild(player, 10);
            this.players[i] = player;
            player.setClickCallback(function (pos) {
                self.playerClickCallback(pos);
            });
        }
    },
    updatePlayers: function () {
        for (var i = 0; i < MJModel.playNum; i++) {
            var player = MJModel.players[i];
            if (player.uid > 0) {
                var pos = MJModel.getPosBySeatid(i);
                var mjPlayer = this.players[pos];
                mjPlayer.login(i);
            }
        }
    },
    reflashSetting: function (isInit) {
        this.updateBg(isInit);
        this.loadConfig("RoomMJ/config_2d/card_config2d.json");
        this.loadCardSprites();

    },
    loadCardSprites: function () {
        Utils.addCardSprites(__String.createWithFormat("RoomMJ/cards/jianjie_dahao_2d_debug", MJModel.font, MJModel.fontSize, MJModel.tableView));
    },
    loadConfig: function (path) {
        try {
            Log(this, "loadCardConfig:" + path);
            var uiNode = this.getChildByName(path);
            if (uiNode == null) {
                var uiJson = ccs.load(getResPath(path));
                var uiNode = uiJson.node;
                Utils.doLayer(uiNode);
                try {
                    Utils.doLayer(uiNode.getChildByName("player"));
                    Utils.doLayer(uiNode.getChildByName("op"));
                    Utils.doLayer(uiNode.getChildByName("outting"));
                } catch (e) {
                    Log(this, e);
                }
                uiNode.setName(path);
                uiNode.setVisible(false);
                this.addChild(uiNode, -1000);
            }
            MJModel.configNode = uiNode;
        } catch (e) {
            Log(this, e);
            MJModel.table.release();
            ZJHModel.getInstance().roomToHall();
            ERROR(this, "loadCardConfig error: uid" + ZJHModel.getInstance().uid + e);
            PlatformHelper.showToast("文件加载出错，请重新进入房间!");
        }
    },
    gameHeartBeat: function () {
        sendHeartbeat();
    },
    reConnect: function (tips, showTip, reconnectTime) {
        cc.log("---------------reConnect---------------");
        if (tips == null || tips == undefined) {
            tips = "";
        }

        SocketClient.getInstance(MJModel.curRoomID).reset_response_queue();
        SocketClient.getInstance(MJModel.curRoomID).connect_req();
    },
    updateGame: function (dt) {
        var response = SocketClient.getInstance(MJModel.curRoomID).get();

        if (response) {
            // Log(this, "cmd=" + response.cmd + " id=" + response.id + " data=" + response.data);
            if (response.cmd == SOCKETCMD.DISCONNECT_RES) {
                this.reConnect()
                return;
            }
            else if (response.cmd == SOCKETCMD.RECV_DATA_OK_RES) {
                this.handler_cmd(response.id, response.data, true, response.seqNo);
            }
            response.release();
        } else {
        }
    },
    resetGame: function () {

    },
    handler_cmd: function (cmd, data, canDelay) {
        cc.log("handler_cmd=" + cmd);
        switch (cmd) {
            case  CMD.SERVER_CONNECT_SUCC_BC:
                this.handler_server_connect_succ_bc(data, canDelay);
                break;
            case  CMD.SERVER_LOGIN_SUCC_UC:
                this.handler_server_login_succ_bc(data, canDelay);
                break;
            case  CMD.SERVER_LOGOUT_SUCC_BC:
                this.handler_server_logout_succ_bc(data, canDelay);
                break;
            case  CMD.SERVER_READY_SUCC_BC:
                this.handler_server_ready_succ_bc(data, canDelay);
            case  CMD.SERVER_DISBAND_ROOM_SUCC_BC:
                this.handler_server_disband_succ_bc(data, canDelay);
                break;
            case CMD.SERVER_UPTABLE_SUCCC_BC:
                this.handler_server_up_table_succ_bc(data, canDelay);
                break;
            case CMD.SERVER_DOWNTABLE_SUCCC_BC:
                this.handler_server_down_table_succ_bc(data, canDelay);
                break;
            case CMD.SERVER_GAME_SCENE:
                //先处理当前缓存的 tableData
                //this.handler_server_table_info_uc(this.tableData);
                //var ackGameFree = parsePacket("proto.game.AckGameScene", jpacket);
                this.handler_server_scene_info_uc(data, canDelay);
                break;
            case CMD.SERVER_TABLE_INFO_UC:
                this.handler_server_table_info_uc(data, canDelay);
                break;
        }
    },
    handler_server_scene_info_uc: function (data, canDelay) {
        cc.log("handler_server_scene_info_uc");
        var data = parsePacket(data);

    },
    handler_server_table_info_uc: function (data, canDelay) {
        cc.log("handler_server_table_info_uc");
        var data = parsePacket(data);

        this.resetGame();

        for (var i = 0; i < MJModel.playNum; i++) {
            var playerUI = this.players[i];
            playerUI.logout(true);
        }

        var playersInfo = data.playersInfo;
        for (var i = 0; i < playersInfo.length; i++) {
            var playData = playersInfo[i];
            var uId = playData.userId;
            var seatId = playData.seatId;
            if (uId == UserModel.getInstance().getUid()) {
                MJModel.seatId = seatId;
            }
            var player = MJModel.players[seatId];
            player.seatId = seatId;
            player.uid = playData.uid;
            player.name = playData.name;
            player.money = playData.money;
            player.ready = playData.ready;
            player.sex = playData.sex;
            player.avatar = playData.avatar;
        }
    },
    handler_server_down_table_succ_bc: function (data, canDelay) {
        cc.log("handler_server_down_table_succ_bc");
        //保存玩家数据，
        var data = parsePacket(data);
        var seatid = data.seatid;
        var player = MJModel.players[seatid];
        player.reset();
        player.seatid = seatid;
        player.uid = data.uid;
        player.name = data.name;
        player.money = data.money;
        player.ready = data.ready;
        player.sex = data.sex;
        player.avatar = data.avatar;

        //显示玩家UI

    },
    handler_server_up_table_succ_bc: function (data, canDelay) {
        cc.log("handler_server_up_table_succ_bc");
        var data = parsePacket(data);
        var seatId = data.seatId;
        var playerData = MJModel.players[seatId];
        playerData.reset();
        playerData.seatId = seatId;
        playerData.uid = data.uid;
        playerData.name = data.name;
        playerData.money = data.money;
        playerData.ready = data.ready;
        playerData.sex = data.sex;
        playerData.avatar = data.avatar;

        if (playerData.uid == UserModel.getInstance().getUid()) {

        } else {
            var pos = MJModel.getPosBySeatid(seatId);
            var player = this.players[pos];
            player.login(seatId);
        }
    },
    handler_server_disband_succ_bc: function (data, canDelay) {
        cc.log("handler_server_disband_succ_bc");
        var data = parsePacket(data);
        if (data.state == 1) {
            this.exitRoom(true);
        }
    },
    handler_server_connect_succ_bc: function (data, canDelay) {
        cc.log("handler_server_connect_succ_bc");
        var data = parsePacket(data);
        sendLogin();
    },
    handler_server_login_succ_bc: function (data, canDelay) {
        cc.log("handler_server_login_succ_bc");
        var data = parsePacket(data);
    },
    handler_server_logout_succ_bc: function (data, canDelay) {
        cc.log("handler_server_logout_succ_bc");
        var data = parsePacket(data);
        if (data.seatId == 1) {
            this.release();
            CommonModel.getInstance().toHall();
        }
    },
    handler_server_ready_succ_bc: function (data, canDelay) {
        cc.log("handler_server_ready_succ_bc");
        var data = parsePacket(data);
    },
    showTingPaiLayer: function () {
        try {
            var selectLayer = MJModel.table.getChildByName("tingpai_layer");

            if (selectLayer) {
                selectLayer.removeFromParent(true);
                return;
            }

            var can_show_listen_info = true;
            for (var j = 0; j < MJModel.final_listen_info.length; j++) {
                if (MJModel.final_listen_info[j].Card == 255) {
                    can_show_listen_info = false;
                    break;
                }
            }

            if (can_show_listen_info) {
                if (MJModel.final_listen_info.length > 0) {
                    selectLayer = new TingPaiLayer();
                    selectLayer.show2(MJModel.final_listen_info);
                    selectLayer.setName("tingpai_layer");
                    MJModel.table.addChild(selectLayer, 100);//一定要在MJAction下边
                }
            } else {
                PlatformHelper.showToast("见字胡不显示听牌张");
            }

        } catch (e) {
            Log(this, e);
        }
    },
    exitRoom: function (isCloseHall) {
        this.release();
        sendLogout();
        if (isCloseHall) {
            CommonModel.getInstance().toHall();
        }
    },
    startSocket: function () {

    },
    btnCallback: function (ref) {
        var name = ref.getName();
        cc.log("btnCallback=" + name);
        if (name == "btn_test_back") {
            CommonModel.getInstance().toHall();
        } else if (name == "btn_connect") {
            SocketClient.getInstance().connect_req();
        } else if (name == "btn_send") {
            var data = {};
            data.cmd = CMD.CLIENT_LOGIN_REQ;

        } else if (name == "btn_recv") {
            var data = SocketClient.getInstance().get();
            if (data) {
                cc.log("recv=data=" + data.cmd)
                cc.log("recv=data=" + data.data)
            }
        } else if (name == "btn_exitRoom") {
            sendEndRoomREQ(1)
        } else if (name == "btn_disband") {
            sendEndRoomREQ(2)
        }
    },
    updateBg: function () {
        var size = cc.director.getWinSize();
        var bg = this.getChildByName(MJTable.TagName.tag_bg_name);
        if (bg == null) {
            bg = new ccui.ImageView(getResPath(this.getBgStr()));
            bg.setAnchorPoint(cc.p(0.5, 0.5));
            bg.setContentSize(size);
            bg.setScale9Enabled(true);
            bg.setPosition(size.width / 2, size.height / 2);
            bg.setName(MJTable.TagName.tag_bg_name);
            this.addChild(bg, -100);
        } else {
            bg.loadTexture(getResPath(this.getBgStr()));
        }
    },
    getBgStr: function () {
        return "RoomMJ/bgs/huaijiu_2d.png";
    },
    release: function () {
        //SoundModel.getInstance().can_play = 1;
        this.unscheduleAllCallbacks();
        ccs.ArmatureDataManager.destroyInstance();
        cc.spriteFrameCache.removeSpriteFrames();
        cc.textureCache.removeAllTextures();
        jsb.fileUtils.purgeCachedEntries();
    },
});
MJTable.TagName = {
    tag_bg_name: "tag_bg_name",
}