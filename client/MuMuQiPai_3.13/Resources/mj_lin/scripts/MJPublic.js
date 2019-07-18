var MJPublic = cc.Layer.extend({
    tag: "MJPublic",
    btns: {},
    rootNode:null,
    power: null,
    code: null,
    jushu: null,
    time: null,
    roomConfig: null,
    roomConfig2: null,
    bg_up: null,
    bg_down: null,
    layout_hide: null,

    cardNumView:null,
    cardNumText:null,

    ctor: function () {
        this._super();
        this.initPublic();
    },

    initPublic: function () {
        var size = cc.director.getWinSize();
        var uiJson = ccs.load(getResPath("RoomMJ/StudioUI/Public/gameHead.json"));
        var uiNode = uiJson.node;
        uiNode.setPosition(size.width / 2, size.height);
        this.addChild(uiNode);
        this.rootNode = uiNode;

        this.schedule(this.updatePower, 60.0);
        this.schedule(this.updateTime, 60.0);

        var self = this;
        this.code = uiNode.getChildByName("code");
        this.code.ignoreContentAdaptWithSize(true);
        this.jushu = uiNode.getChildByName("jushu");
        this.jushu.ignoreContentAdaptWithSize(true);
        this.time = uiNode.getChildByName("time");
        this.time.ignoreContentAdaptWithSize(true);
        this.power = uiNode.getChildByName("power");
        this.roomConfig = uiNode.getChildByName("roomConfig");
        this.roomConfig.ignoreContentAdaptWithSize(true);
        this.roomConfig.setString("");
        this.roomConfig2 = uiNode.getChildByName("roomConfig2");
        this.roomConfig2.ignoreContentAdaptWithSize(true);
        this.roomConfig2.setString("");
        this.bg_up = uiNode.getChildByName("bg_up");
        this.bg_down = uiNode.getChildByName("bg_down");

        this.cardNumView = uiNode.getChildByName("card_num_bg");
        this.cardNumView.setPosition(uiNode.convertToNodeSpace(cc.p(100 , size.height - 55)));
        this.cardNumText = this.cardNumView.getChildByName("card_num");
        this.cardNumText.ignoreContentAdaptWithSize(true);
        this.cardNumText.setString(0);
        this.showCardNum(-1);

        if(!MJModel.isOnVideo){
            var roomId = "房间号:" + MJModel.curRoomID;
            this.code.setString(roomId);

            this.updateJuShu();
            this.updateTime();
            this.updatePower();

            var _listener_end = cc.EventListener.create({
                event: cc.EventListener.CUSTOM,
                eventName: "notify_game_check_end",
                callback: function (evt) {
                    self.updateJuShu();
                }
            });
            if (_listener_end) cc.eventManager.addListener(_listener_end, this);
        }else {
            this.time.setVisible(false);
            this.power.setVisible(false);
            this.rootNode.getChildByName("netTip").setVisible(false);
        }

        //btn
        {
            this.btns = [];
            var btnLayer = uiNode.getChildByName("btns");
            var cs = btnLayer.getChildren();
            for (var i in cs) {
                var btn = cs[i];
                if (btn) {
                    btn.addClickEventListener(function (sender, evt) {
                        self.actionCallBack(sender);
                    });
                    this.btns[btn.getName()] = btn;
                }
            }
        }

        var _listener_config = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "notify_mjpublic_roomConfig",
            callback: function (evt) {
                var msg = evt.getUserData();
                self.updateRoomConfig(msg);
            }
        });
        if (_listener_config) cc.eventManager.addListener(_listener_config, this);

        var layout_hide = new ccui.Layout();
        layout_hide.setContentSize(size);
        layout_hide.setAnchorPoint(cc.p(0.0, 0));
        layout_hide.setPosition(0, 0);
        layout_hide.setTouchEnabled(true);
        layout_hide.setSwallowTouches(false);
        layout_hide.setVisible(false);
        layout_hide.addClickEventListener(function (sender, type) {
            self.hide();
        });
        MJModel.table.addChild(layout_hide, 1000000);
        this.layout_hide = layout_hide;

        //this.updateRoomConfig("封顶40,不能炮胡,自摸加4番,点炮三家给钱,自摸加4番,点炮三家给钱,不能炮胡,自摸加4番,点炮三家给钱,自摸加4番,点炮三家给钱,不能炮胡,自摸加4番,点炮三家给钱,自摸加4番,点炮三家给钱,不能炮胡,自摸加4番,点炮三家给钱,自摸加4番,点炮三家给钱");
    },

    showCardNum:function(num){
        if(num >= 0){
            this.cardNumView.setVisible(true);
            this.cardNumText.setString(num);
        }else {
            this.cardNumText.setString(0);
            this.cardNumView.setVisible(false);
        }
    },

    showDelay:function () {
        this.updateNetDelay(MJModel.netDely);
    },

    updateNetDelay:function (delay) {
        var netTip = this.rootNode.getChildByName("netTip");
        var netDelay = netTip.getChildByName("netDelay");
        netDelay.ignoreContentAdaptWithSize(true);
        netTip.setVisible(true);
        if(delay > 500){
            delay = 500;
        }
        if(delay < 200){
            netTip.setTexture(getResPath("RoomMJ/mj_public/common/head/net4.png"));
        }else if(delay < 300){
            netTip.setTexture(getResPath("RoomMJ/mj_public/common/head/net3.png"));
        }else if(delay < 400){
            netTip.setTexture(getResPath("RoomMJ/mj_public/common/head/net2.png"));
        }else {
            netTip.setTexture(getResPath("RoomMJ/mj_public/common/head/net1.png"));
        }
        netDelay.setString(delay + "ms");
    },

    updateTime: function (dt) {
        var date = new Date(time(null));
        var str = Utils.pad(date.getHours(), 2) + ":" + Utils.pad(date.getMinutes(), 2);
        this.time.setString(str);
    },

    updatePower: function (dt) {
        //var powerState = PlatformHelper.getPowStateSig();
        var powerState = 50;
        if (powerState != "") {
            var json = JSON.parse(powerState);
            var level = json["level"];
            for (var i = 1; i <= 3; i++) {
                this.power.getChildByName("power" + i).setVisible(false);
            }
            if (level > 80) {
                this.power.getChildByName("power3").setVisible(true);
            } else if (level > 40 && level <= 80) {
                this.power.getChildByName("power2").setVisible(true);
            } else {
                this.power.getChildByName("power1").setVisible(true);
            }
        }
    },

    updateJuShu: function () {

    },

    showJuShuTip: function () {
        this.jushu.setString("牌局:" +MJModel.currentCount + "/" + MJModel.RoomCountSum);
    },

    showRoomCode:function (code) {
        var roomId = "房间号:" + code;
        this.code.setString(roomId);
        this.code.ignoreContentAdaptWithSize(true);
    },

    updateRoomConfig: function (msg) {
        var ss = Utils.splitStr(msg, 35, 2);
        this.roomConfig.setString(ss[0]);
        this.roomConfig.ignoreContentAdaptWithSize(true);
        if (ss[1]) {
            this.roomConfig2.setString(ss[1]);
            this.roomConfig2.ignoreContentAdaptWithSize(true);
            this.roomConfig2.setVisible(false);
            this.btns["down"].setVisible(true);
            this.btns["up"].setVisible(false);
            this.bg_down.setVisible(false);
            this.bg_up.setVisible(true);
            if (MJModel.isOnVideo) {
                this.roomConfig2.setVisible(true);
                this.btns["down"].setVisible(false);
                this.btns["up"].setVisible(false);
                this.bg_down.setVisible(true);
                this.bg_up.setVisible(false);
                this.layout_hide.setVisible(false);
            }
        }
    },

    show: function () {
        this.roomConfig2.setVisible(true);
        this.btns["down"].setVisible(false);
        this.btns["up"].setVisible(true);
        this.bg_down.setVisible(true);
        this.bg_up.setVisible(false);
        this.layout_hide.setVisible(true);
    },
    hide: function () {
        this.roomConfig2.setVisible(false);
        this.btns["down"].setVisible(true);
        this.btns["up"].setVisible(false);
        this.bg_down.setVisible(false);
        this.bg_up.setVisible(true);
        this.layout_hide.setVisible(false);
    },
    actionCallBack: function (btn) {
        var name = btn.getName();
        if (name == "down") {
            this.show();
        } else if (name == "up") {
            this.hide();
        } else if (name == "pifu") {
        }
    },

    onExit: function () {
        CCHttpAgent.getInstance().popPackets("room_status1");
        this._super();
    }
});

