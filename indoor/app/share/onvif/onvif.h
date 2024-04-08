#ifndef _ONVIF_H_
#define _ONVIF_H_
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "common/sat_ipcamera.h"
#define XML_FILE_MAX (128 * 1024)
/****************************************************************
**@日期: 2022-09-16
**@作者: leo.liu
**@功能: 设备发现
**@参数：
*****************************************************************/
bool ipc_camera_search(char ipc_addr[8][32], char device_name[8][32], int *num, char device_type);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取media profile token
*****************************************************************/
bool ipc_profile_token_get(const char *ip, int port, const char *user, const char *password, char profile[8][64], int *profile_num, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取media profile digest
*****************************************************************/
bool ipc_profile_digest_get(const char *ip, int port, const char *user, const char *password, char profile[8][64], int *profile_num, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能:获取设备的rtsp
*****************************************************************/
bool ipc_rtsp_token_get(const char *ip, int port, const char *user, const char *password, char *profile_token, char *rstp, char *sip, int len, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 查询是否在线
*****************************************************************/
bool ipc_camera_device_name_get(char *name, const char *ip, int port, const char *user, const char *password, char auther_flag, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 设置用户名
*****************************************************************/
bool ipc_camera_device_name_set(char *name, const char *ip, int port, const char *user, const char *password, char auther_flag, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 密码修改
*****************************************************************/
bool ipc_camera_device_password_change(char *pwd, const char *ip, int port, const char *user, const char *password, char auther_flag, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 数据同步
*****************************************************************/
bool ipc_camera_device_send_data(char *data_type, char *data, const char *ip, int port, const char *user, const char *password, int timeout);

/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: onvfi发送数据
*****************************************************************/
bool ipc_onvif_tcp_send(char *data, int size, const char *ip, int port, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能:获取设备的rtsp 来自摘要
*****************************************************************/
bool ipc_rtsp_digest_get(const char *ip, int port, const char *user, const char *password, char *profile_token, char *rtsp, char *sip, int len, int timeout);
/****************************************************************
**@日期: 2023-09-19
**@作者: leo.liu
**@功能: 获取亮度色度对比度数据,百分比
*****************************************************************/
bool ipc_camera_device_get_imaging(const char *ip, int port, const char *user, const char *password, char auther_flag, int brightness[3], int saturation[3], int contrast[3], int timeout);
/****************************************************************
**@日期: 2023-09-19
**@作者: leo.liu
**@功能: 设置亮度色度对比度数据,百分比
*****************************************************************/
bool ipc_camera_device_setting_imaging(const char *ip, int port, const char *user, const char *password, char auther_flag, int brightness, int saturation, int contrast, int timeout);
/****************************************************************
**@日期: 2023-09-19
**@作者: leo.liu
**@功能: 获取设备的网络信息
*****************************************************************/
bool ipc_camera_device_get_network(const char *ip, int port, const char *user, const char *password, char auther_flag, struct ipcamera_network *info, int timeout);
/****************************************************************
**@日期: 2023-09-21
**@作者: leo.liu
**@功能: 设置网络信息
*****************************************************************/
bool ipc_camera_device_setting_network(const char *ip, int port, const char *user, const char *password, char auther_flag, struct ipcamera_network *info, int timeout);
/****************************************************************
**@日期: 2023-09-21
**@作者: leo.liu
**@功能: 获取对方时间
*****************************************************************/
bool ipc_camera_device_get_system_time(const char *ip, int port, const char *user, const char *password, char auther_flag, struct tm *tm, int timeout);
#endif