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
cc.game.onStart = function () {
    cc.log("lin=enter=MJJS");
    // Pass true to enable retina display, on Android disabled by default to improve performance
    cc.view.enableRetina(cc.sys.os === cc.sys.OS_IOS ? true : false);

    // Adjust viewport meta
    cc.view.adjustViewPort(true);

    //load resources
    cc.LoaderScene.preload({}, function () {
        cc.director.replaceScene(new MJScene());
    }, this);
};
cc.game.run();