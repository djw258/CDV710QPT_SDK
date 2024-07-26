#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_away_count.h"
typedef enum
{
    layout_awat_obj_id_title,
    layout_away_obj_id_cancel,
    layout_away_obj_id_tips,
    layout_away_obj_id_tabview,
    layout_away_obj_id_left_arrow,
    layout_away_obj_id_right_arrow,

    layout_away_obj_id_setting_time,
    layout_away_obj_id_release_time,
    layout_away_obj_id_save_photo,
    layout_away_obj_id_bypass_call,

    layout_away_obj_id_audto_record,

    layout_away_obj_id_execution_btn,

    layout_away_obj_id_main_bg,
    layout_away_obj_id_main_bg_tips,

    layout_away_obj_id_msgbox_bg, // 半透明背景id

    layout_away_obj_id_passwd_cont,

} layout_away_scr_act_obj_id;

typedef enum
{
    layout_away_obj_id_msgbox_cont // 消息框id
} away_msgbox_bg_cont_obj_id;      // 消息框小背景对象的子对象id

typedef enum
{
    layout_away_obj_id_msgbox_title,
    layout_away_obj_id_msgbox_list,
    layout_away_obj_id_msgbox_checkbox1_cont,
    layout_away_obj_id_msgbox_checkbox2_cont,
    layout_away_obj_id_msgbox_checkbox3_cont,
    layout_away_obj_id_msgbox_two_minute,
    layout_away_obj_id_msgbox_three_minute,
    layout_away_obj_id_msgbox_cancel,
    layout_away_obj_id_msgbox_confirm,
} away_msgbox_bg_cont_cont_obj_id; // 消息框对象的子对象id

typedef enum
{
    away_obj_id_t_msgbox_checkbox_label,
    away_obj_id_t_msgbox_checkbox_img,
} away_msgbox_bg_cont_cont_checkbox_obj_id; // 复选框对象的子对象id

typedef enum
{
    layout_away_obj_id_execution_btn_label,
} layout_away_execution_obj_id;

typedef enum
{
    layout_away_obj_id_setting_time_title,
    layout_away_obj_id_setting_time_sub,
    layout_away_setting_time_img_id,
} layout_away_setting_time_obj_id;

typedef enum
{
    layout_away_obj_id_release_time_title,
    layout_away_obj_id_release_time_sub,
    layout_away_release_time_img_id,
} layout_away_release_time_obj_id;

typedef enum
{
    layout_away_obj_id_save_photo_title,
    layout_away_obj_id_save_photo_sub,
    layout_away_save_photo_switch_id,
} layout_away_save_photo_obj_id;

typedef enum
{
    layout_away_obj_id_bypass_call_title,
    layout_away_obj_id_bypass_call_sub,
    layout_away_bypass_call_img_id,
} layout_away_bypass_call_obj_id;

typedef enum
{
    layout_away_obj_id_auto_record_title,
    layout_away_obj_id_auto_record_sub,
    layout_away_auto_record_img_id,
} layout_away_auto_record_obj_id;

typedef enum
{
    layout_away_sensor_select_cont_checkbox_id,
    layout_away_sensor_select_cont_bg_id,
    layout_away_sensor_select_cont_label_id,
    layout_away_sensor_select_cont_img_id,
} layout_away_sensor_select_cont_obj_id;

typedef enum
{
    layout_away_tabview_page_obj_id_cont1,
    layout_away_tabview_page_obj_id_cont2,
    layout_away_tabview_page_obj_id_cont3,
    layout_away_tabview_page_obj_id_cont4,
    layout_away_tabview_page_obj_id_cont5,
    layout_away_tabview_page_obj_id_cont6,
    layout_away_tabview_page_obj_id_cont7,
    layout_away_tabview_page_obj_id_cont8,
} layout_away_tabview_page_obj_id;

/************************************************************
** 函数说明: 界面退出
** 作者: xiaoxiao
** 日期: 2023-04-27 21:11:19
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_back_obj_click(lv_event_t *ev)
{
    sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static unsigned char away_sensor_enabel_flag = 0x00;
unsigned char layout_away_sensor_enabel_flag_get(void)
{
    return away_sensor_enabel_flag;
}
/************************************************************
** 函数说明: 获取传感器使能选项
** 作者: xiaoxiao
** 日期: 2023-05-06 10:37:00
** 参数说明:
** 注意事项:
************************************************************/
unsigned char layout_away_sensor_enable_flag(void)
{
    lv_obj_t *tableview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_tabview);
    lv_obj_t *cont = lv_tabview_get_content(tableview);
    lv_obj_t *page1 = lv_obj_get_child_form_id(cont, 0);
    lv_obj_t *page2 = lv_obj_get_child_form_id(cont, 1);

    unsigned char selected = 0x00;
    for (int i = 0; i < 8; i++)
    {
        lv_obj_t *parent_cont = NULL;

        if (i < 4)
        {
            parent_cont = lv_obj_get_child_form_id(page1, layout_away_tabview_page_obj_id_cont1 + i);
        }
        else
        {
            parent_cont = lv_obj_get_child_form_id(page1, layout_away_tabview_page_obj_id_cont1 + i);
            if (parent_cont == NULL)
            {
                parent_cont = lv_obj_get_child_form_id(page2, layout_away_tabview_page_obj_id_cont1 + i);
            }
        }
        if (parent_cont == NULL)
        {
            continue;
        }
        lv_obj_t *checkbox = lv_obj_get_child_form_id(parent_cont, layout_away_sensor_select_cont_checkbox_id);
        if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) != 0)
        {

            selected |= (0x01 << i);
        }
        else
        {
            lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        }
    }
    return selected;
}

