#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_setting_time.h"
#include "common/language.h"
#include "layout_wifi_info.h"
enum
{

        setting_general_obj_id_title,
        setting_general_obj_id_cancel,

        setting_general_obj_id_main_list,

        setting_general_obj_id_main_general_cont,
        setting_general_obj_id_main_general_title,
        setting_general_obj_id_main_wifi_cont,
        setting_general_obj_id_main_wifi_titile,
        setting_general_obj_id_main_sound_cont,
        setting_general_obj_id_main_sound_title,
        setting_general_obj_id_main_screen_cont,
        setting_general_obj_id_main_screen_title,
        setting_general_obj_id_main_recording_monitoring_cont,
        setting_general_obj_id_main_recording_monitoring_title,
        setting_general_obj_id_main_installation_configuration_cont,
        setting_general_obj_id_main_installation_configuration_title,

        setting_general_obj_id_setting_list,

        setting_general_obj_id_data_and_time_count,
        setting_general_obj_id_data_and_time_title,
        setting_general_obj_id_data_and_time_sub,

        setting_general_obj_id_language_cont,
        setting_general_obj_id_language_title,
        setting_general_obj_id_language_sub,

        setting_general_obj_id_password_cont,
        setting_general_obj_id_password_title,
        setting_general_obj_id_password_sub,

        setting_general_obj_id_storage_space_cont,
        setting_general_obj_id_storage_space_title,

        setting_general_obj_id_door_opening_duration_cont,
        setting_general_obj_id_door_opening_duration_title,
        setting_general_obj_id_door_opening_duration_sub,

        setting_general_obj_id_door_opening_method_cont,
        setting_general_obj_id_door_opening_method_title,
        setting_general_obj_id_door_opening_method_sub,

        setting_general_obj_id_door_opener_module_cont,
        setting_general_obj_id_door_opener_module_title,
        setting_general_obj_id_door_opener_module_sub,

        setting_general_obj_id_door_opener_num_cont,
        setting_general_obj_id_door_opener_num_title,
        setting_general_obj_id_door_opener_num_sub,

        setting_general_obj_id_call_time_count,
        setting_general_obj_id_call_time_title,
        setting_general_obj_id_call_time_sub,

        setting_general_obj_id_sensor_usage_setting_cont,
        setting_general_obj_id_sensor_usage_setting_title,

        setting_general_obj_id_download_mobile_app_cont,
        setting_general_obj_id_doornload_mobile_app_title,

        setting_general_obj_id_app_integration_cont,
        setting_general_obj_id_app_integration_title,

        setting_general_obj_id_initialization_user_data_cont,
        setting_general_obj_id_initialization_user_data_title,

        setting_general_obj_id_version_information_cont,
        setting_general_obj_id_version_information_title,
        setting_general_obj_id_version_information_sub,

        setting_general_obj_id_outdoor_mac_register_cont,
        setting_general_obj_id_outdoor_mac_register_title,
        setting_general_obj_id_outdoor_mac_register_sub,

        setting_general_obj_id_msgbox_cont,
        setting_general_obj_id_msgbox_parent,
        setting_general_obj_id_msgbox_title,
        setting_general_obj_id_msgbox_list,
        setting_general_obj_id_msgbox_check_1,
        setting_general_obj_id_msgbox_check_1_text,
        setting_general_obj_id_msgbox_check_1_img,
        setting_general_obj_id_msgbox_check_2,
        setting_general_obj_id_msgbox_check_2_text,
        setting_general_obj_id_msgbox_check_2_img,
        setting_general_obj_id_msgbox_check_3,
        setting_general_obj_id_msgbox_check_3_text,
        setting_general_obj_id_msgbox_check_3_img,
        setting_general_obj_id_msgbox_confirm = setting_general_obj_id_msgbox_check_1 + 12,
        setting_general_obj_id_msgbox_confirm_img,
        setting_general_obj_id_msgbox_cancel,
        setting_general_obj_id_msgbox_cancel_img,
};

