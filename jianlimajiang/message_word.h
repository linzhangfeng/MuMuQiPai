#ifndef __MESSAGE_WORD_H__
#define __MESSAGE_WORD_H__

enum MSG_WORD
{
	E_MW_ERR,
	E_MW_GUO_ZHUANG,
	E_MW_LOU_PENG,
	E_MW_MING_GANG_BIAN_GONG_GANG,
	E_MW_PING_HU_BU_PAO_HU,
	E_MW_WAN_GANG_GUO_SHOU,
	E_MW_RE_ZI_BU_NENG_HU,
	E_MW_SHUAI_ZI_BU_KE_PAO_HU,
	E_MW_BU_KE_DAO_CHE,
	E_MW_KOU_TING_QI_HU,
	E_MW_PENGPENGHU_JIANZIHU,
	E_MW_BU_NENG_GANG
};
const char* msg_word(int index);

#endif // !__MESSAGE_WORD_H__