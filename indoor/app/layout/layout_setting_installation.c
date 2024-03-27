#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_ipc_camera.h"
#include "tuya/tuya_api.h"
#include "layout_setting_ipaddress.h"
enum
{
        setting_installation_obj_id_title,
        setting_installation_obj_id_cancel,
        setting_installation_obj_id_main_list,
        setting_installation_obj_id_this_menu_label,
        setting_installation_obj_id_we_are_not_label,
        setting_installation_obj_id_i_have_read_label,
        setting_installation_obj_id_i_have_read_checkbox,
        setting_installation_obj_id_confirm_btn,

        setting_installation_obj_id_sub_list,

        setting_installation_obj_id_operating_structure_cont,

        setting_installation_obj_id_building_house_no_cont,

        setting_installation_obj_id_ipaddres_cont,

        setting_installation_obj_id_operation_server_ip_addres_cont,

        setting_installation_obj_id_common_entrance_ip_cont,

        setting_installation_obj_id_guard_station_number_cont,

        setting_installation_obj_id_sensor_cont,

        setting_installation_obj_id_sensor_test_cont,

        setting_installation_obj_id_front_door_camera_cont,

        setting_installation_obj_id_front_cctv_cont,

        setting_installation_obj_id_factory_reset_cont,

        setting_installation_obj_id_factory_reset_msg_bg,

        setting_installation_obj_id_slave_register_cont,

};
typedef enum
{
        factory_reset_obj_id_msgbox,
        factory_reset_obj_id_title,
        factory_reset_obj_id_conrfirm,
        factory_reset_obj_id_cancel,

} factory_reset_msg_bg_obj_id;

