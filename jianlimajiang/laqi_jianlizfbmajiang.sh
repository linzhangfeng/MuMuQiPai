#!/bin/bash

cd /home/xuhui/202server/jianlimajiangzhongfabai
kill -9 `cat conf/jianlimajiangzhongfabai.pid`
rm log/*
cp /home/xuhui/linzhangfeng/jianlimajiang/mjcommon ./jianlimajiangzhongfabai -f
./jianlimajiangzhongfabai -f conf/jianlimajiangzhongfabai.conf -D
sleep 1
ps -ef | grep jianlimajiangzhongfabai
