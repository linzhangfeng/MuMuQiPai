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

function query(sql,callback){  
    m_pool.getConnection(function(err,conn){  
        if(err){  
            callback(err,null,null);  
        }else{  
            conn.query(sql,function(qerr,vals,fields){  
                //释放连接  
                conn.release();  
                //事件驱动回调  
                callback(qerr,vals,fields);  
            });  
        }  
    });  
};


//数据库的所有接口只支持增删改查操作，不进行任何逻辑判读
exports.find_GameGategory = function(object,callback){
    var sql = 'SELECT * FROM Sys_DicType';
    if(!object){

    }
    query(sql, function(err, rows, fields) {
        if (err) {
            callback(null);
            throw err;
        }

        if(rows.length == 0){
            callback(null);
            return;
        }

        callback(rows);
    });
}

exports.update_GameGategory = function(){
    
}

exports.add_GameGategory = function(){
    
}

exports.delate_GameGategory = function(){
    
}