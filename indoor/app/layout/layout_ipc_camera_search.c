#include "layout_define.h"
#include "onvif.h"
#include "layout_ipc_camera.h"
enum
{
        ipc_camera_search_obj_id_title,
        ipc_camera_search_obj_id_tips,
        ipc_camera_search_obj_id_cancel,

        ipc_camera_search_obj_id_searched_door_camera_title,
        ipc_camera_search_obj_id_door_camera_list,

        ipc_camera_search_obj_id_msg_bg,
};

typedef enum
{
        ipc_camera_search_msg_obj_id_msg,
        ipc_camera_search_msg_obj_id_text,
        ipc_camera_search_msg_obj_id_confirm,
        ipc_camera_search_msg_obj_id_cancel,
} ipc_camera_search_msg_bg_obj_id;
static void ipc_camera_search_cancel_click(lv_event_t *ev)
{
        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void ipc_search_door_camera_modify_default_passwd_confirm(lv_event_t *ev)
{
        layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_CHANGE_PWD);
        ipc_camera_password_state_set(1);
        sat_layout_goto(ipc_camera_input, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static void ipc_search_door_camera_modify_default_passwd_concel(lv_event_t *ev)
{
        setting_msgdialog_msg_del(ipc_camera_search_obj_id_msg_bg);
}

static void ipc_search_door_camera_modify_default_passwd_check()
{
        lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_msg_bg);
        if (masgbox != NULL)
        {
                setting_msgdialog_msg_del(ipc_camera_search_obj_id_msg_bg);
        }
        masgbox = setting_msgdialog_msg_bg_create(ipc_camera_search_obj_id_msg_bg, ipc_camera_search_msg_obj_id_msg, 282, 123, 460, 323);
        setting_msgdialog_msg_create(masgbox, ipc_camera_search_msg_obj_id_text, lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_DEFAULT_PASSWORD_IS_SET), 20, 40, 420, 180, false);
        setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, ipc_camera_search_msg_obj_id_confirm, ipc_camera_search_msg_obj_id_cancel, ipc_search_door_camera_modify_default_passwd_confirm, ipc_search_door_camera_modify_default_passwd_concel);
}
static void ipc_camera_serarch_list_click(lv_event_t *ev)
{

        lv_obj_t *parent = lv_event_get_current_target(ev);
        if (parent == NULL)
        {
                return;
        }
        layout_ipc_camera_edit_index_set(parent->id);
        sat_ipcamera_device_status_reset();
        sat_ipcamera_user_password_set(parent->id, "admin", "123456789");
        struct ipcamera_info *ipc_device = sat_ipcamera_node_data_get(parent->id);
        if (1)
        {
                for (int i = 0; i < 1; i++)
                {
                        char name[64] = {0};
                        printf("ipc_device[parent->id].ipaddr is %s\n", ipc_device[parent->id].ipaddr);
                        if (ipc_camera_device_name_get(name, ipc_device[parent->id].ipaddr, ipc_device[parent->id].port, ipc_device[parent->id].username, ipc_device[parent->id].password, ipc_device[parent->id].auther_flag, 1000) == true)

                        {
                                ipc_search_door_camera_modify_default_passwd_check();
                                return;
                        }
                }
        }

        if (layout_ipc_cmeara_is_doorcamera_get() == true) // if(1)
        {
                layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_PWD);
        }
        else
        {
                layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_SEARCH | IPC_CAMERA_FLAG_INPUT_USER);
        }
        sat_layout_goto(ipc_camera_input, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static lv_obj_t *ipc_camera_search_list_create(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_door_camera_list);
        if (list != NULL)
        {
                return list;
        }
        list = lv_list_create(sat_cur_layout_screen_get());
        lv_common_style_set_common(list, ipc_camera_search_obj_id_door_camera_list, 48, layout_ipc_cmeara_is_doorcamera_get() ? 184 : 136, 928, (600 - 200), LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        return list;
}

static void cctv_search_list_display(void)
{
        lv_obj_t *list = ipc_camera_search_list_create();
        lv_obj_clean(list);

        int item_y = 0;

        int user_count = sat_ipcamera_online_num_get();

        for (int i = 0; (i < user_count); i++)
        {
                bool registered = false;
                for (int j = 0; j < 8; j++)
                {
                        if (layout_ipc_cmeara_is_doorcamera_get())
                        {
                                if (network_data_get()->door_device[j].rtsp[0].rtsp_url[0] != 0)
                                {
                                        if (strncmp(network_data_get()->door_device[j].ipaddr, sat_ipcamera_ipaddr_get(i), sizeof(network_data_get()->door_device[j].ipaddr)) == 0)
                                        {
                                                registered = true;
                                                break;
                                        }
                                }
                        }
                        else
                        {
                                if (network_data_get()->cctv_device[j].rtsp[0].rtsp_url[0] != 0)
                                {
                                        if (strncmp(network_data_get()->cctv_device[j].ipaddr, sat_ipcamera_ipaddr_get(i), sizeof(network_data_get()->cctv_device[j].ipaddr)) == 0)
                                        {
                                                registered = true;
                                                break;
                                        }
                                }
                        }
                }
                if (registered == true)
                {
                        continue;
                }
                lv_common_setting_btn_title_sub_info_img_create(list, i, 48, item_y, 928, 88,
                                                                ipc_camera_serarch_list_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                58, 8, 618, 43, 0,
                                                                (char *)sat_ipcamera_door_name_get(i), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                58, 51, 618, 29, 1,
                                                                (char *)sat_ipcamera_ipaddr_get(i), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 20, 48, 48, 3,
                                                                resource_ui_src_get("ic_list_new.png"), LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                item_y += 88;
        }
}
static lv_timer_t *p_ipc_camera_search_timer = NULL;
static void ipc_camera_emty_timer(lv_timer_t *ptimer)
{
        printf("search loading...\n");
        sat_ipcamera_device_discover_search(layout_ipc_cmeara_is_doorcamera_get());
}
static void ipc_camera_state_func(unsigned int type, unsigned int num)
{
        if ((type == 1) && (num > 0))
        {
                cctv_search_list_display();
                if (p_ipc_camera_search_timer != NULL)
                {
                        lv_timer_set_period(p_ipc_camera_search_timer, 10 * 1000);
                        lv_timer_reset(p_ipc_camera_search_timer);
                }
        }
}
static void sat_layout_enter(ipc_camera_search)
{
        standby_timer_close();
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      layout_ipc_cmeara_is_doorcamera_get() == true ? lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_DOOR_CAMERA)
                                                                                    : lang_str_get(HOME_XLS_LANG_ID_CCTV),
                                      0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        {
                if (layout_ipc_cmeara_is_doorcamera_get())
                {
                        lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_tips, 0, 88, 1024, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_IF_YOUT_PRESS),
                                              0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
                }
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步下一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_cancel, 35, 15, 48, 48,
                                         ipc_camera_search_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:50
         ** 说明: 显示搜索的设备
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_searched_door_camera_title, 16, layout_ipc_cmeara_is_doorcamera_get() ? 146 : 88, 584, 48,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(layout_ipc_cmeara_is_doorcamera_get() == true ? DOOR_CAMERA_SEARCH_XLS_LANG_ID_SEARCHED_DOOR_CAMERA : DOOR_CAMERA_SEARCH_XLS_LANG_ID_SEARCHED_CCTV),
                                      0X00A8ff, 0X00A8ff, LV_TEXT_ALIGN_LEFT, lv_font_small);
        }
        if (sat_pre_layout_get() == sat_playout_get(ipc_camera_register))
        {
                p_ipc_camera_search_timer = lv_sat_timer_create(ipc_camera_emty_timer, 3000, NULL);
                lv_timer_reset(p_ipc_camera_search_timer);
        }
        else
        {
                cctv_search_list_display();
        }
        ipcamera_state_callback_register(ipc_camera_state_func);
}
static void sat_layout_quit(ipc_camera_search)
{
        if (user_data_get()->is_device_init == true) // 启动设置会有机会进入这里，所以要加判断
        {
                standby_timer_restart(true, true);
        }
        ipcamera_state_callback_register(NULL);
        // for(int i = 0; i < network_data_get()->cctv_device_count; i++)
        // {
        //         char ch[5];
        //         strncpy(ch,network_data_get()->cctv_device[i].door_name,5);
        //         if(strncmp(ch,"CCTV1",5) == 0)
        //         {
        //                 continue;;
        //         }

        // }
}

sat_layout_create(ipc_camera_search);
