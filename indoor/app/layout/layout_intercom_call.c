#include "layout_define.h"
#include "layout_intercom_call.h"
enum
{
        intercom_call_obj_id_log_msg_bg,
        intercom_call_obj_id_title,
        intercom_call_obj_id_del_all,
        intercom_call_obj_id_del_all_label,
        intercom_call_obj_id_del_all_img,
        intercom_call_obj_id_cancel,
        intercom_call_obj_id_del_cancel,
        intercom_call_log_obj_del_cancel_label,
        intercom_call_obj_id_del,
        intercom_call_obj_id_tabview,
        intercom_call_obj_id_externsion,
        intercom_call_obj_id_guard,
        intercom_call_obj_id_abnormal_title,
        intercom_call_obj_id_id_base,

        intercom_call_obj_id_list = intercom_call_obj_id_id_base + 8,

};

typedef enum
{
        call_log_msg_bg_obj_id_msgdialog,
        call_log_msg_bg_obj_id_title,
        call_log_msg_bg_obj_id_confirm_id,
        call_log_msg_bg_obj_id_cancel_id,

} intercom_call_log_msg_bg_obj;

typedef enum
{
        call_log_list_cont_obj_checkbox_id,
        call_log_list_cont_obj_titie_id,
        call_log_list_cont_obj_tm_id,
        call_log_list_cont_obj_duration_id,

} call_log_list_cont_obj;

static int checkbox_s_num = 0;//call记录被选中的个数
static int enter_intercom_mode = 0;//进入intercom_call界面的途径（0：通过点击call机按键；1：通过点击呼叫记录的按键）
void enter_intercomm_call_mode_set(int mode)
{
        enter_intercom_mode = mode;
}

int enter_intercomm_call_mode_get(void)
{
        return enter_intercom_mode;
}