/************************************************************
** 函数说明: general setting列表获取
** 作者: xiaoxiao
** 日期: 2023-05-11 15:23:47
** 参数说明:
** 注意事项:
************************************************************/
static lv_obj_t *setting_general_list_item_sub_get(int cont, int sub_id)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_general_obj_id_setting_list);
        if (list == NULL)
        {
                return NULL;
        }

        lv_obj_t *item = lv_obj_get_child_form_id(list, cont);
        if (item == NULL)
        {
                return NULL;
        }

        lv_obj_t *sub = lv_obj_get_child_form_id(item, sub_id);
        if (sub == NULL)
        {
                return NULL;
        }

        return sub;
}
/************************************************************
** 函数说明: 时间设置模式次标题显示
** 作者: xiaoxiao
** 日期：2023-10-31 16:40:05
** 参数说明:
** 注意事项：
************************************************************/
static void time_setup_mode_sub_display(void)
{
        lv_obj_t *sub = setting_general_list_item_sub_get(setting_general_obj_id_data_and_time_count, setting_general_obj_id_data_and_time_sub);
        if (sub == NULL)
        {
                return;
        }

        lv_label_set_text(sub, user_data_get()->etc.time_automatically == 1 ? lang_str_get(SETTING_GENERAL_XLS_LANG_ID_AUTO_SETUP) : lang_str_get(SETTING_GENERAL_XLS_LANG_ID_MANUAL_SETUP));
}

/************************************************************
** 函数说明: door_open_method 次标题显示
** 作者: xiaoxiao
** 日期: 2023-05-11 15:19:23
** 参数说明:
** 注意事项:
************************************************************/
static void door_open_method_sub_display(void)
{
        lv_obj_t *sub = setting_general_list_item_sub_get(setting_general_obj_id_door_opening_method_cont, setting_general_obj_id_door_opening_method_sub);
        if (sub == NULL)
        {
                return;
        }

        lv_label_set_text(sub, user_data_get()->etc.open_the_door == 0 ? lang_str_get(SETTING_GENERAL_XLS_LANG_ID_OPEN_AFTER_CALL) : lang_str_get(SETTING_GENERAL_XLS_LANG_ID_OPEN_WHEN_CALL));
}

/************************************************************
** 函数说明: doo人open module 次标题显示
** 作者: xiaoxiao
** 日期: 2023-05-11 15:19:23
** 参数说明:
** 注意事项:
************************************************************/
static void door1_open_moudle_sub_display(void)
{
        lv_obj_t *sub = setting_general_list_item_sub_get(setting_general_obj_id_door_opener_module_cont, setting_general_obj_id_door_opener_module_sub);
        if (sub == NULL)
        {
                return;
        }
        lv_label_set_text(sub, user_data_get()->etc.door1_open_door_mode == 0 ? lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_CAMERA) : lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_LOCK));
}

/************************************************************
** 函数说明: door2 锁数量 次标题显示
** 作者: xiaoxiao
** 日期: 2023-05-11 15:19:23
** 参数说明:
** 注意事项:
************************************************************/
static void door2_open_lock_num_sub_display(void)
{
        lv_obj_t *sub = setting_general_list_item_sub_get(setting_general_obj_id_door_opener_num_cont, setting_general_obj_id_door_opener_num_sub);
        if (sub == NULL)
        {
                return;
        }
        lv_label_set_text(sub, user_data_get()->etc.door_lock_num != 0 ? lang_str_get(SETTING_GENERAL_XLS_LANG_ID_TWO_DOOR_OPEN_APPLY) : lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED));
}
static void setting_main_cancel_obj_click(lv_event_t *ev)
{
        sat_layout_goto(home, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, SAT_VOID);
}
static void setting_main_general_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_main_wifi_obj_click(lv_event_t *ev)
{
        layout_setting_user_wifi_display_mode_set(0);
        sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_main_sound_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_sound, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_main_screen_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_screen, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_main_recording_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_recording, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_main_installation_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}

