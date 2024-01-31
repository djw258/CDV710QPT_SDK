#include "layout_define.h"
#include "tuya_common.h"
#include "tuya_api.h"
static int tuya_monitor_channel = 0;
void tuya_monitor_channel_set(int ch)
{
        tuya_monitor_channel = ch;
}

int tuya_monitor_channel_get(void)
{
        return tuya_monitor_channel;
}

static bool tuya_app_quit_status_active = false;
bool tuya_app_quit_status_active_get(void)
{
        return tuya_app_quit_status_active;
}

void tuya_app_quit_status_active_set(bool en)
{
        tuya_app_quit_status_active = en;
}

static bool tuya_event_cmd_video_start(void)
{

        if (sat_cur_layout_get() != sat_playout_get(monitor))
        {
                tuya_api_monitor_talk_status_reset();
                if ((tuya_monitor_channel < 8) /* && (network_data_get()->door_device_count > 0) */)
                {
                        monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
                }
                else if ((tuya_monitor_channel >= 8) /* && (network_data_get()->cctv_device_count > 0) */)
                {
                        monitor_enter_flag_set(MON_ENTER_MANUAL_CCTV_FLAG);
                }
                if (monitor_valid_channel_check(tuya_monitor_channel))
                {
                        monitor_channel_set(tuya_monitor_channel);
                        tuya_app_quit_status_active_set(true);
                        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
                }
                else if (monitor_door_first_valid_get(true) != -1)
                {
                        monitor_channel_set(monitor_door_first_valid_get(true));
                        tuya_app_quit_status_active_set(true);
                        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
                }
                else if (monitor_door_first_valid_get(false) != -1)
                {
                        monitor_channel_set(monitor_door_first_valid_get(false));
                        tuya_app_quit_status_active_set(true);
                        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
                }
        }
        else
        {
                tuya_monitor_channel = monitor_channel_get();
        }
        return true;
}

static bool tuya_event_cmd_video_stop(void)
{
        return true;
}

bool tuya_event_defalut_handle(TUYA_CMD cmd, int arg)
{
        if (user_data_get()->is_device_init == false)
        {
                return false;
        }
        switch ((cmd))
        {
        case TUYA_EVENT_CMD_VIDEO_START:
                return tuya_event_cmd_video_start();
                break;
        case TUYA_EVENT_CMD_VIDEO_STOP:
                return tuya_event_cmd_video_stop();
                break;
        case TUYA_EVENT_CMD_AUDIO_START:
                // tuya_api_door2_unlock_mode_report(user_data_get()->etc.door2_lock_num);
                // return truye_event_cmd_audio_start();
                break;
        case TUYA_EVENT_CMD_ONLINE:
                return layout_monitor_report_vaild_channel();
                break;
        case TUYA_EVENT_CMD_CH_CHANGE:
                //     return tuya_event_cmd_ch_channge(arg);
                break;
        case TUYA_EVENT_CMD_MOTION_ENBALE:
                //  return tuya_event_cmd_motion_enable(arg);
                break;
        case TUYA_EVENT_CMD_DOOR_OPEN:
                // return tuya_event_cmd_door_open(arg);
                break;
        default:
                SAT_DEBUG("unknow tuya event:%d", cmd);
                break;
        }
        return false;
}

/************************************************************
** 函数说明: 上传有效通道
** 作者: xiaoxiao
** 日期: 2023-05-30 14:24:54
** 参数说明:
** 注意事项:
************************************************************/
bool layout_monitor_report_vaild_channel(void)
{
        int ch = monitor_channel_get();
        int media_type = 0;
        if ((ch >= 8) && ch <= 13)
        {
                ch = ch - 1;
                media_type = 1;
        }
        else if ((ch == 16) || (ch == 17))
        {
                ch = ch - 3;
                if (ch == 17)
                        media_type = 2;
        }
        else if ((ch < 0) || (ch > 6))
        {
                return false;
        }
        else
        {
                ch = ch + 1;
        }

        return tuya_api_channel_report(ch, media_type, monitor_valid_channel_check(MON_CH_DOOR1), language_common_ch_string_get(TUYA_CH_DOOR1),
                                       monitor_valid_channel_check(MON_CH_DOOR2), language_common_ch_string_get(TUYA_CH_DOOR2),
                                       monitor_valid_channel_check(MON_CH_DOOR3), language_common_ch_string_get(TUYA_CH_DOOR3),
                                       monitor_valid_channel_check(MON_CH_DOOR4), language_common_ch_string_get(TUYA_CH_DOOR4),
                                       monitor_valid_channel_check(MON_CH_DOOR5), language_common_ch_string_get(TUYA_CH_DOOR5),
                                       monitor_valid_channel_check(MON_CH_DOOR6), language_common_ch_string_get(TUYA_CH_DOOR6),
                                       monitor_valid_channel_check(MON_CH_CCTV1), language_common_ch_string_get(TUYA_CH_CCTV1),
                                       monitor_valid_channel_check(MON_CH_CCTV2), language_common_ch_string_get(TUYA_CH_CCTV2),
                                       monitor_valid_channel_check(MON_CH_CCTV3), language_common_ch_string_get(TUYA_CH_CCTV3),
                                       monitor_valid_channel_check(MON_CH_CCTV4), language_common_ch_string_get(TUYA_CH_CCTV4),
                                       monitor_valid_channel_check(MON_CH_CCTV5), language_common_ch_string_get(TUYA_CH_CCTV5),
                                       monitor_valid_channel_check(MON_CH_CCTV6), language_common_ch_string_get(TUYA_CH_CCTV6),
                                       monitor_valid_channel_check(MON_CH_LOBBY), language_common_ch_string_get(TUYA_CH_LOBBY),
                                       monitor_valid_channel_check(MON_CH_GUARD), language_common_ch_string_get(TUYA_CH_GUARD));
}