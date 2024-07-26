#include "layout_define.h"
#include "layout_wifi_info.h"
enum
{
        server_operation_network_obj_id_title,
        server_operation_network_obj_id_cancel,
        server_operation_network_obj_id_next,

        server_operation_network_obj_id_setting_cont,
        server_operation_network_obj_id_setting_title,
        server_operation_network_obj_id_setting_sub,
        server_operation_network_obj_id_setting_img,

        server_operation_network_obj_id_keyboard,
        server_operation_network_obj_id_ver_line,

        server_operation_network_obj_id_list,

        server_operation_network_obj_id_building_house_number_title,

        server_operation_network_obj_id_building_number_cont,
        server_operation_network_obj_id_building_number_text,
        server_operation_network_obj_id_building_number_textarea,

        server_operation_network_obj_id_building_household_number_cont,
        server_operation_network_obj_id_building_household_number_text,
        server_operation_network_obj_id_building_household_number_textarea,

        server_operation_network_obj_id_product_ip_title,

        server_operation_network_obj_id_product_ip_cont,
        server_operation_network_obj_id_product_ip_text,
        server_operation_network_obj_id_product_ip_textarea,

        server_operation_network_obj_id_gateway_cont,
        server_operation_network_obj_id_gateway_text,
        server_operation_network_obj_id_gateway_textarea,

        server_operation_network_obj_id_mask_cont,
        server_operation_network_obj_id_mask_text,
        server_operation_network_obj_id_mask_textarea,

        server_operation_network_obj_id_dns_cont,
        server_operation_network_obj_id_dns_text,
        server_operation_network_obj_id_dns_textarea,

        server_operation_network_obj_id_operating_server_title,

        server_operation_network_obj_id_local_server_cont,
        server_operation_network_obj_id_local_server_text,
        server_operation_network_obj_id_local_server_textarea,

        server_operation_network_obj_id_sip_server_cont,
        server_operation_network_obj_id_sip_server_text,
        server_operation_network_obj_id_sip_server_textarea,

        server_operation_network_obj_id_update_server_cont,
        server_operation_network_obj_id_update_server_text,
        server_operation_network_obj_id_update_server_textarea,

        server_operation_network_obj_id_cctv_server_cont,
        server_operation_network_obj_id_cctv_server_text,
        server_operation_network_obj_id_cctv_server_textarea,

        server_operation_network_obj_id_guard_station_title,

        server_operation_network_obj_id_guard_statioon_cont,
        server_operation_network_obj_id_guard_statioon_text,
        server_operation_network_obj_id_guard_statioon_textarea,

        server_operation_network_obj_id_msg_bg,
};

typedef enum
{
        server_operation_network_obj_id_msg,
        server_operation_network_obj_id_text,
        server_operation_network_obj_id_confirm,
} server_operation_network_msg_bg_obj_id;

