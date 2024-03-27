#ifndef _AUTY_API_H_
#define _AUTY_API_H_
#include <stdbool.h>
#include "common/sat_user_time.h"

#define TUYA_MEDIA_RECORD_PATH "/tmp/tf/tuya_media/"
/***
**   日期:2022-06-07 10:12:44
**   作者: leo.liu
**   函数作用：tuya api初始化
**   参数说明:
***/
bool tuya_api_init(const char *pid, const char *dev);
/***
**   日期:2022-06-07 15:52:37
**   作者: leo.liu
**   函数作用：获取tuya在线状态
**   参数说明:
***/
bool tuya_api_online_check(void);
/***
**   日期:2022-06-07 15:51:54
**   作者: leo.liu
**   函数作用：app退出监控
**   参数说明: 1(字符串类型):接收通话
**	   设备端上报到app:设备端已接受通话。
***/
bool tuya_api_preview_quit(void);
/***
**   日期:2022-06-07 15:58:20
**   作者: leo.liu
**   函数作用：app与
**   参数说明:2(字符串类型):挂断通话
**		设备端上报到app:设备端已挂断通话。
***/
bool tuya_api_monitor_handup(void);
/***
**   日期:2022-06-07 16:10:44
**   作者: leo.liu
**   函数作用：强制时间同步
**   参数说明:
***/
bool tuya_api_app_sync_utc_time(void);
/***
**   日期:2022-06-07 18:15:39
**   作者: leo.liu
**   函数作用：上传有效通道
**   参数说明:channe:当前监控的通道;media_type:0\音视频,1\【视频,2\音频；lock_num锁数量
***/
bool tuya_api_channel_report(int cur, int media_type, int lock_num, bool door1_valid, const char *door1_str,
                             bool door2_valid, const char *door2_str,
                             bool door3_valid, const char *door3_str,
                             bool door4_valid, const char *door4_str,
                             bool door5_valid, const char *door5_str,
                             bool door6_valid, const char *door6_str,
                             bool cctv1_valid, const char *cctv1_str,
                             bool cctv2_valid, const char *cctv2_str,
                             bool cctv3_valid, const char *cctv3_str,
                             bool cctv4_valid, const char *cctv4_str,
                             bool cctv5_valid, const char *cctv5_str,
                             bool cctv6_valid, const char *cctv6_str,
                             bool lobby_valid, const char *lobby_str,
                             bool guard_valid, const char *guard_str);

/***
**   日期:2022-06-07 19:30:28
**   作者: leo.liu
**   函数作用：通道切换处理
**   参数说明:
***/
/************************************************************************************************************
下发指令的格式为：
{"cmd":1,"cc":1,"chs":[{"id":1,"n":"door1"},{"id":2,"n":"door2"},{"id":3,"n":"CCTV1"},{"id":4,"n":"CCTV2"}]}

反馈指令的格式为:
"{\\\"res\\\":1,\\\"err\\\":0,\\\"cc\\\":1,\\\"chs\\\":[{\\\"id\\\":1,\\\"n\\\":\\\"door1\\\"},{\\\"id\\\":2,\\\"n\\\":\\\"door2\\\"},{\\\"id\\\":3,\\\"n\\\":\\\"CCTV1\\\"},{\\\"id\\\":4,\\\"n\\\":\\\"CCTV2\\\"}]}\"}"
************************************************************************************************************/
void tuay_channel_channge_func(int channel);
/***
**   日期:2022-06-08 08:26:18
**   作者: leo.liu
**   函数作用：使tuya重新连接
**   参数说明:
***/
bool tuya_api_reconnect_network(void);
/***
**   日期:2022-06-08 08:36:35
**   作者: leo.liu
**   函数作用：上传门2的开锁模式
**   参数说明:  "1"/"2"
***/
bool tuya_api_door2_unlock_mode_report(int mode);
/***
**   日期:2022-06-08 08:50:31
**   作者: leo.liu
**   函数作用：开锁成功失败反馈
**   参数说明:
***/
bool tuya_api_open_door_success_report(bool ok);
/***
**   日期:2022-06-08 09:32:50
**   作者: leo.liu
**   函数作用：获取app连接设备的个数
**   参数说明:
***/
int tuya_api_client_num(void);
/***
**   日期:2022-06-08 09:34:33
**   作者: leo.liu
**   函数作用：获取tuya app是否在通话
**   参数说明:
***/
bool tuya_api_app_talk_check(void);
/***
**   日期:2022-06-08 11:16:24
**   作者: leo.liu
**   函数作用：上传call机照片
**   参数说明:
***/
bool tuya_api_call_event(int channel, const char *jpg, int size);
/***
**   日期:2022-06-08 16:31:58
**   作者: leo.liu
**   函数作用：上传警报事件
**   参数说明:
***/
bool tuya_api_alarm_event(int channel, const char *jpg, int size);
/***
**   日期:2022-06-09 18:22:31
**   作者: leo.liu
**   函数作用：移动侦测事件触发
**   参数说明:
***/
bool tuya_api_motion_event(int channel, const char *jpg, int size);
/***
**   日期:2022-06-08 15:34:00
**   作者: leo.liu
**   函数作用：tuya 循环buffer清除
**   参数说明:
***/
bool tuya_api_ringbuffer_clear(void);
/***
**   日期:2022-06-15 17:26:18
**   作者: leo.liu
**   函数作用：重置tuya data
**   参数说明:
***/
void tuay_api_data_reset(void);
/***
**   日期:2022-06-17 17:46:52
**   作者: leo.liu
**   函数作用：使能tuya 网络检测功能
**   参数说明:
***/
bool tuya_api_network_detect_enable(bool en);
/*
 * 这个只是用于wifi的状态获取,ping www.tuya.com
 *
 *  0x00:网络没连接
 *   0x01:网络连接热点，但是不能访问外网。
 *   0x02:网络通畅
 */
char tuya_api_network_status(void);
/***
**   日期:2022-06-18 08:05:12
**   作者: leo.liu
**   函数作用：同步tuya时间
**   参数说明:
***/
bool tuya_api_time_sync(void);
/***
**   日期:2022-06-18 08:05:12
**   作者: leo.liu
**   函数作用：获取网卡设备
**   参数说明:
***/
char *tuya_network_dev_get(void);
/***
**   日期:2022-06-18 08:05:12
**   作者: leo.liu
**   函数作用：设置网卡设备
**   参数说明:
***/
bool tuya_network_dev_set(char *dev);
/***
**	 日期:2022-06-07 15:58:20
**	 作者: leo.liu
**	 函数作用：通话状态复位i
**	 参数说明:2(字符串类型):
**		设备端上报到app:
***/
bool tuya_api_monitor_talk_status_reset(void);
/***
**	 日期:2022-06-09 18:22:31
**	 作者: leo.liu
**	 函数作用：涂鸦事件推送测试用例
**	 参数说明:
***/
bool tuya_api_test(int channel, const char *jpg, int size);

#endif