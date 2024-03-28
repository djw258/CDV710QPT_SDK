#include "layout_define.h"
typedef enum
{
  always_record_scr_act_obj_id_head_cont,
  always_record_scr_act_obj_id_loop_obj,
  always_record_scr_act_obj_id_record_obj,
} always_record_scr_act_obj_id;
typedef enum
{
  always_record_head_cont_obj_id_cancel_obj,
  always_record_head_cont_obj_id_channel_label,
  always_record_head_cont_obj_id_time_label,
  always_record_head_cont_obj_id_motion_img,
  always_record_head_cont_obj_id_auto_img,
  always_record_head_cont_obj_id_sd_img,
  always_record_head_cont_obj_id_timeout_label
} always_record_head_cont_obj_id;
static void monitor_obj_timeout_timer(lv_timer_t *ptimer);

/************************************************************
** 函数说明: 多通道循环录像还是单通道
** 作者: xiaoxiao
** 日期: 2023-05-26 08:53:53
** 参数说明:
** 注意事项:
************************************************************/
static bool always_record_loop = true;
static bool is_always_record_video_ing = false;
static bool is_always_snapshot_ing = false;

static int always_record_timeout_sec = 10; // 记录时长
void always_record_time_set(int sec)
{
  always_record_timeout_sec = sec;
}

/************************************************************
** 函数说明: 获取通道
** 作者: xiaoxiao
** 日期: 2023-05-26 08:00:46
** 参数说明:
** 注意事项:
************************************************************/
static int always_record_channel_get(void)
{
  if (monitor_door_first_valid_get(true) < 0 && monitor_door_first_valid_get(false) < 0)
  {
    return MON_CH_NONE;
  }

  int ch = monitor_channel_get();

  if ((always_record_loop == false) && (ch != MON_CH_NONE))
  {
    return ch;
  }
  if (ch == MON_CH_NONE)
  {
    ch = MON_CH_DOOR1;
    if (monitor_valid_channel_check(ch) == true)
    {
      return ch;
    }
  }

  for (int i = 0; i < 7; i++)
  {

    if ((ch == MON_CH_DOOR1 + i) && (ch != MON_CH_DOOR8))
    {
      ch = MON_CH_DOOR2 + i;
      if (monitor_valid_channel_check(ch) == true)
      {
        return ch;
      }
    }
  }
  if (ch == MON_CH_DOOR8)
  {
    ch = MON_CH_CCTV1;
    if (monitor_valid_channel_check(ch) == true)
    {
      return ch;
    }
  }
  for (int i = 0; i < 8; i++)
  {
    if ((ch == MON_CH_CCTV1 + i) && (ch != MON_CH_CCTV8))
    {
      ch = MON_CH_CCTV2 + i;
      if (monitor_valid_channel_check(ch) == true)
      {
        return ch;
      }
    }
  }
  if (ch == MON_CH_CCTV8)
  {
    ch = MON_CH_DOOR1;
    if (monitor_valid_channel_check(ch) == true)
    {
      return ch;
    }
  }

  return MON_CH_NONE;
}

/************************************************************
** 函数说明: 录像点击事件
** 作者: xiaoxiao
** 日期: 2023-05-26 09:39:18
** 参数说明:
** 注意事项:
************************************************************/
static void always_record_record_btn_up(lv_event_t *ev)
{
  //     if (is_always_record_video_ing == true)
  //     {
  //         record_video_stop();
  //     }
  //     else if((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
  //     {
  //         record_video_start(true, REC_MODE_MANUAL);
  //     }
}

static void layout_always_monitor_open(void)
{
  int ch = always_record_channel_get();

  if (ch != MON_CH_NONE)
  {
    monitor_channel_set(ch);
    monitor_open(true, true);
  }
}
static void always_record_record_btn_display(void)
{
  lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), (always_record_scr_act_obj_id_record_obj));
  if (obj == NULL)
  {
    obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), always_record_scr_act_obj_id_record_obj, 912, 486, 80, 81,
                                   always_record_record_btn_up, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                   resource_ui_src_get("btn_call_rec.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
  }
  if (obj == NULL)
  {
    return;
  }
  if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
  {
    if (is_always_record_video_ing == true)
    {
      lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_rec_ing.png"), LV_PART_MAIN);
    }
    else
    {
      lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_rec.png"), LV_PART_MAIN);
    }
  }
  else
  {
    if (is_always_snapshot_ing == true)
    {
      lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_save.png"), LV_PART_MAIN);
    }
    else
    {
      lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_camera.png"), LV_PART_MAIN);
    }
  }
}

static void layout_always_record_stop(void)
{
  if (is_always_record_video_ing)
  {
    record_video_stop();
  }
  always_record_record_btn_display();
}

static void layout_always_record_start(void)
{

  if (is_always_record_video_ing == true)
  {
    record_video_stop();
  }
  else if ((media_sdcard_insert_check() == SD_STATE_UNPLUG) || (media_sdcard_insert_check() == SD_STATE_ERROR))
  {
    record_jpeg_start(REC_MODE_ALWAYS);
  }
  else
  {
    record_video_start(true, REC_MODE_ALWAYS);
  }
  always_record_record_btn_display();
}

