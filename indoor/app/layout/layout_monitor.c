#include "layout_define.h"
#include "layout_monitor.h"
#include "layout_intercom_call.h"
#include "layout_away_count.h"
#include "tuya_api.h"
#include "common/commax_websocket.h"
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

        monitor_obj_id_user_state_label,

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

        monitor_obj_id_nor_lock_cont,
        monitor_obj_id_lock_1_cont,
        monitor_obj_id_lock_2_cont,

        monitor_obj_id_unlock_icon,

        monitor_obj_id_other_call_bg,

        monitor_obj_id_outdoor_busy_text,
};

typedef enum
{
        monitor_obj_id_brigtness_cont,
        monitor_obj_id_brigtness_icon,
        monitor_obj_id_brigrtness_slider_cont,
        monitor_obj_id_color_slider_cont,
        monitor_obj_id_color_cont,
        monitor_obj_id_color_icon,
        monitor_obj_id_contrast_slider_cont,
        monitor_obj_id_contrast_cont,
        monitor_obj_id_contrast_icon,
        monitor_obj_id_adj_reset,

} monitor_adj_cont_obj_id;

typedef enum
{
        monitor_vol_obj_id_icon,
        monitor_vol_obj_id_slider_cont,

} monitor_vol_cont_obj_id;

unsigned long long call_timestamp[15];

static void monitor_obj_handup_display(void);
static void monitor_obj_normal_lock_display(void);
static void monitor_obj_lock_1_display(void);
static void monitor_obj_lock_2_display(void);
static void monitor_obj_volume_display(void);
static void monitor_obj_dispaly_display(void);

static void layout_monitor_brightness_bar_display(void);
static void layout_monitor_contrast_bar_display(void);
static void layout_monitor_color_bar_display(void);
static void layout_monitor_door_ch_btn_create(void);
static int monitor_timeout_sec = 0;

static bool is_monitor_door_camera_talk = false;
static bool is_monitor_snapshot_ing = false;
static bool is_monitor_record_video_ing = false;
static bool layout_monitor_tuya_event_handle(TUYA_CMD cmd, int arg);
static lv_timer_t *unlock_timer = NULL;

/****************************************************************
 **@日期: 2023-09-20
 **@作者: leo.liu
 **@功能: 图形调整寄存器
 *****************************************************************/
static int monitor_brightness[3] = {0};
static int monitor_saturation[3] = {0};
static int monitor_contrast[3] = {0};
// static char sip_call_name[4];
static bool monitor_talk_call_end_callback(char *arg);

