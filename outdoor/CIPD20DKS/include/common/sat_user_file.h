#ifndef _USER_FILE_H_
#define _USER_FILE_H_
#include <stdbool.h>

#define SD_BASE_PATH "/tmp/tf"
#define SD_MEDIA_PATH SD_BASE_PATH "/media/"
#define FLASH_PHOTO_PATH "/appconfigs/run/photo/"

#define VIDEO_DOT ".AVI"
#define PHOTO_DOT ".JPG"

typedef enum
{
	FILE_TYPE_PHOTO,
	FILE_TYPE_VIDEO,
	FILE_TYPE_FLASH_PHOTO,
	FILE_TYPE_NONE
} file_type;

typedef struct
{
	char file_name[23];
	char mode;
	char ch;
	bool is_new;
	file_type type;
} file_info;

/***
** 日期: 2022-05-17 10:40
** 作者: leo.liu
** 函数作用：媒体文件初始化
** 返回参数说明：
***/
void media_file_list_init(void);
/***
** 日期: 2022-05-17 11:26
** 作者: leo.liu
** 函数作用：判断sd插入
** 返回参数说明：
***/
bool media_sdcard_insert_check(void);
/***
** 日期: 2022-05-17 14:40
** 作者: leo.liu
** 函数作用：判断sd文件是否已经满
** 返回参数说明：
***/
bool media_sdcard_full_check(void);
/***
** 日期: 2022-05-17 11:28
** 作者: leo.liu
** 函数作用：创建一个文件
** 返回参数说明：
***/
bool media_file_create(file_type type, char ch, char mode, char *file_path);
/***
** 日期: 2022-05-17 13:36
** 作者: leo.liu
** 函数作用：文件总数获取
** 返回参数说明：
***/
bool media_file_total_get(file_type type, int *total, int *new_total);
/***
** 日期: 2022-05-17 13:44
** 作者: leo.liu
** 函数作用：清除新文件标志
** 返回参数说明：
***/
bool media_file_new_clear(file_type type, int index);
/***
** 日期: 2022-05-17 13:53
** 作者: leo.liu
** 函数作用：删除一个文件
** 返回参数说明：
***/
bool media_file_delete(file_type type, int index);
/***
** 日期: 2022-05-17 13:54
** 作者: leo.liu
** 函数作用：判断该文件是否存在
** 返回参数说明：
***/
bool media_file_bad_check(const char *file);
/***
**   日期:2022-05-23 09:51:21
**   作者: leo.liu
**   函数作用：获取格式化sd卡状态
**   参数说明:
***/
bool media_format_sd_state(void);
/***
**   日期:2022-05-23 09:58:49
**   作者: leo.liu
**   函数作用：格式话sd卡
**   参数说明:
***/
void media_format_sd(void);
/***
**   日期:2022-05-23 14:34:43
**   作者: leo.liu
**   函数作用：获取文件信息
**   参数说明:
***/
const file_info *media_file_info_get(file_type type, int index);
/***
**   日期:2022-06-15 11:14:00
**   作者: leo.liu
**   函数作用：删除所有文件
**   参数说明:
 all,true:删除所有文件包括视频和照片
     false:根据type删除指定的文件照片或这视频
***/
bool media_file_delete_all(file_type type, bool all);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-5 17:11:35
** 说明: 查看sd卡信息 
***********************************************/
bool media_sd_memory_query(unsigned int * p_total, unsigned int * p_user, unsigned int * p_free);
#endif