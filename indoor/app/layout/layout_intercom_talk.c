#include "layout_define.h"
#include "layout_intercom_call.h"
#include "layout_monitor.h"
enum
{
        intercom_talk_obj_id_other_call_list, // 此id在页面不可用
        intercom_talk_screen_obj_id = 1,
        intercom_talk_obj_top,
        intercom_talk_obj_status_label,
        intercom_talk_obj_status_icon,
        intercom_talk_obj_volume,
        intercom_talk_obj_handup,
        intercom_talk_obj_answer,
        intercom_talk_call_bottom_cont,
        intercom_talk_obj_id_vol_cont,
        intercom_talk_vol_obj_id_slider_cont,
        intercom_talk_obj_id_other_call_bg,

        intercom_talk_obj_id_buzzer_call_label
};
extern unsigned long long call_timestamp[15];

static int intercom_talk_timeout = 0;
/*0:空闲，1：call outgoing 2:incomming 3:in_talk 4:out_talk*/
static int intercom_call_state = 0;
static char *intercom_call_user = NULL;

void intercom_call_status_setting(int state)
{
        intercom_call_state = state;
}
bool intercom_call_username_setting(const char *user)
{
        if (user == NULL)
        {
                return false;
        }
        if (intercom_call_user != NULL)
        {
                lv_mem_free(intercom_call_user);
        }
        intercom_call_user = lv_mem_alloc(strlen(user) + 1);
        strcpy(intercom_call_user, user);
        return true;
}

static lv_obj_t *intercom_talk_call_top_obj_item_get(int id)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_top);
        if (parent == NULL)
        {
                SAT_DEBUG(" lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_top);");
                return NULL;
        }
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);
        if (obj == NULL)
        {
                SAT_DEBUG(" lv_obj_t* obj = lv_obj_get_child_form_id(parent,0);");
                return NULL;
        }
        return obj;
}

static void intercom_talk_call_info_display(void)
{
        lv_obj_t *obj = intercom_talk_call_top_obj_item_get(0);
        if (obj == NULL)
        {
                SAT_DEBUG("lv_obj_t* obj = intercom_talk_call_top_obj_item_get(0);");
                return;
        }
        struct tm tm;
        user_time_read(&tm);
        int index = extern_index_get_by_user(intercom_call_user);
        if (index != -1)
        {
                lv_label_set_text_fmt(obj, "Call: %s %d    %04d-%02d:%02d %02d:%02d", lang_str_get(INTERCOM_XLS_LANG_ID_EXTENSION), index, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
        else
        {
                lv_label_set_text_fmt(obj, "Call: %s    %04d-%02d:%02d %02d:%02d", intercom_call_user, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
        }
}

static void intercom_talk_call_time_display(void)
{
        lv_obj_t *obj = intercom_talk_call_top_obj_item_get(1);
        if (obj == NULL)
        {
                SAT_DEBUG("lv_obj_t* obj = intercom_talk_call_top_obj_item_get(0);");
                return;
        }
        lv_label_set_text_fmt(obj, "%02d:%02d", intercom_talk_timeout / 60, intercom_talk_timeout % 60);
}

static void intercom_talk_call_time_timer(lv_timer_t *ptime)
{
        if (intercom_talk_timeout == 0)
        {
                int index = extern_index_get_by_user(intercom_call_user);
                if (index != -1)
                {
                        index += 7;
                        CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
                        if (intercom_call_state == 0X01 || intercom_call_state == 0x04)
                        {
                                type = CALL_LOG_CALL_OUT;
                        }
                        else if (intercom_call_state == 0X02)
                        {
                                type = CALL_LOG_IN_AND_NO_ANSWER;
                        }
                        else if (intercom_call_state == 0X03)
                        {
                                type = CALL_LOG_IN_AND_ANSWER;
                        }
                        layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
                }
                layout_monitor_goto_layout_process(true);
                return;
        }
        intercom_talk_call_info_display();
        intercom_talk_call_time_display();
        intercom_talk_timeout--;
}

static void intercom_talk_status_background_display(void)
{
        if ((intercom_call_state != 3) || (intercom_call_state != 4))
        {
                lv_disp_set_bg_image(lv_disp_get_default(), resource_wallpaper_src_get("img_calling_backgroud.jpg", 1024, 600));
        }
        else
        {
                lv_disp_set_bg_image(lv_disp_get_default(), resource_wallpaper_src_get("img_calling_voice_backgroud.jpg", 1024, 600));
        }
}

static void intercom_talk_call_status_label_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_status_label);
        if (obj == NULL)
        {
                SAT_DEBUG("   lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_status_label);");
                return;
        }

        lv_label_set_text(obj, lang_str_get(intercom_call_state == 1   ? INTERCOM_XLS_LANG_ID_OUTCOMING_CALL
                                            : intercom_call_state == 2 ? INTERCOM_XLS_LANG_ID_INCOMMING_CALL
                                                                       : INTERCOM_XLS_LANG_ID_CONNECTED_CALL));
}
static void intercom_talk_call_status_icon_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_status_icon);
        if (obj == NULL)
        {
                SAT_DEBUG("   lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_status_icon);");
                return;
        }
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get((intercom_call_state == 3 || intercom_call_state == 4) ? "img_calling_voice.png" : "img_calling.png"), LV_PART_MAIN);
}

