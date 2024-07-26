#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
  setting_open_duration_time_obj_id_title,
  setting_open_duration_time_obj_id_cancel,
  setting_open_duration_time_obj_id_doorcamera1,
  setting_open_duration_time_obj_id_doorcamera18 = setting_open_duration_time_obj_id_doorcamera1 + 7,

  setting_open_duration_time_obj_id_msgbox_bg_cont,
};

typedef enum
{
  setting_open_duration_time_obj_id_msgbox_cont,
  setting_open_duration_time_obj_id_msgbox_title,
  setting_open_duration_time_obj_id_msgbox_check_1,

  setting_open_duration_time_obj_id_msgbox_cancel = setting_open_duration_time_obj_id_msgbox_check_1 + 7,
  setting_open_duration_time_obj_id_titile_id,
  setting_open_duration_time_obj_id_msgbox_confirm,

  setting_open_duration_time_msgbox_list,

} setting_open_duration_time_msgbox_bg_cont_obj_id;

typedef enum
{
  setting_open_duration_time_obj_id_msgbox_check_text,
  setting_open_duration_time_obj_id_msgbox_check_img,
} setting_open_duration_time_obj_id_msgbox_check_obj_id;

int select_index = 0;

static void setting_open_duration_time_cancel_obj_click(lv_event_t *e)
{
  sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

/************************************************************
** 函数说明: 开锁时长初始值显示
** 作者: xiaoxiao
** 日期：2024-05-30 18:47:24
** 参数说明:
** 注意事项：
************************************************************/
static void layout_setting_open_duration_time_display_time(void)
{
  for (int i = 0; i < DEVICE_MAX; i++)
  {
    lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_open_duration_time_obj_id_doorcamera1 + i);
    if (cont != NULL)
    {
      lv_obj_t *sub = lv_obj_get_child_form_id(cont, 1);

      lv_label_set_text_fmt(sub, lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_1_DOT_5_SEC + user_data_get()->etc.open_duration_time[i]));
    }
  }
}
/************************************************************
** 函数说明: 开锁时长选项创建
** 作者: xiaoxiao
** 日期：2024-05-30 18:47:24
** 参数说明:
** 注意事项：
************************************************************/
static lv_obj_t *layout_setting_open_duration_time_msgbox_creat(const char *title, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb, const char *item[7])
{
  lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_open_duration_time_obj_id_msgbox_bg_cont, 0, 0, 1024, 600,
                                              NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

  lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_open_duration_time_obj_id_msgbox_cont, 282, 131, 460, 343,
                                              NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

  lv_common_text_create(msgbox, setting_open_duration_time_obj_id_msgbox_title, 32, 10, 396, 47,
                        NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                        0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                        0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                        title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

  lv_obj_t *list = setting_list_create(msgbox, setting_open_duration_time_msgbox_list);
  lv_common_style_set_common(list, setting_open_duration_time_msgbox_list, 27, 73, 460 - 27, 200, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
  for (int i = 0; i < 7; i++)
  {
    lv_common_img_text_btn_create(list, setting_open_duration_time_obj_id_msgbox_check_1 + i, 0, 122, 365, 48,
                                  checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  48, 8, 365 - 94, 32, setting_open_duration_time_obj_id_msgbox_check_text,
                                  item[i], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, setting_open_duration_time_obj_id_msgbox_check_img,
                                  "", LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
  }

  lv_common_img_btn_create(msgbox, setting_open_duration_time_obj_id_msgbox_cancel, 0, 281, 230, 62,
                           cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                           0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                           0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                           resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

  lv_common_img_btn_create(msgbox, setting_open_duration_time_obj_id_msgbox_confirm, 230, 281, 230, 62,
                           confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                           0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                           0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                           resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);
  return msgbox;
}
/************************************************************
** 函数说明: 开锁时长保存
** 作者: xiaoxiao
** 日期：2024-05-30 18:46:45
** 参数说明:
** 注意事项：
************************************************************/
static void setting_open_duration_time_msgbox_click(lv_event_t *ev)
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
    for (int i = 0; i < 7; i++)
    {
      lv_obj_t *n_item = lv_obj_get_child_form_id(parent, setting_open_duration_time_obj_id_msgbox_check_1 + i);
      if ((n_item == NULL) || (n_item == item))
      {
        continue;
      }
      item_img_obj = lv_obj_get_child_form_id(n_item, 1);

      lv_obj_set_style_bg_img_src(item_img_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
    }
  }
}
/************************************************************
** 函数说明: 取消选择
** 作者: xiaoxiao
** 日期：2024-05-30 18:46:45
** 参数说明:
** 注意事项：
************************************************************/
static void setting_open_duration_time_msgbox_cancel_click(lv_event_t *e)
{
  setting_msgdialog_msg_del(setting_open_duration_time_obj_id_msgbox_bg_cont);
}

/************************************************************
** 函数说明: 开锁时长选项显示
** 作者: xiaoxiao
** 日期：2024-05-30 18:46:26
** 参数说明:
** 注意事项：
************************************************************/
static void setting_open_duration_times_option_display(void)
{
  lv_obj_t *parent = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_open_duration_time_obj_id_msgbox_bg_cont), setting_open_duration_time_obj_id_msgbox_cont), setting_open_duration_time_msgbox_list);

  lv_obj_t *check = NULL;
  for (int i = 0; i < 7; i++)
  {
    check = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_open_duration_time_obj_id_msgbox_check_1 + i), setting_open_duration_time_obj_id_msgbox_check_img);
    lv_obj_set_style_bg_img_src(check, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
  }

  check = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_open_duration_time_obj_id_msgbox_check_1 + (user_data_get()->etc.open_duration_time[select_index])), setting_open_duration_time_obj_id_msgbox_check_img);
  lv_obj_set_style_bg_img_src(check, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
}

