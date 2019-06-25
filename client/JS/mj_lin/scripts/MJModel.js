var PlayerModel = cc.Class.extend({
    seatid: -1,
    sex: 0,
    uid: 0,
    name: "",
    avatar: "",
    money: 0,
    ready: 0,
    start_money: 0,
    zhuan_surplustime: 0,

    reset: function () {
        this.uid = -1;
        this.ready = 0;

        this.name = "";
        this.sex = 0;
        this.avatar = "";
        this.money = 0;
        this.zhuan_surplustime = 0;
    },
});

var MJModel =
{
    //***************通用数据*****************//
    curRoomID: 0,

    changeTcpIp: function () {
        this.cur_ip_index++;
        if (this.cur_ip_index >= this.ip_list.length || this.ip_list[this.cur_ip_index] == "") {
            this.cur_ip_index = 0;
        }
        cc.log("changeTcpIp:" + this.ip_list[this.cur_ip_index]);
        this.setHostPort();
    },

    //检测登录地址是否为域名
    isDomain: function (host) {
        var ipReg = new RegExp("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$"); //匹配IP
        return !ipReg.test(host);
    },

    setHostPort: function (isDomainFirst) {
        if (isDomainFirst == undefined) isDomainFirst = false;

        if (isDomainFirst) {
            for (var i = 0; i < this.ip_list.length; i++) {
                if (this.isDomain(this.ip_list[i])) {
                    this.cur_ip_index = i;
                    cc.log("domain first:" + this.ip_list[i]);
                }
            }
        }

        var host = this.ip_list[this.cur_ip_index];
        var needChangeIp = false;
        if (this.isDomain(host)) {
            cc.log("domain login start:" + host);
            //不是IP地址，需要解析出IP
            if (PlatformHelper.decodeIPWithHost) {
                var ip = JSON.parse(PlatformHelper.decodeIPWithHost(host));
                if (ip == null || ip["ips"] == null) {
                    //解析失败，直接跳下一个
                    needChangeIp = true;
                    cc.log("get domain error------ ");
                } else {
                    host = ip["ips"][0];
                    cc.log("get domain success : " + host);
                }
            } else {
                //不支持域名解析，直接跳下一个
                needChangeIp = true;
                cc.log("do not support get domain!");
            }
        }
        if (needChangeIp) {
            this.changeTcpIp();
        } else {
            CCTcpClient.getInstance(MJModel.curRoomID).set_host_port(host, this.port);
        }
    },
    init: function () {
        for (var i = 0; i < this.Play_num; i++) {
            var player = new PlayerModel();
            player.seatid = i;
            this.players[i] = player;
            this.mj_mo_cards.push(-1);
        }
        this.initParam();
    },
    initParam: function () {
        try {
            var ls = cc.sys.localStorage;
            var sound_use = ls.getItem("mj_sound_use");
            if (sound_use)this.mj_sound_use = sound_use;
            var bg_use = ls.getItem("mj_bg_use");
            if (bg_use)this.mj_bg_use = bg_use;

            var cardLight = ls.getItem("cardLight");
            if (cardLight)this.cardLight = cardLight;

            var sound = ls.getItem("sound");
            if (sound) {
                ZJHModel.getInstance().sound = sound;
            } else {
                sound = ZJHModel.getInstance().sound;
            }
            cc.audioEngine.setEffectsVolume(sound);

            var bgMusic = ls.getItem("bgMusic");
            if (bgMusic) {
                ZJHModel.getInstance().bgMusic = bgMusic;
            } else {
                bgMusic = ZJHModel.getInstance().bgMusic;
            }
            cc.audioEngine.setMusicVolume(bgMusic);

            var vibrate = ls.getItem("vibrate");
            if (vibrate)ZJHModel.getInstance().vibrate = vibrate;

            var audio = ls.getItem("audio");
            if (audio)SoundModel.getInstance().can_play = audio;


        } catch (e) {

        }
    },
    //重置属性
    reset: function () {
        //玩家信息初始化
        for (var p in this.players) {
            this.players[p].reset();
        }
        this.cur_seat = -1;
        this.state = -1;
        this.isEnterWait = false;
        this.isZhuanZhuan = false;
        this.canChoiceMore = false;
        this.zhuanReadyTime = 0;
        this.mj_lockcard_type = 0;
        this.isChangeRoom = false;
        this.preGame();
        this.mj_mo_cards = [-1, -1, -1, -1];
    },
    preGame: function () {
        this.listen_info = [];
        this.gang_info = [];
        this.bu_info = [];
        this.final_listen_info = [];
        this.isAutoOutCard = false;
        this.canTouchCard = false;
        this.lackType = -1;
        this.mj_mo_cards = [-1, -1, -1, -1];
        this.isEnterWait = false;
        this.isBaoting = false;
        this.curOperateCard = 0;
        this.curOperate = -1;
        this.isHanBaoDou = false;
        this.IsMyHasBaoting = false;
        this.canChoiceMore = false;
        this.TouPiaoING = false;

        this.tangPai_flags = {};

        this.baoting_test = -1;
        this.isLiangPai = 0;
        this.outPlayerList = [];

        this.mj_public_begin_pos = -1;
        this.mj_public_begin_nums = -1;
        this.mj_lockcard_type = 0;
        try {
            MJModel.mj_table.removeChildByName("tingpai_layer");
        } catch (e) {
            Log(this, e);
        }
    },
    release: function () {
        for (var i in this.players) {
            this.players[i].release();
        }
        this.players = {};
    },

    //该座位ID是否是我
    isMyPlayer: function (seatID) {
        return seatID == this.seatid;
    },
    isMyPos: function (pos) {
        if (pos == 0 && this.seatid != -1)return true;
        return false;
    },
    canQuxiaoZhun: function () {
        return false;
    },
    //获取玩家
    getPlayerByUID: function (uid) {
        for (var p in this.players) {
            if (this.players[p].uid == uid) {
                return this.players[p];
            }
        }
        return null;
    },
    getPosByUID: function (uid) {
        for (var i = 0; i < this.players.length; i++) {
            if (this.players[i].uid == uid) {
                return this.getPosBySeatid(i);
            }
        }
        return -1;
    },
    getPlayerBySeatID: function (seatid) {
        if (this.players.hasOwnProperty(seatid)) {
            return this.players[seatid];
        }
        return null;
    },
    getPlayerByPos: function (pos) {
        var seatid = this.getSeatIDByPos(pos);

        if (this.players.hasOwnProperty(seatid)) {
            return this.players[seatid];
        }
        cc.log("getPlayerByPos erro:" + seatid);
        return null;
    },
    //座位ID转换当前位置
    getPosBySeatid: function (seatid) {
        if (seatid == 255 || seatid == -1) return -1;//服务器默认值有些是255 坑爹的服务器
        var rePos = 0;

        if (this.seatid == -1) {
            if (this.tempSeatid != -1) {
                var index = seatid - this.tempSeatid;
                if (index >= 0) {
                    rePos = index;
                } else {
                    rePos = index + this.Play_num;
                }

            } else {
                rePos = seatid;
            }
        } else {
            var index = seatid - this.seatid;
            if (index >= 0) {
                rePos = index;
            } else {
                rePos = index + this.Play_num;
            }
        }

        if (this.mj_roomType == 1) {
            if (rePos == 2) {
                if (this.seatid == -1) {
                    rePos = 3;
                } else {
                    if (this.seatid == 0) {
                        rePos = 3;
                    } else if (this.seatid == 2) {
                        rePos = 1;
                    }
                }
            }
        } else if (this.mj_roomType == 2) {
            if (rePos == 1 || rePos == 3) {
                rePos = 2;
            }
        }

        //防错措施 避免闪退
        if (rePos < 0 || rePos > 3) {
            rePos = 0;
        }
        return rePos;

    },
    getIndexFromBanker: function (seatid) {
        var dseatid = seatid - MJModel.banker_seatid;
        if (dseatid < 0) {
            if (MJModel.mj_roomType == 2) dseatid += 2;
            if (MJModel.mj_roomType == 1) dseatid += 3;
            if (MJModel.mj_roomType == 0) dseatid += 4;
        }
        return dseatid;
    },
    getNextSeat: function (seatid) {
        var dseatid = seatid + 1;
        var pn = 4;
        if (MJModel.mj_roomType == 2) pn = 2;
        if (MJModel.mj_roomType == 1) pn = 3;
        if (dseatid >= pn) {
            dseatid = 0;
        }
        return dseatid;
    },
    getPreSeat: function (seatid) {
        var dseatid = seatid - 1;
        var pn = 4;
        if (MJModel.mj_roomType == 2) pn = 2;
        if (MJModel.mj_roomType == 1) pn = 3;
        if (dseatid < 0) {
            dseatid = pn - 1;
        }
        return dseatid;
    },
    isState: function (status) {
        if (this.state == status) {
            return true;
        }
        return false;
    },
    getTempPosBySeatid: function (seatid, temp) {
        if (temp == -1) {
            return seatid;
        }
        var index = seatid - temp;
        if (index >= 0)
            return index;
        return index + this.Play_num;
    },
    //当前位置转换座位ID
    getSeatidByPos: function (pos) {
        var reSeatId = 0;

        if (this.seatid == -1) {
            if (this.tempSeatid != -1) {
                var seatid = pos + this.tempSeatid;
                if (seatid >= this.Play_num) {
                    reSeatId = seatid - this.Play_num;
                } else {
                    reSeatId = seatid;
                }

            } else {
                reSeatId = pos;
            }
        } else {
            var seatid = pos + this.seatid;
            if (seatid >= this.Play_num) {
                reSeatId = seatid - this.Play_num;
            } else {
                reSeatId = seatid;
            }

        }

        if (this.mj_roomType == 1) {
            if (reSeatId == 3) {
                if (this.seatid == -1) {
                    reSeatId = 2;
                } else {
                    if (this.seatid == 0) {
                        reSeatId = 2;
                    } else if (this.seatid == 2) {
                        reSeatId = 0;
                    }
                }
            }
        } else if (this.mj_roomType == 2) {
            if (reSeatId == 2) {
                reSeatId = 1;
            } else if (reSeatId == 3) {
                reSeatId = 0;
            }
        }
        //防错措施 避免闪退
        if (reSeatId < 0 || reSeatId > 3) {
            reSeatId = 0;
        }
        return reSeatId;
    },

    setTangPaiFlag: function (pos, tangPai_flag) {
        this.tangPai_flags[pos] = tangPai_flag;
    },

    getTangPaiFlag: function (pos) {
        return this.tangPai_flags[pos];
    },

    isProtobufCJJ3: function () {
        if (this.curGameType == ROOM_TYPE.ROOM_TYPE_HNMJ_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_JXMJ_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_HuBei_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_HENAN_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_SHANXIAN_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_ZHEJIANG_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_HuBei_XianTao_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_HuBei_QianJiang_DZ
            || this.curGameType == ROOM_TYPE.ROOM_TYPE_HuBei_WuHan_DZ
        /*|| this.curGameType == ROOM_TYPE.ROOM_TYPE_HuBei_JingShan*/) {
            return true;
        } else {
            return false;
        }
    },

    addOutPlayer: function (player) {
        for (var i = 0; i < this.outPlayerList.length; i++) {
            var outPlayer = this.outPlayerList[i];
            if (outPlayer.uid == player.uid) {
                return;
            }
        }
        this.outPlayerList.push(player);
    },

    removeOutPlayer: function (player) {
        for (var i = 0; i < this.outPlayerList.length; i++) {
            var outPlayer = this.outPlayerList[i];
            if (outPlayer.uid == player.uid) {
                this.outPlayerList.splice(i, 1);
            }
        }
    },

    getOutPlayerTips: function () {
        if (this.outPlayerList.length > 0) {
            var start = "玩家 ";
            var mid = "";
            for (var i = 0; i < this.outPlayerList.length; i++) {
                var outPlayer = this.outPlayerList[i];
                mid += outPlayer.name + " ";
            }
            var end = "离开游戏，请等待！";
            return start + mid + end;
        }
        return "";
    },
};



