#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_away_count.h"
typedef enum
{
    layout_away_count_obj_id_bg,
    layout_away_count_obj_id_end_btn,
    layout_away_count_obj_id_end_btn_label,
    layout_away_count_obj_id_end_time_label,
    layout_away_count_obj_id_main_bg,
    layout_away_count_obj_id_passwd_cont,

} layout_away_count_scr_act_obj_id;

typedef enum
{
    layout_away_count_obj_id_main_bg_tips,
} layout_away_count_main_bg_obj_id;

static layout_away_count_info layout_away_count_default_info = {
    .away_setting_time_countdown_timer = NULL,
    .away_release_time_countdown_timer = NULL,
    .away_release_time[0] = 0,
    .away_release_time[1] = 0,
    .away_release_time[2] = 0,
    .away_release_time[3] = 0,
    .away_release_time[4] = 0,
    .away_release_time[5] = 0,
    .away_release_time[6] = 0,
    .away_count_sec = 0};

layout_away_count_info *layout_away_count_data_get(void)
{
    return &layout_away_count_default_info;
}
/************************************************************
** 函数说明: 密码校验成功，把标志清了，重新刷新ui
** 作者: xiaoxiao
** 日期: 2023-08-05 17:23:49
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_count_passwd_check_success_cb(void)
{
    user_data_get()->alarm.away_setting_countdown = false;

    lv_timer_del(layout_away_count_data_get()->away_setting_time_countdown_timer);
    layout_away_count_data_get()->away_setting_time_countdown_timer = NULL;
    layout_away_count_data_get()->away_count_sec = 0;
    user_data_save(true, true);
    if ((user_data_get()->system_mode & 0x0f) != 0x01)
    {
        sat_ipcamera_data_sync(0x00, 0x04, (char *)user_data_get(), sizeof(user_data_info), 10, 1500, NULL);
    }

    sat_layout_goto(away, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/************************************************************
** 函数说明: 离家模式设防倒计时结束按键事件
** 作者: xiaoxiao
** 日期: 2023-05-04 19:52:19
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_count_end_btn_obj_click(lv_event_t *ev)
{
    lv_obj_t *keyboard = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_count_obj_id_passwd_cont);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/************************************************************
** 函数说明: 倒计时显示
** 作者: xiaoxiao
** 日期: 2023-05-04 21:45:35
** 参数说明:
** 注意事项:
************************************************************/
static void layout_away_count_timer_obj_display(void)
{

    lv_obj_t *label_time = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), layout_away_count_obj_id_end_time_label);
    if (label_time == NULL)
    {
        return;
    }
    lv_label_set_text_fmt(label_time, "%02d:%02d", layout_away_count_data_get()->away_count_sec / 60, layout_away_count_data_get()->away_count_sec % 60);
}