static void intercom_call_cancel_obj_click(lv_event_t *e)
{
        sat_layout_goto(home, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static lv_obj_t *intercom_call_table_view_obj_create(void)
{
        lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview);
        if (tabview != NULL)
        {
                return tabview;
        }
        tabview = lv_tabview_create(sat_cur_layout_screen_get(), LV_DIR_TOP, 56);
        lv_common_style_set_common(tabview, intercom_call_obj_id_tabview, 0, 80, 1024, 464 + 56, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        lv_obj_t *cont = lv_tabview_get_content(tabview);
        lv_common_style_set_common(cont, 0, 0, 56, 1024, 464, LV_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_t *page_1 = lv_tabview_add_tab(tabview, lang_str_get(INTERCOM_XLS_LANG_ID_CAll_CHANNEL));
        lv_common_style_set_common(page_1, 0, 0, 0, 1024, 464, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        lv_obj_t *page_2 = lv_tabview_add_tab(tabview, lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG));
        lv_common_style_set_common(page_2, 1, 0, 0, 1024, 464, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        lv_obj_t *btnmatrix = lv_tabview_get_tab_btns(tabview);
        lv_common_style_set_common(btnmatrix, 1, 0, 8, 1024, 40, LV_ALIGN_CENTER, LV_PART_MAIN);
        lv_common_style_set_text(btnmatrix, NULL, 0XFFFFFF, 0XFFFFFF, LV_ALIGN_CENTER, lv_font_normal, LV_STATE_PRESSED | LV_STATE_CHECKED, LV_PART_MAIN);
        lv_common_style_set_event(btnmatrix, NULL, true, LV_OPA_TRANSP, 0XFFFFFF, LV_PART_ITEMS, LV_OPA_COVER, 0x101010, LV_PART_ITEMS | LV_STATE_CHECKED);

        lv_common_style_set_boader(btnmatrix, 20, LV_OPA_TRANSP, 0, LV_BORDER_SIDE_NONE, 0XFFFFFF, LV_PART_ITEMS);
        lv_common_style_set_boader(btnmatrix, 20, LV_OPA_TRANSP, 0, LV_BORDER_SIDE_NONE, 0XFFFFFF, LV_PART_ITEMS | LV_STATE_CHECKED);

        lv_obj_set_style_pad_left(btnmatrix, 352, LV_PART_MAIN);
        lv_obj_set_style_pad_right(btnmatrix, 352, LV_PART_MAIN);

        return tabview;
}

static void intercom_call_abnormal_title_display(lv_obj_t * obj)
{

        int online_num = 0;
        extension_online_check(-1,&online_num);
        if( online_num == 0)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }else
        {
                lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
        }
        
}

static void intercom_extension_obj_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_target(e);
        lv_obj_t * parent = lv_obj_get_parent(obj);
        lv_obj_add_state(obj, LV_STATE_USER_1);
        lv_obj_clear_state(obj, LV_STATE_USER_2);
        if(obj->id == intercom_call_obj_id_guard)
        {
                lv_obj_t * abnormal = lv_obj_get_child_form_id(parent,intercom_call_obj_id_abnormal_title);
                lv_obj_clear_flag(abnormal,LV_OBJ_FLAG_HIDDEN);;
                lv_obj_t * extension = lv_obj_get_child_form_id(parent,intercom_call_obj_id_externsion);
                if(extension != NULL)
                {
                        lv_obj_clear_state(extension, LV_STATE_USER_1);
                        lv_obj_add_state(extension, LV_STATE_USER_2);
                } 
        }else
        {
                lv_obj_t * abnormal = lv_obj_get_child_form_id(lv_obj_get_parent(obj),intercom_call_obj_id_abnormal_title);
                intercom_call_abnormal_title_display(abnormal);
                lv_obj_t * guard = lv_obj_get_child_form_id(parent,intercom_call_obj_id_guard);
                if(guard != NULL)
                {
                        lv_obj_clear_state(guard, LV_STATE_USER_1);
                        lv_obj_add_state(guard, LV_STATE_USER_2);
                } 
        }
       

        
        
}

static int intercom_call_id_index = 0;
static void intercom_id_obj_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_target(e);
        intercom_call_id_index = lv_btnmatrix_get_selected_btn(obj) + 1;
        char user_name[64] = {0};
        char own_name[64] = {0};
        sprintf(user_name,"50%d",intercom_call_id_index);
        sprintf(own_name,"50%d",user_data_get()->system_mode & 0x0F);
        if(strncmp(user_name,own_name,4) == 0)//不允许打给自己
        {
                return;
        }
        if(extension_online_check(intercom_call_id_index,NULL))//对方分机设备是否在线
        {
                char number[128] = {0};
                sprintf(number, "sip:%s@%s:5066", user_name, user_data_get()->mastar_wallpad_ip);
                sat_linphone_call(number, false, false, NULL);
                //sat_ipcamera_device_discover_search(0x02);
                return;
        }
}

static bool intercom_linphone_outgoing_callback(char *arg)
{
        intercom_call_username_setting(arg);
        sat_layout_goto(intercom_talk, LV_SCR_LOAD_ANIM_FADE_IN, true);
        return true;
}



static bool intercom_linphone_outgoing_arly_media_register(char *arg)
{
        // intercom_call_username_setting(arg);
        // sat_layout_goto(intercom_talk, LV_SCR_LOAD_ANIM_FADE_IN, true);
        return true;
}

/************************************************************
** 函数说明: call_log_checkbox点击事件
** 作者: xiaoxiao
** 日期: 2023-05-17 22:09:28
** 参数说明:
** 注意事项:
************************************************************/
static void intercom_call_log_check_obj_click(lv_event_t *ev)
{
        lv_obj_t *del_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del);
        if (strncmp(del_obj->bg_img_src, resource_ui_src_get("btn_title_delete.png"), strlen(resource_ui_src_get("btn_title_delete.png"))) == 0)
        {
                return;
        }

        lv_obj_t *checkbox = lv_obj_get_child_form_id(lv_obj_get_parent(lv_event_get_current_target(ev)), call_log_list_cont_obj_checkbox_id);
        lv_obj_t *del_num = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_title);
        if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_n.png"), strlen(resource_ui_src_get("btn_checkbox_n.png"))) == 0)
        {

                checkbox_s_num++;
                lv_label_set_text_fmt(del_num,"%d %s",checkbox_s_num,lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED));

                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
        }
        else
        {
                checkbox_s_num--;
                lv_label_set_text_fmt(del_num,"%d %s",checkbox_s_num,lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED));
                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        }
}

