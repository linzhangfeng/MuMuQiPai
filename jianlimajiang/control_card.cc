#include <sys/time.h>
#include "control_card.h"
#include "common/log.h"
extern Log log;

ControlCard::ControlCard(/* args */)
{
    m_b_senior_control      = false;    //本局是否控制
    m_n_rand_persent        = 0;        //控制比例 80
    m_n_ctrl_turn_distance  = 3;        //间隔局数
    m_n_check_win_turn      = 3;        //连胜检测局数
    m_n_check_score         = 50;       //检测相差分数
    m_str_tid.clear();                  //房间号
    
    m_map_uid_total_money.clear();      //累计积分
    m_vct_uid_alter_money.clear();      //单局积分
}

ControlCard::~ControlCard() 
{
}

void ControlCard::init(const std::string& tid, int n_rand_persent, int n_distance, int n_check_win, int n_check_score)
{   
    m_str_tid = tid;
	log.debug("fun[%s] pro[%d] dis[%d] win[%d] score[%d] \n", __FUNCTION__ , n_rand_persent,n_distance, n_check_win, n_check_score) ; 

    //设置房间控制概率
    if (n_rand_persent >= 0 && n_rand_persent <= 100) {
        m_n_rand_persent = n_rand_persent;
    }
    //是否进行控制
    m_b_senior_control = rand() % 100 < m_n_rand_persent;
    if (!m_b_senior_control) {
		log.debug("no control %d \n", m_b_senior_control );
		return ; 
	}
    //间隔局数
    m_n_ctrl_turn_distance = 0 ; // n_distance >= 0 ? n_distance : rand() % 3 + 1;
    m_n_check_win_turn = n_check_win > 0 ? n_check_win : 3;
    //检测分数
    m_n_check_score = n_check_score > 0 ? n_check_score : 50;

    log.debug("ctrl set room[%s] dis[%d] win[%d] score[%d].\n", 
        m_str_tid.c_str(), m_n_ctrl_turn_distance, m_n_check_win_turn, m_n_check_score);
}

void ControlCard::record_alter_money(const std::map<int, int>& uid_alter_money) 
{
    //历史单局
    m_vct_uid_alter_money.push_back(uid_alter_money);
    //累计战绩
    for (std::map<int, int>::const_iterator it=uid_alter_money.begin(); it!=uid_alter_money.end(); ++it) {
        int uid         = it->first;
        int alter_money = it->second;
        std::map<int, int>::iterator it_j = m_map_uid_total_money.find(uid);
        if (it_j == m_map_uid_total_money.end()) {
            m_map_uid_total_money[uid] = alter_money;
        }
        else {
            it_j->second += alter_money;
        }
    }
}

bool ControlCard::is_control(int cur_turn) {
    //非控制房间不需要控制
    if (!m_b_senior_control) return false;
    //第一局房间不需要控制
    if (cur_turn <= 1)  return false;
    //非检测局数不需要设置
    // if (0 != m_n_ctrl_turn_distance && 0 != (cur_turn - 1) % m_n_ctrl_turn_distance) return false;

    //初始化
    //good_uid = 0;
    //bad_uid = 0;
    int n_code = 0;

    //连赢检测
    std::set<int>  set_win_uids;
    if ((int)m_vct_uid_alter_money.size() >= m_n_check_win_turn) {
        int check_last_count = m_n_check_win_turn;
        //推入每局赢的玩家
        for (
            int i=(int)m_vct_uid_alter_money.size()-1; 
            i>=0 && check_last_count > 0; 
            --i, --check_last_count) {
            //每局检测
            const std::map<int, int>& uid_money = m_vct_uid_alter_money[i];
            for (std::map<int, int>::const_iterator it=uid_money.begin(); it!=uid_money.end(); ++it) {
                int uid     = it->first;
                int score   = it->second;
                if (score > 0) {
                    set_win_uids.insert(uid);
                }
            }
        }
        //一旦玩家输了 剔除
        check_last_count = m_n_check_win_turn;
        for (
            int i=(int)m_vct_uid_alter_money.size()-1; 
            i>=0 && check_last_count > 0; 
            --i, --check_last_count) {
            //每局检测
            const std::map<int, int>& uid_money = m_vct_uid_alter_money[i];
            for (std::map<int, int>::const_iterator it=uid_money.begin(); it!=uid_money.end(); ++it) {
                int uid     = it->first;
                int score   = it->second;
                if (score < 0) {
                    std::set<int>::iterator it_find = set_win_uids.find(uid);
                    if (it_find != set_win_uids.end()) {
                        set_win_uids.erase(it_find);
                    }
                }
            }
        }
    }
    
    //找到输的最多的人
    int min_score = 0;
    int min_uid = 0;
    for (std::map<int, int>::const_iterator it=m_map_uid_total_money.begin();
        it!=m_map_uid_total_money.end(); ++it) {
        if (0 == min_uid) {
            min_uid     = it->first;
            min_score   = it->second;
        }
        else if (it->second < min_score) {
            min_score   = it->second;
            min_uid     = it->first;
        }
    }

    //找到赢的最多的人
    int max_score = 0;
    int max_uid = 0;
    //有连赢
    std::map<int, int>::const_iterator it_find;
    for (std::set<int>::const_iterator it=set_win_uids.begin(); it!=set_win_uids.end(); ++it) {
        it_find = m_map_uid_total_money.find(*it);
        if (it_find != m_map_uid_total_money.end()) {
            if (0 == max_uid) {
                max_uid     = it_find->first;
                max_score   = it_find->second;
                n_code |= 0x1;
            }
            else if (it_find->second > max_score) {
                max_uid     = it_find->first;
                max_score   = it_find->second;
            }
        }
    }
    //没有连赢
    if (0 == max_uid) {
        max_score = 0;
        for (std::map<int, int>::const_iterator it=m_map_uid_total_money.begin();
            it!=m_map_uid_total_money.end(); ++it) {
            if (0 == max_uid) {
                max_uid     = it->first;
                max_score   = it->second;
            }
            else if (it->second > max_score) {
                max_score   = it->second;
                max_uid     = it->first;
            }
        }
    }

    if ((max_score - min_score) >= m_n_check_score) {
        n_code |= 0x10;
    }
    
    if (n_code > 0 && max_uid != 0 && min_uid != 0 && max_uid != min_uid) {
        //good_uid = min_uid;
        //bad_uid = max_uid; 
        log.debug("control_card[%s] good uid[%d] bad_uid[%d] code[%d].\n", m_str_tid.c_str(), min_uid, max_uid , n_code);
        return true;
    }
    return false;
}