static void setting_general_data_time_obj_click(lv_event_t *ev)
{
        setting_time_first_enter_set_flag(0x02);
        sat_layout_goto(setting_time, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_general_language_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_language, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_password_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_password, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_storage_space_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_storage_space, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_open_duration_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_open_duration_time, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_general_msgbox_del(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_general_obj_id_msgbox_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}
static lv_obj_t *setting_general_msgbox_create(const char *title, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb, const char *item[3], int n_item, int select_item)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_general_obj_id_msgbox_cont, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_general_obj_id_msgbox_parent, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, setting_general_obj_id_msgbox_title, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_obj_t *list = setting_list_create(msgbox, setting_general_obj_id_msgbox_list);
        lv_common_style_set_common(list, setting_general_obj_id_msgbox_list, 27, 73, 460 - 27, 343 - 57 - 73, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        if (n_item == 2)
        {
                lv_common_img_text_btn_create(list, setting_general_obj_id_msgbox_check_1, 48, 110, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_general_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_general_obj_id_msgbox_check_1_img,
                                              user_data_get()->etc.open_the_door == 0 ? (const char *)resource_ui_src_get("btn_radio_s.png") : (const char *)resource_ui_src_get("btn_radio_n.png"),
                                              LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(list, setting_general_obj_id_msgbox_check_2, 48, 166, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_general_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_general_obj_id_msgbox_check_2_img,
                                              user_data_get()->etc.open_the_door == 1 ? (const char *)resource_ui_src_get("btn_radio_s.png") : (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        else if (n_item == 3)
        {
                lv_common_img_text_btn_create(list, setting_general_obj_id_msgbox_check_1, 48, 89, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_general_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_general_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(list, setting_general_obj_id_msgbox_check_2, 48, 145, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_general_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_general_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(list, setting_general_obj_id_msgbox_check_3, 48, 201, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_general_obj_id_msgbox_check_3_text,
                                              item[2], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_general_obj_id_msgbox_check_3_img,
                                              (const char *)resource_ui_src_get("btn_radio_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        else
        {

                int j = 0;
                for (int i = 0; i < DEVICE_MAX; i++)
                {
                        if (monitor_valid_channel_check(i) == true)
                        {

                                lv_common_img_text_btn_create(list, setting_general_obj_id_msgbox_check_1 + i, 48, 61 + 56 * j, 365, 48,
                                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              48, 8, 365 - 94, 32, 0,
                                                              network_data_get()->door_device[i].door_name, 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                              0, 8, 32, 32, 1,
                                                              (const char *)resource_ui_src_get(user_data_get()->etc.door_lock_num & (0x01 << i) ? "btn_checkbox_s.png" : "btn_checkbox_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                                j++;
                        }
                }
        }

        lv_common_img_btn_create(msgbox, setting_general_obj_id_msgbox_cancel, 0, 281, 230, 62,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, setting_general_obj_id_msgbox_confirm, 230, 281, 230, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}
static void setting_general_msgbox_cancel_click(lv_event_t *e)
{
        setting_general_msgbox_del();
}
static void setting_general_msgbox_moethod_checkbox_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_event_get_current_target(e);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *check_obj, *discheck_obj;
        if (parent->id == setting_general_obj_id_msgbox_check_1)
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_2);
                discheck_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2_img);
        }
        else
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_1);
                discheck_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1_img);
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

/************************************************************
** 函数说明: 设置开锁模式（快模式或标准模式）
** 作者: xiaoxiao
** 日期: 2023-05-11 15:44:35
** 参数说明:
** 注意事项:
************************************************************/
static void setting_general_door1_opening_moethod_msgbox_confirm_click(lv_event_t *ev)
{
        lv_obj_t *item = lv_event_get_current_target(ev);
        lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_general_obj_id_msgbox_list);
        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_1), setting_general_obj_id_msgbox_check_1_img);
        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_2), setting_general_obj_id_msgbox_check_2_img);

        if (!strncmp((const char *)check1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->etc.open_the_door = 0;
        }
        else if (!strncmp((const char *)check2->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->etc.open_the_door = 1;
        }
        user_data_save(true, true);
        setting_general_msgbox_del();
        door_open_method_sub_display();
}

static void setting_general_door1_open_method_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_general_obj_id_msgbox_cont), setting_general_obj_id_msgbox_parent), setting_general_obj_id_msgbox_list);
        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1), setting_general_obj_id_msgbox_check_1_img);

        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2), setting_general_obj_id_msgbox_check_2_img);
        if (user_data_get()->etc.open_the_door == 0)
        {

                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
}

