#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
        setting_standby_screen_obj_id_title,
        setting_standby_screen_obj_id_cancel,

        setting_standby_screen_obj_id_mian_list,
        setting_standby_screen_obj_id_screen_off_cont,
        setting_standby_screen_obj_id_screen_off_title,
        setting_standby_screen_obj_id_screen_off_sub,
        setting_standby_screen_obj_id_screen_off_img,
        setting_standby_screen_obj_id_open_frame_cont,
        setting_standby_screen_obj_id_open_frame_title,
        setting_standby_screen_obj_id_open_frame_img,

        setting_standby_screen_obj_id_sub_list,

        setting_standby_screen_obj_id_wait_display_time_cont,
        setting_standby_screen_obj_id_wait_display_time_title,
        setting_standby_screen_obj_id_wait_display_time_sub,

        setting_standby_screen_obj_id_display_time_cont,
        setting_standby_screen_obj_id_display_time_title,
        setting_standby_screen_obj_id_display_time_sub,
        setting_standby_screen_obj_id_display_night_mode_cont,
        setting_standby_screen_obj_id_display_night_mode_title,
        setting_standby_screen_obj_id_display_night_mode_sub,
        setting_standby_screen_obj_id_display_item_cont,
        setting_standby_screen_obj_id_display_item_title,
        setting_standby_screen_obj_id_display_item_sub,
        setting_standby_screen_obj_id_background_cont,
        setting_standby_screen_obj_id_background_title,
        setting_standby_screen_obj_id_background_sub,

        setting_standby_screen_obj_id_msg_bg,
        setting_standby_screen_obj_id_msgbox,
        setting_standby_screen_obj_id_mesg,

        setting_standby_screen_off_checkbox1_cont,
        setting_standby_screen_off_checkbox1_label,
        setting_standby_screen_off_checkbox1_img,
        setting_standby_screen_off_checkbox2_cont,
        setting_standby_screen_off_checkbox2_label,
        setting_standby_screen_off_checkbox2_img,
        setting_standby_screen_off_checkbox3_cont,
        setting_standby_screen_off_checkbox3_label,
        setting_standby_screen_off_checkbox3_img,
        setting_standby_screen_off_checkbox4_cont,
        setting_standby_screen_off_checkbox4_label,
        setting_standby_screen_off_checkbox4_img,
        setting_standby_screen_off_obj_id_confirm,
        setting_standby_screen_off_obj_id_cancel,

};

