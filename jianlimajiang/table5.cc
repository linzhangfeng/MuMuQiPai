#include "table.h"
#include "zjh.h"
#include "common/log.h"
#include "common/base64.h"

extern Log log;
extern ZJH zjh;

/*
 *恢复服数据恢复
 */
int Table::decompress_str(string &src, string &des)
{
	//base64编码
	static unsigned char ubuf[MaxVideoBufLen] ={0};
	int unoutlen = MaxVideoBufLen;
	int iRet = base64_decode((const unsigned char *)src.c_str(), src.size(), (unsigned char *)ubuf, &unoutlen);
	if (iRet != CRYPT_OK)
	{
		log.error("%s iRet:%d base64_decode error.\n", __FUNCTION__, iRet);
		return -1;
	}
	//zlib解压
	static unsigned char uzbuf[MaxVideoBufLen] ={0};
	uLong unzlen = MaxVideoBufLen;
	iRet = uncompress((Bytef*)uzbuf, (uLongf *)&unzlen, (const Bytef*)ubuf, (uLong)unoutlen);
	if (iRet != Z_OK)
	{
		log.error("%s iRet:%d uncompress error.\n", __FUNCTION__, iRet);
		return -1;
	}
	std::string packStr((char *)uzbuf,unzlen);
	des.clear();
	des = packStr;
	return 0;
}

int Table::compress_str(string &src, string &des)
{
	//zlib压缩
	static unsigned char zbuf[MaxVideoBufLen] ={0};
	uLong nzlen = MaxVideoBufLen;
	int iRet = compress((Bytef*)zbuf, (uLongf *)&nzlen, (const Bytef*)src.c_str(), (uLong)src.size());
	if (iRet != Z_OK)
	{
		log.error("%s iRet:%d compress error.\n", __FUNCTION__, iRet);
		return -1;
	}
	//base64编码
	static unsigned char buf[MaxVideoBufLen] ={0};
	int noutlen = MaxVideoBufLen;
	iRet = base64_encode((const unsigned char *)zbuf, nzlen, (unsigned char *)buf, &noutlen);
	if (iRet != CRYPT_OK)
	{
		log.error("%s iRet:%d base64_encode error.\n", __FUNCTION__, iRet);
		return -1;
	}
	std::string packStr((char *)buf,noutlen);
	des.clear();
	des = packStr;
	return 0;
}

void Table::SerializeMJCustomizedConf(proto::game::redis::MJCustomizedConf* pstconf)
{
	proto::game::redis::MJConfCommon* pstconfcommon = pstconf->mutable_mjconfcommon();
	pstconfcommon->set_m_bsupportchi(m_conf.m_bSupportChi);			
	pstconfcommon->set_m_bsupportwind(m_conf.m_bSupportWind);		
	pstconfcommon->set_m_bsupportting(m_conf.m_bSupportTing);		
	pstconfcommon->set_m_bzimohu(m_conf.m_bZimoHu);             
	pstconfcommon->set_m_byipaoduoxiang(m_conf.m_bYiPaoDuoXiang);     
	pstconfcommon->set_m_bqiangganghu(m_conf.m_bQiangGangHu);      
	pstconfcommon->set_m_bqiangzhigang(m_conf.m_bQiangZhiGang);
	pstconfcommon->set_m_bqiangangang(m_conf.m_bQiangAnGang);	
	pstconfcommon->set_m_bqangshangpao(m_conf.m_bGangShangPao);
	pstconfcommon->set_m_bhuanggang(m_conf.m_bHuangGang);		
	pstconfcommon->set_m_bzhuangxian(m_conf.m_bZhuangXian);	
	pstconfcommon->set_m_bforcehuqiangganghu(m_conf.m_bForceHuQiangGangHu);
	pstconfcommon->set_m_bfourcehumutilhu(m_conf.m_bFourceHuMutilHu);
	pstconfcommon->set_m_angangpoint(m_conf.m_AnGangPoint);		
	pstconfcommon->set_m_wangangpoint(m_conf.m_WanGangPoint);
	pstconfcommon->set_m_zhigangpoint(m_conf.m_ZhiGangPoint);	

	proto::game::redis::MJConfPiao* pstconfpiao = pstconf->mutable_mjconfpiao();
	pstconfpiao->set_m_bsupportpiao(m_conf.m_bSupportPiao);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		pstconfpiao->add_m_piao(m_conf.m_Piao[i]);		
	}

	proto::game::redis::MJConfBuyHorse* pstconfbuyhorse = pstconf->mutable_mjconfbuyhorse();
	pstconfbuyhorse->set_m_bsupportbuyhorse(m_conf.m_bSupportBuyHorse);
	pstconfbuyhorse->set_m_horsenum(m_conf.m_HorseNum);
	pstconfbuyhorse->set_m_horsemaxnum(m_conf.m_HorseMaxNum);
	pstconfbuyhorse->set_m_nbuyhorseincnum(m_conf.m_nBuyHorseIncNum);
	pstconfbuyhorse->set_m_buyhorsemojijiangji(m_conf.m_buyHorseMojiJiangji);

	proto::game::redis::MJConfZhuaniao* pstconfzhuaniao = pstconf->mutable_mjconfzhuaniao();
	pstconfzhuaniao->set_m_bsupportzhuaniao(m_conf.m_bSupportZhuaniao);
	pstconfzhuaniao->set_m_bcalcallniao(m_conf.m_bCalcAllNiao);
	pstconfzhuaniao->set_m_zhuaniaonum(m_conf.m_ZhuaniaoNum);
	pstconfzhuaniao->set_m_bjinniao(m_conf.m_bJinNiao);
	pstconfzhuaniao->set_m_bmojijiangji(m_conf.m_bMojiJiangji);

	proto::game::redis::MJHuType* pstconfhutype= pstconf->mutable_mjhutype();
	pstconfhutype->set_m_bhu7dui(m_conf.m_bHu7dui);
	pstconfhutype->set_m_bhu13yao(m_conf.m_bHu13yao);
	pstconfhutype->set_m_bnosupport_quanqiuren(m_conf.m_bNoSupport_Quanqiuren);

	proto::game::redis::MJConfOther* pstconfother = pstconf->mutable_mjconfother();
	pstconfother->set_m_bsupportchangemode(m_conf.m_bSupportChangeMode);
	pstconfother->set_m_bofflinedelayop(m_conf.m_bOfflineDelayOp);
	pstconfother->set_m_bmulitfirstopisover(m_conf.m_bMulitFirstOpIsOver);
	pstconfother->set_m_bfastmode(m_conf.m_bFastMode);
	pstconfother->set_m_bautodisband(m_conf.m_bAutoDisband);
	pstconfother->set_m_bbankerfirst14(m_conf.m_bBankerFirst14);
	pstconfother->set_m_bangangnoshow(m_conf.m_bAnGangNoShow);
	pstconfother->set_m_bshowgangscore(m_conf.m_bShowGangScore);
}

