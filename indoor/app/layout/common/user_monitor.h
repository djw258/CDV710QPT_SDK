
#ifndef _USER_MONITOR_H_
#define _USER_MONITOR_H_
#include "stdbool.h"
#include "user_network.h"
#define is_channel_ipc_camera(ch) (((ch >= 8 && ch <= 15)) ? 1 : (ch >= 0 && ch <= 7) ? 0 \
                                                                                      : 2)

#define MON_CH_NONE -1
#define MON_CH_DOOR1 0
#define MON_CH_DOOR2 1
#define MON_CH_DOOR3 2
#define MON_CH_DOOR4 3
#define MON_CH_DOOR5 4
#define MON_CH_DOOR6 5
#define MON_CH_DOOR7 6
#define MON_CH_DOOR8 7
#define MON_CH_CCTV1 8
#define MON_CH_CCTV2 9
#define MON_CH_CCTV3 10
#define MON_CH_CCTV4 11
#define MON_CH_CCTV5 12
#define MON_CH_CCTV6 13
#define MON_CH_CCTV7 14
#define MON_CH_CCTV8 15
#define MON_CH_LOBBY 16
#define MON_CH_GUARD 17
typedef enum
{
        MON_ENTER_CALL_FLAG,
        MON_ENTER_TUYA_MANUAL_TALK_FLAG,
        MON_ENTER_TUYA_CALL_TALK_FLAG,
        MON_ENTER_MANUAL_TALK_FLAG,
        MON_ENTER_CALL_TALK_FLAG,
        MON_ENTER_MANUAL_DOOR_FLAG,
        MON_ENTER_MANUAL_CCTV_FLAG
} MON_ENTER_FLAG;
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取监控通道的地址
** 返回参数说明：
***/
const char *monitor_channel_get_url(int index, bool update);
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：依据房号获取通道
** 返回参数说明：
***/
int monitor_index_get_by_user(const char *number);
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取监控通道对应的索引
** 返回参数说明：
***/
int monitor_channel_prev_get(void);
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取下一个通道
** 返回参数说明：
***/
int monitor_channel_next_get(void);
/*获取第一个有效的通道*/
int monitor_door_first_valid_get(bool door_camera);

/*获取最后一个有效通道*/
int monitor_door_last_valid_get(bool door_camera);

// 获取门口机和CCTV的注册状态
bool monitor_door_registered_status_get(void);
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取监控通道
** 返回参数说明：
***/
int monitor_channel_get(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 8:37:5
** 说明: 设置通道
***********************************************/
void monitor_channel_set(int ch);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:43:34
** 说明: 进入监控标志
***********************************************/
void monitor_enter_flag_set(MON_ENTER_FLAG flag);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明:关闭监控
***********************************************/
void monitor_close(char flag);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 开启监控
***********************************************/
void monitor_open(bool refresh, bool rtsp);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 获取进入监控标志
***********************************************/
MON_ENTER_FLAG monitor_enter_flag_get(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 获取门口机信息
** 010.001.001.011
BYTE1. 010(固定部分).
                BYTE2. 001 -> 0x01
                                bit[7]=0:
                                bit[6:0] 栋号

                                bit[7]=1:
                                        分机(门口机/室内分机)
                                        bit[6:5]=00:室内分机
                                        bit[6:5]=10:户外机)

                BYTE3. 001 楼层号
                BYTE4. 001 百分位和十分位 - 房号
                           个位 -BYTE2 bit[7]=0
                                                1:室内机
                                                3:Gateway IP system
                                                7:Lobby IP system
                                                9:Guard Phone IP system
                                                5:KIOSK IP system
                                   -BYTE2 bit[7]=1
                                                个位分机ID号
***********************************************/
#endif
/************************************************************
** 函数说明: 判断通道是否有效
** 作者: xiaoxiao
** 日期: 2023-05-26 08:28:22
** 参数说明:
** 注意事项:
************************************************************/
bool monitor_valid_channel_check(int channel);

/************************************************************
** 函数说明: 门口机注册数量获取
** 作者: xiaoxiao
** 日期: 2023-08-08 14:27:13
** 参数说明:
** 注意事项:
************************************************************/
int door_camera_register_num_get();

/************************************************************
** 函数说明: CCTV注册数量获取
** 作者: xiaoxiao
** 日期: 2023-08-08 14:29:29
** 参数说明:
** 注意事项:
************************************************************/
int cctv_register_num_get();

/************************************************************
** 函数说明: 依据sip消息获取分机号
** 作者: xiaoxiao
** 日期: 2023-08-10 10:51:32
** 参数说明:
** 注意事项:
************************************************************/
int extern_index_get_by_user(const char *user);
/************************************************************
** 函数说明: 门口机在线查询
** 作者: xiaoxiao
** 日期: 2023-08-08 14:29:29
** 参数说明:ch:门口机通道
** 注意事项:
************************************************************/
bool outdoor_online_check(int ch, int *total);
/************************************************************
** 函数说明: 分机在线查询
** 作者: xiaoxiao
** 日期: 2023-08-08 14:29:29
** 参数说明:ch:分机id1-8；total分机在线个数统计
** 注意事项:
************************************************************/
bool extension_online_check(int ch, int *total);