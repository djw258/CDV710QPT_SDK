#include "layout_define.h"
#include "layout_setting_general.h"
typedef enum
{
    layout_security_obj_id_title,
    layout_security_obj_id_tips,
    layout_security_obj_id_audto_record,
    layout_security_obj_id_cancel,
    layout_security_obj_id_log,
    layout_security_obj_id_state_tips,
    layout_security_obj_id_obj_id_confirm_btn,
    layout_security_obj_id_cont1,
    layout_security_obj_id_cont2,
    layout_security_obj_id_cont3,
    layout_security_obj_id_cont4,
    layout_security_obj_id_cont5,
    layout_security_obj_id_cont6,
    layout_security_obj_id_cont7,
    layout_security_obj_id_cont8,

    layout_security_obj_id_passwd_cont,
    layout_security_obj_id_number_keyboard_btn,
    layout_security_obj_id_passwd_input_tx1,
    layout_security_obj_id_passwd_input_tx2,
    layout_security_obj_id_passwd_input_tx3,
    layout_security_obj_id_passwd_input_tx4,
    layout_security_password_input_obj_id_cancel,
    layout_security_password_input_obj_id_tips,

    layout_security_obj_id_msgbox_bg,
} layout_security_scr_act_obj_id;

typedef enum
{
    layout_security_obj_id_msgbox_cont,
    layout_security_obj_id_msgbox_title,
    layout_security_obj_id_msgbox_confirm_btn,
} layout_security_obj_id_msgbox_bg_obj;
typedef enum
{
    layout_security_obj_id_confirm_label,
} layout_security_confirm_btn_obj_id;
typedef enum
{
    layout_security_sensor_select_cont_checkbox_id,
    layout_security_sensor_select_cont_bg_id,
    layout_security_sensor_select_cont_label_id,
    layout_security_sensor_select_cont_img_id,
} layout_security_sensor_select_cont_obj_id;

typedef enum
{
    auto_record_switch_id,
    auto_record_label_id,
} layout_security_obj_id_auto_record_obj_id;
static void layout_security_back_obj_click(lv_event_t *ev)
{
    sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/************************************************************
** 函数说明: 获取传感器使能选项
** 作者: xiaoxiao
** 日期: 2023-05-06 10:37:00
** 参数说明:
** 注意事项:
************************************************************/
static unsigned char layout_security_sensor_enable_flag(void)
{
    unsigned char selected = 0x00;
    for (int i = 0; i < 8; i++)
    {
        lv_obj_t *parent_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_cont1 + i);
        if (parent_cont == NULL)
        {
            continue;
        }
        lv_obj_t *checkbox = lv_obj_get_child_form_id(parent_cont, layout_security_sensor_select_cont_checkbox_id);
        if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) != 0)
        {
            selected |= (0x01 << i);
        }
    }
    return selected;
}

/************************************************************
** 函数说明: 底部确认按键显示
** 作者: xiaoxiao
** 日期: 2023-05-09 21:22:03
** 参数说明:
** 注意事项:
************************************************************/
static void layout_security_ececution_stop_btn_display(void)
{
    lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_obj_id_confirm_btn);
    if (user_data_get()->alarm.security_alarm_enable)
    {
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x00A8FF), LV_PART_MAIN);
        lv_obj_t *label = lv_obj_get_child_form_id(obj, layout_security_obj_id_confirm_label);
        lv_label_set_text(label, lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_END));
    }
    else
    {
        if (layout_security_sensor_enable_flag())
        {
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x00A8FF), LV_PART_MAIN);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
        else
        {
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x47494A), LV_PART_MAIN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
        lv_obj_t *label = lv_obj_get_child_form_id(obj, layout_security_obj_id_confirm_label);
        lv_label_set_text(label, lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_EXECUTION));
    }
}

