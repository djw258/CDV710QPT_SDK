#ifndef _USER_ALARM_LIST_H_
#define _USER_ALARM_LIST_H_
#include "layout_define.h"

typedef enum
{
    emergency_occur,
    emergency_stop,
    emergency_return,
    security_emergency,
    security_emergency_stop,
    security_emergency_return,
} alarm_type;
typedef struct
{
    int type;
    int ch;
    struct tm time;
    int alarm_list_total;
} alarm_list_info;
#define ALARM_LIST_MAX 256
typedef struct
{
    int alarm_list_total;
    alarm_list_info alarm_list[ALARM_LIST_MAX];

} USER_ALARM_LIST;

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
** 函数说明: 返回警报列表的地址
** 作者: xiaoxiao
** 日期：2024-03-27 09:53:32
** 参数说明:
** 注意事项：
************************************************************/
USER_ALARM_LIST *alarm_list_info_get(void);
/************************************************************
** 函数说明: 同步警报信息
** 作者: xiaoxiao
** 日期：2024-03-27 09:53:32
** 参数说明:
** 注意事项：
************************************************************/
bool alarm_list_sync(void);

/************************************************************
** 函数说明: 获得由报警时间先后次序对应的传感器下标
** 作者: xiaoxiao
** 日期：2024-04-03 23:06:05
** 参数说明:
** 注意事项：
************************************************************/

void sort_channels_by_time(int *sorted_channels);
#endif