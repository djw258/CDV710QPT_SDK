
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "user_record.h"
#include "user_monitor.h"
#include "common/sat_user_file.h"
#include "common/sat_linphone_event.h"
#include "tuya/tuya_api.h"
#include "string.h"
#include "pthread.h"
#include "common/sat_user_common.h"

typedef struct user_record_info
{
        int record_mode;
        char *data;
        int size;
} user_record_info;

static void *tuya_event_report(void *arg)
{
        user_record_info *info = (user_record_info *)arg;
        if (info->record_mode & REC_MODE_TUYA_CALL)
        {
                tuya_api_call_event(monitor_channel_get() + 1, (const char *)info->data, info->size);
        }
        if (info->record_mode & REC_MODE_TUYA_ALARM)
        {
                tuya_api_alarm_event(monitor_channel_get() + 1, (const char *)info->data, info->size);
        }
        if (info->record_mode & REC_MODE_TUYA_MOTION)
        {
                tuya_api_motion_event(monitor_channel_get() + 1, (const char *)info->data, info->size);
        }
        if (info->data != NULL)
        {
                free((char *)info->data);
                info->data = NULL;
        }
        return NULL;
}

static bool jpeg_record_is_success = false;
/************************************************************
** 函数说明: 获取抓拍状态
** 作者: xiaoxiao
** 日期：2023-12-21 18:11:40
** 参数说明:
** 注意事项：
************************************************************/
bool jpeg_record_state_get(void)
{
        return jpeg_record_is_success;
}
/************************************************************
** 函数说明: 设置抓拍状态
** 作者: xiaoxiao
** 日期：2023-12-21 18:11:56
** 参数说明:
** 注意事项：
************************************************************/
bool jpeg_record_state_set(bool state)
{
        jpeg_record_is_success = state;
        return true;
}
/***
 *
 ** 日期: 2022-05-19 10:37
 ** 作者: leo.liu
 ** 函数作用：接收编码后的流
 ** 返回参数说明：
 ***/
static int jpeg_record_mode = REC_MODE_MANUAL;

static bool jpeg_write_callback(unsigned char *data, int size, int ch, int mode)
{
        jpeg_record_is_success = true;
        if ((mode & 0x3F))
        {
                file_type type = FILE_TYPE_FLASH_PHOTO;
                if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
                {
                        type = FILE_TYPE_PHOTO;
                }
                char file_path[128] = {0};

                char name[24] = {0};
                if (is_channel_ipc_camera(ch) == true)
                {
                        sprintf(name, "%s", network_data_get()->cctv_device[ch - 8].door_name);
                }
                else if (ch == MON_CH_LOBBY)
                {
                        sprintf(name, "%s", "Lobby");
                }
                else if (ch == MON_CH_GUARD)
                {
                        sprintf(name, "%s", "Guard");
                }
                else
                {
                        sprintf(name, "%s", network_data_get()->door_device[ch].door_name);
                }
                media_file_create(type, name, mode & 0x3F, file_path);
                int fd = open(file_path, O_CREAT | O_WRONLY);
                if (fd >= 0)
                {
                        printf("\n\n snap jpg:%s %dkb \n\n", file_path, size / 1024);
                        write(fd, data, size);
                        close(fd);
                }
                else
                {
                        printf("\n\n snap jpg: %s open failed \n\n", file_path);
                }
                media_file_bad_check(file_path);
        }

        static user_record_info info;
        if ((mode & REC_MODE_TUYA_CALL) || (mode & REC_MODE_TUYA_ALARM) || (mode & REC_MODE_TUYA_MOTION))
        {
                info.record_mode = mode;
                if (info.data != NULL)
                {
                        free((char *)info.data);
                        info.data = NULL;
                }

                info.data = (char *)malloc(size);

                memcpy(info.data, data, size);
                info.size = size;

                pthread_t task_id;
                pthread_create(&task_id, sat_pthread_attr_get(), tuya_event_report, (void *)&info);
                pthread_detach(task_id);
        }

        system("sync");
        return true;
}
/***
** 日期: 2022-05-19 10:37
** 作者: leo.liu
** 函数作用：抓拍一张照片
** 返回参数说明：
***/
bool record_jpeg_start(REC_MODE mode)
{
        jpeg_record_is_success = false;
        jpeg_record_mode = mode;
        return sat_linphone_snap(monitor_channel_get(), jpeg_record_mode, jpeg_write_callback);
}

/***
** 日期: 2022-05-19 10:37
** 作者: leo.liu
** 函数作用：获取图片抓拍模式
** 返回参数说明：
***/
int record_jpeg_mode_get(void)
{
        return jpeg_record_mode;
}

/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 视频录制完成的回调函数
 */
static bool record_video_callback(const char *path, int ch, int mode)
{
        if ((media_sdcard_insert_check() == SD_STATE_UNPLUG) || (media_sdcard_insert_check() == SD_STATE_ERROR))
        {
                return false;
        }
        char file_path[64] = {0};
        char name[24] = {0};
        if (is_channel_ipc_camera(ch) == true)
        {
                sprintf(name, "%s", network_data_get()->cctv_device[ch - 8].door_name);
        }
        else if (ch == MON_CH_LOBBY)
        {
                sprintf(name, "%s", "Lobby");
        }
        else if (ch == MON_CH_GUARD)
        {
                sprintf(name, "%s", "Guard");
        }
        else
        {
                sprintf(name, "%s", network_data_get()->door_device[ch].door_name);
        }
        media_file_create(FILE_TYPE_VIDEO, name, mode, file_path);

        char cmd[128] = {0};
        sprintf(cmd, "mv %s %s", path, file_path);
        system(cmd);
        media_file_bad_check(file_path);

        return true;
}
/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 视频录制
 */
bool record_video_start(int has_audio, REC_MODE mode)
{
        return sat_linphone_start_record_video(has_audio, monitor_channel_get(), mode, record_video_callback);
}
/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 停止录制
 */
bool record_video_stop(void)
{
        return sat_linphone_stop_record_video();
}
