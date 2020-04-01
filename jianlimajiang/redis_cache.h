#ifndef CREDIS_CACHE_H
#define CREDIS_CACHE_H

#include "libnormalmahjong/common.h"
#include "redis_client.h"
#include "game_account.h"
#include <iostream>
#include <string>
#include <ev++.h>
#include <ev.h>
#include <stdio.h>

// 1.�洢��ʽ
// Hash��洢
// Key��	majhong02 + ��Ϸid
// Field : ����id
// Value : json�洢����Ϸ����



//��Ϣ��7212	��Ϸ������ƽ̨���ص�ǰ�ڼ��ֵ���Ч��Ϣ
//��Ϣ��7211	��Ϸ��ʼ�������������ͬ��������Ϣ
// {
// "can_start" : 1,
// "cmd" : 7211,
// "code" : 200,
// "count" : 8,
// "currentCount" : 7,
// "datetime" : 1495087002,
// "roomid" : "161330",
// "status" : 200,
// "vid" : 2225
// }



class CRedisCache
{
public:
	static CRedisCache& GetInstance();

	void StartClearTime(int ex_time);

	static void clear_redis_time_cb(struct ev_loop *loop, struct ev_timer *w, int revents);


	~CRedisCache();


	
	//������Ϸ������Ϣ
	bool UpdateGameEndScore(CGameAccout &game_account);

	//��ȡ ���һ�ֽ�����Ϣ
	bool GetGameScore(CGameAccout &game_account);

	




protected:

	//��ȡ������Ϣ
	bool GetRoomidInfo(int roomid);

	//���·�����Ϣ
	bool UpdateRoomidInfo(int roomid,const std::string &values);

	//��redis������Ϣ �� ת���� json::value��ʽ
	bool TranslateInfoToJson(Json::Value &root);

	//��ճ�ʱ
	void ClearRedisExTimeData();

	//һ�μ�� ���䷿������ �Ƿ�ʱ
	void CheckExTimeFromRoomid(std::vector<std::string> &vec_roomid);

	//ɾ��������Ϣ
	void DelRoomid(std::string roomid);


private:
	CRedisCache();



	ev_timer ev_time_clear_redis;

};

#endif

