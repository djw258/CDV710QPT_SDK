#include "layout_define.h"
#include "layout_ipc_camera.h"
#include "common/sat_ipcamera.h"
#include "onvif.h"
/*进入模式设置:door camera /ipc*/

enum
{
  ipc_camera_registered_obj_id_title,
  ipc_camera_registered_obj_id_cancel,
  ipc_camera_registered_obj_id_next,

  ipc_camera_registered_obj_id_ipc_camera_registered_cont,

  ipc_camera_registered_obj_id_door_camera_list,

  ipc_camera_registered_obj_id_registered_door_camera_title,

  ipc_camera_registered_obj_id_keyboard,

  ipc_camera_registered_obj_id_msgbox,

  ipc_camera_registered_obj_id_textarea,

  ipc_camera_search_obj_id_msg_bg,

  ipc_camera_search_msg_obj_id_msg,

};

static void ipc_camera_registered_cancel_click(lv_event_t *ev)
{

  sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static lv_obj_t *ipc_camera_registered_list_create(void)
{
  lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_door_camera_list);
  if (list != NULL)
  {
    return list;
  }
  list = lv_list_create(sat_cur_layout_screen_get());
  lv_common_style_set_common(list, ipc_camera_registered_obj_id_door_camera_list, 48, 96, 928, 176, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
  return list;
}

static void outdoor_mac_register_confirm(lv_event_t *ev)
{
  setting_msgdialog_msg_del(ipc_camera_search_obj_id_msg_bg);
}
static void outdoor_mac_register_concel(lv_event_t *ev)
{
  setting_msgdialog_msg_del(ipc_camera_search_obj_id_msg_bg);
}

static void outdoor_mac_register_check(bool result)
{
  lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_search_obj_id_msg_bg);
  if (masgbox != NULL)
  {
    setting_msgdialog_msg_del(ipc_camera_search_obj_id_msg_bg);
  }
  masgbox = setting_msgdialog_msg_bg_create(ipc_camera_search_obj_id_msg_bg, ipc_camera_search_msg_obj_id_msg, 282, 123, 460, 323);
  setting_msgdialog_msg_create(masgbox, 0, result ? "Outdoor mac register success" : "Outdoor mac register failed", 20, 100, 420, 180, false);
  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, outdoor_mac_register_confirm, outdoor_mac_register_concel);
}

static int input_index = 0;
static void layout_outdoor_mac_register_ok_btn_up(lv_obj_t *obj)
{
  static char input_buffer[6] = {0};
  obj = lv_obj_get_child_form_id(lv_scr_act(), ipc_camera_registered_obj_id_textarea);
  int pos = lv_textarea_get_cursor_pos(obj);
  if (pos == 0)
  {
    return;
  }
  if ((input_index) == 0)
  {
    memset(input_buffer, 0, sizeof(input_buffer));
    strcpy(input_buffer, lv_textarea_get_text(obj));
    lv_textarea_set_text(obj, "");
    lv_textarea_set_cursor_pos(obj, 0);
    (input_index)++;
  }
  else if ((input_index) == 1)
  {
    if (strcmp(input_buffer, lv_textarea_get_text(obj)) == 0)
    {
      int index;
      char mac[18] = {0};
      sscanf(input_buffer, "%d", &index);
      outdoor_mac_address_get(index, mac);
      int ret = sat_ipcamera_device_mac_update(network_data_get()->door_device[0].ipaddr, network_data_get()->door_device[0].port, network_data_get()->door_device[0].username, network_data_get()->door_device[0].password, network_data_get()->door_device[0].auther_flag, mac, 1000);
      outdoor_mac_register_check(ret);
    }
    else
    {
      lv_textarea_set_text(obj, "");
      lv_textarea_set_cursor_pos(obj, 0);
    }
    (input_index) = 0;
  }
}

static void outdoor_mac_register_num_keyboard_event(lv_event_t *e)
{

  lv_obj_t *obj = lv_event_get_target(e);
  uint32_t id = lv_btnmatrix_get_selected_btn(obj);

  lv_obj_t *ta = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_textarea);

  if (id == 11)
  {
    lv_textarea_del_char(ta);
  }
  else if (id == 9)
  {
    layout_outdoor_mac_register_ok_btn_up(obj);
  }
  else
  {
    const char *text = lv_btnmatrix_get_btn_text(obj, id);

    if (text != NULL)
    {

      lv_textarea_add_text(ta, text);
    }
  }
}

static void layout_outdoor_mac_register_online_check_timer(lv_timer_t *timer)
{
  struct ipcamera_info *ipc_device = layout_ipc_cmeara_is_doorcamera_get() == false ? network_data_get()->cctv_device : network_data_get()->door_device;
  // bool *result = (bool *)timer->user_data;
  bool result[8] = {0};
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
    // if (result[i] == true)
    // {
    //   lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_detect.png"), LV_PART_MAIN);
    //   continue;
    // }
    char name[64] = {0};
    result[i] = ipc_camera_device_name_get(name, ipc_device[i].ipaddr, ipc_device[i].port, ipc_device[i].username, ipc_device[i].password, ipc_device[i].auther_flag, 1000);

    lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(result[i] ? "ic_detect.png" : "ic_error.png"), LV_PART_MAIN);
  }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: SD卡状态变化处理
 ***********************************************/
static void layout_outdoor_mac_register_sd_state_change_callback(void)
{

  sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void sat_layout_enter(outdoor_mac_register)
{
  standby_timer_close();

  /*sd卡状态处理 */
  sd_state_channge_callback_register(layout_outdoor_mac_register_sd_state_change_callback);

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
                          "Outdoor mac register",
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
  }

  {
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
                                                                         NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
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

      item_y += 88;
    }
    // static bool result[8] = {0};
    // memset(result, false, sizeof(result));
    lv_timer_ready(lv_sat_timer_create(layout_outdoor_mac_register_online_check_timer, 3000, NULL /*result*/));
  }

  {
    lv_obj_t *ta = lv_common_textarea_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_textarea, 410, 210, 204, 54,
                                             NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                             LV_OPA_COVER, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                             0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                             0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                             "", 0Xffffff, 0Xffffff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 8,
                                             0, 500, 0Xffffff);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);
    lv_obj_set_style_anim_time(ta, 500, LV_STATE_FOCUSED | LV_PART_CURSOR);
  }
  /************************************************************
  ** 函数说明: 序号键盘输入创建
  ** 作者: xiaoxiao
  ** 日期: 2023-05-12 10:04:53
  ** 参数说明:
  ** 注意事项:
  ************************************************************/
  {
    lv_obj_t *kb = lv_common_number_input_keyboard_create(sat_cur_layout_screen_get(), ipc_camera_registered_obj_id_keyboard, 402, 309, 220, 281,
                                                          outdoor_mac_register_num_keyboard_event, LV_OPA_COVER, 0x808080, LV_OPA_COVER, 0x00a8ff,
                                                          360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                          360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                          0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                          13, 22);
    static const char *btnm_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        "OK", "0", " ", ""};

    lv_btnmatrix_set_map(kb, btnm_map);
  }
}
static void sat_layout_quit(outdoor_mac_register)
{
  sd_state_channge_callback_register(sd_state_change_default_callback);
  standby_timer_restart(true, true);
}

sat_layout_create(outdoor_mac_register);