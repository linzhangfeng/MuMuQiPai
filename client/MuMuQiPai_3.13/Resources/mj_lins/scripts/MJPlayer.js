var MJPlayer = cc.Node.extend({
    tag: "MJPlayer",
    m_pMoney: null,
    m_pMoney_bg: null,
    m_size: null,
    m_pAvatar: null,
    pid: -1,
    uid: 0,
    seatid: -1,
    sec: 0,
    callback: null,
    clickCallback: null,
    banker_flag: null,
    ready_flag: null,
    ready_flag2: null,
    offline_flag: null,
    timeount_flag: null,
    player_black: null,
    name_label: null,
    name_label_bg: null,
    OrderAnim: null,
    m_arrflags: {},
    m_flagTagNames: [],
    FlagTagNames: {
        Flag_Bank: "banker",
        Flag_ok: "ok",
        Flag_ready: "ready",
        Flag_player_black: "player_black",
        Flag_offline: "offline",
        Flag_timeover: "timeover"
    },
    ctor: function () {
        this._super();
        //this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
    },
    login: function (seatid) {
        //try {
            this.seatid = seatid;
            var player = MJModel.players[this.seatid];
            if (!player)return;
            this.uid = player.uid;
            this.name_label.setString(Utils.parseName(6, player.name));
            this.name_label.ignoreContentAdaptWithSize(true);
            this.m_pMoney.ignoreContentAdaptWithSize(true);
            this.updateInfo();

            this.m_pAvatar.removeAllChildren();
            var avatar = Utils.createCircleAvatar(player.avatar, getResPath("RoomMJ/Avatars/user4_unlogin.png"), getResPath("RoomMJ/Avatars/user4_unlogin.png"), cc.size(90, 90));
            avatar.setPosition(cc.p(0, 0));
            this.m_pAvatar.addChild(avatar);

            this.setVisible(true);
            this.setPosition(MJConfig.getPlayerPos(this.pid));

            this.setBankerFlag(false);
            this.setReadyFlag(false);
            this.setTimeOut(false);
            this.setOffline(false);
        //} catch (e) {
        //
        //}
    },

    logout: function (isReset) {
        this.uid = 0;
        this.setVisible(false);
    },

    reset: function () {
        this.setBankerFlag(false);
        this.setReadyFlag(false);
        this.setTimeOut(false);
        this.setOffline(false);
    },

    setId: function (id) {
        this.pid = id;

        var uiJson = ccs.load(getResPath("RoomMJ/StudioUI/Public/player.json"));
        var uiNode = uiJson.node;
        this.addChild(uiNode);
        this.rootNode = uiNode;

        var btn = uiNode.getChildByName("btn");
        btn.setTag(5);
        var self = this;
        btn.addClickEventListener(function (sender, evt) {
            self.playerClickCallback(self);
        });

        this.m_size = btn.getContentSize();
        this.setContentSize(this.m_size);
        uiNode.setPosition(this.m_size.width / 2, this.m_size.height / 2);

        this.name_label = uiNode.getChildByName("name_bg").getChildByName("name");
        this.name_label_bg = uiNode.getChildByName("name_bg");
        this.m_pMoney_bg = uiNode.getChildByName("money_bg");
        this.m_pMoney = uiNode.getChildByName("money_bg").getChildByName("money");
        this.m_pAvatar = uiNode.getChildByName("avatar");


        this.m_flagTagNames = [
            MJPlayer.FlagTagNames.Flag_Bank,
            MJPlayer.FlagTagNames.Flag_ok,
            MJPlayer.FlagTagNames.Flag_offline,
            MJPlayer.FlagTagNames.Flag_timeover,
            MJPlayer.FlagTagNames.Flag_ready,
            MJPlayer.FlagTagNames.Flag_Bank
        ];
        for (var i = 0; i < this.m_flagTagNames.length; i++) {
            this.addFlag(this.m_flagTagNames[i]);
        }
    },
    addFlag: function (tagName) {
        var flag = new cc.Sprite(getResPath("RoomMJ/offline.png"));
        flag.setVisible(false);
        flag.setPosition(this.getCenterFlagPos(this.pid));
        this.addChild(flag, 100);
        this.m_arrflags[tagName] = flag;
    },
    getFlagsPos: function (tagName, pos) {
        switch (tagName){
            case MJPlayer.FlagTagNames.Flag_Bank:
                return this.getBankerFlagPos();
            case MJPlayer.FlagTagNames.Flag_ok:
                return this.getCenterFlagPos();
            case MJPlayer.FlagTagNames.Flag_offline:
                return this.getCenterFlagPos();
            case MJPlayer.FlagTagNames.Flag_player_black:
                return this.getCenterFlagPos();
            case MJPlayer.FlagTagNames.Flag_timeover:
                return this.getCenterFlagPos();
            case MJPlayer.FlagTagNames.Flag_ready:
                return;
        }
    },
    getBankerFlagPos: function (pos) {
        return this.getHeadFlagNodePos(6);
    },
    getReadyFlagPos: function (pos) {
        return this.getHeadFlagNodePos(4);
    },
    getCenterFlagPos: function () {
        return this.getHeadFlagNodePos(1);
    },
    getHeadFlagNodePos: function (dir) {
        //dir  1:中心 2：上面 3：左边 4：右边 6：右上 7：左上
        var flagNodePos = this.rootNode.getChildByName("flagNode").getChildByName("p_" + dir);
        if (flagNodePos) {
            return flagNodePos.getPosition();
        }
        return null;
    },
    getHeadFlagPos: function (tagName) {
        switch (tagName) {
            case "":
                break;
            case "":
                break;
            case "":
                break;
        }
    },
    showMoneyVisible: function (is) {
        this.rootNode.getChildByName("money_bg").setVisible(is);
    },

    reflashNameMoneyPosition: function () {
        if (this.pid == 0) {
            if (MJModel.mj_bg_use == 0) {
                this.rootNode.getChildByName("name_bg").setPosition(cc.p(70, 11));
                this.rootNode.getChildByName("money_bg").setPosition(cc.p(70, -20));
            } else {
                this.rootNode.getChildByName("name_bg").setPosition(cc.p(0, -40));
                this.rootNode.getChildByName("money_bg").setPosition(cc.p(0, -71));
            }
        }
    },


    playerClickCallback: function (ref) {
        if (this.clickCallback) {
            this.clickCallback(this.pid);
        }
    },

    setClickCallback: function (callback) {
        this.clickCallback = callback;
    },

    showBalanceNumber: function (value) {
        if (value == 0) {
            return;
        }

        try {
            var label = null;
            if (value > 0) {
                label = new cc.LabelAtlas(":" + value, getResPath("RoomMJ/dz_balance_num_win.png"), 41, 58, '0');
            } else {
                label = new cc.LabelAtlas(":" + Math.abs(value), getResPath("RoomMJ/dz_balance_num_lose.png"), 41, 58, '0');
            }
            label.setAnchorPoint(cc.p(0.5, 0.5));
            if (this.pid == 2) {
                label.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height / 2));
            }
            else {
                label.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height - 10));
            }

            this.addChild(label, 200);

            label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)), cc.delayTime(3),
                cc.removeSelf()));
        } catch (e) {

        }
    },
    updateInfo: function () {
        var player = MJModel.players[this.seatid];
        if (!player)return;
        var dm = player.money - MJModel.take_in;
        this.m_pMoney.setString(dm);
        //if (dm >= 0) {
        //    this.m_pMoney.setString(dm);
        //} else {
        //    dm = -dm;
        //    this.m_pMoney.setString("" + dm);
        //}
    },

    setBankerFlag: function (v) {
        if (this.banker_flag) this.banker_flag.setVisible(v);
    },

    getBankerFlag: function () {
        if (this.banker_flag) {
            return this.banker_flag.isVisible();
        }
        return false;
    },
    getBankerFlagSprite: function () {
        return this.banker_flag;
    },

    setReadyFlag: function (v) {
        if (MJModel.canQuxiaoZhun()) {
            if (v) {
                if (MJModel.roomState == 0) {
                    if (this.ready_flag2) this.ready_flag2.setVisible(v);
                } else {
                    if (this.ready_flag) this.ready_flag.setVisible(v);
                }
            } else {
                if (this.ready_flag) this.ready_flag.setVisible(v);
                if (this.ready_flag2) this.ready_flag2.setVisible(v);
            }
            return;
        }
        if (this.ready_flag) this.ready_flag.setVisible(v);
    },

    getReadyState: function () {
        var flag = false;
        if (this.ready_flag) flag = this.ready_flag.isVisible();
        var flag2 = false;
        if (this.ready_flag2) flag2 = this.ready_flag2.isVisible();
        var state = flag || flag2 ? 1 : 0;
        return state;
    },

    setOffline: function (v) {
        if (this.offline_flag) this.offline_flag.setVisible(v);
        if (v) this.setTimeOut(false);

        // if (this.player_black) this.player_black.setVisible(v);

        if (v) {
            this.m_pAvatar.setColor(cc.color(0x55, 0x55, 0x55));
        } else {
            this.m_pAvatar.setColor(cc.color(0xFF, 0xFF, 0xFF));
        }
    },

    setTimeOut: function (v) {
        if (v && this.offline_flag && this.offline_flag.isVisible())return;

        if (this.timeount_flag) this.timeount_flag.setVisible(v);

        // if (this.player_black) this.player_black.setVisible(v);

        if (v) {
            this.m_pAvatar.setColor(cc.color(0x55, 0x55, 0x55));
        } else {
            this.m_pAvatar.setColor(cc.color(0xFF, 0xFF, 0xFF));
        }
    },
    showSoundFlag: function () {
        if (!this.soundFlag) {
            this.soundFlag = new cc.Sprite(getResPath("RoomMJ/liaotianshi/spk_1.png"));
            this.addChild(this.soundFlag, 100);
            this.soundFlag.setScale(0.5);
            this.soundFlag.setPosition(cc.p(10, 20));
        } else {
            this.soundFlag.setTexture(getResPath("RoomMJ/liaotianshi/spk_1.png"));
        }
        this.soundFlag.setVisible(true);
    },

    hideSoundFlag: function () {
        if (this.soundFlag) {
            this.soundFlag.setTexture(getResPath("RoomMJ/liaotianshi/spk_2.png"));
        } else {
            this.soundFlag = new cc.Sprite(getResPath("RoomMJ/liaotianshi/spk_2.png"));
            this.addChild(this.soundFlag, 100);
            this.soundFlag.setScale(0.5);
            this.soundFlag.setPosition(cc.p(10, 20));
        }
        this.soundFlag.setVisible(true);
    }
});
MJPlayer.FlagTagNames = {}