static void intercom_talk_handup_obj_click(lv_event_t *e)
{
        CALL_LOG_TYPE type = CALL_LOG_UNKNOW;
        if (intercom_call_state == 0X01 || intercom_call_state == 0x04)
        {
                type = CALL_LOG_CALL_OUT;
        }
        else if (intercom_call_state == 0X02)
        {
                type = CALL_LOG_IN_AND_NO_ANSWER;
        }
        else if (intercom_call_state == 0X03)
        {
                type = CALL_LOG_IN_AND_ANSWER;
        }
        int index = extern_index_get_by_user(intercom_call_user);
        if (index != -1)
        {
                index += 7;
                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[index]) / 1000, index);
        }
        else if (strstr(intercom_call_user, lang_str_get(SOUND_XLS_LANG_ID_GUARD_STATION)))
        {
                layout_call_log_create(type, (user_timestamp_get() - call_timestamp[7]) / 1000, 7);
        }
        layout_monitor_goto_layout_process(true);
}

static void intercom_talk_handup_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_call_bottom_cont), intercom_talk_obj_handup);
        if (obj == NULL)
        {
                SAT_DEBUG("   lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_handup);");
                return;
        }
        lv_obj_set_x(obj, intercom_call_state == 2 ? 536 : 460);
}

static void intercom_talk_answer_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_call_bottom_cont), intercom_talk_obj_answer);
        if (obj == NULL)
        {
                SAT_DEBUG("   lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_handup);");
                return;
        }
        if (intercom_call_state == 2)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

// 呼叫繁忙事件注册
static bool intercom_talk_call_busy_callback(char *arg)
{
        return true;
}

static void intercom_talk_call_volume_obj_click(lv_event_t *e)
{
        lv_obj_t *bottom = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_call_bottom_cont);
        lv_obj_t *vol_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_id_vol_cont);
        lv_obj_add_flag(bottom, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(vol_cont, LV_OBJ_FLAG_HIDDEN);
}

static void intercom_talk_call_volume_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_call_bottom_cont), intercom_talk_obj_volume);
        if (obj == NULL)
        {
                SAT_DEBUG("   lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_volume);");
                return;
        }
        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get((intercom_call_state == 3 || intercom_call_state == 4) ? "btn_call_sound_voice.png" : "btn_call_sound.png"), LV_PART_MAIN);
}

static void layout_intercom_talk_vol_bar_display(void)
{
        lv_obj_t *silder_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_id_vol_cont), intercom_talk_vol_obj_id_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(silder_cont, 1);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(silder_cont, 0);
        int cur_volume = (intercom_call_state == 0x03 || intercom_call_state == 0x04) ? user_data_get()->audio.extension_voice : user_data_get()->audio.extension_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static void intercom_talk_answer_obj_click(lv_event_t *e)
{
        int index = extern_index_get_by_user(intercom_call_user);
        if (index >= 0)
        {
                index += 7;
                call_timestamp[index] = user_timestamp_get();
        }
        sat_linphone_audio_play_stop();
        intercom_call_state = 3;

        intercom_talk_timeout = 60; /*user_data_get()->etc.call_time == 1 ? 1 * 60 : user_data_get()->etc.call_time == 2 ? 3 * 60
                                                                                                            : 5 * 60;*/
        intercom_talk_call_volume_obj_display();
        intercom_talk_answer_obj_display();
        intercom_talk_handup_obj_display();
        intercom_talk_call_status_label_display();
        intercom_talk_call_status_icon_display();
        intercom_talk_status_background_display();
        intercom_talk_answer_obj_display();

        sat_linphone_answer(-1, false);
        layout_intercom_talk_vol_bar_display();
}

