#include "layout_define.h"
enum
{
        emergency_occupy_obj_id_bg,
        emergency_occupy_obj_id_back,
        emergency_occupy_obj_id_bell,
        emergency_occupy_obj_id_log,
        emergency_occupy_obj_id_title,
        emergency_occupy_obj_id_tips,
        emergency_occupy_obj_id_confirm_btn,
        emergency_occupy_obj_id_confirm_label,

};
static void layout_emergeency_back_obj_click(lv_event_t *ev)
{
        sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void emergency_setting_confirm_obj_click(lv_event_t *ev)
{
        main_sync_lock_set(true);
        layout_common_call_log(emergency_occur, 7);
        asterisk_server_alarm_log_force(true);
        main_sync_lock_set(false);
        user_data_get()->alarm.emergency_mode = 0;
        if (user_data_get()->alarm.away_auto_record)
                record_jpeg_start(REC_MODE_AUTO);
        layout_alarm_alarm_channel_set(7);
        user_data_get()->alarm.alarm_trigger[7] = true;

        user_data_get()->alarm.is_alarm_return = false;
        user_data_get()->alarm.alarm_ring_play = true;
        user_data_save(true, true);

        if ((user_data_get()->system_mode & 0x0f) != 0x01)
        {
                sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
        }
        // char buffer[512];
        // tuya_api_alarm_event(0, buffer, 512);
        sat_layout_goto(alarm, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void emergency_setting_list_obj_click(lv_event_t *ev)
{
        sat_layout_goto(alarm_list, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void sat_layout_enter(emergency_setting)
{

        /************************************************************
        ** 函数说明: 背景创建
        ** 作者: xiaoxiao
        ** 日期: 2023-04-25 23:12:18
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_disp_set_bg_image(lv_disp_get_default(), resource_wallpaper_src_get("bg_emergency_occur04.jpg", 1024, 600));
        }

        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), emergency_occupy_obj_id_back, 35, 15, 48, 48,
                                         layout_emergeency_back_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_close.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /************************************************************
        ** 函数说明: emergency title
        ** 作者: xiaoxiao
        ** 日期: 2023-04-25 23:35:58
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), emergency_occupy_obj_id_title, 372, 333, 281, 78,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(HOME_XLS_LANG_ID_EMERGENCY), 0XE40A00, 0XE40A00, LV_TEXT_ALIGN_CENTER, lv_font_large_s);
        }

        /************************************************************
        ** 函数说明: emergency sub_title
        ** 作者: xiaoxiao
        ** 日期: 2023-04-25 23:35:58
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), emergency_occupy_obj_id_tips, 256, 439, 512, 78,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(LAYOUT_EMERGENCY_XLS_LANG_ID_ACTIVE_EMERGENCY), 0XE40A00, 0XE40A00, LV_TEXT_ALIGN_CENTER, lv_font_normal);
        }

        /************************************************************
        ** 函数说明: emergency log
        ** 作者: xiaoxiao
        ** 日期: 2023-04-25 23:35:58
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), emergency_occupy_obj_id_log, 960, 16, 48, 48,
                                         emergency_setting_list_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_emergencylist.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /************************************************************
        ** 函数说明:
        ** 作者: xiaoxiao
        ** 日期: 2023-04-25 22:10:54
        ** 参数说明:
        ** 注意事项: 警报图标显示
        ************************************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), emergency_occupy_obj_id_bell, 392, 70, 240, 240,

                                         NULL, false, LV_OPA_COVER, 0xff5951, LV_OPA_COVER, 0,
                                         180, 30, LV_BORDER_SIDE_FULL, LV_OPA_40, 0xff90bd,

                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("ic_emergency_bell.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /************************************************************
        ** 函数说明: 确认按键创建
        ** 作者: xiaoxiao
        ** 日期: 2023-04-25 22:43:38
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), emergency_occupy_obj_id_confirm_btn, 0, 528, 1024, 72,
                                              emergency_setting_confirm_obj_click, LV_OPA_COVER, 0xFF6B64, LV_OPA_COVER, 0xFF6B64,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              400, 17, 224, 27, emergency_occupy_obj_id_confirm_label,
                                              lang_str_get(LAYOUT_EMERGENCY_XLS_LANG_ID_CONFIRM), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                              3, 0, 77, 77, -1,
                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
}

static void sat_layout_quit(emergency_setting)
{
        lv_disp_set_bg_image(lv_disp_get_default(), NULL);
}

sat_layout_create(emergency_setting);