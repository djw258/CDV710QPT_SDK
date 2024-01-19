#ifndef _ONVIF_H_
#define _ONVIF_H_
#include <stdbool.h>
#include <stdlib.h>
#define XML_FILE_MAX (512 * 1024)
/****************************************************************
**@日期: 2022-09-16
**@作者: leo.liu
**@功能: 设备发现
**@参数：
*****************************************************************/
bool ipc_camera_search(char ipc_addr[8][32], char device_name[8][32], int *num, bool door_camera);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取media profile token
*****************************************************************/
bool ipc_profile_token_get(const char *ip, int port, const char *user, const char *password, char profile[8][64], int *profile_num);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能:获取设备的rtsp
*****************************************************************/
bool ipc_camera_rtsp_get(const char *ip, int port, const char *user, const char *password, const char *profile_token, char *rstp, char *sip, int len);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能:获取设备的rtsp
*****************************************************************/
bool ipc_camera_device_register(const char *loc_sip_uri, const char *ipaddr, int port, const char *username, const char *password, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 删除
*****************************************************************/
bool ipc_camera_device_delete(const char *loc_sip_uri, const char *ip, int port, const char *user, const char *password, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 查询是否在线
*****************************************************************/
bool ipc_camera_device_name_get(char *name, const char *ip, int port, const char *user, const char *password, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 设置用户名
*****************************************************************/
bool ipc_camera_device_name_set(const char *name, const char *ip, int port, const char *user, const char *password, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 密码修改
*****************************************************************/
bool ipc_camera_device_password_change(const char *pwd, const char *ip, int port, const char *user, const char *password, int timeout);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: ip变化上报
*****************************************************************/
bool ipc_camera_device_ip_change(char *client_ip, const char *ip, int port, const char *user, const char *password, int timeout);
#endif