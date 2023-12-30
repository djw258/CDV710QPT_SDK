#include "layout_define.h"
#include "layout_ipc_camera.h"
#include "layout_wifi_input.h"
/*
 * ipc_camera_input_flag = 0x00: 显示页面密码错误
 * ipc_camera_input_flag = 0x01:输入账号
 * ipc_camera_input_flag = 0x02:输入密码，进入预览。
 * ipc_camera_input_flag = 0x03:修改设备名成
 */
static int ipc_camera_input_flag = 0;
void layout_ipc_camera_input_flag_set(int flg)
{
        ipc_camera_input_flag = flg;
}
int layout_ipc_camera_input_flag_get(void)
{
        return ipc_camera_input_flag;
}

/*ipc_camera_input_flag = 0的情况下，
ipc_camera_password_state = 0,输入旧密码
ipc_camera_password_state = 1:输入新密码*/
static int ipc_camera_password_state = 0;

void ipc_camera_password_state_set(int flg)
{
        ipc_camera_password_state = flg;
}
enum
{
        ipc_camera_password_input_obj_id_title,
        ipc_camera_password_input_obj_id_cancel,
        ipc_camera_password_input_obj_id_textarea,
        ipc_camera_password_input_obj_id_password_hidden,
        ipc_camera_password_input_obj_id_keyboard,
        ipc_camera_password_input_obj_id_msgbox,
        ipc_camera_password_keyboard_space_hidden,
};
/*密码缓存buffer*/
static char ipc_camera_password_input_password_old[64] = {0};
static char ipc_camera_password_input_password_temp[64] = {0};

