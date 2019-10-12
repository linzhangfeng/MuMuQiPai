var db = require('../utils/db');
var handlerCMD = require('./proto');
var userMgr = require('./usermgr');

var rooms = {};
var creatingRooms = {};

var userLocation = {};
var totalRooms = 0;

var DI_FEN = [1,2,5];
var MAX_FAN = [3,4,5];
var JU_SHU = [4,8];
var JU_SHU_COST = [2,3];

function generateRoomId(){
	var roomId = "";
	for(var i = 0; i < 6; ++i){
		roomId += Math.floor(Math.random()*10);
	}
	return roomId;
}

function constructRoomFromDb(dbdata){
	var roomInfo = {
		uuid:dbdata.uuid,
		id:dbdata.id,
		numOfGames:dbdata.num_of_turns,
		createTime:dbdata.create_time,
		nextButton:dbdata.next_button,
		seats:new Array(4),
		conf:JSON.parse(dbdata.base_info)
	};


	if(roomInfo.conf.type == "xlch"){
		roomInfo.gameMgr = require("./gamemgr_xlch");
	}
	else{
		roomInfo.gameMgr = require("./gamemgr_xzdd");
	}
	var roomId = roomInfo.id;

	for(var i = 0; i < 4; ++i){
		var s = roomInfo.seats[i] = {};
		s.userId = dbdata["user_id" + i];
		s.score = dbdata["user_score" + i];
		s.name = dbdata["user_name" + i];
		s.ready = false;
		s.seatId = i;
		s.numZiMo = 0;
		s.numJiePao = 0;
		s.numDianPao = 0;
		s.numAnGang = 0;
		s.numMingGang = 0;
		s.numChaJiao = 0;

		if(s.userId > 0){
			userLocation[s.userId] = {
				roomId:roomId,
				seatId:i
			};
		}
	}
	rooms[roomId] = roomInfo;
	totalRooms++;
	return roomInfo;
}

exports.createRoom = function(creator,roomConf,gems,ip,port,callback){
	if(
		roomConf.type == null
		|| roomConf.difen == null
		|| roomConf.zimo == null
		|| roomConf.jiangdui == null
		|| roomConf.huansanzhang == null
		|| roomConf.zuidafanshu == null
		|| roomConf.jushuxuanze == null
		|| roomConf.dianganghua == null
		|| roomConf.menqing == null
		|| roomConf.tiandihu == null){
		callback(1,null);
		return;
	}

	if(roomConf.difen < 0 || roomConf.difen > DI_FEN.length){
		callback(1,null);
		return;
	}

	if(roomConf.zimo < 0 || roomConf.zimo > 2){
		callback(1,null);
		return;
	}

	if(roomConf.zuidafanshu < 0 || roomConf.zuidafanshu > MAX_FAN.length){
		callback(1,null);
		return;
	}

	if(roomConf.jushuxuanze < 0 || roomConf.jushuxuanze > JU_SHU.length){
		callback(1,null);
		return;
	}
	
	var cost = JU_SHU_COST[roomConf.jushuxuanze];
	if(cost > gems){
		callback(2222,null);
		return;
	}

	var fnCreate = function(){
		var roomId = generateRoomId();
		if(rooms[roomId] != null || creatingRooms[roomId] != null){
			fnCreate();
		}
		else{
			creatingRooms[roomId] = true;
			db.is_room_exist(roomId, function(ret) {

				if(ret){
					delete creatingRooms[roomId];
					fnCreate();
				}
				else{
					var createTime = Math.ceil(Date.now()/1000);
					var roomInfo = {
						uuid:"",
						id:roomId,
						numOfGames:0,
						createTime:createTime,
						nextButton:0,
						seats:[],
						UserDisbandstate:[],
						UserOnlinestate:[],
						UserOvertime:[],
						conf:{
							type:roomConf.type,
							baseScore:DI_FEN[roomConf.difen],
						    zimo:roomConf.zimo,
						    jiangdui:roomConf.jiangdui,
						    hsz:roomConf.huansanzhang,
						    dianganghua:parseInt(roomConf.dianganghua),
						    menqing:roomConf.menqing,
						    tiandihu:roomConf.tiandihu,
						    maxFan:MAX_FAN[roomConf.zuidafanshu],
						    maxGames:JU_SHU[roomConf.jushuxuanze],
						    creator:creator,
						}
					};
					
					if(roomConf.type == "xlch"){
						roomInfo.gameMgr = require("./gamemgr_xlch");
					}
					else{
						roomInfo.gameMgr = require("./gamemgr_xzdd");
					}
					console.log(roomInfo.conf);
					
					var seats = {};
					for(var i = 0; i < 4; ++i){
						roomInfo.seats.push({
							userId:0,
							score:0,
							name:"",
							ready:false,
							seatId:i,
						});
					}
					
					//写入数据库
					var conf = roomInfo.conf;
					db.create_room(roomInfo.id,roomInfo.conf,ip,port,createTime,seats,function(uuid){
						delete creatingRooms[roomId];
						if(uuid != null){
							roomInfo.uuid = uuid;
							rooms[roomId] = roomInfo;
							totalRooms++;
							callback(0,roomId);
						}
						else{
							callback(3,null);
						}
					});
				}
			});
		}
	}

	fnCreate();
};

exports.addRoom = function(roomId,userId,callback){

};

exports.destroy = function(roomId){
	var roomInfo = rooms[roomId];
	if(roomInfo == null){
		return;
	}

	for(var i = 0; i < 4; ++i){
		var userId = roomInfo.seats[i].userId;
		if(userId > 0){
			delete userLocation[userId];
			db.updateUsers_roomid(userId,null);
		}
	}
	
	delete rooms[roomId];
	totalRooms--;
	db.delete_room(roomId);
}