void layout_monitor_goto_layout_process(bool active)
{
        if (is_channel_ipc_camera(monitor_channel_get()) == 1)
        {
                monitor_close(0x02);
        }
        else if (active)
        {
                monitor_close(0x01);
        }
        linphone_incomming_info *node = linphone_incomming_used_node_get(true);
        if ((node == NULL) || (is_monitor_door_camera_talk))
        {
                /*没有使用的节点：没有其他呼入的设备,需要考虑indoor 呼叫*/
                if (tuya_api_client_num() > 0)
                {

                        tuya_api_preview_quit();
                        tuya_api_monitor_handup();
                        tuya_app_quit_status_active_set(false);
                }
                if (node == NULL)
                {
                        node = linphone_incomming_used_node_get(false); /*没有使用的节点：没有其他呼入的设备,需要考虑indoor 呼叫*/
                        if (node == NULL)
                        {
                                sat_linphone_handup(0xFFFF);

                                if (layout_away_count_data_get()->away_setting_time_countdown_timer)
                                {
                                        sat_layout_goto(away_count, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                                }
                                if (alarm_trigger_check(true) == false)
                                {
                                        sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                                }
                                /*为了直观，加入return*/
                                return;
                        }
                        else
                        {
                                sat_linphone_incomming_refresh(node->call_id);
                                intercom_call_status_setting(2);
                                char number[128] = {0};
                                sprintf(number, "sip:50%d@%s:5066", node->channel, user_data_get()->mastar_wallpad_ip);
                                intercom_call_username_setting(number);
                                // SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
                                linphone_incomming_node_release(node);
                                sat_layout_goto(intercom_talk, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                        }
                }
        }

        if (is_channel_ipc_camera(monitor_channel_get()) != true)
        {
                sat_linphone_incomming_refresh(node->call_id);
        }
        sat_linphone_incomming_refresh(node->call_id);
        monitor_channel_set(node->channel);
        layout_monitor_report_vaild_channel();
        monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
        // SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
        linphone_incomming_node_release(node);
        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void monitor_obj_cctv_cancel_obj_click(lv_event_t *e)
{
        layout_monitor_goto_layout_process(true);
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
        if (is_channel_ipc_camera(monitor_channel_get()) == 0x01)
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
        if (is_channel_ipc_camera(channel) == 0x01)
        {
                lv_obj_set_x(obj, 60);
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
                channel -= 8;
                lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->cctv_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
        else if (is_channel_ipc_camera(channel) == 0x00)
        {
                lv_obj_set_x(obj, 37);
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
                // lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->door_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
                lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", network_data_get()->door_device[channel].door_name, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
        else if (channel == 16)
        {
                lv_obj_set_x(obj, 37);
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
                lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", lang_str_get(HOME_XLS_LANG_ID_COMMON_ENTRANCE), tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
        else if (channel == 17)
        {
                lv_obj_set_x(obj, 37);
                lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
                lv_label_set_text_fmt(obj, "%s  %04d-%02d-%02d  %02d:%02d", lang_str_get(SOUND_XLS_LANG_ID_GUARD_STATION), tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
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
                if (user_data_get()->audio.ring_mute)
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

void layout_call_log_create(CALL_LOG_TYPE type, int call_duration, int ch)
{
        if (type == CALL_LOG_UNKNOW)
        {
                return;
        }
        char doorname[24] = {0};
        if (ch >= 0 && ch <= 5)
        {
                strcpy(doorname, network_data_get()->door_device[ch].door_name);
        }
        else if (ch == 6)
        {
                strcpy(doorname, "LOBBY");
        }
        else if (ch == 7)
        {
                strcpy(doorname, "GUARD");
        }
        else
        {
                sprintf(doorname, "50%d", ch - 8 + 1);
        }
        time_t time_val;
        time_val = time(NULL);
        time_val -= call_duration;
        struct tm *tm_val = localtime(&time_val);
        tm_val->tm_year += 1900;
        tm_val->tm_mon += 1;
        call_list_add(type, doorname, call_duration, tm_val);
        layout_last_call_new_flag_set(true);
}

static void monitor_obj_timeout_timer(lv_timer_t *ptimer)
{

        if (monitor_timeout_sec > 0)
        {
                monitor_obj_timeout_label_display();
                monitior_obj_channel_info_obj_display();
                monitor_timeout_sec--;
        }
        else
        {
                int ch = monitor_channel_get();
                int index = (ch >= 0 && ch <= 5) ? ch : (ch == 16 || ch == 17) ? ch - 10
                                                                               : -1;
                if (index != -1)
                {
                        MON_ENTER_FLAG flag = monitor_enter_flag_get();
                        CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
                        if (flag == MON_ENTER_CALL_FLAG)
                        {
                                type = CALL_LOG_IN_AND_NO_ANSWER;
                        }
                        else if (flag == MON_ENTER_CALL_TALK_FLAG)
                        {
                                type = CALL_LOG_IN_AND_ANSWER;
                        }
                        else if ((flag == MON_ENTER_MANUAL_TALK_FLAG) || (flag == MON_ENTER_MANUAL_DOOR_FLAG))
                        {
                                type = CALL_LOG_CALL_OUT;
                        }
                        layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
                }
                layout_monitor_goto_layout_process(true);
        }
}

static void layout_monitor_channel_type_switch_btn_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor);
        char *door_ch_png[8] = {"btn_call_cam1.png", "btn_call_cam2.png", "btn_call_cam3.png", "btn_call_cam4.png", "btn_call_cam5.png", "btn_call_cam6.png", "btn_call_cam7.png", "btn_call_cam8.png"};
        char *cctv_ch_png[8] = {"btn_call_cctv1.png", "btn_call_cctv2.png", "btn_call_cctv3.png", "btn_call_cctv4.png", "btn_call_cctv5.png", "btn_call_cctv6.png", "btn_call_cctv7.png", "btn_call_cctv8.png"};

        MON_ENTER_FLAG flag = monitor_enter_flag_get();
        if ((flag != MON_ENTER_MANUAL_DOOR_FLAG) && (flag != MON_ENTER_MANUAL_CCTV_FLAG))
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                return;
        }
        int ch = monitor_channel_get();

        if (is_channel_ipc_camera(ch) == 0x01)
        {
                int index = monitor_door_first_valid_get(true);
                if (index == -1)
                {
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(door_ch_png[index]),
                                                    LV_PART_MAIN);
                }
        }
        else if (is_channel_ipc_camera(ch) == 0x00)
        {
                int index = monitor_door_first_valid_get(false);
                if (index == -1)
                {
                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(cctv_ch_png[index - 8]),
                                                    LV_PART_MAIN);
                }
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
        lv_obj_t *switch_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor);
        if (lv_obj_has_flag(vol_cont, LV_OBJ_FLAG_HIDDEN) == true)
        {
                lv_obj_clear_flag(vol_cont, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(buttom_cont, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(left_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(right_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(switch_btn, LV_OBJ_FLAG_HIDDEN);
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

        if (is_channel_ipc_camera(ch) != 0x01)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/************************************************************
** 函数说明: 监控顶部状态显示定时关闭
** 作者: xiaoxiao
** 日期：2023-10-06 09:18:57
** 参数说明:
** 注意事项：
************************************************************/
static void monitor_top_display_delay_close_task(lv_timer_t *ptimer)
{
        if (user_data_get()->alarm.buzzer_alarm)
        {
                user_data_get()->alarm.buzzer_alarm = false;
                user_data_save(true, true);
        }
        lv_obj_t *obj = (lv_obj_t *)ptimer->user_data;
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        obj->user_data = NULL;
        lv_timer_del(ptimer);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 颜色调整
 ***********************************************/
static void monitor_obj_display_click(lv_event_t *e)
{

        int channel = monitor_channel_get();
        struct ipcamera_info *device = NULL;
        if (is_channel_ipc_camera(channel) == false)
        {
                device = &network_data_get()->door_device[channel];
        }
        else if (is_channel_ipc_camera(channel) == 0x01)
        {
                device = &network_data_get()->cctv_device[channel - 8];
        }
        else
        {
                return;
        }

        if (sat_ipcamera_image_get(device->ipaddr, device->port, device->username, device->password, device->auther_flag, monitor_brightness, monitor_saturation, monitor_contrast, 500) == true)
        {

                lv_obj_t *adj_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
                lv_obj_t *buttom_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
                lv_obj_t *left_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_left_btn);
                lv_obj_t *right_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_right_btn);
                lv_obj_t *switch_btn = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_channel_switch_CCTTV_monitor);
                if (lv_obj_has_flag(adj_cont, LV_OBJ_FLAG_HIDDEN) == true)
                {
                        lv_obj_clear_flag(adj_cont, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(buttom_cont, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(left_btn, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(right_btn, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(switch_btn, LV_OBJ_FLAG_HIDDEN);

                        layout_monitor_brightness_bar_display();
                        layout_monitor_contrast_bar_display();
                        layout_monitor_color_bar_display();
                }
                else
                {
                        lv_obj_add_flag(adj_cont, LV_OBJ_FLAG_HIDDEN);
                }
        }
        else
        {

                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
                if (obj == NULL)
                {
                        return;
                }

                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_GET_DEVICE_ADJ_INFO_FAILED));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
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

        if (is_channel_ipc_camera(ch) != 2)
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
        int ch = monitor_channel_get();
        if ((is_channel_ipc_camera(monitor_channel_get()) != 0x01) && (is_monitor_door_camera_talk == false))
        {
                if (user_data_get()->etc.open_the_door == 0 || (ch == MON_CH_GUARD))
                {
                        lv_obj_set_x(obj, 402);
                }
                else
                {
                        int lock_num = user_data_get()->etc.door2_lock_num;
                        lv_obj_set_x(obj, ((ch == MON_CH_DOOR2) && (lock_num == 2)) ? 286 : 344);
                }
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                if (tuya_api_client_num() > 0)
                {
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        lv_obj_set_style_bg_img_opa(obj, LV_OPA_20, LV_PART_MAIN);
                }
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
        int cur_volume = 0;
        int ch = monitor_channel_get();
        if (is_channel_ipc_camera(ch) == 0)
        {
                cur_volume = is_monitor_door_camera_talk == true ? user_data_get()->audio.entrance_voice : user_data_get()->audio.entrance_volume;
        }
        else if (ch == 16)
        {
                cur_volume = is_monitor_door_camera_talk == true ? user_data_get()->audio.common_entrance_voice : user_data_get()->audio.common_entrance_volume;
        }
        else if (ch == 17)
        {
                cur_volume = is_monitor_door_camera_talk == true ? user_data_get()->audio.guard_station_voice : user_data_get()->audio.guard_station_volume;
        }
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
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
        MON_ENTER_FLAG flag = monitor_enter_flag_get();
        if ((flag != MON_ENTER_MANUAL_DOOR_FLAG) && (flag != MON_ENTER_MANUAL_CCTV_FLAG))
        {
                lv_obj_add_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                return;
        }
        if (is_channel_ipc_camera(monitor_channel_get()) == true)
        {
                if (cctv_register_num_get() <= 1)
                {
                        lv_obj_add_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                        lv_obj_clear_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_clear_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                }
        }
        else
        {
                if ((door_camera_register_num_get() <= 1) || (monitor_enter_flag_get() == MON_ENTER_CALL_FLAG) || tuya_api_client_num() >= 1)
                {
                        lv_obj_add_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_add_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                        lv_obj_clear_flag(obj_left, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_clear_flag(obj_right, LV_OBJ_FLAG_HIDDEN);
                }
        }
}

static void monitor_obj_talk_click(lv_event_t *e)
{
        if (is_monitor_door_camera_talk == false)
        {
                int ch = monitor_channel_get();
                int index = (ch >= 0 && ch <= 5) ? ch : (ch == 16 || ch == 17) ? ch - 10
                                                                               : -1;
                if (index != -1)
                {

                        call_timestamp[index] = user_timestamp_get();
                }

                sat_linphone_audio_play_stop();
                is_monitor_door_camera_talk = true;

                monitor_timeout_sec = user_data_get()->etc.call_time == 1 ? 1 * 60 : user_data_get()->etc.call_time == 2 ? 2 * 60
                                                                                                                         : 3 * 60;
                monitor_enter_flag_set(monitor_enter_flag_get() == MON_ENTER_CALL_FLAG ? MON_ENTER_CALL_TALK_FLAG : MON_ENTER_MANUAL_TALK_FLAG);
                if (is_alarm_trigger())
                {
                        sat_linphone_alarm_backgound_sound(true);
                }
                sat_linphone_answer(-1, false);
                int cur_volume = 0;
                if (is_channel_ipc_camera(ch) == 0)
                {
                        cur_volume = user_data_get()->audio.entrance_voice;
                }
                else if (ch == 16)
                {
                        cur_volume = user_data_get()->audio.common_entrance_voice;
                }
                else if (ch == 17)
                {
                        cur_volume = user_data_get()->audio.guard_station_voice;
                }

                sat_linphone_audio_talk_volume_set(cur_volume);
                monitor_obj_talk_display();
                layout_monitor_switch_btn_display();
                layout_monitor_channel_type_switch_btn_display();
                monitor_obj_handup_display();
                monitor_obj_normal_lock_display();
                monitor_obj_volume_display();
                monitor_obj_lock_1_display();
                monitor_obj_lock_2_display();
                layout_monitor_vol_bar_display();
                if (tuya_api_client_num() > 0)
                {
                        tuya_api_preview_quit();
                        tuya_api_monitor_handup();
                        tuya_app_quit_status_active_set(false);
                }
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 挂断按钮
 ***********************************************/
static void monitor_obj_handup_click(lv_event_t *e)
{
        int ch = monitor_channel_get();
        int index = (ch >= 0 && ch <= 5) ? ch : (ch == 16 || ch == 17) ? ch - 10
                                                                       : -1;
        if (index != -1)
        {
                MON_ENTER_FLAG flag = monitor_enter_flag_get();
                CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
                if (flag == MON_ENTER_CALL_FLAG)
                {
                        type = CALL_LOG_IN_AND_NO_ANSWER;
                }
                else if (flag == MON_ENTER_CALL_TALK_FLAG)
                {
                        type = CALL_LOG_IN_AND_ANSWER;
                }
                else if ((flag == MON_ENTER_MANUAL_TALK_FLAG) || (flag == MON_ENTER_MANUAL_DOOR_FLAG))
                {
                        type = CALL_LOG_CALL_OUT;
                }

                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
        }

        layout_monitor_goto_layout_process(true);
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
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        if (is_channel_ipc_camera(ch) != 0x01)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                if (is_monitor_door_camera_talk == false)
                {
                        if ((user_data_get()->etc.open_the_door == 0) || (ch == MON_CH_GUARD))
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

                        if (ch == MON_CH_GUARD)
                        {
                                lv_obj_set_x(obj, 460);
                        }
                        else
                        {
                                lv_obj_set_x(obj, ((ch == MON_CH_DOOR2) && (lock_num == 2)) ? 576 : 518);
                        }

                        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_call_endcall.png"), LV_PART_MAIN);
                }
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

typedef struct layout_monitor
{
        int ch;
        int mode;
        bool en;
} door_lock_info;
static pthread_mutex_t door_lock_mutex = PTHREAD_MUTEX_INITIALIZER;
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 开锁指令
 ***********************************************/
static void *monitor_unlock_ctrl_task(void *arg)
{
        door_lock_info *info = (door_lock_info *)malloc(sizeof(door_lock_info));
        if (info == NULL)
        {
                printf("[%s]:malloc failed\n", __func__);
        }

        memcpy(info, arg, sizeof(door_lock_info));

        if ((info->ch == MON_CH_DOOR1) && (info->mode == 2))
        {
                door1_lock1_pin_ctrl(info->en);
        }
        else if ((info->ch == MON_CH_DOOR2) && (info->mode == 2))
        {
                // const char *user = monitor_channel_get_url(ch, false);
                char *cmd[3] = {
                    "echo 32 > /sys/class/gpio/export",
                    "echo out > /sys/class/gpio/gpio32/direction",
                    "echo 1 > /sys/class/gpio/gpio32/value"};

                if (info->en == false)
                {
                        cmd[2] = "echo 0 > /sys/class/gpio/gpio32/value";
                }

                for (int i = 0; i < sizeof(cmd) / sizeof(char *); i++)
                {

                        sat_ipcamera_report_shellcmd(network_data_get()->door_device[info->ch].ipaddr, network_data_get()->door_device[info->ch].port, network_data_get()->door_device[info->ch].username, network_data_get()->door_device[info->ch].password, cmd[i], 2000);
                }
        }
        else if (info->ch != MON_CH_LOBBY && info->ch != MON_CH_GUARD)
        {
                // const char *user = monitor_channel_get_url(ch, false);
                char *cmd[3] = {
                    "echo 33 > /sys/class/gpio/export",
                    "echo out > /sys/class/gpio/gpio33/direction",
                    "echo 1 > /sys/class/gpio/gpio33/value"};

                if (info->en == false)
                {
                        cmd[2] = "echo 0 > /sys/class/gpio/gpio33/value";
                }
                for (int i = 0; i < sizeof(cmd) / sizeof(char *); i++)
                {
                        printf("cmd[%d] is %s\n", i, cmd[i]);
                        sat_ipcamera_report_shellcmd(network_data_get()->door_device[info->ch].ipaddr, 80, "admiin", network_data_get()->door_device[info->ch].password, cmd[i], 2000);
                }
        }
        else if (info->ch == MON_CH_LOBBY)
        {
                commax_https_lobbyphone_open_the_door(commax_transport_ip_get(), "29752", 2000);
        }
        free(info);
        pthread_mutex_unlock(&door_lock_mutex);
        return NULL;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 开锁指令
 ***********************************************/
static void monitor_unlock_ctrl(int ch, int mode, bool en)
{
        pthread_mutex_lock(&door_lock_mutex);
        static door_lock_info info;
        info.ch = ch;
        info.en = en;
        info.mode = mode;
        pthread_t task_id;
        pthread_create(&task_id, sat_pthread_attr_get(), monitor_unlock_ctrl_task, (void *)&info);
        pthread_detach(task_id);
}

static void monitor_obj_unlock_open_timer(lv_timer_t *ptimer)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_unlock_icon);

        if (obj != NULL)
        {
                lv_obj_del(obj);
        }
        door_lock_info *info = (door_lock_info *)obj->user_data;
        monitor_unlock_ctrl(info->ch, info->mode, info->en);
        lv_timer_del(unlock_timer);
        unlock_timer = NULL;
}
static bool monitor_obj_unlock_icon_display(int ch, int mode)
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

        monitor_unlock_ctrl(ch, mode, true);
        static door_lock_info info;
        obj->user_data = &info;
        info.ch = ch;
        info.en = false;
        info.mode = mode;
        lv_timer_reset(unlock_timer = lv_timer_create(monitor_obj_unlock_open_timer, 1500, &info));
        return true;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 正常锁按钮
 ***********************************************/
static void monitor_obj_normal_lock_click(lv_event_t *e)
{
        int ch = monitor_channel_get();
        int mode = 0;
        if (ch == MON_CH_DOOR1)
        {
                mode = (user_data_get()->etc.door1_open_door_mode == 0) ? 1 : 2;
        }
        if (monitor_obj_unlock_icon_display(ch, mode) == true)
        {
                ring_unlock_play();
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

        if (is_channel_ipc_camera(ch) != 0x01)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                if (user_data_get()->etc.open_the_door == 0)
                {
                        if (ch == MON_CH_GUARD)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                        else if ((ch != MON_CH_DOOR2) || (user_data_get()->etc.door2_lock_num == 1))
                        {
                                lv_obj_set_x(obj, 402);

                                if (is_monitor_door_camera_talk == false)
                                {
                                        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                }
                        }
                        else if (((ch == MON_CH_DOOR2) && (user_data_get()->etc.door2_lock_num == 2)))
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                }
                else
                {
                        if (ch == MON_CH_GUARD)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                        else if ((ch != MON_CH_DOOR2) || (user_data_get()->etc.door2_lock_num == 1))
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
                        else if (((ch == MON_CH_DOOR2) && (user_data_get()->etc.door2_lock_num == 2)))
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
        int ch = monitor_channel_get();
        if (monitor_obj_unlock_icon_display(ch, 1) == true)
        {
                ring_unlock_play();
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

        if (is_channel_ipc_camera(ch) != 0x01)
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
        int ch = monitor_channel_get();
        if (monitor_obj_unlock_icon_display(ch, 2) == true)
        {
                ring_unlock_play();
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

        if (is_channel_ipc_camera(ch) != 0x01)
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

        if (is_monitor_record_video_ing == true)
        {
                record_video_stop();
        }

        montior_obj_top_icon_display();
}
static void layout_monitor_snap_timeout_timer(lv_timer_t *time)
{
        is_monitor_snapshot_ing = false;
        monitor_obj_record_photo_display();
        lv_timer_del(time);
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
                lv_sat_timer_create(layout_monitor_snap_timeout_timer, 500, NULL);
        }
        else
        {
                is_monitor_snapshot_ing = true;
                if (record_jpeg_mode_get() == REC_MODE_TUYA_CALL) // 如果没设置自动记录，涂鸦抓拍不需要改变图标
                {
                        return;
                }
                monitor_obj_record_photo_display();
        }
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
                if ((is_alarm_trigger() || user_data_get()->alarm.away_alarm_enable))
                {
                        if (is_alarm_trigger())
                                mode |= REC_MODE_ALARM;
                        if (user_data_get()->alarm.away_alarm_enable)
                                mode |= REC_MODE_AWAY;
                }

                if (((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL)) && (user_data_get()->auto_record_mode == 1))
                {
                        if (is_monitor_record_video_ing == false)
                        {
                                record_video_start(true, mode | REC_MODE_AUTO);
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

static void door_call_enent_tuya_report_timer(lv_timer_t *ptimer)
{
        bool jpeg_recoed = jpeg_record_state_get();
        if (jpeg_recoed == false)
        {
                unsigned char buffer[512];
                int ch = monitor_channel_get();
                ch = is_channel_ipc_camera(ch) == 0 ? ch : is_channel_ipc_camera(ch) == 2 ? ch - 10
                                                                                          : -1;

                tuya_event_report(0x01, ch, buffer, 512);
        }
        lv_timer_del(ptimer);
}

static void layout_monitor_streams_running_register_callback(int arg1, int arg2)
{
        is_monitor_snapshot_ing = false;
        is_monitor_record_video_ing = false;

        if (monitor_enter_flag_get() == MON_ENTER_CALL_FLAG)
        {
                lv_sat_timer_create(monitor_call_record_delay_task, 1000, NULL);
        }
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: app状态显示
 ***********************************************/
static void home_use_mobile_app_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
        if (obj == NULL)
        {
                return;
        }

        if (tuya_api_client_num() > 0)
        {
                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                lv_label_set_text(obj, lang_str_get(HOME_XLS_LANG_ID_USE_MOBILE_APP));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
        }
}

/************************************************************
** 函数说明: 获取tuya用户在线状态接口有延迟
** 作者: xiaoxiao
** 日期：2023-10-11 16:36:09
** 参数说明:
** 注意事项：
************************************************************/
static void monitor_app_use_delay_display_task(lv_timer_t *ptimer)
{
        home_use_mobile_app_obj_display();
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

/************************************************************
** 函数说明: 会话列表显示
** 作者: xiaoxiao
** 日期：2023-09-25 11:54:35
** 参数说明:
** 注意事项：
************************************************************/
static bool layout_monitor_other_call_list_display(void)
{
        linphone_incomming_info node[8];
        int total_monitor = 0;
        int total_extension = 0;
        linphone_incomming_vaild_channel_get(true, node, &total_monitor);
        linphone_incomming_vaild_channel_get(false, node, &total_extension);
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_other_call_list);
        if (list == NULL)
        {

                return false;
        }
        if (is_channel_ipc_camera(monitor_channel_get()) == 0x01)
        {
                lv_obj_add_flag(list, LV_OBJ_FLAG_HIDDEN);
        }
        else if (total_monitor + total_extension == 0x00)
        {
                lv_obj_add_flag(list, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_clear_flag(list, LV_OBJ_FLAG_HIDDEN);
        }
        return true;
}

static void layout_monitor_full_screen_display(lv_event_t *e)
{

        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_top_cont);     // 用在cctv通道判断是否在全屏状态
        lv_obj_t *vol_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_vol_cont); // 用来判断是否在调整显示效果
        lv_obj_t *adj_obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont); // 用来判断是否在调整音量效果
        if ((lv_obj_has_flag(adj_obj, LV_OBJ_FLAG_HIDDEN) == false) || (lv_obj_has_flag(vol_obj, LV_OBJ_FLAG_HIDDEN) == false) || (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) == true))
        {
                lv_obj_add_flag(vol_obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(adj_obj, LV_OBJ_FLAG_HIDDEN);
                layout_monitor_switch_btn_display();
                layout_monitor_channel_type_switch_btn_display();
                layout_monitor_other_call_list_display();

                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_buttom_cont);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else if (is_channel_ipc_camera(monitor_channel_get()) == 0x01)
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
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_other_call_list);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

/************************************************************
** 函数说明: 图像效果复位
** 作者: xiaoxiao
** 日期：2023-10-06 11:39:46
** 参数说明:
** 注意事项：
************************************************************/
static void layout_monitor_setting_adj_reset(lv_event_t *e)
{
        int channel = monitor_channel_get();
        struct ipcamera_info *device = NULL;
        if (is_channel_ipc_camera(channel) == false)
        {
                device = &network_data_get()->door_device[channel];
        }
        else
        {
                device = &network_data_get()->cctv_device[channel - 8];
        }
        int brightness_default = (monitor_brightness[0] + monitor_brightness[1]) / 2;
        int contrats_default = (monitor_contrast[0] + monitor_contrast[1]) / 2;
        int saturation_default = (monitor_saturation[0] + monitor_saturation[1]) / 2;
        if (sat_ipcamera_image_setting(device->ipaddr, device->port, device->username, device->password, device->auther_flag, brightness_default, contrats_default, saturation_default, 500) == true)
        {
                lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
                lv_obj_t *brigtness_cont = lv_obj_get_child_form_id(parent, monitor_obj_id_brigrtness_slider_cont);
                lv_obj_t *color_cont = lv_obj_get_child_form_id(parent, monitor_obj_id_color_slider_cont);
                lv_obj_t *contrast_cont = lv_obj_get_child_form_id(parent, monitor_obj_id_contrast_slider_cont);
                monitor_brightness[2] = brightness_default;
                monitor_contrast[2] = contrats_default;
                monitor_saturation[2] = saturation_default;
                lv_obj_t *text = lv_obj_get_child_form_id(brigtness_cont, 0);
                if (text == NULL)
                {
                        return;
                }
                lv_label_set_text_fmt(text, "%d", brightness_default);

                lv_obj_t *slider = lv_obj_get_child_form_id(brigtness_cont, 1);
                if (slider == NULL)
                {
                        return;
                }
                lv_slider_set_value(slider, brightness_default, LV_ANIM_OFF);
                text = lv_obj_get_child_form_id(color_cont, 0);
                if (text == NULL)
                {
                        return;
                }
                lv_label_set_text_fmt(text, "%d", contrats_default);

                slider = lv_obj_get_child_form_id(color_cont, 1);
                if (slider == NULL)
                {
                        return;
                }
                lv_slider_set_value(slider, brightness_default, LV_ANIM_OFF);
                text = lv_obj_get_child_form_id(contrast_cont, 0);
                if (text == NULL)
                {
                        return;
                }
                lv_label_set_text_fmt(text, "%d", saturation_default);

                slider = lv_obj_get_child_form_id(contrast_cont, 1);
                if (slider == NULL)
                {
                        return;
                }
                lv_slider_set_value(slider, brightness_default, LV_ANIM_OFF);
        }
        else
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
                if (obj == NULL)
                {
                        return;
                }

                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_SET_DEVICE_ADJ_INFO_FAILED));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
        }
}

static void layout_monitor_setting_brightness_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);
        int value = lv_slider_get_value(obj);
        int channel = monitor_channel_get();
        struct ipcamera_info *device = NULL;
        if (is_channel_ipc_camera(channel) == false)
        {
                device = &network_data_get()->door_device[channel];
        }
        else if (is_channel_ipc_camera(channel) == 0x01)
        {
                device = &network_data_get()->cctv_device[channel - 8];
        }
        else
        {
                return;
        }
        monitor_brightness[2] = value;
        if (sat_ipcamera_image_setting(device->ipaddr, device->port, device->username, device->password, device->auther_flag, monitor_brightness[2], monitor_saturation[2], monitor_contrast[2], 500) == false)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
                if (obj == NULL)
                {
                        return;
                }

                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_SET_DEVICE_ADJ_INFO_FAILED));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
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
                                layout_monitor_setting_brightness_slider_change_cb, LV_OPA_TRANSP, 0X00,
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
static void layout_monitor_brightness_bar_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
        if (parent == NULL)
        {
                return;
        }

        parent = lv_obj_get_child_form_id(parent, monitor_obj_id_brigrtness_slider_cont);

        lv_obj_t *text = lv_obj_get_child_form_id(parent, 0);
        if (text == NULL)
        {
                return;
        }
        lv_label_set_text_fmt(text, "%d", monitor_brightness[2]);

        lv_obj_t *slider = lv_obj_get_child_form_id(parent, 1);
        if (slider == NULL)
        {
                return;
        }
        lv_slider_set_range(slider, monitor_brightness[0], monitor_brightness[1]);

        lv_slider_set_value(slider, monitor_brightness[2], LV_ANIM_OFF);
}

static void layout_monitor_setting_contrast_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);
        int value = lv_slider_get_value(obj);
        int channel = monitor_channel_get();
        struct ipcamera_info *device = NULL;
        if (is_channel_ipc_camera(channel) == false)
        {
                device = &network_data_get()->door_device[channel];
        }
        else if (is_channel_ipc_camera(channel) == 0x01)
        {
                device = &network_data_get()->cctv_device[channel - 8];
        }
        else
        {
                return;
        }
        monitor_contrast[2] = value;
        if (sat_ipcamera_image_setting(device->ipaddr, device->port, device->username, device->password, device->auther_flag, monitor_brightness[2], monitor_saturation[2], monitor_contrast[2], 500) == false)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
                if (obj == NULL)
                {
                        return;
                }

                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_SET_DEVICE_ADJ_INFO_FAILED));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
        }
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
                                layout_monitor_setting_contrast_slider_change_cb, LV_OPA_TRANSP, 0X00,
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
static void layout_monitor_contrast_bar_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
        if (parent == NULL)
        {
                return;
        }

        parent = lv_obj_get_child_form_id(parent, monitor_obj_id_contrast_slider_cont);

        lv_obj_t *text = lv_obj_get_child_form_id(parent, 0);
        if (text == NULL)
        {
                return;
        }
        lv_label_set_text_fmt(text, "%d", monitor_contrast[2]);

        lv_obj_t *slider = lv_obj_get_child_form_id(parent, 1);
        if (slider == NULL)
        {
                return;
        }
        lv_slider_set_range(slider, monitor_contrast[0], monitor_contrast[1]);

        lv_slider_set_value(slider, monitor_contrast[2], LV_ANIM_OFF);
}

static void layout_monitor_setting_color_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);
        int value = lv_slider_get_value(obj);
        int channel = monitor_channel_get();
        struct ipcamera_info *device = NULL;
        if (is_channel_ipc_camera(channel) == false)
        {
                device = &network_data_get()->door_device[channel];
        }
        else if (is_channel_ipc_camera(channel) == 0x01)
        {
                device = &network_data_get()->cctv_device[channel - 8];
        }
        else
        {
                return;
        }
        monitor_saturation[2] = value;
        if (sat_ipcamera_image_setting(device->ipaddr, device->port, device->username, device->password, device->auther_flag, monitor_brightness[2], monitor_saturation[2], monitor_contrast[2], 500) == false)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
                if (obj == NULL)
                {
                        return;
                }

                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_SET_DEVICE_ADJ_INFO_FAILED));
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
        }
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
        lv_common_slider_create(parent, monitor_obj_id_color_slider_cont, 269, 153, 540, 48,
                                layout_monitor_setting_color_slider_change_cb, LV_OPA_TRANSP, 0X00,
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
static void layout_monitor_color_bar_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont);
        if (parent == NULL)
        {
                return;
        }

        parent = lv_obj_get_child_form_id(parent, monitor_obj_id_color_slider_cont);
        if (parent == NULL)
        {
                return;
        }

        lv_obj_t *text = lv_obj_get_child_form_id(parent, 0);
        if (text == NULL)
        {
                return;
        }
        lv_label_set_text_fmt(text, "%d", monitor_saturation[2]);

        lv_obj_t *slider = lv_obj_get_child_form_id(parent, 1);
        if (slider == NULL)
        {
                return;
        }

        lv_slider_set_range(slider, monitor_saturation[0], monitor_saturation[1]);

        lv_slider_set_value(slider, monitor_saturation[2], LV_ANIM_OFF);
}
static void layout_monitor_setting_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        if (is_monitor_door_camera_talk == true) // 调铃声音量
        {
                int value = lv_slider_get_value(obj);

                int ch = monitor_channel_get();
                if (is_channel_ipc_camera(ch) == 0)
                {
                        user_data_get()->audio.entrance_voice = value;
                }
                else if (ch == 16)
                {
                        user_data_get()->audio.common_entrance_voice = value;
                }
                else if (ch == 17)
                {
                        user_data_get()->audio.guard_station_voice = value;
                }
                user_data_save(false, false);
                sat_linphone_audio_talk_volume_set(value);
        }
        else
        {
                int value = lv_slider_get_value(obj);
                int ch = monitor_channel_get();
                if (is_channel_ipc_camera(ch) == 0)
                {
                        user_data_get()->audio.entrance_volume = value;
                }
                else if (ch == 16)
                {
                        user_data_get()->audio.common_entrance_volume = value;
                }
                else if (ch == 17)
                {
                        user_data_get()->audio.guard_station_volume = value;
                }
                sat_linphone_audio_play_volume_set(value);
                user_data_save(false, false);
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
        MON_ENTER_FLAG flag = monitor_enter_flag_get();
        if ((flag != MON_ENTER_MANUAL_DOOR_FLAG) && (flag != MON_ENTER_MANUAL_CCTV_FLAG))
        {
                return;
        }
        int ch = monitor_channel_get();
        if (is_channel_ipc_camera(ch) == true)
        {
                int index = monitor_door_first_valid_get(true);
                if (index != -1)
                {
                        monitor_channel_set(index);
                        monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
                }
        }
        else if (is_channel_ipc_camera(ch) == 0x00)
        {
                int index = monitor_door_first_valid_get(false);
                if (index != -1)
                {
                        monitor_enter_flag_set(MON_ENTER_MANUAL_CCTV_FLAG);
                        monitor_channel_set(index);
                }
        }
        else
        {
                return;
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

// 呼叫繁忙事件注册
static bool layout_monitor_busy_callback(char *arg)
{
        SAT_DEBUG("arg is %s", arg);
        return true;
}

// 呼叫事件注册
static bool layout_monitor_outgoing_callback(char *arg)
{
        SAT_DEBUG("arg is %s", arg);
        return true;
}

static bool layout_monitor_outgoing_arly_media_register(char *arg)
{
        SAT_DEBUG("arg is %s", arg);
        return false;
}

static bool monitor_talk_call_failed_callback(char *arg)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
        if (obj == NULL)
        {
                return false;
        }
        int channel = monitor_index_get_by_user(arg);
        if ((channel == -1) || (channel != monitor_channel_get() + 1))
        {
                return false;
        }

        if (obj->user_data)
        {
                lv_timer_del((lv_timer_t *)obj->user_data);
        }
        obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
        if (outdoor_online_check(monitor_channel_get(), NULL) == false)
        {
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_OUTDOOR_OFFLINE));
        }
        else
        {
                lv_label_set_text(obj, lang_str_get(MONITOR_XLS_LANG_ID_OUTDOOR_BUSY));
        }
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        return false;
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 门口机切换
 ***********************************************/
static void monitor_obj_channel_switch_click(lv_event_t *e)
{
        MON_ENTER_FLAG flag = monitor_enter_flag_get();
        if ((flag != MON_ENTER_MANUAL_DOOR_FLAG) && (flag != MON_ENTER_MANUAL_CCTV_FLAG))
        {
                return;
        }
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
                        monitor_enter_flag_set(is_channel_ipc_camera(ch) == 0x01 ? MON_ENTER_MANUAL_DOOR_FLAG : MON_ENTER_MANUAL_CCTV_FLAG);
                        monitor_channel_set(ch);
                        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                }
        }
        else if (obj->id == monitor_obj_id_channel_switch_right_btn)
        {
                int ch = monitor_channel_next_get();
                if (ch >= 0)
                {
                        monitor_enter_flag_set(is_channel_ipc_camera(ch) == 0x01 ? MON_ENTER_MANUAL_DOOR_FLAG : MON_ENTER_MANUAL_CCTV_FLAG);
                        monitor_channel_set(ch);
                        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                }
        }
}

