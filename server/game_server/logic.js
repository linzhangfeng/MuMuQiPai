var userMgr = require("./usermgr");
var roomMgr = require("./roommgr");
var handlerCMD = require('./proto');
var http = require('../utils/http');
exports.parsePacket = function(packet){
    var data = JSON.parse(packet);
    console.log("parsePacket:"+JSON.stringify(data));
    return data;
}
exports.sendPacket = function (socket, packet) {
    http.httpLog(1,"sendPacket:",JSON.stringify(packet));
    socket.emit('data', packet);
}

exports.sendHandshake = function(socket){
    var sendData = {};
    sendData.data = { hello: 'world' };
    sendData.id = handlerCMD.CMD.SERVER_CONNECT_SUCC_BC;
   exports.sendPacket(socket,sendData);
}

exports.sendTableInfo = function (userId) {
    var socket = userMgr.get(userId);
    var roomId = socket.roomId;
    var room = roomMgr.getRoom(roomId);

    var seatId = roomMgr.getUserSeatId(userId);
    var playersInfo = [];
    var seats = room.seats;
    for (var i = 0; i < seats.length; i++) {
        var seat = seats[i];
        var seatObject = {
            userId: seat.userId,
            seatId: seat.seatId,
            ready: seat.ready,
            money: seat.money,
            name: seat.name,
            sex: seat.sex,
        }
        playersInfo.push(seatObject);
    }
    var sendData = {};
    sendData.data = {
        seatId: seatId,
        playersInfo: playersInfo,
        playerNum: 2,
    };
    sendData.id = handlerCMD.CMD.SERVER_TABLE_INFO_UC;
    exports.sendPacket(socket,sendData);
}

exports.sendGameScenceInfo = function (userId) {
    var socket = userMgr.get(userId);
    var roomId = socket.roomId;
    var room = roomMgr.getRoom(roomId);

    var sendData = {};
    var disState = room.UserDisbandstate;
    var linestate = room.UserOnlinestate;
    var overtime = room.UserOvertime;
    sendData.data = {
        UserDisbandstate: [],
        UserOnlinestate: [],
        UserOvertime: [],
    };
    sendData.id = handlerCMD.CMD.SERVER_GAME_SCENE;
    exports.sendPacket(socket,sendData);
}