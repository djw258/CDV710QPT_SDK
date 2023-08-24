#include "layout_define.h"
#include "layout_monitor.h"
#include "layout_intercom_call.h"
#include "tuya_api.h"
enum
{
        monitor_obj_id_other_call_list,
        monitor_obj_id_full_display_cont,

        monitor_obj_id_adj_cont,
        monitor_obj_id_vol_cont,

        monitor_obj_id_top_cont,
        monitor_obj_id_disp_btn_cont,
        monitor_obj_id_disp_adj_cont,

        monitor_obj_id_vol_adj_cont,

        monitor_obj_id_user_app_label,

        monitor_obj_id_channel_switch_left_btn,
        monitor_obj_id_channel_switch_right_btn,

        monitor_obj_id_channel_switch_CCTTV_monitor,

        monitor_obj_id_buttom_cont,
        monitor_obj_id_obj_cont,
        monitor_obj_id_volume_cont,
        monitor_obj_id_display_cont,
        monitor_obj_id_talk_cont,
        monitor_obj_id_handup_cont,
        monitor_obj_id_cctv_btn,
        monitor_obj_id_rec_video_cont,
        monitor_obj_id_rec_photo_cont,
        monitor_obj_id_extension_call,
        monitor_obj_id_nor_lock_cont,
        monitor_obj_id_lock_1_cont,
        monitor_obj_id_lock_2_cont,

        monitor_obj_id_unlock_icon,

        monitor_obj_id_other_call_bg,

};

typedef enum
{
        monitor_obj_id_brigtness_cont,
        monitor_obj_id_brigtness_icon,
        monitor_obj_id_brigrtness_slider_cont,
        monitor_obj_id_color_slider_cont,
        monitor_obj_id_color_cont,
        monitor_obj_id_color_icon,
        monitor_obj_id_contrast_cont,
        monitor_obj_id_contrast_icon,
        monitor_obj_id_contrast_slider_cont,

} monitor_adj_cont_obj_id;

typedef enum
{
        monitor_vol_obj_id_icon,
        monitor_vol_obj_id_slider_cont,

} monitor_vol_cont_obj_id;

static void monitor_obj_handup_display(void);
static void monitor_obj_normal_lock_display(void);
static void monitor_obj_lock_1_display(void);
static void monitor_obj_lock_2_display(void);
static void monitor_obj_volume_display(void);
static void monitor_obj_dispaly_display(void);

static int monitor_timeout_sec = 0;
static int call_duration = 0;

static bool is_monitor_door_camera_talk = false;
static bool is_monitor_snapshot_ing = false;
static bool is_monitor_record_video_ing = false;
static bool layout_monitor_tuya_event_handle(TUYA_CMD cmd, int arg);

// static char sip_call_name[4];
static bool monitor_talk_call_end_callback(char *arg);

