#ifndef _LAYOUT_COMMON_H_
#define _LAYOUT_COMMON_H_
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 音频回调处理函数
***********************************************/
void audio_output_event_default_process(int arg);


/************************************************************
** 函数说明: 警报处理函数
** 作者: xiaoxiao
** 日期: 2023-04-28 17:07:39
** 参数说明: 
** 注意事项: 
************************************************************/
void layout_alarm_trigger_default(int arg1,int arg2);

/************************************************************
** 函数说明: 获取警报通道
** 作者: xiaoxiao
** 日期: 2023-05-04 17:06:40
** 参数说明: 
** 注意事项: 
************************************************************/
int layout_alarm_alarm_channel_get(void);

/************************************************************
** 函数说明: 设置警报通道
** 作者: xiaoxiao
** 日期: 2023-05-04 17:09:12
** 参数说明: 
** 注意事项: 
************************************************************/
void layout_alarm_alarm_channel_set(int ch);
/************************************************************
** 函数说明: 检测是否需要触发警报
** 作者: xiaoxiao
** 日期: 2023-05-08 09:07:36
** 参数说明: 
** 注意事项: 
************************************************************/
bool alarm_trigger_check(void);

/************************************************************
** 函数说明: 设置last_call_log new flag
** 作者: xiaoxiao
** 日期: 2023-05-17 16:11:26
** 参数说明: 
** 注意事项: 
************************************************************/
void layout_last_call_new_flag_set(bool new);

/************************************************************
** 函数说明: 获取是否有未查看的通话记录标志
** 作者: xiaoxiao
** 日期: 2023-05-17 16:09:19
** 参数说明: 
** 注意事项: 
************************************************************/
bool layout_last_call_new_flag_get(void);


#endif