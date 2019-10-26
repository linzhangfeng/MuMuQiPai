var m_resultData = require('../../util/result_data');
var m_db = require('../../util/db');
exports.getDicType = function (req, res) {
    //获取管理员用户ID
    var receiveData = req.query;
    var admin_id = receiveData['admin_id'];
    var sendData = {};
    m_db.find_GameGategory(null, function (data) {
        if (data) {
            console.log("lin=data=" + JSON.stringify(data));
            var arr = [];
            for (var i = 0; i < data.length; i++) {
                var obj = {};
                obj["dictype_id"] = data[i]["DT_ID"];
                obj["dictype_name"] = data[i]["DT_Name"];
                obj["dictype_createtime"] = data[i]["UA_CreateTime"];
                obj["dictype_updatetime"] = data[i]["UA_UpdateTime"];
                arr.push(obj);
            }
            sendData["list"] = arr;
        }
        var reusltData = m_resultData.init(100, "请求成功", JSON.stringify(sendData));
        res.send(reusltData.getData());
    });
}