static bool setting_server_operation_data_valid_check(void)
{
        int obj_id[][2] = {
            {server_operation_network_obj_id_building_number_cont, server_operation_network_obj_id_building_number_textarea},
            {server_operation_network_obj_id_building_household_number_cont, server_operation_network_obj_id_building_household_number_textarea},
            {server_operation_network_obj_id_product_ip_cont, server_operation_network_obj_id_product_ip_textarea},
            {server_operation_network_obj_id_gateway_cont, server_operation_network_obj_id_gateway_textarea},
            {server_operation_network_obj_id_mask_cont, server_operation_network_obj_id_mask_textarea},
            {server_operation_network_obj_id_dns_cont, server_operation_network_obj_id_dns_textarea},
            {server_operation_network_obj_id_local_server_cont, server_operation_network_obj_id_local_server_textarea},
            {server_operation_network_obj_id_sip_server_cont, server_operation_network_obj_id_sip_server_textarea},
            {server_operation_network_obj_id_cctv_server_cont, server_operation_network_obj_id_cctv_server_textarea},
            {server_operation_network_obj_id_guard_statioon_cont, server_operation_network_obj_id_guard_statioon_textarea}};

        lv_obj_t *textarea = NULL;

        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_list);

        int total = sizeof(obj_id) / (sizeof(int) * 2);

        for (int i = 0; i < total; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(list, obj_id[i][0]);
                textarea = lv_obj_get_child_form_id(parent, obj_id[i][1]);

                if (textarea != NULL)
                {
                        if (i == 0)
                        {
                                const char *building_str = lv_textarea_get_text(textarea);
                                // int building = 0;
                                // building = atoi(building_str);
                                int len = strlen(building_str);
                                if ((len == 0))
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 1)
                        {
                                const char *household_str = lv_textarea_get_text(textarea);
                                int len = strlen(household_str);
                                if ((len == 0))
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 2)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 3)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 4)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 5)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 6)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 7)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 8)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 9)
                        {
                                int len = strlen(lv_textarea_get_text(textarea));
                                if ((len < 1) || (len > 11))
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                                for (int i = 0; i < 8; i++)
                                {
                                        if (((lv_textarea_get_text(textarea)[i] - 48) < 0) || ((lv_textarea_get_text(textarea)[i] - 48) > 9))
                                        {
                                                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                                return false;
                                        }
                                }
                        }
                }
        }
        return true;
}

static void setting_setver_operation_network_data_save(void)
{
        int obj_id[][2] = {
            {server_operation_network_obj_id_building_number_cont, server_operation_network_obj_id_building_number_textarea},
            {server_operation_network_obj_id_building_household_number_cont, server_operation_network_obj_id_building_household_number_textarea},
            {server_operation_network_obj_id_product_ip_cont, server_operation_network_obj_id_product_ip_textarea},
            {server_operation_network_obj_id_gateway_cont, server_operation_network_obj_id_gateway_textarea},
            {server_operation_network_obj_id_mask_cont, server_operation_network_obj_id_mask_textarea},
            {server_operation_network_obj_id_dns_cont, server_operation_network_obj_id_dns_textarea},
            {server_operation_network_obj_id_local_server_cont, server_operation_network_obj_id_local_server_textarea},
            {server_operation_network_obj_id_sip_server_cont, server_operation_network_obj_id_sip_server_textarea},
            {server_operation_network_obj_id_cctv_server_cont, server_operation_network_obj_id_cctv_server_textarea},
            {server_operation_network_obj_id_guard_statioon_cont, server_operation_network_obj_id_guard_statioon_textarea}};

        lv_obj_t *textarea = NULL;

        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_list);

        int total = sizeof(obj_id) / (sizeof(int) * 2);
        for (int i = 0; i < total; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(list, obj_id[i][0]);
                textarea = lv_obj_get_child_form_id(parent, obj_id[i][1]);

                if (textarea != NULL)
                {
                        if (i == 0)
                        {
                                memset(&network_data_get()->sip_user[0], '0', 4);
                                const char *building_str = lv_textarea_get_text(textarea);
                                strncpy(&network_data_get()->sip_user[4 - strlen(building_str)], building_str, strlen(building_str));
                        }
                        else if (i == 1)
                        {
                                memset(&network_data_get()->sip_user[4], '0', 4);
                                const char *household_str = lv_textarea_get_text(textarea);
                                strncpy(&network_data_get()->sip_user[8 - strlen(household_str)], household_str, strlen(household_str));
                        }
                        else if (i == 2)
                        {
                                strncpy(network_data_get()->network.ipaddr, lv_textarea_get_text(textarea), sizeof(network_data_get()->network.ipaddr));
                        }
                        else if (i == 3)
                        {
                                strncpy(network_data_get()->network.gateway, lv_textarea_get_text(textarea), sizeof(network_data_get()->network.gateway));
                        }
                        else if (i == 4)
                        {
                                strncpy(network_data_get()->network.mask, lv_textarea_get_text(textarea), sizeof(network_data_get()->network.mask));
                        }
                        else if (i == 5)
                        {
                                strncpy(network_data_get()->network.dns, lv_textarea_get_text(textarea), sizeof(network_data_get()->network.dns));
                        }
                        else if (i == 6)
                        {
                                strncpy(network_data_get()->local_server, lv_textarea_get_text(textarea), sizeof(network_data_get()->local_server));
                        }
                        else if (i == 7)
                        {
                                strncpy(network_data_get()->sip_server, lv_textarea_get_text(textarea), sizeof(network_data_get()->sip_server));
                        }
                        else if (i == 8)
                        {
                                strncpy(network_data_get()->cctv_server, lv_textarea_get_text(textarea), sizeof(network_data_get()->cctv_server));
                        }
                        else if (i == 9)
                        {
                                strncpy(network_data_get()->guard_number, lv_textarea_get_text(textarea), sizeof(network_data_get()->guard_number));
                        }
                }
        }
        user_data_save(true, true);
}

