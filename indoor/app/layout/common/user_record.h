#ifndef _USER_RECORD_H_
#define _USER_RECORD_H_
#include <stdbool.h>
typedef enum
{
    REC_MODE_MANUAL = 0x01,
    REC_MODE_AUTO = 0x02,
    REC_MODE_MOTION = 0x04,
    REC_MODE_ALWAYS = 0X08,
    REC_MODE_ALARM = 0x10,
    REC_MODE_AWAY = 0X20,
    REC_MODE_TUYA_CALL = 0x40,
    REC_MODE_TUYA_ALARM = 0X80,
    REC_MODE_TUYA_MOTION = 0x100,

    REC_MODE_TOTAL = 0XFF
} REC_MODE;
/***
** 日期: 2022-05-19 10:37
** 作者: leo.liu
** 函数作用：抓拍一张照片
** 返回参数说明：
***/
bool record_jpeg_start(REC_MODE mode);

/***
** 日期: 2022-05-19 10:37
** 作者: leo.liu
** 函数作用：获取图片抓拍模式
** 返回参数说明：
***/
int record_jpeg_mode_get(void);
/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 停止录制
 */
bool record_video_stop(void);
/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 视频录制
 */
bool record_video_start(int has_audio, REC_MODE mode);
/************************************************************
** 函数说明: 获取抓拍状态
** 作者: xiaoxiao
** 日期：2023-12-21 18:11:40
** 参数说明:
** 注意事项：
************************************************************/
bool jpeg_record_state_get(void);

/************************************************************
** 函数说明: 设置抓拍状态
** 作者: xiaoxiao
** 日期：2023-12-21 18:11:56
** 参数说明:
** 注意事项：
************************************************************/
bool jpeg_record_state_set(bool state);
#endif