var EndRoomLayer = cc.Layer.extend({
    tag: "EndRoomLayer",
    _cur_time: 0,
    _all_time: 0,
    _ownerid: 0,
    _bg: null,
    items: {},
    _time_loading: null,
    _time_label: null,
    btns: {},
    initWithJsonStr: function (data) {
        cc.log("data:" + data);
        /*

         json["ownerid"] = player.uid;
         json["ownername"] = player.name;
         json["time_left"] = time;
         json["players"] = [];
         for (var i = 0; i < jpacket.onlinechairid.length; i++) {
         var seatid = jpacket.onlinechairid[i];
         var p = MJModel.players[seatid];
         var d = {}
         d["uid"] = p.uid;
         d["name"] = p.name + "";
         d["avatar"] = p.avatar;
         d["state"] = 0;
         json["players"].push(d);
         }
         */
        var self = this;
        var size = cc.director.getWinSize();

        var layout = new ccui.Layout();
        layout.setContentSize(size);
        layout.setAnchorPoint(cc.p(0, 0));
        layout.setPosition(cc.p(0, 0));
        layout.setTouchEnabled(true);
        layout.setSwallowTouches(true);
        layout.addClickEventListener(function (btn) {
        });
        this.addChild(layout);

        var uiJson = ccs.load(getResPath("RoomMJ/mj_public/jiesanfangjian.json"));
        var rootNode = uiJson.node;
        this._bg = rootNode.getChildByName("jiesantoupiao");
        this._bg.removeFromParent();
        this._bg.setPosition(size.width / 2, size.height / 2);
        this.addChild(this._bg);
        var json = JSON.parse(data);

        this._ownerid = json["ownerid"];

        this._bg.getChildByName("tip1").setString("玩家 " + json["ownername"] + " 正在申请解散房间，全部同意后房间会被解散");
        this._bg.getChildByName("tip1").ignoreContentAdaptWithSize(true);

        this._time_loading = this._bg.getChildByName("time_loading");
        this._time_label = this._bg.getChildByName("time_label").getChildByName("text");
        this._time_label.ignoreContentAdaptWithSize(true);

        //init button
        {
            var btns_node = this._bg.getChildByName("btns");
            var cs = btns_node.getChildren();
            for (var i in cs) {
                var btn = cs[i];
                if (btn) {
                    btn.addClickEventListener(function (sender, evt) {
                        self.btnsCallBack(sender);
                    });
                    this.btns[btn.getName()] = btn;
                }
            }
        }

        var allL = json["players"].length;
        if (allL <= 0)return;
        var useLayer = null;
        var pos = [];
        if (allL % 2 != 0) {
            useLayer = this._bg.getChildByName("player3");
            if (allL == 1) {
                pos.push(1);
            } else if (allL == 3) {
                pos.push(0);
                pos.push(1);
                pos.push(2);
            }
        } else {
            useLayer = this._bg.getChildByName("player4");
            if (allL == 2) {
                pos.push(1);
                pos.push(2);
            } else if (allL == 4) {
                pos.push(0);
                pos.push(1);
                pos.push(2);
                pos.push(3);
            }
        }

        var realIndex = 0;
        for (var i = 0; i < json["players"].length; i++) {
            var uid = json["players"][i]["uid"];
            if (uid == this._ownerid) {
                // continue;
                json["players"][i]["state"] = 3;
            }
            var item = this.addItem(json["players"][i]["avatar"], json["players"][i]["name"]);
            var state = json["players"][i]["state"];
            this.items[uid] = item;
            var item_node = useLayer.getChildByName("player" + pos[realIndex]);
            item_node.addChild(item);
            realIndex++;
            if (state != 0) {
                this.updateState(uid, state);
                continue;
            }
            // if (state == 2) {
            //     this.updateState(uid, 2);
            //     continue;
            // }

            if (uid == ZJHModel.getInstance().uid) {
                this.showChoose(true);
            } else {
                //this.updateState(uid, 0);
            }
        }

        this._cur_time = json["time_left"];
        this._all_time = json["time_all"];
        this.showTimer();
    },

    btnsCallBack: function (sender) {
        var name = sender.getName();
        if (name == "close") {
            this.removeFromParent();
        } else if (name == "ok") {
            sendEndRoomResult(1);
        }
        else if (name == "no") {
            sendEndRoomResult(2);
        }
    },

    addItem: function (avatars, names) {
        var uiJson = ccs.load(getResPath("RoomMJ/mj_public/jiesanfangjian_item.json"));
        var node = uiJson.node;
        var avatar = node.getChildByName("avatar");
        var m_pAvatar = Utils.createCircleAvatar(avatars, "Avatars/user4_unlogin.png", "Avatars/user4_unlogin.png", cc.size(85, 85));
        avatar.removeAllChildren();
        avatar.addChild(m_pAvatar);
        node.getChildByName("name").setString(Utils.parseName(6 , names));
        node.getChildByName("name").ignoreContentAdaptWithSize(true);
        return node;
    },
    showTimer: function () {
        if (this._cur_time > 0) this.schedule(this.updateTime, 1.0);
        this._time_loading.setPercent(this._cur_time / this.getMaxTime() * 100);
        this._time_label.setString(this._cur_time);
    },

    getMaxTime:function () {
        if (this._all_time > 0) return this._all_time;
        return 90;
    },

    updateTime: function (dt) {
        this._cur_time--;
        if (this._cur_time >= 0) {
            this._time_loading.setPercent(this._cur_time / this.getMaxTime() * 100);
            this._time_label.setString(this._cur_time);
            if (this._cur_time == 0) {
                this.unschedule(this.updateTime);
            }
        }
    },

    showChoose: function (v) {
        this.btns["ok"].setVisible(v);
        this.btns["no"].setVisible(v);
    },

    updateState: function (uid, action) {
        try {
            if (uid == this._ownerid) {
                action = 3;
            }

            if (uid == ZJHModel.getInstance().uid) {
                this.showChoose(false);
            }

            var item = this.items[uid];
            if (item) {
                item.getChildByName("state0").setVisible(false);
                item.getChildByName("state1").setVisible(false);
                item.getChildByName("state2").setVisible(false);
                item.getChildByName("state3").setVisible(false);
                item.getChildByName("state" + action).setVisible(true);
            }
        } catch (e) {

        }
    },
    setCallback: function (callback) {
    },
    setDefaultTips: function (tips) {

    }
});

