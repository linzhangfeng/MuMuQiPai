#ifndef __PROTO_H__
#define __PROTO_H__

enum system_command
{
	SYS_ECHO						= 0001,       /* echo */
	SYS_ONLINE						= 0002,       /* get online */
	SYS_SETUP                		= 0003,       /* setup table */
    SYS_GET_USER_NUM         		= 0004,      //获取房间人数

	SERVER_REGISTER_REQ          	= 7000, //玩牌服务器注册
	LINK_DB_CLIENT_BREAK         	= 7001, //数据中心与数据库服务器断开连接
	SERVER_CONFIGURE_REQ         	= 7002, //请求获取玩牌服务器当前配置
	SERVER_CONFIGURE_RES         	= 7003, //返回玩牌服务器当前配置
	UPDATE_SERVER_CONFIGURE_REQ  	= 7004, //请求更新玩牌服务器配置
	UPDATE_SERVER_CONFIGURE_RES  	= 7005, //返回更新玩牌服务器后的配置
	SERVER_UPDATE_MONEY          	= 7006, //非玩牌服务器请求更新同步玩家金币到玩牌服务器
	SERVER_UPDATE_USER_INFO       	= 7007, //玩牌服务器请求更新玩家数据库数据与玩家在其他玩牌服务器内存数据
	SERVER_UPDATE_DB_TASK_REQ       = 7008, //玩牌服务器请求更新玩家数据库任务数据
	SERVER_UPDATE_DB_TALLY_REQ      = 7009, //玩牌服务器请求更新流水信息到数据库
	SERVER_SPEAK_INFO_WRITE_REQ     = 7010, //玩牌服务器请求更新大喇叭信息到数据库
	SERVER_USER_BROKE_WRITE_REQ     = 7011, //玩牌服务器请求更新玩家破产信息到数据库
	SERVER_USER_ZID_INFO_WRITE_REQ  = 7012, //玩牌服务器请求更新玩家当前场次的vid，zid信息

	SERVER_ADD_ROBOT_MONEY          = 7104, //玩牌服务器请求加机器人金币
	SERVER_INFORM_USER_VID_TO_DATASV= 7105, //玩牌服务器通知datasvr玩家成功进入该场
	SERVER_SET_BANKER_TAG_TO_DB    	= 7106,  //玩牌服务器请求存储当庄标志
	SERVER_SAVE_POOL_TO_DB         	= 7107, //玩牌服务器请求存储奖池数据
	SERVER_GET_POOL_FROM_DB        	= 7108, //玩牌服务器请求当前桌子奖池数据 
	SERVER_GET_POOL_BACK           	= 7109, //数据库返回奖池数据给玩牌服务器
	SERVER_SAVE_USER_OTHER_INFO    	= 7110, //玩牌服务器存储玩家其他相关的数据
	SERVER_SAVE_GAME_INFO          	= 7111, //玩牌服务器存储其他游戏相关数据
	SERVER_GET_GAME_INFO           	= 7112, //玩牌服务器请求获取其他游戏相关数据
	SERVER_GET_GAME_INFO_BACK      	= 7113, //返回游戏数据给玩牌服务器
	SERVER_LAOHUJI_INFO            	= 7114, //玩牌服务器推送老虎机消息到流水DB与大喇叭服务器
	SERVER_INCR_USER_TIPS		   	= 7115, //玩牌服务器请求增减玩家免费提示次数
	
