#include "layout_define.h"
#include "layout_setting_general.h"

/*进入模式设置:door camera /ipc*/

enum
{
  wifi_test_obj_id_title,
  wifi_test_obj_id_cancel,
  wifi_test_obj_id_next,

  wifi_test_obj_id_wifi_test_cont,

  wifi_test_obj_id_main_list,

  wifi_test_obj_id_setting_list,

  wifi_test_obj_id_channel_count,
  wifi_test_obj_id_channel_title,
  wifi_test_obj_id_channel_sub,

  wifi_test_obj_id_mode_cont,
  wifi_test_obj_id_mode_title,
  wifi_test_obj_id_mode_sub,

  wifi_test_obj_id_tx_power_cont,
  wifi_test_obj_id_tx_power_title,
  wifi_test_obj_id_tx_power_sub,

  wifi_test_obj_id_rate_b_cont,
  wifi_test_obj_id_rate_b_title,
  wifi_test_obj_id_rate_b_sub,

  wifi_test_obj_id_rate_g_cont,
  wifi_test_obj_id_rate_g_title,
  wifi_test_obj_id_rate_g_sub,

  wifi_test_obj_id_rate_n_cont,
  wifi_test_obj_id_rate_n_title,
  wifi_test_obj_id_rate_n_sub,

  wifi_test_obj_id_format_msgbox_cont,

  wifi_test_obj_id_format_msgbox,

};

static int channel = 0;
static int mode = 0;
static int tx_power = 0;
static int rate_b = 0;
static int rate_g = 0;
static int rate_n = 0;
static void layout_wifi_test_cancel_click(lv_event_t *ev)
{
  sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}
/************************************************************
** 函数说明: general setting列表获取
** 作者: xiaoxiao
** 日期: 2023-05-11 15:23:47
** 参数说明:
** 注意事项:
************************************************************/
static lv_obj_t *wifi_test_list_item_sub_get(int cont, int sub_id)
{
  lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_setting_list);
  if (list == NULL)
  {
    return NULL;
  }

  lv_obj_t *item = lv_obj_get_child_form_id(list, cont);
  if (item == NULL)
  {
    return NULL;
  }

  lv_obj_t *sub = lv_obj_get_child_form_id(item, sub_id);
  if (sub == NULL)
  {
    return NULL;
  }

  return sub;
}
static void layout_wifi_test_channel_display(void)
{
  lv_obj_t *sub = wifi_test_list_item_sub_get(wifi_test_obj_id_channel_count, wifi_test_obj_id_channel_title);
  if (sub == NULL)
  {
    return;
  }

  lv_label_set_text(sub, "Channel");
  sub = wifi_test_list_item_sub_get(wifi_test_obj_id_channel_count, wifi_test_obj_id_channel_sub);
  if (sub == NULL)
  {
    return;
  }
  char *channel_array[] = {
      "channel 1",
      "channel 2",
      "channel 3",
      "channel 4",
      "channel 5",
      "channel 6",
      "channel 7",
      "channel 8",
      "channel 9",
      "channel 10",
      "channel 11",
      "channel 12",
      "channel 13",
  };
  lv_label_set_text(sub, channel_array[channel]);
}
static void layout_wifi_test_mode_display(void)
{
  lv_obj_t *sub = wifi_test_list_item_sub_get(wifi_test_obj_id_mode_cont, wifi_test_obj_id_mode_title);
  if (sub == NULL)
  {
    return;
  }

  lv_label_set_text(sub, "Mode");
  sub = wifi_test_list_item_sub_get(wifi_test_obj_id_mode_cont, wifi_test_obj_id_mode_sub);
  if (sub == NULL)
  {
    return;
  }
  char *channel_array[] = {
      "Mode b",
      "Mode g",
      "Mode n",
  };
  lv_label_set_text(sub, channel_array[mode]);
}
static void layout_wifi_test_tx_power_display(void)
{
  lv_obj_t *sub = wifi_test_list_item_sub_get(wifi_test_obj_id_tx_power_cont, wifi_test_obj_id_tx_power_title);
  if (sub == NULL)
  {
    return;
  }

  lv_label_set_text(sub, "TX_Power");

  sub = wifi_test_list_item_sub_get(wifi_test_obj_id_tx_power_cont, wifi_test_obj_id_tx_power_sub);
  if (sub == NULL)
  {
    return;
  }
  char channel_array[64][8] = {0};
  for (int i = 0; i < 64; i++)
  {
    sprintf(channel_array[i], "%d", i);
  }
  lv_label_set_text(sub, channel_array[tx_power]);
}
static void layout_wifi_test_rate_b_display(void)
{
  lv_obj_t *sub = wifi_test_list_item_sub_get(wifi_test_obj_id_rate_b_cont, wifi_test_obj_id_rate_b_title);
  if (sub == NULL)
  {
    return;
  }

  lv_label_set_text(sub, "rate/b");

  sub = wifi_test_list_item_sub_get(wifi_test_obj_id_rate_b_cont, wifi_test_obj_id_rate_b_sub);
  if (sub == NULL)
  {
    return;
  }
  char *channel_array[] = {
      "1",
      "2",
      "5.5",
      "11",
  };
  lv_label_set_text(sub, channel_array[rate_b]);
}
static void layout_wifi_test_rate_g_display(void)
{
  lv_obj_t *sub = wifi_test_list_item_sub_get(wifi_test_obj_id_rate_g_cont, wifi_test_obj_id_rate_g_title);
  if (sub == NULL)
  {
    return;
  }

  lv_label_set_text(sub, "Rate/g");

  sub = wifi_test_list_item_sub_get(wifi_test_obj_id_rate_g_cont, wifi_test_obj_id_rate_g_sub);
  if (sub == NULL)
  {
    return;
  }
  char *channel_array[] = {
      "6",
      "9",
      "12",
      "18",
      "24",
      "36",
      "48",
      "54",

  };
  lv_label_set_text(sub, channel_array[rate_g]);
}
static void layout_wifi_test_rate_n_display(void)
{
  lv_obj_t *sub = wifi_test_list_item_sub_get(wifi_test_obj_id_rate_n_cont, wifi_test_obj_id_rate_n_title);
  if (sub == NULL)
  {
    return;
  }

  lv_label_set_text(sub, "Rate/n");

  sub = wifi_test_list_item_sub_get(wifi_test_obj_id_rate_n_cont, wifi_test_obj_id_rate_n_sub);
  if (sub == NULL)
  {
    return;
  }
  char *channel[] = {
      "msc 0",
      "msc 1",
      "msc 2",
      "msc 3",
      "msc 4",
      "msc 5",
      "msc 6",
      "msc 7",

  };
  lv_label_set_text(sub, channel[rate_n]);
}

