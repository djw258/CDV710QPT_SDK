#include "layout_define.h"
#include "layout_setting_sensor_usage.h"
#include "layout_setting_general.h"

enum
{
    setting_sensor_settings_obj_id_title,

    setting_sensor_settings_obj_id_cancel,

    setting_sensor_settings_obj_id_list,

    setting_sensor_settings_obj_id_item1_cont,

    setting_sensor_settings_obj_id_item2_cont,

    setting_sensor_settings_obj_id_item3_cont,

    setting_sensor_settings_obj_id_item4_cont,

    setting_sensor_settings_obj_id_item5_cont,

    setting_sensor_settings_obj_id_item6_cont,

    setting_sensor_settings_obj_id_item7_cont,

    setting_sensor_settings_obj_id_item8_cont,

    sensor_setting_obj_id_msgbox_bg_cont,
};

static void setting_sensor_settings_cancel_obj_click(lv_event_t *e)
{
    sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_sensor_test_sensor_status_display(void)
{
    lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_sensor_settings_obj_id_list);
    for (int i = 0; i < 8; i++)
    {
        lv_obj_t *sensor_cont = lv_obj_get_child_form_id(list, setting_sensor_settings_obj_id_item1_cont + i);
        if (sensor_cont != NULL)
        {
            lv_obj_t *sensor_img = lv_obj_get_child_form_id(sensor_cont, 2);
            int value = user_sensor_value_get(i);
            if ((user_data_get()->alarm.alarm_enable[i] == 1 && value < ALM_LOW) || (user_data_get()->alarm.alarm_enable[i] == 2 && value > ALM_HIGHT))
            {

                lv_obj_set_style_bg_img_src(sensor_img, resource_ui_src_get("ic_undetect.png"), LV_PART_MAIN);
            }
            else
            {
                lv_obj_set_style_bg_img_src(sensor_img, resource_ui_src_get("ic_detect.png"), LV_PART_MAIN);
            }
        }
    }
}
static void layout_sensor_list_create(void)
{
    setting_list_info_t main_list_group[] = {
        {0, 0, 928, 72,
         setting_sensor_settings_obj_id_item1_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_1, lang_str_get,
         user_data_get()->alarm.alarm_enable[0] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[0] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[0] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
        {0, 72, 928, 72,
         setting_sensor_settings_obj_id_item2_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_2, lang_str_get,
         user_data_get()->alarm.alarm_enable[1] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[1] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[1] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
        {0, 72 * 2, 928, 72,
         setting_sensor_settings_obj_id_item3_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_3, lang_str_get,
         user_data_get()->alarm.alarm_enable[2] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[2] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[2] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
        {0, 72 * 3, 928, 72,
         setting_sensor_settings_obj_id_item4_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_4, lang_str_get,
         user_data_get()->alarm.alarm_enable[3] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[3] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[3] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
        {0, 72 * 4, 928, 72,
         setting_sensor_settings_obj_id_item5_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_5, lang_str_get,
         user_data_get()->alarm.alarm_enable[4] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[4] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[4] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
        {0, 72 * 5, 928, 72,
         setting_sensor_settings_obj_id_item6_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_6, lang_str_get,
         user_data_get()->alarm.alarm_enable[5] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[5] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[5] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
        {0, 72 * 6, 928, 72,
         setting_sensor_settings_obj_id_item7_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_7, lang_str_get,
         user_data_get()->alarm.alarm_enable[6] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[6] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[6] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         NULL, 2},
    };

    lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_sensor_settings_obj_id_list);
    lv_common_style_set_common(list, setting_sensor_settings_obj_id_list, 48, 88, 928, 504, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
    int j = 0; // 标志在第几个容器位置上显示数据
    for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
    {
        if (user_data_get()->alarm.alarm_enable[i] == 0)
        {
            continue;
            ;
        }
        lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[j].w, main_list_group[j].h,
                                                        main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                        80, 17, 576, 43, main_list_group[i].title_id,
                                                        main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                        80, 42, 576, 29, main_list_group[i].sub_id,
                                                        main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                        0, 0, 0, 0, -1,
                                                        NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                        0, 10, 80, 48, main_list_group[i].img_id,
                                                        resource_ui_src_get("ic_detect.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        j++;
    }
    layout_sensor_test_sensor_status_display();
}

/************************************************************
** 函数说明: 警报测试触发回调
** 作者: xiaoxiao
** 日期: 2023-06-15 17:16:45
** 参数说明:
** 注意事项:
************************************************************/
static void layout_sensor_test_trigger_func(int arg1, int arg2)
{
    layout_sensor_test_sensor_status_display();
}
static void sat_layout_enter(sensors_test)
{
    /***********************************************
    ** 作者: leo.liu
    ** 日期: 2023-2-2 13:46:56
    ** 说明: 标题显示
    ***********************************************/
    {
        lv_common_text_create(sat_cur_layout_screen_get(), setting_sensor_settings_obj_id_title, 0, 20, 1024, 40,
                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              lang_str_get(INSTALLATION_XLS_LANG_ID_SENSOR_TEST), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
    }

    /***********************************************
    ** 作者: leo.liu
    ** 日期: 2023-2-2 13:42:25
    ** 说明: 上一步
    ***********************************************/
    {
        lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_sensor_settings_obj_id_cancel, 35, 15, 48, 48,
                                 setting_sensor_settings_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
    }
    layout_sensor_list_create();
    alarm_sensor_cmd_register(layout_sensor_test_trigger_func);
}

static void sat_layout_quit(sensors_test)
{
    if (user_data_get()->alarm.away_alarm_enable)
    {
        extern void away_mode_alarm_trigger_callback(int arg1, int arg2);
        alarm_sensor_cmd_register(away_mode_alarm_trigger_callback); // 警报触发函数注册
    }
    else
    {
        alarm_sensor_cmd_register(layout_alarm_trigger_default); // 警报触发函数注册
    }
}

sat_layout_create(sensors_test);