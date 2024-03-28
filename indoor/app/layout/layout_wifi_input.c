#include "layout_define.h"
#include "layout_wifi_input.h"
enum
{
        wifi_input_obj_id_title,
        wifi_input_obj_id_cancel,

        wifi_input_obj_id_textarea,

        wifi_input_obj_id_password_hidden,

        wifi_input_obj_id_keyboard,

        wifi_input_obj_id_msgdialog_bg,
};

enum
{
        wifi_input_obj_id_msgdialog,
        wifi_input_obj_id_connect_status,
        wifi_input_obj_id_confirm_btn,
        wifi_input_obj_id_confirm_btn_label,
};
/***
** 日期: 2022-04-26 09:16
** 作者: leo.liu
** 函数作用：设置wifi输入账号
** 返回参数说明：
***/
static char wifi_input_user_temp[128] = {0};
static char wifi_input_user_passwd[128] = {0};

static int wifi_input_mode = 0x00;
/************************************************************
** 函数说明: wifi输入模式设置(有一些是免费wifi，不需要输入密码，直接连接)
** 作者: xiaoxiao
** 日期：2023-10-24 21:34:15
** 参数说明:0x01:账号输入;0x02:密码输入;0x03:直接连接
** 注意事项：
************************************************************/
void wifi_input_mode_set(int mode)
{
        wifi_input_mode = mode;
}

void wifi_input_user_setting(const char *user)
{
        memset(wifi_input_user_temp, 0, sizeof(wifi_input_user_temp));
        if (user != NULL)
        {
                strncpy(wifi_input_user_temp, user, strlen(user) - 1); // 把换行符去掉
        }
}

static void wifi_input_cancel_click(lv_event_t *ev)
{
        if (user_data_get()->is_device_init == false)
        {
                sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, SAT_VOID);
        }
}

static void wifi_input_password_hidden_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);

        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_textarea);

        bool password_mode = lv_textarea_get_password_mode(textarea);
        lv_textarea_set_password_mode(textarea, password_mode ? false : true);
        if (password_mode == true)
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("5_list_show.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_list_hide.png"), LV_PART_MAIN);
        }
}
static void wifi_input_textarea_placeholder_setting(void)
{
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_textarea);
        if (textarea == NULL)
        {

                return;
        }
        lv_textarea_set_placeholder_text(textarea, lang_str_get(wifi_input_user_temp[0] == 0 ? WIFI_INPUT_XLS_LANG_ID_INPUT_USER : WIFI_INPUT_XLS_LANG_ID_INPUT_PASSWORD));
}

static void wifi_input_password_hidden_icon(void)
{
        lv_obj_t *icon = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_password_hidden);
        if (icon == NULL)
        {

                return;
        }
        if (wifi_input_user_temp[0] == 0)
        {
                lv_obj_add_flag(icon, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_clear_flag(icon, LV_OBJ_FLAG_HIDDEN);
        }
}

static void wifi_input_msg_dialog_error_confirm_up(lv_event_t *ev)
{
        lv_obj_del(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_msgdialog_bg));
}

static void wifi_input_animation_connecting_task(lv_timer_t *task)
{
        bool conneted = 0;
        wifi_device_connection_stauts(NULL, NULL, NULL, &conneted, NULL);
        if (conneted == true)
        {
                wifi_device_tmp_sync();
                if (user_data_get()->is_device_init == false)
                {
                        sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                }
                else
                {
                        sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, SAT_VOID);
                }
        }
        else
        {
                int *ploop = (int *)task->user_data;
                int loop = *ploop;
                if (loop > 20)
                {
                        wifi_device_conneting();

                        lv_obj_t *parent = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_msgdialog_bg), wifi_input_obj_id_msgdialog);
                        lv_obj_clean(parent);
                        lv_timer_del(task);

                        setting_msgdialog_msg_create(parent, wifi_input_obj_id_connect_status, lang_str_get(WIFI_INPUT_XLS_LANG_ID_CONNECT_FAILED), 0, 110, 460, 80, false);
                        setting_msgdialog_msg_confirm_btn_create(parent, wifi_input_obj_id_confirm_btn, wifi_input_msg_dialog_error_confirm_up);
                        if (user_data_get()->is_device_init == true) // 启动设置会有机会进入这里，所以要加判断
                        {
                                standby_timer_restart(true, true);
                        }
                }
                else
                {
                        (*ploop)++;
                }
        }
}

