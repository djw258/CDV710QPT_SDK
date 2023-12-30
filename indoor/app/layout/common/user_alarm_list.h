#ifndef _USER_ALARM_LIST_H_
#define _USER_ALARM_LIST_H_
#include "layout_define.h"

typedef enum
{
    emergency_unknow = 0,
    emergency_occur,
    emergency_stop,
    emergency_return,
    security_emergency,
    security_emergency_stop,
    security_emergency_return,
} alarm_type;
/***
**   日期:2022-06-20 09:56:23
**   作者: leo.liu
**   函数作用：加载列表
**   参数说明:
***/
bool alarm_list_init(void);
/***
**   日期:2022-06-20 10:01:56
**   作者: leo.liu
**   函数作用：添加一个警报信息
**   参数说明:
***/
bool alarm_list_add(int type, int id, struct tm *tm);
/***
**   日期:2022-06-20 10:02:35
**   作者: leo.liu
**   函数作用：删除所有列表
**   参数说明:
***/
bool alarm_list_del_all(void);
/***
**   日期:2022-06-20 10:14:01
**   作者: leo.liu
**   函数作用：获取列表
**   参数说明:
***/
bool alarm_list_get(int index, int *type, int *ch, struct tm *tm);
/***
**   日期:2022-06-20 10:20:05
**   作者: leo.liu
**   函数作用：获取总数
**   参数说明:
***/
bool alarm_list_total_get(int *total);

/************************************************************
** 函数说明: 根据报警通道获取最近一次报警时间
** 作者: xiaoxiao
** 日期: 2023-05-06 23:06:39
** 参数说明:
** 注意事项:
************************************************************/
bool alarm_occur_time_get(int ch, struct tm *tm);
#endif