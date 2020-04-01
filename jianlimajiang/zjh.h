﻿#ifndef _LANDLORD_H_
#define _LANDLORD_H_

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include "redis_client.h"
#include "screenword.h"
#include "common/library.h"

class Game;
class Library;

class ZJH
{
public:
	std::string         conf_file;
	int                 is_daemonize;
	Json::Value         conf;
	Game                *game;
	struct ev_loop      *loop;
 	RedisClient			*main_rc[20];
	int					main_size;
	int                 pub_size;
};

#endif    /* _LANDLORD_H_ */