/***
**   日期:2022-06-07 08:19:56
**   作者: leo.liu
**   函数作用：创建正在连接的动画
**   参数说明:
***/
static void wifi_input_animation_task_create(lv_obj_t *parent)
{
        // 旋转器创建
        {
                static lv_style_t style; // 创建样式
                static lv_style_t bc_style;

                lv_style_init(&style);                                           // 初始化样式
                lv_style_set_arc_color(&style, lv_color_make(0x00, 0x96, 0xFF)); // 设置圆弧颜色
                lv_style_set_arc_width(&style, 12);                              // 设置圆弧宽度；

                lv_style_init(&bc_style);                                           // 初始化样式
                lv_style_set_arc_color(&bc_style, lv_color_make(0xFF, 0xFF, 0xFF)); // 设置背景圆环颜色
                lv_style_set_arc_width(&bc_style, 12);                              // 设置背景圆环宽度

                lv_obj_t *preload = lv_spinner_create(parent, 1000, 45);

                lv_obj_add_style(preload, &style, LV_PART_INDICATOR); // 应用到圆弧部分；
                lv_obj_add_style(preload, &bc_style, LV_PART_MAIN);   // 应用到背景圆环部分；

                // lv_obj_t * preload = lv_spinner_create(lv_scr_act(),1000,100);
                lv_obj_set_size(preload, 100, 100);
                lv_obj_align(preload, LV_ALIGN_CENTER, 0, 70);
        }
        // 连接状态显示
        {
                setting_msgdialog_msg_create(parent, wifi_input_obj_id_connect_status, lang_str_get(WIFI_INPUT_XLS_LANG_ID_CONNECTING), 0, 110, 460, 80, false);
        }

        static int loop_count = 0; // wifi最大连接时长标志
        loop_count = 0;
        lv_sat_timer_create(wifi_input_animation_connecting_task, 1000, &loop_count);

        wifi_device_user_to_temp((unsigned char *)wifi_input_user_temp, wifi_input_user_passwd[0] == '\0' ? NULL : (unsigned char *)wifi_input_user_passwd);
        wifi_device_temp_conneting();
}

/***
**   日期:2022-06-07 08:14:52
**   作者: leo.liu
**   函数作用：创建连接的消息框
**   参数说明:
***/
static void wifi_input_msg_dialog_display(void)
{

        lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_msgdialog_bg);
        if (cont != NULL)
        {
                return;
        }
        lv_obj_t *msgbox = setting_msgdialog_msg_bg_create(wifi_input_obj_id_msgdialog_bg, wifi_input_obj_id_msgdialog, 282, 93, 460, 352);
        wifi_input_animation_task_create(msgbox);
}

