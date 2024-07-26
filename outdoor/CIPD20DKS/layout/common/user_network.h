

#ifndef _USER_NETWORK_H_
#define _USER_NETWORK_H_
#include <stdlib.h>
#include <stdbool.h>
#include "user_data.h"

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 17:13:48
** 说明: 用户网络处理
***********************************************/
void user_network_init(void);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 15:21:6
** 说明: 获取组播IP地址
***********************************************/
char *user_linphone_local_multicast_get(void);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 查询是否在线
*****************************************************************/
bool sat_ipcamera_device_name_get(const char *sip_uri, int timeout);
#endif