void Table::ParseMJCustomizedConf(const proto::game::redis::MJCustomizedConf& stconf)
{
	const proto::game::redis::MJConfCommon& stmjconfcommon = stconf.mjconfcommon();
	m_conf.m_bSupportChi = stmjconfcommon.m_bsupportchi();
	m_conf.m_bSupportWind = stmjconfcommon.m_bsupportwind();
	m_conf.m_bSupportTing = stmjconfcommon.m_bsupportting();
	m_conf.m_bZimoHu = stmjconfcommon.m_bzimohu();
	m_conf.m_bYiPaoDuoXiang = stmjconfcommon.m_byipaoduoxiang();
	m_conf.m_bQiangGangHu = stmjconfcommon.m_bqiangganghu();
	m_conf.m_bQiangZhiGang = stmjconfcommon.m_bqiangzhigang();
	m_conf.m_bQiangAnGang = stmjconfcommon.m_bqiangangang();
	m_conf.m_bGangShangPao = stmjconfcommon.m_bqangshangpao();
	m_conf.m_bHuangGang = stmjconfcommon.m_bhuanggang();
	m_conf.m_bZhuangXian = stmjconfcommon.m_bzhuangxian();
	m_conf.m_bForceHuQiangGangHu = stmjconfcommon.m_bforcehuqiangganghu();
	m_conf.m_bFourceHuMutilHu = stmjconfcommon.m_bfourcehumutilhu();
	m_conf.m_AnGangPoint = stmjconfcommon.m_angangpoint();
	m_conf.m_WanGangPoint = stmjconfcommon.m_wangangpoint();
	m_conf.m_ZhiGangPoint = stmjconfcommon.m_zhigangpoint();

	const proto::game::redis::MJConfPiao& stconfpiao = stconf.mjconfpiao();
	m_conf.m_bSupportPiao = stconfpiao.m_bsupportpiao();
	for (int i = 0; i < stconfpiao.m_piao_size(); ++i)
	{
		m_conf.m_Piao[i] = stconfpiao.m_piao(i);	
	}

	const proto::game::redis::MJConfBuyHorse& stconfbuyhorse = stconf.mjconfbuyhorse();
	m_conf.m_bSupportBuyHorse = stconfbuyhorse.m_bsupportbuyhorse();
	m_conf.m_HorseNum = stconfbuyhorse.m_horsenum();
	m_conf.m_HorseMaxNum = stconfbuyhorse.m_horsemaxnum();
	m_conf.m_nBuyHorseIncNum = stconfbuyhorse.m_nbuyhorseincnum();
	m_conf.m_buyHorseMojiJiangji = stconfbuyhorse.m_buyhorsemojijiangji();

	const proto::game::redis::MJConfZhuaniao& stconfzhuaniao = stconf.mjconfzhuaniao();
	m_conf.m_bSupportZhuaniao = stconfzhuaniao.m_bsupportzhuaniao();
	m_conf.m_bCalcAllNiao = stconfzhuaniao.m_bcalcallniao();
	m_conf.m_ZhuaniaoNum = stconfzhuaniao.m_zhuaniaonum();
	m_conf.m_bJinNiao = stconfzhuaniao.m_bjinniao();
	m_conf.m_bMojiJiangji = stconfzhuaniao.m_bmojijiangji();

	const proto::game::redis::MJHuType& stconfhutype = stconf.mjhutype();
	m_conf.m_bHu7dui = stconfhutype.m_bhu7dui();
	m_conf.m_bHu13yao = stconfhutype.m_bhu13yao();
	m_conf.m_bNoSupport_Quanqiuren = stconfhutype.m_bnosupport_quanqiuren();

	const proto::game::redis::MJConfOther stconfoter = stconf.mjconfother();
	m_conf.m_bSupportChangeMode = stconfoter.m_bsupportchangemode();
	m_conf.m_bOfflineDelayOp = stconfoter.m_bofflinedelayop();
	m_conf.m_bMulitFirstOpIsOver = stconfoter.m_bmulitfirstopisover();
	m_conf.m_bFastMode = stconfoter.m_bfastmode();
	m_conf.m_bAutoDisband = stconfoter.m_bautodisband();
	m_conf.m_bBankerFirst14 = stconfoter.m_bbankerfirst14();
	m_conf.m_bAnGangNoShow = stconfoter.m_bangangnoshow();
	m_conf.m_bShowGangScore = stconfoter.m_bshowgangscore();
}

