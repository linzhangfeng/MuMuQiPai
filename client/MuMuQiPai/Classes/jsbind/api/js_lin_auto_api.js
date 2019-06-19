
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
 * @method init
 */
init : function (
)
{
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
 * @method setUid
 * @param {int} arg0
 */
setUid : function (
int 
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
 * @method setExp
 * @param {int} arg0
 */
setExp : function (
int 
)
{
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
 * @method getExp
 * @return {int}
 */
getExp : function (
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
 * @method setImsi
 * @param {String} arg0
 */
setImsi : function (
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
 * @method setAccount
 * @param {String} arg0
 */
setAccount : function (
str 
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
 * @method getSex
 * @return {int}
 */
getSex : function (
)
{
    return 0;
},

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
 * @method setMoney
 * @param {int} arg0
 */
setMoney : function (
int 
)
{
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
 * @method getChannel
 * @return {String}
 */
getChannel : function (
)
{
    return ;
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
 * @method getUserName
 * @return {String}
 */
getUserName : function (
)
{
    return ;
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
