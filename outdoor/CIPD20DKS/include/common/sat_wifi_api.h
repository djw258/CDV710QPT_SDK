#ifndef __SAT_WIFI_API_H_
#define __SAT_WIFI_API_H_
#include <stdbool.h>
typedef struct
{
	char name[128];
	int db;
	char psk_flags[64];
	bool free;
} wifi_info;

/***
**   日期:2022-06-02 15:55:31
**   作者: leo.liu
**   函数作用：关闭wifi
**   参数说明:
***/
bool wifi_device_close(void);
/***
**   日期:2022-06-02 15:58:44
**   作者: leo.liu
**   函数作用：开启wifi
**   参数说明:
***/
bool wifi_device_open(void);
/***
**   日期:2022-06-02 16:00:02
**   作者: leo.liu
**   函数作用：连接缓存文件
**   参数说明:
***/
bool wifi_device_temp_conneting(void);
/***
**   日期:2022-06-02 16:15:14
**   作者: leo.liu
**   函数作用：连接wifi
**   参数说明:
***/
bool wifi_device_conneting(void);
/***
**   日期:2022-06-02 16:03:37
**   作者: leo.liu
**   函数作用：获取wifi连接状态
**   参数说明:
***/
bool wifi_device_connection_stauts(unsigned char *user, bool *free, unsigned char *ip, bool *conneted, unsigned char *flags);
/***
**   日期:2022-06-02 16:08:34
**   作者: leo.liu
**   函数作用：将密码和用户保存到缓存
**   参数说明:
***/
bool wifi_device_user_to_temp(unsigned char *user, unsigned char *pwd);
/***
**   日期:2022-06-02 16:12:47
**   作者: leo.liu
**   函数作用：同步wifi信息
**   参数说明:
***/
bool wifi_device_tmp_sync(void);
/***
**   日期:2022-06-02 16:18:52
**   作者: leo.liu
**   函数作用：扫描wifi
**   参数说明:
***/
bool wifi_device_scanf(void);
/***
**   日期:2022-06-02 16:50:15
**   作者: leo.liu
**   函数作用：获取扫描的结果
**   参数说明:
***/
bool wifi_device_scanf_info_get(wifi_info **info, int *total);
/***
**   日期:2022-06-02 16:59:55
**   作者: leo.liu
**   函数作用：获取当前wifi的物理地址
**   参数说明:hwadders:必须大于64个字节
***/
bool wifi_device_hwadders_get(unsigned char *hwadders);
/***
**   日期:2022-06-08 17:42:41
**   作者: leo.liu
**   函数作用：判断wpa_supplicant线程存在
**   参数说明:
***/
bool wpa_supplicant_pid_exist_check(void);
/***
**   日期:2022-07-16 08:22:48
**   作者: leo.liu
**   函数作用：重置wifi配置
**   参数说明:
***/
bool wifi_api_reset_default(void);
#endif