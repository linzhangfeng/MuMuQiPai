#ifndef __CONTROL_CARD_H__
#define __CONTROL_CARD_H__

#include <vector>
#include <map>
#include <string>
#include <set>

class ControlCard
{
public:
    ControlCard(/* args */);
    ~ControlCard();
    enum GET_QUAN_TYPE {
        GET_QUAN_TYPE_GOOD   = 1,
        GET_QUAN_TYPE_BAD    = 2,
    };
    /*
    *   功能：初始化房间控制
    *   返回：无
    *   参数：tid                房间号
    *   参数：n_rand_persent     房间控制概率 取值范围[0-100]
    *   参数：n_distance         控制间隔局数 -1--随机（1~3局）
    *   参数: n_check_win        连赢局数     3--连赢3局需要控制
    *   参数：n_check_score      检测相差分数  产品决定
    *   
    *   举例：m_control.init(tid, 80, -1, 3, 50);
    *        控制房间号tid的房间，有50%的概率对此房间进行控制，间隔设定-1(随机1-3)局进行控制，
    *        并且控制条件为连赢3局的玩家或者最大分数相差为 50分的玩家。
    */
    void    init(const std::string& tid, int n_rand_persent, int n_distance, int n_check_win, int n_check_score);
    
    /*
    *   功能：战绩记录
    *   返回：无
    *   参数：uid_alter_money    玩家uid-小局分数
    */
    void    record_alter_money(const std::map<int, int>& uid_alter_money);

    /*
    *   功能：是否干预本房间
    *   返回：true--控制 false--不控制
    *   参数：cur_turn           当前局数
    *   参数：good_uid           好牌玩家
    *   参数: bad_uid            差牌玩家
    */
    //bool    is_control(int cur_turn, int& good_uid, int& bad_uid);
	bool    is_control(int cur_turn);
    
    /*
    *   功能：获取权职对应的uid
    *   返回：0--无效 其他uid
    *   参数：uid_quan           uid权值
    *   参数：get_type           GET_QUAN_TYPE_GOOD/GET_QUAN_TYPE_BAD
    */
    int     get_quan_uid(const std::map<int, float>& uid_quan, GET_QUAN_TYPE get_type);

    /*
    *   功能：换权值，换了手牌后权值会随着uid有变化
    *   返回：无
    *   参数：uid_quan           uid权值
    *   参数：lhs_uid            换的uid1
    *   参数：rhs_uid            换的uid2
    */
    void    swap_quan(std::map<int, float>& uid_quan, int lhs_uid, int rhs_uid);
public:
    
    bool                                   m_b_senior_control;      //本局是否控制
    int                                    m_n_rand_persent;        //控制比例 80
    int                                    m_n_ctrl_turn_distance;  //间隔局数
    int                                    m_n_check_win_turn;      //连胜检测局数
    int                                    m_n_check_score;         //检测分数
    std::string                            m_str_tid;               //房间号
    //历史信息
    std::map<int, int>                     m_map_uid_total_money;   //累计积分
    std::vector<std::map<int, int> >       m_vct_uid_alter_money;   //单局积分
};


class DataStatic{
public:
	static DataStatic& instance(){
		static  DataStatic instance_ ; 
		return  instance_ ; 
	}
	
	DataStatic() ;
	~DataStatic() ; 
	void	add_score(int uid, int score ,int is_last =0); 
	int		get_total_moeny(int uid, size_t days = 1);
private:
	//static void day_cb(struct ev_loop *loop, ev_periodic *w, int revents);
public:
	std::map<int , int >				    m_map_uid_total_money; 
	typedef  std::map<int,int>				day_total_info ; 
	std::vector<day_total_info>             m_vec_map_uid_total_moeny_history ; 

	int    m_curday ;
	

};

#endif