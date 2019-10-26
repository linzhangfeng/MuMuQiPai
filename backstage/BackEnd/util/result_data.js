class ResultDataModel {
    //构造函数
    constructor() {
        this.m_code = -1;
        this.m_status = "";
        this.m_message = "";
    }

    init(code, status, message) {
        this.m_code = code;
        this.m_status = status;
        this.m_message = message;
    }
    getData() {
        var json = {
            code: this.m_code,
            status: this.m_status,
            message: this.m_message
        };
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
}


exports.init = function (code, status, message) {
    var resultData = new ResultDataModel();
    resultData.init(code, status, message);
    return resultData;
}