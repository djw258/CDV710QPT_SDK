#include "layout_define.h"
#include "layout_setting_sensor_usage.h"
#include "layout_setting_general.h"

static int select_index = 0;

int layout_sensor_setting_select_index_get()
{
    return select_index;
}

void layout_sensor_setting_select_index_set(int index)
{
    select_index = index;
}
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

typedef enum
{
    sensor_setting_obj_id_msgbox_cont,
    sensor_setting_obj_id_msgbox_title,
    sensor_setting_obj_id_msgbox_check_1,
    sensor_setting_obj_id_msgbox_check_2,
    sensor_setting_obj_id_msgbox_check_3,
    sensor_setting_obj_id_msgbox_check_4_bg,
    sensor_setting_obj_id_msgbox_check_4,
    sensor_setting_obj_id_msgbox_cancel,
    sensor_setting_obj_id_titile_id,
    sensor_setting_obj_id_msgbox_confirm,

} sensor_setting_msgbox_bg_cont_obj_id;

typedef enum
{
    sensor_setting_obj_id_msgbox_check_text,
    sensor_setting_obj_id_msgbox_check_img,
} sensor_setting_msgbox_check_obj_id;

static void setting_sensor_settings_cancel_obj_click(lv_event_t *e)
{
    sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

/************************************************************
** 函数说明: 总是检测点击事件
** 作者: xiaoxiao
** 日期: 2023-05-18 18:29:20
** 参数说明:
** 注意事项:
************************************************************/
static void sensor_setting_always_detect_click(lv_event_t *e)
{
    lv_obj_t *parent = lv_event_get_current_target(e);
    if (parent == NULL)
    {
        return;
    }
    lv_obj_t *check_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
    if (strncmp((const char *)check_obj->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))))
    {
        lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
    }
}