var MJRecordDetail = cc.Layer.extend({

    recordList: null,

    headIcons:[],
    scoreTexts:[],

    ctor: function () {
        this._super();

        var self = this;
        var size = cc.director.getWinSize();
        var layout = new ccui.Layout();
        layout.setContentSize(size);
        layout.setAnchorPoint(cc.p(0.5, 0.5));
        layout.setPosition(size.width / 2, size.height / 2);
        layout.setTouchEnabled(true);
        layout.setSwallowTouches(true);
        layout.addClickEventListener(function (btn) {
            self.removeFromParent();
        });
        this.addChild(layout, -2);

        var uiJson = ccs.load(getResPath("RoomMJ/mj_public/zhanjiBox.json"));
        var uiNode = uiJson.node;
        var bg = uiNode.getChildByName("bg");
        bg.removeFromParent();
        bg.setPosition(size.width / 2, size.height / 2);

        var bgSize = bg.getContentSize();

        var layout1 = new ccui.Layout();
        layout1.setContentSize(bgSize);
        layout1.setAnchorPoint(cc.p(0.5, 0.5));
        layout1.setPosition(size.width / 2, size.height / 2);
        layout1.setTouchEnabled(true);
        layout1.setSwallowTouches(true);
        layout1.addClickEventListener(function (btn) {
        });
        this.addChild(layout1, -1);

        this.addChild(bg);
        this._bg = bg;

        this.initUI();

        this.addUIClickEvt("Button_1")
    },

    initUI: function () {
        this.recordList = this.getUI("ListView" , this._bg);
        this.recordList.setScrollBarEnabled(false);

        this.headIcons[0] = this.getUI("head1" , this._bg);
        this.headIcons[1] = this.getUI("head2" , this._bg);
        this.headIcons[2] = this.getUI("head3" , this._bg);
        this.headIcons[3] = this.getUI("head4" , this._bg);

        this.scoreTexts[0] = this.getUI("total0" , this._bg);
        this.scoreTexts[1] = this.getUI("total1" , this._bg);
        this.scoreTexts[2] = this.getUI("total2" , this._bg);
        this.scoreTexts[3] = this.getUI("total3" , this._bg);

        if(MJModel.mj_roomType == 1){
            this.headIcons[2].setVisible(false);
            var position = this.headIcons[1].getPosition();
            this.headIcons[1].setPosition(cc.p(position.x + 107.5 , position.y));

            this.scoreTexts[2].setVisible(false);
            var position = this.scoreTexts[1].getPosition();
            this.scoreTexts[1].setPosition(cc.p(position.x + 107.5 , position.y));
        }

        if (MJModel.mj_roomType == 2) {
            this.headIcons[1].setVisible(false);
            this.headIcons[3].setVisible(false);

            this.scoreTexts[1].setVisible(false);
            this.scoreTexts[3].setVisible(false);
        }

        for(var i = 0 ; i < 4 ; i++){
            var pos = i;
            if (MJModel.mj_roomType == 1) {
                if (pos == 2) {
                    continue;
                }
            }

            if (MJModel.mj_roomType == 2) {
                if (pos == 1 || pos == 3) {
                    continue;
                }
            }

            this.updateAvatar(pos);
        }
    },

    updateAvatar: function (pos) {
        var headIcon = this.headIcons[pos];
        var seatid = MJModel.getSeatidByPos(pos);
        var player = MJModel.players[seatid];

        var headPic = Utils.createCircleAvatar(player.avatar, getResPath("RoomMJ/mj_public/common/dialog/zhanji/head_default.png"), "Avatars/user4_unlogin.png", cc.size(85, 85));
        Utils.findNode(headIcon , "head").addChild(headPic);

        var nameText = Utils.findNode(headIcon , "text");
        nameText.ignoreContentAdaptWithSize(true);
        if(player.name != ""){
            Utils.findNode(headIcon , "nameBg").setVisible(true);
            nameText.setString(Utils.parseName(6, player.name));
        }else {
            Utils.findNode(headIcon , "nameBg").setVisible(false);
        }

    },

    getUI:function(key) {
        return  Utils.findNode(this._bg,key);
    },

    addUIClickEvt:function(key) {
        var self = this;
        var ui = this.getUI(key);

        if(ui) {
            ui.addClickEventListener(function (sender, evt) {
                switch (key) {
                    case "Button_1":{
                        self.removeFromParent(true);
                        break;
                    }
                }
            });
        }
    },

    updateScore: function (pos , totalScore) {
        var scoreText = this.scoreTexts[pos];
        scoreText.ignoreContentAdaptWithSize(true);
        var score = totalScore;
        scoreText.setTextColor(cc.color(0xca, 0x58, 0x17));
        if(score < 0){
            scoreText.setTextColor(cc.color(0x65, 0x65, 0x65));
            scoreText.setString("-" + Math.abs(score));
        }else if(score == 0){
            scoreText.setString("0");
        }else {
            scoreText.setString("+" + Math.abs(score));
        }
    },

    updateRecordList: function (records) {
        var totalScore = [0 , 0 , 0 , 0];
        for(var i = 0 ; i < records.length ; i++){
            var item = new RecordDetailItem();
            var scores = records[i].Score;
            item.updateScore(scores , i+1);
            this.recordList.addChild(item);
            for(var j = 0 ; j < scores.length ; j++){
                totalScore[j] += scores[j];
            }
        }

        for(var i = 0 ; i < 4 ; i++){

            var pos = i;
            if (MJModel.mj_roomType == 1) {
                if (pos == 2) {
                    continue;
                }
            }

            if (MJModel.mj_roomType == 2) {
                if (pos == 1 || pos == 3) {
                    continue;
                }
            }

            var seatId = MJModel.getSeatidByPos(pos);
            this.updateScore(pos , totalScore[seatId]);
        }

    },
});