static void setting_general_door1_opening_moethod_obj_click(lv_event_t *ev)
{
        const char *item[2] = {0};
        item[0] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_OPEN_AFTER_CALL);
        item[1] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_OPEN_WHEN_CALL);
        setting_general_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENING_METHOD),
                                      setting_general_msgbox_cancel_click, setting_general_door1_opening_moethod_msgbox_confirm_click, setting_general_msgbox_moethod_checkbox_click,
                                      item, 2, -1);
        setting_general_door1_open_method_display();
}

/************************************************************
** 函数说明: door1开锁模块设置（开室内机的锁还是门口机的锁）
** 作者: xiaoxiao
** 日期: 2023-05-11 15:51:13
** 参数说明:
** 注意事项:
************************************************************/
static void setting_general_door1_opening_modoule_msgbox_confirm_click(lv_event_t *ev)
{
        lv_obj_t *item = lv_event_get_current_target(ev);
        lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_general_obj_id_msgbox_list);
        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_1), setting_general_obj_id_msgbox_check_1_img);
        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_2), setting_general_obj_id_msgbox_check_2_img);

        if (!strncmp((const char *)check1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->etc.door1_open_door_mode = 0;
        }
        else if (!strncmp((const char *)check2->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                if (user_data_get()->etc.door_lock_num & 0x01)
                {

                        lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_general_obj_id_msgbox_cont, setting_general_obj_id_msgbox_parent, 282, 143, 460, 283);
                        setting_msgdialog_msg_create(masgbox, setting_general_obj_id_msgbox_title, lang_str_get(SETTING_GENERAL_XLS_LANG_ID_TWO_DOOR_OPEN_CHANGE_DOOR_NUM), 0, 70, 460, 100, false);
                        setting_msgdialog_msg_confirm_btn_create(masgbox, setting_general_obj_id_msgbox_confirm, setting_general_msgbox_cancel_click);
                        return;
                }
                else
                {
                        user_data_get()->etc.door1_open_door_mode = 1;
                }
        }
        user_data_save(true, true);
        setting_general_msgbox_del();
        door1_open_moudle_sub_display();
}

static void setting_general_door1_open_module_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_general_obj_id_msgbox_cont), setting_general_obj_id_msgbox_parent), setting_general_obj_id_msgbox_list);

        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1), setting_general_obj_id_msgbox_check_1_img);

        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2), setting_general_obj_id_msgbox_check_2_img);
        if (user_data_get()->etc.door1_open_door_mode == 0)
        {

                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
}

static void setting_general_door_opener_module_obj_click(lv_event_t *ev)
{
        const char *item[2] = {0};
        item[0] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_CAMERA);
        item[1] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_LOCK);
        setting_general_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENER_MODULE),
                                      setting_general_msgbox_cancel_click, setting_general_door1_opening_modoule_msgbox_confirm_click, setting_general_msgbox_moethod_checkbox_click,
                                      item, 2, -1);
        setting_general_door1_open_module_display();
}

static void setting_general_msgbox_door_num_checkbox_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_event_get_current_target(e);
        if (parent == NULL)
        {
                return;
        }

        lv_obj_t *checkbox = lv_obj_get_child_form_id(parent, 1);
        if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) == 0)
        {

                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        }
}