/************************************************************
** 函数说明: call机列表
** 作者: xiaoxiao
** 日期: 2023-05-17 13:53:58
** 参数说明:
** 注意事项:
************************************************************/
static void intercom_call_list_item_create(lv_obj_t *parent)
{
        CALL_LOG_TYPE type;
        int ch;
        int duration;
        struct tm tm;
        int item_y = 0;
        int total = call_list_total_get();
        for (int i = total - 1; i >= 0; i--)
        {
                call_list_get(i, &type, &ch, &duration, &tm);
                char tm_buffer[64] = {0};
                char du_buffer[64];
                sprintf(du_buffer, "%02d:%02d", duration / 60, duration % 60);
                sprintf(tm_buffer, "%04d-%02d:%02d  %02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

                lv_obj_t *cont = lv_common_img_btn_create(parent, i, 0, item_y, 1024 - 80, 88,
                                                          NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                          0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0X9B9B9B,
                                                          0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0X9B9B9B,
                                                          NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                {
                        lv_common_img_text_btn_create(cont, call_log_list_cont_obj_titie_id, 0, 0, 350, 86,
                                                      intercom_call_log_check_obj_click, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      50, 25, 300, 43, 0,
                                                      lang_str_get(INTERCOM_XLS_LANG_ID_DOOR_CAMERA), 0XFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                      0, 15, 80, 48, 1,
                                                      type == IN_AND_NO_ANSWER ? (char *)resource_ui_src_get("ic_list_call_absence.png") : type == CALL_OUT ? (char *)resource_ui_src_get("ic_list_call_transmit.png")
                                                                                                                                                            : (char *)resource_ui_src_get("ic_list_call_receive.png"),
                                                      LV_OPA_COVER, 0x00a8ff, LV_ALIGN_LEFT_MID);
                }

                {
                        lv_obj_t *checkbox = lv_common_img_btn_create(cont, call_log_list_cont_obj_checkbox_id, 0, 0, 32, 86,
                                                                      intercom_call_log_check_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                      resource_ui_src_get("btn_checkbox_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        lv_obj_add_flag(checkbox, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_set_ext_click_area(checkbox, 30);
                }
                {
                        lv_common_text_create(cont, call_log_list_cont_obj_tm_id, 800, 40, 124, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              du_buffer, 0X9B9B9B, 0X9B9B9B, LV_TEXT_ALIGN_RIGHT, lv_font_small);
                }

                {
                        lv_common_text_create(cont, call_log_list_cont_obj_duration_id, 700, 10, 224, 29,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              tm_buffer, 0X9B9B9B, 0X9B9B9B, LV_TEXT_ALIGN_RIGHT, lv_font_normal);
                        // lv_obj_set_style_pad_left(obj, 22, LV_PART_MAIN);
                }
                item_y += 88;
        }
}
/************************************************************
** 函数说明: tabview 滑动事件
** 作者: xiaoxiao
** 日期: 2023-05-17 21:34:51
** 参数说明:
** 注意事项:
************************************************************/
static void intercom_call_log_tableview_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        lv_obj_t *del_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del);
        lv_obj_t *del_cancel = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del_cancel);
        lv_obj_t *title = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_title);
        int id = lv_tabview_get_tab_act(obj);
        if (id == 1)
        {
                if (strncmp(del_obj->bg_img_src, resource_ui_src_get("btn_title_check.png"), strlen(resource_ui_src_get("btn_title_check.png"))) == 0)
                {
                        lv_label_set_text_fmt(title, "%d %s",checkbox_s_num,lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED));
                        lv_obj_clear_flag(del_cancel, LV_OBJ_FLAG_HIDDEN);
                }

                lv_obj_clear_flag(del_obj, LV_OBJ_FLAG_HIDDEN);
                layout_last_call_new_flag_set(false);
        }
        else
        {

                lv_label_set_text(title, lang_str_get(HOME_XLS_LANG_ID_CALL));
                lv_obj_add_flag(del_obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(del_cancel, LV_OBJ_FLAG_HIDDEN);
        }
}