/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期：2023-12-01 14:55:49
** 参数说明:
** 注意事项：
************************************************************/
static bool layout_security_sensor_cctv_binding_status_valid(void)
{
    bool cctv_linkage = false;
    // 判断是否所选中的传感器有绑定CCTV
    char sensor_select_list = layout_security_sensor_enable_flag();
    for (int i = 0; i < 7; i++)
    {
        if (sensor_select_list & 0x01 << i)
        {
            for (int j = 0; j < 7; j++)
            {
                if (user_data_get()->alarm.cctv_sensor[j] == i + 1)
                {
                    cctv_linkage = true;
                    return cctv_linkage;
                }
            }
        }
    }
    return cctv_linkage;
}

/************************************************************
** 函数说明: 传感器选择点击事件
** 作者: xiaoxiao
** 日期: 2023-04-27 11:04:59
** 参数说明:
** 注意事项:
************************************************************/
static void layout_security_sensor_select_obj_click(lv_event_t *ev)
{
    if ((user_data_get()->alarm.away_alarm_enable) || (user_data_get()->alarm.security_alarm_enable))
    {
        return;
    }
    lv_obj_t *obj = lv_event_get_current_target(ev);
    for (int i = 0; i < 8; i++)
    {
        lv_obj_t *cont = lv_obj_get_child_form_id(lv_obj_get_parent(obj), layout_security_obj_id_cont1 + i);
        if (cont == obj)
        {
            lv_obj_t *checkbox = lv_obj_get_child_form_id(obj, layout_security_sensor_select_cont_checkbox_id);
            if ((strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) == 0))
            {

                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
                if (user_data_get()->alarm.security_auto_record == true && layout_security_sensor_cctv_binding_status_valid())
                {
                    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_audto_record);
                    lv_obj_t *obj = lv_obj_get_child_form_id(parent, auto_record_switch_id);
                    lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
                }
            }
            else
            {
                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
                if (layout_security_sensor_cctv_binding_status_valid() == false)
                {
                    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_audto_record);
                    lv_obj_t *obj = lv_obj_get_child_form_id(parent, auto_record_switch_id);
                    lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
                }
            }
        }
    }
    layout_security_ececution_stop_btn_display();
}

