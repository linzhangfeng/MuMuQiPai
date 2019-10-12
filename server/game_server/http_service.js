var crypto = require('../utils/crypto');
var express = require('express');
var db = require('../utils/db');
var http = require('../utils/http');
var roomMgr = require("./roommgr");
var userMgr = require("./usermgr");
var tokenMgr = require("./tokenmgr");

var app = express();
var config = null;

var serverIp = "";

function send(res,ret){
	var str = JSON.stringify(ret);
	http.httpLog(1,"game_server",str);
	res.send(str)
}

//测试
app.all('*', function(req, res, next) {
	res.header("Access-Control-Allow-Origin", "*");
	res.header("Access-Control-Allow-Headers", "X-Requested-With");
	res.header("Access-Control-Allow-Methods","PUT,POST,GET,DELETE,OPTIONS");
	res.header("X-Powered-By",' 3.2.1');
	res.header("Content-Type", "application/json;charset=utf-8");
	next();
});

app.post('/create_room',function(req,res){
	http.postCallback(req,res,function(jsonData){
		http.httpLog(0,"game_server_create_room",JSON.stringify(jsonData));
		var userId = jsonData.userId;
		var conf = {};
		conf.type = 1;
		conf.difen = 1;
		conf.zimo = 1;
		conf.jiangdui = 1;
		conf.huansanzhang = 1;
		conf.zuidafanshu = 1;
		conf.jushuxuanze = 1;
		conf.dianganghua = 1;
		conf.menqing = 1;
		conf.tiandihu = 1;

		var gems = "test";
		roomMgr.createRoom(userId,conf,gems,serverIp,config.CLIENT_PORT,function(errcode,roomId){
			if(errcode != 0 || roomId == null){
				http.send(res,errcode,"create failed.");
				return;	
			}
			else{
				var sign = crypto.md5(userId + req.ip + config.ACCOUNT_PRI_KEY);
				var ret = {
					status:200,
					errmsg:"ok",
					roomId:roomId,
					sign:sign
				}
				send(res,ret);
				db.update_user_roomid(userId,roomId);
			}
		});
	});
});

app.post('/check_room',function(req,res){
	http.postCallback(req,res,function(jsonData){
		var userId = jsonData.userId;
		userMgr.getUserDataByUserId(userId,function(errcode,userData){
			if(errcode != 0 || userData == null){
				http.send(res,errcode,"create failed.");
				return;	
			}
			else{
				var sign = crypto.md5(userId + req.ip + config.ACCOUNT_PRI_KEY);
				var roomId = userData["roomId"];
				var is_room_runing = false;
				if(roomId > 0){
					isRoomRuning = true;
				}
				var ret = {
					status:200,
					errmsg:"ok",
					roomId:roomId,
					isRuning:isRoomRuning,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
					sign:sign
				}
				send(res,ret);
			}
		});
	});
});

app.post('/get_room_info',function(req,res){
	http.postCallback(req,res,function(jsonData){
		http.httpLog(1,"game_server_get_room_info",JSON.stringify(jsonData));
		var roomId = jsonData.roomId;
		var userId = jsonData.userId;
		//判断本地有没有该房间好的房间信息
		var room = roomMgr.getRoom(roomId);
		
		roomMgr.getRoomInfo(roomId,function(errcode,roomData){
			var sign = crypto.md5(roomId + req.ip + config.ACCOUNT_PRI_KEY);
			if(roomData == null){
				http.send(res,errcode,"create failed.");	
			}else{
				if(room == null){
					http.send(res,errcode,"create failed.");	
					roomMgr.updateRoomStatus(roomId,0);
					userMgr.updateUserRoomId(userId,"");
					return;
				}
				var ret = {
					status:200,
					errmsg:"ok",
					roomId:roomId,
					roomData:roomData,
					sign:sign 
				}
				send(res,ret);	
			}	
		});
	});
});

app.post('/add_room',function(req,res){
	http.postCallback(req,res,function(jsonData){
		var roomId = jsonData.roomId;
		var userId = jsonData.userId;
		http.httpLog(0,"http_server_add_room",JSON.stringify(jsonData));

		//查看本地缓存是否有该房间
		var room = roomMgr.getRoom(roomId);
		if(!room){
			var ret = {
				status:-1,
				errmsg:"房间已经不存在",
				roomId:roomId,
				sign:sign
			}
			send(res,ret);	
			
			//初始化用户
			userMgr.updateUserRoomId(userId,"");

			//解散房间
			roomMgr.disbandRoom();
			return;
		}

		if(roomMgr > 2){//检测房间是否已满
			var ret = {
				status:-1,
				errmsg:"房间人数已满",
				roomId:roomId,
				sign:sign
			}
			send(res,ret);	
			
			//初始化用户
			userMgr.updateUserRoomId(userId,"");

			//解散房间
			roomMgr.disbandRoom();
			return;
		}

		//更新房间信息
		roomMgr.updateRoomInfo(roomId,userId,function(reslut,seatId){
			var roomInfo = roomMgr.getRoom(roomId);
			var seatInfo = roomInfo.seats[seatId];
			
			if(reslut != 0){
				var ret = {
					status:-1,
					errmsg:"error",
					roomId:roomId,
					sign:sign
				}
				send(res,ret);	
				return;
			}
			//获取当前用户信息
			userMgr.getUserDataByUserId(userId,function(userData){
				if(userData != null){
					seatInfo.userId = userData["userid"];
					seatInfo.score = userData["coins"];
					seatInfo.name = userData["name"];
				}
				var ret = {
					status:200,
					errmsg:"ok",
					roomId:roomId,
					sign:sign
				}
				send(res,ret);	
			});
		});
	});
});

exports.start = function($config){
	config = $config;
	gameServerInfo = {
		id:config.SERVER_ID,
		clientip:config.CLIENT_IP,
		clientport:config.CLIENT_PORT,
		httpPort:config.HTTP_PORT,
		load:roomMgr.getTotalRooms(),
	};
	app.listen(config.HTTP_PORT,function(){
  		console.log("game server is listening on port:" + ":" + config.HTTP_PORT);
	});
	
};