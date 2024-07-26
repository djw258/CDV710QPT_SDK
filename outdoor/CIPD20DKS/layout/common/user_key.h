#ifndef _USER_KEY_H_
#define _USER_KEY_H_
#include <stdbool.h>

enum{
	KEY_STATE_RELEASE,
        KEY_STATE_SLOT_DOWN,
        KEY_STATE_LONG_DOWN,
};
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:50:54
** 说明: 初始化按键检测线程 
***********************************************/
bool user_key_init(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:50:54
** 说明: 按键状态
***********************************************/
int user_key_state_get(void);
#endif
