#include "layout_define.h"
#include "layout_ipc_camera.h"
#include "layout_ipc_camera.h"
#include "layout_setting_general.h"

static int ipc_camera_edit_index = 0;
void layout_ipc_camera_edit_index_set(int index)
{
        ipc_camera_edit_index = index;
}
int layout_ipc_camera_edit_index_get(void)
{
        return ipc_camera_edit_index;
}

enum
{
        ipc_camera_edit_obj_id_title,
        ipc_camera_edit_obj_id_cancel,
        ipc_camera_edit_obj_id_list,
        ipc_camera_edit_obj_id_sensor_linkage_msg_bg,
};

typedef enum
{
        ipc_camera_edit_obj_id_sensor_linkage_msg,
} ipc_camera_edit_sensor_linkage_msg_bg_obj_id;

/*返回按钮点击事件*/
static void
ipc_camera_edit_cancel_obj_click(lv_event_t *e)
{
        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void ipc_camera_edit_channge_name_obj_click(lv_event_t *e)
{
        layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_CHANGE_NAME);
        sat_layout_goto(ipc_camera_input, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void ipc_camera_edit_channge_password_obj_click(lv_event_t *e)
{
        layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_CHANGE_PWD);
        sat_layout_goto(ipc_camera_input, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void ipc_camera_edit_msgbox_item_click(lv_event_t *ev)
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
                for (int i = 0; i < 8; i++)
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

static int ipc_camera_edit_sensor_linkage_select_index_get()
{
        lv_obj_t *msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_edit_obj_id_sensor_linkage_msg_bg);
        if (msgbox == NULL)
        {
                SAT_DEBUG("lv_obj_t* msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_msgbox_cont);");
                return -1;
        }
        lv_obj_t *parent = lv_obj_get_child_form_id(msgbox, ipc_camera_edit_obj_id_sensor_linkage_msg);
        if (parent == NULL)
        {
                SAT_DEBUG("lv_obj_t* parent = lv_obj_get_child_form_id(msgbox, setting_motion_obj_id_msgbox_cont);");
                return -1;
        }
        lv_obj_t *list = lv_obj_get_child_form_id(parent, 3);
        if (list == NULL)
        {
                SAT_DEBUG("lv_obj_t* parent = lv_obj_get_child_form_id(msgbox, setting_motion_obj_id_msgbox_cont);");
                return -1;
        }
        for (int i = 0; i < 8; i++)
        {
                lv_obj_t *checkbox = lv_obj_get_child_form_id(list, i);
                if (checkbox == NULL)
                {
                        continue;
                }
                lv_obj_t *img = lv_obj_get_child_form_id(checkbox, 1);
                if (img == NULL)
                {
                        SAT_DEBUG("  lv_obj_t* img =  lv_obj_get_child_form_id(checkbox, img_obj_id_group[i][1]);");
                        return -1;
                }
                if (strncmp((const char *)img->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
                {
                        return i;
                }
        }
        return -1;
}
/************************************************************
** 函数说明: 传感器链接状态显示
** 作者: xiaoxiao
** 日期：2023-09-27 08:18:10
** 参数说明:
** 注意事项：
************************************************************/
static void layout_ipc_edit_sensor_linkage_display(void)
{
        lv_obj_t *link_cnt = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_edit_obj_id_list), 2);
        if (link_cnt)
        {
                lv_obj_t *sub = lv_obj_get_child_form_id(link_cnt, 1);
                if (user_data_get()->alarm.cctv_sensor[layout_ipc_camera_edit_index_get()] == 0)
                {
                        lv_label_set_text(sub, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED));
                }
                else
                {
                        lv_label_set_text(sub, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_1 + user_data_get()->alarm.cctv_sensor[layout_ipc_camera_edit_index_get()] - 1));
                }
        }
}

static void ipc_camera_edit_sensor_linkage_click_confirm(lv_event_t *e)
{
        user_data_get()->alarm.cctv_sensor[ipc_camera_edit_index] = ipc_camera_edit_sensor_linkage_select_index_get();
        user_data_save(true, true);
        setting_msgdialog_msg_del(ipc_camera_edit_obj_id_sensor_linkage_msg_bg);
        layout_ipc_edit_sensor_linkage_display();
}

