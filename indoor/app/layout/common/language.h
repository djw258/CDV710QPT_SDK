#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_
#include "user_monitor.h"
typedef enum
{
	LANGUAGE_ID_HANYU = 0,
	LANGUAGE_ID_CHINESE,
	LANGUAGE_ID_ENGLISH,
	LANGUAGE_ID_ELUOSI,
	LANGUAGE_ID_XIBANYA,
	LANGUAGE_ID_YUENAN,
	LANGUAGE_ID_ALABOYU,
	LANGUAGE_ID_TUERQI,
	LANGUAGE_ID_BOLAN,
	LANGUAGE_ID_JIEKE,
	LANGUAGE_ID_TOTAL
} LANGUAGE_ID;

/***
** 日期: 2022-04-26 08:55
** 作者: leo.liu
** 函数作用：设置当前语言
** 返回参数说明：
***/
void language_id_set(LANGUAGE_ID id);

/***
** 日期: 2022-04-26 08:56
** 作者: leo.liu
** 函数作用：获取当前语言ID
** 返回参数说明：
***/
LANGUAGE_ID language_id_get(void);

typedef enum
{
	LANG_COMMON_ID_OFF = 0,
	LANG_COMMON_ID_ON,
	LANG_COMMON_ID_VIDEO,
	LANG_COMMON_ID_PHOTO,
	LANG_COMMON_ID_CANCEL,
	LANG_COMMON_ID_CONFIRM,
	LANG_COMMON_ID_DOOR1,
	LANG_COMMON_ID_DOOR2,
	LANG_COMMON_ID_DOOR3,
	LANG_COMMON_ID_DOOR4,
	LANG_COMMON_ID_DOOR5,
	LANG_COMMON_ID_DOOR6,
	LANG_COMMON_ID_DOOR7,
	LANG_COMMON_ID_DOOR8,
	LANG_COMMON_ID_CCTV1,
	LANG_COMMON_ID_CCTV2,
	LANG_COMMON_ID_CCTV3,
	LANG_COMMON_ID_CCTV4,
	LANG_COMMON_ID_CCTV5,
	LANG_COMMON_ID_CCTV6,
	LANG_COMMON_ID_CCTV7,
	LANG_COMMON_ID_CCTV8,
	LANG_COMMON_ID_LANG,
	LANG_COMMON_ID_TOTAL
} LANG_COMMON_ID;

/***
** 日期: 2022-04-28 13:55
** 作者: leo.liu
** 函数作用：获取公共字符串的字符串资源
** 返回参数说明：
***/
const char *language_common_string_get(int id);

/***
**   日期:2022-05-24 10:04:38
**   作者: leo.liu
**   函数作用：根据通道类型获取字符串
**   参数说明:
***/
const char *language_common_ch_string_get(int ch);

typedef enum
{
	/***** volume 在record中已经有了 *****/
	SETTING_VOLUME_LANG_ID_TOUCH_TONE,
	SETTING_VOLUME_LANG_ID_DOOR1_TONE,
	SETTING_VOLUME_LANG_ID_DOOR2_TONE,
	SETTING_VOLUME_LANG_ID_INTERPHONE_TONE,

	SETTING_VOLUME_LANG_ID_TONE1,
	SETTING_VOLUME_LANG_ID_TONE2,
	SETTING_VOLUME_LANG_ID_TONE3,
	SETTING_VOLUME_LANG_ID_TONE4,
	SETTING_VOLUME_LANG_ID_TONE5,
	SETTING_VOLUME_LANG_ID_TONE6,
	SETTING_VOLUME_LANG_ID_TOTAL
} LAYOUT_SETTING_VOLUME_LANG_ID;

#endif