/***
**   日期:2022-06-20 16:38:19
**   作者: leo.liu
**   函数作用：创建任务等待录像
**   参数说明:
***/
static void layout_always_record_delay_task(lv_timer_t *pthime)
{

  layout_always_record_start();
  lv_timer_del(pthime);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 获取顶部容器的子控件
 ***********************************************/
static lv_obj_t *monitor_top_child_obj_get(int id)
{
  lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), always_record_scr_act_obj_id_head_cont);
  if (parent == NULL)
  {
    return NULL;
  }
  return lv_obj_get_child_form_id(parent, id);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 顶部时间通道显示
 ***********************************************/
static void monitior_obj_channel_info_obj_display(void)
{
  lv_obj_t *obj = monitor_top_child_obj_get(1);
  if (obj == NULL)
  {
    return;
  }
  struct tm tm;
  user_time_read(&tm);
  int channel = monitor_channel_get();
  if (is_channel_ipc_camera(channel) == true)
  {
    lv_obj_set_x(obj, 96);
    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    channel -= 8;
    lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->cctv_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
  }
  else
  {
    lv_obj_set_x(obj, 96);
    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->door_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
  }
}
static void layout_always_monitor_open_task(lv_timer_t *task)
{
  /* if (always_record_loop == true) */
  {
    layout_always_monitor_open();
    lv_sat_timer_create(monitor_obj_timeout_timer, 1000, NULL);
    monitior_obj_channel_info_obj_display();
  }
  lv_timer_del(task);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 记录状态状态显示
 ***********************************************/
static void always_record_video_state_callback(bool record_ing)
{
  is_always_record_video_ing = record_ing;
  always_record_record_btn_display();
}

/************************************************************
** 函数说明: 抓拍回调
** 作者: xiaoxiao
** 日期: 2023-06-01 16:24:33
** 参数说明:
** 注意事项:
************************************************************/
static void always_record_snapshot_state_callback(bool record_ing)
{
  is_always_snapshot_ing = record_ing;

  always_record_record_btn_display();
}

static void monitor_obj_cctv_cancel_obj_click(lv_event_t *e)
{
  sat_layout_goto(setting_recording, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 顶部图标显示
 ***********************************************/
static void montior_obj_top_icon_display(void)
{

  lv_obj_t *obj = NULL;
  int pos_x = 863;
  /***********************************************
   ** 作者: leo.liu
   ** 日期: 2023-2-2 13:42:25
   ** 说明: SD卡显示
   ***********************************************/
  {
    obj = monitor_top_child_obj_get(5);
    if (obj == NULL)
    {
      return;
    }
    if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
    {
      lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(media_sdcard_insert_check() == SD_STATE_INSERT ? "ic_monitoring_sdcard.png" : "ic_monitoring_sdcard_full.png"), LV_PART_MAIN);
      lv_obj_set_x(obj, pos_x);
      lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
      pos_x -= 56;
    }
    else
    {
      lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
  }

  /***********************************************
   ** 作者: leo.liu
   ** 日期: 2023-2-2 13:42:25
   ** 说明: 自动记录
   ***********************************************/
  {
    obj = monitor_top_child_obj_get(4);
    if (obj == NULL)
    {
      return;
    }
    lv_obj_set_x(obj, pos_x);
    pos_x -= 56;
    lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->auto_record_mode == 0 ? "ic_monitoring_manual.png" : "ic_monitoring_auto.png"), LV_PART_MAIN);
  }

  /***********************************************
   ** 作者: leo.liu
   ** 日期: 2023-2-2 13:42:25
   ** 说明: 移动侦测
   ***********************************************/
  {
    obj = monitor_top_child_obj_get(3);
    if (obj == NULL)
    {
      return;
    }
    if (user_data_get()->motion.enable)
    {
      lv_obj_set_x(obj, pos_x);
      pos_x -= 56;
      lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(user_data_get()->motion.saving_fmt == 1 ? "ic_list_visitor_img.png" : "ic_list_visitor_video.png"), LV_PART_MAIN);
    }
    else
    {
      lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
  }
}
/************************************************************
** 函数说明: SD卡状态变化及处理
** 作者: xiaoxiao
** 日期: 2023-06-01 14:44:00
** 参数说明:
** 注意事项:
************************************************************/
static void always_record_sd_state_change_callback(void)
{
  montior_obj_top_icon_display();
  layout_always_record_stop();
}

static void always_record_loop_btn_display(void);
static void always_record_loop_btn_up(lv_event_t *ev)
{
  always_record_loop = always_record_loop ? false : true;
  always_record_loop_btn_display();
}
/************************************************************
** 函数说明: 循环还是单通道
** 作者: xiaoxiao
** 日期: 2023-06-01 17:02:18
** 参数说明:
** 注意事项:
************************************************************/
static void always_record_loop_btn_display(void)
{
  lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), always_record_scr_act_obj_id_loop_obj);
  if (obj == NULL)
  {
    always_record_loop = true;
    obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), always_record_scr_act_obj_id_loop_obj, 808, 486, 80, 81,
                                   always_record_loop_btn_up, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                   resource_ui_src_get("btn_monitoring_fixed.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
  }

  lv_obj_set_style_bg_img_src(obj, always_record_loop ? resource_ui_src_get("btn_monitoring_repeat.png") : resource_ui_src_get("btn_monitoring_fixed.png"), LV_PART_MAIN);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 监控时间文本显示
 ***********************************************/
