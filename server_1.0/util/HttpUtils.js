var m_logUtils = require('./LogUtils');
require('./CommonConfig.js');
exports.receive = function (req, res, type) {
    //获取账户名
    //获取操作类型
    //获取操作内容
    if(type == OperatorType.Login){
        m_logUtils.loginLog();
    }else{
        m_logUtils.operatorLog();
    }
}

exports.response = function (req, res, type) {
    if(type == OperatorType.Login){
        m_logUtils.loginLog();
    }else{
        m_logUtils.operatorLog();
    }
}