static void setting_server_operation_network_falid_confirm(lv_event_t *e)
{

        setting_msgdialog_msg_del(server_operation_network_obj_id_msg_bg);
}

/************************************************************
** 函数说明: 数据校验失败提示
** 作者: xiaoxiao
** 日期：2023-09-26 14:23:59
** 参数说明:
** 注意事项：
************************************************************/
static void setting_server_operation_network_falid_tips(void)
{
        lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_msg_bg);
        if (masgbox != NULL)
        {
                setting_msgdialog_msg_del(server_operation_network_obj_id_msg_bg);
        }
        masgbox = setting_msgdialog_msg_bg_create(server_operation_network_obj_id_msg_bg, server_operation_network_obj_id_msg, 282, 143, 460, 283);
        setting_msgdialog_msg_create(masgbox, server_operation_network_obj_id_text, lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_ENTER_FORMAT), 0, 60, 460, 120, false);
        setting_msgdialog_msg_confirm_btn_create(masgbox, server_operation_network_obj_id_confirm, setting_server_operation_network_falid_confirm);
}

static void setting_server_operation_network_cancel_btn_click(lv_event_t *e)
{
        if (setting_server_operation_data_valid_check())
        {
                setting_setver_operation_network_data_save();
                sat_layout_goto(power_setting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                setting_server_operation_network_falid_tips();
        }
}

static void setting_server_operation_network_init(void)
{
        int obj_id[][2] = {
            {server_operation_network_obj_id_building_number_cont, server_operation_network_obj_id_building_number_textarea},
            {server_operation_network_obj_id_building_household_number_cont, server_operation_network_obj_id_building_household_number_textarea},
            {server_operation_network_obj_id_product_ip_cont, server_operation_network_obj_id_product_ip_textarea},
            {server_operation_network_obj_id_gateway_cont, server_operation_network_obj_id_gateway_textarea},
            {server_operation_network_obj_id_mask_cont, server_operation_network_obj_id_mask_textarea},
            {server_operation_network_obj_id_dns_cont, server_operation_network_obj_id_dns_textarea},
            {server_operation_network_obj_id_local_server_cont, server_operation_network_obj_id_local_server_textarea},
            {server_operation_network_obj_id_sip_server_cont, server_operation_network_obj_id_sip_server_textarea},
            {server_operation_network_obj_id_cctv_server_cont, server_operation_network_obj_id_cctv_server_textarea},
            {server_operation_network_obj_id_guard_statioon_cont, server_operation_network_obj_id_guard_statioon_textarea}};

        lv_obj_t *textarea = NULL;

        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_list);

        int total = sizeof(obj_id) / (sizeof(int) * 2);
        char building[8] = {0};
        char household[8] = {0};
        strncpy(building, &network_data_get()->sip_user[0], 4);
        strncpy(household, &network_data_get()->sip_user[4], 4);
        for (int i = 0; i < total; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(list, obj_id[i][0]);
                textarea = lv_obj_get_child_form_id(parent, obj_id[i][1]);

                if (textarea != NULL)
                {
                        if (i == 0)
                        {
                                lv_textarea_set_text(textarea, building);
                        }
                        else if (i == 1)
                        {
                                lv_textarea_set_text(textarea, household);
                        }
                        else if (i == 2)
                        {
                                char ip[32] = {0};
                                char mask[32] = {0};
                                if (network_data_get()->network.ipaddr[0] != 0)
                                {
                                        lv_textarea_set_text(textarea, network_data_get()->network.ipaddr);
                                }
                                else
                                {
                                        sat_ip_mac_addres_get("eth0", ip, NULL, mask);
                                        lv_textarea_set_text(textarea, ip);
                                }
                        }
                        else if (i == 3)
                        {
                                lv_textarea_set_text(textarea, network_data_get()->network.gateway);
                        }
                        else if (i == 4)
                        {
                                char ip[32] = {0};
                                char mask[32] = {0};
                                if (network_data_get()->network.ipaddr[0] != 0)
                                {
                                        lv_textarea_set_text(textarea, network_data_get()->network.mask);
                                }
                                else
                                {
                                        sat_ip_mac_addres_get("eth0", ip, NULL, mask);
                                        lv_textarea_set_text(textarea, mask);
                                }
                        }
                        else if (i == 5)
                        {
                                lv_textarea_set_text(textarea, network_data_get()->network.dns);
                        }
                        else if (i == 6)
                        {
                                lv_textarea_set_text(textarea, network_data_get()->local_server);
                        }
                        else if (i == 7)
                        {
                                lv_textarea_set_text(textarea, network_data_get()->sip_server);
                        }
                        else if (i == 8)
                        {
                                lv_textarea_set_text(textarea, network_data_get()->cctv_server);
                        }
                        else if (i == 9)
                        {
                                lv_textarea_set_text(textarea, network_data_get()->guard_number);
                        }
                }
        }
}

