var express = require('express');
var g_dictionaryCtl = require('./controller/DictionaryController');
var g_upLoadCtl = require('./controller/UpLoadController');
var app = express();

var m_config = null;
exports.start = function name(config) {
    m_config = config;
    app.listen(m_config.CLIENT_PORT);
    console.log("client service is listening on port " + m_config.CLIENT_PORT);
}
app.all("*", function(req, res, next) {
    //设置允许跨域的域名，*代表允许任意域名跨域
    res.header("Access-Control-Allow-Origin", "*");
    //允许的header类型
    res.header("Access-Control-Allow-Headers", "content-type");
    //跨域允许的请求方式 
    res.header("Access-Control-Allow-Methods", "DELETE,PUT,POST,GET,OPTIONS");
    if (req.method.toLowerCase() == 'options')
        res.send(200); //让options尝试请求快速结束
    else
        next();
});
app.get('/getDicType', g_dictionaryCtl.getDicType);
app.get('/operatorDicType', g_dictionaryCtl.operatorDicType);

app.post('/hotupdateUpLoad', g_upLoadCtl.hotupdateUpLoad);