/************************************************************
** 函数说明: 离家设防界面确定按键显示
** 作者: xiaoxiao
** 日期: 2023-05-04 16:15:02
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_execution_stop_btn_display(void)
{
    lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_execution_btn);
    if (user_data_get()->alarm.away_alarm_enable)
    {
        lv_obj_t *label = lv_obj_get_child_form_id(obj, layout_away_obj_id_execution_btn_label);
        lv_label_set_text(label, lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_END));
    }
    else
    {
        if (layout_away_sensor_enable_flag())
        {
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x00A8FF), LV_PART_MAIN);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
        else
        {
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x47494A), LV_PART_MAIN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }

        lv_obj_t *label = lv_obj_get_child_form_id(obj, layout_away_obj_id_execution_btn_label);
        lv_label_set_text(label, lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_EXECUTION));
    }
}

/************************************************************
** 函数说明: 关闭消息框
** 作者: xiaoxiao
** 日期：2023-09-25 16:22:12
** 参数说明:
** 注意事项：
************************************************************/
static void layout_away_msgbox_cancel_click(lv_event_t *ev)
{
    setting_msgdialog_msg_del(layout_away_obj_id_msgbox_bg);
}

void replaceString(char *source, const char *search, const char *replace)
{
    int searchLen = strlen(search);
    int replaceLen = strlen(replace);
    int sourceLen = strlen(source);

    char newString[256]; // 假设最大长度为100，根据实际情况调整

    int i = 0, j = 0, k = 0;
    while (i <= sourceLen - searchLen)
    {
        // 检查当前位置开始是否匹配 search
        if (strncmp(source + i, search, searchLen) == 0)
        {
            // 如果匹配，复制 replace 到新字符串中
            for (k = 0; k < replaceLen; k++)
            {
                newString[j++] = replace[k];
            }
            i += searchLen; // 跳过 search
        }
        else
        {
            // 如果不匹配，复制 source 中的字符到新字符串中
            newString[j++] = source[i++];
        }
    }

    // 复制剩余的字符
    while (i <= sourceLen)
    {
        newString[j++] = source[i++];
    }

    // 在新字符串末尾添加 null 终止符
    newString[j] = '\0';

    // 将新字符串复制回源字符串
    strcpy(source, newString);
}