static void setting_server_operation_network_next_btn_click(lv_event_t *e)
{

        if (setting_server_operation_data_valid_check())
        {
                setting_setver_operation_network_data_save();
                layout_setting_user_wifi_display_mode_set(1);
                sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                setting_server_operation_network_falid_tips();
        }
}

static void setting_server_operating_btn_click(lv_event_t *e)
{
        if (setting_server_operation_data_valid_check())
        {
                setting_setver_operation_network_data_save();
                sat_layout_goto(operating_structure, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                setting_server_operation_network_falid_tips();
        }
}
static lv_obj_t *setting_server_textarea_focused_get(void)
{

        int obj_id[][2] = {
            {server_operation_network_obj_id_building_number_cont, server_operation_network_obj_id_building_number_textarea},
            {server_operation_network_obj_id_building_household_number_cont, server_operation_network_obj_id_building_household_number_textarea},
            {server_operation_network_obj_id_product_ip_cont, server_operation_network_obj_id_product_ip_textarea},
            {server_operation_network_obj_id_gateway_cont, server_operation_network_obj_id_gateway_textarea},
            {server_operation_network_obj_id_mask_cont, server_operation_network_obj_id_mask_textarea},
            {server_operation_network_obj_id_dns_cont, server_operation_network_obj_id_dns_textarea},
            {server_operation_network_obj_id_local_server_cont, server_operation_network_obj_id_local_server_textarea},
            {server_operation_network_obj_id_sip_server_cont, server_operation_network_obj_id_sip_server_textarea},
            {server_operation_network_obj_id_cctv_server_cont, server_operation_network_obj_id_cctv_server_textarea},
            {server_operation_network_obj_id_guard_statioon_cont, server_operation_network_obj_id_guard_statioon_textarea}};

        lv_obj_t *textarea = NULL;

        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_list);

        int total = sizeof(obj_id) / (sizeof(int) * 2);
        for (int i = 0; i < total; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(list, obj_id[i][0]);
                textarea = lv_obj_get_child_form_id(parent, obj_id[i][1]);

                if ((textarea != NULL) && (lv_obj_get_state(textarea) & LV_STATE_FOCUSED))
                {

                        return textarea;
                }
        }

        return textarea;
}

