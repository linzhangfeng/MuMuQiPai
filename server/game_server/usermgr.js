var roomMgr = require('./roommgr');
var db = require('../utils/db');
var CMD = require('./proto');
var userList = {};
var userOnline = 0;
exports.bind = function(userId,socket){
    var socketTemp = userList[userId];
    if(socketTemp){
        exports.del(userId);
    }
    userList[userId] = socket;
    userOnline++;
};

exports.del = function(userId,socket){
    var socket  = exports.get(userId);
    if(socket){
        socket.disconnect();
        var heartBeatInterval = socket.heartBeatInterval;
        if(heartBeatInterval)clearInterval(heartBeatInterval);
    }
    delete userList[userId];
    userOnline--;
};

exports.get = function(userId){
    return userList[userId];
};

exports.isOnline = function(userId){
    var data = userList[userId];
    if(data != null){
        return true;
    }
    return false;
};

exports.getOnlineCount = function(){
    return userOnline;
}

exports.sendMsg = function(userId,event,msgdata){
    var userInfo = userList[userId];
    if(userInfo == null){
        return;
    }
    var socket = userInfo;
    if(socket == null){
        return;
    }
    socket.emit(event,msgdata);
};

exports.logout = function(userId,socket){
   exports.disconnect(userId);
};

exports.upTable = function(socket){
    //广播当前玩家上桌信息
    var sendData = {};
    sendData.data = {
        seatId:roomMgr.getUserSeatId(),
        name:"",
        uId:152,
        money:10000,
        sex:1,
        avatar:"fasd",
    };
    sendData.id = handlerCMD.CMD.SERVER_UPTABLE_SUCCC_BC;
    socket.emit('data', sendData);
};

exports.kickAllInRoom = function(roomId){
    if(roomId == null){
        return;
    }
    var roomInfo = roomMgr.getRoom(roomId);
    if(roomInfo == null){
        return;
    }

    for(var i = 0; i < roomInfo.seats.length; ++i){
        var rs = roomInfo.seats[i];

        //如果不需要发给发送方，则跳过
        if(rs.userId > 0){
            var socket = userList[rs.userId];
            if(socket != null){
                exports.del(rs.userId);
                socket.disconnect();
            }
        }
    }
};

exports.broacastInRoom = function(event,data,userId,includingSender){
    var roomId = roomMgr.getUserRoom(userId);
    if(roomId == null){
        return;
    }
    var roomInfo = roomMgr.getRoom(roomId);
    if(roomInfo == null){
        return;
    }

    for(var i = 0; i < roomInfo.seats.length; ++i){
        var rs = roomInfo.seats[i];

        //如果不需要发给发送方，则跳过
        if(rs.userId == userId && includingSender != true){
            continue;
        }
        var socket = userList[rs.userId];
        if(socket != null){
            socket.emit(event,data);
        }
    }
};

exports.getUserDataByUserId = function(userId,callback){
    db.get_user_data_by_userid(userId ,function(jsonData){
        if(jsonData == null){
            callback(2,null);
        }else{
            callback(0,jsonData);
        }
    });
};

exports.updateUserRoomId = function(userId,roomId){
    db.update_user_roomid(userId,roomId);
};

exports.disconnect = function(userId){
	if(!userId){
		return;
	}

    var socket = userList[userId];

	var data = {
		userid:userId,
        online:false,
        id:handlerCMD.CMD.SERVER_USER_OFFLINE_BC,
	};

	//通知房间内其它玩家
	exports.broacastInRoom('data',data,userId);

	//清除玩家的在线信息
	exports.del(userId);
	socket.userId = null;
};
exports.startHeartBeat = function(userId){
    var socket = userList[userId];
    //根据上一次件收到的时间检测是否连接超时，超时则断开连接
    var heartBeatInterval = setInterval(function () {
        if (socket) {
            /**
             * 如果10秒内没有收到回应，则表明socket.io长连接失败，关闭socket.io连接
             * 因为socket.io会进行不断的断线重连，因此，强行关闭
             */
            // console.log("lin=socket=时间间隔="+(Date.now() - socket.lastRecieveTime ));
            if (Date.now() - socket.lastRecieveTime > 10000) {
                console.log("lin=socket="+socket.userId +" 断开连接");
                exports.del(socket.userId);
                socket.disconnect();
                socket.heartBeatInterval = heartBeatInterval;
                clearInterval(heartBeatInterval);
            }
        }
    }.bind(socket), 5000); //心跳为每间隔0.5s检测一次
};