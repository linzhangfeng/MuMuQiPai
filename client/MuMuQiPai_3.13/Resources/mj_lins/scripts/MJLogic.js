var createPacket = function (obj) {
    cc.log("createPacket:" + obj);
    var Object = Build.build(obj);
    var packet = new Object;
    return packet;
}

var parsePacket = function (data) {
    //cc.log("parsePacket:" + data);
    //var Object = Build.build(obj);
    var packet = JSON.parse(data);
    try{
        cc.log("parsePacket:" + "[" + JSON.stringify(packet) + "]");
    }catch (e){
        cc.log(e);
    }
    return packet;
}

var sendPacket = function (packet, cmd) {
    try{
        cc.log("sendPacket:" + "[" + JSON.stringify(packet) + "]");
    }catch (e){

    }
    SocketClient.getInstance(MJModel.curRoomID).send_Data(JSON.stringify(packet), cmd);
}

var sendLogin = function () {
    var packet = {};
    packet.userId = UserModel.getInstance().getUid();
    packet.roomid = MJModel.curRoomID;
    packet.token = "test_token";
    packet.sign = "test_sign";
    sendPacket(packet, CMD.CLIENT_LOGIN_REQ);
}

var sendLogout = function () {
    //var packet = createPacket("proto.login.ReqLogout");
    var packet = {};
    packet.uid = UserModel.getInstance().getUid();
    sendPacket(packet, CMD.CLIENT_LOGOUT_REQ);
}

var sendTableInfoReq = function () {
    var packet = createPacket("proto.game.ReqRoomInfo");
    packet.uid = ZJHModel.getInstance().uid;
    packet.skey = ZJHModel.getInstance().UserKey;
    packet.vid = 101;
    sendPacket(packet, CMD.CLIENT_MJ_HOME_RETURN);

    MJModel.lastSceneInfoReqTime = time(null);
}

var sendUpTable = function () {
    var packet = createPacket("proto.login.ReqTableAction");
    packet.uid = ZJHModel.getInstance().uid;
    packet.roomid = MJModel.curRoomID;
    sendPacket(packet, CMD.CLIENT_UPTABLE_APPLY_REQ);
}

var sendDownTable = function () {
    var packet = createPacket("proto.login.ReqTableAction");
    packet.uid = ZJHModel.getInstance().uid;
    packet.roomid = MJModel.curRoomID;
    sendPacket(packet, CMD.CLIENT_DOWNTABLE_REQ);
}

var sendReady = function () {
    var packet = createPacket("proto.game.ReqAction");
    packet.action = 2;
    sendPacket(packet, CMD.CLIENT_MJ_USER_ACTION);
}

var sendQuxiaoReady = function () {
    var packet = createPacket("proto.game.ReqAction");
    packet.action = 0;
    sendPacket(packet, CMD.CLIENT_MJ_USER_ACTION);
}

var sendHeartbeat = function () {
    //var packet = createPacket("proto.login.ReqHeatBeat");
    var packet = {};
    packet.state = 1;
    packet.msg = "心跳连接";
    sendPacket(packet, CMD.CLINET_HEART_BEAT_REQ);
}

var sendEndRoomREQ = function (v) {
    //var packet = createPacket("proto.game.ReqChooseDisband");
    var packet = {};
    packet.Choosestate = v;
    sendPacket(packet, CMD.CLIENT_DISBAND_ROOM_REQ);
}

var sendEndRoomResult = function (v) {
    //var packet = createPacket("proto.game.ReqChooseDisband");
    var packet = {};
    packet.Choosestate = v;
    sendPacket(packet, CMD.CLIENT_DISBAND_RESULT_REQ
    );
}

var sendOperateCard = function (type, card) {
    var packet = createPacket("proto.game.ReqOperateResult");
    packet.Type = type;
    packet.Card = card;
    packet.operateid = MJModel.curOperateID;
    sendPacket(packet, CMD.CLIENT_MJ_OPERATE_CARD);
}

var sendChangeCard = function (v, cardlist) {
    var packet = createPacket("proto.game.ReqChangeCard");
    packet.ischange = v;
    packet.Card = [];
    if (v == 1) {
        for (var i in cardlist) {
            packet.Card.push(cardlist[i]);
        }
    }
    sendPacket(packet, CMD.CLIENT_MJ_CHANGE_CARD);
}