var RecordDetailItem = ccui.Layout.extend({
    uiNode: null,
    uiJson: null,

    title: null,
    scoreItems:[],

    item_bg:null,

    ctor: function () {
        this._super();

        this.uiJson = ccs.load(getResPath("RoomMJ/mj_public/zhanjiBox_item.json"));
        this.uiNode = this.uiJson.node;
        this.addChild(this.uiNode);

        var size = this.uiNode.getContentSize();
        this.setContentSize(size);

        this.title = this.getUI("Text_1");
        this.scoreItems[0] = this.getUI("Text_2");
        this.scoreItems[1] = this.getUI("Text_3");
        this.scoreItems[2] = this.getUI("Text_4");
        this.scoreItems[3] = this.getUI("Text_5");
        this.item_bg = this.getUI("item_bg");

        if(MJModel.mj_roomType == 1){
            this.scoreItems[2].setVisible(false);
            var position = this.scoreItems[1].getPosition();
            this.scoreItems[1].setPosition(cc.p(position.x + 107.5 , position.y));
        }

        if (MJModel.mj_roomType == 2) {
            this.scoreItems[1].setVisible(false);
            this.scoreItems[3].setVisible(false);
        }
    },

    updateScore: function (round , index){
        this.title.ignoreContentAdaptWithSize(true);
        this.title.setString(__String.createWithFormat("第%1局" , index));
        this.item_bg.setTexture(index%2 == 0 ? getResPath("RoomMJ/mj_public/common/dialog/zhanji/item_bg1.png") : getResPath("RoomMJ/mj_public/common/dialog/zhanji/item_bg2.png"))
        for(var i = 0 ; i < 4; i++){
            var pos = i;

            if (MJModel.mj_roomType == 1) {
                if (pos == 2) {
                    continue;
                }
            }

            if (MJModel.mj_roomType == 2) {
                if (pos == 1 || pos == 3) {
                    continue;
                }
            }

            this.scoreItems[pos].ignoreContentAdaptWithSize(true);

            var seatId = MJModel.getSeatidByPos(pos);

            var score = round[seatId];
            this.scoreItems[pos].setTextColor(cc.color(0xca, 0x58, 0x17));
            if(score < 0){
                this.scoreItems[pos].setTextColor(cc.color(0x65, 0x65, 0x65));
                this.scoreItems[pos].setString("-" + Math.abs(score));
            }else if(score == 0){
                this.scoreItems[pos].setString("0");
            }else {
                this.scoreItems[pos].setString("+" + Math.abs(score));
            }
        }
    },

    getUI: function (key, node) {
        if (node == undefined) {
            node = this.uiNode;
        }
        return node.getChildByName(key);
    },
});

var MenuList = cc.Layer.extend({
    tag: "MJMore",
    btns: {},
    bg:null,
    ctor: function () {
        this._super();

        this.init_ui();

        //解决进房，退出和解散按钮刷新问题
        var self = this;
        var _listener_end = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "notify_game_check_end",
            callback: function (evt) {
                self.btns["exit"].setVisible(false);
                self.btns["jieshan"].setVisible(true);
            }
        });
        if (_listener_end) cc.eventManager.addListener(_listener_end, this);

        this.show();
    },

    init_ui:function() {
        var size = cc.director.getWinSize();

        var self = this;
        var layout = new ccui.Layout();
        layout.setContentSize(size);
        layout.setAnchorPoint(cc.p(0, 0));
        layout.setPosition(cc.p(0, 0));
        layout.setTouchEnabled(true);
        layout.setSwallowTouches(true);
        layout.addClickEventListener(function (btn) {
            self.hide();
        });
        this.addChild(layout);

        var uiJson = ccs.load(getResPath("RoomMJ/mj_public/more.json"));
        var uiNode = uiJson.node;
        uiNode.setPosition(cc.p(size.width , 0));
        this.bg = uiNode.getChildByName("bg");
        this.addChild(uiNode);

        var self = this;
        //btn
        {
            this.btns = [];
            var btnLayer = this.bg.getChildByName("btns");
            var cs = btnLayer.getChildren();
            for (var i in cs) {
                var btn = cs[i];
                if (btn) {
                    btn.addClickEventListener(function (sender, evt) {
                        self.actionCallBack(sender);
                    });
                    this.btns[btn.getName()] = btn;
                }
            }

            if (true) {
                this.btns["exit"].setVisible(true);
                this.btns["jieshan"].setVisible(false);
            } else {
                this.btns["exit"].setVisible(false);
                this.btns["jieshan"].setVisible(true);
            }
        }
    },

    show:function () {
        this.bg.setPosition(cc.p(299 , 0));
        this.bg.runAction(cc.sequence(cc.moveTo(0.17 , cc.p(0 , 0))));
    },

    hide:function () {
        var self = this;
        this.bg.runAction(cc.sequence(cc.moveTo(0.17 , cc.p(299 , 0)) , cc.callFunc(function () {
            self.removeFromParent(true);
        })));
    },


        actionCallBack: function (btn) {
            this.hide();
            var name = btn.getName();
            if (name == "exit") {
                MJModel.isLogOut = true;
                sendLogout();
                this.removeFromParent(true);
            } else if (name == "help") {
            var help = new HelpBox();
            help.setName("helpBox");
            MJModel.table.addChild(help, 100000);

        } else if (name == "setting") {
            var setting = new Setting();
            setting.setName("dialog");
            cc.director.getRunningScene().addChild(setting);
        } else if (name == "gps") {
            try {
                var gpsInfo = new PlayerInfo();
                gpsInfo.setName("gpsInfo");
                gpsInfo.updateInfo();
                cc.director.getRunningScene().addChild(gpsInfo);
            } catch (e) {
            }
        } else if (name == "jieshan") {
            sendEndRoomREQ(1);
        } else if(name == "speed"){
            try {
                var speedSet = new SpeedSetting();
                speedSet.setName("dialog");
                cc.director.getRunningScene().addChild(speedSet);
            } catch (e) {
            }
        }
        cc.eventManager.dispatchCustomEvent("notify_mjpublic_msg", "hide");
    },
});