// 门口机通道接受通话事件
static void layout_monitor_door_call_btn_click(lv_event_t *ev)
{
        int ch = monitor_channel_get();
        int index = (ch >= 0 && ch <= 5) ? ch : (ch == 16 || ch == 17) ? ch - 10
                                                                       : -1;
        if (index != -1)
        {
                MON_ENTER_FLAG flag = monitor_enter_flag_get();
                CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
                if (flag == MON_ENTER_CALL_FLAG)
                {
                        type = CALL_LOG_IN_AND_NO_ANSWER;
                }
                else if (flag == MON_ENTER_CALL_TALK_FLAG)
                {
                        type = CALL_LOG_IN_AND_ANSWER;
                }
                else if ((flag == MON_ENTER_MANUAL_TALK_FLAG) || (flag == MON_ENTER_MANUAL_DOOR_FLAG))
                {
                        type = CALL_LOG_CALL_OUT;
                }

                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
        }

        lv_obj_t *obj = lv_event_get_current_target(ev);
        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(obj->id);
        if (node != NULL)
        {
                monitor_close(0x03);
                if ((tuya_api_client_num() > 0) && is_monitor_door_camera_talk)
                {
                        tuya_api_preview_quit();
                        tuya_api_monitor_handup();
                        tuya_app_quit_status_active_set(false);
                }
                sat_linphone_incomming_refresh(node->call_id);
                monitor_channel_set(node->channel);
                monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
                SAT_DEBUG("incomming quit channel:%d/call id:%ld", node->channel, node->call_id);
                linphone_incomming_node_release(node);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        else
        {
                SAT_DEBUG("get call failed");
                layout_monitor_door_ch_btn_create();
        }
}

// 内线通道接受通话事件
static void layout_monitor_intercom_call_btn_click(lv_event_t *ev)
{
        int ch = monitor_channel_get();
        int index = (ch >= 0 && ch <= 5) ? ch : (ch == 16 || ch == 17) ? ch - 10
                                                                       : -1;
        if (index != -1)
        {
                MON_ENTER_FLAG flag = monitor_enter_flag_get();
                CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
                if (flag == MON_ENTER_CALL_FLAG)
                {
                        type = CALL_LOG_IN_AND_NO_ANSWER;
                }
                else if (flag == MON_ENTER_CALL_TALK_FLAG)
                {
                        type = CALL_LOG_IN_AND_ANSWER;
                }
                else if ((flag == MON_ENTER_MANUAL_TALK_FLAG) || (flag == MON_ENTER_MANUAL_DOOR_FLAG))
                {
                        type = CALL_LOG_CALL_OUT;
                }

                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
        }

        lv_obj_t *obj = lv_event_get_current_target(ev);
        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(obj->id);
        if (node != NULL)
        {
                monitor_close(0x03);
                if (tuya_api_client_num() > 0)
                {

                        tuya_api_preview_quit();
                        tuya_api_monitor_handup();
                        tuya_app_quit_status_active_set(false);
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
        else
        {
                SAT_DEBUG("get call failed");
                layout_monitor_door_ch_btn_create();
        }
}

// 挂断其他设备的呼叫会话
static void layout_monitor_other_call_handup_btn_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_current_target(ev);
        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(lv_obj_get_parent(obj)->id);
        if (node != NULL)
        {
                int ch = node->channel;
                int index = (ch >= 0 && ch <= 5) ? ch : (ch == 16 || ch == 17) ? ch - 10
                                                                               : -1;
                if (index != -1)
                {

                        CALL_LOG_TYPE type = CALL_LOG_IN_AND_NO_ANSWER;

                        layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
                }
                sat_linphone_handup(node->call_id);
                linphone_incomming_node_release(node);

                layout_monitor_door_ch_btn_create();
        }
}

// 门呼叫，内线呼叫会话按键刷新
static void layout_monitor_door_ch_btn_create(void)
{
        linphone_incomming_info node[8];
        int total;

        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_other_call_list);
        if (list != NULL)
        {
                lv_obj_del(list);
        }
        list = lv_list_create(sat_cur_layout_screen_get());
        lv_common_style_set_common(list, monitor_obj_id_other_call_list, 32, 96, 253, 360, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        lv_obj_set_style_pad_row(list, 20, LV_PART_MAIN);

        int sec_x = 0;
        int sec_y = 0;
        // 门口机通道按键创建
        linphone_incomming_vaild_channel_get(true, node, &total);

        char ch_name[24];
        for (int i = 0; i < total; i++)
        {
                memset(ch_name, 0, sizeof(ch_name));
                int ch = node[i].channel;
                if (is_channel_ipc_camera(ch) == 0x00)
                {
                        strcpy(ch_name, network_data_get()->door_device[ch].door_name);
                }
                else if (ch == 16)
                {
                        strcpy(ch_name, lang_str_get(HOME_XLS_LANG_ID_COMMON_ENTRANCE));
                }
                else if (ch == 17)
                {
                        strcpy(ch_name, lang_str_get(SOUND_XLS_LANG_ID_GUARD_STATION));
                }
                lv_obj_t *obj_answer = lv_common_img_text_btn_create(list, node[i].call_id, sec_x, sec_y, 253, 80,
                                                                     layout_monitor_door_call_btn_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                                     0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                     0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                     110, 21, 93, 35, 1,
                                                                     ch_name, 0, 0, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                     0, 0, 253, 80, 0,
                                                                     (const char *)resource_ui_src_get("btn_call_extension.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_obj_t *handup = lv_common_img_btn_create(obj_answer, 2, 205, 24, 32, 32,
                                                            layout_monitor_other_call_handup_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            (const char *)resource_ui_src_get("btn_call_extension_close.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_set_ext_click_area(handup, 30);
                lv_obj_t *sub = lv_obj_get_child_form_id(obj_answer, 1);
                if (sub != NULL)
                {
                        lv_label_set_long_mode(sub, LV_LABEL_LONG_SCROLL_CIRCULAR);
                }
                sec_y += 100;
        }

        // 内线会话通道创建（坐标沿着门口机通道按键坐标增加）
        linphone_incomming_vaild_channel_get(false, node, &total);
        for (int i = 0; i < total; i++)
        {
                memset(ch_name, 0, sizeof(ch_name));
                sprintf(ch_name, "%s%d", lang_str_get(INTERCOM_XLS_LANG_ID_EXTENSION), node[i].channel);
                lv_obj_t *obj = lv_common_img_text_btn_create(list, node[i].call_id, sec_x, sec_y, 253, 80,
                                                              layout_monitor_intercom_call_btn_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              110, 21, 93, 35, 1,
                                                              ch_name, 0, 0, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                              0, 0, 253, 80, 0,
                                                              (const char *)resource_ui_src_get("btn_call_extension.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_btn_create(obj, 2, 205, 24, 32, 32,
                                         layout_monitor_other_call_handup_btn_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         (const char *)resource_ui_src_get("btn_call_extension_close.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_t *sub = lv_obj_get_child_form_id(obj, 1);
                if (sub != NULL)
                {
                        lv_label_set_long_mode(sub, LV_LABEL_LONG_SCROLL_CIRCULAR);
                }
                sec_y += 100;
        }

        layout_monitor_other_call_list_display();
}

/************************************************************
** 函数说明: 蜂鸣器呼叫回调
** 作者: xiaoxiao
** 日期：2023-10-07 09:23:50
** 参数说明:
** 注意事项：
************************************************************/
static void layout_monitor_buzzer_alarm_call_callback(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
        if (user_data_get()->alarm.buzzer_alarm)
        {
                buzzer_call_timestamp_set(user_timestamp_get());
                if (obj == NULL)
                {
                        return;
                }
                if ((strncmp(lv_label_get_text(obj), lang_str_get(INTERCOM_XLS_LANG_ID_BUZZER_CALL), strlen(lv_label_get_text(obj))) == 0) && (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) == false)) // 蜂鸣器触发显示中不再接受新的触发
                {
                        return;
                }
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(obj, lang_str_get(INTERCOM_XLS_LANG_ID_BUZZER_CALL));
                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                        obj->user_data = NULL;
                }
                if ((user_data_get()->system_mode & 0x0f) == 0x01)
                {

                        obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 6000, obj);
                }
        }
        else
        {
                lv_timer_t *ptimer = (lv_timer_t *)obj->user_data;
                if (ptimer != NULL)
                {

                        lv_timer_del(ptimer);
                }
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = NULL;
        }
}

static void layout_monitor_touch_callback(lv_event_t *e)
{
        standby_timer_restart(false);
}

static void sat_layout_enter(monitor)
{

        /*呼叫繁忙事件注册（在监控状态收到别人的呼叫）*/
        user_linphone_call_busy_register(layout_monitor_busy_callback);

        user_linphone_call_outgoing_call_register(layout_monitor_outgoing_callback);

        user_linphone_call_outgoing_early_media_register(layout_monitor_outgoing_arly_media_register);

        user_linphone_call_error_register(monitor_talk_call_failed_callback);

        user_linphone_call_end_register(monitor_talk_call_end_callback);
        lv_obj_pressed_func = layout_monitor_touch_callback;
        standby_timer_close();
        is_monitor_door_camera_talk = false;
        is_monitor_snapshot_ing = true;
        is_monitor_record_video_ing = true;
        monitor_timeout_sec_reset(30);

        int ch = monitor_channel_get();
        if (is_channel_ipc_camera(monitor_channel_get()) == false)
        {
                if (tuya_api_client_num() >= 1)
                {
                        sat_ipcamera_device_channel_setting(network_data_get()->door_device[ch].ipaddr, network_data_get()->door_device[ch].port, network_data_get()->door_device[ch].username, network_data_get()->door_device[ch].password, network_data_get()->door_device[ch].auther_flag, 1, 1000);
                }
                else
                {
                        sat_ipcamera_device_channel_setting(network_data_get()->door_device[ch].ipaddr, network_data_get()->door_device[ch].port, network_data_get()->door_device[ch].username, network_data_get()->door_device[ch].password, network_data_get()->door_device[ch].auther_flag, 0, 1000);
                }
        }
        if (monitor_channel_get() == MON_CH_DOOR1 && (user_data_get()->etc.door1_open_door_mode == 1))
        {
                door1_lock1_power_pin_ctrl(true);
        }

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
                        lv_obj_t *obj = lv_common_text_create(parent, 1, 37, 23, 950, 42,
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

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: app use
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label, 327, 66, 370, 60,
                                                      NULL, LV_OPA_COVER, 0X303030, LV_OPA_TRANSP, 0,
                                                      16, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      16, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      lang_str_get(HOME_XLS_LANG_ID_USE_MOBILE_APP), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_timer_set_repeat_count(lv_sat_timer_create(monitor_app_use_delay_display_task, 500, obj), 1);
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

                        // monitor_obj_record_video_display();
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

                        // monitor_obj_record_photo_display();
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
                                         NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                layout_monitor_channel_type_switch_btn_display();
        }

        {
                lv_obj_t *adj_cont = lv_common_img_btn_create(sat_cur_layout_screen_get(), monitor_obj_id_adj_cont, 0, 376, 1024, 224,
                                                              NULL, true, LV_OPA_50, 0, LV_OPA_50, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_common_img_btn_create(adj_cont, monitor_obj_id_adj_reset, 944, 24, 56, 56,
                                         layout_monitor_setting_adj_reset, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_refresh.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

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

        if (user_data_get()->alarm.buzzer_alarm)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
                if (obj == NULL)
                {
                        return;
                }
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(obj, lang_str_get(INTERCOM_XLS_LANG_ID_BUZZER_CALL));
                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                int time = user_timestamp_get() - buzzer_call_timestamp_get();
                if (time > 0 && time <= 6000)
                {
                        obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, time, obj);
                }
        }
        // 呼叫事件延迟推送
        if (monitor_enter_flag_get() == MON_ENTER_CALL_FLAG)
        {
                jpeg_record_state_set(false);
                lv_sat_timer_create(door_call_enent_tuya_report_timer, 5000, NULL);
        }

        buzzer_call_callback_register(layout_monitor_buzzer_alarm_call_callback);

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

        if (monitor_enter_flag_get() == MON_ENTER_CALL_TALK_FLAG)
        {
                monitor_obj_talk_click(NULL);
        }
}

static void sat_layout_quit(monitor)
{
        lv_common_video_mode_enable(false);
        standby_timer_restart(true);
        door1_lock1_power_pin_ctrl(false);
        sat_linphone_alarm_backgound_sound(false);
        if (unlock_timer != NULL)
        {
                monitor_obj_unlock_open_timer(NULL); // 退出监控时间=，把已开启的锁关闭
                unlock_timer = NULL;
        }
        buzzer_call_callback_register(buzzer_alarm_trigger_default);
        record_video_stop();
        lv_obj_remove_event_cb(sat_cur_layout_screen_get(), layout_monitor_full_screen_display);
        lv_obj_pressed_func = lv_layout_touch_callback;

        standby_timer_restart(true);

        user_linphone_call_incoming_received_register(monitor_doorcamera_call_extern_func);

        first_refresh_lcd_cmd_callback_register(NULL);
        /*sd卡状态处理 */
        sd_state_channge_callback_register(sd_state_change_default_callback);

        /*抓拍注册*/
        snapshot_state_callback_register(NULL);

        record_state_callback_register(NULL);

        /*tuya事件注册*/
        tuya_event_cmd_register(tuya_event_defalut_handle);

        user_linphone_call_busy_register(layout_monitor_busy_callback);

        user_linphone_call_outgoing_early_media_register(NULL);

        user_linphone_call_end_register(NULL);

        user_linphone_call_error_register(NULL);
}

sat_layout_create(monitor);

/*************************************************************************************************************************************************
*******************************											                                               *******************************
*******************************					                               楚河汉界		                                         *******************************
*******************************											                                               *******************************
*************************************************************************************************************************************************/
static bool guard_call_process(const char *arg, bool is_extern_call)
{
        long call_id = 0;
        char *str = strstr(arg, " id:");
        if (str == NULL)
        {

                return false;
        }
        sscanf(str + 4, "%ld", &call_id);
        char *start = strstr(arg, "guard");

        /*获取别名*/
        char *end = strchr(start, '"');
        if (end == NULL)
        {
                sat_linphone_handup(call_id);
                printf("[%s:%d] get usernmae failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        *end = 0;
        call_timestamp[7] = user_timestamp_get();

        /*如果是外部呼叫，则直接进入监控*/
        if (is_extern_call == true)
        {
                if (user_data_get()->audio.ring_mute == false)
                {
                        ring_guard_play(user_data_get()->audio.securirty_office_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                }
                monitor_channel_set(MON_CH_GUARD);
                monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
        }
        else /*内部呼叫，存储变量*/
        {
                linphone_incomming_info *node = linphone_incomming_unused_node_get(true);
                if (node != NULL)
                {
                        node->enable = true;
                        node->channel = MON_CH_GUARD;
                        node->call_id = call_id;
                        layout_monitor_door_ch_btn_create();
                }
                // 如果当前是手动进监控的，包括手机和室内机，就把当前手动监控的会话关闭。进入由call机发起的会话
                if ((monitor_enter_flag_get() == MON_ENTER_MANUAL_DOOR_FLAG) || (monitor_enter_flag_get() == MON_ENTER_MANUAL_CCTV_FLAG))
                {
                        if (user_data_get()->audio.ring_mute == false)
                        {
                                ring_guard_play(user_data_get()->audio.securirty_office_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                        }
                        layout_monitor_goto_layout_process(true);
                }
                else
                {
                        layout_monitor_door_ch_btn_create();
                }
        }

        return true;
}

static bool lobby_call_process(const char *arg, bool is_extern_call)
{
        long call_id = 0;
        char *str = strstr(arg, " id:");
        if (str == NULL)
        {
                return false;
        }
        sscanf(str + 4, "%ld", &call_id);
        char *start = strstr(arg, "lobby");
        /*获取别名*/
        char *end = strchr(start, '"');
        if (end == NULL)
        {
                sat_linphone_handup(call_id);
                printf("[%s:%d] get usernmae failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        *end = 0;
        call_timestamp[6] = user_timestamp_get();

        /*如果是外部呼叫，则直接进入监控*/
        if (is_extern_call == true)
        {
                if (user_data_get()->audio.ring_mute == false)
                {
                        ring_common_door_play(user_data_get()->audio.common_entrance_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                }
                monitor_channel_set(MON_CH_LOBBY);
                monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
        }
        else /*内部呼叫，存储变量*/
        {
                linphone_incomming_info *node = linphone_incomming_unused_node_get(true);
                if (node != NULL)
                {
                        node->enable = true;
                        node->channel = MON_CH_LOBBY;
                        node->call_id = call_id;
                        layout_monitor_door_ch_btn_create();
                }
                // 如果当前是手动进监控的，包括手机和室内机，就把当前手动监控的会话关闭。进入由call机发起的会话
                if ((monitor_enter_flag_get() == MON_ENTER_MANUAL_DOOR_FLAG) || (monitor_enter_flag_get() == MON_ENTER_MANUAL_CCTV_FLAG))
                {
                        if (user_data_get()->audio.ring_mute == false)
                        {
                                ring_common_door_play(user_data_get()->audio.common_entrance_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                        }
                        layout_monitor_goto_layout_process(true);
                }
                else
                {
                        layout_monitor_door_ch_btn_create();
                }
        }

        return true;
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
        if ((user_data_get()->system_mode & 0x1f) == 0x01) // 防止没注册的门口机呼叫
        {
                if ((monitor_valid_channel_check(index - 1)) == false)
                {
                        sat_linphone_handup(call_id);
                        return false;
                }
        }
        if (index < 0)
        {
                sat_linphone_handup(call_id);
                printf("[%s:%d] get channel failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        call_timestamp[index - 1] = user_timestamp_get();
        /*获取别名*/
        char *s = strchr(arg, '"');
        if (s == NULL)
        {
                sat_linphone_handup(call_id);
                printf("[%s:%d] get usernmae failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        s += 1;
        char *e = strchr(s, '"');
        if (e == NULL)
        {
                sat_linphone_handup(call_id);
                printf("[%s:%d] get username failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }
        *e = 0;
        /*获取固定命名通道*/
        char fulll_name[64] = {0};
        char doorname[17] = {0};
        strncpy(doorname, s, 16);
        snprintf(fulll_name, sizeof(fulll_name) - 1, "Door%d(%s)", index, doorname);
        /*door camera 名称被修改*/
        if (strcmp(fulll_name, network_data_get()->door_device[index - 1].door_name) != 0)
        {
                memset(network_data_get()->door_device[index - 1].door_name, 0, sizeof(network_data_get()->door_device[index - 1].door_name));
                strncpy(network_data_get()->door_device[index - 1].door_name, fulll_name, sizeof(network_data_get()->door_device[index - 1].door_name) - 1);
                network_data_save();
        }

        /*如果是外部呼叫，则直接进入监控*/
        if (is_extern_call == true)
        {
                if (user_data_get()->audio.ring_mute == false)
                {
                        ring_door_call_play(user_data_get()->audio.door_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
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
                }
                // 如果当前是手动进监控的，包括手机和室内机，就把当前手动监控的会话关闭。进入由call机发起的会话
                if ((monitor_enter_flag_get() == MON_ENTER_MANUAL_DOOR_FLAG) || (monitor_enter_flag_get() == MON_ENTER_MANUAL_CCTV_FLAG))
                {
                        if (user_data_get()->audio.ring_mute == false)
                        {
                                ring_door_call_play(user_data_get()->audio.door_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                        }
                        layout_monitor_goto_layout_process(true);
                }
                else
                {
                        layout_monitor_door_ch_btn_create();
                }
        }

        return true;
}

static bool monitor_intercom_extern_call(const char *arg)
{
        long call_id = 0;
        char *str = strstr(arg, " id:");
        if (str == NULL)
        {
                return false;
        }
        sscanf(str + 4, "%ld", &call_id);
        int index = extern_index_get_by_user(arg);
        if (index < 0)
        {
                sat_linphone_handup(call_id);
                printf("[%s:%d] get extention id failed(%s)\n", __func__, __LINE__, arg);
                return false;
        }

        call_timestamp[index - 1 + 8] = user_timestamp_get();
        if (sat_cur_layout_get() == sat_playout_get(monitor) && (is_channel_ipc_camera(monitor_channel_get()) != 0x01))
        {
                linphone_incomming_info *node = linphone_incomming_unused_node_get(false);
                if (node != NULL)
                {
                        node->enable = true;
                        node->channel = index;
                        node->call_id = call_id;
                        layout_monitor_door_ch_btn_create();
                }
        }
        else
        {
                if (user_data_get()->audio.ring_mute == false)
                {
                        ring_intercom_play(user_data_get()->audio.extenion_tone, 1);
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
        if (strstr(arg, "guard") != NULL) /*sip:guard代表警卫室*/
        {
                return guard_call_process(arg, true);
        }
        else if (strstr(arg, "lobby") != NULL) /*sip:lobby代表大厅机*/
        {
                return lobby_call_process(arg, true);
        }
        else if (strstr(arg, "sip:20") != NULL) /*sip:2xxx代表门口机*/
        {
                return monitor_doorcamera_call_process(arg, true);
        }
        else if (strstr(arg, "user:\"50") != NULL) /*sip:5xxx代表室内设备*/
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
        if (strstr(arg, "guard") != NULL) /*sip:guard代表警卫室*/
        {
                return guard_call_process(arg, false);
        }
        else if (strstr(arg, "lobby") != NULL) /*sip:lobby代表大厅机*/
        {
                return lobby_call_process(arg, false);
        }
        else if (strstr(arg, "sip:20") != NULL) /*sip:2xxx代表门口机*/
        {
                return monitor_doorcamera_call_process(arg, false);
        }
        else if (strstr(arg, "user:\"50") != NULL) /*sip:5xxx代表室内设备*/
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

        int index = 0;
        if (strstr(arg, "sip:20") != NULL)
        {
                index = monitor_index_get_by_user(arg);
                if (index < 0)
                {
                        printf("[%s:%d] get channel failed(%s)\n", __func__, __LINE__, arg);
                        return false;
                }
                index -= 1;
        }
        else if ((strstr(arg, "user:\"50") != NULL) || (strstr(arg, "sip:50") != NULL))
        {
                index = extern_index_get_by_user(arg);
                if (index < 0)
                {
                        printf("[%s:%d] get channel failed(%s)\n", __func__, __LINE__, arg);
                        return false;
                }
                index = index - 1 + 8;
        }
        else if (strstr(arg, "lobby") != NULL) /*lobby代表大厅设备*/
        {

                index = 6;
        }
        else if (strstr(arg, "guard") || strstr(arg, network_data_get()->guard_number)) /*guard代表*/
        {

                index = 7;
        }

        CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
        /*判断id是否是存在incomming中的,如果不是暂存的，那么就是当前监控的*/

        if ((node = linphone_incomming_used_node_get_by_call_id(call_id)) != NULL)
        {
                linphone_incomming_node_release(node);
                layout_monitor_door_ch_btn_create();
                type = CALL_LOG_IN_AND_NO_ANSWER;

                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
        }
        else
        {
                MON_ENTER_FLAG flag = monitor_enter_flag_get();

                if (flag == MON_ENTER_CALL_FLAG)
                {
                        type = CALL_LOG_IN_AND_NO_ANSWER;
                }
                else if (flag == MON_ENTER_CALL_TALK_FLAG)
                {
                        type = CALL_LOG_IN_AND_ANSWER;
                }
                else if ((flag == MON_ENTER_MANUAL_TALK_FLAG) || (flag == MON_ENTER_MANUAL_DOOR_FLAG))
                {
                        type = CALL_LOG_CALL_OUT;
                }
                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
                layout_monitor_goto_layout_process(false);
        }

        return true;
}

static bool monitor_talk_call_end_callback(char *arg)
{
        /*sip:2xxx代表门口机*/
        if (strstr(arg, "sip:20") != NULL)
        {
                return monitor_doorcamera_end_process(arg);
        }
        /*sip:5xxx代表室内设备*/
        else if ((strstr(arg, "user:\"50") != NULL) || (strstr(arg, "sip:50") != NULL))
        {
                return monitor_doorcamera_end_process(arg);
        }
        else if (strstr(arg, "lobby") != NULL) /*lobby代表大厅设备*/
        {

                return monitor_doorcamera_end_process(arg);
        }
        else if (strstr(arg, "guard") || strstr(arg, network_data_get()->guard_number)) /*guard代表*/
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
                int ch = monitor_channel_get();
                if (ch == MON_CH_DOOR1)
                {
                        num = (user_data_get()->etc.door1_open_door_mode == 0) ? 1 : 2;
                }
                else if (ch != MON_CH_DOOR2)
                {
                        num = 1;
                }
                if (monitor_obj_unlock_icon_display(ch, num) == true)
                {
                        ring_unlock_play();
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
        // int ch = channel - 1 > MON_CH_DOOR2 ? (MON_CH_CCTV1 + channel - 2 - MON_CH_DOOR2) : channel - 1;
        int ch = channel;
        if ((ch >= 7) && ch <= 12)
        {
                ch = ch + 1;
        }
        else if ((ch >= 13) && (ch <= 14))
        {
                ch = ch + 3;
        }
        else if ((ch > 0) && (ch < 7))
        {
                ch = ch - 1;
        }
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
        monitor_close(0x03);
        int total = 0;
        linphone_incomming_info node_group[8];
        linphone_incomming_vaild_channel_get(true, node_group, &total);
        monitor_channel_set(ch);
        layout_monitor_report_vaild_channel();
        for (int i = 0; i < total; i++)
        {
                if (node_group[i].channel == ch)
                {
                        linphone_incomming_info *node = linphone_incomming_used_node_get_by_call_id(node_group[i].call_id);
                        if (node != NULL)
                        {
                                sat_linphone_incomming_refresh(node_group[i].call_id);
                                linphone_incomming_node_release(node);
                                monitor_enter_flag_set(MON_ENTER_CALL_FLAG);
                                extern unsigned long long call_timestamp[15];
                                call_timestamp[ch] = user_timestamp_get();
                                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
                        }
                }
        }
        if (is_channel_ipc_camera(ch))
        {
                monitor_enter_flag_set(MON_ENTER_MANUAL_CCTV_FLAG);
        }
        else
        {
                monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
        }
        sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
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
        user_data_save(true, true);
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
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), monitor_obj_id_user_state_label);
        if (obj == NULL)
        {
                return false;
        }
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(obj, lang_str_get(HOME_XLS_LANG_ID_TALK_MOBILE_APP));
        if (obj->user_data)
        {
                lv_timer_del((lv_timer_t *)obj->user_data);
        }
        obj->user_data = lv_sat_timer_create(monitor_top_display_delay_close_task, 3000, obj);
        if (is_monitor_door_camera_talk == false)
        {
                sat_linphone_audio_play_stop();
                is_monitor_door_camera_talk = true;

                monitor_timeout_sec = user_data_get()->etc.call_time == 1 ? 1 * 60 : user_data_get()->etc.call_time == 2 ? 2 * 60
                                                                                                                         : 3 * 60;
                monitor_enter_flag_set(MON_ENTER_TUYA_TALK_FLAG);
                if (is_alarm_trigger())
                {
                        sat_linphone_alarm_backgound_sound(true);
                }
                sat_linphone_answer(-1, true);
                monitor_obj_talk_display();
                monitor_obj_handup_display();
                monitor_obj_normal_lock_display();
                monitor_obj_volume_display();
                monitor_obj_lock_1_display();
                monitor_obj_lock_2_display();
                layout_monitor_vol_bar_display();
        }
        return true;
}

/************************************************************
** 函数说明: app退出查看监控
** 作者: xiaoxiao
** 日期: 2023-05-30 22:12:48
** 参数说明:
** 注意事项:
************************************************************/
static void tuya_event_cmd_video_stop(void)
{
        int ch = monitor_channel_get();
        if (is_channel_ipc_camera(monitor_channel_get()) != 1)
        {
                sat_ipcamera_device_channel_setting(network_data_get()->door_device[ch].ipaddr, network_data_get()->door_device[ch].port, network_data_get()->door_device[ch].username, network_data_get()->door_device[ch].password, network_data_get()->door_device[ch].auther_flag, 0, 1000);
        }
        if ((is_monitor_door_camera_talk == false || monitor_enter_flag_get() == MON_ENTER_TUYA_TALK_FLAG) && (tuya_app_quit_status_active_get() == true))
        {
                layout_monitor_goto_layout_process(true);
        }

        return;
}

/************************************************************
** 函数说明: app进入查看监控
** 作者: xiaoxiao
** 日期: 2023-05-30 22:12:48
** 参数说明:
** 注意事项:
************************************************************/
static bool tuya_event_cmd_video_start(void)
{
        tuya_api_monitor_talk_status_reset();
        tuya_app_quit_status_active_set(true);
        int ch = monitor_channel_get();
        if ((is_monitor_door_camera_talk) && (monitor_enter_flag_get() != MON_ENTER_TUYA_TALK_FLAG))
        {
                tuya_api_preview_quit();
                tuya_api_monitor_handup();
                tuya_app_quit_status_active_set(false);
        }
        else
        {
                if (is_channel_ipc_camera(monitor_channel_get()) == false)
                {
                        sat_ipcamera_device_channel_setting(network_data_get()->door_device[ch].ipaddr, network_data_get()->door_device[ch].port, network_data_get()->door_device[ch].username, network_data_get()->door_device[ch].password, network_data_get()->door_device[ch].auther_flag, 1, 1000);
                }
                home_use_mobile_app_obj_display();
                monitor_obj_talk_display();
                layout_monitor_switch_btn_display();
        }

        return true;
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
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_VIDEO_START");
                return tuya_event_cmd_video_start();
                break;
        case TUYA_EVENT_CMD_VIDEO_STOP:
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_VIDEO_STOP");
                tuya_event_cmd_video_stop();
                return true;
                break;
        case TUYA_EVENT_CMD_AUDIO_START:
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_AUDIO_START");
                return truye_event_cmd_audio_start();
                break;
        case TUYA_EVENT_CMD_ONLINE:
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_ONLINE");
                return layout_monitor_report_vaild_channel();
                break;
        case TUYA_EVENT_CMD_CH_CHANGE:
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_CH_CHANGE");
                SAT_DEBUG("change tuya ch %d", arg);
                return tuya_event_cmd_ch_channge(arg);
                break;
        case TUYA_EVENT_CMD_MOTION_ENBALE:
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_MOTION_ENBALE");
                return tuya_event_cmd_motion_enable(arg);
                break;
        case TUYA_EVENT_CMD_DOOR_OPEN:
                SAT_DEBUG("receive tuya cmd is TUYA_EVENT_CMD_DOOR_OPEN");
                return tuya_event_cmd_door_open(arg);
                break;
        default:
                SAT_DEBUG("unknow tuya event:%d", cmd);
                break;
        }
        return false;
}
