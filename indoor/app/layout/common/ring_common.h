#ifndef _RING_COMMON_H_
#define _RING_COMMON_H_
#include <stdbool.h>

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 触摸音效播放
***********************************************/
bool ring_touch_play(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 播放门口机的音效
***********************************************/
bool ring_door_call_play(int index, int count);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 开锁铃声
***********************************************/
bool ring_unlock_play(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明:intercom 铃声
***********************************************/
bool ring_intercom_play(int index, int count);
/************************************************************
** 函数说明: 警报ling
** 作者: xiaoxiao
** 日期: 2023-05-16 11:43:00
** 参数说明:
** 注意事项:
************************************************************/
bool ring_alarm_play(void);
/************************************************************
** 函数说明: 播放蜂鸣器报警
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_buzzer_play(int index);
/************************************************************
** 函数说明: 播放公共入口铃声
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_common_door_play(int index, int count);

/************************************************************
** 函数说明: 播放警卫室call机铃声
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_guard_play(int index, int count);

/************************************************************
** 函数说明:播放发送音
** 作者: xiaoxiao
** 日期：2023-11-21 10:09:43
** 参数说明:
** 注意事项：
************************************************************/
bool send_call_play(int index, int count);
#endif