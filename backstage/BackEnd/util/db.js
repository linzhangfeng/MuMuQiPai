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


//数据库的所有接口只支持增删改查操作，不进行任何逻辑判读 find add update delate
exports.find_GameGategory = function (object, callback) {
    var sql = 'SELECT * FROM Sys_DicType';
    query(sql, function (rows, fields) {
        callback(rows);
    });
}

//删除 只通过ID删除
exports.delate_GameGategory = function (object, callback) {
    var DT_ID = object["DT_ID"];
    var sql = 'DELETE FROM Sys_DicType WHERE DT_ID=' + DT_ID;
    query(sql, function (rows, fields) {
        callback(rows);
    });
}

exports.update_GameGategory = function (object, callback) {
    var DT_ID = object["DT_ID"];
    var sql = "UPDATE Sys_DicType SET runoob_title='学习 C++' WHERE runoob_id=3";
    for (var key in object) {
        if (object[key]) {
            keyStr += key + ",";
            valueStr += object[key] + ",";
        }
    }
}

exports.add_GameGategory = function (object, callback) {
    var DT_ID = generateId();
    object["DT_ID"] = DT_ID;
    var sql = 'INSERT INTO Sys_DicType';
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
        callback(rows);
    });
}