void Table::SerializeTable_TotalRecordInfo(proto::game::redis::Table_TotalRecordInfo* pstRecordInfo)
{
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		pstRecordInfo->add_banker(m_totalrecordinfo.banker[i]);
		pstRecordInfo->add_hu(m_totalrecordinfo.hu[i]);
		pstRecordInfo->add_piao(m_totalrecordinfo.piao[i]);
		pstRecordInfo->add_timerout_op(m_totalrecordinfo.timerout_op[i]);
		pstRecordInfo->add_total_score(m_totalrecordinfo.total_score[i]);
		pstRecordInfo->add_paohu(m_totalrecordinfo.paohu[i]);
		pstRecordInfo->add_zimo(m_totalrecordinfo.zimo[i]);
		pstRecordInfo->add_dianpao(m_totalrecordinfo.dianpao[i]);
		pstRecordInfo->add_win_counts(m_totalrecordinfo.win_counts[i]);
		pstRecordInfo->add_gang_times(m_totalrecordinfo.gang_times[i]);
		pstRecordInfo->add_dian_gang_times(m_totalrecordinfo.dian_gang_times[i]);
		pstRecordInfo->add_piao_count(m_totalrecordinfo.piao_count[i]);
		pstRecordInfo->add_hongzhonggang_count(m_totalrecordinfo.hongzhonggang_count[i]);
		pstRecordInfo->add_dahu_count(m_totalrecordinfo.dahu_count[i]);
		pstRecordInfo->add_laizipigang_count(m_totalrecordinfo.laizipigang_count[i]);
		pstRecordInfo->add_mobao_count(m_totalrecordinfo.mobao_count[i]);
	}

	for (size_t j = 0; j < m_totalrecordinfo.round_score.size(); ++j)
	{
		proto::game::redis::RoundScore* pstRound = pstRecordInfo->add_round_score();
		for (int k = 0; k < GAME_PLAYER; ++k)
		{
			pstRound->add_score(m_totalrecordinfo.round_score[j].score[k]);
		}
	}
}

void Table::ParseTable_TotalRecordInfo(const proto::game::redis::Table_TotalRecordInfo& stRecordInfo)
{
	if (stRecordInfo.banker_size() != GAME_PLAYER) return;

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		m_totalrecordinfo.banker[i] = stRecordInfo.banker(i);
		m_totalrecordinfo.hu[i] = stRecordInfo.hu(i);
		m_totalrecordinfo.piao[i] = stRecordInfo.piao(i);
		m_totalrecordinfo.timerout_op[i] = stRecordInfo.timerout_op(i);
		m_totalrecordinfo.total_score[i] = stRecordInfo.total_score(i);
		m_totalrecordinfo.paohu[i] = stRecordInfo.paohu(i);
		m_totalrecordinfo.zimo[i] = stRecordInfo.zimo(i);
		m_totalrecordinfo.dianpao[i] = stRecordInfo.dianpao(i);
		m_totalrecordinfo.win_counts[i] = stRecordInfo.win_counts(i);
		m_totalrecordinfo.gang_times[i] = stRecordInfo.gang_times(i);
		m_totalrecordinfo.dian_gang_times[i] = stRecordInfo.dian_gang_times(i);
		m_totalrecordinfo.piao_count[i] = stRecordInfo.piao_count(i);
		m_totalrecordinfo.hongzhonggang_count[i] = stRecordInfo.hongzhonggang_count(i);
		m_totalrecordinfo.dahu_count[i] = stRecordInfo.dahu_count(i);
		m_totalrecordinfo.laizipigang_count[i] = stRecordInfo.laizipigang_count(i);
	}

	for (int i = 0; i < stRecordInfo.mobao_count_size(); i++)
	{
		if (i < GAME_PLAYER)
		{
			m_totalrecordinfo.mobao_count[i] = stRecordInfo.mobao_count(i);
		}
	}

	for (int i = 0; i < stRecordInfo.round_score_size(); ++i)
	{
		const proto::game::redis::RoundScore& stRound = stRecordInfo.round_score(i);
		RoundScore roundscore;
		for (int j = 0; j < stRound.score_size(); ++j)
		{
			roundscore.score[j] = stRound.score(j);
		}
		m_totalrecordinfo.round_score.push_back(roundscore);
	}
}

void Table::SerializeHandCards(proto::game::redis::tagRedisInfo &stTableInfo)
{
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		proto::game::redis::HandCards* psthandcards = stTableInfo.add_m_handcard();

		psthandcards->set_changeablecardslen(m_HandCard[i].ChangeableCardsLen);
		for (int j = 0; j < m_HandCard[i].ChangeableCardsLen; j++)
		{
			psthandcards->add_changeablecards(m_HandCard[i].ChangeableCards[j]);		
		}

		psthandcards->set_fixedcardslen(m_HandCard[i].FixedCardsLen);
		for (int j = 0; j < m_HandCard[i].FixedCardsLen; j++)
		{
			proto::game::redis::FixedCard *pFixedcard = psthandcards->add_stfixedcards();
			pFixedcard->set_carddata(m_HandCard[i].FixedCards[j].CardData);
			pFixedcard->set_state(m_HandCard[i].FixedCards[j].state);
			pFixedcard->set_chairid(m_HandCard[i].FixedCards[j].chairID);
			for (int k = 0; k < 3; ++k)
			{
				pFixedcard->add_opcards(m_HandCard[i].FixedCards[j].c[k]);		
			}
		}
	}
}

void Table::ParseHandCards(const proto::game::redis::tagRedisInfo &stTableInfo)
{
	for (int i = 0; i < stTableInfo.m_handcard_size(); ++i)
	{
		const proto::game::redis::HandCards sthandcards = stTableInfo.m_handcard(i);
		m_HandCard[i].ChangeableCardsLen = sthandcards.changeablecardslen();		
		for (int j = 0; j < sthandcards.changeablecards_size(); ++j)
		{
			m_HandCard[i].ChangeableCards[j] = sthandcards.changeablecards(j);
		}
		m_HandCard[i].FixedCardsLen = sthandcards.fixedcardslen();
		for (int j = 0; j < sthandcards.stfixedcards_size(); ++j)
		{
			const proto::game::redis::FixedCard stfixcards = sthandcards.stfixedcards(j);	
			m_HandCard[i].FixedCards[j].CardData = stfixcards.carddata();
			m_HandCard[i].FixedCards[j].state = stfixcards.state();
			m_HandCard[i].FixedCards[j].chairID = stfixcards.chairid();
			for (int k = 0; k < stfixcards.opcards_size(); ++k)
			{
				m_HandCard[i].FixedCards[j].c[k] = stfixcards.opcards(k);
			}
		}
	}	
}

