#include "layout_define.h"
#include "layout_setting_ipaddress.h"
#include "layout_ipc_camera.h"
enum
{
        setting_ipaddress_obj_id_titile_label,
        setting_ipaddress_obj_id_cancel_btn,
        setting_ipaddress_obj_id_next_btn,

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 8:50:19
        ** 说明: 数字键盘
        ***********************************************/
        setting_ipaddress_obj_id_number_keyboard_btn,

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 13:43:33
        ** 说明: ip文本框
        ***********************************************/
        setting_ipaddress_obj_id_product_ip_textbox,
        setting_ipaddress_obj_id_default_gateway_textbox,
        setting_ipaddress_obj_id_subnet_mask_textbox,
        setting_ipaddress_obj_id_dns_textbox,
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 13:43:33
        ** 说明: ip文本显示
        ***********************************************/
        setting_ipaddress_obj_id_product_ip_label,
        setting_ipaddress_obj_id_default_gateway_label,
        setting_ipaddress_obj_id_subnet_mask_label,
        setting_ipaddress_obj_id_dns_label,

        setting_ipaddress_obj_id_msg_bg,

        setting_ipaddress_obj_id_dhcp,
        setting_ipaddress_obj_id_static,

};

static layout_setting_ipaddress_info layout_setting_ipaddress_default_info =
    {
        .ip_setting_flag = 0x00,
        .network = {
            .udhcp = true,
            .ipaddr = {0},
            .mask = {"255.0.0.0"},
            .gateway = {"10.0.0.1"},
            .dns = {"8.8.8.8"}},

};

layout_setting_ipaddress_info *layout_setting_ipaddress_info_get(void)
{
        return &layout_setting_ipaddress_default_info;
}

static void setting_ipaddress_obj_cancel_click(lv_event_t *e)
{
        if (layout_setting_ipaddress_info_get()->ip_setting_flag)
        {
                sat_layout_goto(ipc_camera_display, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
}

static void setting_ipaddress_next_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_next_btn);
        if (obj == NULL)
        {

                return;
        }
        if (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) == true)
        {

                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 8:51:49
** 说明: 输入数字键盘
***********************************************/
static lv_obj_t *setting_ipaddress_textarea_focused_get(void)
{

        int obj_id[] = {
            setting_ipaddress_obj_id_product_ip_textbox,
            setting_ipaddress_obj_id_default_gateway_textbox,
            setting_ipaddress_obj_id_subnet_mask_textbox,
            setting_ipaddress_obj_id_dns_textbox};

        lv_obj_t *textarea = NULL;

        for (int i = 0; i < sizeof(obj_id) / sizeof(int); i++)
        {

                textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), obj_id[i]);

                if ((textarea != NULL) && (lv_obj_get_state(textarea) & LV_STATE_FOCUSED))
                {

                        return textarea;
                }
        }

        return textarea;
}

static void setting_ipaddress_falid_confirm(lv_event_t *e)
{

        setting_msgdialog_msg_del(setting_ipaddress_obj_id_msg_bg);
}

/************************************************************
** 函数说明: 数据校验失败提示
** 作者: xiaoxiao
** 日期：2023-09-26 14:23:59
** 参数说明:
** 注意事项：
************************************************************/
static void setting_ipaddress_data_vaild_fali_tips(void)
{
        lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_msg_bg);
        if (masgbox != NULL)
        {
                setting_msgdialog_msg_del(setting_ipaddress_obj_id_msg_bg);
        }
        masgbox = setting_msgdialog_msg_bg_create(setting_ipaddress_obj_id_msg_bg, 0, 282, 143, 460, 283);
        setting_msgdialog_msg_create(masgbox, 1, lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_ENTER_FORMAT), 0, 60, 460, 120, false);
        setting_msgdialog_msg_confirm_btn_create(masgbox, 2, setting_ipaddress_falid_confirm);
}

/************************************************************
** 函数说明: 更改摄像头ip信息失败提示
** 作者: xiaoxiao
** 日期：2023-10-16 14:28:07
** 参数说明:
** 注意事项：
************************************************************/
static void setting_ipaddress_setting_fail_tips(void)
{
        lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_msg_bg);
        if (masgbox != NULL)
        {
                setting_msgdialog_msg_del(setting_ipaddress_obj_id_msg_bg);
        }
        masgbox = setting_msgdialog_msg_bg_create(setting_ipaddress_obj_id_msg_bg, 0, 282, 143, 460, 283);
        setting_msgdialog_msg_create(masgbox, 1, lang_str_get(IP_SETTING_XLS_LANG_ID_IP_SETTING_FAILED), 0, 60, 460, 120, false);
        setting_msgdialog_msg_confirm_btn_create(masgbox, 2, setting_ipaddress_falid_confirm);
}