static lv_obj_t *layout_senesor_setting_msgbox_create(const char *title, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb, const char *item[3])
{
    lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), sensor_setting_obj_id_msgbox_bg_cont, 0, 0, 1024, 600,
                                                NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_obj_t *msgbox = lv_common_img_btn_create(parent, sensor_setting_obj_id_msgbox_cont, 282, 131, 460, 343,
                                                NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_text_create(msgbox, sensor_setting_obj_id_msgbox_title, 32, 10, 396, 47,
                          NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                          0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                          0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                          title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

    lv_common_img_text_btn_create(msgbox, sensor_setting_obj_id_msgbox_check_1, 48, 66, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, sensor_setting_obj_id_msgbox_check_text,
                                  item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, sensor_setting_obj_id_msgbox_check_img,
                                  "", LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_text_btn_create(msgbox, sensor_setting_obj_id_msgbox_check_2, 48, 122, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, sensor_setting_obj_id_msgbox_check_text,
                                  item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, sensor_setting_obj_id_msgbox_check_img,
                                  "", LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_text_btn_create(msgbox, sensor_setting_obj_id_msgbox_check_3, 48, 177, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, sensor_setting_obj_id_msgbox_check_text,
                                  item[2], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, sensor_setting_obj_id_msgbox_check_img,
                                  "", LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_text_btn_create(msgbox, sensor_setting_obj_id_msgbox_check_4, 48, 233, 365, 48,
                                  sensor_setting_always_detect_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, sensor_setting_obj_id_msgbox_check_text,
                                  item[3], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, sensor_setting_obj_id_msgbox_check_img,
                                  "", LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_btn_create(msgbox, sensor_setting_obj_id_msgbox_check_4_bg, 48, 233, 365, 48,
                             NULL, true, LV_OPA_60, 0x242526, LV_OPA_60, 0x242526,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    lv_common_img_btn_create(msgbox, sensor_setting_obj_id_msgbox_cancel, 0, 281, 230, 62,
                             cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

    lv_common_img_btn_create(msgbox, sensor_setting_obj_id_msgbox_confirm, 230, 281, 230, 62,
                             confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);
    return msgbox;
}

static void sensor_setting_msgbox_click(lv_event_t *e)
{
    lv_obj_t *parent = lv_event_get_current_target(e);
    if (parent == NULL)
    {
        return;
    }
    lv_obj_t *check_obj, *discheck1_obj, *discheck2_obj, *alway_cb_obj, *alway_cb_obj_bg;

    alway_cb_obj_bg = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_4_bg);
    alway_cb_obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_4), sensor_setting_obj_id_msgbox_check_img);
    if (parent->id == sensor_setting_obj_id_msgbox_check_1)
    {
        check_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_2);
        discheck1_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_3);
        discheck2_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);

        lv_obj_set_style_bg_img_src(alway_cb_obj, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        lv_obj_clear_flag(alway_cb_obj_bg, LV_OBJ_FLAG_HIDDEN);
    }
    else if (parent->id == sensor_setting_obj_id_msgbox_check_2)
    {
        check_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_1);
        discheck1_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_3);
        discheck2_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        if (user_sensor_value_get(layout_sensor_setting_select_index_get()) > ALM_HIGHT)
        {
            lv_obj_add_flag(alway_cb_obj_bg, LV_OBJ_FLAG_HIDDEN);
            if (user_data_get()->alarm.alarm_enable_always[layout_sensor_setting_select_index_get()])
            {
                lv_obj_set_style_bg_img_src(alway_cb_obj, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
            }
        }
        else
        {
            lv_obj_clear_flag(alway_cb_obj_bg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_img_src(alway_cb_obj, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        }
    }
    else
    {
        check_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_1);
        discheck1_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);
        parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), sensor_setting_obj_id_msgbox_check_2);
        discheck2_obj = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_img);

        if (user_sensor_value_get(layout_sensor_setting_select_index_get()) < ALM_LOW)
        {
            lv_obj_add_flag(alway_cb_obj_bg, LV_OBJ_FLAG_HIDDEN);
            if (user_data_get()->alarm.alarm_enable_always[layout_sensor_setting_select_index_get()])
            {
                lv_obj_set_style_bg_img_src(alway_cb_obj, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
            }
        }
        else
        {
            lv_obj_clear_flag(alway_cb_obj_bg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_img_src(alway_cb_obj, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        }
    }

    if (strncmp((const char *)check_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
    {
        lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(discheck1_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(discheck2_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
    }
}

static void sensor_setting_msgbox_del(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), sensor_setting_obj_id_msgbox_bg_cont);

    if (parent != NULL)
    {
        lv_obj_del(parent);
    }
}
/************************************************************
** 函数说明: 传感器设置状态保存
** 作者: xiaoxiao
** 日期: 2023-05-08 19:20:39
** 参数说明:
** 注意事项:
************************************************************/
static void layout_sensor_setting_save(void)
{
    lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), sensor_setting_obj_id_msgbox_bg_cont);
    lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_cont), sensor_setting_obj_id_msgbox_check_1), sensor_setting_obj_id_msgbox_check_img);
    lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_cont), sensor_setting_obj_id_msgbox_check_2), sensor_setting_obj_id_msgbox_check_img);
    lv_obj_t *check3 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_cont), sensor_setting_obj_id_msgbox_check_3), sensor_setting_obj_id_msgbox_check_img);
    lv_obj_t *check4 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_cont), sensor_setting_obj_id_msgbox_check_4), sensor_setting_obj_id_msgbox_check_img);
    int index = layout_sensor_setting_select_index_get();

    for (int i = 0; i < 8; i++)
    {
        if (index == i)
        {
            if (!strncmp((const char *)check1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
            {
                user_data_get()->alarm.alarm_enable[i] = 0;
            }
            else if (!strncmp((const char *)check2->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
            {
                user_data_get()->alarm.alarm_enable[i] = 1;
            }
            else if (!strncmp((const char *)check3->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
            {
                user_data_get()->alarm.alarm_enable[i] = 2;
            }
            if (!strncmp((const char *)check4->bg_img_src, resource_ui_src_get("btn_checkbox_s.png"), strlen(resource_ui_src_get("btn_checkbox_s.png"))))
            {

                user_data_get()->alarm.alarm_enable_always[i] = true;
                // 设置总是监测的传感器自动加入到被检测的列表里，不需暴露在被选择的页面中
                user_data_get()->alarm.security_alarm_enable_list |= 0x01 << i; //
                user_data_get()->alarm.away_alarm_enable_list |= 0x01 << i;
            }
            else
            {
                user_data_get()->alarm.alarm_enable_always[i] = false;
                user_data_get()->alarm.security_alarm_enable_list &= ~(0x01 << i);
                user_data_get()->alarm.away_alarm_enable_list &= ~(0x01 << i);
            }
        }
    }
    user_data_save(true, true);
    sat_layout_goto(sensor_settings, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

/************************************************************
** 函数说明: 传感器状态设置取消
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void sensor_setting_msgbox_cancel_click(lv_event_t *e)
{
    sensor_setting_msgbox_del();
}

/************************************************************
** 函数说明: 传感器状态设置保存
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void sensor_setting_msgbox_confirm_click(lv_event_t *e)
{
    layout_sensor_setting_save();
}

/************************************************************
** 函数说明: 传感器状态出事显示
** 作者: xiaoxiao
** 日期: 2023-05-24 14:17:10
** 参数说明:
** 注意事项:
************************************************************/
static void setting_sensor_settings_option_display(lv_obj_t *parent)
{
    lv_obj_t *cb1_img = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_1), sensor_setting_obj_id_msgbox_check_img);
    lv_obj_t *cb2_img = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_2), sensor_setting_obj_id_msgbox_check_img);
    lv_obj_t *cb3_img = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_3), sensor_setting_obj_id_msgbox_check_img);
    lv_obj_t *cb4_img = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_4), sensor_setting_obj_id_msgbox_check_img);

    int ch = layout_sensor_setting_select_index_get();
    lv_obj_t *checkbox4_bg = lv_obj_get_child_form_id(parent, sensor_setting_obj_id_msgbox_check_4_bg);
    int value = user_data_get()->alarm.alarm_enable[ch];
    if (value == 0)
    {
        lv_obj_set_style_bg_img_src(cb1_img, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb2_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb3_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb4_img, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        lv_obj_clear_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
    }
    else if (value == 1)
    {
        lv_obj_set_style_bg_img_src(cb1_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb2_img, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb3_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        if (user_data_get()->alarm.alarm_enable_always[ch])
        {
            lv_obj_set_style_bg_img_src(cb4_img, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
            lv_obj_add_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_set_style_bg_img_src(cb4_img, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
            if (user_sensor_value_get(ch) > ALM_HIGHT)
            {
                lv_obj_add_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
    else if (value == 2)
    {
        lv_obj_set_style_bg_img_src(cb1_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb2_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(cb3_img, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        if (user_data_get()->alarm.alarm_enable_always[ch])
        {
            lv_obj_set_style_bg_img_src(cb4_img, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
            lv_obj_add_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_set_style_bg_img_src(cb4_img, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
            if (user_sensor_value_get(ch) < ALM_LOW)
            {
                lv_obj_add_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(checkbox4_bg, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}
/************************************************************
** 函数说明:  传感器状态设置
** 作者: xiaoxiao
** 日期: 2023-04-29 09:28:32
** 参数说明:
** 注意事项:
************************************************************/
static void setting_sensor_settings_list_click(lv_event_t *e)
{
    if (user_data_get()->alarm.security_alarm_enable || user_data_get()->alarm.away_alarm_enable)
    {
        lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(sensor_setting_obj_id_msgbox_bg_cont, sensor_setting_obj_id_msgbox_cont, 282, 143, 460, 283);
        setting_msgdialog_msg_create(masgbox, sensor_setting_obj_id_titile_id, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_RUNNNG_TIPS), 0, 90, 460, 80, false);
        setting_msgdialog_msg_confirm_btn_create(masgbox, sensor_setting_obj_id_msgbox_confirm, sensor_setting_msgbox_cancel_click);
        return;
    }
    lv_obj_t *obj = lv_event_get_current_target(e);
    for (int i = 0; i < 8; i++)
    {
        if (obj->id == setting_sensor_settings_obj_id_item1_cont + i)
        {
            layout_sensor_setting_select_index_set(i);
        }
    }
    const char *item[4] = {0};
    item[0] = lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED);
    item[1] = lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NO);
    item[2] = lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NC);
    item[3] = lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS);
    lv_obj_t *msgbox = layout_senesor_setting_msgbox_create(lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_SETTINGS),
                                                            sensor_setting_msgbox_cancel_click,
                                                            sensor_setting_msgbox_confirm_click, sensor_setting_msgbox_click,
                                                            item);
    setting_sensor_settings_option_display(msgbox);
}

static void layout_sensor_setting_sensor_status_display(void)
{
    int obj_id[] = {
        setting_sensor_settings_obj_id_item1_cont,
        setting_sensor_settings_obj_id_item2_cont,
        setting_sensor_settings_obj_id_item3_cont,
        setting_sensor_settings_obj_id_item4_cont,
        setting_sensor_settings_obj_id_item5_cont,
        setting_sensor_settings_obj_id_item6_cont,
        setting_sensor_settings_obj_id_item7_cont,
        setting_sensor_settings_obj_id_item8_cont};

    int total = sizeof(obj_id) / (sizeof(int));
    lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_sensor_settings_obj_id_list);
    for (int i = 0; i < total; i++)
    {
        lv_obj_t *sub_btn = lv_obj_get_child_form_id(list, obj_id[i]);
        if (sub_btn)
        {
            lv_obj_t *sun_title = lv_obj_get_child_form_id(sub_btn, 1);

            if (user_data_get()->alarm.alarm_enable[i] == 0)
            {
                lv_label_set_text(sun_title, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED));
            }
            else if (user_data_get()->alarm.alarm_enable[i] == 1)
            {
                if (user_data_get()->alarm.alarm_enable_always[i])
                {
                    lv_label_set_text(sun_title, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NO_ALWAYS));
                }
                else
                {
                    lv_label_set_text(sun_title, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NO));
                }
            }
            else if (user_data_get()->alarm.alarm_enable[i] == 2)
            {
                if (user_data_get()->alarm.alarm_enable_always[i])
                {
                    lv_label_set_text(sun_title, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NC_ALWAYS));
                }
                else
                {
                    lv_label_set_text(sun_title, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NC));
                }
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
         setting_sensor_settings_list_click, 2},
        {0, 72, 928, 72,
         setting_sensor_settings_obj_id_item2_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_2, lang_str_get,
         user_data_get()->alarm.alarm_enable[1] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[1] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[1] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         setting_sensor_settings_list_click, 2},
        {0, 72 * 2, 928, 72,
         setting_sensor_settings_obj_id_item3_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_3, lang_str_get,
         user_data_get()->alarm.alarm_enable[2] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[2] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[2] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         setting_sensor_settings_list_click, 2},
        {0, 72 * 3, 928, 72,
         setting_sensor_settings_obj_id_item4_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_4, lang_str_get,
         user_data_get()->alarm.alarm_enable[3] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[3] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[3] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         setting_sensor_settings_list_click, 2},
        {0, 72 * 4, 928, 72,
         setting_sensor_settings_obj_id_item5_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_5, lang_str_get,
         user_data_get()->alarm.alarm_enable[4] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[4] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[4] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         setting_sensor_settings_list_click, 2},
        {0, 72 * 5, 928, 72,
         setting_sensor_settings_obj_id_item6_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_6, lang_str_get,
         user_data_get()->alarm.alarm_enable[5] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[5] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[5] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         setting_sensor_settings_list_click, 2},
        {0, 72 * 6, 928, 72,
         setting_sensor_settings_obj_id_item7_cont, 0, 1,
         SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_7, lang_str_get,
         user_data_get()->alarm.alarm_enable[6] == 0 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED : user_data_get()->alarm.alarm_enable[6] == 1 ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NO
                                                                                               : user_data_get()->alarm.alarm_enable[6] == 2   ? SETTING_SENSOR_USAGE_XLS_LANG_ID_NC
                                                                                                                                               : SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
         lang_str_get,
         setting_sensor_settings_list_click, 2},
    };

    lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_sensor_settings_obj_id_list);
    lv_common_style_set_common(list, setting_sensor_settings_obj_id_list, 48, 88, 928, 504, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

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
                                                        840, 10, 80, 48, main_list_group[i].img_id,
                                                        NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        layout_sensor_setting_sensor_status_display();
    }
}
static void sat_layout_enter(sensor_settings)
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
                              lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_SETTINGS), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
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

    /************************************************************
    ** 函数说明: 列表
    ** 作者: xiaoxiao
    ** 日期: 2023-04-29 08:30:08
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        layout_sensor_list_create();
    }
}
static void sat_layout_quit(sensor_settings)
{
}

sat_layout_create(sensor_settings);