#ifndef _USER_CALL_LIST_H_
#define _USER_CALL_LIST_H_
#include "layout_define.h"

typedef enum
{
    CALL_LOG_UNKNOW,
    CALL_LOG_IN_AND_NO_ANSWER,
    CALL_LOG_CALL_OUT,
    CALL_LOG_IN_AND_ANSWER

} CALL_LOG_TYPE;
/***
**   日期:2022-06-20 09:56:23
**   作者: leo.liu
**   函数作用：加载列表
**   参数说明:
***/
bool call_list_init(void);
/***
**   日期:2022-06-20 10:01:56
**   作者: leo.liu
**   函数作用：添加一个警报信息
**   参数说明:
***/
bool call_list_add(int type, char *doorname, int duration, struct tm *tm);
/***
**   日期:2022-06-20 10:02:35
**   作者: leo.liu
**   函数作用：删除所有列表
**   参数说明:
***/
bool call_list_del_all(void);

/***
** 日期: 2022-05-17 13:53
** 作者: leo.liu
** 函数作用：删除一个列表
** 返回参数说明：
***/
bool call_list_del(int index);

/***
**   日期:2022-06-20 10:14:01
**   作者: leo.liu
**   函数作用：获取列表
**   参数说明:
***/
bool call_list_get(int index, CALL_LOG_TYPE *type, char *doorname, int *duration, struct tm *tm);
/***
**   日期:2022-06-20 10:20:05
**   作者: leo.liu
**   函数作用：获取总数
**   参数说明:
***/
int call_list_total_get(void);
#endif