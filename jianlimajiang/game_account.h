#ifndef GAME_ACCOUNT_H
#define GAME_ACCOUNT_H

#include "libnormalmahjong/common.h"
#include <iostream>
#include <string>
#include <json/json.h>


// redis �洢 ��Ϸ������Ϣ
class CGameAccout
{
public:
	CGameAccout();
	~CGameAccout();

	//���õڼ���
	void SetRound(int round);
	int GetRound() { return m_round; }
	
	//���÷���id
	void SetRoomid(int id);
	int GetRoomid() { return m_roomid; }

	//�������ÿ���˵�id
	void SetPlayerUid(int charid, int uid);


	//���õ�ǰ�÷�
	void SetScore(int play_id, int score);

	//�����ܼƵ÷�
	void SetTotalScore(int play_id, int total);

	//��ȡuid��ҵ� �ܷ�
	bool GetPlayerScore(int uid, int &score);

	//��ȡ��ǰ����
	void UpdateInfoFromJson(Json::Value root);



	//�������
	void Reset();

	//��Ч����
	bool IsValid(); 

	void GetJsonData(Json::Value &data);

	std::string toString(int);

private:

	int m_round;					//�ڼ���
	int m_roomid;					//����id
	int m_paly_id[GAME_PLAYER];		//�����Ϸid
	int m_score[GAME_PLAYER];		//ÿ�˵��ֵ÷�
	int m_total_score[GAME_PLAYER];	//ÿ�˵�ǰ�ܷ�

};

#endif