static bool intercom_talk_call_answer_callback(char *arg)
{
        if (intercom_call_state == 4)
        {
                return false;
        }
        char *start = strstr(arg, network_data_get()->guard_number);
        if (start != NULL)
        {

                monitor_enter_flag_set(MON_ENTER_CALL_TALK_FLAG);
                monitor_channel_set(MON_CH_GUARD);
                sat_layout_goto(monitor, LV_SCR_LOAD_ANIM_FADE_IN, true);
        }
        sat_linphone_audio_play_stop();
        intercom_call_state = 4;

        intercom_talk_timeout = 60; /*user_data_get()->etc.call_time == 1 ? 1 * 60 : user_data_get()->etc.call_time == 2 ? 3 * 60
                                                                                                            : 5 * 60;*/
        intercom_talk_call_volume_obj_display();
        intercom_talk_answer_obj_display();
        intercom_talk_handup_obj_display();
        intercom_talk_call_status_label_display();
        intercom_talk_call_status_icon_display();
        intercom_talk_status_background_display();
        intercom_talk_answer_obj_display();
        sat_linphone_answer(-1, false);
        sat_linphone_audio_talk_volume_set(user_data_get()->audio.extension_voice);
        layout_intercom_talk_vol_bar_display();
        return true;
}

static void setting_intercom_talk_call_slider_obj_change_cb(lv_event_t *ev)
{

        lv_obj_t *parent = lv_event_get_current_target(ev);

        int value = lv_slider_get_value(parent);
        if ((intercom_call_state == 3 || intercom_call_state == 4))
        {
                user_data_get()->audio.extension_voice = value;
                sat_linphone_audio_talk_volume_set(value);
        }
        else
        {
                user_data_get()->audio.extension_volume = value;
                sat_linphone_audio_play_volume_set(value);
        }
        user_data_save(false, false);
}

static void layout_intercom_talk_vol_bar_create(lv_obj_t *parent)
{

        void *left_src = resource_ui_src_alloc("btn_control_minus.png", 42, 42);
        void *right_src = resource_ui_src_alloc("btn_control_plus.png", 42, 42);
        lv_common_slider_create(parent, intercom_talk_vol_obj_id_slider_cont, 97, 23, 831, 48,
                                setting_intercom_talk_call_slider_obj_change_cb, LV_OPA_TRANSP, 0X00,
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
        layout_intercom_talk_vol_bar_display();
}

static void layout_intercom_talk_call_screen_click(lv_event_t *e)
{
        lv_obj_t *bottom = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_call_bottom_cont);
        lv_obj_t *vol_cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_id_vol_cont);
        if (lv_obj_has_flag(vol_cont, LV_OBJ_FLAG_HIDDEN) == false)
        {
                lv_obj_clear_flag(bottom, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(vol_cont, LV_OBJ_FLAG_HIDDEN);
        }
}

static void intercom_talk_buzzer_call_delay_close_task(lv_timer_t *ptimer)
{
        if ((user_data_get()->system_mode & 0x0f) == 0x01)
        {
                user_data_get()->alarm.buzzer_alarm = false;
                user_data_save(true, true);
        }
        lv_obj_t *obj = (lv_obj_t *)ptimer->user_data;
        if (obj != NULL)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj->user_data = NULL;
        }
        lv_timer_del(ptimer);
}

/************************************************************
** 函数说明: 蜂鸣器呼叫回调
** 作者: xiaoxiao
** 日期：2023-10-07 09:23:50
** 参数说明:
** 注意事项：
************************************************************/
static void intercom_talk_buzzer_alarm_call_callback(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_id_buzzer_call_label);
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
                if ((user_data_get()->system_mode & 0x0f) == 0x01)
                {
                        if (obj->user_data)
                        {
                                lv_timer_del((lv_timer_t *)obj->user_data);
                        }
                        obj->user_data = lv_sat_timer_create(intercom_talk_buzzer_call_delay_close_task, 6000, obj);
                }
        }
        else
        {
                if (strcmp(lv_label_get_text(obj), lang_str_get(INTERCOM_XLS_LANG_ID_BUZZER_CALL)) == 0)
                {
                        lv_timer_t *ptimer = (lv_timer_t *)obj->user_data;
                        if (ptimer != NULL)
                        {
                                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                lv_timer_del(ptimer);
                                obj->user_data = NULL;
                        }
                }
        }
}

static void layout_intercom_talk_touch_callback(lv_event_t *e)
{
        standby_timer_restart(false);
}