static bool layout_intercom_call_log_select_get(void)
{
        lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview);
        lv_obj_t *log_list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_tabview_get_content(tabview), 1), intercom_call_obj_id_list);

        int total = call_list_total_get();
        int sel_num = 0;
        for (int i = total - 1; i >= 0; i--)
        {
                lv_obj_t *list_btn = lv_obj_get_child_form_id(log_list, i);
                lv_obj_t *checkbox = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_checkbox_id);
                if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_s.png"), strlen(resource_ui_src_get("btn_checkbox_s.png"))) == 0)
                {
                        sel_num++;
                }
        }
        return total == sel_num ? true : false;
}

/************************************************************
** 函数说明: 删除消息框取消事件
** 作者: xiaoxiao
** 日期: 2023-05-18 15:08:16
** 参数说明:
** 注意事项:
************************************************************/
static void layout_intercom_call_log_msgbox_cancel(lv_event_t *ev)
{
        lv_obj_del(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_log_msg_bg));
}

/************************************************************
** 函数说明: 删除消息框确认事件
** 作者: xiaoxiao
** 日期: 2023-05-18 15:08:48
** 参数说明:
** 注意事项:
************************************************************/
static void layout_intercom_call_log_msgbox_confirm_click(lv_event_t *ev)
{
        int total = call_list_total_get();
        lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview);
        lv_obj_t *log_list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_tabview_get_content(tabview), 1), intercom_call_obj_id_list);
        if (layout_intercom_call_log_select_get() == true)
        {
                call_list_del_all();
                layout_last_call_new_flag_set(false);
        }
        else
        {
                for (int i = total - 1; i >= 0; i--)
                {
                        lv_obj_t *list_btn = lv_obj_get_child_form_id(log_list, i);
                        lv_obj_t *checkbox = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_checkbox_id);
                        if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_s.png"), strlen(resource_ui_src_get("btn_checkbox_s.png"))) == 0)
                        {
                                call_list_del(i);
                        }
                }
        }
        enter_intercomm_call_mode_set(1);
        sat_layout_goto(intercom_call, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

/************************************************************
** 函数说明: 删除按键点击事件
** 作者: xiaoxiao
** 日期: 2023-05-17 21:35:31
** 参数说明:
** 注意事项:
************************************************************/
static void intercom_call_log_del_obj_click(lv_event_t *ev)
{
        int total = call_list_total_get();
        lv_obj_t *obj = lv_event_get_current_target(ev);
        lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview);
        lv_obj_t *log_list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_tabview_get_content(tabview), 1), intercom_call_obj_id_list);
        lv_obj_t *title = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_title);
        lv_obj_t *del_all = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_tabview_get_content(tabview), 1), intercom_call_obj_id_del_all);
        lv_obj_t *del_cancel = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del_cancel);
        lv_obj_t *back = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_cancel);

        if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_title_delete.png"), strlen(resource_ui_src_get("btn_title_delete.png"))) == 0)
        {

                lv_label_set_text_fmt(title,"%d %s",checkbox_s_num,lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED));
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_title_check.png"), LV_PART_MAIN);
                lv_obj_clear_flag(del_cancel, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(del_all, LV_OBJ_FLAG_HIDDEN);
                lv_obj_set_y(log_list, 88);
                lv_obj_add_flag(back, LV_OBJ_FLAG_HIDDEN);

                for (int i = total - 1; i >= 0; i--)
                {
                        lv_obj_t *list_btn = lv_obj_get_child_form_id(log_list, i);
                        lv_obj_t *checkbox = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_checkbox_id);
                        lv_obj_t *title_cont = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_titie_id);

                        lv_obj_set_x(title_cont, 40);
                        lv_obj_clear_flag(checkbox, LV_OBJ_FLAG_HIDDEN);
                }
        }
        else
        {

                lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_log_msg_bg);
                if (cont != NULL)
                {
                        return;
                }

                lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(intercom_call_obj_id_log_msg_bg, call_log_msg_bg_obj_id_msgdialog, 282, 93, 460, 352);
                setting_msgdialog_msg_create(masgbox, call_log_msg_bg_obj_id_title, lang_str_get(LAYOUT_CALL_LOG_XLS_LANG_ID_DEL_TIPS), 0, 110, 460, 120);
                setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, call_log_msg_bg_obj_id_confirm_id, call_log_msg_bg_obj_id_cancel_id, layout_intercom_call_log_msgbox_confirm_click, layout_intercom_call_log_msgbox_cancel);
        }
}

