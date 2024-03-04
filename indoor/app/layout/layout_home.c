#include "layout_define.h"
#include "layout_setting_time.h"
#include "tuya_api.h"
#include "common/commax_websocket.h"
enum
{
        home_obj_id_sd_icon,

        home_obj_id_topbar,

        home_obj_id_setting_icon,
        home_obj_id_mute_icon,

        home_obj_id_door_icon,

        home_obj_id_network_icon,

        home_obj_id_tuya_connect_icon,

        home_obj_id_top_label,

        home_obj_id_slave_time_sync_failed,

        home_obj_id_time_cont,
        home_obj_id_time_label,
        home_obj_id_date_label,

        home_obj_id_video_cont,
        home_obj_id_video_title,
        home_obj_id_no_video_title,
        home_obj_id_video_img,
        home_obj_id_video_label,
        home_obj_id_video_new,

        home_obj_id_recent_call_cont,
        home_obj_id_recent_call_title,
        home_obj_id_no_call_title,
        home_obj_id_recent_call_list,
        home_obj_id_recent_call_right,
        home_obj_id_recent_call_new,

        home_obj_id_monitor_cont,
        home_obj_id_monitor_img,
        home_obj_id_monitor_busy_img,
        home_obj_id_monitor_label,

        home_obj_id_call_cont,
        home_obj_id_call_img,
        home_obj_id_call_label,

        home_obj_id_cctv_cont,
        home_obj_id_cctv_img,
        home_obj_id_cctv_busy_img,
        home_obj_id_cctv_label,

        home_obj_id_away_cont,
        home_obj_id_away_img,
        home_obj_id_away_label,

        home_obj_id_burglar_cont,
        home_obj_id_burglar_img,
        home_obj_id_burglar_label,

        home_obj_id_elevator_cont,
        home_obj_id_elevator_img,
        home_obj_id_elevator_label,

        home_obj_id_emergency_cont,
        home_obj_id_emergency_img,
        home_obj_id_emergency_label,

        home_obj_id_the_communication_line_msgbox_parent,
        home_obj_id_the_communication_line_msgbox_cont,
        home_obj_id_the_communication_line_msgbox_lable,
        home_obj_id_the_communication_line_msgbox_btn,

        home_obj_id_the_monitoring_msgbox_parent,
        home_obj_id_the_monitoring_msgbox_cont,
        home_obj_id_the_monitoring_msgbox_title,

        home_obj_id_the_monitoring_msgbox_checkbox1_cont,
        home_obj_id_the_monitoring_msgbox_checkbox1_label,
        home_obj_id_the_monitoring_msgbox_checkbox1_img,

        home_obj_id_the_monitoring_msgbox_checkbox2_cont,
        home_obj_id_the_monitoring_msgbox_checkbox2_label,
        home_obj_id_the_monitoring_msgbox_checkbox2_img,

        home_obj_id_the_monitoring_msgbox_cancel,
        home_obj_id_the_monitoring_msgbox_confirm,

        home_obj_id_buzzer_call,

};
#define THUMB_WIDTH (304)
#define THUMB_HIGHT (224)

static void home_setting_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_TOP, SAT_VOID);
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-2 13:42:25
** 说明: home 静音
***********************************************/
static void home_mute_obj_display(lv_obj_t *obj)
{
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->audio.ring_mute == true ? "btn_mute_on.png" : "btn_mute_off.png"), LV_PART_MAIN);
}
static void home_mute_obj_click(lv_event_t *ev)
{
        user_data_get()->audio.ring_mute = user_data_get()->audio.ring_mute == true ? false : true;
        user_data_save(false, false);
        home_mute_obj_display(lv_event_get_current_target(ev));
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 顶部图标显示
 ***********************************************/
static void home_obj_top_icon_display(void)
{
        lv_obj_t *obj = NULL;
        int pos_x = 960;
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 网络状态图标显示
         ***********************************************/
        {
                if ((user_data_get()->system_mode & 0x1f) == 0x01)
                {
                        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_network_icon);
                        if (obj == NULL)
                        {
                                return;
                        }
                        char state = tuya_api_network_status();
                        pos_x -= 56;
                        if ((state == 0x00) || (user_data_get()->etc.tuya_connect_mode == 0 && user_data_get()->wifi_enable == false))
                        {
                                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->etc.tuya_connect_mode == 0 ? "ic_system_network_off.png" : "ic_system_internet_NY.png"), LV_PART_MAIN);
                        }
                        else if (state == 0x01)
                        {
                                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->etc.tuya_connect_mode == 0 ? "ic_system_network_wifi.png" : "ic_system_internet_YN.png"), LV_PART_MAIN);
                        }
                        else
                        {
                                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->etc.tuya_connect_mode == 0 ? "ic_system_network_on.png" : "ic_system_internet_YY.png"), LV_PART_MAIN);
                        }
                }
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: tuya连接状态
         ***********************************************/
        {
                if ((user_data_get()->system_mode & 0x1f) == 0x01)
                {
                        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_tuya_connect_icon);
                        {
                                if (obj != NULL)
                                {
                                        lv_obj_set_x(obj, pos_x);
                                        pos_x -= 56;
                                        if (tuya_api_online_check())
                                        {
                                                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_home_clood_connect.png"), LV_PART_MAIN);
                                        }
                                        else if (tuya_api_network_status() == 0x02)
                                        {
                                                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_home_clood_connectable.png"), LV_PART_MAIN);
                                        }
                                        else
                                        {
                                                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_home_clood_disconnect.png"), LV_PART_MAIN);
                                        }
                                }
                        }
                }
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: SD卡显示
         ***********************************************/
        {
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_sd_icon);
                if (obj == NULL)
                {
                        return;
                }
                lv_obj_set_x(obj, pos_x);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                pos_x -= 56;
                if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
                {
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(media_sdcard_insert_check() == SD_STATE_INSERT ? "ic_system_sdcard.png" : "ic_system_sdcard_full.png"), LV_PART_MAIN);
                }
                else
                {

                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_system_sdcard_empty.png"), LV_PART_MAIN);
                }
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 门口机在线状态显示
         ***********************************************/
        {
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_door_icon);
                {
                        if (obj != NULL)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);

                                int online_num = 0;
                                outdoor_online_check(MON_CH_DOOR1, &online_num);
                                if ((door_camera_register_num_get() != online_num) && door_camera_register_num_get() != 0) // 注册的门口机和在线的门口机数量不一致
                                {
                                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                        lv_obj_set_x(obj, pos_x + 13); // 门口机在线图标有点偏小
                                }
                        }
                }
        }
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: app状态显示
 ***********************************************/
