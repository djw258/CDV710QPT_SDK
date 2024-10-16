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
void layout_alarm_trigger_default(int arg1, int arg2);
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
 ** 函数说明: 根据报警通道获取最近一次报警时间
 ** 作者: xiaoxiao
 ** 日期: 2023-05-06 23:06:39
 ** 参数说明:
 ** 注意事项:
 ************************************************************/
bool alarm_occur_time_get(int ch, struct tm *tm);
/************************************************************
** 函数说明: 检测是否需要触发警报
** 作者: xiaoxiao
** 日期: 2023-05-08 09:07:36
** 参数说明:
** 注意事项:
************************************************************/
bool alarm_trigger_check(bool active);

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
/************************************************************
** 函数说明: SD状态改变默认回调
** 作者: xiaoxiao
** 日期: 2023-07-15 11:50:12
** 参数说明:
** 注意事项:
************************************************************/
void sd_state_change_default_callback(void);

/************************************************************
** 函数说明: 设置用户数据文件改变标记状态
** 作者: xiaoxiao
** 日期: 2023-08-04 08:33:44
** 参数说明: flag : 0x00:user_data 0x01:network_data
** 注意事项:
************************************************************/
void asterisk_server_sync_user_data_force(bool is_sync);
/************************************************************
** 函数说明: 设置警报改变标记状态
** 作者: xiaoxiao
** 日期: 2023-08-04 08:33:44
** 参数说明: flag : 0x00:user_data 0x01:network_data
** 注意事项:
************************************************************/
void asterisk_server_alarm_log_force(bool is_sync);

/************************************************************
** 函数说明: 设置用户注册数据改变标记状态
** 作者: xiaoxiao
** 日期: 2023-08-04 08:33:44
** 参数说明: flag : 0x00:user_data 0x01:network_data
** 注意事项:
************************************************************/
void asterisk_server_asterisk_data_force(bool is_sync);
/************************************************************
** 函数说明: 设置网络配置文件改变标记状态
** 作者: xiaoxiao
** 日期: 2023-08-04 08:33:44
** 参数说明: flag : 0x00:user_data 0x01:network_data
** 注意事项:
************************************************************/
void asterisk_server_sync_network_data_force(bool is_sync);
/************************************************************
** 函数说明: 设置RTC时间设置状态
** 作者: xiaoxiao
** 日期: 2023-08-04 08:33:44
** 参数说明: flag : 0x00:user_data 0x01:network_data
** 注意事项:
************************************************************/
void asterisk_server_sync_rtc_data_force(bool is_sync);

/************************************************************
** 函数说明: 密码输入检验公共函数创建
** 作者: xiaoxiao
** 日期: 2023-08-05 10:50:25
** 参数说明:
** 注意事项:
************************************************************/
void common_passwd_check_func_create(int cont_id, void (*callback)(void));

/************************************************************
** 函数说明: 判断是否是以一个合法的ipv4地址
** 作者: xiaoxiao
** 日期: 2023-04-27 17:30:01
** 参数说明:
** 注意事项:
************************************************************/
bool is_valid_ipv4(const char *s);

/************************************************************
** 函数说明: 蜂鸣器呼叫注册函数
** 作者: xiaoxiao
** 日期: 2023-04-27 17:30:01
** 参数说明:
** 注意事项:
************************************************************/
void buzzer_call_callback_register(void (*callback)(void));
/************************************************************
** 函数说明: 蜂鸣器触发检查
** 作者: xiaoxiao
** 日期: 2023-04-27 17:30:01
** 参数说明:
** 注意事项:
************************************************************/
bool buzzer_call_trigger_check(void);
/************************************************************
** 函数说明: 获取蜂鸣器触发时间戳
** 作者: xiaoxiao
** 日期：2023-10-06 18:41:46
** 参数说明:
** 注意事项：
************************************************************/
unsigned long long buzzer_call_timestamp_get();
/************************************************************
** 函数说明: 设置蜂鸣器触发时间戳
** 作者: xiaoxiao
** 日期：2023-10-06 18:41:46
** 参数说明:
** 注意事项：
************************************************************/
bool buzzer_call_timestamp_set(unsigned long long timestamp);
/************************************************************
** 函数说明: 设置蜂鸣器触发时长
** 作者: xiaoxiao
** 日期：2023-10-06 18:41:46
** 参数说明:
** 注意事项：
************************************************************/
bool buzzer_call_duration_set(int duration);
/************************************************************
** 函数说明: 蜂鸣器呼叫触发默认事件
** 作者: xiaoxiao
** 日期：2023-10-07 08:50:51
** 参数说明:
** 注意事项：
************************************************************/
void buzzer_alarm_trigger_default(void);

/************************************************************
** 函数说明: 是否警备触发状态
** 作者: xiaoxiao
** 日期：2023-10-23 08:21:24
** 参数说明:
** 注意事项：
************************************************************/
bool is_alarm_trigger(void);
/************************************************************
** 函数说明: 获取eth0网卡是否正常插入
** 作者: xiaoxiao
** 日期：2023-12-09 15:54:43
** 参数说明:
** 注意事项：
************************************************************/
bool is_eth0_inserted(void);
/************************************************************
** 函数说明:警报记录
** 作者: xiaoxiao
** 日期：2023-12-27 21:19:30
** 参数说明:
** 注意事项：
************************************************************/
bool layout_common_alarm_log(int type, int ch);
/************************************************************
** 函数说明:ip变化后更新数据
** 作者: xiaoxiao
** 日期：2024-03-25 16:42:12
** 参数说明:
** 注意事项：
************************************************************/
bool replace_ip_address(char *string, const char *ip_addr);

/************************************************************
** 函数说明:主线从同步锁设置
** 作者: xiaoxiao
** 日期：2024-04-03 21:13:24
** 参数说明:
** 注意事项：
************************************************************/
void main_sync_lock_set(bool en);
#endif