/************************************************************
** 函数说明: call机记录删除
** 作者: xiaoxiao
** 日期: 2023-05-18 08:58:32
** 参数说明:
** 注意事项:
************************************************************/
static void intercom_call_log_obj_del_cancel_click(lv_event_t *ev)
{
        int total = call_list_total_get();
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del_cancel);
        lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview);
        lv_obj_t *cont = lv_tabview_get_content(tabview);
        lv_obj_t *log_list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(cont, 1), intercom_call_obj_id_list);
        lv_obj_t *del_all = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_tabview_get_content(tabview), 1), intercom_call_obj_id_del_all);
        lv_obj_t *del_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del);
        lv_obj_t *back = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_cancel);
        lv_obj_t *title = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_title);

        checkbox_s_num = 0;
        lv_label_set_text(title, lang_str_get(HOME_XLS_LANG_ID_CALL));
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(back, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_img_src(lv_obj_get_child_form_id(del_all, intercom_call_obj_id_del_all_img), resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
        lv_obj_add_flag(del_all, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_img_src(del_obj, resource_ui_src_get("btn_title_delete.png"), LV_PART_MAIN);
        lv_obj_set_height(log_list, 466);
        lv_obj_set_y(log_list, 0);
        for (int i = total - 1; i >= 0; i--)
        {
                lv_obj_t *list_btn = lv_obj_get_child_form_id(log_list, i);
                lv_obj_t *checkbox = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_checkbox_id);
                lv_obj_t *title_cont = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_titie_id);

                lv_obj_set_x(title_cont, 0);
                lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
                lv_obj_add_flag(checkbox, LV_OBJ_FLAG_HIDDEN);
        }

        // if (strncmp(obj->bg_img_src, resource_ui_src_get("btn_title_delete.png"), strlen(resource_ui_src_get("btn_title_delete.png"))) == 0)
        // {

        //         lv_label_set_text_fmt(title,"%d seleced",checkbox_s_num);
        //         lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_title_check.png"), LV_PART_MAIN);
        //         lv_obj_clear_flag(del_cancel,LV_OBJ_FLAG_HIDDEN);
        //         lv_obj_clear_flag(del_all,LV_OBJ_FLAG_HIDDEN);
        //         lv_obj_set_y(log_list,88);
        //         lv_obj_add_flag(back,LV_OBJ_FLAG_HIDDEN);

        //         for (int i = total - 1; i >= 0; i--)
        //         {
        //                 lv_obj_t * list_btn = lv_obj_get_child_form_id(log_list,i);
        //                 lv_obj_t * checkbox = lv_obj_get_child_form_id(list_btn,call_log_list_cont_obj_checkbox_id);
        //                 lv_obj_t * title_cont = lv_obj_get_child_form_id(list_btn,call_log_list_cont_obj_titie_id);

        //                 lv_obj_set_x(title_cont,40);
        //                 lv_obj_clear_flag(checkbox,LV_OBJ_FLAG_HIDDEN);

        //         }

        // }
        // else
        // {
        //         for (int i = total - 1; i >= 0; i--)
        //         {
        //                 lv_obj_t * list_btn = lv_obj_get_child_form_id(log_list,i);
        //                 lv_obj_t * checkbox = lv_obj_get_child_form_id(list_btn,call_log_list_cont_obj_checkbox_id);
        //                 if (strncmp(checkbox->bg_img_src, resource_ui_src_get("btn_checkbox_s.png"), strlen(resource_ui_src_get("btn_checkbox_s.png"))) == 0)
        //                 {
        //                         call_list_del(i);
        //                 }

        //         }
        //         enter_intercomm_call_mode_set(1);
        //         sat_layout_goto(intercom_call, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);

        // }
}