var RoomChat = cc.Layer.extend({
    tag: "RoomChat",
    uiNode: null,
    uiJson: null,

    SendBG: null,
    _editText: null,
    btn_send: null,

    face_layout: null,
    fast_list: null,
    chat_list: null,

    faceBtn: null,
    fastBtn: null,
    chatBtn: null,

    time: 0,

    _listenerChat: null,

    input_tip: null,

    layout_show: null,
    layout_hide: null,

    fast_chat_init: false,
    ctor: function () {
        this._super();

        var self = this;

        var size = cc.director.getWinSize();
        this.setContentSize(size);

        var layout = new ccui.Layout();
        layout.setContentSize(size);
        layout.setAnchorPoint(cc.p(0, 0));
        layout.setTouchEnabled(true);
        layout.setSwallowTouches(true);
        layout.setName("hideLayout");
        layout.addClickEventListener(function (btn) {
            self.hide();
        });
        this.addChild(layout, -2);
        layout.setPosition(cc.p(0, 0));

        this.layout_hide = layout;
        this.layout_hide.setVisible(false);

        var boxSize = cc.size(532, 522);
        var layout = new ccui.Layout();
        layout.setContentSize(boxSize);
        layout.setAnchorPoint(cc.p(0, 0));
        layout.setTouchEnabled(true);
        layout.setSwallowTouches(true);
        layout.addClickEventListener(function (btn) {

        });
        this.addChild(layout);
        layout.setPosition(cc.p(size.width - 550, 70));

        this.initUI();

        var bg = this.getUI("bg");
        this.SendBG = bg;

        this.face_layout = this.getUI("face_list", bg);
        if (Utils.isNewYear()){
            var newyearname = ["xbbn","gxfc","hhh","hyll","jzhs","nnyy","xnkl","zlyj","znbf"];
            for (var i = 0; i < 9; i++) {
                var faceBtn = this.getUI(__String.createWithFormat("face%1", i), this.face_layout);
                var parent = faceBtn.getParent();
                var pos = faceBtn.getPosition();
                faceBtn.removeFromParent();
                faceBtn = new ccui.Button();
                faceBtn.loadTextures(getResPath("RoomMJ/newyear/chatface/icon/" + newyearname[i] + ".png"), "", "");
                faceBtn.setPosition(pos);
                parent.addChild(faceBtn);
                faceBtn.setTag(i + 1 + 2019);
                faceBtn.addClickEventListener(function (btn) {
                    self.faceCallback(btn);
                });
            }
        } else {
            for (var i = 0; i < 9; i++) {
                var faceBtn = this.getUI(__String.createWithFormat("face%1", i), this.face_layout);
                faceBtn.setTag(i);
                faceBtn.addClickEventListener(function (btn) {
                    self.faceCallback(btn);
                });
            }
        }

        this.input_tip = this.getUI("input", bg);
        this.input_tip.setVisible(false);
        //输入框,发言按钮
        this._editText = new cc.EditBox(cc.size(310, 62), new cc.Scale9Sprite(getResPath("RoomMJ/mj_public/common/dialog/chat/input.png")));
        this._editText.setName("input");
        this._editText.setAnchorPoint(cc.p(0, 0.5));
        this._editText.setPosition(this.input_tip.getPosition());
        this._editText.setFontColor(cc.color(0xFF, 0xFF, 0xFF));
        this._editText.setMaxLength(36);
        this._editText.setFontSize(32);
        this._editText.setPlaceholderFontSize(32);
        this._editText.setInputMode(cc.EDITBOX_INPUT_MODE_SINGLELINE);
        this._editText.setReturnType(cc.KEYBOARD_RETURNTYPE_DONE);
        this._editText.setPlaceHolder("  点击输入");
        this._editText.setDelegate(this);
        this.SendBG.addChild(this._editText);

        this.btn_send = this.getUI("send", bg);
        this.btn_send.addClickEventListener(function (btn) {
            self.btnCallback(btn);
        });

        this.fast_list = this.getUI("fast_list", bg);
        this.fast_list.setScrollBarEnabled(false);
        this.fast_list.setVisible(false);

        this.chat_list = this.getUI("record_list", bg);
        this.chat_list.setVisible(false);
        this.chat_list.setScrollBarEnabled(false);

        this.faceBtn = this.getUI("face", bg);
        this.fastBtn = this.getUI("fast", bg);
        this.chatBtn = this.getUI("record", bg);

        this.faceBtn.addClickEventListener(function (sender, evt) {
            self.changeList("face");
        });

        this.fastBtn.addClickEventListener(function (sender, evt) {
            self.changeList("fast");
        });

        this.chatBtn.addClickEventListener(function (sender, evt) {
            self.changeList("chat");
        });

        this.initChatRecord();

        this.changeList("fast");
    },

    initUI : function () {
        var size = cc.director.getWinSize();
        this.uiJson = ccs.load(getResPath("RoomMJ/mj_public/chat.json"));
        this.uiNode = this.uiJson.node;
        this.uiNode.setPosition(cc.p(size.width, 70));
        this.addChild(this.uiNode);
    },

    initFastChat: function () {
        if (this.fast_chat_init == true) {
            return;
        }
        this.fast_chat_init = true;
        var self = this;
        if (MJModel.mj_fastChat.length == 6) {
            var layout = new ccui.Layout();
            layout.setContentSize(cc.size(this.fast_list.getContentSize().width, 25));
            this.fast_list.addChild(layout);
        }
        for (var i = 0; i < MJModel.mj_fastChat.length; i++) {
            var fast = new FastChatItem();
            fast.setFastChat(MJModel.mj_fastChat[i]);
            fast.setTouchEnabled(true);
            fast.setSwallowTouches(true);
            fast.setTag(i);
            fast.addClickEventListener(function (btn) {
                self.fastCallback(MJModel.mj_fastChat[btn.getTag()]);
            });

            this.fast_list.addChild(fast);
        }
    },

    show: function () {
        this.layout_hide.setVisible(true);
        var size = cc.director.getWinSize();
        var pointY = this.uiNode.getPositionY();
        this.uiNode.runAction(cc.sequence(cc.moveTo(0.17, cc.p(size.width - 550, pointY))));
    },

    hide: function () {
        var self = this;
        var size = cc.director.getWinSize();
        var callBack = cc.CallFunc(function () {
            self.removeFromParent();
        });
        var pointY = this.uiNode.getPositionY();
        this.uiNode.runAction(cc.sequence(cc.moveTo(0.17, cc.p(size.width, pointY)) , callBack));
    },

    changeList: function (name) {

        this.faceBtn.setEnabled(true);
        this.faceBtn.getChildByName("icon").setTexture(getResPath("RoomMJ/mj_public/common/dialog/chat/bigface1.png"));
        this.fastBtn.setEnabled(true);
        this.fastBtn.getChildByName("icon").setTexture(getResPath("RoomMJ/mj_public/common/dialog/chat/chat1.png"));
        this.chatBtn.setEnabled(true);
        this.chatBtn.getChildByName("icon").setTexture(getResPath("RoomMJ/mj_public/common/dialog/chat/record1.png"));

        this.fast_list.setVisible(false);
        this.chat_list.setVisible(false);
        this.face_layout.setVisible(false);

        if (name == "fast") {
            this.initFastChat();
            this.fastBtn.setEnabled(false);
            this.fast_list.setVisible(true);
            this.fastBtn.getChildByName("icon").setTexture(getResPath("RoomMJ/mj_public/common/dialog/chat/chat2.png"));
        } else if (name == "chat") {
            this.chatBtn.setEnabled(false);
            this.chat_list.setVisible(true);
            this.chatBtn.getChildByName("icon").setTexture(getResPath("RoomMJ/mj_public/common/dialog/chat/record2.png"));
        } else if (name == "face") {
            this.faceBtn.setEnabled(false);
            this.face_layout.setVisible(true);
            this.faceBtn.getChildByName("icon").setTexture(getResPath("RoomMJ/mj_public/common/dialog/chat/bigface2.png"));
        }
    },

    fastCallback: function (fast) {
        var t = new Date();

        if (this.time != 0) {
            var p = Date.parse(new Date()) - this.time;
            if (p < 2000) {
                return;
            }
        }
        this.time = Date.parse(new Date());
        sendChatReq(fast);
        this.hide();
    },

    btnCallback: function (ref) {
        var n = ref;
        var name = n.getName();
        if (name == "send") {
            //发送
            var send_txt = this._editText.getString();
            if (send_txt == "") return;
            //保存发送的信息
            sendChatReq(send_txt);
            this._editText.setString("");
            this.hide();
        }
    },

    faceCallback: function (ref) {
        var type = ref.getTag();
        sendBigface(MJModel.seatid, MJModel.seatid, type);
        this.hide();
    },

    getUI: function (key, node) {
        if (node == undefined) {
            node = this.uiNode;
        }
        return node.getChildByName(key);
    },

    initChatRecord: function () {
        for (var i = 0; i < MJModel.mj_chatInfoList.length; i++) {
            var msg = MJModel.mj_chatInfoList[i];
            var layout = this.addRecordItem(msg);
            this.chat_list.addChild(layout, 100);
        }
        this.chat_list.refreshView();
        this.chat_list.jumpToBottom();
    },

    addRecordItem: function (msg) {
        var Item = new RoomChat_Item();
        Item.init(msg);
        var layout = new ccui.Layout();
        layout.setContentSize(Item.getContentSize());
        layout.addChild(Item);
        return layout;
    },

    //刷新聊天内容
    notificationChat: function (msg) {
        var p = this.chat_list.getInnerContainerPosition();
        var msg = MJModel.mj_chatInfoList[MJModel.mj_chatInfoList.length - 1];
        var layout = this.addRecordItem(msg);
        this.chat_list.addChild(layout, 100);
        this.chat_list.refreshView();
        if (p.y >= -66) this.chat_list.jumpToBottom();
    },

    onExit: function () {
        this._super();
        try {
            if (this._listenerChat) cc.eventManager.removeListener(this._listenerChat);
        } catch (e) {
        }
    },

    onEnter: function () {
        this._super();
        var self = this;
        //通知刷新消息
        this._listenerChat = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "chat_info_notice",
            callback: function (evt) {
                self.notificationChat(evt);
            }
        });
        if (this._listenerChat) cc.eventManager.addListener(this._listenerChat, this);
    },
});