/************************************************************
** 函数说明: 开锁时长设置保存
** 作者: xiaoxiao
** 日期: 2023-04-29 09:31:32
** 参数说明:
** 注意事项:
************************************************************/
static void setting_open_duration_time_msgbox_confirm_click(lv_event_t *ev)
{

  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_parent(lv_event_get_current_target(ev)), setting_open_duration_time_msgbox_list);

  for (int i = 0; i < 7; i++)
  {

    lv_obj_t *checkbox = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, i + setting_open_duration_time_obj_id_msgbox_check_1), setting_open_duration_time_obj_id_msgbox_check_img);
    if (!strncmp((const char *)checkbox->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
    {
      user_data_get()->etc.open_duration_time[select_index] = i;
      user_data_save(true, true);
      if ((user_data_get()->system_mode & 0x0f) != 0x01)
      {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
      }
      break;
    }
  }
  user_data_save(true, true);
  setting_msgdialog_msg_del(setting_open_duration_time_obj_id_msgbox_bg_cont);
  layout_setting_open_duration_time_display_time();
}

/************************************************************
** 函数说明: 选项点击
** 作者: xiaoxiao
** 日期：2024-05-30 18:46:04
** 参数说明:
** 注意事项：
************************************************************/
static void layout_setting_open_duration_time_click(lv_event_t *e)
{
  if (user_data_get()->alarm.security_alarm_enable || user_data_get()->alarm.away_alarm_enable)
  {
    lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(setting_open_duration_time_obj_id_msgbox_bg_cont, setting_open_duration_time_obj_id_msgbox_cont, 282, 143, 460, 283);
    setting_msgdialog_msg_create(masgbox, setting_open_duration_time_obj_id_titile_id, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_RUNNNG_TIPS), 0, 90, 460, 80, false);
    setting_msgdialog_msg_confirm_btn_create(masgbox, setting_open_duration_time_obj_id_msgbox_confirm, setting_open_duration_time_msgbox_cancel_click);
    return;
  }
  lv_obj_t *obj = lv_event_get_current_target(e);
  for (int i = 0; i < 8; i++)
  {
    if (obj->id == setting_open_duration_time_obj_id_doorcamera1 + i)
    {
      select_index = i;
    }
  }
  const char *item[7] = {0};
  item[0] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_1_DOT_5_SEC);
  item[1] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_3_SEC);
  item[2] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_5_SEC);
  item[3] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_7_SEC);
  item[4] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_10_SEC);
  item[5] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_15_SEC);
  item[6] = lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME_20_SEC);

  layout_setting_open_duration_time_msgbox_creat(lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DURATION_TIME),
                                                 setting_open_duration_time_msgbox_cancel_click, setting_open_duration_time_msgbox_confirm_click, setting_open_duration_time_msgbox_click,
                                                 item);
  setting_open_duration_times_option_display();
}

static void sat_layout_enter(setting_open_duration_time)
{
  /***********************************************
  ** 作者: leo.liu
  ** 日期: 2023-2-3 14:13:25
  ** 说明: 标题创建
  ***********************************************/
  {
    lv_common_text_create(sat_cur_layout_screen_get(), setting_open_duration_time_obj_id_title, 0, 20, 1024, 40,
                          NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                          lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_DURATION_TIME), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
  }
  /***********************************************
   ** 作者: leo.liu
   ** 日期: 2023-2-3 14:13:25
   ** 说明: 返回
   ***********************************************/
  {
    lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_open_duration_time_obj_id_cancel, 35, 15, 48, 48,
                             setting_open_duration_time_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
  }
  /************************************************************
  ** 函数说明: 显示已注册的门的开锁时间
  ** 作者: xiaoxiao
  ** 日期：2024-05-30 17:58:49
  ** 参数说明:
  ** 注意事项：
  ************************************************************/
  {
    int offset = 0;
    for (int i = 0; i < 8; i++)
    {
      if (monitor_valid_channel_check(i))
      {

        lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), setting_open_duration_time_obj_id_doorcamera1 + i, 48, 88 * (offset + 1), 928, 88,
                                                        layout_setting_open_duration_time_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                        0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                        0, 30, 576, 43, 0, network_data_get()->door_device[i].door_name, 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                        0, 55, 576, 29, 1, "1.5 s", 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                        0, 0, 0, 0, -1,
                                                        NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                        840, 20, 80, 48, 2, NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
      }
      offset++;
    }
    layout_setting_open_duration_time_display_time();
  }
}

static void
sat_layout_quit(setting_open_duration_time)
{
}

sat_layout_create(setting_open_duration_time);