static void layout_setting_ipaddress_textarea_focus_state_clear(void)
{
        int obj_id[4] =
            {setting_ipaddress_obj_id_product_ip_textbox, setting_ipaddress_obj_id_default_gateway_textbox, setting_ipaddress_obj_id_subnet_mask_textbox, setting_ipaddress_obj_id_dns_textbox};

        int total = sizeof(obj_id) / (sizeof(int));
        for (int i = 0; i < total; i++)
        {
                lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), obj_id[i]);

                if ((textarea != NULL))
                {

                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                }
        }
}

/************************************************************
** 函数说明: 数据合法检查
** 作者: xiaoxiao
** 日期：2023-10-06 19:10:52
** 参数说明:
** 注意事项：
************************************************************/
static bool layout_setting_ipaddress_data_valid_check(void)
{
        layout_setting_ipaddress_textarea_focus_state_clear();
        int obj_id[4] =
            {setting_ipaddress_obj_id_product_ip_textbox, setting_ipaddress_obj_id_default_gateway_textbox, setting_ipaddress_obj_id_subnet_mask_textbox, setting_ipaddress_obj_id_dns_textbox};

        int total = sizeof(obj_id) / (sizeof(int));

        for (int i = 0; i < total; i++)
        {
                lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), obj_id[i]);
                if (textarea != NULL)
                {
                        if (i == 0)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
                                {
                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        return false;
                                }
                        }
                        else if (i == 1)
                        {
                                if (is_valid_ipv4(lv_textarea_get_text(textarea)) == false)
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
                }
        }
        return true;
}

