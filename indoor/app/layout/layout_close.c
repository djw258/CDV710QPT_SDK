#include "layout_define.h"
#include "onvif.h"
#include "tuya/tuya_api.h"
bool frame_display_timeout_check(void);
static void motion_timer_check_task(lv_timer_t *ptimer);
static bool layout_close_motion_dectection_callback(void);

enum
{
    motion_scr_act_obj_id_head_cont,
    motion_scr_act_obj_id_black_bg,
};

static bool is_motion_snapshot_ing = false;
static bool is_motion_record_video_ing = false;
static int motion_timeout_sec = 0;

static lv_timer_t *motion_timerout_timer = NULL;
static unsigned long long motion_detection_timestap = 0;

static lv_timer_t *backlight_timer = NULL;
static void layout_close_click(lv_event_t *ev)
{
    sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void close_cancel_btn_create(void)
{
    lv_obj_add_event_cb(sat_cur_layout_screen_get(), layout_close_click, LV_EVENT_CLICKED, NULL);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 获取顶部容器的子控件
 ***********************************************/
static lv_obj_t *moniton_top_child_obj_get(int id)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), motion_scr_act_obj_id_head_cont);
    if (parent == NULL)
    {
        return NULL;
    }
    return lv_obj_get_child_form_id(parent, id);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 顶部图标显示
 ***********************************************/
