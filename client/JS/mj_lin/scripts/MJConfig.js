
var SEX_ID =
{
    SEX_MAN: 1,
    SEX_FEMALE: 2,
};

var MJ_TYPE =
{
    MJ_TYPE_EMPTY: -1, //
    MJ_TYPE_WAN: 0, //万,0-8,各4张，共36张
    MJ_TYPE_TIAO: 1, //条,0-8,各4张，共36张
    MJ_TYPE_TONG: 2, //筒,0-8,各4张，共36张
};

var PengType =
{
    Peng_Peng: 0,
    Peng_Kang: 1,
    Peng_AnKang: 2,
    Chi_L: 3,
    Chi_M: 4,
    Chi_R: 5,
};

var CardType =
{
    Card_Hand: 0,
    Card_Out: 1,
    Card_Peng: 2,
    Card_Hu: 3,
    Card_Outing: 4,
    Card_Mo: 5,
    Card_End: 6,
    Card_Public: 7,
};

var MJ_STATUS =
{
    MJ_GAME_FREE: 0,				//空闲场景
    MJ_GAME_CHANGECARD: 1,		//换牌场景
    MJ_GAME_CHOOSE_LACKTYPE: 2,	//选择定缺色场景
    MJ_GAME_PLAYING: 3,			//游戏中场景
    END_GAME: 4,
};

var tagHuInfo =
{
    Card: 0,//听的牌
    LeftNum: 0,//剩余张数
    HuFanNum: 0,//胡牌番数
};

var tagListenInfo =
{
    OutCard: 0,	//要打出的牌
    HuInfoLen: 0,
    HuInfo: [],//打出牌后的胡牌信息
};

var tagGangInfo =
{
    curCard: 0,	//杠的牌
    state: 0,      //杠的类型(弯杠 暗杠)
};

var tagOpInfo =
{
    OpType: 0,	//杠的牌
    Cards: [],      //杠的类型(弯杠 暗杠)
};

var ActionType =
{
    TYPE_NULL: 0x00,
    TYPE_PENG: 0x01,
    TYPE_ZHIGANG: 0x02,
    TYPE_WANGANG: 0x04,
    TYPE_ANGANG: 0x08,
    TYPE_HU: 0x10,
    TYPE_LISTEN: 0x20,
    TYPE_PASS: 0xFF,
    TYPE_BAOTING: 0x40,
    TYPE_LEFT_CHI: 0x40,
    TYPE_CENTER_CHI: 0x80,
    TYPE_RIGHT_CHI: 0x100,
    TYPE_HUA_HU: 0x200,
    TYPE_BUZHANG: 0x400,//补张
    TYPE_HAID_YAO: 0x800,//要海底牌
    TYPE_JINHUAN_PAI: 0x1000,//禁换手牌

    TYPE_ShangTing: 0x2000,//上听操作
    TYPE_BAO_TING: 0x4000, //报听操作


    JITYPE_CHONGFENGJI: 1,
    JITYPE_ZERENJI: 2,

    INVALID_CHAIR: 0xFF
};

//听牌类型
var LISTEN_TYPE =
{
    LISTEN_CARD_NOMAL: 0,					//常规听牌处理
    LISTEN_CARD_BAOTING: 1,				//报听
    LISTEN_CARD_BAIPAI: 2,				//摆牌
    LISTEN_CARD_FIRST_BAOPAI: 3,			//首张报牌
};

var SOCKETCMD =
{
    CONNECT_OK_RES: 0,
    CONNECT_ERROR_RES: 1,
    SEND_DATA_OK_RES: 2,
    SEND_DATA_ERROR_RES: 3,
    RECV_DATA_OK_RES: 4,
    RECV_DATA_ERROR_RES: 5,
    DISCONNECT_RES: 6,
};

