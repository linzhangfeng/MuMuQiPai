#ifndef __TABLE_DEF_H__
#define __TABLE_DEF_H__
#include "libnormalmahjong/mj_common2.h"

struct RoundScore
{
	int score[GAME_PLAYER];	
	RoundScore(){}
	RoundScore(int a, int b, int c, int d)
	{
		score[0] = a;		score[1] = b;		score[2] = c;		score[3] = d;
	}
};
struct Table_TotalRecordInfo
{
	int banker[GAME_PLAYER];
	int hu[GAME_PLAYER];
	int piao[GAME_PLAYER];
	int timerout_op[GAME_PLAYER];
	int total_score[GAME_PLAYER];
	int paohu[GAME_PLAYER];
	int zimo[GAME_PLAYER];
	int dianpao[GAME_PLAYER];
	int win_counts[GAME_PLAYER];
	int gang_times[GAME_PLAYER];
	int dian_gang_times[GAME_PLAYER];
	int piao_count[GAME_PLAYER];
	int hongzhonggang_count[GAME_PLAYER];
	int dahu_count[GAME_PLAYER];
	int laizipigang_count[GAME_PLAYER];
	int mobao_count[GAME_PLAYER];

	std::vector<RoundScore> round_score;
	Table_TotalRecordInfo()
	{
		memset(banker, 0, sizeof(banker));
		memset(hu, 0, sizeof(hu));
		memset(piao, 0, sizeof(piao));
		memset(timerout_op, 0, sizeof(timerout_op));
		memset(total_score, 0, sizeof(total_score));
		memset(paohu, 0, sizeof(paohu));
		memset(zimo, 0, sizeof(zimo));
		memset(dianpao, 0, sizeof(dianpao));
		memset(win_counts, 0, sizeof(win_counts));
		memset(gang_times, 0, sizeof(gang_times));
		memset(dian_gang_times, 0, sizeof(dian_gang_times));
		memset(piao_count, 0, sizeof(piao_count));
		memset(hongzhonggang_count, 0, sizeof(hongzhonggang_count));
		memset(dahu_count, 0, sizeof(dahu_count));
		memset(laizipigang_count, 0, sizeof(laizipigang_count));
		memset(mobao_count, 0, sizeof(mobao_count));

		round_score.clear();
	}
};
struct SGSI_SortNode
{
	_uint8 ChairID;
	int iTotalScore;
};


#endif