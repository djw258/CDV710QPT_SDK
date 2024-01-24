#include "layout_define.h"
#include "layout_setting_general.h"
typedef enum
{
        layout_alarm_list_obj_id_title,
        layout_alarm_list_obj_id_cancel,
        layout_alarm_list_obj_id_del,
        layout_alarm_list_obj_id_list,
        layout_alrm_list_obj_id_msgbox_bg_cont,
} alarm_list_scr_act_obj_id;

typedef enum
{
        layout_alarm_list_obj_id_type,
        layout_alarm_list_obj_id_ch,
        layout_alarm_list_obj_id_time,
        layout_alarm_list_obj_id_img,
} layout_alarm_list_list_obj_id;

typedef enum
{
        layout_alarm_list_obj_id_msgbox_cont,
        layout_alarm_list_obj_id_msgbox_title,
        layout_alarm_list_obj_id_msgbox_cancel,
        layout_alarm_list_obj_id_msgbox_confirm,
} alrm_list_msgbox_bg_cont_obj_id;

static void layout_alarm_list_back_obj_click(lv_event_t *ev)
{
        sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void layout_alarm_list_msgbox_del(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_alrm_list_obj_id_msgbox_bg_cont);

        if (parent != NULL)
        {
                lv_obj_del(parent);
        }
}
/************************************************************
** 函数说明: 取消输出警报日志
** 作者: xiaoxiao
** 日期: 2023-05-06 21:58:30
** 参数说明:
** 注意事项:
************************************************************/
static void layout_alarm_list_msgbox_cancel_click(lv_event_t *e)
{
        layout_alarm_list_msgbox_del();
}

/************************************************************
** 函数说明: 传感器状态设置保存
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void layout_alarm_list_msgbox_confirm_click(lv_event_t *e)
{
        alarm_list_del_all();
        sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static lv_obj_t *layout_alarm_list_msgbox_create(const char *title, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_alrm_list_obj_id_msgbox_bg_cont, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, layout_alarm_list_obj_id_msgbox_cont, 282, 108, 460, 283,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, layout_alarm_list_obj_id_msgbox_title, 0, 106, 460, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_NONE, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_NONE, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_normal);

        lv_common_img_btn_create(msgbox, layout_alarm_list_obj_id_msgbox_cancel, 0, 211, 230, 72,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, layout_alarm_list_obj_id_msgbox_confirm, 230, 211, 230, 72,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return NULL;
}

/************************************************************
** 函数说明: 列表信息删除
** 作者: xiaoxiao
** 日期: 2023-05-05 23:21:12
** 参数说明:
** 注意事项:
************************************************************/
static void layout_alarm_list_del_btn_obj_click(lv_event_t *ev)
{

        layout_alarm_list_msgbox_create(lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_DELETED_ALL),
                                        layout_alarm_list_msgbox_cancel_click, layout_alarm_list_msgbox_confirm_click, NULL);
}

static void alarm_list_display(lv_obj_t *parent, int type, int ch, struct tm tm)
{
        // lv_obj_t * list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(),layout_alarm_list_obj_id_list);
        lv_obj_t *type_lebel = lv_obj_get_child_form_id(parent, layout_alarm_list_obj_id_type);
        lv_obj_t *ch_label = lv_obj_get_child_form_id(parent, layout_alarm_list_obj_id_ch);
        lv_obj_t *time_label = lv_obj_get_child_form_id(parent, layout_alarm_list_obj_id_time);
        lv_obj_t *img = lv_obj_get_child_form_id(parent, layout_alarm_list_obj_id_img);
        if (type == 0)
        {
                lv_label_set_text(type_lebel, lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_OCCURS));
        }
        else if (type == 1)
        {
                lv_label_set_text(type_lebel, lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_STOP));
        }
        else if (type == 2)
        {
                lv_label_set_text(type_lebel, lang_str_get(/* LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_RETURN*/ LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_STOP));
        }
        else if (type == 3)
        {
                lv_label_set_text(type_lebel, lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY));
        }
        else if (type == 4)
        {
                lv_label_set_text(type_lebel, lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY_STOP));
        }
        else if (type == 5)
        {
                lv_label_set_text(type_lebel, lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY_STOP /* LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY_RETURN */));
        }
        if ((type == 0) || (type == 1) || (type == 2))
        {
                lv_label_set_text(ch_label, lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_INDOOR_OPERATION));
        }
        else
        {
                lv_label_set_text(ch_label, lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_SENSOR1 + ch));
        }
        lv_obj_set_style_bg_img_src(img, (type == 0 || type == 1 || type == 2) ? resource_ui_src_get("ic_list_emergency_01.png") : resource_ui_src_get("ic_list_emergency_02.png"), LV_PART_MAIN);
        lv_label_set_text_fmt(time_label, "%04d.%02d.%02d %02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/************************************************************
** 函数说明: 警报信息列表创建
** 作者: xiaoxiao
** 日期: 2023-05-05 21:14:42
** 参数说明:
** 注意事项:
************************************************************/
static void alarm_list_info_create(void)
{
        int total = 0;
        if ((alarm_list_total_get(&total) == false) || (total < 1))
        {
                return;
        }
        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), layout_alarm_list_obj_id_list);
        lv_common_style_set_common(list, layout_alarm_list_obj_id_list, 48, 96, 918, 504, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        for (int i = total - 1; i >= 0; i--)
        {
                int type;
                int ch = 0;
                struct tm tm;
                if (alarm_list_get(i, &type, &ch, &tm) == false)
                {
                        break;
                }
                //("===========\n");
                // char tm_buf[128];
                // sprintf(tm_buf, "%04d.%02d.%02d %02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
                lv_obj_t *parent = lv_common_setting_btn_title_sub_info_img_create(list, i, 0, 96 * i, 918, 96,
                                                                                   NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                                   0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                                   0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                                   60, 17, 576, 43, layout_alarm_list_obj_id_type,
                                                                                   "", 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                   60, 42, 576, 54, layout_alarm_list_obj_id_ch,
                                                                                   "", 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT_CENTER, lv_font_small,
                                                                                   700, 24, 200, 72, layout_alarm_list_obj_id_time,
                                                                                   "", 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                   0, 24, 48, 48, layout_alarm_list_obj_id_img,
                                                                                   resource_ui_src_get("ic_list_emergency_01.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                alarm_list_display(parent, type, ch, tm);
        }
}
static void sat_layout_enter(alarm_list)
{
        /************************************************************
        ** 函数说明:
        ** 作者: xiaoxiao
        ** 日期: 2023-04-26 11:43:19
        ** 参数说明: 标题显示
        ** 注意事项:
        ************************************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), layout_alarm_list_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_LOG), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /************************************************************
        ** 函数说明: 退回上一步
        ** 作者: xiaoxiao
        ** 日期: 2023-04-26 11:46:55
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_alarm_list_obj_id_cancel, 35, 15, 48, 48,
                                         layout_alarm_list_back_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /************************************************************
        ** 函数说明: 删除警报猎鸟信息
        ** 作者: xiaoxiao
        ** 日期: 2023-04-26 11:54:47
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_alarm_list_obj_id_del, 952, 16, 48, 48,
                                         layout_alarm_list_del_btn_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_delete.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /************************************************************
        ** 函数说明: 警报列表信息显示
        ** 作者: xiaoxiao
        ** 日期: 2023-04-26 12:00:29
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                alarm_list_info_create();
        }
}

static void sat_layout_quit(alarm_list)
{
}

sat_layout_create(alarm_list);