/************************************************************
** 函数说明: 全选删除
** 作者: xiaoxiao
** 日期: 2023-05-18 09:03:25
** 参数说明:
** 注意事项:
************************************************************/
static void intercom_call_log_obj_del_all_click(lv_event_t *ev)
{
        int total = call_list_total_get();
        lv_obj_t *obj = lv_event_get_current_target(ev);
        lv_obj_t *obj_img = lv_obj_get_child_form_id(obj, intercom_call_obj_id_del_all_img);
        lv_obj_t *tabview = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview);
        lv_obj_t *cont = lv_tabview_get_content(tabview);
        lv_obj_t *log_list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(cont, 1), intercom_call_obj_id_list);
        lv_obj_t *title = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_title);
        lv_obj_t *del_cancel = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_del_cancel);
        lv_obj_t *back = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_cancel);

        if (strncmp(obj_img->bg_img_src, resource_ui_src_get("btn_checkbox_s.png"), strlen(resource_ui_src_get("btn_checkbox_s.png"))) == 0)
        {
                checkbox_s_num = 0;
                lv_label_set_text_fmt(title,"%d %s",checkbox_s_num,lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED));
                lv_obj_set_style_bg_img_src(obj_img, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
                for (int i = total - 1; i >= 0; i--)
                {
                        lv_obj_t *list_btn = lv_obj_get_child_form_id(log_list, i);
                        lv_obj_t *checkbox = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_checkbox_id);

                        lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_n.png"), LV_PART_MAIN);
                }
        }

        else
        {

                lv_obj_set_style_bg_img_src(obj_img, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
                checkbox_s_num = total;
                lv_label_set_text_fmt(title,"%d %s",checkbox_s_num,lang_str_get(INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED));
                lv_obj_clear_flag(del_cancel, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(back, LV_OBJ_FLAG_HIDDEN);
                for (int i = total - 1; i >= 0; i--)
                {
                        lv_obj_t *list_btn = lv_obj_get_child_form_id(log_list, i);
                        lv_obj_t *checkbox = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_checkbox_id);
                        lv_obj_t *title_cont = lv_obj_get_child_form_id(list_btn, call_log_list_cont_obj_titie_id);

                        lv_obj_set_x(title_cont, 40);
                        lv_obj_clear_flag(checkbox, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_set_style_bg_img_src(checkbox, resource_ui_src_get("btn_checkbox_s.png"), LV_PART_MAIN);
                }
        }
}


static void sat_layout_enter(intercom_call)
{
        standby_timer_close();
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), intercom_call_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(HOME_XLS_LANG_ID_CALL), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步下一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_call_obj_id_cancel, 35, 16, 48, 48,
                                         intercom_call_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /************************************************************
        ** 函数说明: 删除选中取消
        ** 作者: xiaoxiao
        ** 日期: 2023-05-18 14:37:35
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_obj_t *obj = lv_common_img_text_btn_create(sat_cur_layout_screen_get(), intercom_call_obj_id_del_cancel, 27, 15, 120, 48,
                                                              intercom_call_log_obj_del_cancel_click, LV_OPA_COVER, 0x0096ff, LV_OPA_COVER, 0x0096ff,
                                                              90, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              90, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 5, 120, 48, 0,
                                                              lang_str_get(INSTALLATION_XLS_LANG_ID_CACCEL), 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                              3, 10, 77, 77, -1,
                                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明:table view 创建
         ***********************************************/
        lv_obj_t *tabview = intercom_call_table_view_obj_create();
        {
                lv_obj_add_event_cb(tabview, intercom_call_log_tableview_click, LV_EVENT_VALUE_CHANGED, NULL);
                lv_obj_t *cont = lv_tabview_get_content(tabview);
                lv_obj_t *page_1 = lv_obj_get_child_form_id(cont, 0);
                {
                        lv_obj_t *exten_txt_obj = lv_common_text_create(page_1, intercom_call_obj_id_externsion, 0, 8, 231, 231,
                                                                        intercom_extension_obj_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x0096ff,
                                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                        lang_str_get(INTERCOM_XLS_LANG_ID_EXTENSION), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);

                        lv_obj_set_style_pad_top(exten_txt_obj, 94, LV_PART_MAIN);
                        lv_obj_set_style_bg_color(exten_txt_obj, lv_color_hex(0x0096ff), LV_STATE_USER_1);
                        lv_obj_set_style_bg_opa(exten_txt_obj, LV_OPA_COVER, LV_STATE_USER_1);
                        lv_obj_set_style_bg_opa(exten_txt_obj, LV_OPA_TRANSP, LV_STATE_USER_2);
                        lv_obj_add_state(exten_txt_obj, LV_STATE_USER_1);


                        lv_obj_t *guard_txt_obj = lv_common_text_create(page_1, intercom_call_obj_id_guard, 0, 231 + 8, 231, 231,
                                                                        intercom_extension_obj_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x0096ff,
                                                                        0, 1, LV_BORDER_SIDE_RIGHT, LV_OPA_COVER, 0x101010,
                                                                        0, 1, LV_BORDER_SIDE_RIGHT, LV_OPA_COVER, 0x101010,
                                                                        lang_str_get(INTERCOM_XLS_LANG_ID_GUARD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);

                        lv_obj_set_style_pad_top(guard_txt_obj, 94, LV_PART_MAIN);
                        lv_obj_set_style_bg_color(guard_txt_obj, lv_color_hex(0x0096ff), LV_STATE_USER_1);
                        lv_obj_set_style_bg_opa(guard_txt_obj, LV_OPA_COVER, LV_STATE_USER_1);
                        lv_obj_set_style_bg_opa(guard_txt_obj, LV_OPA_TRANSP, LV_STATE_USER_2);
                        lv_obj_add_state(guard_txt_obj, LV_STATE_USER_2);

                        lv_obj_t *btnmatrix = lv_common_number_input_keyboard_create(page_1, intercom_call_obj_id_id_base, 231, 8, 793, 384,
                                                                                     intercom_id_obj_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x00a8ff,
                                                                                     0, 1, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x101010,
                                                                                     0, 3, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x101010,
                                                                                     0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                                                     0, 0);
                        if (0/*(user_data_get()->system_mode & 0xF0) != 0xF0*/)
                        {
                                lv_obj_add_flag(exten_txt_obj, LV_OBJ_FLAG_HIDDEN);
                                lv_obj_add_flag(guard_txt_obj, LV_OBJ_FLAG_HIDDEN);
                                lv_obj_set_style_x(btnmatrix, 0, LV_PART_MAIN);
                                lv_obj_set_style_width(btnmatrix, 1024, LV_PART_MAIN);
                        }

                        lv_obj_set_style_bg_color(btnmatrix, lv_color_hex(0x194861), LV_STATE_DISABLED | LV_PART_ITEMS);
                        lv_obj_set_style_bg_opa(btnmatrix, LV_OPA_COVER, LV_STATE_DISABLED | LV_PART_ITEMS);

                        static const char *btnm_map[] = { 
                            "ID\n1", "ID\n2", "ID\n3", "\n",
                            "ID\n4", "ID\n5", "ID\n6", "\n",
                            "ID\n7", "ID\n8", "ID\n9", ""};
                        static const char *btnm_img_map[] = {
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                        };
                        lv_btnmatrix_set_map(btnmatrix, btnm_map);
                        lv_btnmatrix_set_btn_bg_map(btnmatrix, btnm_img_map);
                        lv_btnmatrix_set_btn_ctrl(btnmatrix, 8, LV_BTNMATRIX_CTRL_HIDDEN);
                        lv_btnmatrix_set_btn_ctrl(btnmatrix, network_data_get()->sip_user[11] - 49, LV_BTNMATRIX_CTRL_DISABLED);

                        {
                                lv_obj_t *adnormal_obj = lv_common_text_create(page_1, intercom_call_obj_id_abnormal_title, 231, 8, 793, 384,
                                                NULL, LV_OPA_COVER, 0, LV_OPA_COVER, 0x0096ff,
                                                0, 1, LV_BORDER_SIDE_RIGHT, LV_OPA_COVER, 0x101010,
                                                0, 1, LV_BORDER_SIDE_RIGHT, LV_OPA_COVER, 0x101010,
                                                lang_str_get(CALL_XLS_LANG_ID_CANNOT_USE_BEFORE_SYSTEMSETTING), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                                intercom_call_abnormal_title_display(adnormal_obj);
                                lv_obj_set_style_pad_top(adnormal_obj, 180, LV_PART_MAIN);
                        }
                }
                lv_obj_t *page_2 = lv_obj_get_child_form_id(cont, 1);
                {
                        {
                                lv_obj_t *del_all = lv_common_img_text_btn_create(page_2, intercom_call_obj_id_del_all, 40, 0, 1024 - 80, 88,
                                                                                  intercom_call_log_obj_del_all_click, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                                  0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0X9B9B9B,
                                                                                  0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0X9B9B9B,
                                                                                  50, 20, 224, 43, intercom_call_obj_id_del_all_label,
                                                                                  lang_str_get(LAYOUT_ALARM_LIST_XLS_LANG_ID_DELETED_ALL), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_large,
                                                                                  0, 0, 32, 86, intercom_call_obj_id_del_all_img,
                                                                                  resource_ui_src_get("btn_checkbox_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                                lv_obj_add_flag(del_all, LV_OBJ_FLAG_HIDDEN);
                        }
                        lv_obj_t *list = lv_list_create(page_2);
                        lv_common_style_set_common(list, intercom_call_obj_id_list, 40, 0, 1024 - 40, 464, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                        intercom_call_list_item_create(list);
                }

                {
                        lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_call_obj_id_del, 936, 16, 80, 48,
                                                                 intercom_call_log_del_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 resource_ui_src_get("btn_title_delete.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        if (lv_tabview_get_tab_act(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_call_obj_id_tabview)) == 0)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                }
                lv_tabview_set_act(tabview, enter_intercom_mode, LV_ANIM_OFF);
        }
        user_linphone_call_outgoing_call_register(intercom_linphone_outgoing_callback);

        user_linphone_call_outgoing_early_media_register(intercom_linphone_outgoing_arly_media_register);
}

static void sat_layout_quit(intercom_call)
{
        user_linphone_call_outgoing_call_register(NULL);
        user_linphone_call_outgoing_early_media_register(NULL);
        standby_timer_restart(true);
        checkbox_s_num = 0;
}
sat_layout_create(intercom_call);
