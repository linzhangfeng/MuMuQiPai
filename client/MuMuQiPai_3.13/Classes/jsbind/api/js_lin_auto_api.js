
/**
 * @class PlatformHelper
 */
PlatformHelper = {

/**
 * @method showToast
 * @param {String} arg0
 * @param {int} arg1
 */
showToast : function (
str, 
int 
)
{
},

};

/**
 * @class CommonModel
 */
CommonModel = {

/**
 * @method hallToRoomV
 */
hallToRoomV : function (
)
{
},

/**
 * @method getPackName
 * @return {String}
 */
getPackName : function (
)
{
    return ;
},

/**
 * @method toHall
 */
toHall : function (
)
{
},

/**
 * @method setRoomData
 * @param {String} arg0
 */
setRoomData : function (
str 
)
{
},

/**
 * @method init
 */
init : function (
)
{
},

/**
 * @method getRoomData
 * @return {String}
 */
getRoomData : function (
)
{
    return ;
},

/**
 * @method hallToRoomH
 */
hallToRoomH : function (
)
{
},

/**
 * @method getInstance
 * @return {CommonModel}
 */
getInstance : function (
)
{
    return CommonModel;
},

/**
 * @method CommonModel
 * @constructor
 */
CommonModel : function (
)
{
},

};

/**
 * @class UserModel
 */
UserModel = {

/**
 * @method setUserName
 * @param {String} arg0
 */
setUserName : function (
str 
)
{
},

/**
 * @method getCurRoomId
 * @return {String}
 */
getCurRoomId : function (
)
{
    return ;
},

/**
 * @method setCurRoomId
 * @param {String} arg0
 */
setCurRoomId : function (
str 
)
{
},

/**
 * @method getImsi
 * @return {String}
 */
getImsi : function (
)
{
    return ;
},

/**
 * @method setChannel
 * @param {String} arg0
 */
setChannel : function (
str 
)
{
},

/**
 * @method getSex
 * @return {int}
 */
getSex : function (
)
{
    return 0;
},

/**
 * @method setImsi
 * @param {String} arg0
 */
setImsi : function (
str 
)
{
},

/**
 * @method getMoney
 * @return {int}
 */
getMoney : function (
)
{
    return 0;
},

/**
 * @method getLevel
 * @return {int}
 */
getLevel : function (
)
{
    return 0;
},

/**
 * @method getIp
 * @return {String}
 */
getIp : function (
)
{
    return ;
},

/**
 * @method getAccount
 * @return {String}
 */
getAccount : function (
)
{
    return ;
},

/**
 * @method getUid
 * @return {int}
 */
getUid : function (
)
{
    return 0;
},

/**
 * @method getUserName
 * @return {String}
 */
getUserName : function (
)
{
    return ;
},

/**
 * @method setLevel
 * @param {int} arg0
 */
setLevel : function (
int 
)
{
},

/**
 * @method setAccount
 * @param {String} arg0
 */
setAccount : function (
str 
)
{
},

/**
 * @method setExp
 * @param {int} arg0
 */
setExp : function (
int 
)
{
},

/**
 * @method getChannel
 * @return {String}
 */
getChannel : function (
)
{
    return ;
},

/**
 * @method getExp
 * @return {int}
 */
getExp : function (
)
{
    return 0;
},

/**
 * @method setUid
 * @param {int} arg0
 */
setUid : function (
int 
)
{
},

/**
 * @method setMoney
 * @param {int} arg0
 */
setMoney : function (
int 
)
{
},

/**
 * @method setSex
 * @param {int} arg0
 */
setSex : function (
int 
)
{
},

/**
 * @method setIp
 * @param {String} arg0
 */
setIp : function (
str 
)
{
},

/**
 * @method getInstance
 * @return {UserModel}
 */
getInstance : function (
)
{
    return UserModel;
},

/**
 * @method UserModel
 * @constructor
 */
UserModel : function (
)
{
},

};

/**
 * @class SocketRequest
 */
SocketRequest = {

};

/**
 * @class SocketResponse
 */
SocketResponse = {

/**
 * @method release
 */
release : function (
)
{
},

/**
 * @method parseDatta
 * @return {String}
 */
parseDatta : function (
)
{
    return ;
},

};

/**
 * @class SocketClient
 */
SocketClient = {

/**
 * @method connect_req
 */
connect_req : function (
)
{
},

/**
 * @method getJsonStr
 * @param {SocketRequest} arg0
 * @return {String}
 */
getJsonStr : function (
socketrequest 
)
{
    return ;
},

/**
 * @method seCurRoomId
 * @param {String} arg0
 */
seCurRoomId : function (
str 
)
{
},

/**
 * @method setConnectState
 * @param {bool} arg0
 */
setConnectState : function (
bool 
)
{
},

/**
 * @method get
 * @return {SocketResponse}
 */
get : function (
)
{
    return SocketResponse;
},

/**
 * @method onError
 * @param {cc.network::SIOClient} arg0
 * @param {String} arg1
 */
onError : function (
sioclient, 
str 
)
{
},

/**
 * @method onClose
 * @param {cc.network::SIOClient} arg0
 */
onClose : function (
sioclient 
)
{
},

/**
 * @method getCurRoomId
 * @return {String}
 */
getCurRoomId : function (
)
{
    return ;
},

/**
 * @method create_response
 * @param {int} arg0
 * @param {String} arg1
 * @param {int} arg2
 * @param {int} arg3
 * @param {int} arg4
 */
create_response : function (
int, 
str, 
int, 
int, 
int 
)
{
},

/**
 * @method reset_response_queue
 */
reset_response_queue : function (
)
{
},

/**
 * @method init
 */
init : function (
)
{
},

/**
 * @method isConnect
 * @return {bool}
 */
isConnect : function (
)
{
    return false;
},

/**
 * @method send_Data
 * @param {String} arg0
 * @param {int} arg1
 */
send_Data : function (
str, 
int 
)
{
},

/**
 * @method put
 * @param {SocketRequest} arg0
 */
put : function (
socketrequest 
)
{
},

/**
 * @method data_req
 * @param {SocketRequest} arg0
 */
data_req : function (
socketrequest 
)
{
},

/**
 * @method disconnect_req
 * @param {SocketRequest} arg0
 */
disconnect_req : function (
socketrequest 
)
{
},

/**
 * @method getInstance
 * @return {SocketClient}
 */
getInstance : function (
)
{
    return SocketClient;
},

/**
 * @method SocketClient
 * @constructor
 */
SocketClient : function (
)
{
},

};
