#ifndef _SAT_IPCAMERA_H_
#define _SAT_IPCAMERA_H_
#include <stdbool.h>

#define IPCAMERA_NUM_MAX 8
#define IPCAMERA_PROFILE_MAX 4
typedef struct
{
        char profile_token[32];
        char rtsp_url[128];
} ipcamera_rtsp_info;
struct ipcamera_info
{
        char username[32];
        char password[32];
        char ipaddr[24];
        /*注意此处：因为判断sizeof长度使sip，与rtsp一致，故
        rtsp url与 sip url的数组长度要一致，否则会出现越界处理*/
        char sip_url[128];
        char door_name[64];
        int port;
        ipcamera_rtsp_info rtsp[IPCAMERA_PROFILE_MAX];
        int profile_token_num;
};
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取onvif所有的rtsp流
*****************************************************************/
bool sat_ipcamera_rtsp_url_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:搜索onvif所有的rtsp流
0:IPC
1:DOOR CAMERA
2:IN DOOR
*****************************************************************/
bool sat_ipcamera_device_discover_search(char doorcamera);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:设置账号和密码
*****************************************************************/
bool sat_ipcamera_user_password_set(int index, const char *username, const char *password);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取在线的IP
*****************************************************************/
int sat_ipcamera_online_num_get(void);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取有效的ipcamera
*****************************************************************/
int sat_ipcamera_valid_num_get(void);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取ip
*****************************************************************/
const char *sat_ipcamera_ipaddr_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取端口
*****************************************************************/
int sat_ipcamera_port_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取rtsp url
*****************************************************************/
const char *sat_ipcamera_sip_addr_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取账号
*****************************************************************/
const char *sat_ipcamera_username_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取密码
*****************************************************************/
const char *sat_ipcamera_password_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取别名
*****************************************************************/
const char *sat_ipcamera_door_name_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取rtsp url
*****************************************************************/
const char *sat_ipcamera_rtsp_addr_get(int index, int ch);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取token 数目
*****************************************************************/
int sat_ipcamera_profile_token_num_get(int index);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 注册一个设备
*****************************************************************/
bool sat_ipcamera_device_register(char *loc_sip_uri, int index, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 注册一个设备
*****************************************************************/
bool sat_ipcamera_device_delete(char *loc_sip_uri, int index, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:设置ipcamera参数
*****************************************************************/
bool sat_ipcamera_initialization_parameters(struct ipcamera_info *device, int total);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 查询是否在线
*****************************************************************/
bool sat_ipcamera_device_name_get(int index, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 用户名设置
*****************************************************************/
bool sat_ipcamera_device_name_set(char *name, int index, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 修改密码
*****************************************************************/
bool sat_ipcamera_device_password_set(char *new_pwd, int index, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 修改密码
*****************************************************************/
bool sat_ipcamera_device_version_get(char *version, int index, int timeout);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:获取节点信息
*****************************************************************/
struct ipcamera_info *sat_ipcamera_node_data_get(int index);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 15:21:6
** 说明: 数据同步处理.注意：此接口只有ID1处理
** type: 0:user_data,1:network_data
** flag: bit0:1发送到室内分机，bit1:1发送到门口机,bit2:1发送给主机
** data:需要同步的数据
** size:同步数据的大小
** inline_t:最后刷新注册的时间戳到现在的时间差判定是否在线
** timeout：发送超时
** param: 预留参数，如果需要发送给门口机，则需要传送绑定的设备信息
***********************************************/
bool sat_ipcamera_data_sync(char type, char flag, const char *data, int size, int inline_t, int timeout, void *param);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 15:21:6
** 说明: 发送shell cmd 执行命名
** ip: 对象IP，
** port: 对象端口
** user： 对象用户名
** password: 对象密码
** cmd: shell命令
** timeout : 超时时间
***********************************************/
bool sat_ipcamera_report_shellcmd(char *ip, int port, const char *user, const char *password, char *cmd, int timeout);
#endif