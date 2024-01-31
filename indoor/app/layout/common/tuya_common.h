#ifndef _TUYA_COMMON_H_
#define _TUYA_COMMON_H_
#include "common/sat_main_event.h"
/*tuya 事件处理函数*/
bool tuya_event_defalut_handle(TUYA_CMD cmd, int arg);

/***tuya通道设置****/
void tuya_monitor_channel_set(int ch);

/***tuya通道获取****/
int tuya_monitor_channel_get(void);

bool tuya_app_quit_status_active_get(void);

void tuya_app_quit_status_active_set(bool en);
/************************************************************
** 函数说明: 上传有效通道
** 作者: xiaoxiao
** 日期: 2023-05-30 14:24:54
** 参数说明:
** 注意事项:
************************************************************/
bool layout_monitor_report_vaild_channel(void);
#endif