static void home_use_mobile_app_obj_display()
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_top_label);
        if (obj->user_data != NULL)
        {
                return;
        }
        if (tuya_api_client_num() > 0)
        {
                lv_label_set_text(obj, lang_str_get(HOME_XLS_LANG_ID_USE_MOBILE_APP));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

static void home_slave_sync_time_obj_display_timer(lv_timer_t *ptimer)
{
        lv_obj_t *obj = (lv_obj_t *)ptimer->user_data;
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        lv_timer_del(ptimer);
        obj->user_data = NULL;
}

/************************************************************
** 函数说明: 分机时间提示显示
** 作者: xiaoxiao
** 日期：2023-10-05 19:17:55
** 参数说明:
** 注意事项：
************************************************************/
static void home_slave_sync_time_obj_display()
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_top_label);
        lv_label_set_text(obj, lang_str_get(HOME_XLS_LANG_TIME_SET_BY_MASTER));
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        if (obj->user_data == NULL)
        {
                obj->user_data = (void *)lv_sat_timer_create(home_slave_sync_time_obj_display_timer, 3000, obj);
        }
}

static void home_slave_sync_time_request_timer(lv_timer_t *ptimer)
{
        if (ptimer != NULL)
        {
                lv_timer_set_period(ptimer, 5000);
        }

        struct tm tm;
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_slave_time_sync_failed);
        if (sat_ipcamera_system_time_get(user_data_get()->mastar_wallpad_ip, 80, "admin", "123456789", 0x00, &tm, 2000) == false)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                if (ptimer != NULL)
                {
                        lv_timer_del(ptimer);
                }
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                standby_timer_close();
                user_time_set(&tm);

                standby_timer_restart(true);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明:时间
 ***********************************************/