/************************************************************
** 函数说明: 安防设置不正常提示
** 作者: xiaoxiao
** 日期：2023-09-25 15:38:19
** 参数说明:
** 注意事项：
************************************************************/
static void layout_away_execution_normal_msgbox_create(char normal_select)
{
    lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_msgbox_bg);
    if (masgbox != NULL)
    {
        setting_msgdialog_msg_del(layout_away_obj_id_msgbox_bg);
    }
    masgbox = setting_msgdialog_msg_bg_create(layout_away_obj_id_msgbox_bg, layout_away_obj_id_msgbox_cont, 282, 143, 460, 283);
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
    replaceString(abnormal_str1, "(N)", sensors_str);
    char abnormal_str[256] = {0};
    snprintf(abnormal_str, sizeof(abnormal_str), "%s%s", lang_str_get(SENSOR_SETTING_XLS_LANG_ID_CANNOT), abnormal_str1);
    setting_msgdialog_msg_create(masgbox, layout_away_obj_id_msgbox_title, abnormal_str, 0, 70, 460, 120, false);
    setting_msgdialog_msg_confirm_btn_create(masgbox, layout_away_obj_id_msgbox_confirm, layout_away_msgbox_cancel_click);
}
/************************************************************
** 函数说明: 离家设防保存
** 作者: xiaoxiao
** 日期: 2023-04-28 15:00:58
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_execution_obj_click(lv_event_t *ev)
{
    if (user_data_get()->alarm.away_alarm_enable == false)
    {
        char sensor_select_list = layout_away_sensor_enable_flag();
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
        if (normal_select)
        {
            layout_away_execution_normal_msgbox_create(normal_select);
        }
        else
        {
            user_data_get()->alarm.away_setting_countdown = true;
            user_data_get()->alarm.away_alarm_enable_list = sensor_select_list;
            for (int i = 0; i < 8; i++)
            {
                if ((user_data_get()->alarm.alarm_enable_always[i]))
                {
                    user_data_get()->alarm.away_alarm_enable_list |= 0x01 << i;
                }
            }
            user_data_save(true, true);
            if ((user_data_get()->system_mode & 0x0f) != 0x01)
            {
                sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
            }
            sat_layout_goto(away_count, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
    }
    else
    {
        if (user_data_get()->alarm.away_alarm_enable)
        {
            lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_passwd_cont);
            if (cont != NULL)
            {
                lv_obj_clear_flag(cont, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期：2023-12-01 14:55:49
** 参数说明:
** 注意事项：
************************************************************/
static bool layout_away_sensor_cctv_binding_status_valid(void)
{
    bool cctv_linkage = false;
    // 判断是否所选中的传感器有绑定CCTV
    char sensor_select_list = layout_away_sensor_enable_flag();
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
** 函数说明: 传感求设防选择
** 作者: xiaoxiao
** 日期: 2023-04-27 21:11:48
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_sensor_select_obj_click(lv_event_t *ev)
{
    if ((user_data_get()->alarm.away_alarm_enable) || (user_data_get()->alarm.security_alarm_enable))
    {
        return;
    }
    lv_obj_t *obj = lv_event_get_current_target(ev);
    lv_obj_t *tableview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_tabview);
    lv_obj_t *page1 = lv_obj_get_child_form_id(lv_tabview_get_content(tableview), 0);
    lv_obj_t *page2 = lv_obj_get_child_form_id(lv_tabview_get_content(tableview), 1);
    for (int i = 0; i < 8; i++)
    {
        lv_obj_t *cont = NULL;
        if (i < 4)
        {
            cont = lv_obj_get_child_form_id(page1, layout_away_tabview_page_obj_id_cont1 + i);
        }
        else
        {
            cont = lv_obj_get_child_form_id(page1, layout_away_tabview_page_obj_id_cont1 + i);
            if (cont == NULL)
            {
                cont = lv_obj_get_child_form_id(page2, layout_away_tabview_page_obj_id_cont1 + i);
            }
        }
        if (cont == obj)
        {
            lv_obj_t *checkbox = lv_obj_get_child_form_id(obj, layout_away_sensor_select_cont_checkbox_id);
            if ((strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) == 0))
            {

                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
                if (user_data_get()->alarm.away_auto_record == true && layout_away_sensor_cctv_binding_status_valid())
                {
                    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_audto_record);
                    lv_obj_t *obj = lv_obj_get_child_form_id(parent, layout_away_auto_record_img_id);
                    lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
                }
            }
            else
            {
                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
                if (layout_away_sensor_cctv_binding_status_valid() == false)
                {
                    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_audto_record);
                    lv_obj_t *obj = lv_obj_get_child_form_id(parent, layout_away_auto_record_img_id);
                    lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
                }
            }
            break;
        }
    }

    layout_away_execution_stop_btn_display();
}

