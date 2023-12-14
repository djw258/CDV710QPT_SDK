#include "layout_define.h"
#include "layout_setting_general.h"
#include "common/commax_websocket.h"
enum
{
        setting_password_obj_id_reset_cont,
        setting_password_obj_id_title,
        setting_password_obj_id_cancel,

        setting_password_obj_id_list,
        setting_password_obj_id_reset_household_cont,
        setting_password_obj_id_reset_household_title,

        setting_password_obj_id_reset_common_cont,
        setting_password_obj_id_reset_common_title,

        setting_password_obj_id_modiy_cont,
        setting_password_obj_id_modiy_cancel,
        setting_password_obj_id_modiy_input_btnatirx,
        setting_password_obj_id_modiy_input_keyboard_cover,
        setting_password_obj_id_modiy_password_head_label,
        setting_password_obj_id_modiy_password_reset_label,
        setting_password_obj_id_modiy_password_new_label,
        setting_password_obj_id_modiy_passowrd_verify_label,

        setting_password_obj_id_modiy_inputbox1_cont,
        setting_password_obj_id_modiy_inputbox2_cont,

        setting_password_obj_id_modiy_apply_btn,

        setting_password_obj_id_msgbox_cont,
        setting_password_obj_id_msgbox_parent,
        setting_password_obj_id_text,
        setting_password_obj_id_confirm,
        setting_password_obj_id_confirm_img,

        setting_password_obj_id_easy_msgbox_bg,
};

static bool reset_unit = false;

static void setting_password_modiy_cancel_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont);
        lv_obj_del(obj);
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_reset_cont);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
}
static void setting_password_msg_confirm_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_msgbox_parent);
        lv_obj_del(obj);
}
static lv_obj_t *setting_password_msgbox_create(const char *str)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_password_obj_id_msgbox_parent, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_password_obj_id_msgbox_parent, 282, 159, 460, 283,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, setting_password_obj_id_text, 75, 90, 313, 36,
                              NULL, LV_OPA_TRANSP, 0xffffff, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                              str, 0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_img_text_btn_create(msgbox, setting_password_obj_id_confirm, 0, 221, 460, 62,
                                      setting_password_msg_confirm_click, LV_OPA_COVER, 0x0096ff, LV_OPA_COVER, 0x0096ff,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      48, 8, 365 - 94, 32, -1,
                                      "", 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                      0, 0, 460, 62, setting_password_obj_id_confirm_img,
                                      (const char *)resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        return parent;
}

static void setting_password_modiy_confirm_enable(bool en)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont);

        lv_obj_t *apple = lv_obj_get_child_form_id(parent, setting_password_obj_id_modiy_apply_btn);
        if (en == true)
        {
                lv_obj_add_flag(apple, LV_OBJ_FLAG_CHECKABLE);
                lv_obj_set_style_bg_color(apple, lv_color_hex(0x00a8ff), LV_PART_MAIN);
        }
        else
        {
                lv_obj_clear_flag(apple, LV_OBJ_FLAG_CHECKABLE);
                lv_obj_set_style_bg_color(apple, lv_color_hex(0x47494a), LV_PART_MAIN);
        }
}

static bool setting_password_check_passowrd_easy(char *passwd)
{
        char easy_password[3][4] = {{"0000"}, {"1234"}, {"4321"}};

        if (passwd == NULL)
        {
                return true;
        }
        for (int i = 0; i < 3; i++)
        {
                if (strncmp(easy_password[i], passwd, 4) == 0)
                {
                        return true;
                }
        }
        return false;
}

static void setting_password_easy_passwd_confirm_clickk(lv_event_t *ev)
{
        setting_msgdialog_msg_del(setting_password_obj_id_easy_msgbox_bg);
}