static void sat_layout_enter(intercom_talk)
{

        lv_obj_pressed_func = layout_intercom_talk_touch_callback;
        standby_timer_close();
        intercom_talk_timeout = 30;
        intercom_talk_status_background_display();
        // 满屏查看
        {

                // lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_talk_screen_obj_id, 0, 0, 1024, 600,
                //                          layout_intercom_talk_call_screen_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                //                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                //                          0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                //                          NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);
                lv_obj_add_event_cb(sat_cur_layout_screen_get(), layout_intercom_talk_call_screen_click, LV_EVENT_CLICKED, NULL);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 顶部容器
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_talk_obj_top, 0, 0, 1024, 80,
                                                            NULL, false, LV_OPA_COVER, 0, LV_OPA_TRANSP, 0x808080,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: 标题显示
                 ***********************************************/
                {
                        lv_common_text_create(parent, 0, 32, 25, 850, 42,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
                        intercom_talk_call_info_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: 时间显示
                 ***********************************************/
                {
                        lv_common_text_create(parent, 1, 927, 25, 73, 42,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_RIGHT, lv_font_normal);
                        intercom_talk_call_time_display();
                        lv_sat_timer_create(intercom_talk_call_time_timer, 1000, NULL);
                }
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明:状态文本显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), intercom_talk_obj_status_label, 362, 90, 300, 42,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                intercom_talk_call_status_label_display();
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明:状态文本图标显示
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_talk_obj_status_icon, 303, 142, 418, 314,
                                         NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                intercom_talk_call_status_icon_display();
        }
        {
                lv_obj_t *bottom_cont = lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_talk_call_bottom_cont, 0, 464, 1024, 120,
                                                                 NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                 NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:46:56
                ** 说明:挂断按钮图标显示
                ***********************************************/
                {
                        lv_common_img_btn_create(bottom_cont, intercom_talk_obj_handup, 460, 0, 104, 104,
                                                 intercom_talk_handup_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_endcall.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        intercom_talk_handup_obj_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明:接听按钮图标显示
                 ***********************************************/
                {
                        lv_common_img_btn_create(bottom_cont, intercom_talk_obj_answer, 384, 0, 104, 104,
                                                 intercom_talk_answer_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_call.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                        intercom_talk_answer_obj_display();
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明:音量按钮图标显示
                 ***********************************************/
                {
                        lv_common_img_btn_create(bottom_cont, intercom_talk_obj_volume, 32, 24, 81, 81,
                                                 intercom_talk_call_volume_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_call_sound.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                        intercom_talk_call_volume_obj_display();
                }
        }
        {
                lv_obj_t *vol_cont = lv_common_img_btn_create(sat_cur_layout_screen_get(), intercom_talk_obj_id_vol_cont, 0, 504, 1024, 96,
                                                              NULL, true, LV_OPA_50, 0, LV_OPA_50, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_TOP_MID);

                lv_obj_add_flag(vol_cont, LV_OBJ_FLAG_HIDDEN);
                layout_intercom_talk_vol_bar_create(vol_cont);
        }

        if (user_data_get()->alarm.buzzer_alarm)
        {
                lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), intercom_talk_obj_id_buzzer_call_label);
                if (obj != NULL)
                {
                        lv_label_set_text(obj, lang_str_get(INTERCOM_XLS_LANG_ID_BUZZER_CALL));
                }
                if (obj->user_data)
                {
                        lv_timer_del((lv_timer_t *)obj->user_data);
                }
                int time = user_timestamp_get() - buzzer_call_timestamp_get();
                if (time > 0 && time <= 6000)
                {
                        obj->user_data = lv_sat_timer_create(intercom_talk_buzzer_call_delay_close_task, 6000 - time, obj);
                }
                else
                {
                        if ((user_data_get()->system_mode & 0x0f) == 0x01)
                        {
                                user_data_get()->alarm.buzzer_alarm = false;
                                user_data_save(true, true);
                        }
                }
        }
        buzzer_call_callback_register(intercom_talk_buzzer_alarm_call_callback);

        user_linphone_call_incoming_received_register(monitor_doorcamera_call_inside_func);

        user_linphone_call_streams_connected_receive_register(intercom_talk_call_answer_callback);

        user_linphone_call_busy_register(intercom_talk_call_busy_callback);

        user_linphone_call_error_register(monitor_talk_call_failed_callback);

        user_linphone_call_end_register(monitor_talk_call_end_callback);

        user_linphone_call_outgoing_call_register(layout_monitor_outgoing_arly_media_register);

        user_linphone_call_outgoing_early_media_register(layout_monitor_outgoing_arly_media_register);
}

static void sat_layout_quit(intercom_talk)
{
        buzzer_call_callback_register(buzzer_alarm_trigger_default);
        lv_obj_remove_event_cb(sat_cur_layout_screen_get(), layout_intercom_talk_call_screen_click);
        user_linphone_call_error_register(NULL);
        sat_linphone_audio_play_stop();
        lv_obj_pressed_func = lv_layout_touch_callback;
        standby_timer_restart(true);
        lv_disp_set_bg_image(lv_disp_get_default(), NULL);
        user_linphone_call_busy_register(NULL);
        user_linphone_call_end_register(NULL);
        user_linphone_call_streams_connected_receive_register(NULL);
        user_linphone_call_incoming_received_register(monitor_doorcamera_call_extern_func);
        user_linphone_call_outgoing_early_media_register(NULL);
        user_linphone_call_outgoing_call_register(NULL);
}

sat_layout_create(intercom_talk);