/************************************************************
** 函数说明: 传感器大容器创建
** 作者: xiaoxiao
** 日期: 2023-04-27 19:37:27
** 参数说明:
** 注意事项:
************************************************************/
static lv_obj_t *layout_away_tableview_obj_create(void)
{
    lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_tabview);
    if (tabview != NULL)
    {
        return tabview;
    }
    tabview = lv_tabview_create(sat_cur_layout_screen_get(), LV_DIR_BOTTOM, 50);
    lv_common_style_set_common(tabview, layout_away_obj_id_tabview, 49, 141, 416, 328 + 25, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

    lv_obj_t *cont = lv_tabview_get_content(tabview);
    lv_common_style_set_common(cont, 0, 0, 56, 416, 328, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_t *page_1 = lv_tabview_add_tab(tabview, " ");
    lv_common_style_set_common(page_1, 0, 0, 0, 416, 328, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
    lv_obj_t *page_2 = lv_tabview_add_tab(tabview, " ");
    lv_common_style_set_common(page_2, 1, 0, 0, 416, 328, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

    lv_obj_t *btnmatrix = lv_tabview_get_tab_btns(tabview);
    lv_common_style_set_common(btnmatrix, 1, 0, 8, 416, 8, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_common_style_set_text(btnmatrix, NULL, 0XFFFFFF, 0X9E9EA6, LV_ALIGN_CENTER, lv_font_normal, LV_STATE_PRESSED | LV_STATE_CHECKED, LV_PART_MAIN);
    lv_common_style_set_event(btnmatrix, NULL, true, LV_OPA_30, 0x9e9ea6, LV_PART_ITEMS, LV_OPA_COVER, 0XFFFFFF, LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_common_style_set_boader(btnmatrix, 360, LV_OPA_TRANSP, 0, LV_BORDER_SIDE_NONE, 0XFFFFFF, LV_PART_ITEMS);
    lv_common_style_set_boader(btnmatrix, 360, LV_OPA_TRANSP, 0, LV_BORDER_SIDE_NONE, 0XFFFFFF, LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_set_style_pad_left(btnmatrix, 195, LV_PART_MAIN);
    lv_obj_set_style_pad_right(btnmatrix, 195, LV_PART_MAIN);
    lv_obj_set_style_pad_column(btnmatrix, 10, LV_PART_MAIN);

    return tabview;
}

static void layout_away_sensor_select_left_btn_click(lv_event_t *e)
{
    lv_obj_t *tab = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_tabview);
    lv_tabview_set_act(tab, 0, LV_ANIM_OFF);
}

static void layout_away_sensor_select_right_btn_click(lv_event_t *e)
{
    lv_obj_t *tab = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_tabview);
    lv_tabview_set_act(tab, 1, LV_ANIM_OFF);
}

/************************************************************
** 函数说明: 传感器选择创建
** 作者: xiaoxiao
** 日期: 2023-04-27 19:37:27
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_sensor_select_create(void)
{
    security_list_info_t main_list_group[] = {
        {
            49 - 49,
            141 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont1,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR1,
            lang_str_get,
        },
        {
            261 - 49,
            141 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont2,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR2,
            lang_str_get,
        },
        {
            49 - 49,
            309 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont3,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR3,
            lang_str_get,
        },
        {
            261 - 49,
            309 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont4,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR4,
            lang_str_get,
        },

        {
            49 - 49,
            141 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont5,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR5,
            lang_str_get,
        },
        {
            261 - 49,
            141 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont6,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR6,
            lang_str_get,
        },
        {
            49 - 49,
            309 - 141,
            204,
            160,
            layout_away_tabview_page_obj_id_cont7,
            layout_away_sensor_select_obj_click,
            layout_away_sensor_select_cont_checkbox_id,
            true,
            LAYOUT_SECURITY_XLS_LANG_ID_SENSOR7,
            lang_str_get,
        },
        // {261 -49, 309 - 141, 204, 160,
        // layout_away_tabview_page_obj_id_cont8,
        // layout_away_sensor_select_obj_click,
        // layout_away_sensor_select_cont_checkbox_id,
        // true,
        // LAYOUT_SECURITY_XLS_LANG_ID_SENSOR8, lang_str_get,
        // },
    };

    /************************************************************
    ** 函数说明: 左右箭头图标创建
    ** 作者: xiaoxiao
    ** 日期: 2023-05-06 08:50:42
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_away_obj_id_right_arrow, 466, 282, 48, 48,
                                 layout_away_sensor_select_right_btn_click, true, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("away_right_arrow.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_away_obj_id_left_arrow, 0, 282, 48, 48,
                                 layout_away_sensor_select_left_btn_click, true, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("away_left_arrow.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }

    lv_obj_t *tabview = layout_away_tableview_obj_create();
    lv_obj_t *cont = lv_tabview_get_content(tabview);
    lv_obj_t *page_1 = lv_obj_get_child_form_id(cont, 0);
    lv_obj_t *page_2 = lv_obj_get_child_form_id(cont, 1);

    int j = 0; // 标志在第几个容器位置上显示数据

    for (int i = 0; i < sizeof(main_list_group) / sizeof(security_list_info_t); i++)
    {

        if (((user_data_get()->alarm.alarm_enable[i] != 0)) && (user_data_get()->alarm.away_sensor_enable[i]) && (user_data_get()->alarm.alarm_enable_always[i] == false))
        {
            lv_obj_t *parent = lv_common_img_text_btn_create((j <= 3) ? page_1 : page_2, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[j].w, main_list_group[j].h,
                                                             main_list_group[i].click_cb, LV_OPA_COVER, 0x242526, LV_OPA_COVER, 0x242526,
                                                             8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                             0, 96, 204, 54, layout_away_sensor_select_cont_label_id,
                                                             main_list_group[i].main_language_cb(main_list_group[i].main_language_id), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_small,
                                                             0, 12, 204, 80, layout_away_sensor_select_cont_img_id,
                                                             (const char *)resource_ui_src_get("ic_security_normal_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

            lv_common_img_btn_create(parent, main_list_group[i].check_id, 164, 8, 32, 32,
                                     NULL, false, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                     0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                     0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                     ((user_data_get()->alarm.away_alarm_enable_list & (0x01 << i)) || !user_data_get()->alarm.away_alarm_enable) ? resource_ui_src_get("btn_checkbox_s.png") : (const char *)resource_ui_src_get("btn_checkbox_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
            j++;
        }
    }
}

/************************************************************
** 函数说明: 离家界面时间设置复选框点击事件
** 作者: xiaoxiao
** 日期: 2023-05-08 18:46:10
** 参数说明:
** 注意事项:
************************************************************/
static void away_setting_time_msgbox_checkbox_click(lv_event_t *ev)
{
    lv_obj_t *parent = lv_event_get_current_target(ev);
    if (parent == NULL)
    {
        return;
    }
    lv_obj_t *check_obj, *discheck1_obj, *discheck2_obj;
    if (parent->id == layout_away_obj_id_msgbox_checkbox1_cont)
    {
        check_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), layout_away_obj_id_msgbox_checkbox2_cont);
        discheck1_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), layout_away_obj_id_msgbox_checkbox3_cont);
        discheck2_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
    }
    else if (parent->id == layout_away_obj_id_msgbox_checkbox2_cont)
    {
        check_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), layout_away_obj_id_msgbox_checkbox1_cont);
        discheck1_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), layout_away_obj_id_msgbox_checkbox3_cont);
        discheck2_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
    }
    else
    {
        check_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), layout_away_obj_id_msgbox_checkbox1_cont);
        discheck1_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), layout_away_obj_id_msgbox_checkbox2_cont);
        discheck2_obj = lv_obj_get_child_form_id(parent, away_obj_id_t_msgbox_checkbox_img);
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