static void setting_standby_screen_cancel_click(lv_event_t *e)
{
        sat_layout_goto(setting_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}
static void setting_standby_screen_main_checkbox_obj_display(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_mian_list);
        if (list == NULL)
        {
                return;
        }
        lv_obj_t *item = lv_obj_get_child_form_id(list, setting_standby_screen_obj_id_screen_off_cont);
        if (item == NULL)
        {
                return;
        }
        lv_obj_t *scr_of_img = lv_obj_get_child_form_id(item, setting_standby_screen_obj_id_screen_off_img);
        if (scr_of_img == NULL)
        {
                return;
        }

        item = lv_obj_get_child_form_id(list, setting_standby_screen_obj_id_open_frame_cont);
        lv_obj_t *frame_open_img = lv_obj_get_child_form_id(item, setting_standby_screen_obj_id_open_frame_img);
        if (frame_open_img == NULL)
        {
                return;
        }

        lv_obj_t *sub_list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_sub_list);
        if (sub_list == NULL)
        {
                return;
        }

        if (user_data_get()->display.standby_mode == 0)
        {
                lv_obj_set_style_bg_img_src(scr_of_img, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(frame_open_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);

                lv_obj_add_flag(sub_list, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(frame_open_img, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(scr_of_img, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);

                lv_obj_clear_flag(sub_list, LV_OBJ_FLAG_HIDDEN);
        }
}

static void setting_standby_msgbox_confirm_screen_time_save(lv_event_t *ev)
{
        lv_obj_t *parent = lv_obj_get_parent(lv_event_get_current_target(ev));
        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox1_cont), setting_standby_screen_off_checkbox1_img);
        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox2_cont), setting_standby_screen_off_checkbox2_img);
        lv_obj_t *check3 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox3_cont), setting_standby_screen_off_checkbox3_img);
        if (!strncmp((const char *)check1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->display.screen_off_time = 15;
        }
        else if (!strncmp((const char *)check2->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->display.screen_off_time = 30;
        }
        else if (!strncmp((const char *)check3->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                user_data_get()->display.screen_off_time = 60;
        }
        else
        {
                user_data_get()->display.screen_off_time = 180;
        }
        user_data_save(false, false);
        standby_timer_reset(user_data_get()->display.screen_off_time);
        standby_timer_restart(true, true);
        ;
}
static void setting_standby_screen_off_msgbox_confirm_click(lv_event_t *ev)
{

        setting_standby_msgbox_confirm_screen_time_save(ev);
        user_data_get()->display.standby_mode = 1;
        user_data_save(false, false);
        sat_layout_goto(setting_standby_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID)
}

static void setting_standby_wait_display_time_confirm_click(lv_event_t *ev)
{

        setting_standby_msgbox_confirm_screen_time_save(ev);
        user_data_get()->display.standby_mode = 0;
        user_data_save(false, false);
        sat_layout_goto(setting_standby_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID)
}

static void setting_standby_screen_off_msgbox_option_create(lv_obj_t *msgbox, lv_event_cb_t checkbox_cb)
{

        lv_common_img_text_btn_create(msgbox, setting_standby_screen_off_checkbox1_cont, 48, 46, 365, 48,
                                      checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, setting_standby_screen_off_checkbox1_label,
                                      lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_15SEC), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, setting_standby_screen_off_checkbox1_img,
                                      user_data_get()->display.screen_off_time == 15 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_img_text_btn_create(msgbox, setting_standby_screen_off_checkbox2_cont, 48, 102, 365, 48,
                                      checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, setting_standby_screen_off_checkbox2_label,
                                      lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_30SEC), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, setting_standby_screen_off_checkbox2_img,
                                      user_data_get()->display.screen_off_time == 30 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_img_text_btn_create(msgbox, setting_standby_screen_off_checkbox3_cont, 48, 158, 365, 48,
                                      checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, setting_standby_screen_off_checkbox3_label,
                                      lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_60SEC), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, setting_standby_screen_off_checkbox3_img,
                                      user_data_get()->display.screen_off_time == 60 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        lv_common_img_text_btn_create(msgbox, setting_standby_screen_off_checkbox4_cont, 48, 214, 365, 48,
                                      checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, setting_standby_screen_off_checkbox4_label,
                                      lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_180SEC), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 8, 32, 32, setting_standby_screen_off_checkbox4_img,
                                      user_data_get()->display.screen_off_time == 180 ? (char *)resource_ui_src_get("btn_radio_s.png") : (char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
}

static void setting_standby_screen_off_msgbox_click(lv_event_t *ev)
{

        lv_obj_t *obj = lv_event_get_current_target(ev);
        lv_obj_t *parent = lv_obj_get_parent(obj);
        lv_obj_t *check1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox1_cont), setting_standby_screen_off_checkbox1_img);
        lv_obj_t *check2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox2_cont), setting_standby_screen_off_checkbox2_img);
        lv_obj_t *check3 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox3_cont), setting_standby_screen_off_checkbox3_img);
        lv_obj_t *check4 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_standby_screen_off_checkbox4_cont), setting_standby_screen_off_checkbox4_img);
        if (obj->id == setting_standby_screen_off_checkbox1_cont)
        {

                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check4, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else if (obj->id == setting_standby_screen_off_checkbox2_cont)
        {
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check4, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else if (obj->id == setting_standby_screen_off_checkbox3_cont)
        {
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check4, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(check1, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check3, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(check4, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
        }
}

static void setting_standby_screen_off_msgbox_cancel_click(lv_event_t *ev)
{
        lv_obj_del(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_msg_bg));
}

/************************************************************
** 函数说明: 设置屏幕关闭
** 作者: xiaoxiao
** 日期: 2023-06-02 14:19:55
** 参数说明:
** 注意事项:
************************************************************/
static void setting_standby_screen_off_click(lv_event_t *e)
{
        lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_standby_screen_obj_id_msg_bg, setting_standby_screen_obj_id_msgbox, 282, 93, 460, 352);
        setting_standby_screen_off_msgbox_option_create(masgbox, setting_standby_screen_off_msgbox_click);
        setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, setting_standby_screen_off_obj_id_confirm, setting_standby_screen_off_obj_id_cancel, setting_standby_wait_display_time_confirm_click, setting_standby_screen_off_msgbox_cancel_click);
}