static bool setting_server_textbox_del(void)
{
        lv_obj_t *textarea = setting_server_textarea_focused_get();

        if (textarea == NULL)
        {
                return false;
        }

        lv_textarea_del_char(textarea);

        return true;
}

static bool setting_server_textbox_add(const char *string)
{
        lv_obj_t *textarea = setting_server_textarea_focused_get();

        if (textarea == NULL)
        {

                return false;
        }

        lv_textarea_add_text(textarea, string);
        return true;
}

static void setting_server_number_keyboard_btn_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_target(e);
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);

        if (id == 11)
        {
                setting_server_textbox_del();
        }
        else
        {
                const char *text = lv_btnmatrix_get_btn_text(obj, id);

                if (text != NULL)
                {

                        setting_server_textbox_add(text);
                }
        }
}

static lv_obj_t *server_operating_network_list_create(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_list);
        if (list != NULL)
        {
                return list;
        }
        list = lv_list_create(sat_cur_layout_screen_get());
        lv_common_style_set_common(list, server_operation_network_obj_id_list, 0, 168, 584, 432, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        return list;
}

/************************************************************
** 函数说明: 在进行下一步的时候，会校验数据，数据不合法，输入会停留在第一个不合法的文本区域，需要把不合法的文本区域输入状态取消，变更到最新一次点击的文本区域去
** 作者: xiaoxiao
** 日期：2023-09-26 16:52:11
** 参数说明:
** 注意事项：
************************************************************/
static void server_operating_network_textarea_focus_state_clear(lv_event_t *ev)
{
        lv_obj_t *cur_textarea = lv_event_get_current_target(ev);
        int obj_id[][2] = {
            {server_operation_network_obj_id_building_number_cont, server_operation_network_obj_id_building_number_textarea},
            {server_operation_network_obj_id_building_household_number_cont, server_operation_network_obj_id_building_household_number_textarea},
            {server_operation_network_obj_id_product_ip_cont, server_operation_network_obj_id_product_ip_textarea},
            {server_operation_network_obj_id_gateway_cont, server_operation_network_obj_id_gateway_textarea},
            {server_operation_network_obj_id_mask_cont, server_operation_network_obj_id_mask_textarea},
            {server_operation_network_obj_id_dns_cont, server_operation_network_obj_id_dns_textarea},
            {server_operation_network_obj_id_local_server_cont, server_operation_network_obj_id_local_server_textarea},
            {server_operation_network_obj_id_sip_server_cont, server_operation_network_obj_id_sip_server_textarea},
            {server_operation_network_obj_id_cctv_server_cont, server_operation_network_obj_id_cctv_server_textarea},
            {server_operation_network_obj_id_guard_statioon_cont, server_operation_network_obj_id_guard_statioon_textarea}};

        lv_obj_t *textarea = NULL;

        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), server_operation_network_obj_id_list);

        int total = sizeof(obj_id) / (sizeof(int) * 2);
        for (int i = 0; i < total; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(list, obj_id[i][0]);
                textarea = lv_obj_get_child_form_id(parent, obj_id[i][1]);

                if ((textarea != NULL))
                {

                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                }
        }
        lv_obj_add_state(cur_textarea, LV_STATE_FOCUSED);
}
static void sat_layout_enter(server_operation_network)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), server_operation_network_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_OPERATION_NETWORK), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 上一步下一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), server_operation_network_obj_id_cancel, 35, 15, 48, 48,
                                         setting_server_operation_network_cancel_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_btn_create(sat_cur_layout_screen_get(), server_operation_network_obj_id_next, 952, 15, 48, 48,
                                         setting_server_operation_network_next_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_next.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:50
        ** 说明: 设置按钮创建
        ***********************************************/
        {
                lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), server_operation_network_obj_id_setting_cont, 48, 80, 928, 86,
                                                                setting_server_operating_btn_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                0, 8, 838, 50, server_operation_network_obj_id_setting_title,
                                                                lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_OPERATION_STRCUUTRE), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 45, 838, 50, server_operation_network_obj_id_setting_sub,
                                                                lang_str_get((user_data_get()->system_mode & 0x0f) == 0x01 ? SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_MASTER : SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SLAVE), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                880, 20, 48, 48, server_operation_network_obj_id_setting_img,
                                                                resource_ui_src_get("ic_list_setting.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:18
        ** 说明: 数字键盘创建
        ***********************************************/
        {
                lv_common_number_input_keyboard_create(sat_cur_layout_screen_get(), server_operation_network_obj_id_keyboard, 648, 184, 312, 402,
                                                       setting_server_number_keyboard_btn_click, LV_OPA_COVER, 0x808080, LV_OPA_COVER, 0x00a8ff,
                                                       360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                       18, 24);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:44:0
        ** 说明: 垂直线显示
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), server_operation_network_obj_id_ver_line, 584, 168, 1, 432,
                                         NULL, false, LV_OPA_COVER, 0x323237, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:44:0
         ** 说明: 列表
         ***********************************************/
        lv_obj_t *list = server_operating_network_list_create();
        {
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:44:0
                 ** 说明: building house number
                 ***********************************************/
                lv_obj_t *cont = NULL;
                {
                        lv_obj_t *obj = lv_common_text_create(list, server_operation_network_obj_id_building_house_number_title, 0, 0, 584, 48,
                                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_BUILD_HOUSE_NUMBER), 0X00A8ff, 0X00A8ff, LV_TEXT_ALIGN_LEFT, lv_font_small);

                        lv_obj_set_style_pad_left(obj, 13, LV_PART_MAIN);
                        lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: building number
                         ***********************************************/

                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_building_number_cont, 0, 48, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {
                                lv_common_text_create(cont, server_operation_network_obj_id_building_number_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_BUILD_NUMBER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_building_number_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "01", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 4,
                                                          5, 500, 0Xffffff);
                        }

                        /***********************************************
                        ** 作者: leo.liu
                        ** 日期: 2023-2-2 13:44:0
                        ** 说明: household number
                        ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_building_household_number_cont, 0, 48 + 72, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {

                                lv_common_text_create(cont, server_operation_network_obj_id_building_household_number_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_HOUSEHOLD_NUMBER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_building_household_number_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "01", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 4,
                                                          5, 500, 0Xffffff);
                        }
                }

                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:44:0
                ** 说明: product ip
                ***********************************************/
                {
                        lv_obj_t *obj = lv_common_text_create(list, server_operation_network_obj_id_product_ip_title, 0, 48 + 72 * 2, 584, 48,
                                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_IP), 0X00A8ff, 0X00A8ff, LV_TEXT_ALIGN_LEFT, lv_font_small);

                        lv_obj_set_style_pad_left(obj, 13, LV_PART_MAIN);
                        lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: product ip
                         ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_product_ip_cont, 0, 48 * 2 + 72 * 2, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {

                                lv_common_text_create(cont, server_operation_network_obj_id_product_ip_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_IP), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_product_ip_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }
                        /***********************************************
                        ** 作者: leo.liu
                        ** 日期: 2023-2-2 13:44:0
                        ** 说明: gateway
                        ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_gateway_cont, 0, 48 * 2 + 72 * 3, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {
                                lv_common_text_create(cont, server_operation_network_obj_id_gateway_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_GATEWAY), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_gateway_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }
                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: mask
                         ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_mask_cont, 0, 48 * 2 + 72 * 4, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {
                                lv_common_text_create(cont, server_operation_network_obj_id_mask_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_MASK), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_mask_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: dns
                         ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_dns_cont, 0, 48 * 2 + 72 * 5, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {

                                lv_common_text_create(cont, server_operation_network_obj_id_dns_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_DNS), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_dns_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:44:0
                 ** 说明: operating server
                 ***********************************************/
                {
                        lv_obj_t *obj = lv_common_text_create(list, server_operation_network_obj_id_operating_server_title, 0, 48 * 2 + 72 * 6, 584, 48,
                                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_OPERATING_SERVER), 0X00A8ff, 0X00A8ff, LV_TEXT_ALIGN_LEFT, lv_font_small);

                        lv_obj_set_style_pad_left(obj, 13, LV_PART_MAIN);
                        lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: local server
                         ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_local_server_cont, 0, 48 * 3 + 72 * 6, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {

                                lv_common_text_create(cont, server_operation_network_obj_id_product_ip_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_LOCAL_SERVER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_local_server_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }

                        /***********************************************
                        ** 作者: leo.liu
                        ** 日期: 2023-2-2 13:44:0
                        ** 说明:  sip server
                        ***********************************************/

                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_sip_server_cont, 0, 48 * 3 + 72 * 6, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {
                                lv_common_text_create(cont, server_operation_network_obj_id_sip_server_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_SIP_SERVER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_sip_server_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }

                        // /***********************************************
                        //  ** 作者: leo.liu
                        //  ** 日期: 2023-2-2 13:44:0
                        //  ** 说明: update server
                        //  ***********************************************/

                        // cont = lv_common_img_btn_create(list, server_operation_network_obj_id_update_server_cont, 0, 48 * 3 + 72 * 7, 584, 72,
                        //                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                        //                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                        //                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                        //                                 NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        // {
                        //         lv_common_text_create(cont, server_operation_network_obj_id_update_server_text, 32, 16, 204, 40,
                        //                               NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                        //                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                        //                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                        //                               lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_UPDATE_SERVER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);

                        //         lv_common_textarea_create(cont, server_operation_network_obj_id_update_server_textarea, 298, 9, 262, 54,
                        //                                   NULL, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                        //                                    LV_OPA_TRANSP,0Xffffff,LV_OPA_COVER,0Xffffff,
                        //                                   9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                        //                                   9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                        //                                   "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                        //                                   5, 500, 0Xffffff);
                        // }

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: cctv server
                         ***********************************************/

                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_cctv_server_cont, 0, 48 * 3 + 72 * 7, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {
                                lv_common_text_create(cont, server_operation_network_obj_id_cctv_server_text, 32, 0, 204, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_CCTV_SERVER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_cctv_server_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                                          5, 500, 0Xffffff);
                        }
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:44:0
                 ** 说明: guard station
                 ***********************************************/
                {
                        lv_obj_t *obj = lv_common_text_create(list, server_operation_network_obj_id_guard_station_title, 0, 48 * 3 + 72 * 8, 584, 48,
                                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_GUARD_STATION), 0X00A8ff, 0X00A8ff, LV_TEXT_ALIGN_LEFT, lv_font_small);

                        lv_obj_set_style_pad_left(obj, 13, LV_PART_MAIN);
                        lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:44:0
                         ** 说明: local server
                         ***********************************************/
                        cont = lv_common_img_btn_create(list, server_operation_network_obj_id_guard_statioon_cont, 0, 48 * 4 + 72 * 9, 584, 72,
                                                        NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                        {
                                lv_common_text_create(cont, server_operation_network_obj_id_guard_statioon_text, 32, 0, 266, 72,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_GUARD_STATION_NUMBER), 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal);

                                lv_common_textarea_create(cont, server_operation_network_obj_id_guard_statioon_textarea, 298, 9, 262, 54,
                                                          server_operating_network_textarea_focus_state_clear, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                                          "192.168.0.2", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 11,
                                                          5, 500, 0Xffffff);
                        }
                }
                setting_server_operation_network_init();
        }
}
static void sat_layout_quit(server_operation_network)
{
}

sat_layout_create(server_operation_network);