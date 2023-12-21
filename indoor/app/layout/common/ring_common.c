#include "ring_common.h"
#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "common/sat_linphone_event.h"
#include "common/user_data.h"

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 触摸音效播放
***********************************************/
bool ring_touch_play(void)
{
        if (user_data_get()->audio.touch_notification_volume == 0)
        {
                return false;
        }
        sat_linphone_audio_play_start(RESOURCE_RING_PATH "keysound/touch.mp3", 1); // touch.wav
        sat_linphone_audio_play_volume_set(user_data_get()->audio.touch_notification_volume);
        return true;
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 播放门口机的音效
***********************************************/
bool ring_door_call_play(int index, int count)
{
        if (user_data_get()->audio.entrance_volume == 0)
        {
                return false;
        }
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "door_camera/call_door%d.mp3", index);
        sat_linphone_audio_play_start(cmd, count);
        sat_linphone_audio_play_volume_set(user_data_get()->audio.entrance_volume);
        return true;
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 开锁铃声
***********************************************/
bool ring_unlock_play(void)
{
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "open/open.mp3");
        sat_linphone_audio_play_start(cmd, 1);

        sat_linphone_audio_play_volume_set(100);
        return true;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明:intercom 铃声
***********************************************/
bool ring_intercom_play(int index, int count)
{
        if (user_data_get()->audio.extension_volume == 0)
        {
                return false;
        }
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "extension/extension_call_%d.mp3", index);
        printf("cmd is %s\n", cmd);
        sat_linphone_audio_play_start(cmd, count);
        sat_linphone_audio_play_volume_set(user_data_get()->audio.extension_volume);
        return true;
}

/************************************************************
** 函数说明: 警报ling
** 作者: xiaoxiao
** 日期: 2023-05-16 11:43:00
** 参数说明:
** 注意事项:
************************************************************/
bool ring_alarm_play(void)
{
        return true;
        sat_linphone_audio_play_start(RESOURCE_RING_PATH "alarm/alarm.mp3", 1);
        sat_linphone_audio_play_volume_set(100);
        return true;
}

/************************************************************
** 函数说明: 播放蜂鸣器报警
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_buzzer_play(int index)
{
        if (user_data_get()->audio.buzzer_volume == 0)
        {
                return false;
        }
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "buzzer/buzzer_%d.mp3", index);
        sat_linphone_audio_play_start(cmd, 1);

        sat_linphone_audio_play_volume_set(user_data_get()->audio.buzzer_volume);
        return true;
}

/************************************************************
** 函数说明: 播放公共入口铃声
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_common_door_play(int index, int count)
{
        if (user_data_get()->audio.common_entrance_volume == 0)
        {
                return false;
        }
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "common_entrance/Common_entrance_%d.mp3", index);
        sat_linphone_audio_play_start(cmd, count);

        sat_linphone_audio_play_volume_set(user_data_get()->audio.common_entrance_volume);
        return true;
}

/************************************************************
** 函数说明: 播放警卫室call机铃声
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_guard_play(int index)
{
        if (user_data_get()->audio.guard_station_volume == 0)
        {
                return false;
        }
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "security_office/security_office_%d.mp3", index);
        sat_linphone_audio_play_start(cmd, 1);

        sat_linphone_audio_play_volume_set(user_data_get()->audio.guard_station_volume);
        return true;
}

/************************************************************
** 函数说明: 播放繁忙音
** 作者: xiaoxiao
** 日期: 2023-07-05 11:19:41
** 参数说明:
** 注意事项:
************************************************************/
bool ring_busy_play(int index)
{
        return false;
}

/************************************************************
** 函数说明:播放发送音
** 作者: xiaoxiao
** 日期：2023-11-21 10:09:43
** 参数说明:
** 注意事项：
************************************************************/
bool send_call_play(int index, int count)
{
        if (user_data_get()->audio.guard_station_volume == 0)
        {
                return false;
        }
        char cmd[128] = {0};
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, RESOURCE_RING_PATH "send_call/req_call.mp3");
        sat_linphone_audio_play_start(cmd, count);

        sat_linphone_audio_play_volume_set(100);
        return false;
}
