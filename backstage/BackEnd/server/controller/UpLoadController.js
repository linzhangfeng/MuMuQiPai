var m_resultData = require('../../util/result_data');
var m_db = require('../../util/db');
var fs = require('fs');
var fd = require("formidable"); //载入 formidable
var compressing = require('compressing');

exports.hotupdateUpLoad = function(req, res) {
    if (req.url == '/hotupdateUpLoad') {
        var form = new fd.IncomingForm();
        // 如果文件移动跨盘符依然需要提前设置上传文件的路径，默认在c盘
        form.uploadDir = 'F:/linzhangfeng/hotupdate/download/';
        form.parse(req, function(err, fields, files) {
            // console.log(filds); //表单数据
            // console.log(files); //上传文件的数据 
            // 需要将上传后的文件移动到指定目录
            console.log("下载文件成功");
            var oldPath = files.file.path;
            var newPath = 'F:/linzhangfeng/hotupdate/download/' + files.file.name;
            fs.rename(oldPath, newPath, function(err) {
                console.log("移动文件成功");
                // 压缩成zip
                // compressing.zip.compressDir('nodejs-compressing-demo', 'nodejs-compressing-demo.zip')
                //     .then(() => {
                //         console.log('success');
                //     })
                //     .catch(err => {
                //         console.error(err);
                //     });

                // 解压缩
                compressing.zip.uncompress(newPath, 'F:/linzhangfeng/hotupdate/GameVersion/')
                    .then(() => {
                        console.log('success');
                    })
                    .catch(err => {
                        console.error(err);
                    });
            });
        });
    }
}