static void setting_general_door_num_msgbox_confirm_click(lv_event_t *ev)
{
        lv_obj_t *item = lv_event_get_current_target(ev);
        lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_general_obj_id_msgbox_list);
        char temp = 0x00;
        for (int i = 0; i < DEVICE_MAX; i++)
        {
                item = lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_1 + i);
                if (item != NULL)
                {
                        lv_obj_t *checkbox = lv_obj_get_child_form_id(item, 1);
                        if (checkbox)
                        {
                                if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_s.png"), strlen(resource_ui_src_get("btn_checkbox_s.png"))) == 0)
                                {
                                        if ((i == 0) && (user_data_get()->etc.door1_open_door_mode == 1))
                                        {

                                                lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_general_obj_id_msgbox_cont, setting_general_obj_id_msgbox_parent, 282, 143, 460, 283);
                                                setting_msgdialog_msg_create(masgbox, setting_general_obj_id_msgbox_title, lang_str_get(SETTING_GENERAL_XLS_LANG_ID_TWO_DOOR_OPEN_CHANGE_DOOR_METHOD), 0, 40, 460, 200, false);
                                                setting_msgdialog_msg_confirm_btn_create(masgbox, setting_general_obj_id_msgbox_confirm, setting_general_msgbox_cancel_click);

                                                return;
                                        }
                                        else
                                        {
                                                temp |= (0X01 << i);
                                        }
                                }
                        }
                }
        }
        user_data_get()->etc.door_lock_num = temp;
        user_data_save(true, true);
        setting_general_msgbox_del();
        door2_open_lock_num_sub_display();
}

static void setting_general_door2_opener_num_obj_click(lv_event_t *e)
{

        setting_general_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_TWO_DOOR_OPEN_BUTTONS), setting_general_msgbox_cancel_click, setting_general_door_num_msgbox_confirm_click, setting_general_msgbox_door_num_checkbox_click,
                                      NULL, DEVICE_MAX, 0);
}

static void setting_general_call_time_msgbox_item_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_event_get_current_target(e);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *check_obj, *discheck1_obj, *discheck2_obj;
        if (parent->id == setting_general_obj_id_msgbox_check_1)
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_2);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_3);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_3_img);
        }
        else if (parent->id == setting_general_obj_id_msgbox_check_2)
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_1);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_3);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_3_img);
        }
        else
        {

                check_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_3_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_1);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_general_obj_id_msgbox_check_2);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2_img);
        }

        if ((check_obj == NULL) || (discheck1_obj == NULL) || (discheck2_obj == NULL) || (check_obj->bg_img_src == NULL) || (discheck1_obj->bg_img_src == NULL) || (discheck2_obj->bg_img_src == NULL))
        {
                return;
        }

        if (strncmp(check_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(discheck1_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(discheck2_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
}

/************************************************************
** 函数说明: door2 锁数量 次标题显示
** 作者: xiaoxiao
** 日期: 2023-05-11 15:19:23
** 参数说明:
** 注意事项:
************************************************************/
static void call_time_obj_sub_display(void)
{
        lv_obj_t *sub = setting_general_list_item_sub_get(setting_general_obj_id_call_time_count, setting_general_obj_id_call_time_sub);
        if (sub == NULL)
        {
                return;
        }

        lv_label_set_text(sub, lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_1_MINUTE + user_data_get()->etc.call_time - 1));
}
static void setting_general_call_time_msgbox_confirm_click(lv_event_t *ev)
{

        lv_obj_t *item = lv_event_get_current_target(ev);
        lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_general_obj_id_msgbox_list);
        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_1), setting_general_obj_id_msgbox_check_1_img);
        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_2), setting_general_obj_id_msgbox_check_2_img);
        lv_obj_t *check3 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_general_obj_id_msgbox_check_3), setting_general_obj_id_msgbox_check_3_img);
        if (!strncmp((const char *)check1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->etc.call_time = 1;
        }
        else if (!strncmp((const char *)check2->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->etc.call_time = 2;
        }
        else if (!strncmp((const char *)check3->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->etc.call_time = 3;
        }
        user_data_save(true, true);
        setting_general_msgbox_del();
        call_time_obj_sub_display();
}

static void setting_general_call_time_num_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_general_obj_id_msgbox_cont), setting_general_obj_id_msgbox_parent), setting_general_obj_id_msgbox_list);

        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_1), setting_general_obj_id_msgbox_check_1_img);

        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_2), setting_general_obj_id_msgbox_check_2_img);

        lv_obj_t *check3 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_general_obj_id_msgbox_check_3), setting_general_obj_id_msgbox_check_3_img);
        if (user_data_get()->etc.call_time == 1)
        {

                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else if (user_data_get()->etc.call_time == 2)
        {
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else if (user_data_get()->etc.call_time == 3)
        {
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        }
}
static void setting_general_call_time_obj_click(lv_event_t *ev)
{
        const char *item[3] = {0};
        item[0] = lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_1_MINUTE);
        item[1] = lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_2_MINUTE);
        item[2] = lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_3_MINUTE);
        setting_general_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_CALL_TIMER_OF_DOORCAMERA),
                                      setting_general_msgbox_cancel_click, setting_general_call_time_msgbox_confirm_click, setting_general_call_time_msgbox_item_click,
                                      item, 3, -1);
        setting_general_call_time_num_display();
}