static lv_obj_t *ipc_camera_password_input_msgbox_create(const char *title, const char *msg, lv_event_cb_t confirm_cb)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_msgbox, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_90, 0, LV_OPA_90, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, 0, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, 0, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_text_create(msgbox, 1, 32, 90, 396, 200,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                              msg, 0xFFFFFFFF, 0xFFFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_img_btn_create(msgbox, 2, 0, 281, 460, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}
static void ipc_camera_password_input_cancel_click(lv_event_t *e)
{
        int flag = layout_ipc_camera_input_flag_get();

        if ((flag == IPC_CAMERA_FLAG_CHANGE_NAME) || (flag == IPC_CAMERA_FLAG_CHANGE_PWD))
        {
                if ((layout_ipc_cmeara_is_doorcamera_get() == true) && (sat_pre_layout_get() == sat_playout_get(ipc_camera_search)))
                {
                        sat_layout_goto(ipc_camera_search, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                }
                else
                {
                        sat_layout_goto(ipc_camera_edit, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
                }
        }
        else if (flag & IPC_CAMERA_FLAG_SEARCH)
        {
                sat_layout_goto(ipc_camera_search, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else if (flag & IPC_CAMERA_FLAG_REGISTER)
        {
                sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
}

static void ipc_camera_password_input_textarea_placeholder_setting(void)
{
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
        if (textarea == NULL)
        {
                return;
        }
        if (ipc_camera_input_flag & IPC_CAMERA_FLAG_INPUT_PWD)
        {
                lv_textarea_set_placeholder_text(textarea, lang_str_get(WIFI_INPUT_XLS_LANG_ID_INPUT_PASSWORD));
        }
        else if (ipc_camera_input_flag & IPC_CAMERA_FLAG_INPUT_USER)
        {
                lv_textarea_set_placeholder_text(textarea, lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_ENTER_CAMEARA_ID));
        }
        else if (ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_PWD)
        {
                lv_textarea_set_placeholder_text(textarea, ipc_camera_password_state == 0 ? lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_OLD_PASSWORD)
                                                                                          : lang_str_get(ipc_camera_password_input_password_old[0] == '\0' ? DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_PASSWORD
                                                                                                         : ipc_camera_password_input_password_temp[0] == 0 ? DOOR_CAMERA_SEARCH_XLS_LANG_ID_ENTER_9_DIGITS_OF_NUMBER
                                                                                                                                                           : DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_TH_CONNETION_PASSWORD));
        }
        else if (ipc_camera_input_flag & (IPC_CAMERA_FLAG_INPUT_USER | IPC_CAMERA_FLAG_INPUT_PWD))
        {
                lv_textarea_set_placeholder_text(textarea, layout_ipc_cmeara_is_doorcamera_get() == false ? network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].door_name : network_data_get()->door_device[layout_ipc_camera_edit_index_get()].door_name);
        }
}

static void ipc_camera_password_input_hidden_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);

        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);

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

static bool ipc_camera_password_mixture_char_check(const char *string)
{
        bool exist_number = false;
        bool exist_letters = false;
        bool exist_special_characters = false;
        int len = strlen(string);
        for (int i = 0; i < len; i++)
        {
                if (((string[i] >= 'a') && (string[i] <= 'z')) || ((string[i] >= 'A') && (string[i] <= 'Z')))
                {
                        exist_letters = true;
                }
                else if ((string[i] <= '9') && (string[i] >= '0'))
                {
                        exist_number = true;
                }
                else
                {
                        exist_special_characters = true;
                }

                if (exist_letters && exist_number && exist_special_characters)
                {
                        return true;
                }
        }

        return false;
}

static void ipc_camera_password_input_msgbox_confirm_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_msgbox);
        if (obj == NULL)
        {
                return;
        }

        lv_obj_del(obj);
}
static void ipc_camera_password_success_msgbox_confirm_click(lv_event_t *e)
{
        if (sat_pre_layout_get() == sat_playout_get(ipc_camera_search)) //(layout_ipc_cmeara_is_doorcamera_get() == true)
        {
                sat_layout_goto(ipc_camera_search, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                sat_layout_goto(ipc_camera_edit, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
}

/*判断是否是有效的键值*/
static bool ipc_camera_determine_whether_the_key_value_is_valid(lv_keyboard_mode_t mode, uint16_t btn_id)
{
        if ((((mode == LV_KEYBOARD_MODE_TEXT_LOWER) || (mode == LV_KEYBOARD_MODE_TEXT_UPPER)) && (btn_id != 41)) ||
            ((mode == LV_KEYBOARD_MODE_SPECIAL) && (btn_id != 43)))
        {
                return false;
        }

        return true;
}
/*判断密码长度是否符合要求*/
static bool ipc_camera_determine_password_length_is_valid(const char *txt)
{

        if (1 /*layout_ipc_cmeara_is_doorcamera_get() == true*/)
        {
                // 输入新密码要做非法校验，注册时输入密码或输入旧密码无需校验
                if ((ipc_camera_input_flag & (/*IPC_CAMERA_FLAG_INPUT_PWD | */ IPC_CAMERA_FLAG_CHANGE_PWD)) && (strlen(txt) < 9) && (ipc_camera_password_state == 0x01))
                {
                        return false;
                }
                return true;
        }

        if ((ipc_camera_input_flag & (/*IPC_CAMERA_FLAG_INPUT_PWD |*/ IPC_CAMERA_FLAG_CHANGE_PWD)) && (strlen(txt) < 9) && (ipc_camera_password_state == 0x01)) // 只对新密码做非法校验
        {
                return false;
        }
        return true;
}
static bool ipc_camera_input_new_password_processing(const char *txt)
{
        if (ipc_camera_password_mixture_char_check(txt) == false)
        {
                ipc_camera_password_input_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD),
                                                        lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_SET_9_DIGITS_TO_A_MIXTURE),
                                                        ipc_camera_password_input_msgbox_confirm_click);
                return true;
        }

        if (ipc_camera_password_input_password_temp[0] == 0)
        {
                lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
                strcpy(ipc_camera_password_input_password_temp, lv_textarea_get_text(textarea));
                lv_textarea_set_text(textarea, "");
                ipc_camera_password_input_textarea_placeholder_setting();

                return true;
        }

        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
        if (strcmp(ipc_camera_password_input_password_temp, lv_textarea_get_text(textarea)) == 0)
        {
                // struct ipcamera_info *node = sat_ipcamera_node_data_get(layout_ipc_camera_edit_index_get());
                // printf("node passwd is %s\n", node->password);
                // printf("node ipaddr is %s\n", node->ipaddr);
                // printf("node username is %s\n", node->username);
                // printf("node port is %d\n", node->port);
                // printf("node auther_flag is %d\n", node->auther_flag);
                if (sat_ipcamera_device_password_set(ipc_camera_password_input_password_temp, layout_ipc_camera_edit_index_get(), 1200) == true)
                {

                        ipc_camera_password_input_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD),
                                                                lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_PASSWORD_MODIY_SUCCESS),
                                                                ipc_camera_password_success_msgbox_confirm_click);

                        if (layout_ipc_cmeara_is_doorcamera_get() == true)
                        {
                                memset(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].password, 0, sizeof(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].password));
                                strcpy(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].password, lv_textarea_get_text(textarea));
                        }
                        else
                        {
                                memset(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].password, 0, sizeof(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].password));
                                strcpy(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].password, lv_textarea_get_text(textarea));
                        }
                        network_data_save();
                        return true;
                }
                else
                {
                        ipc_camera_password_input_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD),
                                                                lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_PASSWD_MODIFY_FAIL),
                                                                ipc_camera_password_input_msgbox_confirm_click);
                }
        }
        else
        {
                ipc_camera_password_input_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD),
                                                        lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_PASSWORD_NOT_MATCH),
                                                        ipc_camera_password_input_msgbox_confirm_click);
        }

        return true;
}
static bool ipc_camera_input_old_password_processing(const char *txt)
{
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
        const char *input_password = lv_textarea_get_text(textarea);
        if (strcmp(sat_ipcamera_password_get(layout_ipc_camera_edit_index_get()), input_password) == 0)
        {
                ipc_camera_password_state = 0x01;
                memset(ipc_camera_password_input_password_old, 0, sizeof(ipc_camera_password_input_password_old));
                strcpy(ipc_camera_password_input_password_old, input_password);

                memset(ipc_camera_password_input_password_temp, 0, sizeof(ipc_camera_password_input_password_temp));
                lv_textarea_set_text(textarea, "");
                ipc_camera_password_input_textarea_placeholder_setting();

                return true;
        }

        ipc_camera_password_input_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD),
                                                lang_str_get(SETTING_PASSWORD_XLS_LANG_ID_PASSWORD_NOT_MATCH),
                                                ipc_camera_password_input_msgbox_confirm_click);

        return true;
}
static bool ipc_camera_input_new_name_processing(void)
{
        char input_name[64] = {0};
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
        strncpy(input_name, lv_textarea_get_text(textarea), sizeof(input_name));
        if (sat_ipcamera_device_name_set(input_name, layout_ipc_camera_edit_index_get(), 1500) == true)
        {
                if (layout_ipc_cmeara_is_doorcamera_get() == true)
                {
                        char doorname[128] = {0};
                        sprintf(doorname, "Door%d(%s)", layout_ipc_camera_edit_index_get() + 1, lv_textarea_get_text(textarea));
                        memset(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].door_name, 0, sizeof(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].door_name));
                        strcpy(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].door_name, doorname);
                }
                else
                {
                        char doorname[128] = {0};
                        sprintf(doorname, "CCTV%d(%s)", layout_ipc_camera_edit_index_get() + 1, input_name);
                        memset(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].door_name, 0, sizeof(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].door_name));
                        strcpy(network_data_get()->cctv_device[layout_ipc_camera_edit_index_get()].door_name, doorname);
                }

                network_data_save();

                ipc_camera_password_input_msgbox_create(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_PASSWORD),
                                                        lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_NAME_MODIY_SUCCESS),
                                                        ipc_camera_password_success_msgbox_confirm_click);

                return true;
        }
        ipc_camera_password_input_msgbox_create(network_data_get()->door_device[layout_ipc_camera_edit_index_get()].door_name,
                                                lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_NAME_MODIY_FAIL),
                                                ipc_camera_password_input_msgbox_confirm_click);
        return true;
}