	SERVER_RECONNECT_REQ		   	= 7201, //平台用于统计客户端重连
	SERVER_DZ_GAME_START_REQ	   	= 7210, /* 对战模式玩牌服务器请求游戏开始 */
	SERVER_DZ_GAME_START_RES	   	= 7211, /* 对战模式玩牌服务器请求游戏开始返回 */
	SERVER_DZ_UPDATE_USER_INFO	   	= 7212, /* 对战模式玩牌服务器更新玩家个人信息 */
	SERVER_DZ_UPDATE_USER_INFO_LITTLE = 7213, /*对战模式玩牌服务器更新玩家个人信息小结算 */
	SERVER_DISBAND_ROOM			   	= 7214, //解散房间请求（游戏服务器发送给后台服务器）
	SERVER_DISBAND_ROOM_RESP	   	= 7215, //解散房间成功返回	
	SERVER_ALL_PLAYER_DOWN		   	= 7217, //玩牌服务器通知datasvr 房间里所有的玩家已经断线或者退出
	SERVER_DELETE_ROOM_REQ  	   	= 7219, //datasvr发送给玩牌服务器，查询是否可以删掉房间
	SERVER_DELETE_ROOM_RESP		   	= 7218, //玩牌服务器发送给datasvr， 回复是否可以删掉房间
	SERVER_LOGOUT_ROOM_REQ		   	= 7220, // 玩牌服务器请求退出房间
	SERVER_LOGOUT_ROOM_RESP	       	= 7221, // 退出房间成功返回
	SERVER_FORCE_LOGOUT_ROOM_REQ	= 7222, //强制退出房间
	SERVER_FORCE_LOGOUT_ROOM_RESP	= 7223,	//强制退出房间返回
	SERVER_USER_STATUS_NOTIFY		= 7226,	//玩家状态通知
	SERVER_USER_STATUS_NOTIFY_RESP	= 7227,	//玩家状态通知返回

	SERVER_DZ_STRIP					= 7300,	//平台强制用户退出

	SERVER_DISBAND_ROOM_NOTIFY		= 7400,	//平台发送解散的通知
	SERVER_DZ_SWITCH_PLATFORM 		= 7500,	//平台切服
	SERVER_DZ_SWITCH_PLATFORM_ACK 	= 7501,	//回平台切服消息	
};

enum client_command
{
	CLIENT_LOGIN_REQ				= 1001,       /* join table */
	CLIENT_LOGOUT_REQ				= 1002,
	CLIENT_READY_REQ				= 1003,	      /* game ready */
	CLIENT_BET_REQ		 			= 1004,
	CLIENT_CHAT_REQ					= 1005,
	CLIENT_FACE_REQ					= 1006,
	CLIENT_CHANGE_REQ            = 1007,       /* change table */
	CLIENT_ROBOT_REQ             = 1008,
	CLIENT_INFO_REQ              = 1009,       /* update player info */
	CLIENT_TABLE_INFO_REQ        = 1010,      /* table info */
	CLIENT_EMOTION_REQ           = 1011,      /* interaction emotion */
	CLIENT_PROP_REQ              = 1012,      /* game prop */
	CLIENT_BOX_REQ               = 1013,
	CLIENT_ACTIVE_REQ            = 1014,
	CLIENT_BETTING_REQ           = 1015,
	CLIENT_UPTABLE_INFO_REQ      = 1016,
	CLIENT_UPTABLE_APPLY_REQ     = 1017,   //上桌
	CLIENT_DOWNTABLE_AHEAD_REQ   = 1018,
	CLIENT_DOWNTABLE_REQ         = 1019,   // 下桌
	CLIENT_RETURN_REQ            = 1020,
	CLIENT_DAEMONIZE_REQ         = 1021,
	CLINET_UPBANKER_REQ          = 1022,
	CLINET_UPBANKER_INFO_REQ     = 1023,
	CLINET_DOWN_BANKER_REQ       = 1024,
	CLINET_HEART_BEAT_REQ        = 1025,
	CLINET_TREND_REQ             = 1040,
	CLIENT_ALL_PLAYERS_REQ       = 1051,
	CLIENT_UP_GUEST_REQ          = 1052,
	CLIENT_DOWN_GUEST_REQ        = 1053,
	CLIENT_POOL_INFO_REQ         = 1054,
	CLINET_OTHER_SHOW_REQ        = 1056,
	CLIENT_CHOOSE_CARDS_REQ		 = 1057,
	CLIENT_TIP					 = 1059,	
	CLIENT_CHOSE_BETS_NUM		 = 1060,
	CLIENT_DETAL_INFO_REQ        = 1070,
	CLIENT_RANK_REQ              = 1080,
	CLIENT_HISTORY_REQ           = 1090,
	CLIENT_CONFIRM_REQ           = 1091, //请求确认

