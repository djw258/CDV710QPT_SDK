#include "layout_setting_installation.h"
#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_single_operation_network.h"
#include "layout_ipc_camera.h"
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
};

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
        sat_layout_goto(setting_sensor_settings, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_installation_sensor_test_obj_click(lv_event_t *ev)
{
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
static void setting_installation_factory_reset_obj_click(lv_event_t *ev)
{
}
static lv_obj_t *setting_installation_sub_list_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 622, 72,
             setting_installation_obj_id_operating_structure_cont, 0, 1,
             SIGNLE_OPERATION_NETWORK_ID_LANG_OPERATION_STRCUUTRE, layout_single_operation_network_language_get,
             SIGNLE_OPERATION_NETWORK_ID_LANG_SINGLE, layout_single_operation_network_language_get,
             setting_installation_operating_structure_obj_click, -1},
            {0, 72, 622, 72,
             setting_installation_obj_id_building_house_no_cont, 0, 1,
             INSTALLATION_LANG_ID_BUILDING_HOUSE_NO, layout_setting_installation_language_get,
             -1, NULL,
             setting_installation_building_house_no_obj_click, -1},
            {0, 72 * 2, 622, 72,
             setting_installation_obj_id_ipaddres_cont, 0, 1,
             INSTALLATION_LANG_ID_IP_ADDRES, layout_setting_installation_language_get,
             INSTALLATION_LANG_ID_IP_ADDRES_GATEWAY_DNS_SETTING, layout_setting_installation_language_get,
             setting_installation_ipaddres_obj_click, -1},
            {0, 72 * 3, 622, 72,
             setting_installation_obj_id_operation_server_ip_addres_cont, 0, 1,
             INSTALLATION_LANG_ID_OPERATIONG_SERVER_IP_ADDRES, layout_setting_installation_language_get,
             INSTALLATION_LANG_ID_LOCAL_SERVER_SIP_SERVER_UPDATE, layout_setting_installation_language_get,
             setting_installation_operation_server_ip_obj_click, -1},
            {0, 72 * 4, 622, 72,
             setting_installation_obj_id_common_entrance_ip_cont, 0, 1,
             INSTALLATION_LANG_ID_COMMON_ENTRANCE_IP, layout_setting_installation_language_get,
             INSTALLATION_LANG_ID_COMMON_ENTRANCE_IP_FOR_INTERLOCKING, layout_setting_installation_language_get,
             setting_installation_common_entrance_ip_obj_click, -1},
            {0, 72 * 5, 622, 72,
             setting_installation_obj_id_guard_station_number_cont, 0, 1,
             INSTALLATION_LANG_ID_GUARD_STATION_NUMBER, layout_setting_installation_language_get,
             -1, NULL,
             setting_installation_guard_station_number_obj_click, -1},
            {0, 72 * 6, 622, 72,
             setting_installation_obj_id_sensor_cont, 0, 1,
             INSTALLATION_LANG_ID_SENSOR, layout_setting_installation_language_get,
             LANG_COMMON_ID_OFF, language_common_string_get,
             setting_installation_sensor_obj_click, -1},
            {0, 72 * 7, 622, 72,
             setting_installation_obj_id_sensor_test_cont, 0, 1,
             INSTALLATION_LANG_ID_SENSOR_TEST, layout_setting_installation_language_get,
             -1, NULL,
             setting_installation_sensor_test_obj_click, -1},
            {0, 72 * 8, 622, 72,
             setting_installation_obj_id_front_door_camera_cont, 0, 1,
             INSTALLATION_LANG_ID_FRONT_DOOR_CAMERA, layout_setting_installation_language_get,
             -1, NULL,
             setting_installation_front_door_camera_obj_click, -1},
            {0, 72 * 9, 622, 72,
             setting_installation_obj_id_front_cctv_cont, 0, 1,
             INSTALLATION_LANG_ID_FRONT_CCTV, layout_setting_installation_language_get,
             -1, NULL,
             setting_installation_front_cctv_obj_click, -1},
            {0, 72 * 10, 622, 72,
             setting_installation_obj_id_factory_reset_cont, 0, 1,
             INSTALLATION_LANG_ID_FACTORY_RESET, layout_setting_installation_language_get,
             -1, NULL,
             setting_installation_factory_reset_obj_click, -1},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_installation_obj_id_sub_list);
        lv_common_style_set_common(list, setting_installation_obj_id_sub_list, 354, 88, 622, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[i].x, main_list_group[i].y, main_list_group[i].w, main_list_group[i].h,
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
        }
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
                                      layout_setting_installation_language_get(INSTALLATION_LANG_ID_THE_MENU_IS_TO_OPERATE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
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
                                      layout_setting_installation_language_get(INSTALLATION_LANG_ID_WE_ARE_NOT_RESPONSIBLE), 0x808080, 0x808080, LV_TEXT_ALIGN_CENTER, lv_font_small);
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
                                              layout_setting_installation_language_get(INSTALLATION_LANG_ID_I_HAVE_READ_AND_UNDERSTOOD), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_small,
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
                                                      language_common_string_get(LANG_COMMON_ID_CONFIRM), 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_large);

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
            (sat_pre_layout_get() == sat_playout_get(ipc_camera_register)))
        {
                setting_installation_confirm_obj_click(NULL);
        }
}
static void sat_layout_quit(setting_installation)
{
}
sat_layout_create(setting_installation);