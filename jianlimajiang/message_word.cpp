#include "message_word.h"

const char* msg_word(int index)
{
	const char* word[] =
	{
		"错误,未定义",
		"没过庄不能胡",
		"当前未过庄，不能操作",
		"明杠变公杠不算分",
		"平胡不炮胡",
		"弯杠过手",
		"热字不能胡",
		"甩牌不可炮胡",
		"不可倒车",
		"扣听弃胡只能自摸",
		"碰碰胡见字胡仅限自摸",
		"小于14牌时，不能杠牌"
	};
	if (index < (int)(sizeof(word) / sizeof(word[0])))
	{
		return word[index];
	}
	return word[0];
}