var FastChatItem = ccui.Layout.extend({
    tag: "FastChatItem",
    uiNode: null,
    uiJson: null,

    fast_tv: null,

    ctor: function () {
        this._super();

        this.uiJson = ccs.load(getResPath("RoomMJ/mj_public/chat_fast_item.json"));
        this.uiNode = this.uiJson.node;
        var bg = this.getUI("fast_bg");
        this.addChild(this.uiNode);
        var size = bg.getContentSize();
        this.setContentSize(size);
        this.uiNode.setPosition(size.width / 2 + 10, size.height / 2);

        this.fast_tv = this.getUI("fast_chat", bg);
        this.fast_tv.ignoreContentAdaptWithSize(true);
    },

    setFastChat: function (fastStr) {
        this.fast_tv.setString(Utils.parseName(26, fastStr));
    },

    getUI: function (key, node) {
        if (node == undefined) {
            node = this.uiNode;
        }
        return node.getChildByName(key);
    },
});

var RoomChat_Item = cc.Layer.extend({
    tag: "RoomChat_Item",
    playerChat: null,
    testBool: true,
    ctor: function () {
        this._super();
    },

    init: function (json) {
        if (json.hasOwnProperty("filepath") && json["filepath"] != null) {
            this.initSound(json);
        } else {
            this.initMsg(json);
        }
    },

    getTextWidth: function () {
        return 280;
    },

    getItemWidth: function () {
        return 400;
    },

    getHeadX: function () {
        return 0;
    },

    initSound: function (msg) {
        var content_layer = new cc.Layer();
        content_layer.setAnchorPoint(cc.p(0.0, 0.0));
        content_layer.setPosition(0, 0);
        this.addChild(content_layer);

        for (var i = 0; i < 4; i++) {
            var player = MJModel.players[i];
            if (player && player.uid == msg["uid"]) {
                this.playerChat = player;
            }
        }
        if (this.playerChat == null) return;
        //玩家头像
        var head_bg = new cc.Sprite(getResPath("RoomMJ/roomchat/head.png"));
        //content_layer.addChild(head_bg);

        head_bg = Utils.createCircleAvatar(this.playerChat.avatar, "Avatars/user4_unlogin.png", getResPath("RoomMJ/roomchat/head.png"), head_bg.getContentSize());
        //headPic.setPosition(cc.p(head_bg.getContentSize().width / 2, head_bg.getContentSize().height / 2));
        //head_bg.addChild(headPic);
        content_layer.addChild(head_bg);

        //文本背景
        var imageView = new ccui.ImageView(getResPath("RoomMJ/roomchat/icon_chat_021.png"));
        imageView.setScale9Enabled(true);
        imageView.setCapInsets(cc.rect(40, 25, 18, 13));

        content_layer.addChild(imageView);

        var text = new cc.Sprite(getResPath("RoomMJ/roomchat/icon_chat_003.png"));
        text.setAnchorPoint(cc.p(0.0, 0.5));
        text.setScale(0.2);
        content_layer.addChild(text);

        var times = null;
        if (msg.hasOwnProperty("time") && msg["time"] != null) {
            times = new cc.LabelTTF(msg["time"] + "\"", "Thonburi", 25);
            times.setAnchorPoint(cc.p(0.0, 0.5));
            content_layer.addChild(times);
        }

        var textSize = text.getBoundingBox();
        imageView.setContentSize(cc.size(textSize.width + 100, textSize.height + 40));

        var head_size = head_bg.getBoundingBox();
        var content_size = cc.size(this.getItemWidth(), imageView.getContentSize().height + 10);
        this.setContentSize(content_size);
        content_layer.setContentSize(content_size);

        head_bg.setAnchorPoint(cc.p(0, 1));
        head_bg.setPosition(cc.p(this.getHeadX(), content_size.height));

        text.setAnchorPoint(cc.p(0.0, 0.5));
        if (times)times.setAnchorPoint(cc.p(0, 0.5));

        imageView.setPosition(cc.p(head_bg.getPositionX() + head_size.width + this.getHeadX(), head_bg.getPositionY() - 10));
        imageView.setAnchorPoint(cc.p(0, 1));
        text.setScale(-0.2);
        text.setPosition(cc.p(imageView.getPositionX() + 70, imageView.getPositionY() - imageView.getContentSize().height / 2 + 5));
        if (times)times.setPosition(text.getPositionX() + textSize.width - 15, text.getPositionY());

        imageView.setTouchEnabled(true);
        imageView.setSwallowTouches(true);
        imageView.addClickEventListener(function (btn) {
            PlatformHelper.playRecord(msg["filepath"]);
        });

        this.playerChat = null;

        var start_notify = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "chat_sound_play_start_notify1",
            callback: function (evt) {
                var file = JsUtils.decodeBase64(evt.getUserData());
                if (file.indexOf(msg["filepath"]) != -1) {
                    var animation = new cc.Animation();
                    for (var i = 1; i <= 3; i++) {
                        animation.addSpriteFrameWithFile(__String.createWithFormat(getResPath("RoomMJ/roomchat/icon_chat_00%1.png"), i));
                    }
                    animation.setDelayPerUnit(0.2);
                    var animate = cc.animate(animation);
                    text.runAction(animate.repeatForever());
                    if (msg.hasOwnProperty("time") && msg["time"] != null) {
                        text.runAction(cc.sequence(cc.delayTime(msg["time"]), cc.CallFunc(function () {
                            text.stopAllActions();
                            text.setTexture(getResPath("RoomMJ/roomchat/icon_chat_003.png"));
                        })));
                    }
                }
            }
        });
        cc.eventManager.addListener(start_notify, this);

        var over_notify = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "chat_sound_play_over_notify1",
            callback: function (evt) {
                var file = JsUtils.decodeBase64(evt.getUserData());
                if (file.indexOf(msg["filepath"]) != -1) {
                    text.stopAllActions();
                    text.setTexture(getResPath("RoomMJ/roomchat/icon_chat_003.png"));
                }
            }
        });
        cc.eventManager.addListener(over_notify, this);
    },
    initMsg: function (msg) {
        //消息跟头像 (文字，图片，语音)
        var content_layer = new cc.Layer();
        content_layer.setAnchorPoint(cc.p(0.0, 0.0));
        content_layer.setPosition(0, 0);
        this.addChild(content_layer);

        for (var i = 0; i < 4; i++) {
            var player = MJModel.players[i];
            if (player && player.uid == msg["uid"]) {
                this.playerChat = player;
                break;
            }
        }
        if (this.playerChat == null) return;
        //玩家头像
        var head_bg = new cc.Sprite(getResPath("RoomMJ/roomchat/head.png"));
        //content_layer.addChild(head_bg);

        head_bg = Utils.createCircleAvatar(this.playerChat.avatar, "Avatars/user4_unlogin.png", getResPath("RoomMJ/roomchat/head.png"), head_bg.getContentSize());
        //headPic.setPosition(cc.p(head_bg.getContentSize().width / 2, head_bg.getContentSize().height / 2));
        //head_bg.addChild(headPic);
        content_layer.addChild(head_bg);

        //文本背景
        var imageView = new ccui.ImageView(getResPath(this.playerChat.uid == ZJHModel.getInstance().uid ? "RoomMJ/roomchat/chat_bg_self.png" : "RoomMJ/roomchat/chat_bg_other.png"));
        imageView.setScale9Enabled(true);
        imageView.setCapInsets(cc.rect(50, 24 + 15, 30, 6));

        content_layer.addChild(imageView);
        var isSingleLine = true;
        var text = new cc.LabelTTF(msg["content"], "Thonburi", 25);
        if (text.getContentSize().width > this.getTextWidth()) {
            text = new cc.LabelTTF(msg["content"], "Thonburi", 25, cc.size(this.getTextWidth(), 0));
            isSingleLine = false;
        }

        text.setColor(cc.color(50, 50, 50));
        content_layer.addChild(text);

        var textSize = text.getBoundingBox();
        imageView.setContentSize(cc.size(textSize.width + 50, textSize.height + 30));

        var head_size = head_bg.getBoundingBox();
        var content_size = cc.size(this.getItemWidth(), imageView.getContentSize().height + 10);
        this.setContentSize(content_size);
        content_layer.setContentSize(content_size);

        head_bg.setAnchorPoint(cc.p(0, 1));
        head_bg.setPosition(cc.p(this.getHeadX(), content_size.height));

        text.setAnchorPoint(cc.p(0.5, 0.5));

        imageView.setPosition(cc.p(head_bg.getPositionX() + head_size.width + this.getHeadX(), content_size.height));
        imageView.setAnchorPoint(cc.p(0, 1));
        text.setPosition(cc.p(imageView.getPositionX() + imageView.getContentSize().width / 2 + 13, imageView.getPositionY() - imageView.getContentSize().height / 2));
        this.playerChat = null;
    },

});

