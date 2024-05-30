#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
        setting_storage_space_obj_id_title,
        setting_storage_space_obj_id_cancel,
        setting_storage_space_obj_id_internal,
        setting_storage_space_obj_id_internal_del,
        setting_storage_space_obj_id_internal_arc,
        setting_storage_space_obj_id_internal_remaining_data,
        setting_storage_space_obj_id_internal_per_label,

        setting_storage_space_obj_id_external,
        setting_storage_space_obj_id_external_del,
        setting_storage_space_obj_id_external_arc,
        setting_storage_space_obj_id_external_remaining_data,
        setting_storage_space_obj_id_external_per_label,
        setting_storage_space_obj_id_external_format,

        setting_storage_space_obj_id_ver_line,

        setting_storage_space_obj_id_extern_no_exit,

        setting_storage_space_obj_id_msgbox_cont,
        setting_storage_space_obj_id_msgbox_parent,
        setting_storage_space_obj_id_msgbox_title,
        setting_storage_space_obj_id_msgbox_text,
        setting_storage_space_obj_id_msgbox_check_1,
        setting_storage_space_obj_id_msgbox_check_1_text,
        setting_storage_space_obj_id_msgbox_check_1_img,
        setting_storage_space_obj_id_msgbox_check_2,
        setting_storage_space_obj_id_msgbox_check_2_text,
        setting_storage_space_obj_id_msgbox_check_2_img,
        setting_storage_space_obj_id_msgbox_confirm,
        setting_storage_space_obj_id_msgbox_confirm_img,
        setting_storage_space_obj_id_msgbox_cancel,
        setting_storage_space_obj_id_msgbox_cancel_img,

        setting_storage_space_obj_id_format_msgbox_cont,
        setting_storage_space_obj_id_format_msgbox,
        setting_storage_space_obj_id_format_text,
        setting_storage_space_obj_id_format_process,
        setting_storage_space_obj_id_format_completed,

};
static void setting_storage_space_cancel_click(lv_event_t *e)
{
        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static lv_obj_t *setting_storage_space_msgbox_create(const char *title, const char *msg, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb, bool is_checkbox)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_msgbox_cont, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_storage_space_obj_id_msgbox_parent, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, setting_storage_space_obj_id_msgbox_title, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        if (is_checkbox == true)
        {
                lv_common_img_text_btn_create(msgbox, setting_storage_space_obj_id_msgbox_check_1, 48, 110, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_storage_space_obj_id_msgbox_check_1_text,
                                              lang_str_get(RECORDING_XLS_LANG_ID_SAVE_PICTURE), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_storage_space_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get("btn_radio_s.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_storage_space_obj_id_msgbox_check_2, 48, 166, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_storage_space_obj_id_msgbox_check_2_text,
                                              lang_str_get(RECORDING_XLS_LANG_ID_SAVE_VIDEO), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_storage_space_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        else
        {
                lv_common_text_create(msgbox, setting_storage_space_obj_id_msgbox_text, 35, 133, 392, 71,
                                      NULL, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x505050,
                                      0, 0, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x505050,
                                      msg, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
        }

        lv_common_img_btn_create(msgbox, setting_storage_space_obj_id_msgbox_cancel, 0, 281, 230, 62,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, setting_storage_space_obj_id_msgbox_confirm, 230, 281, 230, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}
static void setting_storage_space_msgbox_cancel_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_storage_space_obj_id_msgbox_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}
static void setting_storage_space_msgbox_internale_confirm_click(lv_event_t *e)
{
        // lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_storage_space_obj_id_msgbox_cont);
        // if (obj == NULL)
        // {
        //         return;
        // }
        media_file_delete_all(FILE_TYPE_FLASH_PHOTO, true);
        sat_layout_goto(setting_storage_space, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_storage_space_msgbox_external_confirm_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_storage_space_obj_id_msgbox_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_t *msgbox = lv_obj_get_child_form_id(obj, setting_storage_space_obj_id_msgbox_parent);
        lv_obj_t *chckbox1_img = lv_obj_get_child_form_id(lv_obj_get_child_form_id(msgbox, setting_storage_space_obj_id_msgbox_check_1), setting_storage_space_obj_id_msgbox_check_1_img);
        if (strncmp(chckbox1_img->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
        {
                media_file_delete_all(FILE_TYPE_PHOTO, false);
        }
        else
        {
                media_file_delete_all(FILE_TYPE_VIDEO, false);
        }
        sat_layout_goto(setting_storage_space, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_storage_space_msgbox_external_checkbox_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_event_get_current_target(e);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *check_obj, *discheck_obj;
        if (parent->id == setting_storage_space_obj_id_msgbox_check_1)
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_storage_space_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_storage_space_obj_id_msgbox_check_2);
                discheck_obj = lv_obj_get_child_form_id(parent, setting_storage_space_obj_id_msgbox_check_2_img);
        }
        else
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_storage_space_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_storage_space_obj_id_msgbox_check_1);
                discheck_obj = lv_obj_get_child_form_id(parent, setting_storage_space_obj_id_msgbox_check_1_img);
        }

        if ((check_obj == NULL) || (discheck_obj == NULL) || (check_obj->bg_img_src == NULL) || (discheck_obj->bg_img_src == NULL))
        {
                return;
        }

        lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(discheck_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
}

static lv_obj_t *setting_storage_space_msgbox_format_process_display(lv_obj_t *parent)
{

        static lv_style_t style_bg;
        static lv_style_t style_indic;

        lv_style_init(&style_bg);
        lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_border_width(&style_bg, 2);
        lv_style_set_pad_all(&style_bg, 0); /*To make the indicator smaller*/
        lv_style_set_radius(&style_bg, 0);

        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_radius(&style_indic, 3);

        lv_obj_t *bar = lv_bar_create(parent);
        lv_obj_set_id(bar, setting_storage_space_obj_id_format_process);
        lv_obj_remove_style_all(bar); /*To have a clean start*/
        lv_obj_add_style(bar, &style_bg, 0);
        lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);

        lv_common_style_set_common(bar, setting_storage_space_obj_id_format_process, 50, 220, 360, 24, LV_ALIGN_DEFAULT, LV_PART_MAIN);
        lv_bar_set_range(bar, 0, 100);
        lv_bar_set_value(bar, 20, LV_ANIM_ON);
        return bar;
}

static void setting_storage_space_msgbox_external_fmatsd_finish_confirm(lv_event_t *ev)
{
        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}

static void format_sd_process_timer(lv_timer_t *ptime)
{
        lv_obj_t *bar = ptime->user_data;
        int value = lv_bar_get_value(bar);
        if (value == 20) //
        {
                lv_bar_set_value(bar, value * 3 / 2, LV_ANIM_OFF);
        }
        else if (media_format_sd_state() == false)
        {
                value = lv_bar_get_value(bar);
                lv_bar_set_value(bar, value * 3 / 2, LV_ANIM_OFF);
        }
        else
        {
                lv_bar_set_value(bar, 100, LV_ANIM_OFF);
                lv_timer_del(ptime);
                lv_obj_t *msgbox = lv_obj_get_parent(bar);
                lv_obj_clean(msgbox);
                setting_msgdialog_msg_create(msgbox, setting_storage_space_obj_id_format_text, lang_str_get(SD_XLS_LANG_ID_SD_FORMAT_COMPLETE), 0, 60, 460, 120, false);
                setting_msgdialog_msg_confirm_btn_create(msgbox, setting_storage_space_obj_id_format_completed, setting_storage_space_msgbox_external_fmatsd_finish_confirm);
        }
}
static void setting_storage_space_msgbox_external_fmatsd_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_storage_space_obj_id_msgbox_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
        lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_storage_space_obj_id_format_msgbox_cont, setting_storage_space_obj_id_format_msgbox, 282, 143, 460, 283);
        setting_msgdialog_msg_create(masgbox, setting_storage_space_obj_id_format_text, lang_str_get(SD_XLS_LANG_ID_FORMATING_SD), 0, 60, 460, 120, false);
        lv_obj_t *bar = setting_storage_space_msgbox_format_process_display(masgbox);
        lv_sat_timer_create(format_sd_process_timer, 500, bar);
        media_format_sd();
}
static void setting_storage_space_internal_del_click(lv_event_t *e)
{
        setting_storage_space_msgbox_create(lang_str_get(LAYOUT_CALL_LOG_XLS_LANG_ID_DEL), lang_str_get(SETTING_STORAGE_XLS_LANG_ID_WOULD_YOU_LIKE_DEL),
                                            setting_storage_space_msgbox_cancel_click, setting_storage_space_msgbox_internale_confirm_click, NULL, false);
}
static void setting_storage_space_external_del_click(lv_event_t *e)
{
        setting_storage_space_msgbox_create(lang_str_get(SD_XLS_LANG_ID_FORMAT), lang_str_get(SETTING_STORAGE_XLS_LANG_ID_WOULD_YOU_LIKE_FORMAT),
                                            setting_storage_space_msgbox_cancel_click, setting_storage_space_msgbox_external_confirm_click, setting_storage_space_msgbox_external_checkbox_click, true);
}
static void setting_storage_space_fmatsd_click(lv_event_t *e)
{
        setting_storage_space_msgbox_create(lang_str_get(SD_XLS_LANG_ID_FORMAT), lang_str_get(SETTING_STORAGE_XLS_LANG_ID_WOULD_YOU_LIKE_FORMAT),
                                            setting_storage_space_msgbox_cancel_click, setting_storage_space_msgbox_external_fmatsd_click, NULL, false);
}
static lv_obj_t *setting_storage_space_arc_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                                  uint32_t bg_color, uint32_t indic_color, int bg_arc_r, int indic_r, int value)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);
        if (obj != NULL)
        {
                return obj;
        }
        obj = lv_arc_create(parent);
        lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        lv_common_style_set_event(obj, NULL, false, LV_OPA_TRANSP, 0xd9d9d9, LV_PART_MAIN, LV_OPA_TRANSP, 0x3a7dff, LV_PART_INDICATOR);

        lv_obj_set_style_arc_width(obj, bg_arc_r, LV_PART_MAIN);
        lv_obj_set_style_arc_width(obj, indic_r, LV_PART_INDICATOR);

        lv_obj_set_style_arc_opa(obj, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_arc_color(obj, lv_color_hex(bg_color), LV_PART_MAIN);

        lv_obj_set_style_arc_opa(obj, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_set_style_arc_color(obj, lv_color_hex(indic_color), LV_PART_INDICATOR);

        lv_arc_set_rotation(obj, 270);
        lv_arc_set_bg_angles(obj, 0, 360);
        lv_arc_set_range(obj, 0, 360);
        lv_arc_set_value(obj, value);

        return obj;
}
static void setting_storage_space_animation(void *obj, int32_t v)
{
        char buffer[10] = {0};
        sprintf(buffer, "%d%%", (360 - v) * 100 / 360);
        lv_label_set_text(obj, buffer);

        lv_obj_t *arc = lv_obj_get_parent(obj);
        lv_arc_set_value(arc, v);
}

static void setting_storage_space_sd_status_channge_callback(void)
{
        sat_layout_goto(setting_storage_space, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}

static void sat_layout_enter(setting_storage_space)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 标题显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_GENERAL_XLS_LANG_ID_STORAGE_SPACE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 上一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_cancel, 35, 15, 48, 48,
                                         setting_storage_space_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: internal storage
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_internal, 118, 93, 277, 36,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_STORAGE_XLS_LANG_ID_INTERNAL_STORAGE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);

                lv_common_text_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_external, 630, 93, 277, 36,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_STORAGE_XLS_LANG_ID_EXTERNAL_STORAGE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: internal storage del
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_internal_del, 455, 90, 32, 32,
                                                         setting_storage_space_internal_del_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_list_delete.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                lv_obj_set_ext_click_area(obj, 30);
                if (0)
                {
                        obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_external_del, 968, 90, 32, 32,
                                                       setting_storage_space_external_del_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       resource_ui_src_get("btn_list_delete.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                        lv_obj_set_ext_click_area(obj, 30);
                }
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 垂直线
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_ver_line, 511, 80, 2, 520,
                                         NULL, true, LV_OPA_COVER, 0x323237, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: internal storage arc
         ***********************************************/
        {
                unsigned long long total = 0, free = 0;
                unsigned long long use_data = 360;
                if (media_capacity_get(FILE_TYPE_FLASH_PHOTO, &total, &free) == true)
                {
                        use_data = (360 * (total - free)) / (total + 0.1);
                }
                lv_obj_t *obj = setting_storage_space_arc_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_internal_arc, 127, 192, 260, 260,
                                                                 0xd9d9d9, 0x3a7dff, 45, 45, use_data);

                lv_common_text_create(obj, setting_storage_space_obj_id_internal_remaining_data, 77, 65, 104, 52,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_STORAGE_XLS_LANG_ID_REMAINING_DATA), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

                char buffer[64] = {0};
                sprintf(buffer, "%llu%%", (360 - use_data) * 100 / 360);
                lv_obj_t *label = lv_common_text_create(obj, setting_storage_space_obj_id_internal_per_label, 69, 135, 121, 63,
                                                        NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        buffer, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large_s);

                lv_anim_t a;
                lv_anim_init(&a);
                lv_anim_set_var(&a, label);
                lv_anim_set_exec_cb(&a, setting_storage_space_animation);
                lv_anim_set_time(&a, 1500);
                lv_anim_set_repeat_count(&a, 1);
                lv_anim_set_repeat_delay(&a, 0);
                lv_anim_set_values(&a, 0, use_data);
                lv_anim_start(&a);
        }
        if (media_sdcard_insert_check() != SD_STATE_UNPLUG)
        {
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: internal storage arc
                 ***********************************************/
                {
                        unsigned long long total = 0, free = 0;
                        unsigned long long use_data = 360;
                        if (media_capacity_get(FILE_TYPE_VIDEO, &total, &free) == true)
                        {
                                use_data = (360 * (total - free)) / (total + 0.1);
                        }
                        lv_obj_t *obj = setting_storage_space_arc_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_external_arc, 640, 192, 260, 260,
                                                                         0xd9d9d9, 0x3a7dff, 45, 45, use_data);

                        lv_common_text_create(obj, setting_storage_space_obj_id_external_remaining_data, 77, 65, 104, 52,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(SETTING_STORAGE_XLS_LANG_ID_REMAINING_DATA), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

                        char buffer[64] = {0};
                        sprintf(buffer, "%llu%%", (360 - use_data) * 100 / 360);
                        lv_obj_t *label = lv_common_text_create(obj, setting_storage_space_obj_id_external_per_label, 69, 135, 121, 63,
                                                                NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                buffer, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large_s);

                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, label);
                        lv_anim_set_exec_cb(&a, setting_storage_space_animation);
                        lv_anim_set_time(&a, 1500);
                        lv_anim_set_repeat_count(&a, 1);
                        lv_anim_set_repeat_delay(&a, 0);
                        lv_anim_set_values(&a, 0, use_data);
                        lv_anim_start(&a);
                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:46:56
                         ** 说明: sd format
                         ***********************************************/
                        {
                                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_external_format, 513, 528, 511, 72,
                                                                      setting_storage_space_fmatsd_click, LV_OPA_COVER, 0x00a8ff, LV_OPA_COVER, 0x00a8ff,
                                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                      lang_str_get(SETTING_STORAGE_XLS_LANG_ID_SD_FORMAT), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);

                                lv_obj_set_style_pad_top(obj, 15, LV_PART_MAIN);
                        }
                }
        }
        else
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_storage_space_obj_id_extern_no_exit, 540, 336, 460, 40,
                                      NULL, LV_OPA_TRANSP, 0x00a8ff, LV_OPA_TRANSP, 0x00a8ff,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_STORAGE_XLS_LANG_ID_SD_NOT_RECOGNIZED), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
        }

        sd_state_channge_callback_register(setting_storage_space_sd_status_channge_callback);
}
static void sat_layout_quit(setting_storage_space)
{
        sd_state_channge_callback_register(sd_state_change_default_callback);
}

sat_layout_create(setting_storage_space)