static void setting_installation_operating_structure_obj_click(lv_event_t *ev)
{
        sat_layout_goto(operating_structure, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_installation_building_house_no_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_building_house_number, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_ipaddres_obj_click(lv_event_t *ev)
{

        layout_setting_ipaddress_info_get()->ip_setting_flag = 0x00;
        sat_layout_goto(setting_ipaddress, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_operation_server_ip_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_server_ipaddress, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_common_entrance_ip_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_common_entrance_ip, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_guard_station_number_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_guard_station_number, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_sensor_obj_click(lv_event_t *ev)
{
        sat_layout_goto(sensor_settings, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_installation_front_door_camera_obj_click(lv_event_t *ev)
{
        layout_ipc_cmeara_is_doorcamera_set(true);
        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_front_cctv_obj_click(lv_event_t *ev)
{
        layout_ipc_cmeara_is_doorcamera_set(false);
        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_installation_sensor_test_obj_click(lv_event_t *ev)
{
        sat_layout_goto(sensors_test, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_installation_factory_reset_obj_click(lv_event_t *ev)
{

        sat_layout_goto(factory_reset, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_installation_slave_indoor_register_display(lv_event_t *ev)
{
        sat_layout_goto(indoor_register, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void layout_setting_installation_open_structure_dispaly(lv_obj_t *list)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_installation_obj_id_operating_structure_cont), 1);
        if ((user_data_get()->system_mode & 0x0F) == 0x01)
        {
                // if ((user_data_get()->system_mode & 0xF0) == 0x00)
                // {
                //         lv_label_set_text(obj, lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_SINGLE));
                // }
                // else if ((user_data_get()->system_mode & 0xF0) == 0x10)
                // {
                //         lv_label_set_text(obj, lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_SERVER_SYSTEM));
                // }
                lv_label_set_text(obj, lang_str_get(SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_MASTER));
        }
        else
        {
                lv_label_set_text(obj, lang_str_get(SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SLAVE));
        }
}

static void layout_setting_installation_build_house_no_display(lv_obj_t *list)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(list, setting_installation_obj_id_building_house_no_cont);
        if (parent != NULL)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(parent, 1);

                // char building[8] = {0};
                // char household[8] = {0};
                // int loacal_number[8] = {0};

                // const char *username = network_data_get()->sip_user;

                // loacal_number[0] = ((username[3] - 48) * 100 + (username[4] - 48) * 10 + (username[5] - 48)) & 0x1F;

                // loacal_number[1] = (username[6] - 48) * 10000 + (username[7] - 48) * 1000 + (username[8] - 48) * 100 + (username[9] - 48) * 10 + (username[10] - 48);

                // sprintf(building, "%04d", loacal_number[0]);
                // sprintf(household, "%04d", loacal_number[1]);

                char building[8] = {0};
                char household[8] = {0};
                strncpy(building, &network_data_get()->sip_user[0], 4);
                strncpy(household, &network_data_get()->sip_user[4], 4);

                lv_label_set_text_fmt(obj, "%s-%s", building, household);
        }
}

static void layout_setting_installation_guard_no_display(lv_obj_t *list)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(list, setting_installation_obj_id_guard_station_number_cont);
        if (parent != NULL)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(parent, 1);

                lv_label_set_text(obj, network_data_get()->guard_number);
        }
}

static void layout_setting_installation_slave_display(lv_obj_t *list)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(list, setting_installation_obj_id_slave_register_cont);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, 0);

        lv_label_set_text(obj, "Slave registerd display");
}
static lv_obj_t *setting_installation_sub_list_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 622, 72,
             setting_installation_obj_id_operating_structure_cont, 0, 1,
             SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_OPERATION_STRCUUTRE, lang_str_get,
             SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_SINGLE, lang_str_get,
             setting_installation_operating_structure_obj_click, -1},
            {0, 72, 622, 72,
             setting_installation_obj_id_building_house_no_cont, 0, 1,
             INSTALLATION_XLS_LANG_ID_BUILDING_HOUSE_NO, lang_str_get,
             -1, NULL,
             setting_installation_building_house_no_obj_click, -1},
            {0, 72 * 2, 622, 72,
             setting_installation_obj_id_ipaddres_cont, 0, 1,
             INSTALLATION_XLS_LANG_ID_IP_ADDRES, lang_str_get,
             INSTALLATION_XLS_LANG_ID_IP_ADDRES_GATEWAY_DNS_SETTING, lang_str_get,
             setting_installation_ipaddres_obj_click, -1},
            {0, 72 * 3, 622, 72,
             setting_installation_obj_id_operation_server_ip_addres_cont, 0, 1,
             INSTALLATION_XLS_LANG_ID_OPERATIONG_SERVER_IP_ADDRES, lang_str_get,
             INSTALLATION_XLS_LANG_ID_LOCAL_SERVER_SIP_SERVER_UPDATE, lang_str_get,
             setting_installation_operation_server_ip_obj_click, -1},
            {0, 72 * 4, 622, 72,
             setting_installation_obj_id_common_entrance_ip_cont, 0, 1,
             INSTALLATION_XLS_LANG_ID_COMMON_ENTRANCE_IP, lang_str_get,
             INSTALLATION_XLS_LANG_ID_COMMON_ENTRANCE_IP_FOR_INTERLOCKING, lang_str_get,
             setting_installation_common_entrance_ip_obj_click, -1},
            {0, 72 * 5, 622, 72,
             setting_installation_obj_id_guard_station_number_cont, 0, 1,
             INSTALLATION_XLS_LANG_ID_GUARD_STATION_NUMBER, lang_str_get,
             -1, NULL,
             setting_installation_guard_station_number_obj_click, -1},
            {0, 72 * 6, 622, 72,
             setting_installation_obj_id_sensor_cont, 0, -1,
             INSTALLATION_XLS_LANG_ID_SENSOR, lang_str_get,
             RECORDING_XLS_LANG_ID_SAVE_OFF, lang_str_get,
             setting_installation_sensor_obj_click, -1},
            {0, 72 * 7, 622, 72,
             setting_installation_obj_id_sensor_test_cont, 0, -1,
             INSTALLATION_XLS_LANG_ID_SENSOR_TEST, lang_str_get,
             -1, NULL,
             setting_installation_sensor_test_obj_click, -1},
            {0, 72 * 8, 622, 72,
             setting_installation_obj_id_front_door_camera_cont, 0, -1,
             INSTALLATION_XLS_LANG_ID_FRONT_DOOR_CAMERA, lang_str_get,
             -1, NULL,
             setting_installation_front_door_camera_obj_click, -1},
            {0, 72 * 9, 622, 72,
             setting_installation_obj_id_front_cctv_cont, 0, -1,
             INSTALLATION_XLS_LANG_ID_IP_CCTV_CAMERA, lang_str_get,
             -1, NULL,
             setting_installation_front_cctv_obj_click, -1},
            {0, 72 * 10, 622, 72,
             setting_installation_obj_id_factory_reset_cont, 0, -1,
             INSTALLATION_XLS_LANG_ID_FACTORY_RESET, lang_str_get,
             -1, NULL,
             setting_installation_factory_reset_obj_click, -1},

            {0, 72 * 11, 622, 72,
             setting_installation_obj_id_slave_register_cont, 0, -1,
             INSTALLATION_XLS_LANG_ID_FACTORY_RESET, lang_str_get,
             -1, NULL,
             setting_installation_slave_indoor_register_display, -1},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_installation_obj_id_sub_list);
        lv_common_style_set_common(list, setting_installation_obj_id_sub_list, 354, 88, 622, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        int j = 0;

        char system_mode = user_data_get()->system_mode;

        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t) - 1; i++)
        {
                /*单系统*/
                if ((system_mode & 0x0f) != 0x01)
                {
                        if (i == 1 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9)
                        {
                                continue;
                        }
                }

                lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[i].w, main_list_group[i].h,
                                                                main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                0, 17, 576, 43, main_list_group[i].title_id,
                                                                main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 42, 576, 29, main_list_group[i].sub_id,
                                                                main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 0, 0, 0, -1,
                                                                NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                j++;
        }

        layout_setting_installation_open_structure_dispaly(list);
        layout_setting_installation_build_house_no_display(list);
        layout_setting_installation_guard_no_display(list);
        layout_setting_installation_slave_display(list);
        return list;
}

static void setting_installation_i_heave_read_obj_click(lv_event_t *ev)
{
        lv_obj_t *parent = lv_event_get_current_target(ev);
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, setting_installation_obj_id_i_have_read_checkbox);
        if (obj == NULL)
        {
                printf("not find obj \n");
                return;
        }
        if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) == 0)
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_confirm_btn);
                lv_obj_set_style_bg_color(obj, lv_color_hex(0x00a8ff), LV_PART_MAIN);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_confirm_btn);
                lv_obj_set_style_bg_color(obj, lv_color_hex(0x47494a), LV_PART_MAIN);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
}