static void layout_wifi_test_msgbox_del(lv_event_t *ev)
{
  setting_msgdialog_msg_del(wifi_test_obj_id_format_msgbox_cont);
}

static void wifi_test_msgbox_moethod_checkbox_click(lv_event_t *ev)
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

    int item_n = lv_obj_get_child_cnt(parent);
    for (int i = 0; i < item_n; i++)
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

static void layout_wifi_test_list_create(lv_obj_t *msgbox, int num, char *str[], lv_event_cb_t checkbox_cb, int index)
{
  lv_obj_t *list = setting_list_create(msgbox, 3);
  lv_common_style_set_common(list, 3, 27, 70, 460 - 27, 200, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
  for (int i = 0; i < num; i++)
  {
    lv_common_img_text_btn_create(list, i, 0, 0, 460 - 27, 55,
                                  wifi_test_msgbox_moethod_checkbox_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                  46, 8, 460 - 27 - 16, 32, 0,
                                  str[i], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                  0, 8, 32, 32, 1,
                                  i == index ? (const char *)resource_ui_src_get("btn_radio_s.png") : (const char *)resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
  }
}
static void layout_wifi_test_channel_change_confirm(lv_event_t *ev)
{
  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_format_msgbox_cont), wifi_test_obj_id_format_msgbox), 3);
  int item_n = lv_obj_get_child_cnt(list);
  for (int i = 0; i < item_n; i++)
  {
    lv_obj_t *n_item = lv_obj_get_child_form_id(list, i);

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(n_item, 1);
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
    {
      channel = i;
      break;
    }
  }
  sat_layout_goto(wifi_test, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_wifi_test_mode_change_confirm(lv_event_t *ev)
{
  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_format_msgbox_cont), wifi_test_obj_id_format_msgbox), 3);
  int item_n = lv_obj_get_child_cnt(list);
  for (int i = 0; i < item_n; i++)
  {
    lv_obj_t *n_item = lv_obj_get_child_form_id(list, i);

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(n_item, 1);
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
    {
      mode = i;

      break;
    }
  }
  sat_layout_goto(wifi_test, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_wifi_test_param_setting(void)
{
  char cmd[128] = {0};
  if (mode == 2)
  {
    system("/app/data/rtwpriv wlan0 mp_ctx stop");
    system("/app/data/rtwpriv wlan0 mp_start");
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_channel %d", channel + 1);
    system(cmd);
    system("/app/data/rtwpriv wlan0 mp_bandwidth 40M=1,shortGI=0");
    system("/app/data/rtwpriv wlan0 mp_ant_tx a");
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_rate  %d", rate_n + 128);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_txpower patha=%d", tx_power);
    system(cmd);
    system("/app/data/rtwpriv wlan0 mp_ctx background,pkt");
    system("/app/data/rtwpriv wlan0 mp_pwrctldm start");
  }
  else if (mode == 0)
  {
    system("/app/data/rtwpriv wlan0 mp_ctx stop");
    system("/app/data/rtwpriv wlan0 mp_start");
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_channel %d", channel + 1);
    system(cmd);
    system("/app/data/rtwpriv wlan0 mp_bandwidth 40M=1,shortGI=0");
    system("/app/data/rtwpriv wlan0 mp_ant_tx a");
    memset(cmd, 0, sizeof(cmd));
    char *rate_array[] = {
        "1",
        "2",
        "5.5",
        "11",
    };
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_rate  %d", (int)atof(rate_array[rate_b]) * 2);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_txpower patha=%d", tx_power);
    system(cmd);
    system("/app/data/rtwpriv wlan0 mp_ctx background,pkt");
    system("/app/data/rtwpriv wlan0 mp_pwrctldm start");
  }
  else if (mode == 1)
  {
    system("/app/data/rtwpriv wlan0 mp_ctx stop");
    system("/app/data/rtwpriv wlan0 mp_start");
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_channel %d", channel + 1);
    system(cmd);
    system("/app/data/rtwpriv wlan0 mp_bandwidth 40M=1,shortGI=0");
    system("/app/data/rtwpriv wlan0 mp_ant_tx a");
    memset(cmd, 0, sizeof(cmd));
    char *rate_array[] = {
        "6",
        "9",
        "12",
        "18",
        "24",
        "36",
        "48",
        "54",
    };
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_rate  %d", atoi(rate_array[rate_b]) * 2);
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "/app/data/rtwpriv wlan0 mp_txpower patha=%d", tx_power);
    system(cmd);
    system("/app/data/rtwpriv wlan0 mp_ctx background,pkt");
    system("/app/data/rtwpriv wlan0 mp_pwrctldm start");
  }
}

