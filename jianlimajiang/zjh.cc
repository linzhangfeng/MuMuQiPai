#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ev.h>

#include "zjh.h"
#include "game.h"
#include "common/daemonize.h"
#include "common/log.h"
#include "libnormalmahjong/game_logic.h"
#include "redis_cache.h"


static int parse_arg(int argc, char **argv);
static int init_conf();
static void dump_conf();
static int set_rlimit(int n);
static int init_redis();

ZJH zjh;
Log log;

int main(int argc, char **argv)
{
	int ret;
	srand(time(NULL));	
	ret = parse_arg(argc, argv);
	if (ret < 0) {
		log.fatal("File: %s Func: %s Line: %d => parse_arg.\n", __FILE__, __FUNCTION__, __LINE__);
		exit(1);
	}
	ret = init_conf();
	if (ret < 0) {
		log.fatal("File: %s Func: %s Line: %d => init_conf.\n", __FILE__, __FUNCTION__, __LINE__);
		exit(1);
	}
	dump_conf();

	if (zjh.is_daemonize == 1)
		daemonize();
	signal(SIGPIPE, SIG_IGN);

	ret = single_instance_running(zjh.conf.get("pid_file", "conf/normalmahjong.pid").asString().c_str());
	if (ret < 0) {
		log.fatal("File: %s Func: %s Line: %d => single_instance_running.\n", __FILE__, __FUNCTION__, __LINE__);
		exit(1);
    }

	log.start(zjh.conf["log"].get("log_file", "log/normalmahjong.log").asString(),
			zjh.conf["log"].get("level", 5).asInt(),
			zjh.conf["log"].get("console", 0).asInt(),
			zjh.conf["log"].get("rotate", 1).asInt(),
			zjh.conf["log"].get("max_size", 1073741824).asInt(),
			zjh.conf["log"].get("max_file", 10).asInt());
	
	set_rlimit(10240);

	if (zjh.conf["open_redis"].asInt() || zjh.conf["tables"]["open_datasave"].asInt())
	{
		ret = init_redis();
		if (ret < 0) //connect redis
		{
			log.fatal("init redis failed\n");
			exit(1);
		}
	}

	struct ev_loop *loop = ev_default_loop(0);
	zjh.loop = loop;
	zjh.game = new (std::nothrow) Game();
	zjh.game->start();

	if ( zjh.conf["open_redis"].asInt() )
	{
		CRedisCache::GetInstance().StartClearTime(zjh.conf["timers"]["redis_call_ptime"].asInt());
	}	

	ev_loop(loop, 0);
	return 0;
}

static int parse_arg(int argc, char **argv)
{
	int flag = 0;
	int oc; /* option chacb. */
	char ic; /* invalid chacb. */

	zjh.is_daemonize = 0;
	while ((oc = getopt(argc, argv, "Df:")) != -1) {
		switch (oc) {
		case 'D':
			zjh.is_daemonize = 1;
			break;
		case 'f':
			flag = 1;
			zjh.conf_file = string(optarg);
			break;
		case '?':
			ic = (char) optopt;
			printf("invalid \'%c\'\n", ic);
			break;
		case ':':
			printf("lack option arg\n");
			break;
		}
	}

	if (flag == 0)
		return -1;

	return 0;
}

static int init_conf()
{
	std::ifstream in(zjh.conf_file.c_str(), std::ifstream::binary);
	if (!in) {
		std::cout << "init file no found." << endl;
		return -1;
	}

	Json::Reader reader;
	bool ret = reader.parse(in, zjh.conf);
	if (!ret) {
		in.close();
		std::cout << "init file parser." << endl;
		return -1;
	}

	in.close();
	return 0;
}

static void dump_conf()
{
	std::cout << "program on .." << endl;
	
}

static int set_rlimit(int n)
{
	struct rlimit rt;
	rt.rlim_max = rt.rlim_cur = n;
	if (setrlimit(RLIMIT_NOFILE, &rt) == -1) {
		log.error("File: %s Func: %s Line: %d => setrlimit %s.\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		return -1;
	}

	return 0;
}


static int init_redis()
{
	int ret;
	
	zjh.main_size = zjh.conf["main-db"].size();
	for (int i = 0; i < zjh.main_size; i++) {
		zjh.main_rc[i] = new RedisClient();
		ret = zjh.main_rc[i]->init(zjh.conf["main-db"][i]["host"].asString(),
				zjh.conf["main-db"][i]["port"].asInt(), 1000, zjh.conf["main-db"][i]["pass"].asString());
		if (ret < 0) {
			log.error("main db redis error\n");
			return -1;
		}
	}

//     zjh.eventlog_rc = new RedisClient();
//     ret = zjh.eventlog_rc->init(zjh.conf["eventlog-db"]["host"].asString(),
//     		zjh.conf["eventlog-db"]["port"].asInt(), 1000, zjh.conf["eventlog-db"]["pass"].asString());
//     if (ret < 0) {
//     	log.error("eventlog db redis error.\n");
//     	return -1;
//     }
// 
//     // init pub
//     zjh.pub_size = zjh.conf["pub-db"].size();
//     for (int i = 0; i < zjh.pub_size; ++i) {
//     	zjh.pub_rc[i] = new RedisClient();
// 		ret = zjh.pub_rc[i]->init(zjh.conf["pub-db"][i]["host"].asString(),
// 				zjh.conf["pub-db"][i]["port"].asInt(), 1000, zjh.conf["pub-db"][i]["pass"].asString());
// 		if (ret < 0) {
// 			log.error("pub_rc db redis error.\n");
// 			return -1;
// 		}
// 	}
	
    return 0;
}
