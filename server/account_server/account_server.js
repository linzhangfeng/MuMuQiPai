var crypto = require('../utils/crypto');
var express = require('express');
var db = require('../utils/db');
var http = require("../utils/http");
var fibers = require('fibers');

var app = express();
var hallAddr = "";

function send(res,ret){
	var str = JSON.stringify(ret);
	console.log("send:"+str);
	res.send(str)
}

var config = null;

exports.start = function(cfg){
	config = cfg;
	hallAddr = config.HALL_IP  + ":" + config.HALL_CLIENT_PORT;
	app.listen(config.CLIENT_PORT);
	console.log("account server is listening on " + config.CLIENT_PORT);
}





//设置跨域访问
app.all('*', function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Access-Control-Allow-Methods","PUT,POST,GET,DELETE,OPTIONS");
    res.header("X-Powered-By",' 3.2.1')
	res.header("Content-Type", "application/json;charset=utf-8");
	fibers(function(){
		next();
	}).run();
});

app.post('/register',function(req,res){
	var str = "";

    req.on('data', data => {
		str += data
	});
  
	http.callback(req,res,function(json){
		var json = JSON.parse(str)
		console.log(json)
		var account = json.account;
		var password = json.password;
		db.is_user_exist(account,function(exist){
			if(!exist){
				db.create_account(account,password,function(ret){
					if (ret) {
						http.send(res,200,"ok.");
					}
					else{
						http.send(res,2,"create_account system error.");
					}
				});

				var name = "我是一颗小虎牙";
				var coins = 1000;
				var gems = 21;
				db.create_user(account,name,coins,gems,0,null);
			}
			else{
				fnFailed();
			}
		});
	});
});
app.get('/get_version',function(req,res){
	var ret = {
		version:config.VERSION,
	}
	send(res,ret);
});

app.post('/get_serverinfo',function(req,res){
	var ret = {
		version:config.VERSION,
		hall:hallAddr,
		appweb:config.APP_WEB,
	}
	send(res,ret);
});

app.get('/guest',function(req,res){
	var account = "guest_" + req.query.account;
	var sign = crypto.md5(account + req.ip + config.ACCOUNT_PRI_KEY);
	var ret = {
		errcode:0,
		errmsg:"ok",
		account:account,
		halladdr:hallAddr,
		sign:sign
	}
	send(res,ret);
});

app.post('/login',function(req,res){
	var str = "";
    req.on('data', data => {
		str += data
	});
  
	req.on('end', () => {
		//console.log(str)
		var json = JSON.parse(str)
		console.log("recv_account_login:"+str);
		var account = json.account;
		var password = json.password;
		db.get_account_info(account,password,function(info){
			if(info == null){
				send(res,{status:1,errmsg:"account or password error."});
				return;
			}
	
			var account = "vivi_" + json.account;
			console.log("linzhangfeng="+config.ACCOUNT_PRI_KEY);
			var sign = crypto.md5(account + req.ip + config.ACCOUNT_PRI_KEY);
			var ret = {
				status:200,
				errmsg:"ok",
				account:account,
				sign:sign
			}
			send(res,ret);
		});
	});
});

var appInfo = {
	Android:{
		appid:"wxe39f08522d35c80c",
		secret:"fa88e3a3ca5a11b06499902cea4b9c01",
	},
	iOS:{
		appid:"wxcb508816c5c4e2a4",
		secret:"7de38489ede63089269e3410d5905038",		
	}
};

function get_access_token(code,os,callback){
	var info = appInfo[os];
	if(info == null){
		callback(false,null);
	}
	var data = {
		appid:info.appid,
		secret:info.secret,
		code:code,
		grant_type:"authorization_code"
	};

	http.get2("https://api.weixin.qq.com/sns/oauth2/access_token",data,callback,true);
}

function get_state_info(access_token,openid,callback){
	var data = {
		access_token:access_token,
		openid:openid
	};

	http.get2("https://api.weixin.qq.com/sns/userinfo",data,callback,true);
}

function create_user(account,name,sex,headimgurl,callback){
	var coins = 1000;
	var gems = 21;
	db.is_user_exist(account,function(ret){
		if(!ret){
			db.create_user(account,name,coins,gems,sex,headimgurl,function(ret){
				callback();
			});
		}
		else{
			db.update_user_info(account,name,headimgurl,sex,function(ret){
				callback();
			});
		}
	});
};
app.get('/wechat_auth',function(req,res){
	var code = req.query.code;
	var os = req.query.os;
	if(code == null || code == "" || os == null || os == ""){
		return;
	}
	console.log(os);
	get_access_token(code,os,function(suc,data){
		if(suc){
			var access_token = data.access_token;
			var openid = data.openid;
			get_state_info(access_token,openid,function(suc2,data2){
				if(suc2){
					var openid = data2.openid;
					var nickname = data2.nickname;
					var sex = data2.sex;
					var headimgurl = data2.headimgurl;
					var account = "wx_" + openid;
					create_user(account,nickname,sex,headimgurl,function(){
						var sign = crypto.md5(account + req.ip + config.ACCOUNT_PRI_KEY);
					    var ret = {
					        errcode:0,
					        errmsg:"ok",
					        account:account,
					        halladdr:hallAddr,
					        sign:sign
					    };
					    send(res,ret);
					});						
				}
			});
		}
		else{
			send(res,{errcode:-1,errmsg:"unkown err."});
		}
	});
});

app.get('/base_info',function(req,res){
	var userid = req.query.userid;
	db.get_user_base_info(userid,function(data){
		var ret = {
	        errcode:0,
	        errmsg:"ok",
			name:data.name,
			sex:data.sex,
	        headimgurl:data.headimg
	    };
	    send(res,ret);
	});
});

app.get('/image', function (req, res) {
	var url = req.query.url;
	if (!url) {
	  http.send(res, 1, 'invalid url', {});
	  return;
	}
	if(url.search('http://') != 0 && url.search('https://') != 0){
		http.send(res, 1, 'invalid url', {});
		return;
	}

	url = url.split('.jpg')[0];
	

	var safe = url.search('https://') == 0;
	console.log(url);
	var ret = http.getSync(url, null, safe, 'binary');
	if (!ret.type || !ret.data) {
	  http.send(res, 1, 'invalid url', true);
	  return;
	}
	res.writeHead(200, { "Content-Type": ret.type });
	res.write(ret.data, 'binary');
	res.end();
});