static void ipc_camera_input_hidden_btn_display(void)
{
        lv_obj_t *hide_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_password_hidden);
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
        bool password_mode = lv_textarea_get_password_mode(textarea);
        if (password_mode == true)
        {
                lv_obj_set_style_bg_img_src(hide_btn, resource_ui_src_get("btn_list_hide.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(hide_btn, resource_ui_src_get("5_list_show.png"), LV_PART_MAIN);
        }
        if ((ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_NAME) || (ipc_camera_input_flag & IPC_CAMERA_FLAG_INPUT_USER))
        {
                lv_obj_add_flag(hide_btn, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_clear_flag(hide_btn, LV_OBJ_FLAG_HIDDEN);
        }
}
/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期：2023-11-20 10:50:16
** 参数说明:
** 注意事项：
************************************************************/
static void layout_ipc_camera_input_space_btn_ctrl(void)
{
        int flag = layout_ipc_camera_input_flag_get();

        if ((flag == IPC_CAMERA_FLAG_CHANGE_NAME) || (flag == IPC_CAMERA_FLAG_CHANGE_PWD))
        {
                // lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_keyboard);
                // lv_keyboard_mode_t mode = lv_keyboard_get_mode(obj);
                // if ((mode == LV_KEYBOARD_MODE_TEXT_LOWER) || (mode == LV_KEYBOARD_MODE_TEXT_UPPER))
                // {
                //         lv_btnmatrix_set_btn_ctrl(obj, 40, LV_BTNMATRIX_CTRL_DISABLED);
                //         lv_btnmatrix_clear_btn_ctrl(obj, 42, LV_BTNMATRIX_CTRL_DISABLED);
                // }
                // else if (mode == LV_KEYBOARD_MODE_SPECIAL)
                // {
                //         lv_btnmatrix_set_btn_ctrl(obj, 42, LV_BTNMATRIX_CTRL_DISABLED);
                //         lv_btnmatrix_clear_btn_ctrl(obj, 40, LV_BTNMATRIX_CTRL_DISABLED);
                // }
                lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_password_keyboard_space_hidden, 220, 525, 585, 69,
                                         NULL, true, LV_OPA_COVER, 0x292929, LV_OPA_COVER, 0x292929,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("keyboard_space.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
}

static void ipc_camera_password_input_keyboard_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_target(ev);
        lv_keyboard_mode_t mode = lv_keyboard_get_mode(obj);
        int btn_id = lv_btnmatrix_get_selected_btn(obj);
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);
        const char *txt = lv_textarea_get_text(textarea);
        /*判断是否是有效的键值*/
        if (ipc_camera_determine_whether_the_key_value_is_valid(mode, btn_id) == false)
        {
                return;
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明:没有9个字符，修改失败
         ***********************************************/
        /*判断密码长度是否符合要求*/
        if (ipc_camera_determine_password_length_is_valid(txt) == false)
        {
                return;
        }
        /*搜索,输入密码进入*/
        if ((ipc_camera_input_flag & (IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_PWD)) == (IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_PWD))
        {
                lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea);

                if (layout_ipc_cmeara_is_doorcamera_get() == true)
                {
                        strcpy(ipc_camera_password_input_password_old, "admin");
                }

                sat_ipcamera_user_password_set(layout_ipc_camera_edit_index_get(), ipc_camera_password_input_password_old, lv_textarea_get_text(textarea));
                layout_ipc_camera_input_flag_set(1);
                sat_layout_goto(ipc_camera_display, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }

        if ((ipc_camera_input_flag & (IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_USER)) == (IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_USER))
        {
                memset(ipc_camera_password_input_password_old, 0, sizeof(ipc_camera_password_input_password_old));
                strcpy(ipc_camera_password_input_password_old, lv_textarea_get_text(textarea));
                layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_PWD);
                ipc_camera_password_input_textarea_placeholder_setting();
                lv_textarea_set_text(textarea, "");
                lv_textarea_set_password_mode(textarea, true);
                ipc_camera_input_hidden_btn_display();
                return SAT_VOID;
        }

        if (ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_PWD)
        {
                lv_textarea_set_max_length(textarea, 9);
                if (ipc_camera_password_state == 0x01)
                {
                        ipc_camera_input_new_password_processing(txt);
                        return SAT_VOID;
                }

                ipc_camera_input_old_password_processing(txt);
                return SAT_VOID;
        }

        if ((ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_NAME) && (strlen(txt) >= 1))
        {

                ipc_camera_input_new_name_processing();

                return SAT_VOID;
        }
        return SAT_VOID;
}

static void sat_layout_enter(ipc_camera_input)
{
        lv_obj_t *textarea;
        memset(ipc_camera_password_input_password_temp, 0, sizeof(ipc_camera_password_input_password_temp));
        memset(ipc_camera_password_input_password_old, 0, sizeof(ipc_camera_password_input_password_old));

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      (ipc_camera_input_flag & (IPC_CAMERA_FLAG_INPUT_PWD | IPC_CAMERA_FLAG_INPUT_USER) ? sat_ipcamera_door_name_get(layout_ipc_camera_edit_index_get())
                                       : (ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_PWD)                           ? lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_PASSWORD)
                                       : (ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_NAME)                          ? lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_NAME)
                                                                                                                        : lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_PASSWORD)),
                                      0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_cancel, 35, 15, 48, 48,
                                         ipc_camera_password_input_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
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
                textarea = lv_common_textarea_create(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_textarea, 48, 80, 928, 88,
                                                     NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                                     LV_OPA_COVER, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                                     0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                     0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                     NULL, 0Xffffff, 0Xffffff, LV_TEXT_ALIGN_LEFT, lv_font_normal, ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_NAME ? 16 : ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_PWD ? 9
                                                                                                                                                                                                                                 : 32,
                                                     30, 500, 0Xffffff);

                lv_obj_set_style_text_color(textarea, lv_color_hex(0x929292), LV_PART_TEXTAREA_PLACEHOLDER);
                lv_obj_set_style_text_opa(textarea, LV_OPA_COVER, LV_PART_TEXTAREA_PLACEHOLDER);
                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                lv_textarea_set_password_bullet(textarea, "*");
                ipc_camera_password_input_textarea_placeholder_setting();
                if ((ipc_camera_input_flag & IPC_CAMERA_FLAG_CHANGE_PWD) || (ipc_camera_input_flag & IPC_CAMERA_FLAG_INPUT_PWD))
                {
                        lv_textarea_set_password_mode(textarea, true);
                }
                else
                {
                        lv_textarea_set_password_mode(textarea, false);
                }

                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:43:29
                ** 说明: 隐藏按钮
                ***********************************************/
                {
                        lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_password_hidden, 928, 100, 48, 48,
                                                 ipc_camera_password_input_hidden_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_list_show.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                        ipc_camera_input_hidden_btn_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:43:29
                 ** 说明: 键盘
                 ***********************************************/
                {

                        lv_obj_t *keyboard = lv_common_keyboard_create(sat_cur_layout_screen_get(), ipc_camera_password_input_obj_id_keyboard, 0, 200, 1024, 400,
                                                                       ipc_camera_password_input_keyboard_click, LV_OPA_COVER, 0x292929, LV_OPA_COVER, 0x00a8ff,
                                                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                       8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                       8, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                       0xFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                                       24, 18, 18, 8, 12, 12,
                                                                       LV_OPA_COVER, 0x353535, LV_OPA_COVER, 0x353535);
                        lv_keyboard_set_textarea(keyboard, textarea);
                        layout_ipc_camera_input_space_btn_ctrl();
                }
        }
}
static void sat_layout_quit(ipc_camera_input)
{
        ipc_camera_password_state = 0;
}

sat_layout_create(ipc_camera_input);