#include "layout_define.h"
#include "layout_ipc_camera.h"

enum
{
        ipc_camera_search_display_obj_id_top_cont,
        ipc_camera_search_display_obj_id_title,

        ipc_camera_search_display_obj_id_cancel,
        ipc_camera_search_display_obj_id_ip_edit,
        ipc_camera_search_display_obj_id_if_you,

        ipc_camera_search_display_obj_id_register_label,
};

static void ipc_camera_search_display_cancel_click(lv_event_t *ev)
{
        if (layout_ipc_camera_input_flag_get() & IPC_CAMERA_FLAG_REGISTER)
        {
                sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        else
        {
                sat_layout_goto(ipc_camera_search, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
}
static void ipc_camera_search_display_ip_edit_click(lv_event_t *ev)
{
}

static void ipc_camera_search_display_register_click(lv_event_t *ev)
{
        if (network_data_get()->door_device_count > 7)
        {
                return;
        }

        int total = network_data_get()->door_device_count;

        /**
         * 判断 预览的SIP是否已经注册过了
         *  如果sip账号完全相同，说明该账号已经被注册过了。
         **/
        for (int i = 0; i < total; i++)
        {
                /** 判断 预览的SIP是否已经注册过了**/
                if (layout_ipc_cmeara_is_doorcamera_get() == true)
                {
                        const char *door_sip_uri = sat_ipcamera_sip_addr_get(layout_ipc_camera_edit_index_get());
                        if (strcmp(network_data_get()->door_device[i].sip_url, door_sip_uri) == 0)
                        {
                                SAT_DEBUG("strcmp(network_data_get()->door_device[i].user, door_sip_uri) == 0");
                                return;
                        }
                }
                else
                {
                        const char *door_sip_uri = sat_ipcamera_rtsp_addr_get(layout_ipc_camera_edit_index_get(), 0);
                        if (strcmp(network_data_get()->cctv_device[i].rtsp[0].rtsp_url, door_sip_uri) == 0)
                        {
                                SAT_DEBUG("strcmp(network_data_get()->cctv_device[i].user, door_sip_uri) == 0");
                                return;
                        }
                }
        }

        char local_sip_uri[128] = {0};
        sat_sip_local_user_get(local_sip_uri);
        if ((layout_ipc_cmeara_is_doorcamera_get() == false) || (sat_ipcamera_device_register(local_sip_uri, layout_ipc_camera_edit_index_get(), 300) == true))
        {
                if (layout_ipc_cmeara_is_doorcamera_get() == true)
                {
                        memcpy(&network_data_get()->door_device[network_data_get()->door_device_count], sat_ipcamera_node_data_get(layout_ipc_camera_edit_index_get()), sizeof(struct ipcamera_info));
                        network_data_get()->door_device_count++;
                }
                else
                {
                        memcpy(&network_data_get()->cctv_device[network_data_get()->cctv_device_count], sat_ipcamera_node_data_get(layout_ipc_camera_edit_index_get()), sizeof(struct ipcamera_info));
                        network_data_get()->cctv_device_count++;
                }
                network_data_save();
                sat_layout_goto(ipc_camera_register, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
}

static void ipc_camera_display_ipcamera_state_func(unsigned int type, unsigned int num)
{
        char buffer[128] = {0};
        if (type == 2)
        {
                const char *rtsp = sat_ipcamera_rtsp_addr_get(layout_ipc_camera_edit_index_get(), 0);
                if ((rtsp != NULL) && (strstr(rtsp, "rtsp://")))
                {
                        sprintf(buffer, "%s %s %s", rtsp, sat_ipcamera_username_get(layout_ipc_camera_edit_index_get()), sat_ipcamera_password_get(layout_ipc_camera_edit_index_get()));
                        SAT_DEBUG("%s", buffer);
                        sat_linphone_ipcamera_start(buffer);
                        lv_common_video_mode_enable(true);
                }
        }
}
static void sat_layout_enter(ipc_camera_display)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 顶部半透明显示
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), ipc_camera_search_display_obj_id_top_cont, 0, 0, 1024, layout_ipc_cmeara_is_doorcamera_get() == true ? 160 : 80,
                                                            NULL, false, LV_OPA_50, 0, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                {
                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:46:56
                         ** 说明: 标题显示
                         ***********************************************/
                        {
                                char buffer[32] = {0};
                                sprintf(buffer, "%s", sat_ipcamera_door_name_get(layout_ipc_camera_edit_index_get()));
                                lv_common_text_create(parent, ipc_camera_search_display_obj_id_title, 0, 20, 1024, 40,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      (char *)buffer, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                        }

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:42:25
                         ** 说明: 上一步下一步
                         ***********************************************/
                        {
                                lv_common_img_btn_create(parent, ipc_camera_search_display_obj_id_cancel, 35, 15, 48, 48,
                                                         ipc_camera_search_display_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        }

                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:42:25
                         ** 说明: ip
                         ***********************************************/
                        if (layout_ipc_cmeara_is_doorcamera_get() == true)
                        {
                                {
                                        lv_common_img_btn_create(parent, ipc_camera_search_display_obj_id_ip_edit, 952, 15, 48, 48,
                                                                 ipc_camera_search_display_ip_edit_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 resource_ui_src_get("btn_title_IPset.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                                }

                                /***********************************************
                                 ** 作者: leo.liu
                                 ** 日期: 2023-2-2 13:46:56
                                 ** 说明: 标题显示
                                 ***********************************************/
                                {
                                        lv_common_text_create(parent, ipc_camera_search_display_obj_id_if_you, 16, 93, 823, 54,
                                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              layout_ipc_camera_search_language_get(DOOR_CAMERA_SEARCH_ID_IF_YOUT_CANNOT_SEE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_small);
                                }
                        }
                }
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 标题显示
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), ipc_camera_search_display_obj_id_register_label, 0, 528, 1024, 72,
                                                      ipc_camera_search_display_register_click, LV_OPA_COVER, 0X00A8FF, LV_OPA_COVER, 0X00A8FF,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      layout_ipc_camera_search_language_get(DOOR_CAMERA_SEARCH_ID_REGISTRATION), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                lv_obj_set_style_pad_top(obj, 15, LV_PART_MAIN);
        }

        sat_ipcamera_rtsp_url_get(layout_ipc_camera_edit_index_get());

        ipcamera_state_callback_register(ipc_camera_display_ipcamera_state_func);
}
static void sat_layout_quit(ipc_camera_display)
{
        lv_common_video_mode_enable(false);
        sat_linphone_ipcamera_stop();
        ipcamera_state_callback_register(NULL);
}

sat_layout_create(ipc_camera_display);