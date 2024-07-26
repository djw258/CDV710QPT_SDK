

#ifndef _USER_LINPHONE_H_
#define _USER_LINPHONE_H_
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 9:59:5
** 说明: 初始化user linphone相关 
***********************************************/
void user_linphone_init(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 获取编译日期
***********************************************/
bool platform_build_date_get(struct tm *tm);

#endif

