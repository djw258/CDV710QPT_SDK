#include "user_alarm_list.h"

#define USER_ALARM_LIST_PATH "/app/data/user_alarm_list.cfg"

static USER_ALARM_LIST user_alarm_list = {0};

/************************************************************
** 函数说明: 返回警报列表的地址
** 作者: xiaoxiao
** 日期：2024-03-27 09:53:32
** 参数说明:
** 注意事项：
************************************************************/
USER_ALARM_LIST *
alarm_list_info_get(void)
{
	return &user_alarm_list;
}
bool alarm_list_sync(void)
{
	int fd = open(USER_ALARM_LIST_PATH, O_CREAT | O_WRONLY);
	if (fd < 0)
	{
		return false;
	}
	if (write(fd, &user_alarm_list, sizeof(user_alarm_list)) < 0)
	{
		perror("write failed\n");
	}
	close(fd);
	return true;
}
/***
**   日期:2022-06-20 09:56:23
**   作者: leo.liu
**   函数作用：加载列表
**   参数说明:
***/
bool alarm_list_init(void)
{
	user_alarm_list.alarm_list_total = 0;
	memset(&user_alarm_list, 0, sizeof(user_alarm_list));
	int fd = open(USER_ALARM_LIST_PATH, O_RDONLY);
	if (fd < 0)
	{
		return false;
	}
	read(fd, &user_alarm_list, sizeof(USER_ALARM_LIST));
	printf("alarm_list_total is %d\n", user_alarm_list.alarm_list_total);
	close(fd);
	alarm_list_sync();
	return true;
}
/***
**   日期:2022-06-20 10:01:56
**   作者: leo.liu
**   函数作用：添加一个警报信息
**   参数说明:
***/
bool alarm_list_add(int type, int id, struct tm *tm)
{

	printf("%04d.%02d.%02d  %02d:%02d:%02d\n", tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	if (user_alarm_list.alarm_list_total == ALARM_LIST_MAX)
	{
		user_alarm_list.alarm_list_total--;
		memmove(&user_alarm_list.alarm_list[0], &user_alarm_list.alarm_list[1], user_alarm_list.alarm_list_total * sizeof(alarm_list_info));
	}
	user_alarm_list.alarm_list[user_alarm_list.alarm_list_total].type = type;
	user_alarm_list.alarm_list[user_alarm_list.alarm_list_total].ch = id;
	user_alarm_list.alarm_list[user_alarm_list.alarm_list_total].time = *tm;
	user_alarm_list.alarm_list_total++;
	alarm_list_sync();
	return true;
}
/***
**   日期:2022-06-20 10:02:35
**   作者: leo.liu
**   函数作用：删除所有列表
**   参数说明:
***/
bool alarm_list_del_all(void)
{
	remove(USER_ALARM_LIST_PATH);
	system("sync");
	user_alarm_list.alarm_list_total = 0;
	memset(user_alarm_list.alarm_list, 0, sizeof(user_alarm_list.alarm_list));
	return true;
}
/***
**   日期:2022-06-20 10:14:01
**   作者: leo.liu
**   函数作用：获取列表
**   参数说明:
***/
bool alarm_list_get(int index, int *type, int *ch, struct tm *tm)
{
	if (index >= user_alarm_list.alarm_list_total)
	{
		return false;
	}
	*type = user_alarm_list.alarm_list[index].type;
	*ch = user_alarm_list.alarm_list[index].ch;
	*tm = user_alarm_list.alarm_list[index].time;
	return true;
}
/***
**   日期:2022-06-20 10:20:05
**   作者: leo.liu
**   函数作用：获取总数
**   参数说明:
***/
bool alarm_list_total_get(int *total)
{
	*total = user_alarm_list.alarm_list_total;
	return true;
}

/************************************************************
** 函数说明: 根据报警通道获取最近一次报警时间
** 作者: xiaoxiao
** 日期: 2023-05-06 23:06:39
** 参数说明:
** 注意事项:
************************************************************/
bool alarm_occur_time_get(int ch, struct tm *tm)
{
	if ((ch > 8) || (ch < 0))
	{
		return false;
	}

	for (int i = user_alarm_list.alarm_list_total; i >= 0; i--)
	{
		if (user_alarm_list.alarm_list[i].ch == ch)
		{
			*tm = user_alarm_list.alarm_list[i].time;
		}
	}
	return true;
}