var sendLackCard = function (v) {
    var packet = createPacket("proto.game.ReqChooseLackType");
    packet.Type = v;
    sendPacket(packet, CMD.CLIENT_MJ_CHOOSE_LACKTYPE);
}

var sendPiaoNums = function (v) {
    var packet = createPacket("proto.game.scmj.ReqChoosePiao");
    packet.PiaoNum = v;
    sendPacket(packet, CMD.CLIENT_MJ_CHOOSE_PIAO);
}

var sendGaNums = function (v) {
    var packet = createPacket("proto.game.hnmahjonggame.ReqChoosePiao");
    packet.PiaoNum = v;
    sendPacket(packet , CMD.CLIENT_MJ_CHOOSE_PIAO);
}

var sendPaoNums = function (v) {
    var packet = createPacket("proto.game.sxmahjonggame.ReqChoosePiao");
    packet.PiaoNum = v;
    sendPacket(packet , CMD.CLIENT_MJ_CHOOSE_PIAO);
}

var sendListenType = function (type) {
    var packet = createPacket("proto.game.scmj.ReqChooseListen");
    packet.ListenType = type;
    sendPacket(packet, CMD.CLIENT_MJ_LISTEN_TYPE);
}

var sendUpTableInfoReq = function (start_index, end_index) {
    var packet = createPacket("proto.login.AckUpTableInfoReq");
    packet.start_index = start_index;
    packet.end_index = end_index;
    sendPacket(packet, CMD.CLIENT_UPTABLE_INFO_REQ);
}

var sendOutCard = function (card) {
    var packet = createPacket("proto.game.ReqOutCard");
    packet.Card = card;
    packet.ShowCard = MJModel.isLiangPai;
    sendPacket(packet, CMD.CLIENT_MJ_OUT_CARD);
}

var sendBigface = function (seatid,target_seatid,type) {
    var packet = createPacket("proto.login.ReqEmotion");
    packet.seatid = seatid;
    packet.target_seatid = target_seatid;
    packet.type = type;
    sendPacket(packet, CMD.CLIENT_EMOTION_REQ);
}

var sendGoHome = function () {
    //var packet = createPacket("proto.login.ReqHeatBeat");
    //packet.state = 1;
    //sendPacket(packet, CMD.CLIENT_HOME_OUT);
}

//请求总结算
var sendGameEndRecordReq = function () {
    var packet = createPacket("proto.login.ReqHeatBeat");
    packet.state = 1;
    sendPacket(packet, CMD.CLIENT_GAME_END_RECORD_REG);
}

var sendBaoTingReq = function (seatId, tingState) {
    var packet = createPacket("proto.game.guizhou.tagBaoTing");
    packet.ChairID = seatId;
    packet.TingState = tingState;
    sendPacket(packet, CMD.CLINET_MJ_TIAN_TING);
}

//发送聊天
var sendChatReq = function (content) {
    var packet = createPacket("proto.login.AckChatBC");
    packet.seatid = MJModel.seatid;
    packet.charmsg = content;
    sendPacket(packet, CMD.CLIENT_CHAT_REQ);
}

var sendHubeiPaoNums = function (v , isTongYontong) {
    var packet = createPacket("proto.game.jingshanmj.ReqChoosePiao");
    packet.PiaoNum = v;
    packet.IsGouxuan = isTongYontong;
    sendPacket(packet , CMD.CLIENT_MJ_CHOOSE_PIAO);
}

//请求结算记录
var sendRecordReq = function () {
    var packet = createPacket("proto.login.ReqHeatBeat");
    packet.state = 1;
    sendPacket(packet, CMD.CLIENT_GAME_RECORD_REG);
}

var sendSelfGPSInfo = function(data){
    try{
        var packet = createPacket("proto.login.AckGPS");
        packet.uid = ZJHModel.getInstance().uid;
        packet.latitude = data.latitude;
        packet.longitude = data.longitude;
        packet.city = data.city;
        sendPacket(packet, CMD.CLIENT_PLAYER_GPS);
    }catch(e){

    }
}

var sendSelectMode = function (n) {
    var packet = createPacket("proto.game.ReqChangeMode");
    packet.num = n;
    sendPacket(packet, CMD.CLIENT_REQ_CHANGE_MODE);
}

// 实时语音
var sendRealTimeVoiceStatus = function (state) {
    var packet = createPacket("proto.game.RealTimeSpeech");
    packet.code = state;
    sendPacket(packet, CMD.CLIENT_REAL_TIME_SPEECH);
};