static void layout_away_setting_time_msgbox_option_create(lv_obj_t *msgbox, lv_event_cb_t checkbox_cb)
{

    lv_common_img_text_btn_create(msgbox, layout_away_obj_id_msgbox_checkbox1_cont, 48, 80, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, away_obj_id_t_msgbox_checkbox_label,
                                  lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_1_MINUTE), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, away_obj_id_t_msgbox_checkbox_img,
                                  user_data_get()->alarm.away_setting_time == 1 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_text_btn_create(msgbox, layout_away_obj_id_msgbox_checkbox2_cont, 48, 136, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, away_obj_id_t_msgbox_checkbox_label,
                                  lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_2_MINUTE), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, away_obj_id_t_msgbox_checkbox_img,
                                  user_data_get()->alarm.away_setting_time == 2 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_text_btn_create(msgbox, layout_away_obj_id_msgbox_checkbox3_cont, 48, 192, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, away_obj_id_t_msgbox_checkbox_label,
                                  lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_3_MINUTE), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, away_obj_id_t_msgbox_checkbox_img,
                                  user_data_get()->alarm.away_setting_time == 3 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
}

/************************************************************
** 函数说明: 离家界面创建消息框
** 作者: xiaoxiao
** 日期: 2023-05-08 18:33:35
** 参数说明:
** 注意事项:
************************************************************/
static lv_obj_t *layout_away_msgbox_create(const char *title, int x, int y, int width, int height, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb)
{
    lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_away_obj_id_msgbox_bg, 0, 0, 1024, 600,
                                                NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_obj_t *msgbox = lv_common_img_btn_create(parent, layout_away_obj_id_msgbox_cont, x, y, width, height,
                                                NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_text_create(msgbox, layout_away_obj_id_msgbox_title, 0, 10, width, 47,
                          NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                          0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                          0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                          title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

    lv_common_img_btn_create(msgbox, layout_away_obj_id_msgbox_cancel, 0, 281, 230, 62,
                             cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

    lv_common_img_btn_create(msgbox, layout_away_obj_id_msgbox_confirm, 230, 281, 230, 62,
                             confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

    return msgbox;
}

static void layout_away_msgbox_del(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_msgbox_bg);

    if (parent != NULL)
    {
        lv_obj_del(parent);
    }
}

static void layout_away_setting_time_display(void)
{
    lv_obj_t *time = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_setting_time), layout_away_obj_id_setting_time_sub);
    if (time == NULL)
    {
        return;
    }
    if (user_data_get()->alarm.away_setting_time == 1)
    {
        lv_label_set_text(time, lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_1_MINUTE));
    }
    if (user_data_get()->alarm.away_setting_time == 2)
    {
        lv_label_set_text(time, lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_2_MINUTE));
    }
    if (user_data_get()->alarm.away_setting_time == 3)
    {
        lv_label_set_text(time, lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_3_MINUTE));
    }
}

static void layout_away_release_time_display(void)
{
    lv_obj_t *time = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_release_time), layout_away_obj_id_release_time_sub);
    if (time == NULL)
    {
        return;
    }

    lv_label_set_text(time, lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_0 + user_data_get()->alarm.away_release_time / 10));
}