static void setting_general_sensor_usage_setting_obj_click(lv_event_t *ev)
{
        if (user_data_get()->alarm.security_alarm_enable || user_data_get()->alarm.away_alarm_enable)
        {
                lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_general_obj_id_msgbox_cont, setting_general_obj_id_msgbox_parent, 282, 143, 460, 283);
                setting_msgdialog_msg_create(masgbox, setting_general_obj_id_msgbox_title, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_RUNNNG_TIPS), 0, 50, 460, 120, false);
                setting_msgdialog_msg_confirm_btn_create(masgbox, setting_general_obj_id_msgbox_confirm, setting_general_msgbox_cancel_click);
        }
        else
        {
                sat_layout_goto(setting_sensor_usage, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
}

static void setting_general_download_mobile_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_download, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_app_integration_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_app_integration, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_initialization_userdata_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_initialize, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_version_information_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_version_information, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_general_outdoor_mac_register_obj_click(lv_event_t *ev)
{
        sat_layout_goto(outdoor_mac_register, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_general_outdoor_mac_register_display(void)
{
        lv_obj_t *sub = setting_general_list_item_sub_get(setting_general_obj_id_outdoor_mac_register_cont, setting_general_obj_id_outdoor_mac_register_title);
        if (sub == NULL)
        {
                return;
        }

        lv_label_set_text(sub, "Outdoor Mac Register");
}
lv_obj_t *setting_list_create(lv_obj_t *parent, int id)
{
        lv_obj_t *list = lv_obj_get_child_form_id(parent, id);
        if (list != NULL)
        {
                return list;
        }
        list = lv_list_create(parent);
        return list;
}
lv_obj_t *setting_main_list_create(int id)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 标题显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_general_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_GENERAL_XLS_LANG_ID_SETTINGS), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_general_obj_id_cancel, 35, 15, 48, 48,
                                         setting_main_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        setting_list_info_t main_list_group[] = {
            {0, 0, 272, 88, setting_general_obj_id_main_general_cont, setting_general_obj_id_main_general_title, -1, SETTING_GENERAL_XLS_LANG_ID_GENERAL, lang_str_get, -1, NULL, setting_main_general_obj_click},
            {0, 88, 272, 88, setting_general_obj_id_main_wifi_cont, setting_general_obj_id_main_wifi_titile, -1, WIFI_SETTING_XLS_LANG_ID_WIFI, lang_str_get, -1, NULL, setting_main_wifi_obj_click},
            {0, 88 * 2, 272, 88, setting_general_obj_id_main_sound_cont, setting_general_obj_id_main_sound_title, -1, SETTING_GENERAL_XLS_LANG_ID_SOUND, lang_str_get, -1, NULL, setting_main_sound_obj_click},
            {0, 88 * 3, 272, 88, setting_general_obj_id_main_screen_cont, setting_general_obj_id_main_screen_title, -1, SETTING_GENERAL_XLS_LANG_ID_SCREEN, lang_str_get, -1, NULL, setting_main_screen_obj_click},
            {0, 88 * 4, 272, 88, setting_general_obj_id_main_recording_monitoring_cont, setting_general_obj_id_main_recording_monitoring_title, -1, SETTING_GENERAL_XLS_LANG_ID_RECORD_MONITORING, lang_str_get, -1, NULL, setting_main_recording_obj_click},
            {0, 88 * 5, 272, 88, setting_general_obj_id_main_installation_configuration_cont, setting_general_obj_id_main_installation_configuration_title, -1, SETTING_GENERAL_XLS_LANG_ID_INSTALLATION_CONFIGURATION, lang_str_get, -1, NULL, setting_main_installation_obj_click},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_general_obj_id_main_list);
        lv_common_style_set_common(list, setting_general_obj_id_main_list, 16, 88, 272, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        int j = 0;
        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                if ((user_data_get()->system_mode & 0x0F) != 0X01 && ((i == 1) /* || i == 4 */))
                {
                        continue;
                }

                lv_common_img_text_btn_create(list, main_list_group[i].cont_id, main_list_group[i].x, main_list_group[j].y, main_list_group[i].w, main_list_group[i].h,
                                              id == i ? NULL : main_list_group[i].click_cb, id == i ? LV_OPA_COVER : LV_OPA_TRANSP, 0x00A8FF, id == i ? LV_OPA_COVER : LV_OPA_TRANSP, 0x00A8FF,
                                              20, 8, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0,
                                              20, 8, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0,
                                              31, 0, 241, 70, main_list_group[i].title_id,
                                              main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xffffff, id == i ? 0xffffff : 0x00A8FF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                              0, 8, 32, 32, -1,
                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                j++;
        }

        return list;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 右边副列表
 ***********************************************/
static lv_obj_t *setting_sub_list_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 622, 72, setting_general_obj_id_data_and_time_count, setting_general_obj_id_data_and_time_title, setting_general_obj_id_data_and_time_sub, SETTING_GENERAL_XLS_LANG_ID_DATA_AND_TIME, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_AUTO_SETUP, lang_str_get, setting_general_data_time_obj_click},
            {0, 72, 622, 72, setting_general_obj_id_language_cont, setting_general_obj_id_language_title, setting_general_obj_id_language_sub, SETTING_GENERAL_XLS_LANG_ID_LANG, lang_str_get, XLS_LANG_ID_LANGUAGE_TYPE, lang_str_get, setting_general_language_obj_click},
            {0, 72 * 2, 622, 72, setting_general_obj_id_password_cont, setting_general_obj_id_password_title, setting_general_obj_id_password_sub, SETTING_GENERAL_XLS_LANG_ID_PASSWORD, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_CHANNGE_PASSWORD_FOR_SECURITY, lang_str_get, setting_general_password_obj_click},
            {0, 72 * 3, 622, 72, setting_general_obj_id_storage_space_cont, setting_general_obj_id_storage_space_title, -1, SETTING_GENERAL_XLS_LANG_ID_STORAGE_SPACE, lang_str_get, -1, NULL, setting_general_storage_space_obj_click},
            {0, 72 * 4, 622, 72, setting_general_obj_id_door_opening_duration_cont, setting_general_obj_id_door_opening_duration_title, setting_general_obj_id_door_opening_duration_sub, SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_DOOR_MODULE, lang_str_get, setting_general_open_duration_obj_click},
            {0, 72 * 5, 622, 72, setting_general_obj_id_door_opening_method_cont, setting_general_obj_id_door_opening_method_title, setting_general_obj_id_door_opening_method_sub, SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENING_METHOD, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_OPEN_AFTER_CALL, lang_str_get, setting_general_door1_opening_moethod_obj_click},
            {0, 72 * 6, 622, 72, setting_general_obj_id_door_opener_module_cont, setting_general_obj_id_door_opener_module_title, setting_general_obj_id_door_opener_module_sub, SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENER_MODULE, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_LOCK, lang_str_get, setting_general_door_opener_module_obj_click},
            {0, 72 * 7, 622, 72, setting_general_obj_id_door_opener_num_cont, setting_general_obj_id_door_opener_num_title, setting_general_obj_id_door_opener_num_sub, SETTING_GENERAL_XLS_LANG_ID_TWO_DOOR_OPEN_BUTTONS, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_TWO_DOOR_OPEN_APPLY, lang_str_get, setting_general_door2_opener_num_obj_click},
            {0, 72 * 8, 622, 72, setting_general_obj_id_call_time_count, setting_general_obj_id_call_time_title, setting_general_obj_id_call_time_sub, SETTING_GENERAL_XLS_LANG_ID_CALL_TIMER, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_1_MINUTE, lang_str_get, setting_general_call_time_obj_click},
            {0, 72 * 9, 622, 72, setting_general_obj_id_sensor_usage_setting_cont, setting_general_obj_id_sensor_usage_setting_title, -1, SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_USAGE_SETTINGS, lang_str_get, -1, NULL, setting_general_sensor_usage_setting_obj_click},
            {0, 72 * 10, 622, 72, setting_general_obj_id_download_mobile_app_cont, setting_general_obj_id_doornload_mobile_app_title, -1, SETTING_GENERAL_XLS_LANG_ID_DOWNLOAD_MOBILE_APP, lang_str_get, -1, NULL, setting_general_download_mobile_obj_click},
            {0, 72 * 11, 622, 72, setting_general_obj_id_app_integration_cont, setting_general_obj_id_app_integration_title, -1, SETTING_GENERAL_XLS_LANG_ID_APP_INTEGRATION, lang_str_get, -1, NULL, setting_general_app_integration_obj_click},
            {0, 72 * 12, 622, 72, setting_general_obj_id_initialization_user_data_cont, setting_general_obj_id_initialization_user_data_title, -1, SETTING_GENERAL_XLS_LANG_ID_INITIALIZATION_USER_DATA, lang_str_get, -1, NULL, setting_general_initialization_userdata_obj_click},
            {0, 72 * 13, 622, 72, setting_general_obj_id_version_information_cont, setting_general_obj_id_version_information_title, setting_general_obj_id_version_information_sub, SETTING_GENERAL_XLS_LANG_ID_VERSION_INFORMATION, lang_str_get, -1, NULL, setting_general_version_information_obj_click},
            {0, 72 * 14, 622, 72, setting_general_obj_id_outdoor_mac_register_cont, setting_general_obj_id_outdoor_mac_register_title, -1, SETTING_GENERAL_XLS_LANG_ID_VERSION_INFORMATION, lang_str_get, -1, NULL, setting_general_outdoor_mac_register_obj_click},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_general_obj_id_setting_list);
        lv_common_style_set_common(list, setting_general_obj_id_setting_list, 354, 88, 622, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        int j = 0;
        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                // 此处请重新判断
                if ((((user_data_get()->system_mode & 0x0f) != 0x01) && ((i == 0) || (i == 2) || (i == 4) || (i == 5) || (i == 6) || (i == 7) || (i == 8) || (i == 9) || (i == 11))))
                {
                        continue;
                }
                if (i == 14)
                {
                        if (outdoor_mac_txt_exist_check() == false)
                        {
                                continue;
                        }
                }

                lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[j].w, main_list_group[j].h,
                                                                main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                0, 17, 576, 43, main_list_group[i].title_id,
                                                                main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 42, 576, 29, main_list_group[i].sub_id,
                                                                main_list_group[i].cont_id == setting_general_obj_id_version_information_cont ? SYSTEM_VERSION : main_list_group[i].sub_language_id == -1 ? NULL
                                                                                                                                                                                                          : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id),
                                                                0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 0, 0, 0, -1,
                                                                NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                j++;
        }
        time_setup_mode_sub_display();
        door_open_method_sub_display();
        door1_open_moudle_sub_display();
        door2_open_lock_num_sub_display();
        call_time_obj_sub_display();
        setting_general_outdoor_mac_register_display();
        return list;
}

static void sat_layout_enter(setting_general)
{

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 左边主列表
        ***********************************************/
        setting_main_list_create(0);
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 右边副列表
        ***********************************************/
        setting_sub_list_create();
}
static void sat_layout_quit(setting_general)
{
}

sat_layout_create(setting_general);