static void home_date_obj_click(lv_event_t *ev)
{
        if ((user_data_get()->system_mode & 0x0F) != 0x01)
        {
                home_slave_sync_time_request_timer(NULL);
                home_slave_sync_time_obj_display();
                return;
        }
        setting_time_first_enter_set_flag(0x01);
        sat_layout_goto(setting_time, LV_SCR_LOAD_ANIM_MOVE_TOP, SAT_VOID);
}
static void home_time_obj_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_time_cont);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, home_obj_id_time_label);
        if (obj == NULL)
        {
                return;
        }
        struct tm tm;
        user_time_read(&tm);
        lv_label_set_text_fmt(obj, "%02d:%02d", tm.tm_hour, tm.tm_min);
}
static void home_date_obj_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_time_cont);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *label_date = lv_obj_get_child_form_id(parent, home_obj_id_date_label);
        if (label_date == NULL)
        {
                return;
        }
        struct tm tm;
        user_time_read(&tm);
        char week_str[64] = {0};
        strcpy(week_str, lang_str_get(tm.tm_wday - 1 + HOME_XLS_LANG_ID_MON));
        // layout_home_week_language_get(tm.tm_wday - 1);
        char mon_str[64] = {0};
        strcpy(mon_str, lang_str_get(tm.tm_mon - 1 + HOME_XLS_LANG_ID_JAN));
        // layout_home_month_language_get(tm.tm_mon - 1);
        LANGUAGE_ID lang = language_id_get();
        if (lang == LANGUAGE_ID_ENGLISH)
        {
                lv_label_set_text_fmt(label_date, "%s %s %d. %04d", week_str, mon_str, tm.tm_mday, tm.tm_year);
        }
        else if (lang == LANGUAGE_ID_HANYU)
        {
                lv_label_set_text_fmt(label_date, "%04d년 %d월 %d일 %s", tm.tm_year, tm.tm_mon, tm.tm_mday, week_str);
        }
        else if (lang == LANGUAGE_ID_ELUOSI)
        {
                lv_label_set_text_fmt(label_date, "%s%d %s %04d", week_str, tm.tm_mday, mon_str, tm.tm_year);
        }
        else if (lang == LANGUAGE_ID_XIBANYA)
        {
                lv_label_set_text_fmt(label_date, "%s %d %s %04d", week_str, tm.tm_mday, mon_str, tm.tm_year);
        }
        else if (lang == LANGUAGE_ID_CHINESE)
        {
                lv_label_set_text_fmt(label_date, "%04d年 %d月 %d日 %s", tm.tm_year, tm.tm_mon, tm.tm_mday, week_str);
        }
        else if (lang == LANGUAGE_ID_YUENAN)
        {
                lv_label_set_text_fmt(label_date, "%s %d %s %04d", week_str, tm.tm_mday, mon_str, tm.tm_year);
        }
        else if (lang == LANGUAGE_ID_ALABOYU)
        {
                lv_label_set_text_fmt(label_date, "%d %s %04d %s", tm.tm_mday, mon_str, tm.tm_year, week_str);
        }
}
static void home_date_timer(lv_timer_t *ptimer)
{
        home_time_obj_display();
        home_date_obj_display();
        home_use_mobile_app_obj_display();
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: latest video
 ***********************************************/
static void home_latest_video_obj_click(lv_event_t *ev)
{
        sat_layout_goto(playback, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: latest call
 ***********************************************/
static void home_latest_call_obj_click(lv_event_t *ev)
{
        extern void enter_intercomm_call_mode_set(int mode);
        enter_intercomm_call_mode_set(1);
        layout_last_call_new_flag_set(false);
        sat_layout_goto(intercom_call, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 消息框
 ***********************************************/
static void home_monitoring_msgbox_checkbox_click(lv_event_t *ev)
{
        lv_obj_t *parent = lv_event_get_current_target(ev);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *check_obj, *discheck_obj;
        if (parent->id == home_obj_id_the_monitoring_msgbox_checkbox1_cont)
        {
                check_obj = lv_obj_get_child_form_id(parent, home_obj_id_the_monitoring_msgbox_checkbox1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), home_obj_id_the_monitoring_msgbox_checkbox2_cont);
                discheck_obj = lv_obj_get_child_form_id(parent, home_obj_id_the_monitoring_msgbox_checkbox2_img);
        }
        else
        {
                check_obj = lv_obj_get_child_form_id(parent, home_obj_id_the_monitoring_msgbox_checkbox2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), home_obj_id_the_monitoring_msgbox_checkbox1_cont);
                discheck_obj = lv_obj_get_child_form_id(parent, home_obj_id_the_monitoring_msgbox_checkbox1_img);
        }

        if ((check_obj == NULL) || (discheck_obj == NULL) || (check_obj->bg_img_src == NULL) || (discheck_obj->bg_img_src == NULL))
        {
                return;
        }

        if (strncmp(check_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(discheck_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
}
static void home_monitoring_msgbox_cancel_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_the_monitoring_msgbox_parent);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}
static void home_monitoring_msgbox_confirm_click(lv_event_t *ev)
{
        lv_obj_t *msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_the_monitoring_msgbox_parent);
        if (msgbox == NULL)
        {
                return;
        }
        lv_obj_t *parent = lv_obj_get_child_form_id(msgbox, home_obj_id_the_monitoring_msgbox_cont);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *checkbox = lv_obj_get_child_form_id(parent, home_obj_id_the_monitoring_msgbox_checkbox1_cont);
        if (checkbox == NULL)
        {
                return;
        }
        lv_obj_t *img = lv_obj_get_child_form_id(checkbox, home_obj_id_the_monitoring_msgbox_checkbox1_img);
        if (img == NULL)
        {
                return;
        }

        if (strncmp((const char *)img->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
        {
                monitor_channel_set(0);
        }
        else
        {
                monitor_channel_set(0xFF);
        }
        lv_obj_del(msgbox);

        monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static lv_obj_t *home_monitoring_msgbox_create(void)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_the_monitoring_msgbox_parent, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, home_obj_id_the_monitoring_msgbox_cont, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, home_obj_id_the_monitoring_msgbox_title, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              lang_str_get(HOME_XLS_LANG_ID_MONITORING), 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_img_text_btn_create(msgbox, home_obj_id_the_monitoring_msgbox_checkbox1_cont, 48, 110, 365, 48,
                                      home_monitoring_msgbox_checkbox_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, home_obj_id_the_monitoring_msgbox_checkbox1_label,
                                      lang_str_get(HOME_XLS_LANG_ID_MONITORING), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, home_obj_id_the_monitoring_msgbox_checkbox1_img,
                                      (const char *)resource_ui_src_get("btn_radio_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_img_text_btn_create(msgbox, home_obj_id_the_monitoring_msgbox_checkbox2_cont, 48, 166, 365, 48,
                                      home_monitoring_msgbox_checkbox_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, home_obj_id_the_monitoring_msgbox_checkbox2_label,
                                      lang_str_get(HOME_XLS_LANG_ID_COMMON_ENTRANCE), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, home_obj_id_the_monitoring_msgbox_checkbox2_img,
                                      (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, home_obj_id_the_monitoring_msgbox_cancel, 0, 281, 230, 62,
                                 home_monitoring_msgbox_cancel_click, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, home_obj_id_the_monitoring_msgbox_confirm, 230, 281, 230, 62,
                                 home_monitoring_msgbox_confirm_click, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: monitor
 ***********************************************/
static void home_monitor_obj_click(lv_event_t *ev)
{
        if (0 /*(user_data_get()->system_mode & 0xF0) == 0x10*/) // 室内机不允许监控大厅
        {
                home_monitoring_msgbox_create();
        }
        else
        {
                int channel = monitor_door_first_valid_get(true);
                if (channel < 0)
                {
                        SAT_DEBUG("Invalid channel");
                        return;
                }

                monitor_channel_set(channel);
                monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
}

static void home_call_obj_click(lv_event_t *ev)
{
        extern void enter_intercomm_call_mode_set(int mode);
        enter_intercomm_call_mode_set(0);
        sat_layout_goto(intercom_call, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}

static void home_cctv_obj_click(lv_event_t *ev)
{
        {
                int channel = monitor_door_first_valid_get(false);
                if (channel < 0)
                {
                        SAT_DEBUG("Invalid channel");
                        return;
                }
                monitor_channel_set(channel);
                monitor_enter_flag_set(MON_ENTER_MANUAL_CCTV_FLAG);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
}
static void home_away_obj_click(lv_event_t *ev)
{
        sat_layout_goto(away, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void home_burglar_obj_click(lv_event_t *ev)
{
        sat_layout_goto(security, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 消息框
 ***********************************************/
static void home_communication_line_msg_box_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_the_communication_line_msgbox_parent);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}
static lv_obj_t *home_communication_line_msg_box_create(void)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_the_communication_line_msgbox_parent, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, home_obj_id_the_communication_line_msgbox_cont, 282, 131, 460, 283,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, home_obj_id_the_communication_line_msgbox_lable, 35, 73, 392, 71,
                              NULL, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x505050,
                              0, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x505050,
                              lang_str_get(HOME_XLS_LANG_ID_THE_COMMUNICATION_LINE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_img_btn_create(msgbox, home_obj_id_the_communication_line_msgbox_btn, 0, 221, 460, 62,
                                 home_communication_line_msg_box_click, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}
static void home_elevator_obj_click(lv_event_t *ev)
{
        home_communication_line_msg_box_create();
}
static void home_emergency_obj_click(lv_event_t *ev)
{
        sat_layout_goto(emergency_setting, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void home_call_list_item_create(lv_obj_t *parent)
{
        CALL_LOG_TYPE type;
        char doorname[24] = {0};
        int duration;
        struct tm tm;
        int item_y = 0;
        int total = call_list_total_get();
        for (int i = total - 1; i >= 0; i--)
        {
                call_list_get(i, &type, doorname, &duration, &tm);
                char buffer[64] = {0};
                sprintf(buffer, "%04d-%02d-%02d  %02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
                lv_common_text_create(parent, i, 0, item_y, 180, 39,
                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
                                      0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x505050,
                                      0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x505050,
                                      buffer, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                // lv_obj_set_style_pad_left(obj, 22, LV_PART_MAIN);
                item_y += 39;
        }
}
static lv_img_dsc_t *home_thumb_img_dsc = NULL;

static bool home_thumb_media_display_callback(const char *data, int x, int y, int w, int h)
{
        if ((home_thumb_img_dsc != NULL) && (home_thumb_img_dsc->data != NULL))
        {
                if (home_thumb_img_dsc->header.w != w)
                {
                        SAT_DEBUG("home_thumb_img_dsc->header.w != w (%d != %d)", home_thumb_img_dsc->header.w, w);
                        return false;
                }
                if (home_thumb_img_dsc->header.h != h)
                {
                        SAT_DEBUG("home_thumb_img_dsc->header.h != h (%d != %d)", home_thumb_img_dsc->header.h, h);
                        return false;
                }
                lv_memcpy_small((uint8_t *)home_thumb_img_dsc->data, data, w * h * 3);

                // char filename[128] = {0};
                // struct tm date;
                // user_time_read(&date);

                // sprintf(filename, "/tmp/nfs/%02d%02d%02d-%02d%02d%02d-%dx%d",
                //         date.tm_year % 100,
                //         date.tm_mon,
                //         date.tm_mday,
                //         date.tm_hour,
                //         date.tm_min,
                //         date.tm_sec, w, h);
                // int fd = open(filename, O_CREAT | O_WRONLY);
                // if (fd < 0)
                // {
                //         printf("open %s failed \n", filename);
                //         return false;
                // }

                // write(fd, data, w * h * 3);
                // close(fd);
                // SAT_DEBUG("thumb media data copy (%dx%d) ", w, h);
                return true;
        }
        //    SAT_DEBUG("thumb display failed(%p,%p)\n", home_thumb_img_dsc, home_thumb_img_dsc->data);
        return false;
}
static void home_thumb_refresh_display_callback(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);
        if (parent == NULL)
        {
                SAT_DEBUG(" lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);");
                return;
        }

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, home_obj_id_video_img);
        if (obj == NULL)
        {
                SAT_DEBUG("v_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_img);");
                return;
        }
        // // 创建一个样式并设置圆角半径
        // static lv_style_t style;
        // lv_style_init(&style);
        // lv_style_set_radius(&style, 20); // 设置圆角半径
        // lv_obj_add_style(obj, &style, LV_PART_MAIN);
        // lv_obj_refresh_style(obj, LV_PART_MAIN, LV_STYLE_RADIUS); // 刷新样式以反映更改

        lv_obj_set_style_bg_img_src(obj, home_thumb_img_dsc, LV_PART_MAIN);
        lv_img_cache_invalidate_src(obj->bg_img_src);
}

static void layout_home_video_call_title_param_init(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);
        if (parent == NULL)
        {
                return;
        }

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, home_obj_id_no_video_title);
        lv_obj_t *video_date = lv_obj_get_child_form_id(parent, home_obj_id_video_label);
        if ((obj == NULL) || video_date == NULL)
        {
                return;
        }
        file_type type = FILE_TYPE_FLASH_PHOTO;
        int total = 0;
        if (((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL)))
        {
                type = FILE_TYPE_PHOTO;
        }
        media_file_total_get(type, &total, NULL);
        if (total <= 0)
        {

                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(video_date, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(video_date, LV_OBJ_FLAG_HIDDEN);
        }
}

static void home_media_thumb_time_display(const char *filename)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);
        if (parent == NULL)
        {
                SAT_DEBUG(" lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);");
                return;
        }

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, home_obj_id_video_label);
        if (obj == NULL)
        {
                SAT_DEBUG("v_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_label);");
                return;
        }
        struct tm tm;
        if (media_filename_to_time(filename, &tm) == true)
        {
                lv_label_set_text_fmt(obj, "%04d-%02d-%02d\n%02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        }
}
static void home_media_thumb_new_display(bool new)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);
        if (parent == NULL)
        {
                SAT_DEBUG(" lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);");
                return;
        }

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, home_obj_id_video_new);
        if (obj == NULL)
        {
                SAT_DEBUG("v_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_label);");
                return;
        }
        if (new == true)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

static void home_media_thumb_display(void)
{

        file_type type = FILE_TYPE_FLASH_PHOTO;
        char arry[1][128] = {0}; //{"/tmp/tf/media/300116-1911038.JPG 1 42 216 174"};
        int total = 0, new_total = 0;
        if (((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL)))
        {
                type = FILE_TYPE_VIDEO;
        }
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);
        if (parent == NULL)
        {
                SAT_DEBUG(" lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_cont);");
                return;
        }

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, home_obj_id_video_img);
        if (obj == NULL)
        {
                SAT_DEBUG("v_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_video_img);");
                return;
        }
        lv_obj_set_style_bg_img_src(obj, NULL, LV_PART_MAIN);
        // SAT_DEBUG("file type is %d\n", type);
        media_file_total_get(type, &total, &new_total);
        if (total <= 0)
        {
                return;
        }
        const file_info *info = NULL;

        info = media_file_info_get(type, total - 1);

        memset(arry[0], 0, sizeof(arry[0]));
        sprintf(arry[0], "%s%s 1 42 %d %d", type == FILE_TYPE_FLASH_PHOTO ? FLASH_PHOTO_PATH : SD_MEDIA_PATH, info->file_name, THUMB_WIDTH, THUMB_HIGHT);
        sat_linphone_media_thumb_display(arry, 1, home_thumb_media_display_callback);
        home_media_thumb_time_display(info->file_name);
        home_media_thumb_new_display(info->is_new);
}

static void home_sd_state_change_callback(void)
{

        layout_home_video_call_title_param_init();
        home_media_thumb_display();
        home_obj_top_icon_display();
}

static void layout_home_monitor_icon_display()
{

        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_monitor_cont);

        if (obj == NULL)

        {
                return;
        }
        lv_obj_t *offline = lv_obj_get_child_form_id(obj, home_obj_id_monitor_busy_img);
        if (offline == NULL)
        {
                return;
        }
        if (door_camera_register_num_get() <= 0)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);

                lv_obj_clear_flag(offline, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);

                lv_obj_add_flag(offline, LV_OBJ_FLAG_HIDDEN);
        }
}

static void layout_home_cctv_icon_display()
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_cctv_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_t *offline = lv_obj_get_child_form_id(obj, home_obj_id_cctv_busy_img);
        if (offline == NULL)
        {
                return;
        }
        if (cctv_register_num_get() <= 0)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);

                lv_obj_clear_flag(offline, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);

                lv_obj_add_flag(offline, LV_OBJ_FLAG_HIDDEN);
        }
}

static void layout_home_security_away_btn_display(void)
{
        lv_obj_t *away = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_away_cont);
        lv_obj_t *security = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), home_obj_id_burglar_cont);
        lv_obj_t *icon_away_ban = lv_obj_get_child_form_id(away, 0);
        lv_obj_t *icon_away_en = lv_obj_get_child_form_id(away, 1);
        lv_obj_t *icon_security_ban = lv_obj_get_child_form_id(security, 0);
        lv_obj_t *icon_security_en = lv_obj_get_child_form_id(security, 1);
        if (user_data_get()->alarm.security_alarm_enable)
        {
                lv_obj_clear_flag(away, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(security, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(icon_security_ban, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(icon_security_en, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(icon_away_en, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(icon_away_ban, LV_OBJ_FLAG_HIDDEN);
        }
        else if (user_data_get()->alarm.away_alarm_enable)
        {
                lv_obj_clear_flag(security, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(away, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(icon_away_ban, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(icon_away_en, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(icon_security_en, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(icon_security_ban, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(security, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(away, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(icon_security_ban, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(icon_away_ban, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(icon_security_en, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(icon_away_en, LV_OBJ_FLAG_HIDDEN);
        }
}

static void home_obj_top_icon_display_timer(lv_timer_t *ptimer)
{
        layout_home_monitor_icon_display();
        layout_home_cctv_icon_display();
        home_obj_top_icon_display();
        layout_home_security_away_btn_display();
}

static void sat_layout_enter(home)
{

        tuya_api_network_detect_enable(true);
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 缩略图显示相关
         ***********************************************/
        {
                if (home_thumb_img_dsc == NULL)
                {
                        home_thumb_img_dsc = lv_img_buf_alloc(THUMB_WIDTH, THUMB_HIGHT, LV_IMG_CF_TRUE_COLOR);
                        home_thumb_img_dsc->header.reserved = 0x03;
                }
                thumb_display_refresh_register(home_thumb_refresh_display_callback);
                sd_state_channge_callback_register(home_sd_state_change_callback);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: topbar
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_topbar, 0, 0, 1024, 48,
                                         NULL, false, LV_OPA_COVER, 0X101010, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: setting
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_setting_icon, 32, 72, 48, 48,
                                         home_setting_obj_click, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_setting.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: mute
        ***********************************************/
        {
                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_mute_icon, 904, 72, 88, 48,
                                                         home_mute_obj_click, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_mute_off.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                if (obj == NULL)
                {
                        return;
                }

                home_mute_obj_display(obj);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: app use
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), home_obj_id_top_label, 277, 66, 470, 50,
                                                      NULL, LV_OPA_COVER, 0X303030, LV_OPA_TRANSP, 0,
                                                      16, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      16, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(HOME_XLS_LANG_ID_USE_MOBILE_APP), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                if (obj == NULL)
                {
                        return;
                }
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);
                home_use_mobile_app_obj_display();
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: home date
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_time_cont, 116, 198, 252, 144,
                                                            home_date_obj_click, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                if (parent == NULL)
                {
                        return;
                }
                // 时间
                lv_obj_t *obj = lv_common_text_create(parent, home_obj_id_time_label, 0, 0, 252, 110,
                                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_plus);
                if (obj == NULL)
                {
                        return;
                }
                lv_obj_set_style_pad_top(obj, 15, LV_PART_MAIN);
                home_time_obj_display();
                lv_common_text_create(parent, home_obj_id_date_label, 0, 118, 252, 26,
                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                home_date_obj_display();
                lv_sat_timer_create(home_date_timer, 1000, NULL);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: video
        ***********************************************/
        {

                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_video_cont, 428, 168, 216, 216,
                                                            home_latest_video_obj_click, true, LV_OPA_COVER, 0x242526, LV_OPA_COVER, 0x242526,
                                                            8, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_TRANSP, 0,
                                                            8, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_text_create(parent, home_obj_id_video_title, 14, 8, 216 - 14, 22,
                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0X303030,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(HOME_XLS_LANG_ID_RECENT_VIDEO), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_small);

                lv_common_img_btn_create(parent, home_obj_id_video_img, 0, 40, THUMB_WIDTH, THUMB_HIGHT,
                                         NULL, false, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x00,
                                         8, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_TRANSP, 0,
                                         8, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_text_create(parent, home_obj_id_no_video_title, 0, 80, 216, 72,
                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0X303030,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(XLS_LANG_ID_NO_DATA_SAVE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

                lv_common_text_create(parent, home_obj_id_video_label, 0, 216 - 54, 224, 54,
                                      NULL, LV_OPA_60, 0, LV_OPA_60, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

                lv_common_img_btn_create(parent, home_obj_id_video_new, 158, 10, 48, 24,
                                         NULL, false, LV_OPA_TRANSP, 0x242526, LV_OPA_TRANSP, 0x242526,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: last call
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_recent_call_cont, 668, 168, 216, 216,
                                                            home_latest_call_obj_click, false, LV_OPA_COVER, 0x242526, LV_OPA_COVER, 0x242526,
                                                            8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_text_create(parent, home_obj_id_recent_call_title, 14, 8, 216 - 14, 22,
                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(HOME_XLS_LANG_ID_RECENT_CALL), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_small);

                lv_obj_t *list = lv_list_create(parent);
                lv_common_style_set_common(list, home_obj_id_recent_call_list, 16, 50, 200, 107, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                home_call_list_item_create(list);

                lv_obj_t *obj = lv_common_img_btn_create(parent, home_obj_id_recent_call_right, 105, 178, 24, 24,
                                                         home_latest_call_obj_click, true, LV_OPA_TRANSP, 0x242526, LV_OPA_TRANSP, 0x242526,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_widget_more.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                lv_obj_set_ext_click_area(obj, 30);
                if (!call_list_total_get())
                {
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }

                obj = lv_common_text_create(parent, home_obj_id_no_call_title, 18, 80, 180, 80,
                                            NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0X303030,
                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                            lang_str_get(XLS_LANG_ID_NO_DATA_SAVE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                if (call_list_total_get())
                {
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
                lv_common_img_btn_create(parent, home_obj_id_recent_call_new, 158, 10, 48, 24,
                                         NULL, false, LV_OPA_TRANSP, 0x242526, LV_OPA_TRANSP, 0x242526,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         layout_last_call_new_flag_get() ? resource_ui_src_get("ic_main_new.png") : "", LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        layout_home_video_call_title_param_init();

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 14:16:18
        ** 说明: 监控设置
        ***********************************************/
        {
                int sec_x = ((user_data_get()->system_mode & 0xF0) != 0x10) ? 125 : 57; // user_data_get()->system_mode == 1?193:329;
                int unit_offset = ((user_data_get()->system_mode & 0xF0) == 0x10) ? 136 : 136;
                lv_obj_t *obj = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_monitor_cont, sec_x, 436, 103, 121,
                                                              home_monitor_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 83, 103, 27, home_obj_id_monitor_label,
                                                              lang_str_get(HOME_XLS_LANG_ID_MONITORING), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                              13, 0, 77, 77, home_obj_id_monitor_img,
                                                              (const char *)resource_ui_src_get("btn_main_monitoring_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                lv_common_img_btn_create(obj, home_obj_id_monitor_busy_img, 55, 45, 48, 48,
                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("ic_main_inactive.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                layout_home_monitor_icon_display();

                sec_x += unit_offset;
                if (1 /*user_data_get()->system_mode == 0*/)
                {
                        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_call_cont, sec_x, 436, 103, 121,
                                                      home_call_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 83, 103, 27, home_obj_id_call_label,
                                                      lang_str_get(HOME_XLS_LANG_ID_CALL), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                      13, 0, 77, 77, home_obj_id_call_img,
                                                      (const char *)resource_ui_src_get("btn_main_interphone_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        sec_x += unit_offset;
                }

                obj = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_cctv_cont, sec_x, 436, 103, 121,
                                                    home_cctv_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 83, 103, 27, home_obj_id_cctv_label,
                                                    lang_str_get(HOME_XLS_LANG_ID_CCTV), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                    13, 0, 77, 77, home_obj_id_cctv_img,
                                                    (const char *)resource_ui_src_get("btn_main_cctv_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                lv_common_img_btn_create(obj, home_obj_id_cctv_busy_img, 55, 45, 48, 48,
                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("ic_main_inactive.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                layout_home_cctv_icon_display();
                sec_x += unit_offset;
                if (1 /*user_data_get()->system_mode == 0*/)
                {
                        lv_obj_t *away = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_away_cont, sec_x, 436, 103, 121,
                                                                       home_away_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                       0, 83, 103, 27, home_obj_id_away_label,
                                                                       lang_str_get(HOME_XLS_LANG_ID_AWAY), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                       13, 0, 77, 77, home_obj_id_away_img,
                                                                       (const char *)resource_ui_src_get("btn_main_away_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                        lv_common_img_btn_create(away, 0, 55, 45, 48, 48,
                                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("ic_main_inactive.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        lv_common_img_btn_create(away, 1, 55, 45, 48, 48,
                                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("ic_detect.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                        sec_x += unit_offset;
                }
                if (1 /*(user_data_get()->system_mode == 0) || (user_data_get()->system_mode == 1)*/)
                {
                        lv_obj_t *security = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_burglar_cont, sec_x, 436, 103, 121,
                                                                           home_burglar_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                                           0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                           0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                           0, 83, 103, 27, home_obj_id_burglar_label,
                                                                           lang_str_get(HOME_XLS_LANG_ID_BURGLAR), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                           13, 0, 77, 77, home_obj_id_burglar_img,
                                                                           (const char *)resource_ui_src_get("btn_main_security_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        lv_common_img_btn_create(security, 0, 55, 45, 48, 48,
                                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("ic_main_inactive.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        lv_common_img_btn_create(security, 1, 55, 45, 48, 48,
                                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("ic_detect.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        sec_x += unit_offset;
                }

                if ((user_data_get()->system_mode & 0xF0) == 0x10)
                {
                        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_elevator_cont, sec_x, 436, 103, 121,
                                                      home_elevator_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 83, 103, 27, home_obj_id_elevator_label,
                                                      lang_str_get(HOME_XLS_LANG_ID_ELEVATOR), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                      13, 0, 77, 77, home_obj_id_elevator_img,
                                                      (const char *)resource_ui_src_get("btn_main_elevator_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        sec_x += unit_offset;
                }
                layout_home_security_away_btn_display();

                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), home_obj_id_emergency_cont, sec_x, 436, 103, 121,
                                              home_emergency_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 83, 103, 27, home_obj_id_emergency_label,
                                              lang_str_get(HOME_XLS_LANG_ID_EMERGENCY), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                              13, 0, 77, 77, home_obj_id_emergency_img,
                                              (const char *)resource_ui_src_get("btn_main_emergency_w.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }

        {
                /************************************************************
                ** 函数说明: 时间同步提示
                ** 作者: xiaoxiao
                ** 日期: 2023-06-28 10:45:40
                ** 参数说明:
                ** 注意事项:
                ************************************************************/
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), home_obj_id_slave_time_sync_failed, 68, 173, 346, 28,
                                                      NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0X303030,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(HOME_XLS_LANG_TIME_MISMATCH), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }

        {
                /************************************************************
                ** 函数说明: 网络图标创建
                ** 作者: xiaoxiao
                ** 日期: 2023-06-28 10:45:40
                ** 参数说明:
                ** 注意事项:
                ************************************************************/
                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_network_icon, 960, 12, 35, 35,
                                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("ic_system_network_on.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                if ((user_data_get()->system_mode & 0x0f) != 0x01)
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        {
                /************************************************************
                ** 函数说明: 门口机状态图标创建
                ** 作者: xiaoxiao
                ** 日期: 2023-06-28 10:45:40
                ** 参数说明:
                ** 注意事项:
                ************************************************************/
                lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_door_icon, 854, 15, 32, 32,
                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("ic_system_callcam_no.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: SD状态图标显示
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_sd_icon, 895, 5, 48, 48,
                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("ic_system_sdcard.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /************************************************************
        ** 函数说明:
        ** 作者: xiaoxiao
        ** 日期：2023-12-09 15:26:35
        ** 参数说明:
        ** 注意事项：
        ************************************************************/
        {
                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), home_obj_id_tuya_connect_icon, 895, 5, 48, 48,
                                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("ic_home_clood_connect.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                if ((user_data_get()->system_mode & 0x0f) != 0x01)
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        lv_timer_ready(lv_sat_timer_create(home_obj_top_icon_display_timer, 1000, NULL));

        home_media_thumb_display();

        linphone_incomming_node_release_all();

        if ((user_data_get()->system_mode & 0x0f) != 0x01)
        {

                lv_timer_set_repeat_count(lv_sat_timer_create(home_slave_sync_time_request_timer, 1000, NULL), 3);
        }

        if (user_data_get()->alarm.buzzer_alarm)
        {
                int time = user_timestamp_get() - buzzer_call_timestamp_get();
                if (time > 0 && time <= 6000)
                {
                        buzzer_call_duration_set(6000 - time);
                        buzzer_alarm_trigger_default();
                }
                else
                {
                        if ((user_data_get()->system_mode & 0x0f) == 0x01)
                        {
                                user_data_get()->alarm.buzzer_alarm = false;
                                user_data_save(true, true);
                        }
                }
        }
}

static void sat_layout_quit(home)
{
        buzzer_call_duration_set(6000);
        tuya_api_network_detect_enable(false);

        sat_linphone_media_thumb_destroy();

        thumb_display_refresh_register(NULL);
        sd_state_channge_callback_register(sd_state_change_default_callback);
        lv_img_buf_free(home_thumb_img_dsc);
        home_thumb_img_dsc = NULL;
}
sat_layout_create(home);