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
    doLayer: function (node) {
        node.setContentSize(cc.director.getWinSize());
        ccui.helper.doLayout(node);
    },
    pad: function (num, n) {
        var len = num.toString().length;
        while (len < n) {
            num = "0" + num;
            len++;
        }
        return num + "";
    },
    addCardSprites: function (f, isjiami) {
        Utils.removeSpriteFram(f);
        Utils.addSpriteFrame(f, isjiami);
    },
    addSpriteFrame: function (f, isjiami) {
        if (isjiami == true) {
            cc.spriteFrameCache.addSpriteFrames(getResPath(__String.createWithFormat("%1.plist", f)));
        } else {
            cc.spriteFrameCache.addSpriteFrames(getResPath(__String.createWithFormat("%1.plist", f)), getResPath(__String.createWithFormat("%1.png", f)));
        }
    },
    removeSpriteFram: function (f) {
        cc.spriteFrameCache.removeSpriteFramesFromFile(getResPath(__String.createWithFormat("%1.plist", f)));
    },
    parseName: function (max, name) {
        try {
            var l = name.length;
            var blen = 0;
            var len = [];
            var n = 0;
            for (var i = 0; i < l; i++) {
                n = 0;
                if ((name.charCodeAt(i) & 0xff00) != 0) {
                    blen++;
                    n++;
                }
                blen++;
                n++;
                len[i] = n;
            }
            if (blen > max) {
                n = 0;
                for (var i = 0; i < l; i++) {
                    n += len[i];
                    if (n >= max) {
                        n = i + 1;
                        break;
                    }
                }
                return name.substr(0, n) + "..";
            }
            return name;
        } catch (e) {
            return "";
        }
    },
    createCircleAvatar: function (url, defualtFile, clipfile, size) {
        try {
            var savePath = Utils.getImageSavePath(url);

            if (Utils.hasImageFile(url)) {
                var avatar = new cc.Sprite(savePath);
                if (avatar == null) {
                    avatar = new cc.Sprite(defualtFile);
                }
                Utils.makeCircleSprite(avatar, savePath, clipfile, size);
                return avatar;
            }
            else {
                var avatar = new cc.Sprite(defualtFile);
                Utils.makeCircleSprite(avatar, defualtFile, clipfile, size);
                var tag = savePath + time(null);
                CCHttpAgent.getInstance().sendHttpImage(url, savePath, tag, function (tag) {
                    if (Utils.hasImageFile(url)) {
                        Utils.makeCircleSprite(avatar, savePath, clipfile, size);
                    }
                    CCHttpAgent.getInstance().popPackets(tag);
                }, 15);

                return avatar;
            }
        } catch (e) {
            var avatar = new cc.Sprite(defualtFile);
            var s_izex = size.width / avatar.getContentSize().width;
            var s_izey = size.height / avatar.getContentSize().height;
            avatar.setScale(s_izex, s_izey);
            return avatar;
        }

    },
    makeCircleSprite: function (avatar_s, avatafile, clipfile, size) {
        try {
            var Stencil = new cc.Sprite(clipfile);
            var avatar = new cc.Sprite(avatafile);

            if (avatar == null) {
                var s_izex = size.width / avatar_s.getContentSize().width;
                var s_izey = size.height / avatar_s.getContentSize().height;
                avatar_s.setScale(s_izex, s_izey);
                return;
            }
            var a_rect = avatar.getContentSize();
            var cl_rect = Stencil.getContentSize();

            var scaleX = (a_rect.width) / cl_rect.width;
            var scaleY = (a_rect.height) / cl_rect.height;

            var minscale = Math.min(scaleX, scaleY);
            Stencil.setScale(minscale);

            var renderTexture = new cc.RenderTexture(a_rect.width, a_rect.height);

            Stencil.setPosition(cc.p(a_rect.width / 2, a_rect.height / 2));
            avatar.setPosition(cc.p(a_rect.width / 2, a_rect.height / 2));

            Stencil.setBlendFunc(cc.ONE, cc.ZERO);//Stencil.setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
            avatar.setBlendFunc(cc.DST_ALPHA, cc.ZERO);//avatar.setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});

            renderTexture.begin();
            Stencil.visit();
            avatar.visit();
            renderTexture.end();

            avatar_s.setTexture(renderTexture.getSprite().getTexture());
            avatar_s.setFlippedY(true);
            var texture = avatar_s.getTexture();
            if (typeof texture.setAntiAliasTexParameters === "function") texture.setAntiAliasTexParameters();

            var s_izex = size.width / avatar_s.getContentSize().width;
            var s_izey = size.height / avatar_s.getContentSize().height;
            avatar_s.setScale(s_izex, s_izey);
        } catch (e) {
            try {
                var s_izex = size.width / avatar_s.getContentSize().width;
                var s_izey = size.height / avatar_s.getContentSize().height;
                avatar_s.setScale(s_izex, s_izey);
            } catch (e) {

            }
        }
    },
    hasImageFile: function (f) {
        return jsb.fileUtils.isFileExist(this.getImageSavePath(f));
    },
    getImageSavePath: function (fileUrl) {
        if (!fileUrl) {
            return;
        }
        var findFlag = "/chess_img/";
        var cutPos = fileUrl.indexOf(findFlag);
        if (cutPos <= 0) {
            return fileUrl;
        } else {
            var localPath = fileUrl.slice(cutPos + findFlag.length);
            return this.getSavePath() + "images/" + localPath;
        }
    },
    getSavePath: function () {
        return jsb.fileUtils.getWritablePath() + "Assets/";
    },
}
var time = function (v) {
    var t = (new Date).valueOf();
    return t;
};

var Log = function (obj, msg) {
    cc.log("[" + obj.tag + "]:" + msg);
}
var __String =
{
    createWithFormat: function (string) {
        try {
            var args = arguments;
            var pattern = new RegExp("%([0-9]+)", "g");
            return String(string).replace(pattern
                , function (match, index) {
                    if (index == 0 || index >= args.length)
                        throw "Invalid index in format string";
                    return args[index];
                });
        } catch (e) {
            cc.log("createWithFormat error : " + e);
            return "";
        }
    }
};