int ControlCard::get_quan_uid(const std::map<int, float>& uid_quan, GET_QUAN_TYPE get_type)
{
    float quan  = 0.0;
    int   uid   = 0;
    for (std::map<int, float>::const_iterator it=uid_quan.begin(); it!=uid_quan.end(); ++it) {
        if (0 == uid) {
            uid    = it->first;
            quan   = it->second;
        }
        else {
            if (get_type == GET_QUAN_TYPE_GOOD) {
                if (it->second > quan) {
                    uid    = it->first;
                    quan   = it->second;
                }
            }
            else if (get_type == GET_QUAN_TYPE_BAD) {
                if (it->second < quan) {
                    uid    = it->first;
                    quan   = it->second;
                }
            }
        }
    }
    return uid;
}


void ControlCard::swap_quan(std::map<int, float>& uid_quan, int lhs_uid, int rhs_uid) {
    if (lhs_uid == rhs_uid) return;
    std::map<int, float>::iterator it_l = uid_quan.find(lhs_uid);
    std::map<int, float>::iterator it_r = uid_quan.find(rhs_uid);
    if (it_l != uid_quan.end() && it_r != uid_quan.end()) {
        float tmp = it_l->second;
        it_l->second = it_r->second;
        it_r->second = tmp;
    }
}

//DataStatic* DataStatic::m_p_instance = NULL;

DataStatic::DataStatic()
{
	//ev_day_tick.data = this;
	//ev_periodic_init(&ev_day_tick, day_cb, 3600 * (24-8), 3600 * 24, NULL);
	//ev_periodic_start(pdk.loop, &ev_day_tick);
	m_vec_map_uid_total_moeny_history.clear();
	m_curday = 0 ;

}
DataStatic::~DataStatic()
{	
}
void DataStatic::add_score(int uid, int score , int is_last )
{
	if (m_map_uid_total_money.find(uid) != m_map_uid_total_money.end()) {
		m_map_uid_total_money[uid] += score;
	}
	else {
		m_map_uid_total_money[uid] = score;
	}
	 
	if(is_last) { 		
		struct timeval _tv; 
		gettimeofday(&_tv, NULL); 
		struct tm _tm; 
		localtime_r(&_tv.tv_sec, &_tm);  
		log.debug("fun[%s] uid[%d]  score[%d]  pre-day[%d] cur-day[%d] \n" , __FUNCTION__ , uid , score , m_curday , _tm.tm_mday) ;
		if(_tm.tm_mday  != m_curday)
		{ 
			m_curday= _tm.tm_mday   ;   
			log.debug("fun[%s] day[%d] \n" , __FUNCTION__ ,  m_curday) ;
			m_vec_map_uid_total_moeny_history.insert(m_vec_map_uid_total_moeny_history.begin(), m_map_uid_total_money);
			m_map_uid_total_money.clear(); 
			if(m_vec_map_uid_total_moeny_history.size()> 3){
				m_vec_map_uid_total_moeny_history.pop_back() ; 
			}
		}
	}
}

int DataStatic::get_total_moeny(int uid, size_t days /*= 1*/)
{
	int ret_moeny = 0;
	for( size_t i=0; i<days; i++)
	{
		if (i == 0)	{
			if (m_map_uid_total_money.find(uid) != m_map_uid_total_money.end()) {
				ret_moeny = m_map_uid_total_money[uid];
			}
		}
		else {
			if( m_vec_map_uid_total_moeny_history.size() >= i) {
				std::map<int, int>::iterator it = m_vec_map_uid_total_moeny_history[i - 1].find(uid);
				if (it != m_vec_map_uid_total_moeny_history[i - 1].end())
					ret_moeny += it->second;
			}
		}
	}
	return ret_moeny;
}

//void DataStatic::day_cb(struct ev_loop *loop, ev_periodic *w, int revents)
//{
//	DataStatic *pDataStatic = (DataStatic *)w->data;
//	int record_count = pDataStatic->m_vec_map_uid_total_moeny_history.size();
//	log_info("history days[%d]", record_count);
//
//	if (record_count > 0) {
//		pDataStatic->m_vec_map_uid_total_moeny_history.insert(pDataStatic->m_vec_map_uid_total_moeny_history.begin(), pDataStatic->m_map_uid_total_money);
//	}
//	else {
//		pDataStatic->m_vec_map_uid_total_moeny_history.push_back(pDataStatic->m_map_uid_total_money);
//	}
//	record_count++;
//	//历史记录最多5天
//	if (record_count > 3 )
//		pDataStatic->m_vec_map_uid_total_moeny_history.pop_back();
//
//	pDataStatic->m_map_uid_total_money.clear();
//}