var MJConfig =
{
    getTablePos: function () {
        var size = cc.director.getWinSize();
        return cc.p(size.width / 2, size.height / 2);
    },

    getPlayerChatPos: function (pos) {
        var p = this.getPlayerPos(pos);
        if (pos == 0){
            p.y += 50;
        }else if (pos == 1){
            p.y += 50;
        }
        else if (pos == 2){

        }
        else if (pos == 3){
            p.y += 50;
        }
        return p;
    },
    getPlayerPos: function (index) {
        var point = this.getWorldPosition("player", "player" + index);
        if(index == 3 && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
            point.y -= 75;
        }
        return point;
    },

    getOutCardPos: function (pos, index) {
        if (MJModel.mj_roomType == 2) {
            index = index%58;
            return this.getWorldPosition("out_" + pos, "o_" + index);
        }

        if (MJModel.mj_bg_use == 1) {
            if(pos == 0 || pos == 2){
                index = index%26;
            }else {
                if(MJModel.mj_paiban == "vertical"){
                    index = index%28;
                }else {
                    index = index%30;
                }
            }
        } else {
            if(MJModel.has_hu_layout == 1){
                index = index%22;
            }else {
                index = index%26;
            }
        }
        if((pos == 1 || pos == 3) && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
            return this.getWorldPosition("out_v_" + pos, "o_" + index);
        }else {
            return this.getWorldPosition("out_" + pos, "o_" + index);
        }
    },

    getOutCardOrder: function (pos, index) {
        if (MJModel.mj_roomType == 2) {
            index = index%58;
            var order = null;
            if (MJModel.mj_bg_use == 1) {
                order = [
                    [
                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    ],
                    [
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    ],
                    [
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                    ],
                    [
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    ],
                ];
                return order[pos][index];
            } else {
                if (pos == 0) {
                    return index%58;
                } else if (pos == 2) {
                    return 57 - index%58;
                }
                return 0;
            }
        }


        if (MJModel.mj_bg_use == 1) {
            if(pos == 0 || pos == 2){
                index = index%26;
            }else {
                if(MJModel.mj_paiban == "vertical"){
                    index = index%28;
                }else {
                    index = index%30;
                }
            }
            var order = null;
            if((pos == 1 || pos == 3) && MJModel.mj_paiban == "vertical"){
                order = [
                    [
                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        1, 1, 1, 1, 1, 1,
                        0, 0, 0, 0, 0, 0,
                    ],
                    [
                        6, 5, 4, 3, 2, 1, 0,
                        6, 5, 4, 3, 2, 1, 0,
                        6, 5, 4, 3, 2, 1, 0,
                        6, 5, 4, 3, 2, 1, 0,
                    ],
                    [
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        2, 2, 2, 2, 2, 2,
                        3, 3, 3, 3, 3, 3,
                    ],
                    [
                        0, 1, 2, 3, 4, 5, 6,
                        0, 1, 2, 3, 4, 5, 6,
                        0, 1, 2, 3, 4, 5, 6,
                        0, 1, 2, 3, 4, 5, 6,
                    ],
                ];
            }else {
                order = [
                    [
                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        1, 1, 1, 1, 1, 1,
                        0, 0, 0, 0, 0, 0,
                    ],
                    [
                        9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                        9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                        9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                    ],
                    [
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        2, 2, 2, 2, 2, 2,
                        3, 3, 3, 3, 3, 3,
                    ],
                    [
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                    ],
                ];
            }
            return order[pos][index];
        }else{

            if(MJModel.has_hu_layout == 1){
                index = index%22;
            }else {
                index = index%26;
            }

            var order = [
                [
                                0,  1,  2,  3,  4,  5,
                            6,  7,  8,  9,  10, 11, 12, 13,
                    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 , 25,
                ],
                [
                    25 , 24 , 23, 22, 21, 20,
                    19, 18, 17, 16, 15, 14, 13, 12,
                    11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                ],
                [
                    25 , 24 , 23, 22, 21, 20,
                    19, 18, 17, 16, 15, 14, 13, 12,
                    11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                ],
                [
                    20, 21, 22, 23, 24, 25,
                    12 , 13, 14, 15, 16, 17, 18, 19,
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                ],
            ];
            return order[pos][index];
        }
    },

    getInfoPos: function () {
        return MJModel.cardConfig.getChildByName("info").getPosition();
    },

    getDirPos: function (pos) {
        var p = this.getInfoPos();
        if (pos == 0)p.y -= 50;
        if (pos == 1)p.x += 80;
        if (pos == 2)p.y += 50;
        if (pos == 3)p.x -= 80;
        return p;
    },

    getOutingCardPos: function (pos, index) {
        return MJModel.cardConfig.getChildByName("outting").getChildByName("outting" + pos).getPosition();
    },

    getOutingCardOrder: function (pos, index) {

        return 1000;
    },

    getHandCardOrder: function (pos, index) {
        var order = [
            [
                400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
            ],
            [
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
            ],
            [
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            ],
            [
                13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
            ],
        ];
        return order[pos][index];
    },

    getHandCardPos: function (pos, index, all, needRestri) {
        if (needRestri == undefined)needRestri = false;
        if (MJModel.isOnVideo) {
            return this.getEndCardPos(pos, index, all, needRestri);
        }
        var findex = index;
        if (needRestri) {
            findex = index - 1;
        }
        if (findex == -1 && MJModel.mj_reflashcardtype == 1){
            return MJConfig.getMoCardPos(pos);
        }
        return this.getWorldPosition("hand_" + pos, "h_" + findex);
    },

    getEndCardPos: function (pos, index, all, needRestri) {
        if (needRestri == undefined)needRestri = false;
        var findex = index;
        if (needRestri) {
            findex = index - 1;
        }
        return this.getWorldPosition("end_" + pos, "e_" + findex);
    },

    getEndCardMoPos: function (pos) {
        return this.getWorldPosition("end_" + pos, "e_mo");
    },

    getMoCardOrder: function (pos) {
        var order = this.getHandCardOrder(pos, 0);
        if (pos == 1) {
            order -= 1;
        }
        if (pos == 3) {
            order += 1;
        }
        return order;
    },
    getMoCardPos: function (pos) {
        if (MJModel.isOnVideo) {
            return this.getEndCardMoPos(pos);
        }
        return this.getWorldPosition("hand_" + pos, "h_mo");
    },

    getPengKangCardPos: function (pos, index) {
        if((pos == 1 || pos == 3) && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
            return this.getWorldPosition("peng_v_" + pos, "p_" + index);
        }else {
            return this.getWorldPosition("peng_" + pos, "p_" + index);
        }
    },

    getPengKangCardOrder: function (pos, index) {
        var order = this.getHandCardOrder(pos, 13 - index);
        return order;
    },

    getPengKangCardPos2: function (pos, index, dex) {
        if((pos == 1 || pos == 3) && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
            return MJModel.cardConfig.getChildByName("peng_v_" + pos).getChildByName("p_" + index).getChildByName("p_" + dex).getPosition();
        }else {
            return MJModel.cardConfig.getChildByName("peng_" + pos).getChildByName("p_" + index).getChildByName("p_" + dex).getPosition();
        }
    },

    getPengKangCardOrder2: function (pos, index, dex) {
        var order = [
            [
                0, 0, 0, 1,
            ],
            [
                2, 1, 0, 3
            ],
            [
                0, 0, 0, 1,
            ],
            [
                0, 1, 2, 3,
            ],
        ];
        return order[pos][dex];
    },

    getOperatorTipPos: function (pos) {
        return MJModel.cardConfig.getChildByName("op").getChildByName("op" + pos).getPosition();
    },

    getScale_new: function (pos, type, index, isCard) {
        var ss = 1;
        cc.log("MJConfig.getScale_new: pos" + pos + "type:" + type + "index:" + index);
        try{
            if (type == CardType.Card_Hand) {
                ss = MJModel.cardConfig.getChildByName("hand_" + pos).getChildByName("h_" + index).getScale();
            }
            if (type == CardType.Card_Peng && isCard == false) {
                if((pos == 1 || pos == 3) && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
                    ss = MJModel.cardConfig.getChildByName("peng_v_" + pos).getChildByName("p_" + index).getScale();
                }else {
                    ss = MJModel.cardConfig.getChildByName("peng_" + pos).getChildByName("p_" + index).getScale();
                }
            }
            if (type == CardType.Card_Peng && isCard == true) {
                if((pos == 1 || pos == 3) && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
                    ss = MJModel.cardConfig.getChildByName("peng_v_" + pos).getChildByName("p_0").getChildByName("p_" + index).getScale();
                }else {
                    ss = MJModel.cardConfig.getChildByName("peng_" + pos).getChildByName("p_0").getChildByName("p_" + index).getScale();
                }
            }
            if (type == CardType.Card_Out) {
                if (MJModel.mj_bg_use == 1) {
                    if(pos == 0 || pos == 2){
                        index = index%26;
                    }else {
                        if(MJModel.mj_paiban == "vertical"){
                            index = index%28;
                        }else {
                            index = index%30;
                        }
                    }
                }else {
                    index = index%26;
                }
                return MJModel.cardConfig.getChildByName("out_" + pos).getChildByName("o_" + index).getScale();
            }
            if (type == CardType.Card_Outing) {
            }
            if (type == CardType.Card_Public) {
                ss = MJModel.cardConfig.getChildByName("public_" + pos).getChildByName("p_" + index).getScale();
            }
            if (type == CardType.Card_Hu) {
                ss = MJModel.cardConfig.getChildByName("hu_" + pos).getChildByName("h_" + index).getScale();
            }
            if (type == CardType.Card_End) {
                ss = MJModel.cardConfig.getChildByName("end_" + pos).getChildByName("e_" + index).getScale();
            }
        }catch(e){
            ss = 1;
        }

        return ss;
    },

    getHuCardPos: function (pos, index) {
        index = index%12;
        if((pos == 1 || pos == 3) && MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
            return this.getWorldPosition("hu_v_" + pos , "h_" + index);

        }else {
            return this.getWorldPosition("hu_" + pos , "h_" + index);
        }
    },

    getHuCardOrder: function (pos, index) {
        index = index%12;
        if(MJModel.mj_bg_use == 0){
            var order = [
                [
                    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                ],
                [
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                ],
                [
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                ],
                [
                    11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16,
                ],
            ];
            return order[pos][index];
        }else {
            var order = [
                [
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                ],
                [
                    11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                ],
                [
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                ],
                [
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                ],
            ];
            return order[pos][index];
        }

    },

    getCardStr_new: function (value, pos, type, index) {
        var _reValue = "";
        var c = Utils.toHex2(value);
        if (pos == 0) {
            if (type == CardType.Card_Hand) {
                if (MJModel.seatid != -1) {
                    _reValue = "hand_0/" + c + ".png";
                } else {
                    _reValue = "peng_end_0/" + c + ".png";
                }
            } else if (type == CardType.Card_Outing) {
                _reValue = "hand_0/" + c + ".png";
            } else if (type == CardType.Card_Peng || type == CardType.Card_End) {
                _reValue = "peng_end_0/" + c + ".png";
            } else if (type == CardType.Card_Mo) {
                _reValue = "hand_0/" + c + ".png";
            } else if (type == CardType.Card_Out || type == CardType.Card_Hu) {
                _reValue = "out_0/" + c + ".png";
            } else if (type == CardType.Card_Public) {
                _reValue = "public_0/" + c + ".png";
            }
        }
        else if (pos == 1) {
            if (type == CardType.Card_Hand) {
                _reValue = "hand_1/00.png";
            } else if (type == CardType.Card_Outing) {
                _reValue = "hand_0/" + c + ".png";
            } else if (type == CardType.Card_Peng || type == CardType.Card_End) {
                _reValue = "peng_end_1/" + c + ".png";
            } else if (type == CardType.Card_Mo) {
                _reValue = "hand_1/00.png";
            } else if (type == CardType.Card_Out || type == CardType.Card_Hu) {
                _reValue = "out_1/" + c + ".png";
            } else if (type == CardType.Card_Public) {
                _reValue = "public_1/" + c + ".png";
            }

            if(MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
                if(type == CardType.Card_Out || type == CardType.Card_Hu || type == CardType.Card_Peng){
                    _reValue = "out_0/" + c + ".png";
                }
            }
        }
        else if (pos == 2) {
            if (type == CardType.Card_Hand) {
                _reValue = "hand_2/00.png";
            } else if (type == CardType.Card_Outing) {
                _reValue = "hand_0/" + c + ".png";
            } else if (type == CardType.Card_Peng || type == CardType.Card_End) {
                _reValue = "out_peng_end_2/" + c + ".png";
            } else if (type == CardType.Card_Mo) {
                _reValue = "hand_2/00.png";
            } else if (type == CardType.Card_Out || type == CardType.Card_Hu) {
                _reValue = "out_peng_end_2/" + c + ".png";
            } else if (type == CardType.Card_Public) {
                _reValue = "public_2/" + c + ".png";
            }
        }
        else if (pos == 3) {
            if (type == CardType.Card_Hand) {
                _reValue = "hand_3/00.png";
            } else if (type == CardType.Card_Outing) {
                _reValue = "hand_0/" + c + ".png";
            } else if (type == CardType.Card_Peng || type == CardType.Card_End) {
                _reValue = "peng_end_3/" + c + ".png";
            } else if (type == CardType.Card_Mo) {
                _reValue = "hand_3/00.png";
            } else if (type == CardType.Card_Out || type == CardType.Card_Hu) {
                _reValue = "out_3/" + c + ".png";
            } else if (type == CardType.Card_Public) {
                _reValue = "public_3/" + c + ".png";
            }

            if(MJModel.mj_bg_use == 1 && MJModel.mj_paiban == "vertical"){
                if(type == CardType.Card_Out || type == CardType.Card_Hu || type == CardType.Card_Peng){
                    _reValue = "out_0/" + c + ".png";
                }
            }
        }

        return _reValue;
    },

    getTipCardScale: function () {
        return 1;
    },

    getPublicCardPos: function (pos,index) {
        try{
            return this.getWorldPosition("public_" + pos, "p_" + index);
        }catch(e){
            return cc.p(-1000,-1000);
        }
    },

    getPublicCardOrder: function (pos,index) {
        var mi = parseInt((index / 2) % (MJModel.mj_public_nums[pos] / 2));
        var mj = parseInt(index % 2);
        if (pos == 1){
            return 20 * mj - mi + 50;
        }else if (pos == 3) {
            return 20 * mj + mi;
        }else if (pos == 2){
            return mj + 1;
        }else{
            return mj + 100;
        }
    },

    getNiaoEndCardPos:function (pos , index, length , isQiShouHu) {
        if(isQiShouHu == undefined){
            isQiShouHu = false;
        }
        var begin = cc.p(0, 0);
        var qpos = this.getQiShouHuTipPos(pos);
        var cardSize = cc.size(85, 123);
        if(pos == 0 || pos == 2){
            if(pos == 2){
                begin.y = qpos.y - 50;
                if(isQiShouHu){
                    begin.y += 50;
                }
            }else {
                begin.y = qpos.y
            }
            var startX = qpos.x - parseInt(cardSize.width*length/2);
            begin.x = startX + cardSize.width/2 + index*cardSize.width;
        }else if(pos == 1){
            begin.y = qpos.y;
            begin.x = qpos.x - (length - index - 1)*cardSize.width;
            if(isQiShouHu){
                begin.x -= 50;
            }
        }else {
            begin.y = qpos.y;
            begin.x = qpos.x + index*cardSize.width;
            if(isQiShouHu){
                begin.x += 50;
            }
        }
        return begin;
    },

    getNiaoBigCardPos:function (index, length) {
        if(length > 15){
            length = parseInt(length/2);
        }
        var count = parseInt(index/length);
        index = index%length;
        var begin = cc.p(0, 0);
        var size = cc.director.getWinSize();
        var cardSize = cc.size(83, 125);
        var startX = parseInt((size.width - cardSize.width * length)/2);
        begin = cc.p(startX + cardSize.width / 2  + index * cardSize.width, 320 - count * 125 + cardSize.height / 2);
        return begin;
    },

    getShaiZiPos:function (index , length) {
        var begin = cc.p(0, 0);
        var size = cc.director.getWinSize();
        var cardSize = cc.size(100, 100);
        var marginW = -10;
        var startX = parseInt((size.width - cardSize.width * length - (length - 1) * marginW)/2);
        begin = cc.p(startX + cardSize.width / 2  + index * (cardSize.width + marginW), 390);
        return begin;
    },

    getQiShouHuTipPos: function (pos) {
        var begin = cc.p(0, 0);
        var size = cc.director.getWinSize();
        if(MJModel.mj_bg_use == 0){
            if (pos == 0) {
                begin = cc.p(size.width / 2, 150);
            }
            else if (pos == 1) {
                begin = cc.p(size.width - 220, 383);
            }
            else if (pos == 2) {
                begin = cc.p(size.width / 2, size.height - 90);
            }
            else if (pos == 3) {
                begin = cc.p(230, 383);
            }
        }else {
            if (pos == 0) {
                begin = cc.p(size.width / 2, 150);
            }
            else if (pos == 1) {
                begin = cc.p(size.width - 190, 383);
            }
            else if (pos == 2) {
                begin = cc.p(size.width / 2, size.height - 130);
            }
            else if (pos == 3) {
                begin = cc.p(190, 383);
            }
        }
        return begin;
    },

    getWorldPosition: function (parentName, childName) {
        cc.log("parentName:" + parentName + "-------------" + "childName:" + childName);
        var pNode = MJModel.cardConfig.getChildByName(parentName);
        var childNone = pNode.getChildByName(childName);
        return pNode.convertToWorldSpace(childNone.getPosition());
    },
};