static void setting_installation_confirm_obj_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_this_menu_label);
        if (obj != NULL)
        {
                lv_obj_del(obj);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_we_are_not_label);
        if (obj != NULL)
        {
                lv_obj_del(obj);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_i_have_read_label);
        if (obj != NULL)
        {
                lv_obj_del(obj);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_i_have_read_checkbox);
        if (obj != NULL)
        {
                lv_obj_del(obj);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_installation_obj_id_confirm_btn);
        if (obj != NULL)
        {
                lv_obj_del(obj);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 右边副列表
         ***********************************************/
        setting_installation_sub_list_create();
}
static void sat_layout_enter(setting_installation)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 左边主列表
        ***********************************************/
        setting_main_list_create(5);
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: themenu
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_installation_obj_id_this_menu_label, 327, 196, 675, 70,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(INSTALLATION_XLS_LANG_ID_THE_MENU_IS_TO_OPERATE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: themenu
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_installation_obj_id_we_are_not_label, 327, 290, 675, 29,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(INSTALLATION_XLS_LANG_ID_WE_ARE_NOT_RESPONSIBLE), 0x808080, 0x808080, LV_TEXT_ALIGN_CENTER, lv_font_small);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 14:16:18
        ** 说明: check
        ***********************************************/
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), setting_installation_obj_id_i_have_read_label, 433, 408, 463, 32,
                                              setting_installation_i_heave_read_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 3, 415, 32, setting_installation_obj_id_i_have_read_label,
                                              lang_str_get(INSTALLATION_XLS_LANG_ID_I_HAVE_READ_AND_UNDERSTOOD), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                              0, 0, 32, 32, setting_installation_obj_id_i_have_read_checkbox,
                                              (const char *)resource_ui_src_get("btn_checkbox_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: cofirm
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), setting_installation_obj_id_confirm_btn, 304, 528, 720, 72,
                                                      setting_installation_confirm_obj_click, LV_OPA_COVER, 0x47494a, LV_OPA_COVER, 0x47494a,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SETTING_COMPLETE_XLS_LANG_ID_CONFIRM), 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_large);

                lv_obj_set_style_pad_top(obj, 15, LV_PART_MAIN);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }

        if ((sat_pre_layout_get() == sat_playout_get(operating_structure)) ||
            (sat_pre_layout_get() == sat_playout_get(setting_building_house_number)) ||
            (sat_pre_layout_get() == sat_playout_get(setting_ipaddress)) ||
            (sat_pre_layout_get() == sat_playout_get(setting_server_ipaddress)) ||
            (sat_pre_layout_get() == sat_playout_get(setting_common_entrance_ip)) ||
            (sat_pre_layout_get() == sat_playout_get(setting_guard_station_number)) ||
            (sat_pre_layout_get() == sat_playout_get(setting_sensor_settings)) ||
            (sat_pre_layout_get() == sat_playout_get(ipc_camera_register)) ||
            (sat_pre_layout_get() == sat_playout_get(factory_reset)) ||
            (sat_pre_layout_get() == sat_playout_get(sensor_settings)) ||
            (sat_pre_layout_get() == sat_playout_get(sensors_test)))
        {
                setting_installation_confirm_obj_click(NULL);
        }
}
static void sat_layout_quit(setting_installation)
{
}
sat_layout_create(setting_installation);