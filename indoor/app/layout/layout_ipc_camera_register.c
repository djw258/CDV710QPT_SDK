#include "layout_define.h"
#include "layout_ipc_camera.h"
#include "common/sat_ipcamera.h"
#include "onvif.h"
/*进入模式设置:door camera /ipc*/
static bool ipc_cmeara_is_doorcamera = true;
bool layout_ipc_cmeara_is_doorcamera_get(void)
{
        return ipc_cmeara_is_doorcamera;
}
void layout_ipc_cmeara_is_doorcamera_set(bool yenon)
{
        ipc_cmeara_is_doorcamera = yenon;
}

enum
{
        ipc_camera_registered_obj_id_title,
        ipc_camera_registered_obj_id_cancel,
        ipc_camera_registered_obj_id_next,

        ipc_camera_registered_obj_id_ipc_camera_registered_cont,

        ipc_camera_registered_obj_id_door_camera_list,
        ipc_camera_registered_obj_id_registered_door_camera_title,

        ipc_camera_registered_obj_id_msgbox,
};
static lv_obj_t *ipc_camera_register_msgbox_create(const char *title, const char *msg, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_msgbox, 0, 0, 1024, 600,
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

        lv_common_img_btn_create(msgbox, 2, 0, 281, 230, 62,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, 3, 230, 281, 230, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}

static void ipc_camera_registered_cancel_click(lv_event_t *ev)
{
        if (user_data_get()->is_device_init == 0)
        {
                sat_layout_goto(setting_time, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
}

static void ipc_camera_registered_next_click(lv_event_t *ev)
{
        sat_layout_goto(setting_complete, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void ipc_camera_registered_register_list_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        if ((obj == NULL) || (obj->id > 7))
        {
                return;
        }
        int count = 0;
        while (sat_ipcamera_device_status_get() == true)

        {
                usleep(1000);
                if ((count++) > 500)

                {
                        printf("wait ipacamera status timeout \n");
                        return;
                }
        }

        layout_ipc_camera_edit_index_set(obj->id);
        layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_REGISTER);
        sat_layout_goto(ipc_camera_display, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void ipc_camera_registered_register_edit_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_t *parent = lv_obj_get_parent(obj);
        if (parent == NULL)
        {
                return;
        }

        //   int total = layout_ipc_cmeara_is_doorcamera_get() == true ? network_data_get()->door_device_count : network_data_get()->cctv_device_count;
        // if (parent->id > total)
        // {
        //         return;
        // }

        if (0) // (layout_ipc_cmeara_is_doorcamera_get() == false)
        {
                layout_ipc_camera_edit_index_set(parent->id);
                layout_ipc_camera_input_flag_set(IPC_CAMERA_FLAG_CHANGE_NAME);
                sat_layout_goto(ipc_camera_input, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        else
        {
                layout_ipc_camera_edit_index_set(parent->id);
                sat_layout_goto(ipc_camera_edit, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
}

static void ipc_camera_register_del_msgbox_confirm_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_msgbox);
        if (obj == NULL)
        {
                return;
        }

        lv_obj_t *item = (lv_obj_t *)obj->user_data;
        int device_index = item->id;
        if ((item == NULL) || (device_index > 7))
        {
                return;
        }

        if (layout_ipc_cmeara_is_doorcamera_get() == true)
        {
                sat_ipcamera_device_delete(device_index, 1500);
                memset(&(network_data_get()->door_device[device_index]), 0, sizeof(network_data_get()->door_device[device_index]));
        }
        else
        {
                memset(&(network_data_get()->cctv_device[device_index]), 0, sizeof(network_data_get()->cctv_device[device_index]));
                user_data_get()->alarm.cctv_sensor[device_index] = 0;
        }
        network_data_save();
        sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void ipc_camera_register_msgbox_del_cancel_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_msgbox);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}

static void ipc_camera_registered_register_del_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_t *parent = lv_obj_get_parent(obj);
        if ((parent == NULL) || (parent->id > 7))
        {
                return;
        }

        lv_obj_t *msgbox = ipc_camera_register_msgbox_create(lang_str_get(LAYOUT_CALL_LOG_XLS_LANG_ID_DEL),
                                                             lang_str_get(layout_ipc_cmeara_is_doorcamera_get() ? DOOR_CAMERA_SEARCH_XLS_LANG_ID_DO_YOU_WANT_DELETE_DOOR_CAMERA : DOOR_CAMERA_SEARCH_XLS_LANG_ID_DO_YOU_WANT_DELETE_CCTV),
                                                             ipc_camera_register_msgbox_del_cancel_click, ipc_camera_register_del_msgbox_confirm_click);

        msgbox->user_data = parent;
}
static void ipc_camera_registered_add_serch_click(lv_event_t *ev)
{
        sat_layout_goto(ipc_camera_search, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static lv_obj_t *ipc_camera_registered_list_create(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_door_camera_list);
        if (list != NULL)
        {
                return list;
        }
        list = lv_list_create(sat_cur_layout_screen_get());
        lv_common_style_set_common(list, ipc_camera_registered_obj_id_door_camera_list, 48, 194, 928, (600 - 194), LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        return list;
}

static void layout_ipc_cameara_register_online_check_timer(lv_timer_t *timer)
{
        lv_timer_set_period(timer, 10000);
        struct ipcamera_info *ipc_device = layout_ipc_cmeara_is_doorcamera_get() == false ? network_data_get()->cctv_device : network_data_get()->door_device;
        bool *result = (bool *)timer->user_data;

        lv_obj_t *list = ipc_camera_registered_list_create();
        for (int i = 0; i < DEVICE_MAX; i++)
        {
                if (ipc_device[i].rtsp[0].rtsp_url[0] == 0)
                {
                        continue;
                }
                lv_obj_t *obj = lv_obj_get_child_form_id(list, i);
                obj = lv_obj_get_child_form_id(obj, 2);
                if (obj == NULL)
                {
                        continue;
                }
                if (result[i] == true)
                {
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_detect.png"), LV_PART_MAIN);
                        continue;
                }
                char name[64] = {0};
                result[i] = ipc_camera_device_name_get(name, ipc_device[i].ipaddr, ipc_device[i].port, ipc_device[i].username, ipc_device[i].password, ipc_device[i].auther_flag, 1000);

                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(result[i] ? "ic_detect.png" : "ic_error.png"), LV_PART_MAIN);
        }
}

static void sat_layout_enter(ipc_camera_register)
{

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      layout_ipc_cmeara_is_doorcamera_get() == false ? lang_str_get(HOME_XLS_LANG_ID_CCTV) : lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_DOOR_CAMERA),
                                      0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步下一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_cancel, 35, 15, 48, 48,
                                         ipc_camera_registered_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                if (user_data_get()->is_device_init == 0)
                {
                        lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_next, 952, 15, 48, 48,
                                                 ipc_camera_registered_next_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_title_next.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                }
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:50
         ** 说明: 搜索户外机
         ***********************************************/
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_ipc_camera_registered_cont, 352, 95, 319, 35,
                                              ipc_camera_registered_add_serch_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              160, 0, 150, 35, 0,
                                              layout_ipc_cmeara_is_doorcamera_get() == false ? lang_str_get(HOME_XLS_LANG_ID_CCTV) : lang_str_get(DOOR_CAMERA_SEARCH_XLS_LANG_ID_DOOR_CAMERA_SEARCH),
                                              0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_large,
                                              70, 1, 80, 32, 1,
                                              (const char *)resource_ui_src_get("wifi_add.png"), LV_OPA_50, 0x00a8ff, LV_ALIGN_RIGHT_MID);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:50
         ** 说明: 搜索户外机
         ***********************************************/
        {
                //   int camera_count = layout_ipc_cmeara_is_doorcamera_get() == false ? network_data_get()->cctv_device_count : network_data_get()->door_device_count;

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: 显示注册的设备
                 ***********************************************/
                // if (camera_count > 0)
                {
                        lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_registered_door_camera_title, 0, 146, 584, 48,
                                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              lang_str_get(layout_ipc_cmeara_is_doorcamera_get() == false ? DOOR_CAMERA_SEARCH_XLS_LANG_ID_REGISTER_CCTV : DOOR_CAMERA_SEARCH_XLS_LANG_ID_REGISTERED_DOOR_CAMERA),
                                                              0X00A8ff, 0X00A8ff, LV_TEXT_ALIGN_LEFT, lv_font_small);

                        lv_obj_set_style_pad_left(obj, 16, LV_PART_MAIN);

                        struct ipcamera_info *p_device = layout_ipc_cmeara_is_doorcamera_get() == false ? network_data_get()->cctv_device : network_data_get()->door_device;

                        lv_obj_t *list = ipc_camera_registered_list_create();
                        int item_y = 0;
                        for (int i = 0; i < DEVICE_MAX; i++)
                        {
                                if (p_device[i].rtsp[0].rtsp_url[0] == 0)
                                {
                                        continue;
                                }

                                lv_obj_t *parent = lv_common_setting_btn_title_sub_info_img_create(list, i, 0, item_y, 928, 88,
                                                                                                   ipc_camera_registered_register_list_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                                                   0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                                                   0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                                                   58, 8, 838, 50, 0,
                                                                                                   p_device[i].door_name, 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                                   58, 45, 838, 50, 1,
                                                                                                   p_device[i].ipaddr, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                                                   0, 0, 0, 0, -1,
                                                                                                   NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                                   0, 20, 48, 48, 2,
                                                                                                   resource_ui_src_get("ic_error.png"), LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
                                lv_obj_t *sub = lv_obj_get_child_form_id(parent, 1);
                                if (sub != NULL)
                                {
                                        lv_label_set_long_mode(sub, LV_LABEL_LONG_DOT);
                                }

                                lv_common_img_btn_create(parent, 3, 808, 20, 48, 48,
                                                         ipc_camera_registered_register_edit_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,

                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_list_edit.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                                lv_common_img_btn_create(parent, 4, 880, 20, 48, 48,
                                                         ipc_camera_registered_register_del_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_list_delete.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                                item_y += 88;
                        }
                }
        }

        if (layout_ipc_cmeara_is_doorcamera_get() == true)
        {
                sat_ipcamera_initialization_parameters(network_data_get()->door_device, DEVICE_MAX);
        }
        else
        {
                sat_ipcamera_initialization_parameters(network_data_get()->cctv_device, DEVICE_MAX);
        }
        static bool result[8] = {0};
        memset(result, false, sizeof(result));
        lv_timer_t *timer = lv_sat_timer_create(layout_ipc_cameara_register_online_check_timer, 1000, &result);
        lv_timer_set_repeat_count(timer, 3);
}
static void sat_layout_quit(ipc_camera_register)
{
}

sat_layout_create(ipc_camera_register);