void Table::SerializeCardPool(proto::game::redis::CCardPool* pstpool)
{
	int SeedID;
	E_POOL_MODE Mode;
	_uint8 Pool[POOL_CARD_MAX];		
	_uint8 MaxCount, Count, Current, Last, JokerCard, JokerCount;
	m_CardPool.GetInfo(SeedID, Mode, Pool, MaxCount, Count, Current, Last, JokerCard, JokerCount);

	// proto::game::redis::CCardTypePool* psttypepool = pstpool->mutable_m_cardtypepool();
	// psttypepool->set_m_jokercard(JokerCard);
	// for (int i = 0; i < POOL_CARD_TYPE_MAX; ++i)
	// {
	// 	psttypepool->add_m_pool(Pool[i]);	
	// }
	// psttypepool->set_m_maxcount(MaxCount);
	// psttypepool->set_m_mode(Mode);

	pstpool->set_m_seedid(SeedID);
	pstpool->set_m_mode(Mode);
	for (int i = 0; i < POOL_CARD_MAX; ++i)
	{
		pstpool->add_m_pool(Pool[i]);
	}
	pstpool->set_m_maxcount(MaxCount);
	pstpool->set_m_count(Count);
	pstpool->set_m_current(Current);
	pstpool->set_m_last(Last);
	pstpool->set_m_jokercard(JokerCard);
	pstpool->set_m_jokercount(JokerCount);
}

void Table::ParseCardPool(const proto::game::redis::CCardPool& stpool)
{
	_uint8 Pool[POOL_CARD_MAX];
	for (int i = 0; i < stpool.m_pool_size(); ++i)
	{
		Pool[i] = stpool.m_pool(i);
	}

	m_CardPool.SetInfo(stpool.m_seedid(), (E_POOL_MODE)stpool.m_mode(), Pool, stpool.m_maxcount(), 
		stpool.m_count(), stpool.m_current(), stpool.m_last(), stpool.m_jokercard(), stpool.m_jokercount());

	//m_CardPool.Type().SetInfo((E_POOL_MODE)stpool.m_mode(), Pool, stpool.m_maxcount(), stpool.m_jokercard());
}

void Table::SerializeHuFanInfo(proto::game::redis::HuFanInfo* psthufaninfo)
{
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		psthufaninfo->add_m_bishu(m_bIsHu[i]);
		psthufaninfo->add_m_hufannums(m_HuFanNums[i]);

		proto::game::redis::tagGangScoreInfo* pstgangscoreinfo = psthufaninfo->add_m_stgang();
		pstgangscoreinfo->set_itotalscore(m_stGang[i].iTotalScore);
		for (int j = 0; j < m_stGang[i].Gang.Size(); ++j)
		{
			proto::game::redis::tagGangScore* pstgang = pstgangscoreinfo->add_gang();
			pstgang->set_card(m_stGang[i].Gang[j].Card);
			pstgang->set_chairid(m_stGang[i].Gang[j].ChairID);
			pstgang->set_itype(m_stGang[i].Gang[j].iType);
		}

		proto::game::redis::tagHuInfo* psthuinfo = psthufaninfo->add_m_sthuscore();
		psthuinfo->set_itotalscore(m_stHuScore[i].iTotalScore);
		for (int j = 0; j < m_stHuScore[i].stHu.Size(); ++j)
		{
			proto::game::redis::tagHuRecord* psthu = psthuinfo->add_sthu();
			psthu->set_bhucard(m_stHuScore[i].stHu[j].bHuCard);
			psthu->set_bpashuchairid(m_stHuScore[i].stHu[j].bPasHuChairID);
			psthu->set_btype(m_stHuScore[i].stHu[j].bType);	
			psthu->set_ifannum(m_stHuScore[i].stHu[j].iFanNum);	
		}

		proto::game::redis::MJ_BUFFER* pstofan = psthufaninfo->add_m_ofan();
		for (int j = 0; j < m_oFan[i].Size(); ++j)
		{
			pstofan->add_card(m_oFan[i][j]);
		}
	}	
}

void Table::ParseHuFanInfo(const proto::game::redis::HuFanInfo& sthufaninfo)
{
	if (sthufaninfo.m_bishu_size() != GAME_PLAYER) return;

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		m_bIsHu[i] = sthufaninfo.m_bishu(i);
		m_HuFanNums[i] = sthufaninfo.m_hufannums(i);

		const proto::game::redis::tagGangScoreInfo& stgangscoreinfo = sthufaninfo.m_stgang(i);
		m_stGang[i].iTotalScore = stgangscoreinfo.itotalscore();
		for (int j = 0; j < stgangscoreinfo.gang_size(); ++j)
		{
			const proto::game::redis::tagGangScore& stgangscore = stgangscoreinfo.gang(j);
			tagGangScore stGangScore;
			stGangScore.Card = stgangscore.card();
			stGangScore.ChairID = stgangscore.chairid();
			stGangScore.iType = stgangscore.itype();
			m_stGang[i].Gang.Add(stGangScore);
		}

		const proto::game::redis::tagHuInfo& sthuinfo = sthufaninfo.m_sthuscore(i);
		m_stHuScore[i].iTotalScore = sthuinfo.itotalscore();
		for (int j = 0; j < sthuinfo.sthu_size(); ++j)
		{
			const proto::game::redis::tagHuRecord& sthurecord = sthuinfo.sthu(j);
			tagHuRecord stHuRecord;
			stHuRecord.bHuCard = sthurecord.bhucard();
			stHuRecord.bPasHuChairID = sthurecord.bpashuchairid();
			stHuRecord.bType = sthurecord.btype();
			stHuRecord.iFanNum = sthurecord.ifannum();
			m_stHuScore[i].stHu.Add(stHuRecord);
		}

		const proto::game::redis::MJ_BUFFER& stofan = sthufaninfo.m_ofan(i);
		for (int j = 0; j < stofan.card_size(); ++j)
		{
			m_oFan[i].Add(stofan.card(j));
		}
	}
}