/************************************************************
** 函数说明: 查看日志
** 作者: xiaoxiao
** 日期: 2023-04-27 11:05:55
** 参数说明:
** 注意事项:
************************************************************/
static void layout_security_log_obj_click(lv_event_t *ev)
{
    sat_layout_goto(alarm_list, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/************************************************************
** 函数说明: 关闭消息框
** 作者: xiaoxiao
** 日期：2023-09-25 16:22:12
** 参数说明:
** 注意事项：
************************************************************/
static void layout_security_msgbox_cancel_click(lv_event_t *ev)
{
    setting_msgdialog_msg_del(layout_security_obj_id_msgbox_bg);
}

/************************************************************
** 函数说明: 安防设置不正常提示
** 作者: xiaoxiao
** 日期：2023-09-25 15:38:19
** 参数说明:
** 注意事项：
************************************************************/
static void layout_security_execution_normal_msgbox_create(char normal_select)
{
    lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_msgbox_bg);
    if (masgbox != NULL)
    {
        setting_msgdialog_msg_del(layout_security_obj_id_msgbox_bg);
    }
    masgbox = setting_msgdialog_msg_bg_create(layout_security_obj_id_msgbox_bg, layout_security_obj_id_msgbox_cont, 282, 143, 460, 283);
    char sensors_str[64] = {0};
    char index[4] = {0};
    for (int i = 0; i < 7; i++)
    {
        if (normal_select & 0x01 << i)
        {
            memset(index, 0, sizeof(index));
            if (sensors_str[0] == '\0')
            {
                sprintf(index, "%d", i + 1);
            }
            else
            {
                sprintf(index, ",%d", i + 1);
            }
            strcat(sensors_str, index);
        }
    }
    char abnormal_str1[256] = {0};
    strcpy(abnormal_str1, lang_str_get(SENSOR_SETTING_XLS_LANG_ID_SENSOR_IS_NORMAL));
    extern void replaceString(char *source, const char *search, const char *replace);
    replaceString(abnormal_str1, "(N)", sensors_str);
    char abnormal_str[256] = {0};
    snprintf(abnormal_str, sizeof(abnormal_str), "%s%s", lang_str_get(SENSOR_SETTING_XLS_LANG_ID_CANNOT), abnormal_str1);

    setting_msgdialog_msg_create(masgbox, layout_security_obj_id_msgbox_title, abnormal_str, 0, 70, 460, 120, false);
    setting_msgdialog_msg_confirm_btn_create(masgbox, layout_security_obj_id_msgbox_confirm_btn, layout_security_msgbox_cancel_click);
}

/************************************************************
** 函数说明: 确认警报设备
** 作者: xiaoxiao
** 日期: 2023-04-27 11:05:55
** 参数说明:
** 注意事项:
************************************************************/
static void layout_security_confirm_btn_obj_click(lv_event_t *ev)
{
    if (user_data_get()->alarm.security_alarm_enable == true)
    {
        lv_obj_t *passwd_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_passwd_cont);
        lv_obj_clear_flag(passwd_cont, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        char sensor_select_list = layout_security_sensor_enable_flag();
        char normal_select = 0;
        for (int i = 0; i < 7; i++)
        {
            if (sensor_select_list & 0x01 << i)
            {
                float value = user_sensor_value_get(i);
                if ((user_data_get()->alarm.alarm_enable[i] == 1 && value < ALM_LOW) || (user_data_get()->alarm.alarm_enable[i] == 2 && value > ALM_HIGHT))
                {
                    normal_select |= 0x01 << i;
                }
            }
        }
        if (normal_select) // 是否有不正常的传感器
        {
            layout_security_execution_normal_msgbox_create(normal_select);
        }
        else
        {
            if ((user_data_get()->system_mode & 0x0f) != 0x01)
            {
                sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
            }
            if (layout_security_sensor_cctv_binding_status_valid() == false)
            {
                lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_audto_record);
                lv_obj_t *obj = lv_obj_get_child_form_id(parent, auto_record_switch_id);
                if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_switch_off.png"), strlen(resource_ui_src_get("btn_switch_off.png"))))
                {
                    user_data_get()->alarm.security_auto_record = true;
                }
                else
                {
                    user_data_get()->alarm.security_auto_record = false;
                }
            }
            user_data_get()->alarm.security_alarm_enable = user_data_get()->alarm.security_alarm_enable ? false : true;
            unsigned char list = layout_security_sensor_enable_flag();
            user_data_get()->alarm.security_alarm_enable_list |= list;
            user_data_save(true, true);
            if ((user_data_get()->system_mode & 0x0f) != 0x01)
            {
                sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
            }
            sat_layout_goto(security, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
    }
}

static void layout_security_sensor_select_create()
{
    security_list_info_t main_list_group[] = {

        {
            92,
            144,
            204,
            160,
            layout_security_obj_id_cont1,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR1,
            lang_str_get,
        },
        {
            304,
            144,
            204,
            160,
            layout_security_obj_id_cont2,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR2,
            lang_str_get,
        },
        {
            516,
            144,
            204,
            160,
            layout_security_obj_id_cont3,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR3,
            lang_str_get,
        },
        {
            728,
            144,
            204,
            160,
            layout_security_obj_id_cont4,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR4,
            lang_str_get,
        },
        {
            92,
            312,
            204,
            160,
            layout_security_obj_id_cont5,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR5,
            lang_str_get,
        },
        {
            304,
            312,
            204,
            160,
            layout_security_obj_id_cont6,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR6,
            lang_str_get,
        },
        {
            516,
            312,
            204,
            160,
            layout_security_obj_id_cont7,
            layout_security_sensor_select_obj_click,
            layout_security_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR7,
            lang_str_get,
        },
        //     {728, 312, 204, 160,
        //     layout_security_obj_id_cont8,
        //     layout_security_sensor_select_obj_click,
        //     layout_security_sensor_select_cont_checkbox_id,
        //     true,
        //     LAYOUT_SECURITY_XLS_LANG_ID_SENSOR8, lang_str_get,
        //     },
    };

    int j = 0; // 标志在第几个容器位置上显示数据

    for (int i = 0; i < sizeof(main_list_group) / sizeof(security_list_info_t); i++)
    {
        // if(user_data_get()->alarm.away_alarm_enable_list & (0x01 << i))
        if ((user_data_get()->alarm.alarm_enable[i] != 0) && (user_data_get()->alarm.security_sensor_enable[i]) && (user_data_get()->alarm.alarm_enable_always[i] == false))
        {
            lv_obj_t *parent = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[j].w, main_list_group[j].h,
                                                             main_list_group[j].click_cb, LV_OPA_COVER, 0x242526, LV_OPA_COVER, 0x242526,
                                                             8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                             0, 96, 204, 54, layout_security_sensor_select_cont_label_id,
                                                             main_list_group[i].main_language_cb(main_list_group[i].main_language_id), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_small,
                                                             0, 12, 204, 80, layout_security_sensor_select_cont_img_id,
                                                             (const char *)resource_ui_src_get("ic_security_normal_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

            lv_common_img_btn_create(parent, main_list_group[i].check_id, 164, 8, 32, 32,
                                     NULL, false, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                     0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                     0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                     ((user_data_get()->alarm.security_alarm_enable_list & (0x01 << i)) || !user_data_get()->alarm.security_alarm_enable) ? resource_ui_src_get("btn_checkbox_s.png") : resource_ui_src_get("btn_checkbox_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
            j++;
        }
    }
}

static void layout_security_cctv_record_enable_init_display(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_audto_record);
    lv_obj_t *obj = lv_obj_get_child_form_id(parent, auto_record_switch_id);
    if (user_data_get()->alarm.security_auto_record == true)
    {

        if (layout_security_sensor_cctv_binding_status_valid() == false)
        {
            lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
        }
        else
        {
            lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
        }
    }
    else
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
    }
}

/************************************************************
** 函数说明: 设置CCTV自动记录失败
** 作者: xiaoxiao
** 日期：2023-10-06 14:49:18
** 参数说明:
** 注意事项：
************************************************************/
static void layout_security_cctv_record_enable_failed_display(void)
{
    lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_msgbox_bg);
    if (masgbox != NULL)
    {
        setting_msgdialog_msg_del(layout_security_obj_id_msgbox_bg);
    }
    masgbox = setting_msgdialog_msg_bg_create(layout_security_obj_id_msgbox_bg, layout_security_obj_id_msgbox_cont, 282, 143, 460, 283);

    setting_msgdialog_msg_create(masgbox, layout_security_obj_id_msgbox_title, lang_str_get(INTERCOM_XLS_LANG_ID_FUNC_NOT_AVAILABLE), 0, 70, 460, 120, false);
    setting_msgdialog_msg_confirm_btn_create(masgbox, layout_security_obj_id_msgbox_confirm_btn, layout_security_msgbox_cancel_click);
}

static void layout_security_auto_record_click(lv_event_t *ev)
{

    if (user_data_get()->alarm.security_alarm_enable)
    {
        return;
    }
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_security_obj_id_audto_record);
    lv_obj_t *obj = lv_obj_get_child_form_id(parent, auto_record_switch_id);
    if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_switch_off.png"), strlen(resource_ui_src_get("btn_switch_off.png"))) == 0)
    {

        if (layout_security_sensor_cctv_binding_status_valid() == false)
        {
            layout_security_cctv_record_enable_failed_display();
            return;
        }
    }

    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }

    if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_switch_off.png"), strlen(resource_ui_src_get("btn_switch_off.png"))))
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
        user_data_get()->alarm.security_auto_record = false;
        user_data_save(true, true);
    }
    else
    {
        user_data_get()->alarm.security_auto_record = true;
        user_data_save(true, true);
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
    }
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }
}

