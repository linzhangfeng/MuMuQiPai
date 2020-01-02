
var SERVER_HALL_PORT = 9007; //服务器端口
var SERVER_GAME_PORT = 9008; //服务器端口
var SERVER_IP = "127.0.0.1";//如果非本机访问，这里要变
exports.mysql = function () {
    return {
        HOST: '119.23.221.227',
        USER: 'root',
        PSWD: '123456',//如果连接失败，请检查这里
        DB: 'chess_card',//如果连接失败，请检查这里
        PORT: 3306,
    }
}

exports.serverHall = function () {
    return {
        CLIENT_PORT: SERVER_HALL_PORT,
        CLIENT_ID: SERVER_IP,
        VERSION:'20191023',
    }
}

exports.serverGame = function () {
    return {
        CLIENT_PORT: SERVER_GAME_PORT,
        CLIENT_ID: SERVER_IP,
        VERSION:'20191023',
    }
}