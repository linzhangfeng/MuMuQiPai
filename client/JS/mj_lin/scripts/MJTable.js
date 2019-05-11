var MJTable = cc.Layer.extend({
    tag: "RoomMJTable",
    rootNode: null,
    ctor: function () {
        this._super();
        this.init();
    },

    init: function () {
        this.initData();
        this.updateBg();
    },

    onEnter: function () {
        this._super();
    },
    onExit: function () {
        this.release();
        this._super();

    },
    initData: function () {
        var self = this;
        var size = cc.director.getWinSize();
        var uiJson = ccs.load(getResPath("RoomMJ/StudioUI/GameTable/MJTable.json"));
        var rootNode = uiJson.node;
        rootNode.setPosition(0, 0);
        this.addChild(rootNode);
        this.rootNode = rootNode;

        var btn_test_back = Utils.findNode(this.rootNode, "btn_test_back");
        btn_test_back.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        //版本号
        var banbenhao = new cc.LabelTTF("版本号：1", "Arial", 20);
        banbenhao.setPosition(cc.p(size.width * 0.5, size.height * 0.5));
        this.addChild(banbenhao, 100);

        this.test();
    },
    window :{},
    test: function () {
        cc.log("lin=test=start=1");
        var ssio = require(getSrcPath("Net/socket-io.js"));

        var opts = {
            'reconnection':false,
            'force new connection': true,
            'transports':['websocket', 'polling']
        }
        this.sio = ssio.connect('http://127.0.0.1:9003',opts);
        this.sio.on('connection', function(msg){
            console.log('连接上了..')
        });
        cc.log("lin=test=start=2");
        //socket.emit('data', 'dagger')

    },
    btnCallback: function (ref) {
        var name = ref.getName();
        if (name == "btn_test_back") {
            CommonModel.getInstance().toHall();
        }
    },
    updateBg: function (isInit) {

    },
    release: function () {
        //SoundModel.getInstance().can_play = 1;
        this.unscheduleAllCallbacks();
        ccs.ArmatureDataManager.destroyInstance();
        cc.spriteFrameCache.removeSpriteFrames();
        cc.textureCache.removeAllTextures();
        jsb.fileUtils.purgeCachedEntries();
    },
});