static void setting_password_modiy_confirm_click(lv_event_t *ev)
{
        lv_obj_t *input_cont1 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont), setting_password_obj_id_modiy_inputbox1_cont);
        lv_obj_t *input_cont2 = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont), setting_password_obj_id_modiy_inputbox2_cont);
        char buffer[5];
        memset(buffer, 0, sizeof(buffer));
        char verify_buffer[5];
        memset(verify_buffer, 0, sizeof(verify_buffer));
        for (int i = 0; i < 4; i++)
        {
                strcat(buffer, lv_textarea_get_text(lv_obj_get_child_form_id(input_cont1, i)));
                strcat(verify_buffer, lv_textarea_get_text(lv_obj_get_child_form_id(input_cont2, i)));
        }
        if (strncmp(buffer, verify_buffer, 4) != 0)
        {
                setting_password_msgbox_create(lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_PASSWORD_NOT_MATCH));
        }
        else
        {
                if (setting_password_check_passowrd_easy(verify_buffer) == true)
                {
                        lv_obj_t *msgbox = setting_msgdialog_msg_bg_create(setting_password_obj_id_easy_msgbox_bg, 0, 282, 143, 460, 283);
                        setting_msgdialog_msg_create(msgbox, 1, lang_str_get(POWER_SETTING_XLS_LASETTING_PASSWORD_XLS_LANG_ID_EASY_PASSWD), 0, 60, 460, 120, false);
                        setting_msgdialog_msg_confirm_btn_create(msgbox, 2, setting_password_easy_passwd_confirm_clickk);
                }
                else
                {
                        strncpy(reset_unit ? user_data_get()->etc.password : user_data_get()->etc.comm_ent_password, buffer, 4);
                        user_data_save(true, true);
                        if (!reset_unit)
                        {
                                commax_https_server_lobbyphone_change_password(network_data_get()->local_server, "443", "1234", "5678", buffer, NULL, 0, 500);
                        }
                        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                }
        }
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 8:51:49
** 说明: 输入数字键盘
***********************************************/
static lv_obj_t *setting_password_textarea_focused_get(void)
{
        lv_obj_t *textarea = NULL;

        lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont);
        for (int i = 0; i < 2; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(cont, i == 0 ? setting_password_obj_id_modiy_inputbox1_cont : setting_password_obj_id_modiy_inputbox2_cont);
                for (int j = 0; j < 4; j++)
                {
                        textarea = lv_obj_get_child_form_id(parent, j);
                        if (lv_obj_get_state(textarea) == LV_STATE_FOCUSED)
                        {
                                return textarea;
                        }
                }
        }
        return textarea;
}

static void setting_password_text_next_foucued(void)
{
        lv_obj_t *textarea = NULL;

        lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont);
        for (int i = 0; i < 2; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(cont, i == 0 ? setting_password_obj_id_modiy_inputbox1_cont : setting_password_obj_id_modiy_inputbox2_cont);
                for (int j = 0; j < 4; j++)
                {
                        textarea = lv_obj_get_child_form_id(parent, j);
                        if (lv_obj_get_state(textarea) == LV_STATE_FOCUSED)
                        {
                                if ((i != 1) || (j != 3))
                                {
                                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                                        if ((i == 0) && (j == 3))
                                        {
                                                parent = lv_obj_get_child_form_id(cont, setting_password_obj_id_modiy_inputbox2_cont);
                                                textarea = lv_obj_get_child_form_id(parent, 0);
                                                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        }
                                        else
                                        {
                                                textarea = lv_obj_get_child_form_id(parent, j + 1);
                                                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        }
                                }
                                if ((i == 1 && j == 3))
                                {
                                        setting_password_modiy_confirm_enable(true);
                                }
                                return;
                        }
                }
        }
}
static void setting_password_text_prev_foucued(void)
{
        lv_obj_t *textarea = NULL;

        lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont);
        for (int i = 0; i < 2; i++)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(cont, i == 0 ? setting_password_obj_id_modiy_inputbox1_cont : setting_password_obj_id_modiy_inputbox2_cont);
                for (int j = 0; j < 4; j++)
                {
                        textarea = lv_obj_get_child_form_id(parent, j);
                        if (lv_obj_get_state(textarea) == LV_STATE_FOCUSED)
                        {
                                if ((i != 0) || (j != 0))
                                {
                                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                                        if ((i == 1) && (j == 0))
                                        {
                                                parent = lv_obj_get_child_form_id(cont, setting_password_obj_id_modiy_inputbox1_cont);
                                                textarea = lv_obj_get_child_form_id(parent, 3);
                                                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        }
                                        else if ((i == 1) && (j == 3))
                                        {

                                                textarea = lv_obj_get_child_form_id(parent, 3);
                                                if (lv_textarea_get_cursor_pos(textarea) != 0)
                                                {
                                                        textarea = lv_obj_get_child_form_id(parent, 3);
                                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                                }
                                                else
                                                {
                                                        textarea = lv_obj_get_child_form_id(parent, j - 1);
                                                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                                }
                                        }
                                        else
                                        {
                                                textarea = lv_obj_get_child_form_id(parent, j - 1);
                                                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                        }
                                }
                                if ((i == 1 && j == 3))
                                {
                                        setting_password_modiy_confirm_enable(false);
                                }
                                return;
                        }
                }
        }
}