/************************************************************
** 函数说明: 传感器释放时间保存
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_release_time_msgbox_confirm_click(lv_event_t *e)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_msgbox_bg);
    for (int i = 0; i < 10; i++)
    {
        lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, layout_away_obj_id_msgbox_cont), layout_away_obj_id_msgbox_list);
        lv_obj_t *checkbox = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, i), away_obj_id_t_msgbox_checkbox_img);
        if (!strncmp((const char *)checkbox->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
            user_data_get()->alarm.away_release_time = i * 10;
            user_data_save(true, true);
            if ((user_data_get()->system_mode & 0x0f) != 0x01)
            {
                sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
            }
            break;
        }
    }

    layout_away_msgbox_del();
    layout_away_release_time_display();
}

/************************************************************
** 函数说明: 传感器状态设置取消
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_setting_time_msgbox_cancel_click(lv_event_t *e)
{
    layout_away_msgbox_del();
}

static void layout_away_setting_time_save(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_msgbox_bg);
    lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, layout_away_obj_id_msgbox_cont), layout_away_obj_id_msgbox_checkbox1_cont), away_obj_id_t_msgbox_checkbox_img);
    lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, layout_away_obj_id_msgbox_cont), layout_away_obj_id_msgbox_checkbox2_cont), away_obj_id_t_msgbox_checkbox_img);
    if (!strncmp((const char *)check1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
    {
        user_data_get()->alarm.away_setting_time = 1;
    }
    else if (!strncmp((const char *)check2->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
    {
        user_data_get()->alarm.away_setting_time = 2;
    }
    else
    {
        user_data_get()->alarm.away_setting_time = 3;
    }
    user_data_save(true, true);
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }
    layout_away_msgbox_del();
    layout_away_setting_time_display();
}

/************************************************************
** 函数说明: 传感器时间设置保存
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_setting_time_msgbox_confirm_click(lv_event_t *e)
{
    layout_away_setting_time_save();
}

/************************************************************
** 函数说明: 传感器解除时间设置
** 作者: xiaoxiao
** 日期: 2023-05-08 20:11:48
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_release_time_msgbox_checkbox_click(lv_event_t *ev)
{
    lv_obj_t *item = lv_event_get_current_target(ev);
    if (item == NULL)
    {
        return;
    }
    lv_obj_t *parent = lv_obj_get_parent(item);
    if (parent == NULL)
    {
        return;
    }

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(item, 1);
    if (item_img_obj == NULL)
    {
        return;
    }
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
    {
        lv_obj_set_style_bg_img_src(item_img_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);

        int item_n = lv_obj_get_child_cnt(parent);
        for (int i = 0; i < item_n; i++)
        {
            lv_obj_t *n_item = lv_obj_get_child_form_id(parent, i);
            if ((n_item == NULL) || (n_item == item))
            {
                continue;
            }
            item_img_obj = lv_obj_get_child_form_id(n_item, 1);

            lv_obj_set_style_bg_img_src(item_img_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
    }
}
/************************************************************
** 函数说明: 离家设防缓冲时间设置
** 作者: xiaoxiao
** 日期: 2023-05-08 16:29:53
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_setting_time_click(lv_event_t *ev)
{
    lv_obj_t *masgbox = layout_away_msgbox_create(lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_OUT_GOING), 282, 131, 460, 343,
                                                  layout_away_setting_time_msgbox_cancel_click, layout_away_setting_time_msgbox_confirm_click);
    layout_away_setting_time_msgbox_option_create(masgbox, away_setting_time_msgbox_checkbox_click);
}

/************************************************************
** 函数说明: 离家设防设置释放时间消息框选项创建
** 作者: xiaoxiao
** 日期: 2023-05-08 20:17:33
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_release_time_msgbox_option_create(lv_obj_t *msgbox, lv_event_cb_t checkbox_cb)
{
    lv_obj_t *list = setting_list_create(msgbox, layout_away_obj_id_msgbox_list);
    lv_common_style_set_common(list, layout_away_obj_id_msgbox_list, 27, 73, 460 - 27, 200, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
    for (int i = 0; i < 10; i++)
    {
        lv_common_img_text_btn_create(list, i, 0, 0, 460 - 27, 55,
                                      checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      46, 8, 460 - 27 - 16, 32, 0,
                                      lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_0 + i), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, 1,
                                      i == user_data_get()->alarm.away_release_time / 10 ? (const char *)resource_ui_src_get("btn_radio_s.png") : (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
}
/************************************************************
** 函数说明: 离家设防释放时间
** 作者: xiaoxiao
** 日期: 2023-05-08 20:05:21
** 参数说明:
** 注意事项: 在触发电位的传感器必须在此时间段内复位，否则会触发警报
************************************************************/
static void layout_away_release_time_click(lv_event_t *ev)
{
    lv_obj_t *masgbox = layout_away_msgbox_create(lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_RELEASE), 282, 131, 460, 343,
                                                  layout_away_setting_time_msgbox_cancel_click, layout_away_release_time_msgbox_confirm_click);
    layout_away_release_time_msgbox_option_create(masgbox, layout_away_release_time_msgbox_checkbox_click);
}

static void layout_away_save_visitor_photo(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_save_photo);
    lv_obj_t *obj = lv_obj_get_child_form_id(parent, layout_away_save_photo_switch_id);
    if (user_data_get()->alarm.away_save_photo == true)
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
    }
}