/************************************************************
** 函数说明: 设置open frame
** 作者: xiaoxiao
** 日期: 2023-07-06 15:14:04
** 参数说明:
** 注意事项:
************************************************************/
static void setting_standby_open_frame_msgbox_confirm_click(lv_event_t *ev)
{

        user_data_get()->display.standby_mode = 1;
        user_data_save(false, false);
        setting_standby_screen_main_checkbox_obj_display();
        sat_layout_goto(setting_standby_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID)
}

/************************************************************
** 函数说明: 取消设置open frame
** 作者: xiaoxiao
** 日期: 2023-07-06 15:15:11
** 参数说明:
** 注意事项:
************************************************************/
static void setting_standby_open_frame_msgbox_cancel_click(lv_event_t *ev)
{

        setting_msgdialog_msg_del(setting_standby_screen_obj_id_msg_bg);
}

/************************************************************
** 函数说明: 点击按键open frame
** 作者: xiaoxiao
** 日期: 2023-07-06 15:14:43
** 参数说明:
** 注意事项:
************************************************************/
static void setting_standby_open_frame_click(void)
{
        lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_standby_screen_obj_id_msg_bg, setting_standby_screen_obj_id_msgbox, 282, 93, 460, 352);
        setting_msgdialog_msg_create(masgbox, setting_standby_screen_obj_id_mesg, lang_str_get(SETTING_STANDBY_SCREEN_XLS_LANG_ID_OPEN_FRAME_APPLY_CONFIRM), 0, 90, 460, 120, false);
        // setting_standby_screen_off_msgbox_option_create(masgbox,setting_standby_screen_off_msgbox_click);
        setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, setting_standby_screen_off_obj_id_confirm, setting_standby_screen_off_obj_id_cancel, setting_standby_open_frame_msgbox_confirm_click, setting_standby_open_frame_msgbox_cancel_click);
}

static void setting_standby_screen_main_list_click(lv_event_t *e)
{
        lv_obj_t *item = lv_event_get_current_target(e);
        if (item == NULL)
        {
                return;
        }
        if (item->id == setting_standby_screen_obj_id_screen_off_cont)
        {

                setting_standby_screen_off_click(e);
        }
        else if (user_data_get()->display.standby_mode == 0)
        {
                if (user_data_get()->always_monitoring)
                {
                        setting_standby_open_frame_click();
                }
                else
                {
                        user_data_get()->display.standby_mode = 1;
                        user_data_save(false, false);
                        setting_standby_screen_main_checkbox_obj_display();
                        sat_layout_goto(setting_standby_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID)
                }
        }
}

static void setting_standby_screen_off_sub_display(lv_obj_t *list)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, setting_standby_screen_obj_id_screen_off_cont), setting_standby_screen_obj_id_screen_off_sub);
        if (user_data_get()->display.screen_off_time == 15)
        {
                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_15SEC));
        }
        else if (user_data_get()->display.screen_off_time == 30)
        {
                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_30SEC));
        }
        else if (user_data_get()->display.screen_off_time == 60)
        {
                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_60SEC));
        }
        else if (user_data_get()->display.screen_off_time == 180)
        {
                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_180SEC));
        }
}

