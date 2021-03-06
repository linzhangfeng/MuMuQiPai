var MJAction = cc.Layer.extend({
    tag: "MJAction",
    callback: null,
    uptable: null,
    ready: null,
    ready2: null,
    quxiaoReady: null,
    weiguan: null,
    sound: null,
    tingpai: null,
    varOutCard: null,
    paidui: null,
    actions: {},
    opBtns: [],
    gpsBtn: null,
    btns: {},
    setGPSVisible: function (v) {
        if (this.gpsBtn)this.gpsBtn.setVisible(v);
    },

    ctor: function () {
        this._super();
        var size = cc.director.getWinSize();
        var self = this;

        this.initOpBtns();
        this.initUI();

        this.tingpai = this.btns["tingpai"];
    },

    initOpBtns: function () {
        var opBtnsName = ["chi", "peng", "gang", "hu", "pass"];
        var self = this;
        for (var i = 0; i < opBtnsName.length; i++) {
            var names = opBtnsName[i];
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/" + names + "1.png"), "", "");
            this.actions[names] = btn;
            btn.setName(names);
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
            this.opBtns.push(btn);
        }
    },
    initUI: function () {
        var self = this;
        var uiJson = ccs.load(getResPath("RoomMJ/StudioUI/Public/action.json"));
        var uiNode = uiJson.node;
        uiNode.setPosition(0, 0);
        this.addChild(uiNode);
        var size = cc.director.getWinSize();
        var cs = uiNode.getChildren();
        for (var i in cs) {
            var btn = cs[i];
            if (btn) {
                btn.addClickEventListener(function (sender, evt) {
                    self.actionCallBack(sender);
                });
                this.btns[btn.getName()] = btn;
            }
        }
    },

    updateGPSTip: function (hasnocity, hastoonear) {
        if (hastoonear) {
            this.btns["gps"].loadTextures(getResPath("RoomMJ/mj_public/common/table/gps2.png"), "", "")
        } else if (hasnocity) {
            this.btns["gps"].loadTextures(getResPath("RoomMJ/mj_public/common/table/gps1.png"), "", "")
        } else {
            this.btns["gps"].loadTextures(getResPath("RoomMJ/mj_public/common/table/gps.png"), "", "")
        }
    },

    actionCallBack: function (sender) {
        var name = sender.getName();
        if (name == "tingpai") {
            MJModel.mj_table.showTingPaiLayer();
            return;
        } else if (name == "downtable") {

        } else if (name == "ready1" || name == "ready2") {
            sendReady();
        } else if (name == "chat") {

        } else if (name == "gps") {

        } else if (name == "yaoqing") {

        } else if (name == "copyshare") {

        } else if (name == "sound") {

        }
        if (this.callback) this.callback(name);
    },

    setCallback: function (callback) {
        this.callback = callback;
    },
    //进入新聊天界面
    btnChatBack: function (ref) {
        if (MJModel.chatInfo == null) {
            MJModel.chatInfo = new RoomChat();
            MJModel.mj_table.addChild(MJModel.chatInfo, 100000);
        }
        MJModel.chatInfo.show();
    },

    sureOperate: function (type, card) {
        if (MJModel.needTipWhenPass == 1) {
            var op = MJModel.curOperate;
            if (type == ActionType.TYPE_PASS && (op & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
                this.showPassWarn(type, card, "确定放弃胡牌？");
                return;
            }
        } else if (MJModel.needTipWhenPass == 2) {
            var op = MJModel.curOperate;
            if (type == ActionType.TYPE_PASS && (op & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
                if (MJModel.mj_table.room_Card.isDuoPai(0) || MJModel.gangMoPai) {
                    this.showPassWarn(type, card, "确定放弃胡牌？");
                    return;
                }
            }
        }
        sendOperateCard(type, card);
        this.setOperator(-1);
    },

    showPassWarn: function (type, card, tips) {
        var self = this;
        var checkLayer = new PassWarn();
        checkLayer.setChoiceInfos(false, false, tips);
        checkLayer.setName("checkLayer_selectLayer");
        checkLayer.setCallback(function () {
            sendOperateCard(type, card);
            self.setOperator(-1);
        });
        this.addChild(checkLayer, 10000);
    },

    btnCallback: function (ref) {
        if (MJModel.outtimeplayers.length > 1) {
            MJModel.mj_table.room_Tip.setAutoEndRoomVis(true);
        }
        var n = ref;
        var name = n.getName();
        Log(this, "btnCallback:" + name);
        var self = this;

        if (name == "yaoqing") {
            if (MJModel.shareType == 1) {
                var myposition = cc.p(n.getContentSize().width / 2, n.getContentSize().height / 2);
                var sharelist = new ShareList();
                sharelist.initList(this, 0, myposition);
                n.addChild(sharelist);
                return;
            }
        }
        if (name == "pass" || name == "hu" || name == "gang" || name == "peng" || name == "chi" || name == "huaHu") {
            var type = 0;
            if (name == "pass") {
                type = ActionType.TYPE_PASS;
                var op = MJModel.curOperate;
                var card = MJModel.curOperateCard;
                var hasGang = false;
                var hasHu = false;
                if ((op & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG
                    || (op & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG
                    || (op & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG) {
                    hasGang = true;
                }

                if ((op & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
                    hasHu = true;
                }

                hasGang = false;//暂时不要杠
                if (MJModel.needTipWhenPass == 1 && (hasGang || hasHu)) {
                    var checkLayer = new PassWarn();
                    checkLayer.setChoiceInfos(hasGang, hasHu);
                    checkLayer.setName("checkLayer_selectLayer");
                    checkLayer.setCallback(function () {
                        sendOperateCard(type, card);
                        self.setOperator(-1);
                        self.removeChildByName("checkLayer_selectLayer");
                    });
                    this.addChild(checkLayer, 10000);
                    return;
                }
                sendOperateCard(type, card);
                this.setOperator(-1);
                return;
            }
            if (name == "hu") type = ActionType.TYPE_HU;
            if (name == "peng") type = ActionType.TYPE_PENG;
            if (name == "huaHu") type = ActionType.TYPE_HUA_HU;
            if (name == "gang") {
                var op = MJModel.curOperate;
                if ((op & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG || (op & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG) {
                    if (MJModel.gang_info.length > 1) {
                        var selectLayer = new ChooseGang();
                        selectLayer.setChoiceInfos(MJModel.gang_info);
                        selectLayer.setName("ChooseGang_selectLayer");
                        selectLayer.setTitle("请点击选择要杠的牌");
                        selectLayer.setPosition(cc.p(n.getPosition().x + 0, n.getPosition().y + 50));
                        selectLayer.setCallback(function (index) {
                            self.removeChildByName("ChooseGang_selectLayer");

                            self.sureOperate(MJModel.gang_info[index].state, MJModel.gang_info[index].curCard);
                        });
                        this.addChild(selectLayer, 10000);
                        return;
                    }
                    else if (MJModel.gang_info.length > 0) {
                        MJModel.curOperateCard = MJModel.gang_info[0].curCard;
                    }
                    else {
                        return;
                    }
                }

                type = ActionType.TYPE_ZHIGANG;

                if ((op & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG) {
                    type = ActionType.TYPE_WANGANG;
                }
                else if ((op & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                    type = ActionType.TYPE_ANGANG;
                }
            }

            if (name == "chi") {
                var op = MJModel.curOperate;
                var op_nums = 0;
                var op_type = ActionType.TYPE_LEFT_CHI;
                if ((op & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
                    op_nums++;
                    op_type = ActionType.TYPE_LEFT_CHI;
                }
                if ((op & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
                    op_nums++;
                    op_type = ActionType.TYPE_CENTER_CHI;
                }
                if ((op & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                    op_nums++;
                    op_type = ActionType.TYPE_RIGHT_CHI;
                }

                if (op_nums > 1) {
                    var selectLayer = new ChooseChi();
                    selectLayer.setChoiceInfos(MJModel.curOperate, MJModel.curOperateCard);
                    selectLayer.setName("ChooseGang_selectLayer");
                    selectLayer.setPosition(cc.p(n.getPosition().x, n.getPosition().y + 50));
                    selectLayer.setCallback(function (type) {
                        sendOperateCard(type, MJModel.curOperateCard);
                        self.setOperator(-1);
                        self.removeChildByName("ChooseGang_selectLayer");
                    });
                    this.addChild(selectLayer, 10000);
                    return;
                }
                type = op_type;
            }
            var card = MJModel.curOperateCard;
            sendOperateCard(type, card);
            this.setOperator(-1);
            return;
        }

        if (this.callback) this.callback(name);
    },

    setUpTableVisible: function (v) {
        if (MJModel.isOnVideo)return;
        if (this.uptable) this.uptable.setVisible(v);
    },

    setReadyVisible: function (v) {
        if (MJModel.isOnVideo)return;
        if (this.ready) this.ready.setVisible(v);
        if (v == false && this.ready2)this.ready2.setVisible(false);
    },

    setReady2Visible: function (v) {
        if (MJModel.isOnVideo)return;
        if (this.ready2) this.ready2.setVisible(v);
    },

    setQuxiaoReadyVisible: function (v) {
        if (MJModel.isOnVideo)return;
        if (MJModel.canQuxiaoZhun()) {
            if (this.quxiaoReady) this.quxiaoReady.setVisible(v);
        }
    },

    checkAutoOutCardVis: function () {
        if (MJModel.isOnVideo)return;
        this.setAutoOutCardVis(MJModel.final_listen_info.length >= 1);
    },

    setAutoOutCardVis: function (v) {
        if (MJModel.isOnVideo)return;
        if (this.autoOutCard) {
            this.autoOutCard.setVisible(v);
            if (v == true) {
                this.autoOutCard.getChildByName("ok").setVisible(MJModel.isAutoOutCard);
            }
        }
    },

    isHuOpVis: function () {
        if (this.actions["hu"]) {
            return this.actions["hu"].isVisible();
        }
        return false;
    },

    reflashOpBtns: function () {
        var size = cc.director.getWinSize();
        var bx = this.getOpBtnBX();
        var by = 200;
        var dx = this.getOpBtnWidth();
        var index = 0;

        for (var i = 0; i < this.opBtns.length; i++) {
            var opBtn = this.opBtns[i];
            if (opBtn.isVisible()) {
                opBtn.setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }
        return index;
    },
    getOpBtnBX: function () {
        var size = cc.director.getWinSize();
        var bx = size.width - 184 - 92;
        return bx;
    },
    getOpBtnWidth: function () {
        var count = 0;
        for (var i = 0; i < this.opBtns.length; i++) {
            var opBtn = this.opBtns[i];
            if (opBtn.isVisible()) {
                count++;
            }
        }
        if (count <= 4) {
            return 220;
        } else {
            return 180;
        }
    },

    setOperator: function (operat) {
        this.removeChildByName("ChooseGang_selectLayer");

        if (operat == -1 || operat == 0 || operat == null || operat == undefined) {
            for (var i = 0; i < this.opBtns.length; i++) {
                this.opBtns[i].setVisible(false);
            }
            MJModel.curOperate = -1;
            return false;
        }

        MJModel.curOperate = operat;


        if (this.actions["pass"]) {
            this.actions["pass"].setVisible(true);
        }

        if ((operat & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
            if (this.actions["hu"]) {
                this.actions["hu"].setVisible(true);
            }
        }

        if ((operat & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (operat & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (operat & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
            if (this.actions["gang"]) {
                this.actions["gang"].setVisible(true);
            }
        }

        if ((operat & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
            if (this.actions["peng"]) {
                this.actions["peng"].setVisible(true);
            }
        }

        if ((operat & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI || (operat & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI || (operat & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
            if (this.actions["chi"]) {
                this.actions["chi"].setVisible(true);
            }
        }

        this.reflashOpBtns();
        return true;
    },
    setBaoTingOperator: function (Gangoperat) {
        if (this.actions["pass"]) {
            this.actions["pass"].setVisible(true);
        }


        if ((Gangoperat & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (Gangoperat & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (Gangoperat & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
            if (this.actions["gang"]) {
                this.actions["gang"].setVisible(true);
            }
        }

        if (this.actions["baoting"]) {
            this.actions["baoting"].setVisible(true);
        }

        this.reflashOpBtns();
    },

    setDuiLieStr: function (str) {
        if (!this.paidui) {
            return;
        }
        var paiduitext = this.paidui.getChildByName("paiduitext");
        var paiduibg = this.paidui.getChildByName("paiduibg");
        paiduitext.setString(str);
        paiduibg.setContentSize(cc.size(paiduitext.getContentSize().width + 30, paiduitext.getContentSize().height));
    },

    setPlayerQueueVisible: function (v) {
        var roomPlayerQueue = this.getChildByName("playerqueue_layer");
        if (roomPlayerQueue) {
            roomPlayerQueue.setVisible(v);
        }
    },

    setDuiLieData: function (data) {
        if (!this.paidui) {
            return;
        }
        var str = "不在队列中";

        var status = 0;
        if (data != "") {
            var upTableInfo = parsePacket("proto.login.AckUpTableInfo_UC", data);
            if (upTableInfo.rank == 1) {
                status = 0;
                str = "等待上桌";
            }
            else if (upTableInfo.rank > 1) {
                status = 1;
                str = "队列中";
            }
            else if (MJModel.seatid != -1) {
                str = "游戏中";
                status = 2;
            }
            else {
                str = "不在队列中";
                status = 3;
            }
        }

        this.setDuiLieStr(str);

        var roomPlayerQueue = this.getChildByName("playerqueue_layer");
        if (roomPlayerQueue && roomPlayerQueue.isVisible()) {
            roomPlayerQueue.updateView(data);
            roomPlayerQueue.updateButton(status);
        }
    },

    setDuiLieVisible: function (v) {
        if (this.paidui) {
            this.paidui.setVisible(v);
        }
    },

    checkTingPaiVis: function () {
        this.setTingPaiVis(MJModel.final_listen_info.length >= 1);
    },

    setTingPaiVis: function (v) {
        if (this.tingpai) {
            this.tingpai.setVisible(v);
            if (v == false) MJModel.mj_table.removeChildByName("tingpai_layer");
        }
    },
    checkZuiXiaoHuaVis: function () {
        var zuixiaohuaBtn = this.btns["zuixiaohua"];
        if (MJModel.isSupportZuiXiaoHua && zuixiaohuaBtn && MJModel.isClubRoom) {
            zuixiaohuaBtn.setVisible(true);
        }
    },
    hideSound: function () {
        this.btns["sound"].setVisible(false);
        if (MJModel.mj_table.pre_scene && MJModel.mj_table.pre_scene.btns["sound"]) {
            MJModel.mj_table.pre_scene.btns["sound"].setVisible(false);
        }
    },
});

var ChooseGang = cc.Layer.extend({
    _callback: null,
    title_label: null,
    ctor: function () {
        this._super();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
    },
    setTitle: function (title) {
        this.title_label.setString(title);
    },
    setChoiceInfos: function (choiceInfos) {
        var self = this;
        var size = choiceInfos.length;
        var ddx = 40;
        var card_size = cc.size(83, 125);
        var allW = size * card_size.width + ddx * (size + 1);
        var sp = new ccui.ImageView(getResPath("RoomMJ/bg.9.2.png"));
        sp.setScale9Enabled(true);
        sp.setAnchorPoint(cc.p(0.5, 0.0));
        sp.setContentSize(cc.size(allW, 190));
        sp.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
        this.addChild(sp);

        var bg_size = sp.getContentSize();
        this.setContentSize(bg_size);

        this.title_label = new cc.LabelTTF("请点击选择要操作的牌", "Thonburi", 28);
        this.title_label.setAnchorPoint(cc.p(0.5, 0.5));
        this.title_label.setPosition(cc.p(bg_size.width / 2, bg_size.height - 28));
        this.title_label.setColor(cc.color(255, 255, 255));
        //this.title_label.runAction(cc.sequence(cc.delayTime(1),cc.hide(),cc.delayTime(0.5),cc.show()).repeatForever());
        sp.addChild(this.title_label);

        var bx = ddx;
        for (var j = 0; j < size; j++) {
            var gif = choiceInfos[j];
            var layout = new ccui.Layout();
            layout.setTag(j);
            layout.setAnchorPoint(cc.p(0.5, 0.5));
            layout.setContentSize(card_size);
            sp.addChild(layout);

            var tingCard = new MJCard();
            tingCard.setValue(gif.curCard, 0, CardType.Card_Hand, 0);
            tingCard.setPosition(cc.p(bx + card_size.width / 2, 73));
            tingCard.setAnchorPoint(cc.p(0.5, 0.5));
            tingCard.setScale(card_size.width / tingCard.getContentSize().width * MJConfig.getTipCardScale());
            sp.addChild(tingCard);

            layout.setPosition(tingCard.getPosition());
            layout.setTouchEnabled(true);
            layout.addClickEventListener(function (sender, eve) {
                if (self._callback) self._callback(sender.getTag());
            });

            bx += card_size.width + ddx;
        }
    },

    setCallback: function (callback) {
        this._callback = callback;
    },

    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
    },
    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height)
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchEnded: function (touch, event) {
        if (this._isTouchInside(touch)) {
            return;
        }
        this.removeFromParent(true);
    }
});

var ChooseChi = cc.Layer.extend({
    _callback: null,
    ctor: function () {
        this._super();

        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.0));

    },
    setChoiceInfos: function (curOperate, curOperateCard) {
        var op_type_v = [];
        var op_cards_v = [];
        var op = curOperate;
        var cur_card = curOperateCard;

        if ((op & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
            op_type_v.push(ActionType.TYPE_RIGHT_CHI);
        }
        if ((op & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
            op_type_v.push(ActionType.TYPE_CENTER_CHI);
        }
        if ((op & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
            op_type_v.push(ActionType.TYPE_LEFT_CHI);
        }

        for (var i = 0; i < op_type_v.length; i++) {
            op_cards_v[i] = [];
            var opType = op_type_v[i];
            for (var j = 0; j < 3; j++) {
                var card_value = 0;
                if (opType == ActionType.TYPE_LEFT_CHI) {
                    card_value = cur_card + j;
                }
                if (opType == ActionType.TYPE_CENTER_CHI) {
                    card_value = cur_card + j - 1;
                }
                if (opType == ActionType.TYPE_RIGHT_CHI) {
                    card_value = cur_card + j - 2;
                }
                op_cards_v[i].push(card_value);
            }
            if (MJModel.operator_info && MJModel.operator_info.length > 0) {
                for (var j = 0; j < MJModel.operator_info.length; j++) {
                    if (MJModel.operator_info[j].OpType == opType) {
                        op_cards_v[i] = MJModel.operator_info[j].Cards;
                        break;
                    }
                }
            }
        }
        this.showUI(op_type_v, op_cards_v, cur_card);
    },
    showUI: function (op_type_v, op_cards_v, cur_card) {
        var self = this;
        var op_nums = op_type_v.length;
        var card_size = cc.size(65, 96);
        var item_size = cc.size(65 * 3 + 30, 96 + 20);
        var size = cc.size((item_size.width + 20) * op_nums, item_size.height);
        this.setContentSize(size);

        var bx = size.width / 2 - (item_size.width + 20) / 2 * (op_nums - 1);
        for (var i = 0; i < op_nums; i++) {
            var opType = op_type_v[i];
            var layout = new ccui.Layout();
            layout.setAnchorPoint(cc.p(0.5, 0.5));
            layout.setContentSize(item_size);
            layout.setTag(opType);
            this.addChild(layout);
            layout.setPosition(cc.p(bx + (item_size.width + 20) * i, size.height / 2));
            layout.setTouchEnabled(true);
            layout.addClickEventListener(function (ref, evt) {
                var lay = ref;
                if (self._callback) self._callback(lay.getTag());
            });

            var sp = new ccui.ImageView(getResPath("RoomMJ/bg.9.png"));
            sp.setScale9Enabled(true);
            sp.setAnchorPoint(cc.p(0.5, 0.5));
            sp.setContentSize(item_size);
            sp.setPosition(cc.p(layout.getContentSize().width / 2, layout.getContentSize().height / 2));
            layout.addChild(sp);

            for (var j = 0; j < op_cards_v[i].length; j++) {
                var card_value = op_cards_v[i][j];
                var tingCard = new MJCard();
                tingCard.setValue(card_value, 0, CardType.Card_Hand, 0);
                tingCard.setPosition(cc.p(45 + (card_size.width + 3) * j, item_size.height / 2));
                tingCard.setAnchorPoint(cc.p(0.5, 0.5));
                if (card_value == cur_card) {
                    tingCard.setChoiceTipShow(true);
                }
                tingCard.setScale(card_size.width / tingCard.getContentSize().width * MJConfig.getTipCardScale());
                sp.addChild(tingCard);
            }
        }
    },

    setCallback: function (callback) {
        this._callback = callback;
    },

    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
    },
    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height)
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchEnded: function (touch, event) {
        if (this._isTouchInside(touch)) {
            return;
        }
        this.removeFromParent(true);
    }
});

var TingPaiLayer = cc.Layer.extend({
    tag: "TingPaiLayer",
    ctor: function () {
        Log(this, "TingPaiLayer:init");
        this._super();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0, 0));
        this.setPosition(cc.p(280, 155));

        var self = this;
        var _listenerChat = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "tingpai_info_notice_reflash",
            callback: function (evt) {
                self.reflash();
            }
        });
        if (_listenerChat) cc.eventManager.addListener(_listenerChat, this);
    },
    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: false,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);

        var _listenerChat = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "tingpai_info_notice_reflash",
            callback: function (evt) {
                self.reflash();
            }
        });
        if (_listenerChat) cc.eventManager.addListener(_listenerChat, this);
    },
    getCardSize: function () {
        return cc.size(49, 65);
    },
    getLineH: function () {
        return 115;
    },
    getCardNums: function (vv, sver_num) {
        var allcards = {};
        for (var j = 0; j < MJModel.mj_table.room_Card.hand_card[0].length; j++) {
            try {
                var card = MJModel.mj_table.room_Card.hand_card[0][j];
                Utils.checkCard(allcards, card.getValue());
            } catch (e) {
            }
        }

        for (var i = 0; i < MJModel.mj_table.room_Card.out_card.length; i++) {
            for (var j = 0; j < MJModel.mj_table.room_Card.out_card[i].length; j++) {
                try {
                    var card = MJModel.mj_table.room_Card.out_card[i][j];
                    Utils.checkCard(allcards, card.getValue());
                } catch (e) {

                }
            }
        }

        for (var i = 0; i < MJModel.mj_table.room_Card.hu_card.length; i++) {
            for (var j = 0; j < MJModel.mj_table.room_Card.hu_card[i].length; j++) {
                try {
                    var card = MJModel.mj_table.room_Card.hu_card[i][j];
                    Utils.checkCard(allcards, card.getValue());
                } catch (e) {
                }
            }
        }

        for (var i = 0; i < MJModel.mj_table.room_Card.peng_card.length; i++) {
            for (var j = 0; j < MJModel.mj_table.room_Card.peng_card[i].length; j++) {
                try {
                    var peng = MJModel.mj_table.room_Card.peng_card[i][j];
                    for (var k = 0; k < peng.cardList.length; k++) {
                        var card = peng.cardList[k];
                        if (card.isVisible() == false)continue;
                        var v = card.getValue();
                        if (v == 0) {
                            v = peng.getValue();
                        }
                        if (v == 0)continue;
                        Utils.checkCard(allcards, v);
                    }
                } catch (e) {

                }
            }
        }
        var curN = allcards[vv];
        if (allcards[vv] == undefined)curN = 0;
        var n = 4 - curN;
        if (n < 0) n = 0;
        return n;
    },
    show2: function (list) {
        var size = list.length;
        var v_nums = size;
        if (size > 9 && size < 18) {
            v_nums = parseInt(size / 2);
            if (size % 2 != 0) {
                v_nums += 1;
            }
        } else if (size >= 18) {
            v_nums = 9;
        }

        var h_nums = parseInt(size / v_nums);
        if (h_nums == 0) {
            h_nums = 1;
        }

        if (h_nums * v_nums < size) {
            h_nums += 1;
        }

        var card_size = this.getCardSize();
        var dy = this.getLineH();
        var dx = card_size.width + 10;
        var sp = new ccui.ImageView(getResPath("RoomMJ/bg.9.png"));
        sp.setScale9Enabled(true);
        sp.setAnchorPoint(cc.p(0.5, 0.5));
        sp.setContentSize(cc.size(dx * v_nums + 64, dy * h_nums));
        sp.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
        this.addChild(sp);

        var listenTip = new cc.Sprite(getResPath("RoomMJ/listen_tip.png"));
        listenTip.setPosition(cc.p(32, this.getLineH() * h_nums * 0.5));
        sp.addChild(listenTip);

        this.setContentSize(sp.getContentSize());

        var bx = 64 + card_size.width / 2;
        var by = dy / 2;
        this.allLabel = [];
        for (var j = 0; j < size; j++) {
            var mi = parseInt(j % v_nums);
            var mj = parseInt(j / v_nums);
            var huinfo = list[j];

            var tingCard = new MJCard();
            tingCard.setValue(huinfo.Card, 0, CardType.Card_Hand, 0);
            tingCard.setPosition(cc.p(bx + dx * mi, dy * mj + dy / 2 + 10));
            tingCard.setAnchorPoint(cc.p(0.5, 0.5));
            tingCard.setScale(card_size.width / tingCard.getContentSize().width * MJConfig.getTipCardScale());
            sp.addChild(tingCard);

            var s = this.getCardNums(huinfo.Card, huinfo.LeftNum);
            var textlable = new cc.LabelTTF(s + "张", "Thonburi", 20);
            textlable.setPosition(cc.p(tingCard.getPositionX(), dy * mj + 17));
            textlable.setAnchorPoint(cc.p(0.5, 0.5));
            textlable.setColor(cc.color(0xb9, 0xff, 0x66, 255));
            textlable.setTag(huinfo.Card);
            sp.addChild(textlable);
            this.allLabel.push(textlable);
        }
    },
    allLabel: [],
    reflash: function () {
        try {
            for (var i in this.allLabel) {
                var label = this.allLabel[i];
                var s = this.getCardNums(label.getTag(), 0);
                label.setString(s + "张");
            }
        } catch (e) {

        }
    },

    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height);
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchEnded: function (touch, event) {
        if (this._isTouchInside(touch)) {
            return;
        }
        if (MJModel.mj_listenLayer_autoshow || MJModel.mj_listenLayer_need_autoshow)return;
        this.removeFromParent(true);
    }

});
var RealTimeVoices = cc.Layer.extend({
    micBtn: null,
    spkBtn: null,

    ctor: function () {
        this._super();
        var csize = cc.size(200, 200);

        this.setIgnoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0));
        this.setContentSize(csize);

        this.micBtn = new ccui.CheckBox(getResPath("RoomMJ/liaotianshi/mic_1.png"),
            getResPath("RoomMJ/liaotianshi/mic_2.png"));
        this.micBtn.setName("micBtn");
        this.micBtn.setAnchorPoint(cc.p(0.5, 0.5));
        this.addChild(this.micBtn);
        this.micBtn.setScale(0.75);
        this.micBtn.setPosition(cc.p(csize.width / 2 - this.micBtn.getContentSize().width / 3 + 2, csize.height / 2));

        this.spkBtn = new ccui.CheckBox(getResPath("RoomMJ/liaotianshi/spk_1.png"),
            getResPath("RoomMJ/liaotianshi/spk_2.png"));
        this.spkBtn.setName("spkBtn");
        this.spkBtn.setAnchorPoint(cc.p(0.5, 0.5));
        this.addChild(this.spkBtn);
        this.spkBtn.setScale(0.75);
        this.spkBtn.setPosition(cc.p(csize.width / 2 + this.spkBtn.getContentSize().width / 3 - 3, csize.height / 2));
    },

    addClickEventListener: function (callback) {
        this.micBtn.addClickEventListener(callback);
        this.spkBtn.addClickEventListener(callback);
    },
});