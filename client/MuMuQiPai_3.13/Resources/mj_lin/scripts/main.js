var PackName = CommonModel.getInstance().getPackName();
if (PackName == "") {
    PackName = "mj_lin";
}
var getSrcPath = function (v) {
    return PackName + "/scripts/" + v;
}

var getResPath = function (v) {
    //cc.log("getResPath:" + v);
    return PackName + "/res/" + v;
}

require(getSrcPath("MJScene.js"));
require(getSrcPath("MJTable.js"));
require(getSrcPath("Utils.js"));
require(getSrcPath("MJLogic.js"));
require(getSrcPath("proto.js"));
require(getSrcPath("MJModel.js"));
require(getSrcPath("MJConfig.js"));
require(getSrcPath("MJPublic.js"));
require(getSrcPath("MJAction.js"));
require(getSrcPath("MJPlayer.js"));

cc.game.onStart = function () {
    cc.log("lin=enter=MJJS");
    // Pass true to enable retina display, on Android disabled by default to improve performance
    cc.view.enableRetina(cc.sys.os === cc.sys.OS_IOS ? true : false);

    // Adjust viewport meta
    cc.view.adjustViewPort(true);

    //获取配置信息
    MJModel.curRoomID = UserModel.getInstance().getCurRoomId();

    SocketClient.getInstance(MJModel.curRoomID).connect_req();

    //load resources
    cc.LoaderScene.preload({}, function () {
        cc.director.replaceScene(new MJScene());
    }, this);
};
cc.game.run();