void Table::SerializePlayers(proto::game::redis::tagRedisInfo &stTableInfo)
{
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		proto::game::redis::tagPlayers* pstPlayer = stTableInfo.add_players();

		Player* player = m_SeatPlayer[i].player;

		if (player == NULL) continue;

		pstPlayer->set_uid(player->uid);
		pstPlayer->set_tid(player->tid_str);
		pstPlayer->set_skey(player->skey);
		pstPlayer->set_name(player->name);
		pstPlayer->set_avatar(player->avatar);
		pstPlayer->set_seatid(player->seatid);
		pstPlayer->set_sex(player->sex);				
		pstPlayer->set_exp(player->exp);
		pstPlayer->set_rmb(player->rmb);
		pstPlayer->set_money(player->money);
		pstPlayer->set_coin(player->coin);
		pstPlayer->set_pcount(player->pcount);
		pstPlayer->set_vlevel(player->vlevel);
		pstPlayer->set_status(player->status);
		pstPlayer->set_logout_type(player->logout_type);
		pstPlayer->set_down_tag(player->down_tag);
		pstPlayer->set_usertype(player->usertype);
		pstPlayer->set_disbandcount(player->DisBandCount);
	}
}

void Table::ParsePlayers(const proto::game::redis::tagRedisInfo &stTableInfo)
{
	for (int i = 0; i < stTableInfo.players_size(); ++i)
	{
		const proto::game::redis::tagPlayers& stPlayer = stTableInfo.players(i);

		if (stPlayer.uid() == 0) continue;

		Player* player = new Player();
		
		player->uid = stPlayer.uid();
		player->tid_str = stPlayer.tid();
		player->skey = stPlayer.skey(); 
		player->name = stPlayer.name();
		player->avatar = stPlayer.avatar();
		player->seatid = stPlayer.seatid();
		player->sex = stPlayer.sex();				
		player->exp = stPlayer.exp();
		player->rmb = stPlayer.rmb();
		player->money = stPlayer.money();
		player->coin = stPlayer.coin();
		player->pcount = stPlayer.pcount();
		player->vlevel = stPlayer.vlevel();
		player->status = stPlayer.status();
		player->logout_type = stPlayer.logout_type();
		player->down_tag = stPlayer.down_tag();
		player->usertype = stPlayer.usertype();
		player->DisBandCount = stPlayer.disbandcount();

		m_SeatPlayer[i].player = player;
		m_mapTable_Player[player->uid] = player;
	}
}