static void layout_away_bypass_call_display(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_bypass_call);
    lv_obj_t *obj = lv_obj_get_child_form_id(parent, layout_away_bypass_call_img_id);
    lv_obj_t *text = lv_obj_get_child_form_id(parent, layout_away_obj_id_bypass_call_title);

    lv_obj_clear_flag(parent, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_color(text, lv_color_hex(0x242526), LV_PART_MAIN);
    if (user_data_get()->alarm.bypass_call == true)
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
    }
}

static void layout_away_cctv_auto_record_init_display(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_audto_record);
    lv_obj_t *obj = lv_obj_get_child_form_id(parent, layout_away_auto_record_img_id);
    if (user_data_get()->alarm.away_auto_record == true)
    {
        if (user_data_get()->alarm.away_alarm_enable == false)
        {
            if (layout_away_sensor_cctv_binding_status_valid() == false)
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
            lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
        }
    }
    else
    {
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
    }
}

/************************************************************
** 函数说明: 离家模式自动记录
** 作者: xiaoxiao
** 日期: 2023-05-06 22:28:44
** 参数说明:
** 注意事项:
************************************************************/
static void away_alarm_save_photo_enable_btn_click(lv_event_t *ev)
{

    user_data_get()->alarm.away_save_photo = user_data_get()->alarm.away_save_photo ? false : true;
    user_data_save(true, true);
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }
    layout_away_save_visitor_photo();
}

static void away_bypass_call_enable_btn_click(lv_event_t *ev)
{

    user_data_get()->alarm.bypass_call = user_data_get()->alarm.bypass_call ? false : true;
    user_data_save(true, true);
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }
    layout_away_bypass_call_display();
}

/************************************************************
** 函数说明: 设置CCTV自动记录失败
** 作者: xiaoxiao
** 日期：2023-10-06 14:49:18
** 参数说明:
** 注意事项：
************************************************************/
static void layout_away_cctv_record_enable_failed_display(void)
{
    lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_msgbox_bg);
    if (masgbox != NULL)
    {
        setting_msgdialog_msg_del(layout_away_obj_id_msgbox_bg);
    }
    masgbox = setting_msgdialog_msg_bg_create(layout_away_obj_id_msgbox_bg, layout_away_obj_id_msgbox_cont, 282, 143, 460, 283);

    setting_msgdialog_msg_create(masgbox, layout_away_obj_id_msgbox_title, lang_str_get(INTERCOM_XLS_LANG_ID_FUNC_NOT_AVAILABLE), 0, 70, 460, 120, false);
    setting_msgdialog_msg_confirm_btn_create(masgbox, layout_away_obj_id_msgbox_confirm, layout_away_msgbox_cancel_click);
}

static void away_cctv_record_enable_btn_click(lv_event_t *ev)
{
    if (user_data_get()->alarm.away_alarm_enable)
    {
        return;
    }
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_obj_id_audto_record);
    lv_obj_t *obj = lv_obj_get_child_form_id(parent, layout_away_auto_record_img_id);
    if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_switch_off.png"), strlen(resource_ui_src_get("btn_switch_off.png"))) == 0)
    {
        if (layout_away_sensor_cctv_binding_status_valid() == false)
        {
            layout_away_cctv_record_enable_failed_display();
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
        user_data_get()->alarm.away_auto_record = false;
        user_data_save(true, true);
    }
    else
    {
        user_data_get()->alarm.away_auto_record = true;
        user_data_save(true, true);
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
    }
}

static void layout_away_func_setting_create()
{
    lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), layout_away_obj_id_setting_time, 514, 80, 510, 72,
                                                    layout_away_setting_time_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                    0, 0, 350, 72, layout_away_obj_id_setting_time_title,
                                                    lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_FUNCRION_SETTING_TIME), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                    370, 0, 120, 72, layout_away_obj_id_setting_time_sub,
                                                    "", 0x00a8ff, 0x6d6d79, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                    0, 42, 576, 29, -1,
                                                    NULL, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                    0, 0, 80, 48, -1,
                                                    NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_RIGHT_MID);
    layout_away_setting_time_display();

    lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), layout_away_obj_id_release_time, 514, 80 + 72 * 1, 510, 72,
                                                    layout_away_release_time_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                    0, 0, 370, 72, layout_away_obj_id_release_time_title,
                                                    lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_FUNCRION_RELEASE_TIME), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                    370, 0, 120, 72, layout_away_obj_id_release_time_sub,
                                                    "", 0x00a8ff, 0x6d6d79, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                    0, 42, 576, 29, -1,
                                                    NULL, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_small,
                                                    0, 0, 80, 48, -1,
                                                    NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_RIGHT_MID);
    layout_away_release_time_display();
    if (0)
    {
        lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), layout_away_obj_id_save_photo, 514, 80 + 72 * 2, 510, 72,
                                                        away_alarm_save_photo_enable_btn_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                        0, 0, 350, 72, layout_away_obj_id_save_photo_title,
                                                        lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_SAVE_VISITORS_PHORO), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                        0, 0, 120, 72, -1,
                                                        "", 0x00a8ff, 0x6d6d79, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                        0, 42, 576, 29, -1,
                                                        NULL, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                        370, 12, 80, 48, layout_away_save_photo_switch_id,
                                                        (const char *)resource_ui_src_get("btn_switch_on.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        layout_away_save_visitor_photo();
    }

    if (0)
    {
        lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), layout_away_obj_id_bypass_call, 514, 80 + 72 * 2, 510, 72,
                                                        away_bypass_call_enable_btn_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                        0, 0, 300, 72, layout_away_obj_id_bypass_call_title,
                                                        lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_BYPASS_CALL), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                        0, 17, 120, 40, -1,
                                                        "", 0x00a8ff, 0x6d6d79, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                        0, 42, 576, 29, -1,
                                                        NULL, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                        370, 12, 80, 48, layout_away_bypass_call_img_id,
                                                        (const char *)resource_ui_src_get("btn_switch_on.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        layout_away_bypass_call_display();
    }

    lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), layout_away_obj_id_audto_record, 514, 80 + 72 * 2, 510, 72,
                                                    away_cctv_record_enable_btn_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                    0, 0, 300, 72, layout_away_obj_id_auto_record_title,
                                                    lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_AUTO_RECORD), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                    0, 0, 120, 72, -1,
                                                    "", 0x00a8ff, 0x6d6d79, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_normal,
                                                    0, 42, 576, 29, -1,
                                                    NULL, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                    370, 12, 80, 48, layout_away_auto_record_img_id,
                                                    (const char *)resource_ui_src_get("btn_switch_on.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
    layout_away_cctv_auto_record_init_display();
}