static void layout_wifi_input_head_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), 0);
        if (obj != NULL)
        {
                lv_label_set_text_fmt(obj, "%s", wifi_input_user_temp[0] == 0 ? lang_str_get(WIFI_SETTING_XLS_LANG_ID_WIFI) : wifi_input_user_temp);
        }
}
static void wifi_input_keyboard_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_keyboard);
        uint16_t btn_id = lv_btnmatrix_get_selected_btn(obj);
        lv_keyboard_mode_t mode = lv_keyboard_get_mode(obj);
        if ((((mode == LV_KEYBOARD_MODE_TEXT_LOWER) || (mode == LV_KEYBOARD_MODE_TEXT_UPPER)) && (btn_id != 41)) ||
            ((mode == LV_KEYBOARD_MODE_SPECIAL) && (btn_id != 43)))
        {
                return;
        }
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_textarea);
        if (wifi_input_user_temp[0] == 0)
        {
                lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_textarea);
                strcpy(wifi_input_user_temp, lv_textarea_get_text(textarea));
                lv_textarea_set_text(textarea, "");
                wifi_input_textarea_placeholder_setting();
                wifi_input_password_hidden_icon();
                layout_wifi_input_head_display();
                lv_textarea_set_password_mode(textarea, true);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_input_obj_id_password_hidden);
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_list_hide.png"), LV_PART_MAIN);
        }
        else
        {
                int pos = lv_textarea_get_cursor_pos(textarea);
                if ((pos < 8) && (pos != 0))
                {
                        return;
                }
                memset(wifi_input_user_passwd, 0, sizeof(wifi_input_user_passwd));
                if (pos != 0)
                {
                        strcpy((char *)wifi_input_user_passwd, lv_textarea_get_text(textarea));
                }
                if (1)
                {
                        standby_timer_close();
                        wifi_input_msg_dialog_display();
                }
        }
}

static void sat_layout_enter(wifi_input)
{
        memset(wifi_input_user_passwd, 0, sizeof(wifi_input_user_passwd));
        lv_obj_t *textarea;
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), wifi_input_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      wifi_input_user_temp[0] == 0 ? lang_str_get(WIFI_INPUT_XLS_LANG_ID_ADD_WIFI) : wifi_input_user_temp, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), wifi_input_obj_id_cancel, 35, 15, 48, 48,
                                         wifi_input_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:29
        ** 说明: 文本输入框显示
        ***********************************************/
        {
                textarea = lv_common_textarea_create(sat_cur_layout_screen_get(), wifi_input_obj_id_textarea, 48, 80, 928, 88,
                                                     NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                                     LV_OPA_COVER, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                     0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                     0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                     NULL, 0Xffffff, 0Xffffff, LV_TEXT_ALIGN_LEFT, lv_font_normal, 64,
                                                     30, 500, 0Xffffff);

                lv_obj_set_style_text_color(textarea, lv_color_hex(0x929292), LV_PART_TEXTAREA_PLACEHOLDER);
                lv_obj_set_style_text_opa(textarea, LV_OPA_COVER, LV_PART_TEXTAREA_PLACEHOLDER);
                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                lv_textarea_set_password_bullet(textarea, "*");
                wifi_input_textarea_placeholder_setting();
                printf("wifi_input_user_temp[0] is %c\n", wifi_input_user_temp[0]);
                lv_textarea_set_password_mode(textarea, wifi_input_user_temp[0] == 0 ? false : true);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:29
        ** 说明: 隐藏按钮
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), wifi_input_obj_id_password_hidden, 928, 100, 48, 48,
                                         wifi_input_password_hidden_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_list_hide.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                wifi_input_password_hidden_icon();
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 键盘
         ***********************************************/
        {

                lv_obj_t *keyboard = lv_common_keyboard_create(sat_cur_layout_screen_get(), wifi_input_obj_id_keyboard, 0, 200, 1024, 400,
                                                               wifi_input_keyboard_click, LV_OPA_COVER, 0x292929, LV_OPA_COVER, 0x00a8ff,
                                                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                               8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                               8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                               0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                               24, 18, 18, 8, 12, 12,
                                                               LV_OPA_COVER, 0x353535, LV_OPA_COVER, 0x353535);

                lv_keyboard_set_textarea(keyboard, textarea);
        }

        if (wifi_input_mode == 0x03)
        {
                standby_timer_close();
                wifi_input_msg_dialog_display();
        }
}
static void sat_layout_quit(wifi_input)
{
        if (user_data_get()->is_device_init == true) // 启动设置会有机会进入这里，所以要加判断
        {
                standby_timer_restart(true, true);
        }
}
sat_layout_create(wifi_input);