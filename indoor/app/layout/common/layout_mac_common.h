
#ifndef _LAYOUT_MAC_COMMON_H_
#define _LAYOUT_MAC_COMMON_H_
#define MAC_ADDRESS_CONF_PATH "/app/tuya/mac/"
#define APPCONFIG_MAC_ADDRESS_CONF_PATH "/appconfigs/eth0_mac"
#define INDOOR_MAC_TXT_PATH "/tmp/tf/indoor_mac/"
#define OUTDOOR_MAC_TXT_PATH "/tmp/tf/outdoor_mac/"
/***
**   日期:2022-05-27 16:35:28
**   作者: leo.liu
**   函数作用：保存指定的mac地址
**   参数说明:
***/
bool local_mac_address_set(int index);
/************************************************************
** 函数说明: 根据索引号生成mac地址
** 作者: xiaoxiao
** 日期：2023-10-23 08:21:24
** 参数说明:
** 注意事项：
************************************************************/
bool outdoor_mac_address_get(int index, char *mac_address);
/************************************************************
** 函数说明: 获取本地mac
** 作者: xiaoxiao
** 日期：2023-10-23 08:21:24
** 参数说明:
** 注意事项：
************************************************************/
bool local_mac_address_get(char *mac_address);
/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期：2023-12-05 11:59:56
** 参数说明:
** 注意事项：
************************************************************/
bool indoor_mac_init(int index);

bool outdoor_mac_txt_exist_check(void);
#endif