static void layout_wifi_test_tx_power_change_confirm(lv_event_t *ev)
{
  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_format_msgbox_cont), wifi_test_obj_id_format_msgbox), 3);
  int item_n = lv_obj_get_child_cnt(list);
  for (int i = 0; i < item_n; i++)
  {
    lv_obj_t *n_item = lv_obj_get_child_form_id(list, i);

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(n_item, 1);
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
    {
      tx_power = i;
      break;
    }
  }
  sat_layout_goto(wifi_test, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_wifi_test_rate_b_change_confirm(lv_event_t *ev)
{
  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_format_msgbox_cont), wifi_test_obj_id_format_msgbox), 3);
  int item_n = lv_obj_get_child_cnt(list);
  for (int i = 0; i < item_n; i++)
  {
    lv_obj_t *n_item = lv_obj_get_child_form_id(list, i);

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(n_item, 1);
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
    {
      rate_b = i;
      break;
    }
  }
  sat_layout_goto(wifi_test, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_wifi_test_rate_g_change_confirm(lv_event_t *ev)
{
  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_format_msgbox_cont), wifi_test_obj_id_format_msgbox), 3);
  int item_n = lv_obj_get_child_cnt(list);
  for (int i = 0; i < item_n; i++)
  {
    lv_obj_t *n_item = lv_obj_get_child_form_id(list, i);

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(n_item, 1);
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
    {
      rate_g = i;
      break;
    }
  }
  sat_layout_goto(wifi_test, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_wifi_test_rate_n_change_confirm(lv_event_t *ev)
{
  lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), wifi_test_obj_id_format_msgbox_cont), wifi_test_obj_id_format_msgbox), 3);
  int item_n = lv_obj_get_child_cnt(list);
  for (int i = 0; i < item_n; i++)
  {
    lv_obj_t *n_item = lv_obj_get_child_form_id(list, i);

    lv_obj_t *item_img_obj = lv_obj_get_child_form_id(n_item, 1);
    if (strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
    {
      rate_n = i;
      break;
    }
  }
  sat_layout_goto(wifi_test, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void layout_wifi_test_channel_change_btn_click(lv_event_t *ev)
{
  lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(wifi_test_obj_id_format_msgbox_cont, wifi_test_obj_id_format_msgbox, 282, 143, 460, 350);
  setting_msgdialog_msg_create(masgbox, 0, "Channel chnage", 0, 20, 460, 40, true);

  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_wifi_test_channel_change_confirm, layout_wifi_test_msgbox_del);
  char *channel_array[] = {
      "channel 1",
      "channel 2",
      "channel 3",
      "channel 4",
      "channel 5",
      "channel 6",
      "channel 7",
      "channel 8",
      "channel 9",
      "channel 10",
      "channel 11",
      "channel 12",
      "channel 13",
  };
  layout_wifi_test_list_create(masgbox, 13, channel_array, NULL, channel);
}

static void layout_wifi_test_mode_change_btn_click(lv_event_t *ev)
{
  lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(wifi_test_obj_id_format_msgbox_cont, wifi_test_obj_id_format_msgbox, 282, 143, 460, 350);
  setting_msgdialog_msg_create(masgbox, 0, "Mode chnage", 0, 20, 460, 40, true);

  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_wifi_test_mode_change_confirm, layout_wifi_test_msgbox_del);
  char *channel_array[] = {
      "Mode b",
      "Mode g",
      "Mode n",
  };
  layout_wifi_test_list_create(masgbox, sizeof(channel_array) / sizeof(char *), channel_array, NULL, mode);
}