void Table::SerializeTableInfo(proto::game::redis::tagRedisInfo &stTableInfo)
{
	stTableInfo.set_tid(tid);
	stTableInfo.set_vid(vid);
	stTableInfo.set_zid(zid);
	stTableInfo.set_base_money(base_money);
	stTableInfo.set_roomgold(roomGold);
	stTableInfo.set_stand_money(stand_money);
	stTableInfo.set_lose_exp(lose_exp);
	stTableInfo.set_win_exp(win_exp);
	stTableInfo.set_m_basescore(m_BaseScore);

	SerializeMJCustomizedConf(stTableInfo.mutable_m_conf());

	stTableInfo.set_m_room_owner_uid(m_room_owner_uid);
	stTableInfo.set_m_room_status(m_room_status);

	SerializeTable_TotalRecordInfo(stTableInfo.mutable_m_totalrecordinfo());

	stTableInfo.set_m_game_player(m_GAME_PLAYER);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_online_beat(m_Online_Beat[i]);
	}
	stTableInfo.set_m_videostr(m_videostr);
	stTableInfo.set_m_randomseat(m_RandomSeat);
	stTableInfo.set_m_offlinenoop(m_OfflineNoOp);
	stTableInfo.set_m_bdelaytime(m_bDelayTime);

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		proto::game::redis::Seat* pstseat = stTableInfo.add_m_seatplayer();
		pstseat->set_state(m_SeatPlayer[i].state);
		pstseat->set_seatid(m_SeatPlayer[i].seatid);
		pstseat->set_action(m_SeatPlayer[i].action);
		pstseat->set_uid(m_SeatPlayer[i].uid);
	}

	proto::game::redis::Shai_Set* pstshai = stTableInfo.mutable_m_start_shai();
	pstshai->set_num(m_Start_Shai.Num);
	for (int i = 0; i < 7; ++i)
	{
		pstshai->add_shai(m_Start_Shai.Shai[i]);
	}

	stTableInfo.set_m_bcurplayers(m_bCurPlayers);
	stTableInfo.set_m_bgamestate(m_bGameState);
	stTableInfo.set_m_disband_resuser(m_Disband_ResUser);

	SerializeHandCards(stTableInfo);

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_isoffline(m_IsOffline[i]);
		stTableInfo.add_m_bistrustee(m_bIsTrustee[i]);
		stTableInfo.add_m_bautohu(m_bAutoHu[i]);
	}

	stTableInfo.set_m_bgangcalcimme(m_bGangCalcImme);
	stTableInfo.set_m_bbanker(m_bBanker);
	stTableInfo.set_m_last_banker(m_last_Banker);

	SerializeCardPool(stTableInfo.mutable_m_cardpool());

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_bgetcardcount(m_bGetCardCount[i]);
		stTableInfo.add_m_buseroperate(m_bUserOperate[i]);
	}

	stTableInfo.set_m_bcurchairid(m_bCurChairID);
	stTableInfo.set_m_boutcardchairid(m_bOutCardChairID);
	stTableInfo.set_m_bcuroutcard(m_bCurOutCard);
	stTableInfo.set_m_bcurcard(m_bCurCard);

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		proto::game::redis::MJ_BUFFER* pstoutcard = stTableInfo.add_m_stuseroutcard();
		for (int j = 0; j < m_stUserOutCard[i].Size(); ++j)
		{
			pstoutcard->add_card(m_stUserOutCard[i][j]);
		}
	}

	proto::game::redis::MultiOpInfo* pstmultiopinfo = stTableInfo.mutable_multiopinfo();
	pstmultiopinfo->set_m_bmultiopusernum(m_bMultiOpUserNum);
	pstmultiopinfo->set_m_bmultidoneopnum(m_bMultiDoneOpNum);
	pstmultiopinfo->set_m_bmultihuopecount(m_bMulitHuOpeCount);
	pstmultiopinfo->set_m_bmultiophunum(m_bMultiOpHuNum);
	pstmultiopinfo->set_m_bmulitfirstoperate(m_bMulitFirstOperate);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		pstmultiopinfo->add_m_bmultiuserdoneop(m_bMultiUserDoneOp[i]);
		pstmultiopinfo->add_m_bmultiuserdoneopcard(m_bMultiUserDoneOpCard[i]);	
	}

	for (int i = 0; i < m_PassHuPlayer.Size(); ++i)
	{
		stTableInfo.add_m_passhuplayer(m_PassHuPlayer[i]);
	}

	proto::game::redis::RobAddInfo* pstrobaddinfo = stTableInfo.mutable_robaddinfo();
	pstrobaddinfo->set_m_roboperatenum(m_RobOperateNum);
	pstrobaddinfo->set_m_robhunum(m_RobHuNum);
	pstrobaddinfo->set_m_wanggangplayer(m_WangGangPlayer);
	pstrobaddinfo->set_m_wanggangplayeroptype(m_WangGangPlayerOpType);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		pstrobaddinfo->add_m_robhudoneop(m_RobHuDoneOp[i]);
		pstrobaddinfo->add_m_baddfanflag(m_bAddFanFlag[i]);		
	}
	pstrobaddinfo->set_m_robwangangcard(m_RobWanGangCard);
	pstrobaddinfo->set_m_brobwangang(m_bRobWanGang);
	pstrobaddinfo->set_m_brobhu(m_bRobHu);
	pstrobaddinfo->set_m_brobganghujudge(m_bRobGangHuJudge);

	proto::game::redis::LOWLEVELOPERATE* pstrecord = stTableInfo.mutable_m_brecordmultiop();
	pstrecord->set_ioptype(m_bRecordMultiOp.iOpType);
	pstrecord->set_chairid(m_bRecordMultiOp.ChairID);
	pstrecord->set_bcard(m_bRecordMultiOp.bCard);
	
	SerializeHuFanInfo(stTableInfo.mutable_hufaninfo());

	stTableInfo.set_m_applydisbandchairid(m_ApplyDisbandChairid);
	stTableInfo.set_m_bisdisbandtable(m_bIsDisbandTable);
	stTableInfo.set_m_bdisbandcount(m_bDisbandCount);
	stTableInfo.set_m_nplayingtimes(m_nPlayingTimes);
	stTableInfo.set_m_bouttimer(m_bOutTimer);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_disbandchoose(m_DisbandChoose[i]);
		stTableInfo.add_m_bovertimestate(m_bOverTimeState[i]);	
		stTableInfo.add_m_userdissstate(m_UserDissState[i]);
	}

	stTableInfo.set_m_bdeletetable(m_bDeleteTable);
	stTableInfo.set_m_bistestsvr(m_bIsTestSvr);

	proto::game::redis::MJ_BUFFER* pstconfig = stTableInfo.mutable_m_room_config();
	for (int i = 0; i < m_room_config.Size(); ++i)
	{
		pstconfig->add_card(m_room_config[i]);
	}
	proto::game::redis::MJ_BUFFER* pstniao = stTableInfo.mutable_m_niao();
	for (int i = 0; i < m_niao.Size(); ++i)
	{
		pstniao->add_card(m_niao[i]);
	}
	proto::game::redis::MJ_BUFFER* pstzhongniao = stTableInfo.mutable_m_zhong_niao();
	for (int i = 0; i < m_zhong_niao.Size(); ++i)
	{
		pstzhongniao->add_card(m_zhong_niao[i]);
	}
	stTableInfo.set_m_daniaoid(m_DaNiaoID);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		proto::game::redis::MJ_BUFFER* pstpeng = stTableInfo.add_m_peng();
		for (int j = 0; j < m_Peng[i].Size(); ++j)
		{
			pstpeng->add_card(m_Peng[i][j]);
		}
	}

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		proto::game::redis::tagPlayerGPS* pstgps = stTableInfo.add_m_mapplayergps();
		pstgps->set_latitude(m_mapPlayerGPS[m_SeatPlayer[i].uid].latitude);
		pstgps->set_longitude(m_mapPlayerGPS[m_SeatPlayer[i].uid].longitude);
		pstgps->set_city(m_mapPlayerGPS[m_SeatPlayer[i].uid].city);
	}

	SerializePlayers(stTableInfo);

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_useroperateid(m_UserOperateID[i]);
	}
	stTableInfo.set_m_autodisband(m_AutoDisband);
	stTableInfo.set_m_bisautodisband(m_bIsAutoDisband);
	stTableInfo.set_m_tablebranchid(m_TableBranchId);
	stTableInfo.set_m_ntotalcount(m_nTotalCount);
	stTableInfo.set_m_ncurrentcount(m_nCurrentCount);
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_userchoosemode(m_UserChooseMode[i]);
	}

	stTableInfo.set_m_start_time(m_start_time);

	stTableInfo.set_m_voice_room(m_voice_room);
	stTableInfo.set_m_public_channel_id(m_public_channel_id);
	for (size_t i = 0; i < m_group_channel_ids.size(); ++i)
	{
		stTableInfo.add_m_group_channel_ids(m_group_channel_ids[i]);
	}
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_voice_status(m_voice_status[i]);
	}

	for (size_t i = 0; i < m_vecOpCards.size(); ++i)
	{
		stTableInfo.add_m_vecopcards(m_vecOpCards[i]);
	}
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		stTableInfo.add_m_preofflinestate(m_PreOfflineState[i]);
	}
	stTableInfo.set_m_isclubroom(m_IsClubRoom);
	stTableInfo.set_m_maxhandcardsize(m_MaxHandCardSize);
}

