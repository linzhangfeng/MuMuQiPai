var m_pool = null;
var m_mysql = require("mysql");

exports.init = function (config) {
    m_pool = m_mysql.createPool({
        host: config.HOST,
        user: config.USER,
        password: config.PSWD,
        database: config.DB,
        port: config.PORT,
    });
};

function generateId() {
    var Id = "";
    for (var i = 0; i < 6; ++i) {
        if (i > 0) {
            Id += Math.floor(Math.random() * 10);
        } else {
            Id += Math.floor(Math.random() * 9) + 1;
        }
    }
    return Id;
}

function query(sql, callback) {
    console.log("lin=query=sql:" + sql);
    m_pool.getConnection(function (err, conn) {
        if (err) {
            if (err) {
                callback(null, fields);
                throw err;
            }
        } else {
            conn.query(sql, function (qerr, vals, fields) {
                //释放连接  
                conn.release();
                //事件驱动回调  
                if (qerr) {
                    callback(null, fields);
                    throw qerr;
                }

                if (vals.length == 0) {
                    callback(null);
                    return;
                }
                callback(vals, fields);
            });
        }
    });
};

//获取房间信息
exports.find_GameRoom = function (object, callback) {
    var GR_ID = object["GR_ID"];
    var sql = 'SELECT * FROM Game_Room WHERE GR_ID=' + GR_ID;
    query(sql,function(rows){
        callback(rows);
    });
}

//创建房间
exports.addRoom = function(object,callback){
    var GR_ID = generateId();
    object["GR_ID"] = GR_ID;
    var sql = 'INSERT INTO Game_Room';
    var keyStr = '(';
    var valueStr = '(';

    for (var key in object) {
        if (object[key]) {
            keyStr += key + ",";
            valueStr += object[key] + ",";
        }
    }
    keyStr = keyStr.substring(0, keyStr.lastIndexOf(','));
    valueStr = valueStr.substring(0, valueStr.lastIndexOf(','));

    sql = sql + keyStr + ') VALUES' + valueStr + ')';
    query(sql, function (rows, fields) {
        var data = {};
        if(rows){
            data["GR_ID"] = GR_ID;
        }else{
            data = null;
        }
        callback(data);
    });
}

//获取房间信息
exports.update_GameRoom = function (object, callback) {
    var GR_ID = object["GR_ID"];
    var sql = "UPDATE Game_Room SET ";
    for (var key in object) {
        if (object[key]) {
            sql += (key + "=" + object[key]+ ",");
        }
    }
    sql = sql.substring(0, sql.lastIndexOf(','));
    sql = sql + ' WHERE GR_ID=' + GR_ID;

    query(sql,function(){
        callback(rows);
    });
}

//获取房间信息
exports.update_UserInfo = function (object, callback) {
    var UI_ID = object["UI_ID"];
    var sql = "UPDATE User_Info SET ";
    for (var key in object) {
        if (object[key]) {
            sql += (key + "=" + object[key]+ ",");
        }
    }
    sql = sql.substring(0, sql.lastIndexOf(','));
    sql = sql + ' WHERE UI_ID=' + UI_ID;

    query(sql,function(rows){
        callback(rows);
    });
}

//数据库的所有接口只支持增删改查操作，不进行任何逻辑判读 find add update delate
exports.find_GameGategory = function (object, callback) {
    var sql = 'SELECT * FROM Sys_DicType';
    query(sql, function (rows, fields) {
        callback(rows);
    });
}