extern bool modify_rtsp_url_by_ip(const char *original_url, const char *new_ip_address, char *modified_url);
static void setting_ipaddress_obj_confirm_click(lv_event_t *e)
{
        /*这个地方id寻找的控件与实际意义不符*/
        lv_obj_t *item1_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_product_ip_textbox);

        lv_obj_t *item2_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_default_gateway_textbox);

        lv_obj_t *item3_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_subnet_mask_textbox);

        lv_obj_t *item4_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dns_textbox);

        lv_obj_t *dhcp = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dhcp), 1);
        const char *ipaddr = lv_textarea_get_text(item1_txt);
        const char *mask = lv_textarea_get_text(item3_txt);
        const char *dns = lv_textarea_get_text(item4_txt);
        const char *gateway = lv_textarea_get_text(item2_txt);
        if (layout_setting_ipaddress_info_get()->ip_setting_flag == 0x00)
        {
                if (!strncmp((const char *)dhcp->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
                {
                        network_data_get()->network.udhcp = true;
                }
                else
                {
                        if (layout_setting_ipaddress_data_valid_check())
                        {
                                strncpy(network_data_get()->network.ipaddr, ipaddr, sizeof(network_data_get()->network.ipaddr));
                                strncpy(network_data_get()->network.mask, mask, sizeof(network_data_get()->network.mask));
                                strncpy(network_data_get()->network.dns, dns, sizeof(network_data_get()->network.dns));
                                strncpy(network_data_get()->network.gateway, gateway, sizeof(network_data_get()->network.gateway));
                                network_data_get()->network.udhcp = false;
                                for (int i = 0; i < DEVICE_MAX; i++)
                                {
                                        if (network_data_get()->door_device[i].sip_url[0] != 0)
                                        {
                                                replace_ip_address(network_data_get()->door_device[i].sip_url, network_data_get()->network.ipaddr);
                                                char number[32] = {0};
                                                sprintf(number, "sip:20%d@%s", i + 1, network_data_get()->network.ipaddr);
                                                sat_ipcamera_device_register(number, i, 5000, false);
                                                // sat_ipcamera_device_update_server_ip(i, network_data_get()->network.ipaddr, 1000);
                                        }
                                }
                        }
                        else
                        {
                                setting_ipaddress_data_vaild_fali_tips();
                                return;
                        }
                }
                network_data_save();
                backlight_enable(false);
                usleep(100 * 1000);
                system("reboot");
        }
        else
        {
                if (!strncmp((const char *)dhcp->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
                {
                        layout_setting_ipaddress_info_get()->network.udhcp = true;
                }
                else
                {
                        if (layout_setting_ipaddress_data_valid_check())
                        {
                                layout_setting_ipaddress_info_get()->network.udhcp = false;
                                strncpy(layout_setting_ipaddress_info_get()->network.ipaddr, ipaddr, sizeof(network_data_get()->network.ipaddr));
                                strncpy(layout_setting_ipaddress_info_get()->network.mask, mask, sizeof(network_data_get()->network.mask));
                                strncpy(layout_setting_ipaddress_info_get()->network.dns, dns, sizeof(network_data_get()->network.dns));
                                strncpy(layout_setting_ipaddress_info_get()->network.gateway, gateway, sizeof(network_data_get()->network.gateway));
                        }
                        else
                        {
                                setting_ipaddress_data_vaild_fali_tips();
                                return;
                        }
                }
                if (sat_ipcamera_network_setting(layout_setting_ipaddress_info_get()->pinfo.ipaddr, layout_setting_ipaddress_info_get()->pinfo.port, layout_setting_ipaddress_info_get()->pinfo.username,
                                                 layout_setting_ipaddress_info_get()->pinfo.password, layout_setting_ipaddress_info_get()->pinfo.auther_flag, &layout_setting_ipaddress_info_get()->network, 3000) == true)
                {
                        if (layout_ipc_camera_input_flag_get() & IPC_CAMERA_FLAG_REGISTER)
                        {
                                if (layout_ipc_cmeara_is_doorcamera_get() && monitor_valid_channel_check(layout_ipc_camera_edit_index_get()))
                                {
                                        char newurl[128] = {0};
                                        for (int i = 0; i < network_data_get()->door_device[layout_ipc_camera_edit_index_get()].profile_token_num; i++)
                                        {
                                                memset(newurl, 0, sizeof(newurl));
                                                modify_rtsp_url_by_ip(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].rtsp[i].rtsp_url, layout_setting_ipaddress_info_get()->network.ipaddr, newurl);
                                                if (layout_setting_ipaddress_info_get()->network.udhcp == true)
                                                {
                                                        // sat_ipcamera_device_delete(layout_ipc_camera_edit_index_get(), 1500);
                                                        // memset(&network_data_get()->door_device[layout_ipc_camera_edit_index_get()], 0, sizeof(network_data_get()->door_device[layout_ipc_camera_edit_index_get()]));
                                                        // network_data_save();
                                                        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                                                }

                                                strncpy(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].rtsp[i].rtsp_url, newurl, sizeof(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].rtsp[i].rtsp_url));
                                        }
                                        strncpy(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].ipaddr, layout_setting_ipaddress_info_get()->network.ipaddr, sizeof(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].ipaddr));
                                }
                                else if (monitor_valid_channel_check(layout_ipc_camera_edit_index_get() + 8))
                                {
                                        char newurl[128] = {0};
                                        for (int i = 0; i < network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].profile_token_num; i++)
                                        {

                                                memset(newurl, 0, sizeof(newurl));

                                                modify_rtsp_url_by_ip(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].rtsp[i].rtsp_url, layout_setting_ipaddress_info_get()->network.ipaddr, newurl);
                                                if (layout_setting_ipaddress_info_get()->network.udhcp == true)
                                                {
                                                        memset(&network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()], 0, sizeof(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()]));
                                                        network_data_save();
                                                        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                                                }
                                                strncpy(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].rtsp[i].rtsp_url, newurl, sizeof(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].rtsp[i].rtsp_url));
                                        }
                                        strncpy(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].ipaddr, layout_setting_ipaddress_info_get()->network.ipaddr, sizeof(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].ipaddr));
                                }
                        }
                        network_data_save();
                        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                }
                else
                {
                        setting_ipaddress_setting_fail_tips();
                }
        }
}

static bool setting_ipaddress_textbox_del(void)
{
        lv_obj_t *textarea = setting_ipaddress_textarea_focused_get();

        if (textarea == NULL)
        {
                return false;
        }

        lv_textarea_del_char(textarea);

        return true;
}
static bool setting_ipaddress_textbox_add(const char *string)
{
        lv_obj_t *textarea = setting_ipaddress_textarea_focused_get();

        if (textarea == NULL)
        {

                return false;
        }

        lv_textarea_add_text(textarea, string);
        return true;
}

static void setting_ipaddress_obj_keyboad_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_target(e);
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);

        if (id == 11)
        {

                setting_ipaddress_textbox_del();
        }
        else
        {
                const char *text = lv_btnmatrix_get_btn_text(obj, id);

                if (text != NULL)
                {

                        setting_ipaddress_textbox_add(text);
                }
        }
        setting_ipaddress_next_obj_display();
}