static void monitor_obj_timeout_label_display(void)
{
  lv_obj_t *obj = monitor_top_child_obj_get(6);
  if (obj == NULL)
  {
    return;
  }
  lv_label_set_text_fmt(obj, "%02d:%02d", always_record_timeout_sec / 60, always_record_timeout_sec % 60);
}
static void monitor_obj_timeout_timer(lv_timer_t *ptimer)
{

  if (always_record_timeout_sec > 0)
  {
    always_record_timeout_sec--;
    monitor_obj_timeout_label_display();
  }
  else
  {
    layout_always_record_stop();
    usleep(500 * 1000);
    monitor_close(0x02);
    always_record_time_set(user_data_get()->always_monitoring == 1 ? 10 : user_data_get()->always_monitoring == 2 ? 30
                                                                                                                  : 60);
    lv_timer_del(ptimer);
    lv_sat_timer_create(layout_always_monitor_open_task, 1000, NULL);
  }
}

static bool layout_always_record_streams_running_register_callback(char *arg)
{
  lv_sat_timer_create(layout_always_record_delay_task, 1500, NULL);
  return true;
}

static void sat_layout_enter(always_record)
{

  always_record_loop = true;
  is_always_record_video_ing = false;
  always_record_time_set(user_data_get()->always_monitoring == 1 ? 10 : user_data_get()->always_monitoring == 2 ? 30
                                                                                                                : 60);
  user_linphone_call_streams_running_receive_register(layout_always_record_streams_running_register_callback);
  monitor_channel_set(MON_CH_NONE);
  layout_always_monitor_open();
  lv_sat_timer_create(monitor_obj_timeout_timer, 1000, NULL);
  /***********************************************
   ** 作者: leo.liu
   ** 日期: 2023-2-2 13:42:25
   ** 说明: 顶部容器
   ***********************************************/
  {
    lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), always_record_scr_act_obj_id_head_cont, 0, 0, 1024, 80,
                                                NULL, false, LV_OPA_30, 0, LV_OPA_30, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 返回键(CCTV)
     ***********************************************/
    {
      lv_common_img_btn_create(parent, 0, 0, 0, 80, 80,
                               monitor_obj_cctv_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               resource_ui_src_get("btn_title_back.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 通道显示
     ***********************************************/
    {
      lv_obj_t *obj = lv_common_text_create(parent, 1, 0, 23, 950, 42,
                                            NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                            NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
      if (obj != NULL)
      {
        lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
      }
      monitior_obj_channel_info_obj_display();
    }

    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 移动侦测图标显示
     ***********************************************/
    {
      lv_common_img_btn_create(parent, 3, 751, 16, 48, 48,
                               NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               resource_ui_src_get("ic_list_visitor_video.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 自动侦测图标显示
     ***********************************************/
    {
      lv_common_img_btn_create(parent, 4, 807, 16, 48, 48,
                               NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               resource_ui_src_get("ic_monitoring_auto.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    /***********************************************
    ** 作者: leo.liu
    ** 日期: 2023-2-2 13:42:25
    ** 说明: sd图标显示
    ***********************************************/
    {
      lv_common_img_btn_create(parent, 5, 872, 16, 48, 48,
                               NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                               resource_ui_src_get("ic_monitoring_sdcard.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }

    montior_obj_top_icon_display();
    /***********************************************
     ** 作者: leo.liu
     ** 日期: 2023-2-2 13:42:25
     ** 说明: 倒计时
     ***********************************************/
    {
      lv_common_text_create(parent, 6, 927, 23, 73, 42,
                            NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                            NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
      monitor_obj_timeout_label_display();
    }
  }

  always_record_record_btn_display();
  always_record_loop_btn_display();
  standby_timer_close();

  /*sd卡状态处理*/
  sd_state_channge_callback_register(always_record_sd_state_change_callback);

  /*记录注册*/
  record_state_callback_register(always_record_video_state_callback);

  /*抓拍注册*/
  snapshot_state_callback_register(always_record_snapshot_state_callback);
}
static void sat_layout_quit(always_record)
{
  user_data_get()->always_monitoring = 0;
  user_data_save(false, true);
  record_video_stop();
  monitor_close(0x02);
  /*sd卡状态处理 */
  sd_state_channge_callback_register(sd_state_change_default_callback);
  record_state_callback_register(NULL);
  snapshot_state_callback_register(NULL);

  standby_timer_restart(true, true);

  user_linphone_call_streams_running_receive_register(NULL);
}

sat_layout_create(always_record);