void layout_monitor_goto_layout_process(void)
{
        monitor_close(0x03);
        linphone_incomming_info *node = linphone_incomming_used_node_get(true);

        if (node == NULL)
        { /*没有使用的节点：没有其他呼入的设备,需要考虑indoor 呼叫*/

               
                node = linphone_incomming_used_node_get(false);
                if (node == NULL)
                { /*没有使用的节点：没有其他呼入的设备,需要考虑indoor 呼叫*/
                        sat_linphone_handup(0xFFFF);
                        sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                        /*为了直观，加入return*/
                        return;
                }
                sat_linphone_incomming_refresh(node->call_id);
                intercom_call_status_setting(2);
                char number[128] = {0};
                sprintf(number, "sip:50%d@%s:5066", node->channel, user_data_get()->mastar_wallpad_ip);
                intercom_call_username_setting(number);
                SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
                linphone_incomming_node_release(node);
                sat_layout_goto(intercom_talk, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);

               
        }
        sat_linphone_incomming_refresh(node->call_id);
        monitor_channel_set(node->channel);
        monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
        SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
        linphone_incomming_node_release(node);
        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void monitor_obj_cctv_cancel_obj_click(lv_event_t *e)
{
        layout_monitor_goto_layout_process();
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 获取顶部容器的子控件
 ***********************************************/
static lv_obj_t *monitor_top_child_obj_get(int id)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_top_cont);
        if (parent == NULL)
        {
                return NULL;
        }
        return lv_obj_get_child_form_id(parent, id);
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 获取顶部容器的子控件
 ***********************************************/
static lv_obj_t *monitor_buttom_child_obj_get(int id)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
        if (parent == NULL)
        {
                return NULL;
        }
        return lv_obj_get_child_form_id(parent, id);
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: cctv 返回按钮
 ***********************************************/
static void monitor_obj_cctv_cancel_obj_display(void)
{
        lv_obj_t *obj = monitor_top_child_obj_get(0);
        if (obj == NULL)
        {
                return;
        }
        if (is_channel_ipc_camera(monitor_channel_get()) == true)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
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
                lv_obj_set_x(obj, 60);
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
                channel -= 8;
                lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->cctv_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
        else
        {
                lv_obj_set_x(obj, 37);
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

                // lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->door_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
                lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->door_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
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
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 静音
        ***********************************************/
        {
                obj = monitor_top_child_obj_get(2);
                if (obj == NULL)
                {
                        return;
                }
                if (user_data_get()->audio.ring_mute == 0) 
                {
                        lv_obj_set_x(obj, pos_x);
                        pos_x -= 56;
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("ic_ring_volume_mute.png"), LV_PART_MAIN);
                }
                else
                {
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
        }
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 监控时间
 ***********************************************/
static void monitor_timeout_sec_reset(int sec)
{
        if (sec > 0)
        {
                monitor_timeout_sec = sec;
                return;
        }

        if (monitor_enter_flag_get() == MON_ENTER_MANUAL_CCTV_FLAG)
        {
                monitor_timeout_sec = 60;
        }
        else
        {
                monitor_timeout_sec = 60;
        }
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
        lv_label_set_text_fmt(obj, "%02d:%02d", monitor_timeout_sec / 60, monitor_timeout_sec % 60);
}
static void monitor_obj_timeout_timer(lv_timer_t *ptimer)
{
        int num = tuya_api_client_num();
        if (monitor_timeout_sec > 0)
        {
                monitor_obj_timeout_label_display();
                monitor_timeout_sec--;
        }
        else if (num == 0)
        {

                layout_monitor_goto_layout_process();
        }
        else
        {
                monitor_obj_timeout_label_display();

                monitor_timeout_sec_reset(is_monitor_door_camera_talk ? (user_data_get()->call_time == 1 ? 1 * 60 : user_data_get()->call_time == 2 ? 3 * 60
                                                                                                                                                    : 5 * 60)
                                                                      : 30);
        }
}

static void layout_monitor_channel_type_switch_btn_display(void)
{
        char *door_ch_png[8] = {"btn_call_cam1.png", "btn_call_cam2.png", "btn_call_cam3.png", "btn_call_cam4.png", "btn_call_cam5.png", "btn_call_cam6.png", "btn_call_cam7.png", "btn_call_cam8.png"};
        char *cctv_ch_png[8] = {"btn_call_cctv1.png", "btn_call_cctv2.png", "btn_call_cctv3.png", "btn_call_cctv4.png", "btn_call_cctv5.png", "btn_call_cctv6.png", "btn_call_cctv7.png", "btn_call_cctv8.png"};
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor);
        int ch = monitor_channel_get();
        if (is_channel_ipc_camera(ch))
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(door_ch_png[ch - 8]),
                                            LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(cctv_ch_png[ch]),
                                            LV_PART_MAIN);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 音量调整
 ***********************************************/
static void monitor_obj_volume_click(lv_event_t *e)
{
        lv_obj_t *vol_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_vol_cont);
        lv_obj_t *buttom_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
        lv_obj_t *left_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn);
        lv_obj_t *right_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn);
        if (lv_obj_has_flag(vol_cont, LV_OBJ_FLAG_HIDDEN) == true)
        {
                lv_obj_clear_flag(vol_cont, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(buttom_cont, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(left_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(right_btn, LV_OBJ_FLAG_HIDDEN);
        }
}
static void monitor_obj_volume_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_volume_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(is_monitor_door_camera_talk == true ? "btn_call_sound_voice.png" : "btn_call_sound.png"), LV_PART_MAIN);
        int ch = monitor_channel_get();

        if (is_channel_ipc_camera(ch) == false)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 颜色调整
 ***********************************************/
static void monitor_obj_display_click(lv_event_t *e)
{
        lv_obj_t *adj_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
        lv_obj_t *buttom_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
        lv_obj_t *left_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn);
        lv_obj_t *right_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn);
        if (lv_obj_has_flag(adj_cont, LV_OBJ_FLAG_HIDDEN) == true)
        {
                lv_obj_clear_flag(adj_cont, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(buttom_cont, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(left_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(right_btn, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(adj_cont, LV_OBJ_FLAG_HIDDEN);
        }
}
static void monitor_obj_dispaly_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_display_cont);
        if (obj == NULL)
        {
                return;
        }
        int ch = monitor_channel_get();

        if (is_channel_ipc_camera(ch) == false)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 通话按钮
 ***********************************************/
static void monitor_obj_talk_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_talk_cont);
        if (obj == NULL)
        {
                return;
        }
        if ((is_channel_ipc_camera(monitor_channel_get()) == false) && (is_monitor_door_camera_talk == false))
        {
                if (user_data_get()->etc.open_the_door == 0)
                {
                        lv_obj_set_x(obj, 402);
                }
                else
                {
                        int ch = monitor_channel_get();
                        int lock_num = user_data_get()->etc.door2_lock_num;
                        lv_obj_set_x(obj, ((ch == MON_CH_DOOR2) && (lock_num == 2)) ? 286 : 344);
                }
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

static void layout_monitor_vol_bar_display(void)
{
        lv_obj_t *silder_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_vol_cont), monitor_vol_obj_id_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(silder_cont, 1);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(silder_cont, 0);
        int cur_volume = is_monitor_door_camera_talk == true ? user_data_get()->audio.door_talk_volume : user_data_get()->audio.door_ring_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static void monitor_obj_talk_click(lv_event_t *e)
{
        if (is_monitor_door_camera_talk == false)
        {
                sat_linphone_audio_play_stop();
                is_monitor_door_camera_talk = true;

                call_duration = 0;
                monitor_timeout_sec = user_data_get()->call_time == 1 ? 1 * 60 : user_data_get()->call_time == 2 ? 3 * 60
                                                                                                                 : 5 * 60;
                monitor_enter_flag_set(monitor_enter_flag_get() == MON_ENTER_CALL_FLAG ? MON_ENTER_CALL_TALK_FLAG : MON_ENTER_MANUAL_TALK_FLAG);

                sat_linphone_answer(-1);
                monitor_obj_talk_display();
                monitor_obj_handup_display();
                monitor_obj_normal_lock_display();
                monitor_obj_volume_display();
                monitor_obj_lock_1_display();
                monitor_obj_lock_2_display();
                layout_monitor_vol_bar_display();
        }
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 挂断按钮
 ***********************************************/
static void monitor_obj_handup_click(lv_event_t *e)
{
        if (tuya_api_client_num() > 0)
        {
                tuya_api_preview_quit();
                tuya_api_monitor_handup();
        }
        layout_monitor_goto_layout_process();
}
static void monitor_obj_handup_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_handup_cont);
        if (obj == NULL)
        {
                return;
        }
        int ch = monitor_channel_get();
        int lock_num = user_data_get()->etc.door2_lock_num;

        if (is_channel_ipc_camera(ch) == false)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                if (is_monitor_door_camera_talk == false)
                {
                        if (user_data_get()->etc.open_the_door == 0)
                        {
                                lv_obj_set_x(obj, 518);
                        }
                        else
                        {
                                lv_obj_set_x(obj, ((ch == MON_CH_DOOR2) && (lock_num == 2)) ? 634 : 576);
                        }
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_cancel.png"), LV_PART_MAIN);
                }
                else
                {

                        lv_obj_set_x(obj, ((ch == MON_CH_DOOR2) && (lock_num == 2)) ? 576 : 518);

                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_endcall.png"), LV_PART_MAIN);
                }
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 开锁指令
 ***********************************************/
static void monitor_unlock_ctrl(int ch, int mode, bool en)
{
        if (ch == MON_CH_DOOR1)
        {
                if (mode == 2)
                {
                        door1_lock1_pin_ctrl(en);
                }
                else
                {
                        const char *user = monitor_channel_get_url(ch, false);
                        char *cmd[3] = {
                            "SAT_SHELL echo 33 > /sys/class/gpio/export",
                            "SAT_SHELL echo out > /sys/class/gpio/gpio33/direction",
                            "SAT_SHELL echo 1 > /sys/class/gpio/gpio33/value"};

                        if (en == false)
                        {
                                cmd[2] = "SAT_SHELL echo 0 > /sys/class/gpio/gpio33/value";
                        }

                        for (int i = 0; i < sizeof(cmd) / sizeof(char *); i++)
                        {

                                sat_linphone_message_cmd_send(user, cmd[i]);
                        }
                }
        }
        else if (ch == MON_CH_DOOR2)
        {
                if (mode == 1)
                {

                        const char *user = monitor_channel_get_url(ch, false);
                        char *cmd[3] = {
                            "SAT_SHELL echo 33 > /sys/class/gpio/export",
                            "SAT_SHELL echo out > /sys/class/gpio/gpio33/direction",
                            "SAT_SHELL echo 1 > /sys/class/gpio/gpio33/value"};

                        if (en == false)
                        {
                                cmd[2] = "SAT_SHELL echo 0 > /sys/class/gpio/gpio33/value";
                        }

                        for (int i = 0; i < sizeof(cmd) / sizeof(char *); i++)
                        {
                                sat_linphone_message_cmd_send(user, cmd[i]);
                        }
                }
                else
                {

                        const char *user = monitor_channel_get_url(ch, false);
                        char *cmd[3] = {
                            "SAT_SHELL echo 32 > /sys/class/gpio/export",
                            "SAT_SHELL echo out > /sys/class/gpio/gpio32/direction",
                            "SAT_SHELL echo 1 > /sys/class/gpio/gpio32/value"};

                        if (en == false)
                        {
                                cmd[2] = "SAT_SHELL echo 0 > /sys/class/gpio/gpio32/value";
                        }

                        for (int i = 0; i < sizeof(cmd) / sizeof(char *); i++)
                        {
                                sat_linphone_message_cmd_send(user, cmd[i]);
                        }
                }
        }
}

static void monitor_lock_close(void)
{
        monitor_unlock_ctrl(0, 1, false);
        monitor_unlock_ctrl(0, 2, false);
        monitor_unlock_ctrl(1, 1, false);
        monitor_unlock_ctrl(1, 2, false);
}

static void monitor_obj_unlock_open_timer(lv_timer_t *ptimer)
{
        lv_obj_t *obj = (lv_obj_t *)ptimer->user_data;

        if (obj != NULL)
        {
                lv_obj_del(obj);
        }
        monitor_lock_close();
        lv_timer_del(ptimer);
}
static bool monitor_obj_unlock_icon_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_unlock_icon);
        if (obj != NULL)
        {
                return false;
        }

        obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_unlock_icon, 436, 224, 152, 152,
                                       NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                       0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                       resource_ui_src_get("btn_call_notice_door.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

        lv_timer_reset(lv_sat_timer_create(monitor_obj_unlock_open_timer, 1500, obj));
        return true;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 正常锁按钮
 ***********************************************/
static void monitor_obj_normal_lock_click(lv_event_t *e)
{
        if (monitor_obj_unlock_icon_display() == true)
        {
                ring_unlock_play();
                int ch = monitor_channel_get();
                int mode = 0;
                if (ch == MON_CH_DOOR1)
                {
                        mode = (user_data_get()->etc.door1_open_door_mode == 0) ? 1 : 2;
                }
                else if (ch == MON_CH_DOOR2)
                {
                        mode = user_data_get()->etc.door2_lock_num;
                }
                monitor_unlock_ctrl(ch, mode, true);
        }
}
static void monitor_obj_normal_lock_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_nor_lock_cont);
        if (obj == NULL)
        {
                return;
        }
        int ch = monitor_channel_get();

        if (is_channel_ipc_camera(ch) == false)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                if (user_data_get()->etc.open_the_door == 0)
                {

                        if ((ch != MON_CH_DOOR2) || (user_data_get()->etc.door2_lock_num == 1))
                        {
                                lv_obj_set_x(obj, 402);

                                if (is_monitor_door_camera_talk == false)
                                {
                                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                }
                        }
                        else if ((ch == MON_CH_DOOR2) && (user_data_get()->etc.door2_lock_num == 2))
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                }
                else
                {
                        if ((ch != MON_CH_DOOR2) || (user_data_get()->etc.door2_lock_num == 1))
                        {
                                if (is_monitor_door_camera_talk)
                                {
                                        lv_obj_set_x(obj, 402);
                                }
                                else
                                {
                                        lv_obj_set_x(obj, 460);
                                }
                        }
                        else if ((ch == MON_CH_DOOR2) && (user_data_get()->etc.door2_lock_num == 2))
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                }
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 正常锁按钮
 ***********************************************/
static void monitor_obj_lock_1_click(lv_event_t *e)
{
        if (monitor_obj_unlock_icon_display() == true)
        {
                ring_unlock_play();
                int ch = monitor_channel_get();
                monitor_unlock_ctrl(ch, 1, true);
        }
}
static void monitor_obj_lock_1_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_lock_1_cont);
        if (obj == NULL)
        {
                return;
        }
        int ch = monitor_channel_get();
        int lock_num = user_data_get()->etc.door2_lock_num;
        if ((ch != MON_CH_DOOR2) || (lock_num != 2))
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                return;
        }

        if (is_channel_ipc_camera(ch) == false)
        {
                if (is_monitor_door_camera_talk == false)
                {
                        if (user_data_get()->etc.open_the_door == 0)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                return;
                        }
                        else
                        {
                                lv_obj_set_x(obj, 402);
                                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                }
                else
                {
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_set_x(obj, 344);
                }
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 锁2
 ***********************************************/
static void monitor_obj_lock_2_click(lv_event_t *e)
{
        if (monitor_obj_unlock_icon_display() == true)
        {
                ring_unlock_play();
                int ch = monitor_channel_get();
                monitor_unlock_ctrl(ch, 1, true);
        }
}
static void monitor_obj_lock_2_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_lock_2_cont);
        if (obj == NULL)
        {
                return;
        }
        int ch = monitor_channel_get();
        int lock_num = user_data_get()->etc.door2_lock_num;
        if ((ch != MON_CH_DOOR2) || (lock_num != 2))
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                return;
        }

        if (is_channel_ipc_camera(ch) == false)
        {
                if (is_monitor_door_camera_talk == false)
                {
                        if (user_data_get()->etc.open_the_door == 0)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                        else
                        {
                                lv_obj_set_x(obj, 518);
                                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                }
                else
                {
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_set_x(obj, 460);
                }
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 通话按钮
 ***********************************************/
static void monitor_obj_record_video_click(lv_event_t *e)
{
        if (is_monitor_record_video_ing == true)
        {
                record_video_stop();
        }
        else if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
        {
                record_video_start(true, REC_MODE_MANUAL);
        }
}
static void monitor_obj_record_video_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_rec_video_cont);
        if (obj == NULL)
        {
                return;
        }
        if (is_monitor_record_video_ing == true)
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_rec_ing.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_rec.png"), LV_PART_MAIN);
        }
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 拍照
 ***********************************************/