/************************************************************
** 函数说明: 离家设防缓冲时间结束检查是否需要触发警报任务
** 作者: xiaoxiao
** 日期：2023-10-12 16:43:57
** 参数说明:
** 注意事项：
************************************************************/
static void layout_away_alarm_release_detetion_timer(lv_timer_t *ptimer)
{
    for (int i = 0; i < 7; i++)
    {
        if (user_data_get()->alarm.alarm_trigger[i] == true && user_data_get()->alarm.alarm_trigger_enable[i] == false)
        {
            if (layout_away_count_data_get()->away_release_time[i]++ == user_data_get()->alarm.away_release_time)
            {
                layout_common_call_log(security_emergency, i);
                user_data_get()->alarm.alarm_trigger_enable[i] = true;
                user_data_save(true, true);
                if (sat_cur_layout_get() != sat_playout_get(alarm))
                {
                    layout_alarm_alarm_channel_set(i);
                    user_data_get()->alarm.emergency_mode = 1;
                    user_data_get()->alarm.alarm_ring_play = true;
                    sat_linphone_handup(0xFFFF);
                    sat_layout_goto(alarm, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                }
            }
        }
    }
}
/************************************************************
** 函数说明: 离家模式下的传感器回调事件
** 作者: xiaoxiao
** 日期：2023-11-02 09:33:38
** 参数说明:
** 注意事项：
************************************************************/
void away_mode_alarm_trigger_callback(int arg1, int arg2)
{
    if ((arg1 == 7) && (arg2 < ALM_LOW))
    {
        if (user_data_get()->alarm.buzzer_alarm == false)
        {
            user_data_get()->alarm.buzzer_alarm = true;
            user_data_save(true, true);
            buzzer_call_trigger_check();
        }
    }
    else
    {
        if ((!(user_data_get()->alarm.away_alarm_enable_list & (0x01 << arg1))) && (!(user_data_get()->alarm.security_alarm_enable_list & (0x01 << arg1))))
        {
            return;
        }
        if ((user_data_get()->alarm.alarm_enable_always[arg1] == false))
        {
            if (user_data_get()->alarm.away_alarm_enable == false && user_data_get()->alarm.security_alarm_enable == false)
            {
                return;
            }
        }
        if ((user_data_get()->alarm.alarm_enable[arg1] == 1 && arg2 < ALM_LOW) || (user_data_get()->alarm.alarm_enable[arg1] == 2 && arg2 > ALM_HIGHT))
        {
            if (user_data_get()->alarm.alarm_trigger[arg1] == false)
            {
                user_data_get()->alarm.alarm_trigger[arg1] = true;
                if (user_data_get()->alarm.alarm_enable_always[arg1])
                {
                    layout_common_call_log(security_emergency, arg1);
                    if (sat_cur_layout_get() != sat_playout_get(alarm))
                    {
                        layout_alarm_alarm_channel_set(arg1);
                        user_data_get()->alarm.emergency_mode = 1;
                        user_data_get()->alarm.alarm_ring_play = true;
                        user_data_save(true, true);
                        sat_linphone_handup(0xFFFF);
                        sat_layout_goto(alarm, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                    }
                }
                else
                {

                    layout_away_count_data_get()->away_release_time[arg1] = 0;
                    if (user_data_get()->alarm.away_release_time == 0)
                    {
                        layout_away_alarm_release_detetion_timer(NULL);
                    }
                }
                user_data_save(true, true);
            }
        }
    }
}

/************************************************************
** 函数说明: 离家模式设防检测任务创建
** 作者: xiaoxiao
** 日期：2023-11-02 18:13:36
** 参数说明:
** 注意事项：
************************************************************/
void away_mode_alarm_trigger_timer_create(void)
{
    if ((user_data_get()->system_mode & 0x1f) == 0x01)
    {
        if (layout_away_count_data_get()->away_release_time_countdown_timer == NULL)
        {
            if (user_data_get()->alarm.away_release_time)
            {
                lv_timer_ready(layout_away_count_data_get()->away_release_time_countdown_timer = lv_timer_create(layout_away_alarm_release_detetion_timer, 1000, NULL));
            }
            else
            {
                layout_away_alarm_release_detetion_timer(NULL);
            }
        }
    }
}

/************************************************************
** 函数说明: 离家模式设防setting time倒计时定时器任务
** 作者: xiaoxiao
** 日期：2023-11-02 18:13:36
** 参数说明:
** 注意事项：
************************************************************/
static void layout_away_count_timer(lv_timer_t *ptimer)
{
    if (layout_away_count_data_get()->away_count_sec != 0)
    {
        layout_away_count_data_get()->away_count_sec--;
        if (sat_cur_layout_get() == sat_playout_get(away_count))
        {
            layout_away_count_timer_obj_display();
        }
    }
    else
    {
        lv_timer_del(layout_away_count_data_get()->away_setting_time_countdown_timer);
        layout_away_count_data_get()->away_setting_time_countdown_timer = NULL;
        user_data_get()->alarm.away_alarm_enable = true;

        for (int i = 0; i < 7; i++)
        {
            if (user_data_get()->alarm.away_alarm_enable_list & 0x01 << i)
            {
                float value = user_sensor_value_get(i);
                if ((user_data_get()->alarm.alarm_enable[i] == 1 && value < ALM_LOW) || (user_data_get()->alarm.alarm_enable[i] == 2 && value > ALM_HIGHT))
                {
                    user_data_get()->alarm.alarm_trigger[i] = true;
                }
            }
        }
        user_data_save(true, true);
        away_mode_alarm_trigger_timer_create();
        if (sat_cur_layout_get() == sat_playout_get(away_count))
        {
            sat_layout_goto(away, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
    }
}

/************************************************************
** 函数说明: 离家设防缓冲页面参数初始化
** 作者: xiaoxiao
** 日期: 2023-05-06 08:25:05
** 参数说明:
** 注意事项:
************************************************************/
static void layout_alarm_count_param_init(void)
{

    if (layout_away_count_data_get()->away_release_time_countdown_timer != NULL)
    {
        lv_timer_del(layout_away_count_data_get()->away_release_time_countdown_timer);
        layout_away_count_data_get()->away_release_time_countdown_timer = NULL;
    }
    if (layout_away_count_data_get()->away_count_sec == 0)
    {
        layout_away_count_data_get()->away_count_sec = user_data_get()->alarm.away_setting_time * 60;
    }
}

static void sat_layout_enter(away_count)
{
    memset(&user_data_get()->alarm.alarm_trigger_enable, 0, sizeof(user_data_get()->alarm.alarm_trigger_enable));

    alarm_sensor_cmd_register(away_mode_alarm_trigger_callback); // 警报回调注册

    standby_timer_close();
    /************************************************************
    ** 函数说明:离家设防缓冲页面参数初始化
    ** 作者: xiaoxiao
    ** 日期: 2023-05-06 08:30:13
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        layout_alarm_count_param_init();
    }
    /************************************************************
    ** 函数说明: 离家设防倒计时背景创建
    ** 作者: xiaoxiao
    ** 日期: 2023-05-04 19:55:11
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_btn_create(sat_cur_layout_screen_get(), layout_away_count_obj_id_bg, 0, 0, 1024, 600,
                                 NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }

    /************************************************************
    ** 函数说明: 背景图标
    ** 作者: xiaoxiao
    ** 日期: 2023-05-04 21:04:31
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), layout_away_count_obj_id_main_bg, 0, 0, 1024, 500,
                                      NULL, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 250, 1024, 46, layout_away_count_obj_id_main_bg_tips,
                                      lang_str_get(LAYOUT_AWAY_XLS_LANG_ID_AWAY), 0XFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                      0, 0, 1024, 300, -1,
                                      resource_ui_src_get("img_away.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }

    /************************************************************
    ** 函数说明: 离家模式设防倒计时显示
    ** 作者: xiaoxiao
    ** 日期: 2023-05-04 19:59:05
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        // 时间
        lv_common_text_create(sat_cur_layout_screen_get(), layout_away_count_obj_id_end_time_label, 0, 336, 1024, 134,
                              NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                              NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_plus);
        layout_away_count_timer_obj_display();
        if (layout_away_count_data_get()->away_setting_time_countdown_timer == NULL)
        {
            layout_away_count_data_get()->away_setting_time_countdown_timer = lv_timer_create(layout_away_count_timer, 1000, NULL);
        }
    }

    /************************************************************
    ** 函数说明: 离家模式设防结束倒计时按键创建
    ** 作者: xiaoxiao
    ** 日期: 2023-05-04 19:43:18
    ** 参数说明:
    ** 注意事项:
    ************************************************************/
    {
        lv_common_img_text_btn_create(sat_cur_layout_screen_get(), layout_away_count_obj_id_end_btn, 0, 528, 1024, 72,
                                      layout_away_count_end_btn_obj_click, LV_OPA_COVER, 0x00A8FF, LV_OPA_COVER, 0X101010,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 17, 1024, 46, layout_away_count_obj_id_end_btn_label,
                                      lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_END), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                      3, 0, 77, 77, -1,
                                      NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
    }
    common_passwd_check_func_create(layout_away_count_obj_id_passwd_cont, layout_away_count_passwd_check_success_cb);
}

static void sat_layout_quit(away_count)
{
    standby_timer_restart(true, true);
}

sat_layout_create(away_count);