	//麻将协议
	CLIENT_MJ_CHANGE_CARD		 = 1100, //请求换牌
	CLIENT_MJ_CHOOSE_LACKTYPE	 = 1101, //选择定缺色
	CLIENT_MJ_OUT_CARD			 = 1102, //出牌
	CLIENT_MJ_OPERATE_CARD	 	 = 1103, //选择操作
	CLIENT_MJ_AUTOHU			 = 1104, //自动胡牌
	CLIENT_MJ_TRUSTEE			 = 1105, //托管
	CLIENT_MJ_USER_ACTION		 = 1106, //玩家选择准备、离开、请求换桌
	CLIENT_MJ_HOME_RETURN		 = 1107, //客户端按home键返回
	CLIENT_MJ_CHOOSE_PIAO		 = 1108, //选择飘
	CLIENT_MJ_LISTEN_TYPE		 = 1109, //玩家选择听牌(报听、躺牌、首张报听)
	CLINET_MJ_TIAN_TING			 = 1110, //玩家天听
	CLINET_MJ_YOUJIN			 = 1111, //游金按钮消息
	CLIENT_MJ_CHOOSE_JIA		 = 1112, //选择夹，应城卡五星亮牌操作，宜昌麻将扑三张操作
	CLIENT_MJ_PUCARD_REQ		 = 1113, //请求扑牌
	CLIENT_MJ_OPERATE_CARD2		 = 1114, //请求操作2
	
	CLIENT_REAL_TIME_SPEECH      = 1118, //客户端请求实时语音

	CLIENT_MJ_SHOW_REQ			 = 1120, //襄阳请求亮牌信息
	/* 对战 */
	CLIENT_DZ_LOGIN_REQ			 = 1200, //登陆请求
	
	CLIENT_MJ_VIDEO_DATA_REQ     = 1300, //录像数据请求 
	CLIENT_HOME_OUT              = 1400, //玩家按home键离开
	CLIENT_REQ_ROUND_SCORE		 = 1401, //战绩
	CLIENT_REQ_CHANGE_MODE		 = 1500, //请求修改模式
	CLIENT_GAME_END_RECORD_REG   = 1600, //玩家请求总结算信息
	CLIENT_MJ_ZUOMA 			 = 1601, //请求坐马
	CLIENT_PLAYER_GPS 			 = 1700, //玩家GPS信息

	CLIENT_STRSIP				 = 1701, //主动退出房间

	/* 2000 - 2100 是私有客户端请求协议 */

	//解散房间
	CLIENT_DISBAND_ROOM			 = 10000, //玩家请求解散房间
	CLIENT_DISBAND_CHOOSE		 = 10001, //玩家是否解散房间的选择
};

