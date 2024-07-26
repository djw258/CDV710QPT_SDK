#ifndef _USER_TIME_H_
#define _USER_TIME_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
/***
** 日期: 2022-04-26 10:58
** 作者: leo.liu
** 函数作用：将RTC时间同步到系统时间
** 返回参数说明：
***/
void user_time_sync(void);
/***
** 日期: 2022-04-26 10:33
** 作者: leo.liu
** 函数作用：用户时间初始化
** 返回参数说明：
***/
bool user_time_init(void);
/***
** 日期: 2022-04-26 10:49
** 作者: leo.liu
** 函数作用：获取用户时间
** 返回参数说明：
***/
bool user_time_read(struct tm *tm);
/***
** 日期: 2022-04-26 10:57
** 作者: leo.liu
** 函数作用：设置系统时间并同步到RTC
** 返回参数说明：
***/
bool user_time_set(struct tm *tm);
/***
** 日期: 2022-04-26 11:02
** 作者: leo.liu
** 函数作用：获取当前时间戳
** 返回参数说明：
***/
unsigned long long user_timestamp_get(void);
/***
** 日期: 2022-04-26 11:02
** 作者: leo.liu
** 函数作用：获取当前时间戳
** 返回参数说明：
***/
unsigned long long user_timestamp_get_us(void);
/***
**   日期:2022-06-09 11:45:51
**   作者: leo.liu
**   函数作用：将tm转换成sec
**   参数说明:
***/
int user_tm_conver_sec(struct tm *info);
/***
** 日期: 2022-05-09 17:29
** 作者: leo.liu
** 函数作用：获取当前日期是星期几
** 返回参数说明：
***/
int user_cur_time_is_week(struct tm *date);
/***
** 日期: 2022-05-09 17:32
** 作者: leo.liu
** 函数作用：获取当前月份下最后一天日期
** 返回参数说明：
***/
int user_cur_time_last_day(struct tm *date);
/***
**   日期:2022-06-18 09:42:23
**   作者: leo.liu
**   函数作用：判断时间是否合法
**   参数说明:
***/
bool user_time_valid_check(struct tm *tm);
#endif