/************************************************************
** 函数说明: 密码校验成功，把标志清了，重新刷新ui
** 作者: xiaoxiao
** 日期: 2023-08-05 17:23:49
** 参数说明:
** 注意事项:
************************************************************/
static void layout_security_passwd_check_success_cb(void)
{
    user_data_get()->alarm.security_alarm_enable = false;
    unsigned char list = layout_security_sensor_enable_flag();
    user_data_get()->alarm.security_alarm_enable_list &= (~list);
    user_data_save(true, true);
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }
    sat_layout_goto(security, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void sat_layout_enter(security)
{
    /************************************************************
    ** 函数说明:
    ** 作者: xiaoxiao
    ** 日期: 2023-04-26 11:43:19
    ** 参数说明: 标题显示
    ** 注意事项:
    ************************************************************/
    {
        lv_common_text_create(sat_cur_layout_screen_get(), layout_security_obj_id_title, 0, 20, 1024, 40,
                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_HEAD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
    }

    /************************************************************
    ** 函数说明: 消息提示
    ** 作者: xiaoxiao
    ** 日期: 2023-04-27 11:27:18
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_text_create(sat_cur_layout_screen_get(), layout_security_obj_id_tips, 36, 97, 512, 88,
                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_TIP), 0XB8B8B8, 0XB8B8B8, LV_TEXT_ALIGN_LEFT, lv_font_small);
    }

    /************************************************************
     ** 函数说明: 传感器选择
     ** 作者: xiaoxiao
     ** 日期: 2023-04-27 10:48:40
     ** 参数说明:
     ** 注意事项:
     ************************************************************/
    {
        layout_security_sensor_select_create();
    }

    /************************************************************
    ** 函数说明: CCTV自动录像
    ** 作者: xiaoxiao
    ** 日期: 2023-04-27 11:36:09
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {

        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), layout_security_obj_id_audto_record, 548, 93, 476, 88,
                                      layout_security_auto_record_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 10, 476 - 110, 32, auto_record_label_id,
                                      lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_AUTO_RECORD), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_RIGHT, lv_font_small,
                                      476 - 100, 0, 80, 48, auto_record_switch_id,
                                      (const char *)resource_ui_src_get("btn_switch_on.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        layout_security_cctv_record_enable_init_display();
    }

    /************************************************************
    ** 函数说明: 退回上一步
    ** 作者: xiaoxiao
    ** 日期: 2023-04-26 11:46:55
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_security_obj_id_cancel, 35, 15, 48, 48,
                                 layout_security_back_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
    }

    /************************************************************
    ** 函数说明: emergency log
    ** 作者: xiaoxiao
    ** 日期: 2023-04-25 23:35:58
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_security_obj_id_log, 960, 16, 48, 48,
                                 layout_security_log_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_emergencylist.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }

    /************************************************************
    ** 函数说明: 在家模式设防状态显示
    ** 作者: xiaoxiao
    ** 日期: 2023-04-28 16:26:21
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), layout_security_obj_id_state_tips, 0, 481, 1024, 38,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_RUN), 0xFF6666, 0xFF6666, LV_TEXT_ALIGN_CENTER, lv_font_normal);
        if (!user_data_get()->alarm.security_alarm_enable)
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }

    /************************************************************
    ** 函数说明: 传感器警报选择确认
    ** 作者: xiaoxiao
    ** 日期: 2023-04-27 11:11:46
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), layout_security_obj_id_obj_id_confirm_btn, 0, 528, 1024, 72,
                                      layout_security_confirm_btn_obj_click, LV_OPA_COVER, 0x00A8FF, LV_OPA_COVER, 0X101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      400, 17, 224, 27, layout_security_obj_id_confirm_label,
                                      lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_EXECUTION), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                      3, 0, 77, 77, -1,
                                      NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        layout_security_ececution_stop_btn_display();
    }

    common_passwd_check_func_create(layout_security_obj_id_passwd_cont, layout_security_passwd_check_success_cb);
}

static void sat_layout_quit(security)
{
}

sat_layout_create(security);