enum server_command
{
	SERVER_LOGIN_SUCC_UC       		 = 4000,
	SERVER_LOGIN_SUCC_BC       		 = 4001,
	SERVER_LOGIN_ERR_UC         	 = 4002,
	SERVER_REBIND_UC				 = 4003,
	SERVER_LOGOUT_SUCC_BC			 = 4004,
	SERVER_LOGOUT_ERR_UC			 = 4005,
	SERVER_TABLE_INFO_UC			 = 4006,
	SERVER_READY_SUCC_BC			 = 4007,
	SERVER_READY_ERR_UC				 = 4008,
	SERVER_GAME_START_BC			 = 4009,
	SERVER_NEXT_BET_BC				 = 4010, //下一个执行动作的玩家
	SERVER_BET_SUCC_BC				 = 4011, //玩家操作结果的广播
	SERVER_BET_SUCC_UC				 = 4012,
	SERVER_BET_ERR_UC                = 4013,
	SERVER_GAME_END_BC				 = 4014, //游戏结束
	SERVER_GAME_PREREADY_BC          = 4015,
	SERVER_CHAT_BC					 = 4016,
	SERVER_FACE_BC					 = 4017,
	SERVER_ROBOT_SER_UC              = 4018,
	SERVER_ROBOT_NEED_UC             = 4019,
	SERVER_UPDATE_INFO_BC			 = 4020,
	SERVER_EMOTION_BC				 = 4021,
	SERVER_PROP_SUCC_UC              = 4022,
	SERVER_PROP_SUCC_BC              = 4023,
	SERVER_PROP_ERR_UC               = 4024,
	SERVER_BOX_SUCC_BC				 = 4025,
	SERVER_ACTIVE_SUCC_UC            = 4026,
	SERVER_BETTING_SUCC_UC			 = 4027,//押注成功
	SERVER_ACTIVE_UPDATE_BC			 = 4028,//旁观押注更新
	SERVER_UPTABLE_INFO_UC			 = 4029,//上桌列表信息
	SERVER_APPLY_UPTABLE_UC			 = 4030,//申请上桌成功单播
	SERVER_UPTABLE_SUCC_BC			 = 4031,//有玩家上桌
	SERVER_AHEAD_DOWNTABLE_UC		 = 4032,//
	SERVER_DOWNTABLE_SUCC_BC		 = 4033,//有玩家下桌
	SERVER_START_BET_BC				 = 4034,//押注开始
	SERVER_STOP_BET_BC				 = 4035,//停止旁观押注
	SERVER_WIN_UC					 = 4036,//押注奖励返还
	SERVER_BETTING_ERR_UC			 = 4037,//押注失败
	SERVER_UPTABLE_ERR_UC			 = 4038,//上桌失败
	SERVER_RETURN_UC                 = 4039,
	SERVER_REWARD_SUCC_UC            = 4040,
	SERVER_DAEMONIZE_SUCC_UC		 = 4041,
	SERVER_BANKER_CANDIDATE_UC       = 4042,
	SERVER_COMPARE_INFO_BC           = 4043,
	SERVER_DOWN_BANKER_UC            = 4044,
	SERVER_GIVE_UP_BANKER_BC         = 4045,	
	SERVER_GET_BANKER_BC             = 4046,
	SERVER_SHORT_UPTABLE_INFO_BC     = 4049,
	SERVER_MULTI_BETS				 = 4060,//选择倍率
	SERVER_CHOSE_COMPLETE			 = 4065,//完成组牌
	SERVER_COMPARE_CARDS_BC			 = 4066,//结算
	SERVER_TIP						 = 4067,//没用的
	SERVER_SECOND_CARD				 = 4068,//第二轮明牌
	SERVER_THIRD_CARD				 = 4069,//第三轮明牌
	SERVER_GAME_DARK_CARD			 = 4075,//第一轮发的暗牌
	SERVER_START_COUNTDOWN			 = 4084,//普通场的
	SERVER_CHANGE_BANKER			 = 4085,//普通场的
	SERVER_BET_OVERSTEP_UC			 = 4086,
	SERVER_BETS_BC					 = 4087,
	SERVER_CONFIRM_RES         	 	 = 4088,//确认返回
	SERVER_TREND_UC                  = 4100,
	SERVER_ALL_PLAYER_INFO_UC        = 4110,
	SERVER_UP_GUEST_SEAT_ERROR_UC    = 4111,
	SERVER_UP_GUEST_SEAT_SUC_BC      = 4112,
	//SERVER_DOWN_GUEST_SEAT_ERR_UC  = 4113,
	SERVER_DOWN_GUEST_SEAT_BC        = 4114,
	SERVER_POOL_INFO_UC              = 4115,
	SERVER_POOL_ALTER_BC             = 4116,
	SERVER_ROBOT_SER                 = 4117,
	SERVER_COMMUNITY_BC              = 4118,
	SERVER_TWO_COMMUNITY_UC          = 4119,
	SERVER_EMOTION_NOSEAT_BC         = 4120,
	SERVER_OTHER_SHOW_BC             = 4121,
	SERVER_BANNE_UC 			     = 4122,
	SERVER_GAME_RESULT_BC			 = 4123,
	SERVER_UPBANKER_ERR_UC			 = 4124,
	SERVER_DETAL_INFO_UC             = 4140,
	SERVER_DETAL_INFO_ERROR_UC       = 4141,
	SERVER_RANK_UC                   = 4160,
	SERVER_LOTTERY_SHOW_CARDS        = 4170,
	SERVER_HISTORY_UC                = 4171,
	SERVER_MULTI_OF_LOSE_BC          = 4210,
	SERVER_COPARE_RESULT_FORROBOT    = 4212,

	/* 对战 */
	SERVER_DZ_LOGIN_SUCC_RES     = 4400, // 对战模式登录成功返回 
	SERVER_DZ_LOGIN_ERR_RES      = 4401, /* 对战模式登录失败返回 */    