static bool setting_password_textbox_del(void)
{
        setting_password_text_prev_foucued();
        lv_obj_t *textarea = setting_password_textarea_focused_get();

        if (textarea == NULL)
        {
                return false;
        }

        lv_textarea_del_char(textarea);
        return true;
}

static bool setting_password_textbox_add(const char *string)
{
        lv_obj_t *textarea = setting_password_textarea_focused_get();

        if (textarea == NULL)
        {
                return false;
        }

        lv_textarea_add_text(textarea, string);
        setting_password_text_next_foucued();
        return true;
}

static void setting_password_modiy_keyboard_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_target(ev);
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);

        if (lv_btnmatrix_has_btn_ctrl(obj, id, LV_BTNMATRIX_CTRL_HIDDEN) == true)
        {
                return;
        }

        if (id == 11)
        {
                setting_password_textbox_del();
        }
        else
        {
                const char *text = lv_btnmatrix_get_btn_text(obj, id);
                if (text != NULL)
                {

                        setting_password_textbox_add(text);
                }
        }
}

static void setting_password_modiy_obj_create(void)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_password_obj_id_modiy_cont, 0, 0, 1024, 600,
                                                    NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        {
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-3 14:13:25
                 ** 说明: 标题显示
                 ***********************************************/
                lv_common_text_create(parent, setting_password_obj_id_modiy_password_head_label, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(reset_unit ? SETTING_PASSWORD_XLS_LANG_ID_RESET_HOUSEHOLD_PASSWORD : SETTING_PASSWORD_XLS_LANG_ID_RESET_COMMON_ENTRANCE_PASSWORD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-3 14:13:25
                ** 说明: 返回按钮
                ***********************************************/
                {
                        lv_common_img_btn_create(parent, setting_password_obj_id_modiy_cancel, 35, 15, 48, 48,
                                                 setting_password_modiy_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_close.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-3 14:13:25
                 ** 说明:输入键盘
                 ***********************************************/
                {
                        lv_obj_t *obj = lv_common_number_input_keyboard_create(parent, setting_password_obj_id_modiy_input_btnatirx, 128, 90, 312, 402,
                                                                               setting_password_modiy_keyboard_click, LV_OPA_COVER, 0x808080, LV_OPA_COVER, 0x00a8ff,
                                                                               360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                               360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                               0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                                               18, 24);
                        lv_btnmatrix_set_btn_ctrl(obj, 9, LV_BTNMATRIX_CTRL_HIDDEN | LV_BTNMATRIX_CTRL_DISABLED);
                        lv_common_img_btn_create(parent, setting_password_obj_id_modiy_input_keyboard_cover, 128, 390, 100, 102,
                                                 NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                }
                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-3 14:13:25
                ** 说明:password reset
                ***********************************************/
                {
                        lv_common_text_create(parent, setting_password_obj_id_modiy_password_reset_label, 652, 80, 211, 43,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_PASSWORD_RESET), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-3 14:13:25
                 ** 说明:password new password
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_password_obj_id_modiy_password_new_label, 674, 147, 164, 35,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_NEW_PASSWORD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-3 14:13:25
                 ** 说明:password verify password
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_password_obj_id_modiy_passowrd_verify_label, 668, 322, 177, 35,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_VERIFY_PASSWORD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                }

                for (int i = 0; i < 2; i++)
                {

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-3 14:13:25
                         ** 说明:testarea
                         ***********************************************/
                        lv_obj_t *cont = lv_common_img_btn_create(parent, i == 0 ? setting_password_obj_id_modiy_inputbox1_cont : setting_password_obj_id_modiy_inputbox2_cont, 600, i == 0 ? 206 : 381, 312, 80,
                                                                  NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                  NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        for (int j = 0; j < 4; j++)
                        {

                                lv_obj_t *obj = lv_common_textarea_create(cont, j, 0 + 84 * j, 0, 60, 76,
                                                                          NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                                                          LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                                                          0, 4, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0Xffffff,
                                                                          0, 4, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                          NULL, 0Xffffff, 0Xffffff, LV_TEXT_ALIGN_CENTER, lv_font_large, 1,
                                                                          20, 500, 0Xffffff);

                                lv_textarea_set_password_mode(obj, true);
                                lv_textarea_set_password_show_time(obj, 500);
                                lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                                if (i == 0 && j == 0)
                                {
                                        lv_obj_add_state(obj, LV_STATE_FOCUSED);
                                }
                        }
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-3 14:13:25
                 ** 说明: 返回按钮
                 ***********************************************/
                {
                        lv_obj_t *obj = lv_common_text_create(parent, setting_password_obj_id_modiy_apply_btn, 0, 528, 1024, 72,
                                                              setting_password_modiy_confirm_click, LV_OPA_COVER, 0x47494a, LV_OPA_COVER, 0x00a8ff,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_APPLE), 0XFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                        lv_obj_set_style_pad_top(obj, 15, LV_PART_MAIN);
                        setting_password_modiy_confirm_enable(false);
                }
        }
}
static void setting_password_cancel_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}
static void setting_password_reset_household_obj_click(lv_event_t *ev)
{
        reset_unit = true;
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_reset_cont);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        setting_password_modiy_obj_create();
}
static void setting_password_reset_common_entrance_obj_click(lv_event_t *ev)
{
        reset_unit = false;
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_password_obj_id_reset_cont);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        setting_password_modiy_obj_create();
}

static lv_obj_t *setting_password_reset_list_create(lv_obj_t *parent)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 928, 72,
             setting_password_obj_id_reset_household_cont, setting_password_obj_id_reset_household_title, -1,
             SETTING_PASSWORD_XLS_LANG_ID_RESET_HOUSEHOLD_PASSWORD, lang_str_get,
             -1, NULL,
             setting_password_reset_household_obj_click},
            {0, 72, 928, 72,
             setting_password_obj_id_reset_common_cont, setting_password_obj_id_reset_common_title, -1,
             SETTING_PASSWORD_XLS_LANG_ID_RESET_COMMON_ENTRANCE_PASSWORD, lang_str_get,
             -1, NULL,
             setting_password_reset_common_entrance_obj_click}};

        lv_obj_t *list = setting_list_create(parent, setting_password_obj_id_list);
        lv_common_style_set_common(list, setting_password_obj_id_list, 48, 88, 928, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                // if (((user_data_get()->system_mode & 0xF0) != 0x10) && (i == 1))
                // {
                //         continue;
                // }
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
        return NULL;
}
static void sat_layout_enter(setting_password)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_password_obj_id_reset_cont, 0, 0, 1024, 600,
                                                            NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                {
                        {
                                lv_common_text_create(parent, setting_password_obj_id_title, 0, 20, 1024, 40,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                        }

                        /***********************************************
                        ** 作者: leo.liu
                        ** 日期: 2023-2-3 14:13:25
                        ** 说明: 返回按钮
                        ***********************************************/
                        {
                                lv_common_img_btn_create(parent, setting_password_obj_id_cancel, 35, 15, 48, 48,
                                                         setting_password_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                        }
                }

                setting_password_reset_list_create(parent);
        }
}
static void sat_layout_quit(setting_password)
{
}

sat_layout_create(setting_password);