static void ipc_camera_edit_sensor_linkage_click_cancel(lv_event_t *e)
{
        setting_msgdialog_msg_del(ipc_camera_edit_obj_id_sensor_linkage_msg_bg);
}
static void ipc_camera_edit_sensor_linkage_click(lv_event_t *e)
{
        lv_obj_t *msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_edit_obj_id_sensor_linkage_msg_bg);
        if (msgbox != NULL)
        {
                setting_msgdialog_msg_del(ipc_camera_edit_obj_id_sensor_linkage_msg_bg);
        }
        msgbox = setting_msgdialog_msg_bg_create(ipc_camera_edit_obj_id_sensor_linkage_msg_bg, ipc_camera_edit_obj_id_sensor_linkage_msg, 282, 143, 460, 360);
        setting_msgdialog_msg_create(msgbox, 0, lang_str_get(SENSOR_SETTING_XLS_LANG_ID_SETTING_SENSOR_CONNECT), 0, 20, 460, 120, true);
        setting_msgdialog_msg_confirm_and_cancel_btn_create(msgbox, 1, 2, ipc_camera_edit_sensor_linkage_click_confirm, ipc_camera_edit_sensor_linkage_click_cancel);
        lv_obj_t *list = setting_list_create(msgbox, 3);
        lv_common_style_set_common(list, 3, 27, 73, 460 - 27, 343 - 57 - 73, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        int j = 0;
        for (int i = 0; i < 8; i++)
        {
                if (i == 0)
                {
                        lv_common_img_text_btn_create(list, i, 48, 61 + 56 * j, 365, 48,
                                                      ipc_camera_edit_msgbox_item_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      48, 8, 365 - 94, 32, 0,
                                                      lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                      0, 8, 32, 32, 1,
                                                      (const char *)resource_ui_src_get(user_data_get()->alarm.cctv_sensor[ipc_camera_edit_index] == i ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        continue;
                }
                if (user_data_get()->alarm.alarm_enable[i - 1] && user_data_get()->alarm.alarm_enable_always[i - 1] == 0)
                {

                        lv_obj_t *obj = lv_common_img_text_btn_create(list, i, 48, 61 + 56 * j, 365, 48,
                                                                      ipc_camera_edit_msgbox_item_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                      48, 8, 365 - 94, 32, 0,
                                                                      lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_1 + i - 1), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                      0, 8, 32, 32, 1,
                                                                      (const char *)resource_ui_src_get(user_data_get()->alarm.cctv_sensor[ipc_camera_edit_index] == i ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        for (int h = 0; h < 7; h++)
                        {
                                if ((user_data_get()->alarm.cctv_sensor[h] == i) && (h != ipc_camera_edit_index)) // 传感器被选择了，且不是被当前CCTV选择的
                                {

                                        lv_common_img_btn_create(obj, 2, 0, 0, 365, 48,
                                                                 NULL, true, LV_OPA_60, 0x242526, LV_OPA_60, 0x242526,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                                        break;
                                }
                        }

                        j++;
                }
        }
}

static void sat_layout_enter(ipc_camera_edit)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 标题显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_edit_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_DOOR_CAMERA_INFOMATION), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步下一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_edit_obj_id_cancel, 35, 15, 48, 48,
                                         ipc_camera_edit_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 列表创建
         ***********************************************/
        {
                setting_list_info_t main_list_group[] = {

                    {0, 0, 928, 88,
                     0, 0, -1,
                     DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_NAME, lang_str_get,
                     -1, NULL,
                     ipc_camera_edit_channge_name_obj_click, -1},

                    {0, 88, 928, 88,
                     1, 0, -1,
                     DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_TH_CONNETION_PASSWORD, lang_str_get,
                     -1, NULL,
                     ipc_camera_edit_channge_password_obj_click, -1},

                    {0, 88 * 2, 928, 88,
                     2, 0, 1,
                     SENSOR_SETTING_XLS_LANG_ID_SETTING_SENSOR_CONNECT, lang_str_get,
                     -1, NULL,
                     ipc_camera_edit_sensor_linkage_click, -1}};

                lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), ipc_camera_edit_obj_id_list);
                lv_common_style_set_common(list, ipc_camera_edit_obj_id_list, 48, 88, 928, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                int j = 0;
                for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
                {
                        if ((layout_ipc_cmeara_is_doorcamera_get() && i == 2) || (layout_ipc_cmeara_is_doorcamera_get() == false && i == 1))
                        {
                                continue;
                        }

                        lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[i].w, main_list_group[i].h,
                                                                        main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                        0, 30, 576, 43, main_list_group[i].title_id,
                                                                        main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                        0, 52, 576, 29, main_list_group[i].sub_id,
                                                                        main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                        0, 0, 0, 0, -1,
                                                                        NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                        0, 28, 32, 32, main_list_group[i].img_id,
                                                                        NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                        j++;
                }
                layout_ipc_edit_sensor_linkage_display();
        }
}
static void sat_layout_quit(ipc_camera_edit)
{
}

sat_layout_create(ipc_camera_edit);