	//麻将游戏协议码
	SERVER_USER_READY			 = 4998, //广播准备状态
	SERVER_GAME_SCENE			 = 4999,//场景协议
	SERVER_GAME_START 			 = 5000,//游戏开始
	SERVER_FIRST_HANDCARD		 = 5001,//第一手牌
	SERVER_CHANGE_CARD			 = 5009,//广播换牌
	SERVER_USER_LACK_TYPE 		 = 5002,//广播玩家的定缺色
	SERVER_USER_GETCARD			 = 5003,//摸牌
	SERVER_USER_OUTCARD			 = 5004,//出牌
	SERVER_OPERATE_NOTIFY    	 = 5005,//出牌后引起操作提示
	SERVER_OPERATE_RESULT        = 5006,//广播操作结果
	SERVER_LISTEN_NOTIFY		 = 5010,//碰后有听牌提示
	SERVER_USER_HU				 = 5007,//用户胡牌
	SERVER_GAME_END				 = 5008,//游戏结束
	SERVER_TRUSTEE				 = 5011,//托管
	SERVER_UPDATE_USERINFO	 	 = 5012,//更新玩家信息
	SERVER_UPDATE_TIMER	 		 = 5013,//玩家超时，广播更新时钟
	SERVER_HEART_BEAT_RESP		 = 5014,//心跳包返回
    SERVER_BUY_HORSE             = 5015,//开局买马信息
	SERVER_VIDEO_FIRST_HANDCARD  = 5016,//录像发牌协议
	SERVER_BEGIN_QUAN_FENG       = 5017,//开始圈风
	SERVER_TURN_SEATID           = 5018,  //开始转换桌位（下发桌子信息AckTableInfo）
	SERVER_ENSURE_JOKER          = 5019,  //开始翻癞子
	SERVER_USER_OFFLINE_BC		 = 5020,//广播玩家掉线
	SERVER_OVERTIME_USER		 = 5021,//广播超时没出牌的玩家
	SERVER_BROAD_PIAO			 = 5022,//广播玩家选择的飘
	SERVER_BROAD_LISTENTYPE		 = 5023,//广播玩家选择的听牌类型（报听、躺牌、首张报听）
	SERVER_START_PIAO			 = 5024,//开始选择飘	
	SERVER_CP_BANKER             = 5025,  //抄庄达成
	SERVER_FLOWER_HAND_CARD      = 5026,  //发第一手牌后补花
	SERVER_GET_HUA_CARD          = 5027,  //游戏过程中摸到花牌
	SERVER_SEND_MESSAGE			 = 5028,//发送提示消息
	SERVER_HUBEI_LAIZIINFO		 = 5029,//发送湖北麻将癞子信息	
	SERVER_GAME_END_JIANGMA		 = 5030,//游戏结束发送 奖码（抓鸡，抓鸟）信息
	SERVER_OPERATE_WAITE         = 5031, //等待操作消息
	SERVER_BAOPAI_REACH          = 5032, //三四道牌达成
	SERVER_VIDEO_CHANGE_FLOWER   = 5033, //发牌后换花  
	SERVER_ZIMOHU_HUANGHUANG	 = 5034, //湖北晃晃麻将，自摸胡
	SERVER_LAIZIGANG_HUANGHUANG	 = 5035, //湖北晃晃麻将，下发癞子杠倍数
	SERVER_FOURCARD_HUANGHUANG   = 5036, //湖北晃晃麻将，下发最后四张牌	
	SERVER_START_BUYHORSE		 = 5037, //游戏开始下发买马信息
	SERVER_END_BUYHORSE			 = 5038, //游戏结算，下发马牌等信息
	SERVER_OPERATE_RESULT2		 = 5039,//广播操作结果
	SERVER_GET_JI_PAI			 = 5040, //翻鸡牌		
	SERVER_BAO_TING_SUCC		 = 5041, //报听成功
	SERVER_DA_SHAIZI			 = 5042,//打筛子
	SERVER_QISHOUHU				 = 5043,//起手胡，板牌
	SERVER_BANPAI_END			 = 5044,//起手胡，板牌结束，收起手牌
	SERVER_USER_OUTCARD_ERR		 = 5045,//出牌失败
	SERVER_GANG_MO				 = 5055,//杠牌后摸牌
	SERVER_GANG_CHU				 = 5056,//杠牌后打牌
	SERVER_TIAN_TING			 = 5057,//贵州天听
	SERVER_JI_PAI_OP			 = 5058,//广播鸡牌操作
	SERVER_LOCK_CARD			 = 5059,//锁定手牌
	SERVER_FZ_CHANGE_HUA_CARD    = 5060,//发牌之后换花牌
	SERVER_FZ_GET_LAI_HUA_CARD   = 5061,//翻得癞子是花牌	
	SERVER_FZ_GET_HUA_CARD		 = 5062,//摸牌之后是花牌
	SERVER_USER_CHOOSE_LACK		 = 5063,//广播用户选择定缺
	SERVER_SANBI_FENG_HU		 = 5064,//南宁三笔封胡
	SERVER_LAIBIN_SANBI			 = 5065,//来宾麻将三笔
	SERVER_LAIBIN_FENG_HU		 = 5066,//来宾麻将封胡
	SERVER_NANPING_SENDZHONG	 = 5067,		//摸到花牌，发送“中”数量
	SERVER_LONGYAN_YOUJIN		 = 5068,		//龙岩麻将 游金锁牌
	SERVER_ZUNYI_YUANQUE		 = 5069,//遵义原缺
	SERVER_XY_SHOW_RESP			 = 5070,//襄阳亮牌返回
	SERVER_BROAD_BEISHU			 = 5071, //荆门倍数广播, 应城卡五星广播亮牌结果，宜昌麻将开始换张
	SERVER_START_JIA			 = 5072, //武穴开始选夹
	SERVER_BROAD_JIA			 = 5073, //武穴广播夹
	SERVER_START_CHANGECARDS 	 = 5074,	//开始换牌
	SERVER_CHANGECARDS_OVER 	 = 5075, //换牌结束
	SERVER_START_PUCARD 		 = 5076,	//开始扑牌
	SERVER_PUCARD 				 = 5077,	//扑牌结果
	SERVER_LAIZIPAI_INFO		 = 5078,	//发送癞子信息
	SERVER_BUHUA 				 = 5080,	//补花
	SERVER_JUSHUQUANSHU 		 = 5083, //通知局数圈数
	SERVER_PEIZI 				 = 5084, //配子信息
	SERVER_CHOOSE_PIAO_OVER		 = 5085,	//选飘结束
	SERVER_START_LACKTYPE		 = 5086,//开始选择缺门