static void monitor_obj_record_photo_click(lv_event_t *e)
{
        if (is_monitor_snapshot_ing == false)
        {
                sat_linphone_calls_cmd_send();
                record_jpeg_start(REC_MODE_MANUAL);
        }
}
static void monitor_obj_record_photo_display(void)
{
        lv_obj_t *obj = monitor_buttom_child_obj_get(monitor_obj_id_rec_photo_cont);
        if (obj == NULL)
        {
                return;
        }
        if (is_monitor_snapshot_ing == true)
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_save.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_camera.png"), LV_PART_MAIN);
        }
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: SD卡状态变化处理
 ***********************************************/
static void monitor_sd_state_change_callback(void)
{
        montior_obj_top_icon_display();
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 抓拍状态显示
 ***********************************************/
static void monitor_snapshot_state_callback(bool snapshot_ing)
{
        if (snapshot_ing == false)
        {
                SAT_DEBUG("jpeg record finish\n");
        }
        else
        {
                SAT_DEBUG("jpeg record start\n");
        }
        is_monitor_snapshot_ing = snapshot_ing;
        monitor_obj_record_photo_display();
}

/************************************************************
** 函数说明: call机任务
** 作者: xiaoxiao
** 日期: 2023-05-11 16:34:40
** 参数说明:
** 注意事项:
************************************************************/
static void monitor_call_record_delay_task(lv_timer_t *ptimer)
{
        int mode = REC_MODE_TUYA_CALL;

        if (user_data_get()->auto_record_mode != 0)
        {
                if (((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL)) && (user_data_get()->auto_record_mode == 1))
                {
                        if (is_monitor_record_video_ing == false)
                        {
                                record_video_start(true, REC_MODE_AUTO);
                        }
                }
                else
                {
                        mode |= REC_MODE_AUTO;
                }
        }

        record_jpeg_start(mode);
        lv_timer_del(ptimer);
}

static void layout_monitor_streams_running_register_callback(int arg1, int arg2)
{
        if (monitor_enter_flag_get() == MON_ENTER_CALL_FLAG)
        {
                lv_sat_timer_create(monitor_call_record_delay_task, 500, NULL);
        }
}

/************************************************************
** 函数说明:call机日志
** 作者: xiaoxiao
** 日期: 2023-05-17 11:02:00
** 参数说明:
** 注意事项:
************************************************************/
static void layout_monitor_timer_task(lv_timer_t *ptimer)
{

        call_duration++;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 记录状态状态显示
 ***********************************************/
static void monitor_record_video_state_callback(bool record_ing)
{
        is_monitor_record_video_ing = record_ing;
        monitor_obj_record_video_display();
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: app状态显示
 ***********************************************/
static void home_use_mobile_app_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_app_label);
        if (obj == NULL)
        {
                return;
        }

        if (tuya_api_client_num() > 0)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/************************************************************
** 函数说明: switch按钮显示
** 作者: xiaoxiao
** 日期: 2023-06-15 08:32:55
** 参数说明:
** 注意事项:
************************************************************/
static void layout_monitor_switch_btn_display(void)
{
        lv_obj_t *obj_left = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn);
        lv_obj_t *obj_right = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn);
        if (is_channel_ipc_camera(monitor_channel_get()) == true)
        {
                if (0) // (network_data_get()->cctv_device_count <= 1)
                {
                        lv_obj_add_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                }
        }
        else
        {
                if (0) //(network_data_get()->door_device_count <= 1)
                {
                        lv_obj_add_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                }
        }
}

