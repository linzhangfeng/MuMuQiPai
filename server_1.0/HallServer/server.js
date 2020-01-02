var express = require('express');
var m_roomCtl = require('./controller/RoomController');
var app = express();

var m_config = null;
exports.start = function name(config) {
    m_config = config;
    app.listen(m_config.CLIENT_PORT);
    console.log("client service is listening on port " + m_config.CLIENT_PORT);
}

app.get('/createRoom',m_roomCtl.createRoom);
app.get('/addRoom',m_roomCtl.addRoom);