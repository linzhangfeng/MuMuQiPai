var m_resultData = require('../../util/result_data');
var m_db = require('../../util/db');
var m_roomMgr = require('../../Manager/RoomMgr');
exports.addRoom = function (req, res) {
    var receiveData = req.query;
    var userId = receiveData['userId'];
    var roomId = receiveData['roomId'];

    var sqlObject = {};
    sqlObject["GR_ID"] = roomId;
    var sendData = {};
    //分配座位ID
    m_roomMgr.addRoom(roomId, userId);
    var seadIt = m_roomMgr.getSeatId(roomId, userId);
    if (seadIt == -1) {//加入房间失败
        var reusltData = m_resultData.init(101, 0, "加入房间失败", JSON.stringify(sendData));
        res.send(reusltData.getDataStr());
        return;
    }
    m_db.find_GameRoom(sqlObject, function (roomData) {
        if (roomData && roomData[0]["GR_State"]) {
            //更新用户信息
            if (roomData && roomData[0]) {
                sqlObject = {};
                sqlObject["UI_ID"] = userId;
                sqlObject["GR_ID"] = roomId;

                var sRoomData = {};
                sRoomData["RoomConfig"] = roomData[0]["GR_RoomInfo"];
                sRoomData["RoomPort"] = roomData[0]["GR_Port"];
                sRoomData["RoomId"] = roomData[0]["GR_ID"];
                sendData["RoomInfo"] = sRoomData;
                m_db.update_UserInfo(sqlObject, function (userData) {
                    if (userData) {
                        var reusltData = m_resultData.init(100, 1, "请求成功", JSON.stringify(sendData));
                        res.send(reusltData.getDataStr());
                    } else {
                        //房间不存在
                        var reusltData = m_resultData.init(101, 0, "未知错误:加入房间失败", JSON.stringify({}));
                        res.send(reusltData.getDataStr());
                    }

                });
            }
        } else {
            //房间不存在
            var reusltData = m_resultData.init(101, 0, "未知错误:加入房间失败", JSON.stringify({}));
            res.send(reusltData.getDataStr());
        }
    });
}

exports.createRoom = function (req, res) {
    var receiveData = req.query;
    var roomConfig = receiveData['roomInfo'];
    var userId = receiveData['userId'];
    // var port = receiveData['port'];
    var sendData = {};
    var sqlObject = {};
    sqlObject["CreateTime"] = 'NOW()';
    sqlObject["UpdateTime"] = 'NOW()';
    sqlObject["GR_RoomInfo"] = "'" + roomConfig + "'";
    console.log("lin=createRoom:" + roomConfig);
    var roomConfigJson = JSON.parse(roomConfig)
    sqlObject["GR_Port"] = roomConfigJson["port"];
    m_db.addRoom(sqlObject, function (data) {
        if (data) {
            var roomId = data["GR_ID"];//返回roomId
            sqlObject = {};
            sqlObject["GR_ID"] = roomId;
            m_db.find_GameRoom(sqlObject, function (roomData) {
                if (roomData && roomData[0]) {
                    //更新用户信息
                    if (roomData && roomData[0]) {
                        sqlObject = {};
                        sqlObject["UI_ID"] = userId;
                        sqlObject["GR_ID"] = roomId;

                        var sRoomData = {};
                        sRoomData["RoomConfig"] = roomData[0]["GR_RoomInfo"];
                        sRoomData["RoomPort"] = roomData[0]["GR_Port"];
                        sRoomData["RoomId"] = roomData[0]["GR_ID"];
                        sendData["RoomInfo"] = sRoomData;
                        m_db.update_UserInfo(sqlObject, function (userData) {//更新用户信息
                            if (userData) {
                                var reusltData = m_resultData.init(100, 1, "请求成功", JSON.stringify(sendData));
                                res.send(reusltData.getDataStr());
                            } else {
                                //房间不存在
                                var reusltData = m_resultData.init(101, 0, "未知错误:加入房间失败", JSON.stringify({}));
                                res.send(reusltData.getDataStr());
                            }

                        });
                    }
                } else {
                    //房间不存在
                    var reusltData = m_resultData.init(101, 0, "未知错误:加入房间失败", JSON.stringify({}));
                    res.send(reusltData.getDataStr());
                }
            });
        } else {
            //创建房间失败
            var reusltData = m_resultData.init(101, 0, "未知错误:创建房间失败", JSON.stringify({}));
            res.send(reusltData.getDataStr());
        }
    });

}

exports.getUserInfo = function (req, res) {

}

//获取房间信息
exports.getRoomInfo = function (req, res) {
    var receiveData = req.query;
    var roomId = receiveData['roomId'];

}

exports.getDicType = function (req, res) {
    //获取管理员用户ID
    var receiveData = req.query;
    var admin_id = receiveData['admin_id'];
    var sendData = {};
    m_db.find_GameGategory(null, function (data) {
        if (data) {
            console.log("lin=data=" + JSON.stringify(data));
            var arr = [];
            for (var i = 0; i < data.length; i++) {
                var obj = {};
                obj["dictype_id"] = data[i]["DT_ID"];
                obj["dictype_name"] = data[i]["DT_Name"];
                obj["dictype_createtime"] = data[i]["DT_CreateTime"];
                obj["dictype_updatetime"] = data[i]["DT_UpdateTime"];
                arr.push(obj);
            }
            sendData["list"] = arr;
        }
        var reusltData = m_resultData.init(100, "ok", "请求成功", JSON.stringify(sendData));
        res.send(reusltData.getDataStr());
    });
}

exports.operatorDicType = function (req, res) {
    var receiveData = req.query;
    var admin_id = receiveData['admin_id'];
    var type = receiveData['type'];
    var dictype_name = receiveData['dictype_name'];
    var dictype_id = receiveData['dictype_id'];
    var sendData = {};
    var sqlObject = {};
    sqlObject["DT_ID"] = receiveData["dictype_id"];
    sqlObject["DT_Name"] = receiveData["dictpe_name"];
    sqlObject["DT_UpdateTime"] = 'NOW()';
    if (type == 1) { //   添加
        sqlObject["DT_CreateTime"] = 'NOW()';
        m_db.add_GameGategory(sqlObject, function (data) {
            var code = -1;
            var status = 0;
            var message = "请求错误";
            if (data) {
                console.log("lin=data=" + JSON.stringify(data));
                code = 100;
                status = 1;
                message = "";
            }

            var reusltData = m_resultData.init(code, status, message, JSON.stringify(sendData));
            res.send(reusltData.getDataStr());
        });

    } else if (type == 2) {    // 删除
        m_db.delate_GameGategory(sqlObject, function (data) {
            var code = -1;
            var status = 0;
            var message = "数据请求错误";
            if (data) {
                console.log("lin=data=" + JSON.stringify(data));
                code = 100;
                status = 1;
                message = "";
            }

            var reusltData = m_resultData.init(code, status, message, JSON.stringify(sendData));
            res.send(reusltData.getDataStr());
        });
    } else if (type == 3) {    // 修改
        m_db.update_GameGategory(sqlObject, function (data) {
            var code = -1;
            var status = 0;
            var message = "数据请求错误";
            if (data) {
                console.log("lin=data=" + JSON.stringify(data));
                code = 100;
                status = 1;
                message = "操作成功";
            }

            var reusltData = m_resultData.init(code, status, message, JSON.stringify(sendData));
            res.send(reusltData.getDataStr());
        });
    }
}