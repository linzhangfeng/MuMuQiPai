var MJScene = cc.Scene.extend({
    onEnter: function () {
        this._super();
        var stable = createTable();
        this.addChild(stable);
    }
});

var createTable = function () {
    var layer = new MJTable();
    return layer;
};