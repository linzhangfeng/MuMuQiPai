var express = require('express');
var m_dictionaryCtl = require('./controller/DictionaryController');
var app = express();

var m_config = null;
exports.start = function name(config) {
    m_config = config;
    app.listen(m_config.CLIENT_PORT);
    console.log("client service is listening on port " + m_config.CLIENT_PORT);
}

app.get('/getDicType',m_dictionaryCtl.getDicType);