static void layout_wifi_test_tx_power_change_btn_click(lv_event_t *ev)
{
  lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(wifi_test_obj_id_format_msgbox_cont, wifi_test_obj_id_format_msgbox, 282, 143, 460, 350);
  setting_msgdialog_msg_create(masgbox, 0, "TX_Power chnage", 0, 20, 460, 40, true);

  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_wifi_test_tx_power_change_confirm, layout_wifi_test_msgbox_del);

  char *channel_array[64] = {0};
  for (int i = 0; i < 64; i++)
  {
    channel_array[i] = (char *)malloc(8);
    sprintf(channel_array[i], "%d", i);
  }
  layout_wifi_test_list_create(masgbox, 64, channel_array, NULL, tx_power);
  for (int i = 0; i < 64; i++)
  {
    free(channel_array[i]);
  }
}

static void layout_wifi_test_rate_b_chnage_btn_click(lv_event_t *ev)
{
  lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(wifi_test_obj_id_format_msgbox_cont, wifi_test_obj_id_format_msgbox, 282, 143, 460, 350);
  setting_msgdialog_msg_create(masgbox, 0, "Rate/b chnage", 0, 20, 460, 40, true);

  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_wifi_test_rate_b_change_confirm, layout_wifi_test_msgbox_del);

  char *channel_array[] = {
      "1",
      "2",
      "5.5",
      "11",
  };
  layout_wifi_test_list_create(masgbox, sizeof(channel_array) / sizeof(char *), channel_array, NULL, rate_b);
}

static void layout_wifi_test_rate_g_chnage_btn_click(lv_event_t *ev)
{
  lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(wifi_test_obj_id_format_msgbox_cont, wifi_test_obj_id_format_msgbox, 282, 143, 460, 350);
  setting_msgdialog_msg_create(masgbox, 0, "Rate/g chnage", 0, 20, 460, 40, true);

  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_wifi_test_rate_g_change_confirm, layout_wifi_test_msgbox_del);
  char *channel_array[] = {
      "6",
      "9",
      "12",
      "18",
      "24",
      "36",
      "48",
      "54",

  };
  layout_wifi_test_list_create(masgbox, sizeof(channel_array) / sizeof(char *), channel_array, NULL, rate_g);
}

static void layout_wifi_test_rate_n_chnage_btn_click(lv_event_t *ev)
{
  lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(wifi_test_obj_id_format_msgbox_cont, wifi_test_obj_id_format_msgbox, 282, 143, 460, 350);
  setting_msgdialog_msg_create(masgbox, 0, "Rate/n chnage", 0, 20, 460, 40, true);
  setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_wifi_test_rate_n_change_confirm, layout_wifi_test_msgbox_del);
  char *channel[] = {
      "msc 0",
      "msc 1",
      "msc 2",
      "msc 3",
      "msc 4",
      "msc 5",
      "msc 6",
      "msc 7",

  };
  layout_wifi_test_list_create(masgbox, sizeof(channel) / sizeof(char *), channel, NULL, rate_n);
}