	SERVER_TRUSTEE_NOTIFY        = 5120,//通知托管倒计时

	/* 5100 -  5200 为私有游戏服务器协议 */

	SERVER_ACK_ROUND_SCORE		 = 6101, 	//战绩
	
	SERVER_USER_ZUOMA_DONE 		 = 6102, //坐马结束
	SERVER_START_ZUOMA 			 = 6103, //开始坐马
	SERVER_ZUOMA_RES 			 = 6104, //坐马返回
	SERVER_LIPU_FENGHU 			 = 6105, //荔浦风胡

	SERVER_REAL_TIME_SPEECH      = 6120, //实时语音

	SERVER_ACK_CHANGE_MODE		 = 6200, //修改模式返回
	SERVER_PLAYER_GPS 			 = 6500, //玩家GPS信息

	SERVER_VIDEO_DATA_SAVE       = 10002, //保存录像数据
	SERVER_VIDEO_DATA_ERR_RESP   = 10003, //录像数据保存错误返回
	SERVER_ROOM_CONFIG_REQ       = 10004, //请求定制信息
	SERVER_ROOM_CONFIG_RES       = 10005, //定制信息返回
	SERVER_ACK_PUTCARD_INFO		 = 10006, //广播摆牌
	SERVER_OPERATE_RESULT_ANGANG = 10006, //咸宁广播操作结果

	//解散房间
	SERVER_APPLY_DISBAND_FAILD   = 15000,//玩家申请解散房间失败
	SERVER_APPLY_DISBAND_SUCC	 = 15001,//申请解散成功后，广播该解散房间申请
	SERVER_DISBAND_CHOOSE		 = 15002,//玩家是否解散房间的选择
	SERVER_DISBAND_RESULT		 = 15003,//解散房间的结果
	//请求统计信息
    SERVER_GAME_RECORD           = 16000,//游戏结束统计信息 

    SERVER_ROOM_MAJINAG_LAI_ZI 	 = 44002,//咸宁鬼牌
	SERVER_ROOM_MAJINAG_QINGYISE_WANING = 44003,//咸宁清一色告警
};

#endif