void Table::ParseTableInfo(const proto::game::redis::tagRedisInfo &stTableInfo)
{
	tid = stTableInfo.tid();
	vid = stTableInfo.vid();
	zid = stTableInfo.zid();
	base_money = stTableInfo.base_money();
	roomGold = stTableInfo.roomgold();
	stand_money = stTableInfo.stand_money();
	lose_exp = stTableInfo.lose_exp();
	win_exp = stTableInfo.win_exp();
	m_BaseScore = stTableInfo.m_basescore();

	ParseMJCustomizedConf(stTableInfo.m_conf());

	m_room_owner_uid = stTableInfo.m_room_owner_uid();
	m_room_status = stTableInfo.m_room_status();

	ParseTable_TotalRecordInfo(stTableInfo.m_totalrecordinfo());

	m_GAME_PLAYER = stTableInfo.m_game_player();

	for (int i = 0; i < stTableInfo.m_online_beat_size(); ++i)
	{
		m_Online_Beat[i] = stTableInfo.m_online_beat(i);
	}
	m_videostr = stTableInfo.m_videostr();
	m_RandomSeat = stTableInfo.m_randomseat();
	m_OfflineNoOp = stTableInfo.m_offlinenoop();
	m_bDelayTime = stTableInfo.m_bdelaytime();

	for (int i = 0; i < stTableInfo.m_seatplayer_size(); ++i)
	{
		const proto::game::redis::Seat& stseat = stTableInfo.m_seatplayer(i);
		m_SeatPlayer[i].state = (Seat::SeatState)stseat.state();
		m_SeatPlayer[i].seatid = stseat.seatid();
		m_SeatPlayer[i].action = stseat.action();
		m_SeatPlayer[i].uid = stseat.uid();
	}

	const proto::game::redis::Shai_Set& stshai = stTableInfo.m_start_shai();
	m_Start_Shai.Num = stshai.num();
	for (int i = 0; i < stshai.shai_size(); ++i)
	{
		m_Start_Shai.Shai[i] = stshai.shai(i);
	}

	m_bCurPlayers = stTableInfo.m_bcurplayers();
	m_bGameState = (GAME_STATUS)stTableInfo.m_bgamestate();
	m_Disband_ResUser = stTableInfo.m_disband_resuser();

	ParseHandCards(stTableInfo);

	for (int i = 0; i < stTableInfo.m_isoffline_size(); ++i)
	{
		m_IsOffline[i] = stTableInfo.m_isoffline(i);
		m_bIsTrustee[i] = stTableInfo.m_bistrustee(i);
		m_bAutoHu[i] = stTableInfo.m_bautohu(i);
	}

	m_bGangCalcImme = stTableInfo.m_bgangcalcimme();
	m_bBanker = stTableInfo.m_bbanker();
	m_last_Banker = stTableInfo.m_last_banker();

	ParseCardPool(stTableInfo.m_cardpool());

	for (int i = 0; i < stTableInfo.m_buseroperate_size(); ++i)
	{
		m_bGetCardCount[i] = stTableInfo.m_bgetcardcount(i);
		m_bUserOperate[i] = stTableInfo.m_buseroperate(i);
	}

	m_bCurChairID = stTableInfo.m_bcurchairid();
	m_bOutCardChairID = stTableInfo.m_boutcardchairid();
	m_bCurOutCard = stTableInfo.m_bcuroutcard();
	m_bCurCard = stTableInfo.m_bcurcard();

	for (int i = 0; i < stTableInfo.m_stuseroutcard_size(); ++i)
	{
		const proto::game::redis::MJ_BUFFER& stoutcard = stTableInfo.m_stuseroutcard(i);
		for (int j = 0; j < stoutcard.card_size(); ++j)
		{
			m_stUserOutCard[i].Add(stoutcard.card(j));
		}
	}

	const proto::game::redis::MultiOpInfo& stmultiopinfo = stTableInfo.multiopinfo();
	m_bMultiOpUserNum = stmultiopinfo.m_bmultiopusernum();
	m_bMultiDoneOpNum = stmultiopinfo.m_bmultidoneopnum();
	m_bMulitHuOpeCount = stmultiopinfo.m_bmultihuopecount();
	m_bMultiOpHuNum = stmultiopinfo.m_bmultiophunum();
	m_bMulitFirstOperate = stmultiopinfo.m_bmulitfirstoperate();
	for (int i = 0; i < stmultiopinfo.m_bmultiuserdoneop_size(); ++i)
	{
		m_bMultiUserDoneOp[i] = stmultiopinfo.m_bmultiuserdoneop(i);
		m_bMultiUserDoneOpCard[i] = stmultiopinfo.m_bmultiuserdoneopcard(i);	
	}

	for (int i = 0; i < stTableInfo.m_passhuplayer_size(); ++i)
	{
		m_PassHuPlayer.Add(stTableInfo.m_passhuplayer(i));
	}

	const proto::game::redis::RobAddInfo& strobaddinfo = stTableInfo.robaddinfo();
	m_RobOperateNum = strobaddinfo.m_roboperatenum();
	m_RobHuNum = strobaddinfo.m_robhunum();
	m_WangGangPlayer = strobaddinfo.m_wanggangplayer();
	m_WangGangPlayerOpType = strobaddinfo.m_wanggangplayeroptype();
	for (int i = 0; i < strobaddinfo.m_robhudoneop_size(); ++i)
	{
		m_RobHuDoneOp[i] = strobaddinfo.m_robhudoneop(i);
		m_bAddFanFlag[i] = strobaddinfo.m_baddfanflag(i);		
	}
	m_RobWanGangCard = strobaddinfo.m_robwangangcard();
	m_bRobWanGang = strobaddinfo.m_brobwangang();
	m_bRobHu = strobaddinfo.m_brobhu();
	m_bRobGangHuJudge = strobaddinfo.m_brobganghujudge();

	const proto::game::redis::LOWLEVELOPERATE& strecord = stTableInfo.m_brecordmultiop();
	m_bRecordMultiOp.iOpType = strecord.ioptype();
	m_bRecordMultiOp.ChairID = strecord.chairid();
	m_bRecordMultiOp.bCard = strecord.bcard();
	
	ParseHuFanInfo(stTableInfo.hufaninfo());

	m_ApplyDisbandChairid = stTableInfo.m_applydisbandchairid();
	m_bIsDisbandTable = stTableInfo.m_bisdisbandtable();
	m_bDisbandCount = stTableInfo.m_bdisbandcount();
	m_nPlayingTimes = stTableInfo.m_nplayingtimes();
	m_bOutTimer = stTableInfo.m_bouttimer();
	for (int i = 0; i < stTableInfo.m_disbandchoose_size(); ++i)
	{
		m_DisbandChoose[i] = stTableInfo.m_disbandchoose(i);
		m_bOverTimeState[i] = stTableInfo.m_bovertimestate(i);	
		m_UserDissState[i] = stTableInfo.m_userdissstate(i);
	}

	m_bDeleteTable = stTableInfo.m_bdeletetable();
	m_bIsTestSvr = stTableInfo.m_bistestsvr();

	const proto::game::redis::MJ_BUFFER& stconfig = stTableInfo.m_room_config();
	for (int i = 0; i < stconfig.card_size(); ++i)
	{
		m_room_config.Add(stconfig.card(i));
	}
	const proto::game::redis::MJ_BUFFER& stniao = stTableInfo.m_niao();
	for (int i = 0; i < stniao.card_size(); ++i)
	{
		m_niao.Add(stniao.card(i));
	}
	const proto::game::redis::MJ_BUFFER& stzhongniao = stTableInfo.m_zhong_niao();
	for (int i = 0; i < stzhongniao.card_size(); ++i)
	{
		m_zhong_niao.Add(stzhongniao.card(i));
	}
	m_DaNiaoID = stTableInfo.m_daniaoid();
	for (int i = 0; i < stTableInfo.m_peng_size(); ++i)
	{
		const proto::game::redis::MJ_BUFFER& stpeng = stTableInfo.m_peng(i);
		for (int j = 0; j < stpeng.card_size(); ++j)
		{
			m_Peng[i].Add(stpeng.card(j));
		}
	}

	for (int i = 0; i < stTableInfo.m_mapplayergps_size(); ++i)
	{
		const proto::game::redis::tagPlayerGPS& stgps = stTableInfo.m_mapplayergps(i);
		tagPlayerGPS gps;
		gps.latitude = stgps.latitude();
		gps.longitude = stgps.longitude();
		gps.city = stgps.city();
		m_mapPlayerGPS[m_SeatPlayer[i].uid] = gps;
	}

	ParsePlayers(stTableInfo);

	for (int i = 0; i < stTableInfo.m_useroperateid_size(); ++i)
	{
		m_UserOperateID[i] = stTableInfo.m_useroperateid(i);
	}
	m_AutoDisband = stTableInfo.m_autodisband();
	m_bIsAutoDisband = stTableInfo.m_bisautodisband();
	m_TableBranchId = stTableInfo.m_tablebranchid();
	m_nTotalCount = stTableInfo.m_ntotalcount();
	m_nCurrentCount = stTableInfo.m_ncurrentcount();
	for (int i = 0; i < stTableInfo.m_userchoosemode_size(); ++i)
	{
		m_UserChooseMode[i] = stTableInfo.m_userchoosemode(i);
	}

	m_start_time = stTableInfo.m_start_time();

	m_voice_room = stTableInfo.m_voice_room();
	m_public_channel_id = stTableInfo.m_public_channel_id();
	for (int i = 0;i < stTableInfo.m_group_channel_ids_size(); ++i)
	{
		m_group_channel_ids.push_back(stTableInfo.m_group_channel_ids(i));
	}
	for (int i = 0; i < stTableInfo.m_voice_status_size(); ++i)
	{
		m_voice_status[i] = stTableInfo.m_voice_status(i);
	}
	
	for (int i = 0; i < stTableInfo.m_vecopcards_size(); ++i)
	{
		m_vecOpCards.push_back(stTableInfo.m_vecopcards(i));
	}
	for (int i = 0; i < stTableInfo.m_preofflinestate_size(); ++i)
	{
		m_PreOfflineState[i] = stTableInfo.m_preofflinestate(i);
	}
	m_IsClubRoom = stTableInfo.m_isclubroom();
	m_MaxHandCardSize = stTableInfo.m_maxhandcardsize();	
}

int Table::SaveTableInfoToRedis()
{
	
	return 0;
}

int Table::ParseTableInfoFromRedis(std::string &data_str)
{

	return 0;
}

int Table::DeleteDataFromRedis(std::string tid)
{

	return 0;
}