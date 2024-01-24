#ifndef _USER_STANDBY_H_
#define _USER_STANDBY_H_
#include "common/sat_main_event.h"
#include <stdbool.h>
/***
** 日期: 2022-05-10 08:33
** 作者: leo.liu
** 函数作用：初始化待机检测
** 返回参数说明：
***/
bool standby_timer_init(sat_layout_info *playout, int timeout);

/***
** 日期: 2022-05-10 08:47
** 作者: leo.liu
** 函数作用：待机检测重新及时
** 返回参数说明：
***/
bool standby_timer_restart(bool fouce_enable);

/***
** 日期: 2022-05-10 08:42
** 作者: leo.liu
** 函数作用：关闭待机检测
** 返回参数说明：
***/
bool standby_timer_close(void);
/***
** 日期: 2022-05-10 08:42
** 作者: leo.liu
** 函数作用：待机检测状态获取
** 返回参数说明：
***/
bool standby_timer_status_get(void);

/************************************************************
** 函数说明: 重置待机时间
** 作者: xiaoxiao
** 日期: 2023-06-07 14:52:47
** 参数说明:
** 注意事项:
************************************************************/
bool standby_timer_reset(int timeout);

#endif