static void layout_monitor_full_screen_display(lv_event_t *e)
{

        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_top_cont);
        lv_obj_t *adj_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
        lv_obj_t *vol_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_vol_cont);

        if ((lv_obj_has_flag(adj_obj, LV_OBJ_FLAG_HIDDEN) == false) || (lv_obj_has_flag(vol_obj, LV_OBJ_FLAG_HIDDEN) == false) || (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) == true))
        {
                lv_obj_add_flag(adj_obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(vol_obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else if (is_channel_ipc_camera(monitor_channel_get()) == true)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_vol_cont);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

static void layout_monitor_brightness_bar_create(lv_obj_t *parent)
{

        lv_common_img_btn_create(parent, monitor_obj_id_brigtness_icon, 251, 23, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_brightness.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        void *left_src = resource_ui_src_alloc("btn_control_minus.png", 42, 42);
        void *right_src = resource_ui_src_alloc("btn_control_plus.png", 42, 42);
        lv_common_slider_create(parent, monitor_obj_id_brigrtness_slider_cont, 269, 23, 540, 48,
                                NULL, LV_OPA_TRANSP, 0X00,
                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                38, 8, 35, 23, 0,
                                "30", 0Xffffff, LV_TEXT_ALIGN_RIGHT, lv_font_normal,
                                127, 18, 328, 12, 1, LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                74, 3, 42, 42, 2,
                                left_src, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                466, 3, 42, 42, 3,
                                right_src, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                360, 9, 0Xffffff, LV_OPA_COVER, NULL,
                                0, 100, user_data_get()->display.lcd_brigtness);

        resouce_file_src_free(left_src);
        resouce_file_src_free(right_src);
}

static void layout_monitor_contrast_bar_create(lv_obj_t *parent)
{
        lv_common_img_btn_create(parent, monitor_obj_id_contrast_icon, 251, 88, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_contrasst.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        void *left_src = resource_ui_src_alloc("btn_control_minus.png", 42, 42);
        void *right_src = resource_ui_src_alloc("btn_control_plus.png", 42, 42);

        lv_common_slider_create(parent, monitor_obj_id_contrast_slider_cont, 269, 88, 540, 48,
                                NULL, LV_OPA_TRANSP, 0X00,
                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                38, 8, 35, 23, 0,
                                "30", 0Xffffff, LV_TEXT_ALIGN_RIGHT, lv_font_normal,
                                127, 18, 328, 12, 1, LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                74, 3, 42, 42, 2,
                                left_src, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                466, 3, 42, 42, 3,
                                right_src, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                360, 9, 0Xffffff, LV_OPA_COVER, NULL,
                                0, 100, user_data_get()->display.lcd_brigtness);

        resouce_file_src_free(left_src);
        resouce_file_src_free(right_src);
}

static void layout_monitor_color_bar_create(lv_obj_t *parent)
{
        lv_common_img_btn_create(parent, monitor_obj_id_color_icon, 251, 153, 48, 48,
                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("ic_saturation.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        void *left_src = resource_ui_src_alloc("btn_control_minus.png", 42, 42);
        void *right_src = resource_ui_src_alloc("btn_control_plus.png", 42, 42);
        lv_common_slider_create(parent, monitor_obj_id_color_cont, 269, 153, 540, 48,
                                NULL, LV_OPA_TRANSP, 0X00,
                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                38, 8, 35, 23, 0,
                                "30", 0Xffffff, LV_TEXT_ALIGN_RIGHT, lv_font_normal,
                                127, 18, 328, 12, 1, LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                74, 3, 42, 42, 2,
                                left_src, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                466, 3, 42, 42, 3,
                                right_src, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                360, 9, 0Xffffff, LV_OPA_COVER, NULL,
                                0, 100, user_data_get()->display.lcd_brigtness);

        resouce_file_src_free(left_src);
        resouce_file_src_free(right_src);
}

static void layout_monitor_setting_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        if (is_monitor_door_camera_talk == true) // 调铃声音量
        {
                int value = lv_slider_get_value(obj);

                user_data_get()->audio.door_talk_volume = value;
                user_data_save();
                sat_linphone_audio_talk_volume_set(value);
        }
        else
        {
                int value = lv_slider_get_value(obj);
                user_data_get()->audio.door_ring_volume = value;
                sat_linphone_audio_play_volume_set(value);
                user_data_save();
        }
}

static void layout_monitor_vol_bar_create(lv_obj_t *parent)
{

        void *left_src = resource_ui_src_alloc("btn_control_minus.png", 42, 42);
        void *right_src = resource_ui_src_alloc("btn_control_plus.png", 42, 42);
        lv_common_slider_create(parent, monitor_vol_obj_id_slider_cont, 97, 23, 831, 48,
                                layout_monitor_setting_volume_slider_change_cb, LV_OPA_TRANSP, 0X00,
                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                38, 8, 35, 23, 0,
                                "30", 0Xffffff, LV_TEXT_ALIGN_RIGHT, lv_font_normal,
                                127, 18, 577, 12, 1, LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                74, 3, 42, 42, 2,
                                left_src, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                715, 3, 42, 42, 3,
                                right_src, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                360, 9, 0Xffffff, LV_OPA_COVER, NULL,
                                0, 100, user_data_get()->display.lcd_brigtness);

        resouce_file_src_free(left_src);
        resouce_file_src_free(right_src);
        layout_monitor_vol_bar_display();
}

static void layout_monitor_channel_type_switch_btn_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        int ch = monitor_channel_get();
        if (is_channel_ipc_camera(ch))
        {
                // if((network_data_get()->door_device_count >= 0))
                {
                        monitor_channel_set(MON_CH_DOOR1);
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_cctv1.png"), LV_PART_MAIN);
                }
        }
        else
        {

                //   if((network_data_get()->cctv_device_count >= 0))
                {

                        if (monitor_valid_channel_check(ch + 8) == false)
                        {

                                return;
                        }
                        monitor_channel_set(ch + 8);
                }
        }
        layout_monitor_channel_type_switch_btn_display();
        monitor_obj_cctv_cancel_obj_display();

        layout_monitor_switch_btn_display();
        monitor_obj_dispaly_display();
        monitor_obj_volume_display();
        monitor_obj_talk_display();
        monitor_obj_handup_display();
        monitor_obj_normal_lock_display();
        monitor_obj_lock_1_display();
        monitor_obj_lock_2_display();
        monitior_obj_channel_info_obj_display();
        monitor_timeout_sec_reset(30);
        monitor_open(true, false);
}

static void layout_monitor_extension_call_click(lv_event_t *e)
{
#if 0
        char *user_name = strstr(sip_call_name, "20");
        if (user_name != NULL)
        {
                user_name += strlen("20");
                int index = user_name[0] - 48;
                if (monitor_valid_channel_check(index - 1) == false)
                {
                        return;
                }
                monitor_close(0x03);
                monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
                monitor_channel_set(index - 1);
                monitor_open(true, false);
                layout_monitor_channel_type_switch_btn_display();
                monitor_obj_cctv_cancel_obj_display();

                monitor_obj_dispaly_display();
                monitor_obj_volume_display();
                monitor_obj_talk_display();
                monitor_obj_handup_display();
                monitor_obj_normal_lock_display();
                monitor_obj_lock_1_display();
                monitor_obj_lock_2_display();

                monitior_obj_channel_info_obj_display();
                monitor_timeout_sec_reset(30);
                layout_monitor_switch_btn_display();
        }
        else if (strstr(sip_call_name, "50") != NULL)
        {
                char number[128] = {0};
                sprintf(number, "sip:%s@%s:5066", sip_call_name, user_data_get()->mastar_wallpad_ip);

                intercom_call_status_setting(1);
                intercom_call_username_setting(number);
                layout_monitor_goto_layout_process();
        }
#endif
}

static bool layout_monitor_outgoing_arly_media_register(char *arg)
{
        return false;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 门口机切换
 ***********************************************/
static void monitor_obj_channel_switch_click(lv_event_t *e)
{
        if (is_monitor_record_video_ing == true)
        {
                record_video_stop();
        }
        lv_obj_t *obj = lv_event_get_current_target(e);
        if (obj == NULL)
        {
                return;
                
        }

        if (obj->id == monitor_obj_id_channel_switch_left_btn)
        {
                int ch = monitor_channel_prev_get();
                if (ch >= 0)
                {
                        monitor_enter_flag_set(is_channel_ipc_camera(ch) == true ? MON_ENTER_MANUAL_DOOR_FLAG : MON_ENTER_MANUAL_CCTV_FLAG);
                        monitor_channel_set(ch);
                        monitor_open(true, false);
                        layout_monitor_channel_type_switch_btn_display();
                        monitor_obj_cctv_cancel_obj_display();

                        monitor_obj_dispaly_display();
                        monitor_obj_volume_display();
                        monitor_obj_talk_display();
                        monitor_obj_handup_display();
                        monitor_obj_normal_lock_display();
                        monitor_obj_lock_1_display();
                        monitor_obj_lock_2_display();

                        monitior_obj_channel_info_obj_display();
                        monitor_timeout_sec_reset(30);
                        layout_monitor_switch_btn_display();
                }
        }
        else if (obj->id == monitor_obj_id_channel_switch_right_btn)
        {
                int ch = monitor_channel_next_get();
                if (ch >= 0)
                {
                        monitor_enter_flag_set(is_channel_ipc_camera(ch) == true ? MON_ENTER_MANUAL_DOOR_FLAG : MON_ENTER_MANUAL_CCTV_FLAG);
                        monitor_channel_set(ch);
                        monitor_open(true, false);
                        layout_monitor_channel_type_switch_btn_display();
                        monitor_obj_cctv_cancel_obj_display();

                        monitor_obj_dispaly_display();
                        monitor_obj_volume_display();
                        monitor_obj_talk_display();
                        monitor_obj_handup_display();
                        monitor_obj_normal_lock_display();
                        monitor_obj_lock_1_display();
                        monitor_obj_lock_2_display();

                        monitior_obj_channel_info_obj_display();
                        monitor_timeout_sec_reset(30);
                        layout_monitor_switch_btn_display();
                }
        }
}

//门口机通道接受通话事件
static void layout_monitor_door_call_btn_click(lv_event_t * ev)
{
        monitor_close(0x03);
        lv_obj_t *obj = lv_event_get_current_target(ev);
        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(obj->id);
        if(node != NULL)
        {
                sat_linphone_incomming_refresh(node->call_id);
                monitor_channel_set(node->channel);
                monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
                SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
                linphone_incomming_node_release(node);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        
}

//内线通道接受通话事件
static void layout_monitor_intercom_call_btn_click(lv_event_t * ev)
{
        monitor_close(0x03);
        lv_obj_t *obj = lv_event_get_current_target(ev);
        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(obj->id);
        if(node != NULL)
        {
                sat_linphone_incomming_refresh(node->call_id);
                intercom_call_status_setting(2);
                char number[128] = {0};
                sprintf(number, "sip:50%d@%s:5066", node->channel, user_data_get()->mastar_wallpad_ip);
                intercom_call_username_setting(number);
                SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
                linphone_incomming_node_release(node);
                sat_layout_goto(intercom_talk, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        
}
static void layout_monitor_door_ch_btn_create(void);
//挂断其他设备的呼叫会话
static void layout_monitor_other_call_handup_btn_click(lv_event_t * ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(lv_obj_get_parent(obj)->id);
        if(node != NULL)
        {
                sat_linphone_handup(node->call_id);
                linphone_incomming_node_release(node);

                layout_monitor_door_ch_btn_create();
        }
}

//会话列表创建
static lv_obj_t * layout_monitor_other_call_list_btn_create(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_other_call_list);
        if (list != NULL)
        {
                lv_obj_clean(list);
                return list;
        }
        list = lv_list_create(sat_cur_layout_screen_get());
        lv_common_style_set_common(list, monitor_obj_id_other_call_list, 32, 96, 253, 360, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        lv_obj_set_style_pad_row(list,20,LV_PART_MAIN);
        
        return list;
}

//门呼叫，内线呼叫会话按键刷新
static void layout_monitor_door_ch_btn_create(void)
{
        linphone_incomming_info node[8] ;
        int total;
   
        lv_obj_t *parent = layout_monitor_other_call_list_btn_create();

        int sec_x = 0;
        int sec_y = 0;
        //门口机通道按键创建
        linphone_incomming_vaild_channel_get(true,node,&total);
        for(int i = 0; i < total; i++)
        {
                lv_obj_t * obj_answer = lv_common_img_text_btn_create(parent, node[i].call_id, sec_x, sec_y, 253, 80,
                                                layout_monitor_door_call_btn_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                110, 21, 93, 35, 1,
                                                network_data_get()->door_device[node[i].channel].door_name, 0, 0, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                0, 0, 253, 80, 0,
                                                (const char *)resource_ui_src_get("btn_call_extension.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);


                lv_obj_t * handup = lv_common_img_btn_create(obj_answer, 2, 205, 24, 32, 32, 
                                        layout_monitor_other_call_handup_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        (const char *)resource_ui_src_get("btn_call_extension_close.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_set_ext_click_area(handup, 30);
                lv_obj_t * sub = lv_obj_get_child_form_id(obj_answer,1);
                if (sub != NULL)
                {
                        lv_label_set_long_mode(sub, LV_LABEL_LONG_SCROLL_CIRCULAR);
                }
                sec_y += 100;   
 
        }

        //内线会话通道创建（坐标沿着门口机通道按键坐标增加）
        linphone_incomming_vaild_channel_get(false,node,&total);
        for(int i = 0; i < total ; i++)
        {
                SAT_DEBUG("inter total is %d",total);
                char sip_user[4] = {0};
                sprintf(sip_user,"50%d",node[i].channel);
                lv_obj_t * obj = lv_common_img_text_btn_create(parent, node[i].call_id, sec_x, sec_y, 253, 80,
                                                layout_monitor_intercom_call_btn_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                110, 21, 93, 35, 1,
                                                sip_user, 0, 0, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                0, 0, 253, 80, 0,
                                                (const char *)resource_ui_src_get("btn_call_extension.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_btn_create(obj, 2,205, 24, 32, 32, 
                                                layout_monitor_other_call_handup_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                (const char *)resource_ui_src_get("btn_call_extension_close.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_t * sub = lv_obj_get_child_form_id(obj,1);
                if (sub != NULL)
                {
                        lv_label_set_long_mode(sub, LV_LABEL_LONG_SCROLL_CIRCULAR);
                }
                sec_y += 100;   
        }

}

static void sat_layout_enter(monitor)
{

        lv_obj_pressed_func = NULL;
        standby_timer_close();
        is_monitor_door_camera_talk = false;
        is_monitor_snapshot_ing = false;
        is_monitor_record_video_ing = false;
        monitor_timeout_sec_reset(30);
        call_duration = 0;
        // 满屏查看
        {

                // lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_full_display_cont, 0, 0, 1024, 600,
                //                          layout_monitor_full_screen_display, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                //                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                //                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                //                          NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_add_event_cb(sat_cur_layout_screen_get(), layout_monitor_full_screen_display, LV_EVENT_CLICKED, NULL);

        }
        layout_monitor_door_ch_btn_create();
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 顶部容器
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_top_cont, 0, 0, 1024, 80,
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

                        monitor_obj_cctv_cancel_obj_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 通道显示
                 ***********************************************/
                {
                        lv_obj_t * obj = lv_common_text_create(parent, 1, 37, 23, 950, 42,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
                        if(obj != NULL)
                        {
                                lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
                        }
                        monitior_obj_channel_info_obj_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 静音图标显示
                 ***********************************************/
                {
                        lv_common_img_btn_create(parent, 2, 695, 16, 48, 48,
                                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("ic_ring_volume_mute.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
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

                        lv_timer_ready(lv_sat_timer_create(monitor_obj_timeout_timer, 1000, NULL));
                }
        }

        /************************************************************
        ** 函数说明: 切换到CCTV
        ** 作者: xiaoxiao
        ** 日期: 2023-07-05 18:25:47
        ** 参数说明:
        ** 注意事项:
        ************************************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor, 912, 384, 80, 80,
                                         layout_monitor_channel_type_switch_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_call_cctv1.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                layout_monitor_channel_type_switch_btn_display();
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: app use
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), monitor_obj_id_user_app_label, 327, 66, 370, 60,
                                                      NULL, LV_OPA_COVER, 0X303030, LV_OPA_TRANSP, 0,
                                                      16, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      16, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(HOME_XLS_LANG_ID_USE_MOBILE_APP), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);
                home_use_mobile_app_obj_display();
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 通道切换
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn, 10, 276, 82, 82,
                                         monitor_obj_channel_switch_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_thumbnail_arrow_left_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn, 932, 276, 82, 82,
                                         monitor_obj_channel_switch_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_thumbnail_arrow_right_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                layout_monitor_switch_btn_display();
        }
        // 分机call机图标创建
        {
                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_extension_call, 61, 104, 253, 80,
                                                         layout_monitor_extension_call_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_call_extension.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 底部容器
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont, 0, 464, 1024, 120,
                                                            NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:42:25
                ** 说明: 显示调整
                ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_display_cont, 32, 24, 80, 81,
                                                 monitor_obj_display_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_brightness.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                        monitor_obj_dispaly_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 音量调整
                 ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_volume_cont, 136, 24, 80, 81,
                                                 monitor_obj_volume_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_sound.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_volume_display();
                }

                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:42:25
                ** 说明: 通话按钮
                ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_talk_cont, 235, 0, 104, 104,
                                                 monitor_obj_talk_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_call.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_talk_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 挂断按钮
                 ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_handup_cont, 517, 0, 104, 104,
                                                 monitor_obj_handup_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_cancel.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_handup_display();
                }
                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:42:25
                ** 说明: 正常锁
                ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_nor_lock_cont, 460, 0, 104, 104,
                                                 monitor_obj_normal_lock_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_door_open.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_normal_lock_display();
                }
                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:42:25
                ** 说明: 锁1
                ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_lock_1_cont, 350, 0, 104, 104,
                                                 monitor_obj_lock_1_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_door1_open.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_lock_1_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 锁2
                 ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_lock_2_cont, 350, 0, 104, 104,
                                                 monitor_obj_lock_2_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_door2_open.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_lock_2_display();
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 视频
                 ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_rec_video_cont, 808, 24, 80, 81,
                                                 monitor_obj_record_video_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_rec.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_record_video_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:25
                 ** 说明: 照片
                 ***********************************************/
                {
                        lv_common_img_btn_create(parent, monitor_obj_id_rec_photo_cont, 912, 24, 80, 81,
                                                 monitor_obj_record_photo_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_camera.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                        monitor_obj_record_photo_display();
                }
        }

        {
                lv_obj_t *adj_cont = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont, 0, 376, 1024, 224,
                                                              NULL, true, LV_OPA_50, 0, LV_OPA_50, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                layout_monitor_brightness_bar_create(adj_cont);
                layout_monitor_contrast_bar_create(adj_cont);
                layout_monitor_color_bar_create(adj_cont);
                lv_obj_add_flag(adj_cont, LV_OBJ_FLAG_HIDDEN);
        }

        {
                lv_obj_t *vol_cont = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_vol_cont, 0, 504, 1024, 96,
                                                              NULL, true, LV_OPA_50, 0, LV_OPA_50, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                layout_monitor_vol_bar_create(vol_cont);
                lv_obj_add_flag(vol_cont, LV_OBJ_FLAG_HIDDEN);
        }

        first_refresh_lcd_cmd_callback_register(layout_monitor_streams_running_register_callback);

        monitor_open(true, is_channel_ipc_camera(monitor_channel_get()) ? true : false);

        layout_monitor_report_vaild_channel();

        /*門口機内部呼叫*/
        user_linphone_call_incoming_received_register(monitor_doorcamera_call_inside_func);
        /*sd卡状态处理*/
        sd_state_channge_callback_register(monitor_sd_state_change_callback);

        /*抓拍注册*/
        snapshot_state_callback_register(monitor_snapshot_state_callback);
        /*记录注册*/
        record_state_callback_register(monitor_record_video_state_callback);

        /*tuya事件注册*/
        tuya_event_cmd_register(layout_monitor_tuya_event_handle);

        /*呼叫繁忙事件注册（在监控状态收到别人的呼叫）*/
        user_linphone_call_busy_register(NULL);

        user_linphone_call_outgoing_early_media_register(layout_monitor_outgoing_arly_media_register);

        user_linphone_call_end_register(monitor_talk_call_end_callback);

        lv_sat_timer_create(layout_monitor_timer_task, 1000, NULL);
}
static void sat_layout_quit(monitor)
{
        lv_obj_remove_event_cb(sat_cur_layout_screen_get(), layout_monitor_full_screen_display);
        lv_obj_pressed_func = lv_layout_touch_callback;
        MON_ENTER_FLAG flag = monitor_enter_flag_get();
        int ch = monitor_channel_get();
        CALL_LOG_TYPE type;

        if (flag == MON_ENTER_CALL_FLAG)
        {
                type = IN_AND_NO_ANSWER;
                time_t time_val;
                time_val = time(NULL);
                time_val -= call_duration;
                struct tm *tm_val = localtime(&time_val);
                user_time_read(tm_val);
                call_list_add(type, ch, call_duration, tm_val);
                layout_last_call_new_flag_set(true);
        }
        else if (flag == MON_ENTER_MANUAL_TALK_FLAG)
        {
                type = CALL_OUT;
                time_t time_val;
                time_val = time(NULL);
                time_val -= call_duration;
                struct tm *tm_val = localtime(&time_val);
                user_time_read(tm_val);
                call_list_add(type, ch, call_duration, tm_val);
                layout_last_call_new_flag_set(true);
        }
        else if (flag == MON_ENTER_CALL_TALK_FLAG)
        {
                type = IN_AND_ANSWER;
                time_t time_val;
                time_val = time(NULL);
                time_val -= call_duration;
                struct tm *tm_val = localtime(&time_val);
                user_time_read(tm_val);
                call_list_add(type, ch, call_duration, tm_val);
                layout_last_call_new_flag_set(true);
        }
        else if (flag == MON_ENTER_TUYA_TALK_FLAG)
        {
                type = IN_AND_ANSWER;
                time_t time_val;
                time_val = time(NULL);
                time_val -= call_duration;
                struct tm *tm_val = localtime(&time_val);
                user_time_read(tm_val);
                call_list_add(type, ch, call_duration, tm_val);
                layout_last_call_new_flag_set(true);
        }

        standby_timer_restart(true);

        user_linphone_call_incoming_received_register(monitor_doorcamera_call_extern_func);

        user_linphone_call_streams_running_receive_register(NULL);
        /*sd卡状态处理 */
        sd_state_channge_callback_register(sd_state_change_default_callback);

        /*抓拍注册*/
        snapshot_state_callback_register(NULL);

        record_state_callback_register(NULL);

        /*tuya事件注册*/
        tuya_event_cmd_register(tuya_event_defalut_handle);

        user_linphone_call_busy_register(NULL);

        user_linphone_call_outgoing_early_media_register(NULL);

        user_linphone_call_end_register(NULL);
}

sat_layout_create(monitor);

/*************************************************************************************************************************************************
*******************************											                                               *******************************
*******************************					                               楚河汉界		                                         *******************************
*******************************											                                               *******************************
*************************************************************************************************************************************************/
// 格式：user"user_name" <sip:sip_name1@172.16.0.110>,sip_name
void extractDataInQuotes(const char *inputStr, char *extractedStr, size_t maxLen)
{
        const char *startPos = strstr(inputStr, "sip:");
        if (startPos != NULL)
        {
                startPos += strlen("sip:");
                const char *endPos = strchr(startPos, '@');
                if (endPos != NULL)
                {

                        size_t length = endPos - startPos;

                        if (length > maxLen - 1)
                                length = maxLen - 1;

                        strncpy(extractedStr, startPos, length);
                        extractedStr[length] = '\0';
                        return;
                }
        }
        extractedStr[0] = '\0';
}

/* arg 的格式为：user:"uername"<sip:xxx@proxy> msg:消息内容 id:建立连接的id号*/
static bool monitor_doorcamera_call_process(const char *arg, bool is_extern_call)
{
        long call_id = 0;
        char *str = strstr(arg, " id:");
        if (str == NULL)
        {
                return false;
        }
        sscanf(str + 4, "%ld", &call_id);




        int index = monitor_index_get_by_user(arg);
        if (index < 0)
        {

                printf("[%s:%d] get channel failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        /*获取别名*/
        char *s = strchr(arg, '"');
        if (s == NULL)
        {
                printf("[%s:%d] get usernmae failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        s += 1;
        char *e = strchr(s, '"');
        if (e == NULL)
        {
                printf("[%s:%d] get username failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        *e = 0;

        /*获取固定命名通道*/
        char fulll_name[128] = {0};
        snprintf(fulll_name, sizeof(fulll_name) - 1, "Door%d(%s)", index, s);

        /*door camera 名称被修改*/
        if (strcmp(fulll_name, network_data_get()->door_device[index - 1].door_name) != 0)
        {
                memset(network_data_get()->door_device[index - 1].door_name, 0, sizeof(network_data_get()->door_device[index - 1].door_name));
                strncpy(network_data_get()->door_device[index - 1].door_name, fulll_name, sizeof(network_data_get()->door_device[index - 1].door_name) - 1);
                network_data_save();
        }

        if (!user_data_get()->audio.ring_mute)
        {
                ring_door_call_play();
        }
        
        /*如果是外部呼叫，则直接进入监控*/
        if (is_extern_call == true)
        {
                if(sat_cur_layout_get() == sat_playout_get(intercom_talk))
                {
                        linphone_incomming_info *node = linphone_incomming_unused_node_get(true);
                        if (node != NULL)
                        {
                                node->enable = true;
                                node->channel = index - 1;
                                node->call_id = call_id;
                                //门呼叫，内线呼叫会话按键刷新
                                extern void layout_intercom_talk_door_ch_btn_create(void);
                                layout_intercom_talk_door_ch_btn_create();
                                SAT_DEBUG("incomming join channel:%d/call id:%ld", node->channel, node->call_id);
                                return true;
                        }
                }
                /*监控参数设置，准备进入监控页面*/
                monitor_channel_set(index - 1);
                monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
        }
        else /*内部呼叫，存储变量*/
        {
                linphone_incomming_info *node = linphone_incomming_unused_node_get(true);
                if (node != NULL)
                {
                        node->enable = true;
                        node->channel = index - 1;
                        node->call_id = call_id;
                        layout_monitor_door_ch_btn_create();
                        SAT_DEBUG("incomming join channel:%d/call id:%ld", node->channel, node->call_id);
                }
        }

        return true;
}

static bool monitor_intercom_extern_call(const char *arg)
{
        if(sat_cur_layout_get() == sat_playout_get(monitor))
        {
                long call_id = 0;
                int index = extern_index_get_by_user(arg);
                if (index < 0)
                {

                        printf("[%s:%d] get extention id failed(%s)\n", __func__, __LINE__, arg);
                        return false;
                }
                char *str = strstr(arg, " id:");
                if (str == NULL)
                {
                        return false;
                }
                sscanf(str + 4, "%ld", &call_id);

                linphone_incomming_info *node = linphone_incomming_unused_node_get(false);
                if (node != NULL)
                {
                        node->enable = true;
                        node->channel = index;
                        node->call_id = call_id;
                        layout_monitor_door_ch_btn_create();
                }
        }else
        {
                if (!user_data_get()->audio.ring_mute)
                {
                        ring_intercom_play();
                }
                intercom_call_status_setting(2);

                intercom_call_username_setting(arg);
                sat_layout_goto(intercom_talk, LV_SCR_LOAD_ANIM_FADE_IN, true);
        }

        return true;
}

/* arg 的格式为：user:"uername"<sip:xxx@proxy> msg:消息内容*/
bool monitor_doorcamera_call_extern_func(char *arg)
{
        printf("[%s:%d] sip device incomming:%s\n", __func__, __LINE__, arg);
        if (user_data_get()->is_device_init == false)
        {
                return false;
        }
        /*sip:2xxx代表门口机*/
        if (strstr(arg, "sip:2") != NULL)
        {
                return monitor_doorcamera_call_process(arg, true);
        }
        /*sip:5xxx代表室内设备*/
        if (strstr(arg, "sip:5") != NULL)
        {
                return monitor_intercom_extern_call(arg);
        }
        return true;
}

bool monitor_doorcamera_call_inside_func(char *arg)
{
        printf("[%s:%d] sip device incomming:%s\n", __func__, __LINE__, arg);
        if (user_data_get()->is_device_init == false)
        {
                return false;
        }
        /*sip:2xxx代表门口机*/
        if (strstr(arg, "sip:2") != NULL)
        {
                return monitor_doorcamera_call_process(arg, false);
        }
        /*sip:5xxx代表室内设备*/
        if (strstr(arg, "sip:5") != NULL)
        {
                return monitor_intercom_extern_call(arg);
        }
        return true;
}

// 呼叫结束事件注册
/* arg 的格式为：user:"uername"<sip:xxx@proxy> msg:消息内容 id:建立连接的id号*/
static bool monitor_doorcamera_end_process(char *arg)
{
        linphone_incomming_info *node = NULL;
        long call_id = 0;
        char *str = strstr(arg, " id:");
        if (str == NULL)
        {
                return false;
        }
        sscanf(str + 4, "%ld", &call_id);

        /*判断id是否是存在incomming中的,如果不是暂存的，那么就是当前监控的*/

        if ((node = linphone_incomming_used_node_get_by_call_id(call_id)) != NULL)
        {
                linphone_incomming_node_release(node);
                layout_monitor_door_ch_btn_create();
        }
        else
        {
                layout_monitor_goto_layout_process();
        }
        return true;
}

static bool monitor_talk_call_end_callback(char *arg)
{
        SAT_DEBUG("arg is %s",arg);
        /*sip:2xxx代表门口机*/
        if (strstr(arg, "sip:20") != NULL)
        {
                return monitor_doorcamera_end_process(arg);
        }
        /*sip:5xxx代表室内设备*/
        else if (strstr(arg, "sip:50") != NULL)
        {
                return monitor_doorcamera_end_process(arg);
        }
        return true;
}


/************************************************************
** 函数说明: tuya开锁处理
** 作者: xiaoxiao
** 日期: 2023-05-30 10:24:19
** 参数说明:
** 注意事项:
************************************************************/
static bool tuya_event_cmd_door_open(int arg)
{
        bool ok = false;
        char password[4] = {0};
        int num = arg % 10;
        sprintf(password, "%d", (arg / 10) & 0xFFFFFFFF);
        if (strncmp(user_data_get()->etc.password, password, 4) == 0)
        {
                ok = true;
                if (monitor_obj_unlock_icon_display() == true)
                {
                        ring_unlock_play();
                        int ch = monitor_channel_get();
                        if (ch == MON_CH_DOOR1)
                        {
                                num = (user_data_get()->etc.door1_open_door_mode == 0) ? 1 : 2;
                        }
                        else if (ch == MON_CH_DOOR2)
                        {
                                num = user_data_get()->etc.door2_lock_num;
                        }
                        monitor_unlock_ctrl(ch, num, true);
                }
        }
        tuya_api_open_door_success_report(ok);
        return ok;
}

/************************************************************
** 函数说明: 通道切换
** 作者: xiaoxiao
** 日期: 2023-05-30 14:02:14
** 参数说明:
** 注意事项:
************************************************************/
static bool tuya_event_cmd_ch_channge(int channel)
{
        int ch = channel - 1 > MON_CH_DOOR2 ? (MON_CH_CCTV1 + channel - 2 - MON_CH_DOOR2) : channel - 1;

        if (monitor_valid_channel_check(ch) == false)
        {
                return false;
        }
        /*****  记录上次的通道 *****/
        tuya_monitor_channel_set(ch);
        if (monitor_channel_get() == ch)
        {
                return layout_monitor_report_vaild_channel();
        }
        monitor_enter_flag_set(MON_ENTER_MANUAL_CCTV_FLAG);
        monitor_channel_set(ch);

        monitor_open(true, false);
        monitor_obj_talk_display();
        monitor_obj_handup_display();
        monitor_obj_normal_lock_display();
        monitor_obj_volume_display();
        monitor_obj_lock_1_display();
        monitor_obj_lock_2_display();
        monitior_obj_channel_info_obj_display();
        monitor_obj_cctv_cancel_obj_display();
        layout_monitor_report_vaild_channel();

        return true;
}
/************************************************************
** 函数说明: 开启或关闭移动侦测
** 作者: xiaoxiao
** 日期: 2023-05-30 13:50:18
** 参数说明:
** 注意事项:
************************************************************/
static bool tuya_event_cmd_motion_enable(int arg)
{
        user_data_get()->motion.enable = (bool)arg;
        user_data_save();
        return true;
}

/************************************************************
** 函数说明: app与设备通话处理
** 作者: xiaoxiao
** 日期: 2023-05-30 21:22:00
** 参数说明:
** 注意事项:
************************************************************/
static bool truye_event_cmd_audio_start(void)
{
        tuya_api_door2_unlock_mode_report(user_data_get()->etc.door2_lock_num);
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_app_label);
        if (obj != NULL)
        {
                lv_label_set_text(obj, lang_str_get(HOME_XLS_LANG_ID_USE_MOBILE_APP));
        }

        return true;
}

/************************************************************
** 函数说明: app退出查看监控
** 作者: xiaoxiao
** 日期: 2023-05-30 22:12:48
** 参数说明:        user_linphone_call_error_register(intercom_talk_call_failed_callback);
** 注意事项:
************************************************************/
static void tuya_event_cmd_video_stop(void)
{
        layout_monitor_goto_layout_process();
        return;
}

/************************************************************
** 函数说明: 监控页面涂鸦事件处理
** 作者: xiaoxiao
** 日期: 2023-05-30 10:23:31
** 参数说明:
** 注意事项:
************************************************************/
static bool layout_monitor_tuya_event_handle(TUYA_CMD cmd, int arg)
{
        switch ((cmd))
        {
        case TUYA_EVENT_CMD_VIDEO_START:
                // return tuya_event_cmd_video_start();
                break;
        case TUYA_EVENT_CMD_VIDEO_STOP:
                tuya_event_cmd_video_stop();
                return true;
                break;
        case TUYA_EVENT_CMD_AUDIO_START:

                return truye_event_cmd_audio_start();
                break;
        case TUYA_EVENT_CMD_ONLINE:
                return layout_monitor_report_vaild_channel();
                break;
        case TUYA_EVENT_CMD_CH_CHANGE:
                return tuya_event_cmd_ch_channge(arg);
                break;
        case TUYA_EVENT_CMD_MOTION_ENBALE:
                return tuya_event_cmd_motion_enable(arg);
                break;
        case TUYA_EVENT_CMD_DOOR_OPEN:
                return tuya_event_cmd_door_open(arg);
                break;
        default:
                SAT_DEBUG("unknow tuya event:%d", cmd);
                break;
        }
        return false;
}