var RealTimeVoicesLayer = cc.Layer.extend({
    tag: "RealTimeVoice",
    playersStates: {},
    playersBtns: [],
    playersEffects: [],
    allState: true,
    myState: true,
    micBtn: null,
    spkBtn: null,
    dataJson: {},
    isJionIn: false,
    ctor: function (players) {
        this._super();
        this.initBtns();
        this.initPlayerSound(players);
    },
    initBtns: function () {
        var size = cc.director.getWinSize();
        var self = this;
        //this.spkBtn = new ccui.CheckBox(getResPath("RoomMJ/liaotianshi/spk_1.png"),
        //    getResPath("RoomMJ/liaotianshi/spk_2.png"));
        //this.spkBtn.setName("spkBtn");
        //this.spkBtn.setAnchorPoint(cc.p(0.5, 0.5));
        //this.addChild(this.spkBtn);
        //this.spkBtn.setPosition(cc.p(size.width - 150 - this.spkBtn.getContentSize().width / 2, size.height - 80));
        //this.spkBtn.addClickEventListener(function (btn) {
        //    var selectedState = btn.isSelected();
        //    self.allState = !selectedState;
        //    try {
        //        PlatformHelper.openMic(self.allState);
        //    } catch (e) {
        //    }
        //});

        //this.micBtn = new ccui.CheckBox(getResPath("RoomMJ/liaotianshi/mic_1.png"),
        //    getResPath("RoomMJ/liaotianshi/mic_2.png"));
        //this.micBtn.setName("micBtn");
        //this.micBtn.setAnchorPoint(cc.p(0.5, 0.5));
        //this.addChild(this.micBtn);
        //this.micBtn.setPosition(cc.p(this.spkBtn.getPositionX() - this.spkBtn.getContentSize().width / 2 - this.micBtn.getContentSize().width / 2 - 10, this.spkBtn.getPositionY()));
        //this.micBtn.addClickEventListener(function (btn) {
        //    var selectedState = btn.isSelected();
        //    self.myState = !selectedState;
        //    try {
        //        PlatformHelper.openSpeaker(self.myState);
        //    } catch (e) {
        //    }
        //});
    },
    initPlayerSound: function (players) {
        if (players == undefined) players = MJModel.table.players;
        var self = this;
        this.playersStates = {};
        this.playersEffects = [];
        this.playersBtns = [];
        var ss = 1.25 * 0.66;

        var super_player_login = MJPlayer.prototype.login;
        MJPlayer.prototype.login = function (seatid) {
            super_player_login.call(this, seatid);
            self.readOneRecord(this.uid);
        };

        for (var i = 0; i < MJModel.Play_num; i++) {
            var spkBtn = new cc.Sprite(getResPath("RoomMJ/liaotianshi/voice_on_1.png"));
            spkBtn.setScale(ss);
            spkBtn.setVisible(false);
            players[i].addChild(spkBtn, 10000);
            var p = this.getInPlayerPos(i);
            spkBtn.setPosition(p);
            this.playersBtns[i] = spkBtn;
            if (i == 1) {
                spkBtn.setScaleX(-ss);
                spkBtn.setScaleY(ss);
                //spkBtn.setTexture(getResPath("RoomMJ/liaotianshi/voice_close.png"));
            }

            var layout = new ccui.Layout();
            layout.setContentSize(spkBtn.getContentSize());
            layout.setAnchorPoint(cc.p(0.5, 0.5));
            layout.setPosition(p);
            layout.setTag(i);
            layout.setVisible(i != 0);
            layout.setTouchEnabled(true);
            layout.setSwallowTouches(true);
            layout.addClickEventListener(function (btn) {
                var tag = btn.getTag();
                var p = MJModel.players[MJModel.getSeatidByPos(tag)];
                if (p.uid <= 0) return;
                self.playersStates[p.uid] = !self.playersStates[p.uid];
                self.updateSomeOneState(p.uid);
                self.saveRecord();
                if (PlatformHelper.muteRemoteAudioStream)PlatformHelper.muteRemoteAudioStream(p.uid, self.playersStates[p.uid]);
                //PlatformHelper.showToast("uid:" + p.uid + " state:" + self.playersStates[tag]);
            });
            players[i].addChild(layout, 10000);

            var spkBtn_effect = new cc.Sprite(getResPath("RoomMJ/liaotianshi/voice_on_1.png"));
            spkBtn_effect.setScale(ss);
            spkBtn_effect.setVisible(false);
            if (i == 1) {
                spkBtn_effect.setScaleX(-ss);
                spkBtn_effect.setScaleY(ss);
            }
            this.playersEffects[i] = spkBtn_effect;
            players[i].addChild(spkBtn_effect, 10000);
            spkBtn_effect.setPosition(p);
            var animation = new cc.Animation();
            for (var j = 1; j <= 3; j++) {
                animation.addSpriteFrameWithFile(__String.createWithFormat(getResPath("RoomMJ/liaotianshi/voice_on_%1.png"), j));
            }
            animation.setDelayPerUnit(0.2);
            var animate = cc.animate(animation);
            spkBtn_effect.runAction(animate.repeatForever());
        }
        //self.readRecord(true);
    },
    getInPlayerPos: function (pos) {
        //if (pos == 2) {
        //    return cc.p(-45 * 0.66, 45 * 0.66);
        //}
        return cc.p(50 * 0.66, -45 * 0.66);
    },
    someonespeaking: function (data) {
        for (var i = 1; i < MJModel.Play_num; i++) {
            this.updateSomeOneState(MJModel.players[i].uid);
        }
        var json = JSON.parse(data);
        for (var i = 0; i < json.length; i++) {
            var uid = parseInt(json[i]);
            var pos = MJModel.getPosByUID(uid);
            if (pos > 0 && pos < MJModel.Play_num && this.playersStates[uid] == false) {
                var player = this.playersBtns[pos];
                var playereffect = this.playersEffects[pos];
                playereffect.setVisible(true);
                player.setVisible(false);
            }
        }
    },
    updateSomeOneState: function (uid) {
        try {
            //this.isJionIn = true;
            var pos = MJModel.getPosByUID(uid);
            if (pos <= 0 || !this.isJionIn)return;
            var player = this.playersBtns[pos];
            var playereffect = this.playersEffects[pos];
            var state = this.playersStates[uid];
            if (state == null) return;
            if (state == true) {
                player.setTexture(getResPath("RoomMJ/liaotianshi/voice_close.png"));
            } else {
                player.setTexture(getResPath("RoomMJ/liaotianshi/voice_on_1.png"));
            }
            playereffect.setVisible(false);
            player.setVisible(true);
        } catch (e) {

        }
    },
    readRecord: function (isInit) {
        if (!this.isJionIn)return;
        for (var j = 0; j < 4; j++) {
            var p = MJModel.players[j];
            var uid = p.uid;
            this.readOneRecord(uid,isInit);
        }
    },
    readOneRecord: function (uid,isInit) {
        if (!this.isJionIn)return;
        if (uid > 0) {
            var v = getItem("realvoice_" + uid);
            var pos = MJModel.getPosByUID(uid);
            if (pos == -1 || v == null)return;
            this.playersStates[uid] = (v == 1 ? true : false);
            this.updateSomeOneState(uid);
            //PlatformHelper.showToast("readRecord:" + p.uid);
            if (PlatformHelper.muteRemoteAudioStream && !isInit)PlatformHelper.muteRemoteAudioStream(uid, this.playersStates[uid]);
        }
    },
    saveRecord: function () {
        cc.log("-------saveRecord-------");
        for (var j = 0; j < 4; j++) {
            var p = MJModel.players[j];
            var uid = p.uid;
            if (uid > 0) {
                var v = this.playersStates[uid];
                var pos = MJModel.getPosBySeatid(j);
                if (pos <= 0)continue;
                setItem("realvoice_" + uid, v ? 1 : 0);
            }
        }
    },
    onEnter: function () {
        this._super();
        // 实时语音监听事件
        var self = this;
        var voiceNotify = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "voice_notify1",
            callback: function (evt) {
                var jsonData = JsUtils.decodeBase64(evt.getUserData());
                var json = JSON.parse(jsonData);
                switch (json["rtcEvent"]) {
                    case 1: // 加入频道回调
                        self.isJionIn = true;
                        self.setVisible(true);
                        for (var i = 1; i < 4; i++) {
                            var btn = self.playersBtns[i];
                            btn.setVisible(true);
                        }
                        MJModel.table.room_Action.hideSound();
                        PlatformHelper.openMic(true); // 别人说话声音开关
                        PlatformHelper.openSpeaker(true); // 自己说话声音开关
                        self.readRecord(false);
                        break;
                    case 2: // 离开频道回调
                        break;
                    case 3: // 其他用户加入当前频道回调
                        break;
                    case 4: // 其他用户离开当前频道回调
                        break;
                    case 5: // 说话提示回调
                        self.someonespeaking(json["uids"]);
                        break;
                    case 6: // 错误码
                        var errorCode = parseInt(json["error_code"]);
                        if (errorCode == 1027) { // 通道被占用
                            PlatformHelper.showToast("录音正在被其他程序使用，请关闭该程序后再尝试!");
                        }
                        if (errorCode == 1019) {// 没有权限
                            PlatformHelper.showToast("没有录音权限，请在设置中添加权限!");
                        }

                        if (errorCode > 0) {
                        }
                        break;
                }

            }
        });
        cc.eventManager.addListener(voiceNotify, this);
    },
    onExit: function () {
        this._super();
        // 退出实时语音
        try {
            PlatformHelper.quitRoom(ZJHModel.getInstance().uid, "");
        } catch (e) {
        }
    },
});