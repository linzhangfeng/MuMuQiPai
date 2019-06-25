var MJTable = cc.Layer.extend({
    tag: "RoomMJTable",
    rootNode: null,
    ctor: function () {
        this._super();
        this.init();
    },

    init: function () {
        this.initData();
        this.updateBg();
    },

    onEnter: function () {
        this._super();
    },
    onExit: function () {
        this.release();
        this._super();

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

        //版本号
        var banbenhao = new cc.LabelTTF("版本号：1", "Arial", 20);
        banbenhao.setPosition(cc.p(size.width * 0.5, size.height * 0.5));
        this.addChild(banbenhao, 100);

        this.schedule(this.updateGame, 0.0);
        this.schedule(this.gameHeartBeat, 3.0);
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
        }
    },
    handler_server_disband_succ_bc: function (data, canDelay) {
        cc.log("handler_server_disband_succ_bc");
        var data = parsePacket(data);
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
    },
    handler_server_ready_succ_bc: function (data, canDelay) {
        cc.log("handler_server_ready_succ_bc");
        var data = parsePacket(data);
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
            var data = SocketClient.getInstance().get();
            if (data) {
                cc.log("recv=data=" + data.cmd)
                cc.log("recv=data=" + data.data)
            }
        } else if (name == "btn_disband") {
            var data = SocketClient.getInstance().get();
            if (data) {
                cc.log("recv=data=" + data.cmd)
                cc.log("recv=data=" + data.data)
            }
        }
    },
    updateBg: function (isInit) {

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
