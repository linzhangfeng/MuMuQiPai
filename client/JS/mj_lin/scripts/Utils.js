Utils = {
    findNode: function (node, name) {
        if (!node) {
            return null;
        }
        if (node.getName() == name) {
            return node;
        }
        var cs = node.getChildren();
        for (var i in cs) {
            var ret = Utils.findNode(cs[i], name);
            if (ret) {
                return ret;
            }
        }
        return null;
    },
}