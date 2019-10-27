class ResultDataModel {
    //构造函数
    constructor() {
        this.m_code = -1;
        this.m_status = "";
        this.m_message = "";
        this.m_data = "";
    }

    init(code, status, message, data) {
        this.m_code = code;
        this.m_status = status;
        this.m_message = message;
        this.m_data = data;
    }
    getDataStr() {
        var json = {
            code: this.m_code,
            status: this.m_status,
            message: this.m_message,
            data: this.m_data
        };
        console.log("lin=sendData="+JSON.stringify(json));
        return JSON.stringify(json);
    }
    setCode(code) {
        this.m_code = code;
    }
    getCode() {
        return this.m_code;
    }
    setStatus(status) {
        this.m_status = status;
    }
    getStatus() {
        return this.m_status;
    }
    getMessage(msg) {
        this.m_message = msg;
    }
    setMessage() {
        return this.m_message;
    }
    getMessage(msg) {
        this.m_message = msg;
    }
    setMessage() {
        return this.m_message;
    }
    getData(data) {
        this.m_data = data;
    }
    setData() {
        return this.m_data;
    }
}


exports.init = function (code, status, message, sendData) {
    var resultData = new ResultDataModel();
    resultData.init(code, status, message, sendData);
    return resultData;
}