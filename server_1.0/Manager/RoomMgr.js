var m_rooms = {};       //正在游戏中的所有房间
var m_roomsInfo = {};   //存放房间信息

//获取房间信息
exports.getRoom = function (roomId) {

}

exports.addRoom = function (roomId, userId) {
    var room = m_rooms[roomId];
    //判断房间是否已满
    if (room) {
        //分配座位ID
        var seatId = -1
        for (var key in room) {
            if (room[key] == -1) {
                seatId = key;
                room[key] = userId;
                break;
            }
        }

        if (seatId == -1) {
            return false;
        }
    } else {
        room[roomId] = {
            "0": userId,
            "1": -1,
            "2": -1,
            "3": -1,
        }
    }
    return true;
}

exports.getSeatId = function (roomId, userId) {
    var room = m_rooms[roomId];
    var seatId = -1
    for (var key in room) {
        if (room[key] == userId) {
            seatId = key;
            break;
        }
    }
    return seatId;
}

exports.getRoomInfo = function (roomId) {

}

exports.setRoomInfo = function (roomId, roomInfo) {

}