void layout_wifi_test_init(void)
{

  sat_kill_task_process("/config/wifi/wpa_supplicant -Dnl80");
  system("rmmod rtl8188ftv");
  system("insmod /app/data/rtl8188ftv.ko  rtw_mp_mode=1");
  system("ifconfig wlan0 down");
  system("ifconfig wlan0 up");
}

static void sat_layout_enter(wifi_test)
{
  standby_timer_close();
  layout_wifi_test_param_setting();
  /***********************************************
  ** 作者: leo.liu
  ** 日期: 2023-2-2 13:46:56
  ** 说明: 标题显示
  ***********************************************/
  {
    lv_common_text_create(sat_cur_layout_screen_get(), wifi_test_obj_id_title, 0, 20, 1024, 40,
                          NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                          "WIFI_TEST",
                          0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
  }
  /***********************************************
   ** 作者: leo.liu
   ** 日期: 2023-2-2 13:42:25
   ** 说明: 上一步下一步
   ***********************************************/
  {
    lv_common_img_btn_create(sat_cur_layout_screen_get(), wifi_test_obj_id_cancel, 35, 15, 48, 48,
                             layout_wifi_test_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                             resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
  }
  setting_list_info_t main_list_group[] = {

      {0, 0, 622, 72, wifi_test_obj_id_channel_count, wifi_test_obj_id_channel_title, wifi_test_obj_id_channel_sub, SETTING_GENERAL_XLS_LANG_ID_DATA_AND_TIME, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_AUTO_SETUP, lang_str_get, layout_wifi_test_channel_change_btn_click},
      {0, 72, 622, 72, wifi_test_obj_id_mode_cont, wifi_test_obj_id_mode_title, wifi_test_obj_id_mode_sub, SETTING_GENERAL_XLS_LANG_ID_LANG, lang_str_get, XLS_LANG_ID_LANGUAGE_TYPE, lang_str_get, layout_wifi_test_mode_change_btn_click},
      {0, 72 * 2, 622, 72, wifi_test_obj_id_tx_power_cont, wifi_test_obj_id_tx_power_title, wifi_test_obj_id_tx_power_sub, SETTING_GENERAL_XLS_LANG_ID_PASSWORD, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_CHANNGE_PASSWORD_FOR_SECURITY, lang_str_get, layout_wifi_test_tx_power_change_btn_click},
      {0, 72 * 3, 622, 72, wifi_test_obj_id_rate_b_cont, wifi_test_obj_id_rate_b_title, wifi_test_obj_id_rate_b_sub, SETTING_GENERAL_XLS_LANG_ID_STORAGE_SPACE, lang_str_get, -1, NULL, layout_wifi_test_rate_b_chnage_btn_click},
      {0, 72 * 4, 622, 72, wifi_test_obj_id_rate_g_cont, wifi_test_obj_id_rate_g_title, wifi_test_obj_id_rate_g_sub, SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENING_METHOD, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_OPEN_AFTER_CALL, lang_str_get, layout_wifi_test_rate_g_chnage_btn_click},
      {0, 72 * 5, 622, 72, wifi_test_obj_id_rate_n_cont, wifi_test_obj_id_rate_n_title, wifi_test_obj_id_rate_n_sub, SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENER_MODULE, lang_str_get, SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_LOCK, lang_str_get, layout_wifi_test_rate_n_chnage_btn_click},

  };

  lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), wifi_test_obj_id_setting_list);
  lv_common_style_set_common(list, wifi_test_obj_id_setting_list, 0, 88, 1024, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

  int j = 0;
  for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
  {

    lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[j].w, main_list_group[j].h,
                                                    main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                    0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                    20, 17, 576, 43, main_list_group[i].title_id,
                                                    main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                    20, 42, 576, 29, main_list_group[i].sub_id,
                                                    main_list_group[i].sub_language_id == -1 ? NULL
                                                                                             : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id),
                                                    0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                    0, 0, 0, 0, -1,
                                                    NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                    0, 0, 0, 0, -1,
                                                    NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
    j++;
    layout_wifi_test_channel_display();
    layout_wifi_test_mode_display();
    layout_wifi_test_tx_power_display();
    layout_wifi_test_rate_b_display();
    layout_wifi_test_rate_g_display();
    layout_wifi_test_rate_n_display();
  }
}
static void sat_layout_quit(wifi_test)
{
  standby_timer_restart(true, true);
}

sat_layout_create(wifi_test);