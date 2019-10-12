var crypto = require('../utils/crypto');
var db = require('../utils/db');
var express = require('express');

var tokenMgr = require('./tokenmgr');
var roomMgr = require('./roommgr');
var userMgr = require('./usermgr');
var http = require('../utils/http');
var handlerCMD = require('./proto');
var logic = require('./logic');
var io = null;

var app = express();

//设置跨域访问
app.all('*', function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Access-Control-Allow-Methods","PUT,POST,GET,DELETE,OPTIONS");
    res.header("X-Powered-By",' 3.2.1')
    res.header("Content-Type", "application/json;charset=utf-8");
	http.send(res,0,"ok",{});
});

var config = null;

exports.start = function(conf,mgr){
	config = conf;
	
	var httpServer = require('http').createServer(app);
	io = require('socket.io')(httpServer);
	httpServer.listen(config.CLIENT_PORT);
	console.log("socket server start");
	io.sockets.on('connection',function(socket){
		console.log("client connect server");

		//握手成功
		logic.sendHandshake(socket);
		
		var self = this;
		socket.on('data',function(data){
			jsonData = JSON.parse(data);
			if(jsonData){
				var cmd = jsonData.cmd;
				var data = jsonData.data;
				var id = jsonData.id;
				handlerCmd(id,data);
			}
		});

		function handlerCmd(cmd,data){
			switch(cmd){
				case handlerCMD.CMD.CLIENT_LOGIN_REQ:
					handler_client_login_req(data);
				break;
				case handlerCMD.CMD.CLIENT_LOGOUT_REQ:
					handler_client_logout_req(data);
				break;
				case handlerCMD.CMD.CLIENT_READY_REQ:
					handler_client_ready_req(data);
				break;
				case handlerCMD.CMD.CLIENT_DISBAND_ROOM_REQ:
					handler_client_disband_room_req(data);
				break;
				case handlerCMD.CMD.CLINET_HEART_BEAT_REQ:
					handler_client_heart_beat_req(data);
				break; 
			}
		};

		function handler_client_disband_room_req(data){
			console.log("handler_client_disband_room_req");
			roomMgr.exitRoom(socket.userId);
		};
		function handler_client_login_req(data){
			console.log("handler_client_login_req");
			var jsonData = logic.parsePacket(data); 
			var roomId = jsonData["roomId"];
			var userId = jsonData["userId"];
			var name = jsonData["name"];
			var sex = jsonData["sex"];
			var score  = jsonData["score"];
			userMgr.bind(userId,socket);
			socket.userId = userId;
			socket.roomId = roomId;

			var sendData = {};
			sendData.data = {};
			sendData.id = handlerCMD.CMD.SERVER_LOGIN_SUCC_UC;
			socket.emit('data', sendData);
			
			//开启心跳
			socket.lastRecieveTime = Date.now();
			userMgr.startHeartBeat(userId);

			//初始化用户信息
			var roomInfo = roomMgr.getRoom(roomId);
			var seats = roomInfo["seats"];

			var seatId = roomMgr.getUserSeatId(userId);
			if(seatId == null){//分配seatId
				seatId = roomMgr.setUserSeatId(userId,roomId);
			}
			if(seatId != -1){
				var info = seats[seatId];
				info.userId = userId;
				info.name = name;
				info.sex = sex;
				info.score = score;
			}else{
				//房间已满，退出房间
			}
			//发送桌面信息
			logic.sendTableInfo(userId);

			//发送场景消息
			logic.sendGameScenceInfo(userId);
		};

		function handler_client_logout_req(data){
			console.log("handler_client_logout_req");
			var sendData = {};
			sendData.data = { hello: 'world' };
			sendData.cmd = handlerCMD.CMD.SERVER_LOGOUT_SUCC_BC;
			userMgr.sendMsg(socket.userId,'data',sendData);
			
			userMgr.logout(socket.userId);
			// userMgr.upTable(socket);
		};

		function handler_client_ready_req(data){
			console.log("handler_client_ready_req");
			var sendData = {};
			sendData.data = { hello: 'world' };
			sendData.cmd = handlerCMD.CMD.SERVER_READY_SUCC_BC;

			userMgr.sendMsg(socket.userId,'data',sendData);
		};

		function handler_client_heart_beat_req(data){
			socket.lastRecieveTime = Date.now();
		};

		//断开链接
		socket.on('disconnect',function(data){
			var userId = socket.userId;
			if(!userId){
				return;
			}

			//如果是旧链接断开，则不需要处理。
			if(userMgr.get(userId) != socket){
				return;
			}

			var data = {
				userid:userId,
				online:false
			};

			//通知房间内其它玩家
			userMgr.broacastInRoom('user_state_push',data,userId);

			//清除玩家的在线信息
			userMgr.del(userMgr.get(userId));
		});
	});

	console.log("game server is listening on " + config.CLIENT_PORT);	
};