static void layout_setting_ipaddress_textarea_click_enable()
{
        lv_obj_t *item1_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_product_ip_textbox);

        lv_obj_t *item2_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_default_gateway_textbox);

        lv_obj_t *item3_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_subnet_mask_textbox);

        lv_obj_t *item4_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dns_textbox);

        lv_obj_t *dhcp = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dhcp), 1);
        if (!strncmp((const char *)dhcp->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                lv_obj_clear_flag(item1_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(item2_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(item3_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(item4_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_text_color(item1_txt, lv_color_hex(0x808080), LV_PART_MAIN);
                lv_obj_set_style_text_color(item2_txt, lv_color_hex(0x808080), LV_PART_MAIN);
                lv_obj_set_style_text_color(item3_txt, lv_color_hex(0x808080), LV_PART_MAIN);
                lv_obj_set_style_text_color(item4_txt, lv_color_hex(0x808080), LV_PART_MAIN);
        }
        else
        {
                lv_obj_add_flag(item1_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(item2_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(item3_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_flag(item4_txt, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_text_color(item1_txt, lv_color_hex(0Xffffff), LV_PART_MAIN);
                lv_obj_set_style_text_color(item2_txt, lv_color_hex(0Xffffff), LV_PART_MAIN);
                lv_obj_set_style_text_color(item3_txt, lv_color_hex(0Xffffff), LV_PART_MAIN);
                lv_obj_set_style_text_color(item4_txt, lv_color_hex(0Xffffff), LV_PART_MAIN);
        }
}

static void layout_setting_ipaddress_item_init_display()
{
        lv_obj_t *item1_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_product_ip_textbox);

        lv_obj_t *item2_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_default_gateway_textbox);

        lv_obj_t *item3_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_subnet_mask_textbox);

        lv_obj_t *item4_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dns_textbox);

        if (layout_setting_ipaddress_info_get()->ip_setting_flag)
        {
                lv_textarea_set_text(item1_txt, layout_setting_ipaddress_info_get()->network.ipaddr);
                lv_textarea_set_text(item3_txt, layout_setting_ipaddress_info_get()->network.mask);
                lv_textarea_set_text(item2_txt, layout_setting_ipaddress_info_get()->network.gateway);
                lv_textarea_set_text(item4_txt, layout_setting_ipaddress_info_get()->network.dns);
        }
        else
        {
                if (network_data_get()->network.udhcp)
                {
                        char ip[32] = {0};
                        char mask[32] = {0};
                        char gateway[64] = {0};
                        char dns[32] = {0};
                        sat_ip_mac_addres_get("eth0", ip, NULL, mask);
                        sat_default_gateway_get("eth0", gateway, sizeof(gateway));
                        sat_default_dns_get("eth0", dns, sizeof(dns));

                        lv_textarea_set_text(item1_txt, ip);
                        lv_textarea_set_text(item3_txt, mask);
                        lv_textarea_set_text(item4_txt, dns);
                        lv_textarea_set_text(item2_txt, gateway);
                }
                else
                {
                        lv_textarea_set_text(item1_txt, network_data_get()->network.ipaddr);
                        lv_textarea_set_text(item3_txt, network_data_get()->network.mask);
                        lv_textarea_set_text(item4_txt, network_data_get()->network.dns);
                        lv_textarea_set_text(item2_txt, network_data_get()->network.gateway);
                }
        }
        layout_setting_ipaddress_textarea_click_enable();
};

static void setting_ipaddress_obj_display(lv_obj_t *obj_s, int dst_obj_id)
{
        lv_obj_t *obj_n = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), dst_obj_id);
        if (obj_n == NULL)
        {
                return;
        }
        obj_n = lv_obj_get_child_form_id(obj_n, 1);
        obj_s = lv_obj_get_child_form_id(obj_s, 1);
        lv_obj_set_style_bg_img_src(obj_s, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);

        lv_obj_set_style_bg_img_src(obj_n, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
}

static void setting_ipaddress_dhcp_static_init_display(void)
{
        lv_obj_t *dhcp_obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dhcp), 1);
        lv_obj_t *static_obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_static), 1);

        bool udhcp = false;
        if (layout_setting_ipaddress_info_get()->ip_setting_flag)
        {
                udhcp = layout_setting_ipaddress_info_get()->network.udhcp;
        }
        else
        {
                udhcp = network_data_get()->network.udhcp;
        }
        if (udhcp)
        {

                lv_obj_set_style_bg_img_src(dhcp_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(static_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(static_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(dhcp_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
}

static void setting_ipaddress_dhcp_check_click(lv_event_t *e)
{
        lv_obj_t *obj_s = lv_event_get_current_target(e);
        if (obj_s == NULL)
        {
                return;
        }
        setting_ipaddress_next_obj_display();
        setting_ipaddress_obj_display(obj_s, setting_ipaddress_obj_id_static);
        layout_setting_ipaddress_textarea_click_enable();
}

static void setting_ipaddress_static_check_click(lv_event_t *e)
{
        lv_obj_t *obj_s = lv_event_get_current_target(e);
        if (obj_s == NULL)
        {
                return;
        }
        setting_ipaddress_next_obj_display();
        setting_ipaddress_obj_display(obj_s, setting_ipaddress_obj_id_dhcp);
        layout_setting_ipaddress_textarea_click_enable();
}

/************************************************************
** 函数说明: 在进行下一步的时候，会校验数据，数据不合法，输入会停留在第一个不合法的文本区域，需要把不合法的文本区域输入状态取消，变更到最新一次点击的文本区域去
** 作者: xiaoxiao
** 日期：2023-09-26 16:52:11
** 参数说明:
** 注意事项：
************************************************************/
static void layout_setting_ipaddress_textarea_click(lv_event_t *ev)
{
        layout_setting_ipaddress_textarea_focus_state_clear();

        lv_obj_t *cur_textarea = lv_event_get_current_target(ev);

        lv_obj_add_state(cur_textarea, LV_STATE_FOCUSED);
}

static void sat_layout_enter(setting_ipaddress)
{

        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dhcp, 60, 118, 201, 48,
                                      setting_ipaddress_dhcp_check_click, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      50, 0, 161, 48, 0,
                                      "DHCP", 0XFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 0, 48, 48, 1,
                                      resource_ui_src_get("btn_radio_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_static, 277, 118, 201, 48,
                                      setting_ipaddress_static_check_click, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      50, 0, 161, 48, 0,
                                      "Static", 0XFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 0, 48, 48, 1,
                                      resource_ui_src_get("btn_radio_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        setting_ipaddress_dhcp_static_init_display();
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_titile_label, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(INSTALLATION_XLS_LANG_ID_IP_ADDRES), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 上一步下一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_cancel_btn, 35, 15, 48, 48,
                                         setting_ipaddress_obj_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_next_btn, 912, 15, 80, 48,
                                                         setting_ipaddress_obj_confirm_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_title_check.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:18
        ** 说明: 数字键盘创建
        ***********************************************/
        {
                lv_common_number_input_keyboard_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_number_keyboard_btn, 608, 127, 312, 402,
                                                       setting_ipaddress_obj_keyboad_click, LV_OPA_COVER, 0x808080, LV_OPA_COVER, 0x00a8ff,
                                                       360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                       18, 24);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:29
        ** 说明: 文本输入框显示
        ***********************************************/
        char ip[32] = {0};
        char mask[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, NULL, mask);
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_product_ip_textbox, 298, 206, 250, 50,
                                          layout_setting_ipaddress_textarea_click, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          ip, 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_product_ip_label, 32, 213, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_IP), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_default_gateway_textbox, 298, 290, 250, 50,
                                          layout_setting_ipaddress_textarea_click, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          "192.168.0.1", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_default_gateway_label, 32, 297, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_GATEWAY), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_subnet_mask_textbox, 298, 374, 250, 50,
                                          layout_setting_ipaddress_textarea_click, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          mask, 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_subnet_mask_label, 32, 381, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_MASK), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dns_textbox, 298, 458, 250, 50,
                                          layout_setting_ipaddress_textarea_click, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          "192.168.0.1", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 15,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_ipaddress_obj_id_dns_label, 32, 465, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_DNS), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }

        layout_setting_ipaddress_item_init_display();
        if (layout_ipc_cmeara_is_doorcamera_get() == true)
        {
                sat_ipcamera_initialization_parameters(network_data_get()->door_device, DEVICE_MAX);
        }
        else
        {
                sat_ipcamera_initialization_parameters(network_data_get()->cctv_device, DEVICE_MAX);
        }
}
static void sat_layout_quit(setting_ipaddress)
{
}

sat_layout_create(setting_ipaddress);