exports.getTotalRooms = function(){
	return totalRooms;
}

exports.getRoom = function(roomId){
	return rooms[roomId];
};

exports.isCreator = function(roomId,userId){
	var roomInfo = rooms[roomId];
	if(roomInfo == null){
		return false;
	}
	return roomInfo.conf.creator == userId;
};

exports.updateRoomInfo = function (roomId,userId,callback) {
	callback = callback == null? nop:callback;
	exports.getRoomInfo(roomId,function(errcode,roomData){
		if(roomData != null){
			var seatId = -1;
			var seatsStr = roomData["seats"];
			var seatsJson = JSON.parse(seatsStr);
			for(var i = 0; i < 4;i ++){
				if(seatsJson[i] == ""){
					seatsJson[i] = userId;
					seatId = i;
					break;
				}
			}
			db.update_user_roomid(userId,roomId);
			db.update_seats_info(roomId,seatsJson);
			callback(0,seatId);
		}	
	});
};

exports.enterRoom = function(roomId,userId,userName,callback){
	var fnTakeSeat = function(room){
		if(exports.getUserRoom(userId) == roomId){
			//已存在
			return 0;
		}

		for(var i = 0; i < 4; ++i){
			var seat = room.seats[i];
			if(seat.userId <= 0){
				seat.userId = userId;
				seat.name = userName;
				userLocation[userId] = {
					roomId:roomId,
					seatId:i
				};
				//console.log(userLocation[userId]);
				db.update_seat_info(roomId,i,seat.userId,"",seat.name);
				//正常
				return 0;
			}
		}	
		//房间已满
		return 1;	
	}
	var room = rooms[roomId];
	if(room){
		var ret = fnTakeSeat(room);
		callback(ret);
	}
	else{
		db.get_room_data(roomId,function(dbdata){
			if(dbdata == null){
				//找不到房间
				callback(2);
			}
			else{
				//construct room.
				room = constructRoomFromDb(dbdata);
				//
				var ret = fnTakeSeat(room);
				callback(ret);
			}
		});
	}
};

exports.getRoomInfo = function(roomId,callback){
	db.get_room_data(roomId,function(dbdata){
		if(dbdata == null){
			//找不到房间
			callback(2);
		}
		else{
			room = constructRoomFromDb(dbdata);
			dbdata.base_info = JSON.parse(dbdata.base_info);
			callback(0,dbdata);
		}
	});
};
exports.setReady = function(userId,value){
	var roomId = exports.getUserRoom(userId);
	if(roomId == null){
		return;
	}

	var room = exports.getRoom(roomId);
	if(room == null){
		return;
	}

	var seatIndex = exports.getUserSeat(userId);
	if(seatIndex == null){
		return;
	}

	var s = room.seats[seatIndex];
	s.ready = value;
}

exports.isReady = function(userId){
	var roomId = exports.getUserRoom(userId);
	if(roomId == null){
		return;
	}

	var room = exports.getRoom(roomId);
	if(room == null){
		return;
	}

	var seatIndex = exports.getUserSeat(userId);
	if(seatIndex == null){
		return;
	}

	var s = room.seats[seatIndex];
	return s.ready;	
}


exports.getUserRoom = function(userId){
	var location = userLocation[userId];
	if(location != null){
		return location.roomId;
	}
	return null;
};

exports.getUserSeatId = function(userId){
	var location = userLocation[userId];
	if(location != null){
		return location.seatId;
	}
	return null;
};

exports.getUserLocations = function(){
	return userLocation;
};

exports.exitRoom = function(userId){
	var location = userLocation[userId];
	if(location == null){
		db.updateUsers_roomid(userId,null,function(isOk){
			console.log("isOk="+isOk);
			if(isOk){
				var sendData = {};
				sendData.data = {"state":1};
				sendData.id = handlerCMD.CMD.SERVER_DISBAND_ROOM_SUCC_BC;
				userMgr.sendMsg(userId,'data',sendData);
			}
		});
		return;
	}

	var roomId = location.roomId;
	var seatIndex = location.seatIndex;
	var room = rooms[roomId];
	delete userLocation[userId];
	if(room == null || seatIndex == null) {
		return;
	}

	var seat = room.seats[seatIndex];
	seat.userId = 0;
	seat.name = "";

	var numOfPlayers = 0;
	for(var i = 0; i < room.seats.length; ++i){
		if(room.seats[i].userId > 0){
			numOfPlayers++;
		}
	}
	
	db.updateUsers_roomid(userId,null);
	db.updateRooms_room_state(roomId,1);
	if(numOfPlayers == 0){
		exports.destroy(roomId);
	}
};

exports.setUserSeatId = function(userId,roomId){
	//如果已经分配，
	var seatId = exports.getUserSeatId(userId);
	if(seatId != null){
		return seatId;
	}else{
		var roomInfo = exports.getRoom(roomId);
		var playersInfo = roomInfo.seats;
		for(var i = 0;i < playersInfo.length;i++ ){
			var info = playersInfo[i];
			if(info.userId == 0){
				return i;
			}
		}
		return -1;//房间已满
	}
};

//获取房间人数
exports.getUserNum = function(roomId){
	var count = 0;
	var room = exports.getRoom(roomId);
	var playersInfo  = room["playersInfo"];
	for(var i = 0 ; i < playersInfo.length;i++){
		var info = playersInfo[i];
		if(info.userId != 0){
			count++;
		}
	}
	return count;
};

exports.disbandRoom = function(roomId){
	exports.destroy(0);
	exports.updateRoomStatus(roomId,0);
};

exports.updateRoomStatus = function(roomId,status){
	db.updateRooms_room_state(roomId,status);
}

exports.checkRoomRunning = function(){

}