static void motion_obj_top_icon_display(void)
{

    lv_obj_t *obj = NULL;
    int pos_x = 863;
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: SD卡显示
     ***********************************************/
    {
        obj = moniton_top_child_obj_get(5);
        if (obj == NULL)
        {
            return;
        }
        if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
        {
            lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(media_sdcard_insert_check() == SD_STATE_INSERT ? "ic_monitoring_sdcard.png" : "ic_monitoring_sdcard_full.png"), LV_PART_MAIN);
            lv_obj_set_x(obj, pos_x);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
            pos_x -= 56;
        }
        else
        {
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
    }

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 自动记录
     ***********************************************/
    {
        obj = moniton_top_child_obj_get(4);
        if (obj == NULL)
        {
            return;
        }
        lv_obj_set_x(obj, pos_x);
        pos_x -= 56;
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->auto_record_mode == 0 ? "ic_monitoring_manual.png" : "ic_monitoring_auto.png"), LV_PART_MAIN);
    }

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 移动侦测
     ***********************************************/
    {
        obj = moniton_top_child_obj_get(3);
        if (obj == NULL)
        {
            return;
        }
        if (user_data_get()->motion.enable)
        {
            lv_obj_set_x(obj, pos_x);
            pos_x -= 56;
            lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->motion.saving_fmt == 1 ? "ic_list_visitor_img.png" : "ic_list_visitor_video.png"), LV_PART_MAIN);
        }
        else
        {
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
    }

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 记录
     ***********************************************/
    {
        obj = moniton_top_child_obj_get(2);
        if (obj == NULL)
        {
            return;
        }
        lv_obj_set_x(obj, pos_x);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

static void layout_motion_sd_state_change_callback(void)
{
    record_video_stop();
    motion_obj_top_icon_display();
}

/************************************************************
** 函数说明: 移动侦测超时检测
** 作者: xiaoxiao
** 日期: 2023-06-05 17:20:02
** 参数说明:
** 注意事项:
************************************************************/
static bool motion_timer_timeout_check(void)
{
    if (user_data_get()->motion.timer_en == false)
    {
        return true;
    }
    struct tm tm;
    struct tm m_start;
    struct tm m_end;
    m_start = user_data_get()->motion.start;
    m_end = user_data_get()->motion.end;
    user_time_read(&tm);
    int cur = user_tm_conver_sec(&tm);
    int start = user_tm_conver_sec(&m_start);
    int end = user_tm_conver_sec(&m_end);
    if (end < start)
    {
        end += 24 * 60 * 60;
    }
    if ((cur > start) && (cur < end))
    {
        return true;
    }
    return false;
}

/************************************************************
** 函数说明: 开启移动侦测监控
** 作者: xiaoxiao
** 日期: 2023-06-05 17:29:24
** 参数说明:
** 注意事项:
************************************************************/
static void layout_motion_monitor_open(void)
{
    monitor_channel_set(user_data_get()->motion.select_camera);
    monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
    monitor_open(true, true);
    int ch = monitor_channel_get();
    if (is_channel_ipc_camera(monitor_channel_get()) == false)
    {
        sat_ipcamera_device_channel_setting(network_data_get()->door_device[ch].ipaddr, network_data_get()->door_device[ch].port, network_data_get()->door_device[ch].username, network_data_get()->door_device[ch].password, network_data_get()->door_device[ch].auther_flag, 0, 1000);
    }
}

/***
**   日期:2022-06-09 11:23:26
**   作者: leo.liu
**   函数作用：重新开启移动侦测
**   参数说明:
***/
static void layout_motion_restart_motion_detection(void)
{
    backlight_enable(false);

    monitor_close(0x02);
    lv_timer_reset(lv_sat_timer_create(motion_timer_check_task, 3000, NULL));
}

/************************************************************
** 函数说明:移动侦测超时处理
** 作者: xiaoxiao
** 日期：2024-01-19 08:40:45
** 参数说明:
** 注意事项：
************************************************************/
static void motion_timerout_task(lv_timer_t *ptimer)
{
    if (abs(motion_detection_timestap - user_timestamp_get()) > (60 * 60 * 1000))
    {
        layout_motion_restart_motion_detection();
        lv_timer_del(motion_timerout_timer);
        motion_timerout_timer = NULL;
    }
}

/***
**   日期:2022-06-09 11:40:01
**   作者: leo.liu
**   函数作用：定时超时处理
**   参数说明:
***/
static void motion_timer_check_task(lv_timer_t *ptimer)
{

    if ((motion_timer_timeout_check() == true))
    {
        layout_motion_monitor_open();
        motion_detection_timestap = user_timestamp_get();
        motion_timerout_timer = lv_sat_timer_create(motion_timerout_task, 1000, NULL);
        // sat_linphone_motion_detection_start(80,user_data_get()->motion.sensivity);
        lv_timer_del(ptimer);
    }
}

/************************************************************
** 函数说明: 超时文本显示
** 作者: xiaoxiao
** 日期: 2023-06-05 18:51:09
** 参数说明:
** 注意事项:
************************************************************/
static void monitor_obj_timeout_label_display(void)
{
    lv_obj_t *obj = moniton_top_child_obj_get(6);
    if (obj == NULL)
    {
        return;
    }
    lv_label_set_text_fmt(obj, "%02d:%02d", motion_timeout_sec / 60, motion_timeout_sec % 60);
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 顶部时间通道显示
 ***********************************************/
static void monitior_obj_channel_info_obj_display(void)
{
    lv_obj_t *obj = moniton_top_child_obj_get(1);
    if (obj == NULL)
    {
        return;
    }
    struct tm tm;
    user_time_read(&tm);
    int channel = user_data_get()->motion.select_camera;
    lv_obj_set_x(obj, 37);
    if (is_channel_ipc_camera(channel) == true)
    {
        // lv_obj_set_x(obj, 96);
        channel -= 8;
        lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->cctv_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
    }
    else
    {
        lv_obj_set_x(obj, 37);
        lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->door_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
    }
}

static void motion_obj_timeout_timer(lv_timer_t *ptimer)
{
    if (motion_timeout_sec > 0)
    {
        motion_timeout_sec--;
        monitior_obj_channel_info_obj_display();
        monitor_obj_timeout_label_display();
    }
    else if (motion_timeout_sec == 0)
    {
        record_video_stop();
        motion_timeout_sec = 10;
        lv_timer_del(ptimer);
        layout_motion_restart_motion_detection();
    }
}
static void motion_event_tuya_report_timer(lv_timer_t *ptimer)
{
    bool jpeg_recoed = jpeg_record_state_get();
    if (jpeg_recoed == false)
    {
        unsigned char buffer[512];
        int ch = monitor_channel_get();
        ch = is_channel_ipc_camera(ch) == 0 ? ch : is_channel_ipc_camera(ch) == 1 ? ch - 2
                                                                                  : -1;
        tuya_event_report(0x04, ch, buffer, 512);
    }
    lv_timer_del(ptimer);
}

static bool layout_close_motion_dectection_callback(void)
{
    if (is_motion_snapshot_ing || is_motion_record_video_ing)
    {
        return false;
    }
    if (motion_timerout_timer != NULL)
    {
        lv_timer_del(motion_timerout_timer);
    }

    jpeg_record_state_set(false);
    lv_sat_timer_create(motion_event_tuya_report_timer, 5000, NULL);

    monitior_obj_channel_info_obj_display();
    monitor_obj_timeout_label_display();
    if (user_data_get()->motion.lcd_en == true)
    {
        backlight_enable(true);
    }

    if (((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL)) && (user_data_get()->motion.saving_fmt == 0))
    {
        record_video_start(false, REC_MODE_MOTION);
        record_jpeg_start(REC_MODE_TUYA_MOTION);
    }
    else
    {
        record_jpeg_start(REC_MODE_MOTION | REC_MODE_TUYA_MOTION);
    }
    lv_sat_timer_create(motion_obj_timeout_timer, 1000, NULL);
    return true;
}

static void layout_motion_rec_icon_hidden(bool en)
{
    lv_obj_t *obj = moniton_top_child_obj_get(2);
    if (obj == NULL)
    {
        return;
    }
    if (en)
    {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

/************************************************************
** 函数说明: 创建顶部显示区域
** 作者: xiaoxiao
** 日期: 2023-06-05 18:33:43
** 参数说明:
** 注意事项:
************************************************************/
static void layout_motion_head_cont_create(void)
{

    lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), motion_scr_act_obj_id_head_cont, 0, 0, 1024, 80,
                                                NULL, false, LV_OPA_30, 0, LV_OPA_30, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 通道显示
     ***********************************************/
    {
        lv_obj_t *obj = lv_common_text_create(parent, 1, 0, 23, 950, 42,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        if (obj != NULL)
        {
            lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
        }
        monitior_obj_channel_info_obj_display();
    }
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 静音图标显示
     ***********************************************/
    {
        lv_common_img_btn_create(parent, 2, 695, 16, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_monitoring_recording.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 移动侦测图标显示
     ***********************************************/
    {
        lv_common_img_btn_create(parent, 3, 751, 16, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_list_visitor_video.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 自动侦测图标显示
     ***********************************************/
    {
        lv_common_img_btn_create(parent, 4, 807, 16, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_monitoring_auto.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    /***********************************************
    ** 作者: leo.liu
    ** 日期: 2023-2-2 13:42:25
    ** 说明: sd图标显示
    ***********************************************/
    {
        lv_common_img_btn_create(parent, 5, 872, 16, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_monitoring_sdcard.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 倒计时
     ***********************************************/
    {
        lv_common_text_create(parent, 6, 927, 23, 73, 42,
                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        monitor_obj_timeout_label_display();
    }

    motion_obj_top_icon_display();
}

/************************************************************
** 函数说明: 录像回调
** 作者: xiaoxiao
** 日期: 2023-06-05 20:13:44
** 参数说明:
** 注意事项:
************************************************************/
static void layout_motion_video_state_callback(bool record_ing)
{
    is_motion_record_video_ing = record_ing;
    if (is_motion_record_video_ing == false)
    {
        layout_motion_rec_icon_hidden(true);

        return;
    }

    layout_motion_rec_icon_hidden(false);
}

/************************************************************
** 函数说明: 抓拍回调
** 作者: xiaoxiao
** 日期: 2023-06-05 20:14:03
** 参数说明:
** 注意事项:
************************************************************/
static void layout_motion_snapshot_state_callback(bool record_ing)
{

    is_motion_snapshot_ing = record_ing;
    lv_obj_t *obj = moniton_top_child_obj_get(2);
    if (obj == NULL)
    {
        return;
    }
    if ((media_sdcard_insert_check() == SD_STATE_UNPLUG) || (media_sdcard_insert_check() == SD_STATE_ERROR) || (user_data_get()->motion.saving_fmt == 1))
    {
        if (is_motion_snapshot_ing == true)
        {
            layout_motion_rec_icon_hidden(false);
        }
        else
        {
            layout_motion_rec_icon_hidden(true);
        }
    }
}

static void motion_detection_start_timer(lv_timer_t *timer)
{
    int level = user_data_get()->motion.sensivity;
    sat_linphone_motion_detection_start(80, level == 0 ? 150 : level == 1 ? 270
                                                                          : 400);
    lv_timer_del(timer);
}

static bool layout_motion_streams_stopping_register_callback(char *arg)
{

    sat_layout_goto(close, LV_SCR_LOAD_ANIM_FADE_IN, true);

    return true;
}

static bool layout_motion_streams_running_register_callback(char *arg)
{
    lv_timer_reset(lv_sat_timer_create(motion_detection_start_timer, 1000, NULL));

    return true;
}
static void layout_close_buzzer_alarm_trigger_default(void)
{
    buzzer_call_timestamp_set(user_timestamp_get());
    layout_close_click(NULL);
}

bool layout_frame_show_ch_vaile_check(void)
{
    if (user_data_get()->display.frame_list & 0x03)
    {
        return true;
    }
    if (user_data_get()->display.frame_list & 0x04)
    {
        int total = 0;
        file_type type = FILE_TYPE_FLASH_PHOTO;
        if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
        {
            type = FILE_TYPE_VIDEO;
        }
        media_file_total_get(type, &total, NULL);
        if (total > 0)
        {
            return true;
        }
    }
    if (user_data_get()->display.frame_list & 0x08)
    {
        if (monitor_door_first_valid_get(true) != -1)
        {
            return true;
        }
    }
    if (user_data_get()->display.frame_list & 0x10)
    {
        if (monitor_door_first_valid_get(false) != -1)
        {
            return true;
        }
    }
    return false;
}

static void frame_show_param_checktimer(lv_timer_t *ptimer)
{
    if (layout_frame_show_ch_vaile_check())
    {
        sat_layout_goto(frame_show, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
    }
}

static void sat_layout_enter(close)
{

    standby_timer_close();
    backlight_enable(false);
    close_cancel_btn_create();
    buzzer_call_callback_register(layout_close_buzzer_alarm_trigger_default);
    if (user_data_get()->motion.enable && ((user_data_get()->system_mode & 0x0f) == 0x01) && (monitor_valid_channel_check(user_data_get()->motion.select_camera)))
    {
        motion_timeout_sec = 10;
        is_motion_snapshot_ing = false;
        is_motion_record_video_ing = false;
        motion_timerout_timer = NULL;

        if (backlight_timer != NULL)
        {
            lv_timer_del(backlight_timer);
            backlight_timer = NULL;
        }
        user_linphone_call_streams_stopping_receive_register(layout_motion_streams_stopping_register_callback);

        user_linphone_call_streams_running_receive_register(layout_motion_streams_running_register_callback);
        /*记录注册*/
        record_state_callback_register(layout_motion_video_state_callback);
        /*抓拍注册*/
        snapshot_state_callback_register(layout_motion_snapshot_state_callback);
        /*移动侦测事件注册*/
        moiton_detection_event_cmd_register(layout_close_motion_dectection_callback);

        /*sd卡状态处理*/
        sd_state_channge_callback_register(layout_motion_sd_state_change_callback);

        layout_motion_head_cont_create();

        if (user_data_get()->motion.timer_en == false)
        {
            layout_motion_monitor_open();
            motion_detection_timestap = user_timestamp_get();

            motion_timerout_timer = lv_sat_timer_create(motion_timerout_task, 1000, NULL);
        }
        else
        {
            lv_timer_reset(lv_sat_timer_create(motion_timer_check_task, 1000, NULL));
        }
    }
    else if ((user_data_get()->display.standby_mode == 1) && (frame_display_timeout_check() == false))
    {
        if (layout_frame_show_ch_vaile_check())
        {
            sat_layout_goto(frame_show, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        else
        {
            lv_timer_reset(lv_sat_timer_create(frame_show_param_checktimer, 1000, NULL));
        }
    }
}

static void layout_close_backlight_open_timer(lv_timer_t *t)
{
    if (sat_cur_layout_get() != sat_playout_get(close))
    {
        backlight_enable(true);
    }
    lv_timer_del(backlight_timer);
    backlight_timer = NULL;
}

static void sat_layout_quit(close)
{
    buzzer_call_callback_register(buzzer_alarm_trigger_default);

    record_video_stop();

    monitor_close(0x02);
    if (user_data_get()->etc.time_automatically)
    {
        extern bool tuya_api_time_sync(void);
        tuya_api_time_sync();
    }
    lv_obj_remove_event_cb(sat_cur_layout_screen_get(), layout_close_click);

    /*记录注册*/
    record_state_callback_register(NULL);
    /*抓拍注册*/
    snapshot_state_callback_register(NULL);
    /*移动侦测事件注册*/
    moiton_detection_event_cmd_register(NULL);

    /*sd卡状态处理*/
    sd_state_channge_callback_register(sd_state_change_default_callback);

    user_linphone_call_streams_stopping_receive_register(NULL);

    user_linphone_call_streams_running_receive_register(NULL);
    lv_obj_clean(sat_cur_layout_screen_get());

    backlight_timer = lv_timer_create(layout_close_backlight_open_timer, 300, NULL);

    standby_timer_restart(true, true);
}

sat_layout_create(close);