static lv_obj_t *setting_standby_screen_main_list_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 928, 88,
             setting_standby_screen_obj_id_screen_off_cont, setting_standby_screen_obj_id_screen_off_title, setting_standby_screen_obj_id_screen_off_sub,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_SCREEN_OFF, lang_str_get,
             SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_15SEC, lang_str_get,
             setting_standby_screen_main_list_click, setting_standby_screen_obj_id_screen_off_img},
            {0, 88, 928, 88,
             setting_standby_screen_obj_id_open_frame_cont, setting_standby_screen_obj_id_open_frame_title, -1,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_OPEN_FRAME, lang_str_get,
             -1, NULL,
             setting_standby_screen_main_list_click,
             setting_standby_screen_obj_id_open_frame_img}};

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_mian_list);
        lv_common_style_set_common(list, setting_standby_screen_obj_id_mian_list, 48, 88, 928, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[i].x, main_list_group[i].y, main_list_group[i].w, main_list_group[i].h,
                                                                main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                42, 25, 576, 43, main_list_group[i].title_id,
                                                                main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                42, 52, 576, 29, main_list_group[i].sub_id,
                                                                main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 28, 32, 32, main_list_group[i].img_id,
                                                                resource_ui_src_get("btn_radio_n.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        setting_standby_screen_off_sub_display(list);
        return list;
}
static void setting_standby_screen_sub_list_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);
        if (obj == NULL)
        {
                return;
        }
        if (obj->id == setting_standby_screen_obj_id_wait_display_time_cont)
        {
                // sat_layout_goto(setting_frame_display_time, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
                lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_standby_screen_obj_id_msg_bg, setting_standby_screen_obj_id_msgbox, 282, 93, 460, 352);
                setting_standby_screen_off_msgbox_option_create(masgbox, setting_standby_screen_off_msgbox_click);
                setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, setting_standby_screen_off_obj_id_confirm, setting_standby_screen_off_obj_id_cancel, setting_standby_screen_off_msgbox_confirm_click, setting_standby_screen_off_msgbox_cancel_click);
        }
        if (obj->id == setting_standby_screen_obj_id_display_time_cont)
        {
                sat_layout_goto(setting_frame_display_time, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
        else if (obj->id == setting_standby_screen_obj_id_display_night_mode_cont)
        {
                sat_layout_goto(setting_frame_night_mode, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
        else if (obj->id == setting_standby_screen_obj_id_display_item_cont)
        {
                sat_layout_goto(setting_frame_item, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
        else if (obj->id == setting_standby_screen_obj_id_background_cont)
        {
                sat_layout_goto(setting_frame_backgorund, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
}

static void setting_standby_screen_sub_list_display(lv_obj_t *parent)
{
        lv_obj_t *cont = lv_obj_get_child_form_id(parent, setting_standby_screen_obj_id_display_time_cont);
        lv_obj_t *obj = NULL;
        if (cont != NULL)
        {
                obj = lv_obj_get_child_form_id(cont, setting_standby_screen_obj_id_display_time_sub);
                if (obj != NULL)
                {

                        if (user_data_get()->display.frame_time_en == false)
                        {
                                lv_label_set_text(obj, lang_str_get(SETTING_STANDBY_SCREEN_XLS_LANG_ID_ALWAYS));
                        }
                        else
                        {
                                int s_hour = user_data_get()->display.frame_time_start / 60;
                                int s_min = user_data_get()->display.frame_time_start % 60;
                                int e_hour = user_data_get()->display.frame_time_end / 60;
                                int e_min = user_data_get()->display.frame_time_end % 60;
                                lv_label_set_text_fmt(obj, "%02d:%02d-%02d:%02d", s_hour, s_min, e_hour, e_min);
                        }
                }
        }
        cont = lv_obj_get_child_form_id(parent, setting_standby_screen_obj_id_wait_display_time_cont);
        if (cont != NULL)
        {
                obj = lv_obj_get_child_form_id(cont, setting_standby_screen_obj_id_wait_display_time_sub);
                if (obj != NULL)
                {
                        if (user_data_get()->display.screen_off_time == 15)
                        {
                                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_15SEC));
                        }
                        else if (user_data_get()->display.screen_off_time == 30)
                        {
                                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_30SEC));
                        }
                        else if (user_data_get()->display.screen_off_time == 60)
                        {
                                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_60SEC));
                        }
                        else if (user_data_get()->display.screen_off_time == 180)
                        {
                                lv_label_set_text(obj, lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_180SEC));
                        }
                }
        }
        cont = lv_obj_get_child_form_id(parent, setting_standby_screen_obj_id_display_night_mode_cont);
        if (cont != NULL)
        {
                obj = lv_obj_get_child_form_id(cont, setting_standby_screen_obj_id_display_night_mode_sub);
                if (obj != NULL)
                {
                        if (user_data_get()->display.night_mode == false)
                        {
                                lv_label_set_text(obj, lang_str_get(RECORDING_XLS_LANG_ID_SAVE_OFF));
                        }
                        else
                        {
                                int s_hour = user_data_get()->display.night_time_start / 60;
                                int s_min = user_data_get()->display.night_time_start % 60;
                                int e_hour = user_data_get()->display.night_time_end / 60;
                                int e_min = user_data_get()->display.night_time_end % 60;
                                lv_label_set_text_fmt(obj, "%02d:%02d-%02d:%02d", s_hour, s_min, e_hour, e_min);
                        }
                }
        }

        cont = lv_obj_get_child_form_id(parent, setting_standby_screen_obj_id_display_item_cont);
        if (cont != NULL)
        {
                obj = lv_obj_get_child_form_id(cont, setting_standby_screen_obj_id_display_item_sub);
                if (obj != NULL)
                {
                        int data = user_data_get()->display.frame_list;
                        int count = 0;
                        for (int i = 0; i < 5; i++)
                        {
                                if (data & (0x01 << i))
                                {
                                        count++;
                                }
                        }
                        lv_label_set_text(obj, lang_str_get(SETTING_STANDBY_SCREEN_XLS_LANG_ID_0_ITEM + count));
                }
        }

        cont = lv_obj_get_child_form_id(parent, setting_standby_screen_obj_id_background_cont);
        if (cont != NULL)
        {
                obj = lv_obj_get_child_form_id(cont, setting_standby_screen_obj_id_background_sub);
                if (obj != NULL)
                {
                        if (user_data_get()->display.frame_background)
                        {
                                lv_label_set_text(obj, lang_str_get(SETTING_STANDBY_SCREEN_XLS_LANG_ID_RANDOM_IMAGE));
                        }
                        else
                        {
                                lv_label_set_text(obj, lang_str_get(SETTING_STANDBY_SCREEN_XLS_LANG_ID_BASIC));
                        }
                }
        }
}
static lv_obj_t *setting_standby_screen_sub_list_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 836, 72,
             setting_standby_screen_obj_id_wait_display_time_cont, setting_standby_screen_obj_id_wait_display_time_title, setting_standby_screen_obj_id_wait_display_time_sub,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_WAIT_DISPLAY_TIME, lang_str_get,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_WAIT_DISPLAY_TIME, lang_str_get,
             setting_standby_screen_sub_list_click},
            //     {0, 72, 836, 72,
            //      setting_standby_screen_obj_id_display_time_cont, setting_standby_screen_obj_id_display_time_title, setting_standby_screen_obj_id_display_time_sub,
            //      SETTING_STANDBY_SCREEN_XLS_LANG_ID_DISPLAY_TIME, lang_str_get,
            //      SETTING_STANDBY_SCREEN_XLS_LANG_ID_ALWAYS, lang_str_get,
            //      setting_standby_screen_sub_list_click},
            {0, 72 * 1, 836, 72,
             setting_standby_screen_obj_id_display_night_mode_cont, setting_standby_screen_obj_id_display_night_mode_title, setting_standby_screen_obj_id_display_night_mode_sub,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_NIGHT_MODE, lang_str_get,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_ADJUSTS_THE_BRIGHTNESS, lang_str_get,
             setting_standby_screen_sub_list_click},
            {0, 72 * 2, 836, 72,
             setting_standby_screen_obj_id_display_item_cont, setting_standby_screen_obj_id_display_item_title, setting_standby_screen_obj_id_display_item_sub,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_ITEM_DISPLAY, lang_str_get,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_1_ITEM, lang_str_get,
             setting_standby_screen_sub_list_click},
            {0, 72 * 3, 836, 72,
             setting_standby_screen_obj_id_background_cont, setting_standby_screen_obj_id_background_title, setting_standby_screen_obj_id_background_sub,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_BACKROUND, lang_str_get,
             SETTING_STANDBY_SCREEN_XLS_LANG_ID_RANDOM_IMAGE, lang_str_get,
             setting_standby_screen_sub_list_click},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_sub_list);
        lv_common_style_set_common(list, setting_standby_screen_obj_id_sub_list, 94, 264, 836, 314, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

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
        setting_standby_screen_sub_list_display(list);
        return list;
}
static void sat_layout_enter(setting_standby_screen)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 标题创建
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SCREEN_XLS_LANG_ID_LCD_STANDBY_SCREEN), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-3 14:13:25
        ** 说明: 返回按钮
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_standby_screen_obj_id_cancel, 35, 15, 48, 48,
                                         setting_standby_screen_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 主列表创建
         ***********************************************/
        {
                setting_standby_screen_main_list_create();
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 子列表创建
         ***********************************************/
        {
                setting_standby_screen_sub_list_create();
        }

        setting_standby_screen_main_checkbox_obj_display();
}
static void sat_layout_quit(setting_standby_screen)
{
}

sat_layout_create(setting_standby_screen);