static void layout_away_passwd_check_success_cb(void)
{
    unsigned char list = layout_away_sensor_enable_flag();
    user_data_get()->alarm.away_alarm_enable = false;
    user_data_get()->alarm.away_alarm_enable_list &= (~list);
    user_data_get()->alarm.away_setting_countdown = false;
    user_data_save(true, true);
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }
    sat_layout_goto(away, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void sat_layout_enter(away)
{

    /************************************************************
    ** 函数说明:
    ** 作者: xiaoxiao
    ** 日期: 2023-04-26 11:43:19
    ** 参数说明: 标题显示
    ** 注意事项:
    ************************************************************/
    {
        lv_common_text_create(sat_cur_layout_screen_get(), layout_awat_obj_id_title, 0, 20, 1024, 40,
                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_AWAY), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
    }

    /************************************************************
    ** 函数说明: 退回上一步
    ** 作者: xiaoxiao
    ** 日期: 2023-04-26 11:46:55
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_away_obj_id_cancel, 35, 15, 48, 48,
                                 layout_away_back_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
    }

    /************************************************************
    ** 函数说明: 离家设防提示
    ** 作者: xiaoxiao
    ** 日期: 2023-04-27 20:49:45
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {

        lv_common_text_create(sat_cur_layout_screen_get(), layout_away_obj_id_tips, 71, 80, 372, 48,
                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_DETECTS_INSTRUDERS), 0XB8B8B8B, 0XB8B8B8B, LV_TEXT_ALIGN_CENTER, lv_font_small);
    }

    /************************************************************
    ** 函数说明: 传感器选择
    ** 作者: xiaoxiao
    ** 日期: 2023-04-27 21:03:57
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        layout_away_sensor_select_create();
    }

    /************************************************************
    ** 函数说明: 离家设防功能选项设置
    ** 作者: xiaoxiao
    ** 日期: 2023-04-27 22:25:35
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        layout_away_func_setting_create();
    }

    /************************************************************
    ** 函数说明: 离家设防确认事件
    ** 作者: xiaoxiao
    ** 日期: 2023-04-28 14:36:40
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), layout_away_obj_id_execution_btn, 0, 528, 514, 72,
                                      layout_away_execution_obj_click, LV_OPA_COVER, 0x00A8FF, LV_OPA_COVER, 0X101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 17, 514, 46, layout_away_obj_id_execution_btn_label,
                                      lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_EXECUTION), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                      3, 0, 77, 77, -1,
                                      NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        layout_away_execution_stop_btn_display();
    }
    /************************************************************
    ** 函数说明: 离家模式设防运行ui显示
    ** 作者: xiaoxiao
    ** 日期: 2023-04-28 21:02:26
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_obj_t *obj = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), layout_away_obj_id_main_bg, 514, 100, 510, 500,
                                                      NULL, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 300, 514, 46, layout_away_obj_id_main_bg_tips,
                                                      lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_RUNNING), 0XFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                      0, 0, 510, 300, -1,
                                                      resource_ui_src_get("img_away.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        if (!user_data_get()->alarm.away_alarm_enable)
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }

    common_passwd_check_func_create(layout_away_obj_id_passwd_cont, layout_away_passwd_check_success_cb);
}

static void sat_layout_quit(away)
{
}

sat_layout_create(away);