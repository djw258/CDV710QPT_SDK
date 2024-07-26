
#include "user_linphone.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <pthread.h>
#include "common/sat_main_event.h"
#include "common/sat_user_common.h"

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 9:59:5
** 说明: 初始化user linphone相关
***********************************************/
void user_linphone_init(void)
{

	/*
	 * @日期: 2022-08-20
	 * @作者: leo.liu
	 * @注释: 开启pjsip
	 */
	extern int linphone_init(int argc, char *argv[]);

	linphone_init(0, NULL);
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 获取编译日期
***********************************************/
bool platform_build_date_get(struct tm *tm)
{
#define MONTH_PER_YEAR 12   // 一年12月
#define YEAR_MONTH_DAY 20   // 年月日缓存大小
#define HOUR_MINUTES_SEC 20 // 时分秒缓存大小

	const char year_month[MONTH_PER_YEAR][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char compile_date[YEAR_MONTH_DAY] = {0};
	char compile_time[HOUR_MINUTES_SEC] = {0};
	char str_month[4] = {0};

	sprintf(compile_date, "%s", __DATE__); // "Aug 23 2016"
	sprintf(compile_time, "%s", __TIME__); // "10:59:19"

	sscanf(compile_date, "%s %d %d", str_month, &(tm->tm_mday), &(tm->tm_year));
	sscanf(compile_time, "%d:%d:%d", &(tm->tm_hour), &(tm->tm_min), &(tm->tm_sec));

	for (int i = 0; i < MONTH_PER_YEAR; ++i)
	{
		if (strncmp(str_month, year_month[i], 3) == 0)
		{
			tm->tm_mon = i + 1;
			return true;
		}
	}
	return false;
}
