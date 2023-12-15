

#ifndef _SAT_USER_COMMON_H_
#define _SAT_USER_COMMON_H_
#include <netinet/in.h>

#include "ssd20x_common.h"
#define SAT_DEBUG(x...)                                        \
        printf("\n\033[41;37;1m [%s:%d]", __func__, __LINE__); \
        printf(x);                                             \
        printf("\033[0m\n");
#define XIAOXIAO_DEBUG(x...)                                   \
        printf("\n\033[42;37;1m [%s:%d]", __func__, __LINE__); \
        printf(x);                                             \
        printf("\033[0m\n");
#define NETWORK_ETH_NAME "eth0"

typedef struct
{
        char name[64];
        char ip[32];
        unsigned long long timestamp;
} asterisk_register_info;

/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: 获取线程栈内存大小
 * @return:
 */
pthread_attr_t *sat_pthread_attr_get(void);

/****************************************************************
**@日期: 2022-09-28
**@作者: leo.liu
**@功能:获取文件大小
*****************************************************************/
size_t sat_file_size_get(const char *path);

/****************************************************************
**@日期: 2022-09-28
**@作者: leo.liu
**@功能:读取文件
**@ data
*****************************************************************/
size_t sat_file_read(const char *path, char *data, size_t size);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-21 10:29:2
** 说明: 读取本机的UUID
***********************************************/
char *platform_read_device_uuid(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 10:45:38
** 说明: 获取linphone sip账号
***********************************************/
bool sat_sip_local_user_get(char *user);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 10:45:38
** 说明: 获取指定door camera 的 sip账号
local 010001001011@192.168.31.25
***********************************************/
bool sat_sip_local_doorcamera_number_get(int camera_index, char *dst_number);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 10:45:38
** 说明: 获取指定室内分机 的 sip账号
local 010-001-001-011@192.168.31.25
***********************************************/
bool sat_sip_local_indoor_number_get(int id, char *dst_number);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 9:49:0
** 说明: 获取网卡的Ip
***********************************************/
bool sat_ip_mac_addres_get(const char *eth, char *ip, char *mac, char *mask);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 11:25:54
** 说明: udhchc 获取IP
***********************************************/
void sat_network_udhcpc_ip(const char *eth);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 11:26:51
** 说明: 写入mac
***********************************************/
bool sat_network_eth0_mac_write(const char *mac);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-26 11:26:51
** 说明: 写入mac
***********************************************/
bool sat_network_eth1_mac_write(const char *mac);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-28 10:6:44
** 说明: 开启和关闭网络
***********************************************/
void sat_network_enable(const char *eth, bool en);

/****************************************************************
**@日期: 2022-09-16
**@作者: leo.liu
**@功能:关闭
*****************************************************************/
bool sat_socket_close(int socket_fd);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 16:15:12
** 说明: 创建套接字，并且接入组播ip
***********************************************/
bool sat_socket_multicast_join(int socket_fd, const char *multicast_ip);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-6 8:37:33
** 说明: tcp server open
***********************************************/
bool sat_socket_tcp_open(int *fd, int port, int max_client);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-6 9:8:19
** 说明: server fd 同意连接
***********************************************/
int sat_socket_tcp_accept(int server_fd, struct sockaddr_in *client_addr, unsigned int timeout_ms);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 10:1:24
** 说明: 连接服务器
***********************************************/
bool sat_socket_tcp_connect(int socket_fd, const char *server_ip, int port, unsigned int wait_ms);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-6 9:8:19
** 说明: server fd 接收数据
***********************************************/
int sat_socket_tcp_receive(int client_fd, unsigned char *data, int data_len, int timeout_ms);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 9:46:26
** 说明: tcp 发送数据
***********************************************/
bool sat_socket_tcp_send(int socket_fd, unsigned char *data, int data_len, unsigned int timeout_ms);

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-9 13:39:14
** 说明: UDP打开套接字
***********************************************/
bool sat_socket_udp_open(int *socket_fd, int port, bool broadcast);

/****************************************************************
**@日期: 2022-09-16
**@作者: leo.liu
**@功能:发送探测消息
*****************************************************************/
bool sat_socket_udp_send(int socket_fd, const char *data, size_t data_len, const char *ip, int port, int timeout_ms);

/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能:接受一个消息
*****************************************************************/
int sat_socket_udp_receive(int socket_fd, char *data, size_t data_len, struct sockaddr_in *client_addr, int timeout_ms);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 14:46:50
** 说明: 发现IP是否存在
***********************************************/
bool sat_network_ip_pings_check(const char *ip, int pings_count, int one_wait_sec);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 14:46:50
** 说明: 杀死指定的进程
***********************************************/
bool sat_kill_task_process(const char *process_name);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 14:46:50
** 说明: 将子网掩码转换成长度
***********************************************/
int convert_subnet_mask(char *mask);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 14:46:50
** 说明: 获取默认网关
***********************************************/
bool sat_default_gateway_get(const char *eth, char *_gateway, int len);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-7 14:46:50
** 说明: 获取默认dns
***********************************************/
bool sat_default_dns_get(const char *eth, char *dns, int len);
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 15:21:6
** 说明: 通过连接asterisk 获取注册设备信息
***********************************************/
#define ASTERISK_REIGSTER_DEVICE_MAX (20)
asterisk_register_info *asterisk_register_info_get(void);

#endif
