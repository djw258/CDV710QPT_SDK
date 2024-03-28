#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
        setting_volume_obj_id_title,
        setting_volume_obj_id_cancel,

        setting_volume_obj_id_list,

        setting_volume_obj_id_buzzer_cont,
        setting_volume_obj_id_buzzer_label,
        setting_volume_obj_id_buzzer_volume_img,
        setting_volume_obj_id_buzzer_volume_slider_cont,
        setting_volume_obj_id_buzzer_volume_slider_text,
        setting_volume_obj_id_buzzer_volume_slider_left_btn,
        setting_volume_obj_id_buzzer_volume_slider_right_btn,
        setting_volume_obj_id_buzzer_volume_slider,

        setting_volume_obj_id_entrance_cont,
        setting_volume_obj_id_entrance_label,
        setting_volume_obj_id_entrance_volume_img,
        setting_volume_obj_id_entrance_volume_slider_cont,
        setting_volume_obj_id_entrance_volume_slider_text,
        setting_volume_obj_id_entrance_volume_slider_left_btn,
        setting_volume_obj_id_entrance_volume_slider_right_btn,
        setting_volume_obj_id_entrance_volume_slider,
        setting_volume_obj_id_entrance_voice_img,
        setting_volume_obj_id_entrance_voice_slider_cont,
        setting_volume_obj_id_entrance_voice_slider_text,
        setting_volume_obj_id_entrance_voice_slider_left_btn,
        setting_volume_obj_id_entrance_voice_slider_right_btn,
        setting_volume_obj_id_entrance_voice_slider,

        setting_volume_obj_id_common_entrance_cont,
        setting_volume_obj_id_common_entrance_label,
        setting_volume_obj_id_common_entrance_volume_img,
        setting_volume_obj_id_common_entrance_volume_slider_cont,
        setting_volume_obj_id_common_entrance_volume_slider_text,
        setting_volume_obj_id_common_entrance_volume_slider_left_btn,
        setting_volume_obj_id_common_entrance_volume_slider_right_btn,
        setting_volume_obj_id_common_entrance_volume_slider,
        setting_volume_obj_id_common_entrance_voice_img,
        setting_volume_obj_id_common_entrance_voice_slider_cont,
        setting_volume_obj_id_common_entrance_voice_slider_text,
        setting_volume_obj_id_common_entrance_voice_slider_left_btn,
        setting_volume_obj_id_common_entrance_voice_slider_right_btn,
        setting_volume_obj_id_common_entrance_voice_slider,

        setting_volume_obj_id_guard_station_cont,
        setting_volume_obj_id_guard_station_label,
        setting_volume_obj_id_guard_station_volume_img,
        setting_volume_obj_id_guard_station_volume_slider_cont,
        setting_volume_obj_id_guard_station_volume_slider_text,
        setting_volume_obj_id_guard_station_volume_slider_left_btn,
        setting_volume_obj_id_guard_station_volume_slider_right_btn,
        setting_volume_obj_id_guard_station_volume_slider,
        setting_volume_obj_id_guard_station_voice_img,
        setting_volume_obj_id_guard_station_voice_slider_cont,
        setting_volume_obj_id_guard_station_voice_slider_text,
        setting_volume_obj_id_guard_station_voice_slider_left_btn,
        setting_volume_obj_id_guard_station_voice_slider_right_btn,
        setting_volume_obj_id_guard_station_voice_slider,

        setting_volume_obj_id_extension_cont,
        setting_volume_obj_id_extension_label,
        setting_volume_obj_id_extension_volume_img,
        setting_volume_obj_id_extension_volume_slider_cont,
        setting_volume_obj_id_extension_volume_slider_text,
        setting_volume_obj_id_extension_volume_slider_left_btn,
        setting_volume_obj_id_extension_volume_slider_right_btn,
        setting_volume_obj_id_extension_volume_slider,
        setting_volume_obj_id_extension_voice_img,
        setting_volume_obj_id_extension_voice_slider_cont,
        setting_volume_obj_id_extension_voice_slider_text,
        setting_volume_obj_id_extension_voice_slider_left_btn,
        setting_volume_obj_id_extension_voice_slider_right_btn,
        setting_volume_obj_id_extension_voice_slider,

        setting_volume_obj_id_touch_notification_cont,
        setting_volume_obj_id_touch_notification_label,
        setting_volume_obj_id_touch_notification_volume_img,
        setting_volume_obj_id_touch_notification_volume_slider_cont,
        setting_volume_obj_id_touch_notification_volume_slider_text,
        setting_volume_obj_id_touch_notification_volume_slider_left_btn,
        setting_volume_obj_id_touch_notification_volume_slider_right_btn,
        setting_volume_obj_id_touch_notification_volume_slider,
        setting_volume_obj_id_touch_notification_voice_img,
        setting_volume_obj_id_touch_notification_voice_slider_cont,
        setting_volume_obj_id_touch_notification_voice_slider_text,
        setting_volume_obj_id_touch_notification_voice_slider_left_btn,
        setting_volume_obj_id_touch_notification_voice_slider_right_btn,
        setting_volume_obj_id_touch_notification_voice_slider,
};

static bool is_setting_volume_ring_play_runing = false;
static int playing_index;

static void setting_volume_cancel_obj_click(lv_event_t *e)
{
        sat_layout_goto(setting_sound, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void setting_buzzer_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        int y = lv_obj_get_style_y(lv_obj_get_parent(obj), LV_PART_MAIN);
        if (y == 8) // 调铃声音量
        {
                int value = lv_slider_get_value(obj);
                sat_linphone_audio_play_volume_set(value);
                user_data_get()->audio.buzzer_volume = value;
                user_data_save(false, false);

                if ((user_data_get()->audio.buzzer_volume != 0))
                {
                        if ((playing_index != 0) || is_setting_volume_ring_play_runing == false)
                                ring_buzzer_play(user_data_get()->audio.buzzer_tone);
                }
                else if ((is_setting_volume_ring_play_runing == true) && (user_data_get()->audio.buzzer_volume == 0))
                {
                        sat_linphone_audio_play_stop();
                        is_setting_volume_ring_play_runing = false;
                }
                playing_index = 0;

                // printf("setting valume is %d\n",value);
        }
        else if (y == 78) // 调通话声音
        {
        }
}

static void setting_buzzer_volume_slider_display(lv_obj_t *parent)
{
        lv_obj_t *slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_buzzer_cont), setting_volume_obj_id_buzzer_volume_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_buzzer_volume_slider);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_buzzer_volume_slider_text);
        int cur_volume = user_data_get()->audio.buzzer_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
        user_data_save(false, false);
}

static void setting_entrance_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        int y = lv_obj_get_style_y(lv_obj_get_parent(obj), LV_PART_MAIN);

        if (y == 8) // 调铃声音量
        {

                int value = lv_slider_get_value(obj);
                sat_linphone_audio_play_volume_set(value);
                user_data_get()->audio.entrance_volume = value;
                user_data_save(false, false);
                if ((user_data_get()->audio.entrance_volume != 0))
                {
                        if ((playing_index != 1) || is_setting_volume_ring_play_runing == false)
                                ring_door_call_play(user_data_get()->audio.door_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                }
                else if ((is_setting_volume_ring_play_runing == true) && (user_data_get()->audio.entrance_volume == 0))
                {
                        sat_linphone_audio_play_stop();
                        is_setting_volume_ring_play_runing = false;
                }
                playing_index = 1;
        }
        else if (y == 78) // 调通话声音
        {
                int value = lv_slider_get_value(obj);
                user_data_get()->audio.entrance_voice = value;
                user_data_save(false, false);
        }
}
static void setting_entrance_volume_slider_display(lv_obj_t *parent)
{
        lv_obj_t *slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_entrance_cont), setting_volume_obj_id_entrance_volume_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_entrance_volume_slider);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_entrance_volume_slider_text);
        int cur_volume = user_data_get()->audio.entrance_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);

        slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_entrance_cont), setting_volume_obj_id_entrance_voice_slider_cont);
        slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_entrance_voice_slider);
        value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_entrance_voice_slider_text);
        cur_volume = user_data_get()->audio.entrance_voice;

        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static void setting_common_entrance_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        int y = lv_obj_get_style_y(lv_obj_get_parent(obj), LV_PART_MAIN);

        if (y == 8) // 调铃声音量
        {
                int value = lv_slider_get_value(obj);
                sat_linphone_audio_play_volume_set(value);
                user_data_get()->audio.common_entrance_volume = value;
                user_data_save(false, false);
                if ((user_data_get()->audio.common_entrance_volume != 0))
                {
                        if ((playing_index != 2) || is_setting_volume_ring_play_runing == false)
                                ring_common_door_play(user_data_get()->audio.common_entrance_tone, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                }
                else if ((is_setting_volume_ring_play_runing == true) && (user_data_get()->audio.common_entrance_volume == 0))
                {
                        sat_linphone_audio_play_stop();
                        is_setting_volume_ring_play_runing = false;
                }
                playing_index = 2;
        }
        else if (y == 78) // 调通话声音
        {
                int value = lv_slider_get_value(obj);
                user_data_get()->audio.common_entrance_voice = value;
                user_data_save(false, false);
        }
}

static void setting_common_entrance_volume_slider_display(lv_obj_t *parent)
{
        lv_obj_t *obj_cont = lv_obj_get_child_form_id(parent, setting_volume_obj_id_common_entrance_cont);
        if (obj_cont == NULL)
        {
                return;
        }
        lv_obj_t *slider_cont = lv_obj_get_child_form_id(obj_cont, setting_volume_obj_id_common_entrance_volume_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_common_entrance_volume_slider);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_common_entrance_volume_slider_text);
        int cur_volume = user_data_get()->audio.common_entrance_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);

        slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_common_entrance_cont), setting_volume_obj_id_common_entrance_voice_slider_cont);
        slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_common_entrance_voice_slider);
        value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_common_entrance_voice_slider_text);
        cur_volume = user_data_get()->audio.common_entrance_voice;

        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static void setting_guard_station_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        int y = lv_obj_get_style_y(lv_obj_get_parent(obj), LV_PART_MAIN);

        if (y == 8) // 调铃声音量
        {

                int value = lv_slider_get_value(obj);
                sat_linphone_audio_play_volume_set(value);
                user_data_get()->audio.guard_station_volume = value;
                user_data_save(false, false);
                if ((user_data_get()->audio.guard_station_volume != 0))
                {
                        if ((playing_index != 3) || is_setting_volume_ring_play_runing == false)
                                ring_guard_play(user_data_get()->audio.securirty_office_tone, 1);
                }
                else if ((is_setting_volume_ring_play_runing == true) && (user_data_get()->audio.guard_station_volume == 0))
                {
                        sat_linphone_audio_play_stop();
                        is_setting_volume_ring_play_runing = false;
                }
                playing_index = 3;
        }
        else if (y == 78) // 调通话声音
        {
                int value = lv_slider_get_value(obj);
                user_data_get()->audio.guard_station_voice = value;
                user_data_save(false, false);
        }
}

static void setting_guard_station_slider_display(lv_obj_t *parent)
{
        lv_obj_t *obj_cont = lv_obj_get_child_form_id(parent, setting_volume_obj_id_guard_station_cont);
        if (obj_cont == NULL)
        {
                return;
        }
        lv_obj_t *slider_cont = lv_obj_get_child_form_id(obj_cont, setting_volume_obj_id_guard_station_volume_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_guard_station_volume_slider);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_guard_station_volume_slider_text);
        int cur_volume = user_data_get()->audio.guard_station_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);

        slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_guard_station_cont), setting_volume_obj_id_guard_station_voice_slider_cont);
        slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_guard_station_voice_slider);
        value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_guard_station_voice_slider_text);
        cur_volume = user_data_get()->audio.guard_station_voice;

        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static void setting_extension_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        int y = lv_obj_get_style_y(lv_obj_get_parent(obj), LV_PART_MAIN);

        if (y == 8) // 调铃声音量
        {
                int value = lv_slider_get_value(obj);
                sat_linphone_audio_play_volume_set(value);
                user_data_get()->audio.extension_volume = value;
                user_data_save(false, false);
                if ((user_data_get()->audio.extension_volume != 0))
                {
                        if ((playing_index != 4) || is_setting_volume_ring_play_runing == false)
                                ring_intercom_play(user_data_get()->audio.extenion_tone, 1);
                }
                else if ((is_setting_volume_ring_play_runing == true) && (user_data_get()->audio.extension_volume == 0))
                {
                        sat_linphone_audio_play_stop();
                        is_setting_volume_ring_play_runing = false;
                }
                playing_index = 4;
        }
        else if (y == 78) // 调通话声音
        {
                int value = lv_slider_get_value(obj);
                user_data_get()->audio.extension_voice = value;
                user_data_save(false, false);
        }
}

static void setting_extension_volume_slider_display(lv_obj_t *parent)
{
        lv_obj_t *slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_extension_cont), setting_volume_obj_id_extension_volume_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_extension_volume_slider);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_extension_volume_slider_text);
        int cur_volume = user_data_get()->audio.extension_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);

        slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_extension_cont), setting_volume_obj_id_extension_voice_slider_cont);
        slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_extension_voice_slider);
        value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_extension_voice_slider_text);
        cur_volume = user_data_get()->audio.extension_voice;

        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static void setting_touch_notification_volume_slider_change_cb(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_current_target(e);

        int y = lv_obj_get_style_y(lv_obj_get_parent(obj), LV_PART_MAIN);

        if (y == 8) // 调铃声音量
        {
                int value = lv_slider_get_value(obj);
                sat_linphone_audio_play_volume_set(value);
                user_data_get()->audio.touch_notification_volume = value;
                user_data_save(false, false);
                if ((user_data_get()->audio.touch_notification_volume != 0))
                {
                        if ((playing_index != 5) || is_setting_volume_ring_play_runing == false)
                                ring_touch_play();
                }
                else if ((is_setting_volume_ring_play_runing == true) && (user_data_get()->audio.touch_notification_volume == 0))
                {
                        sat_linphone_audio_play_stop();
                        is_setting_volume_ring_play_runing = false;
                }
                playing_index = 5;
        }
        else if (y == 78) // 调通话声音
        {
                int value = lv_slider_get_value(obj);
                user_data_get()->audio.touch_notification_voice = value;
                user_data_save(false, false);
        }
}
static void setting_touch_notification_slider_display(lv_obj_t *parent)
{
        lv_obj_t *slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_touch_notification_cont), setting_volume_obj_id_touch_notification_volume_slider_cont);
        lv_obj_t *slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_touch_notification_volume_slider);
        lv_obj_t *value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_touch_notification_volume_slider_text);
        int cur_volume = user_data_get()->audio.touch_notification_volume;
        char value_str[32] = {0};
        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);

        slider_cont = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_volume_obj_id_touch_notification_cont), setting_volume_obj_id_touch_notification_voice_slider_cont);
        if (slider_cont == NULL)
        {
                return;
        }
        slider_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_touch_notification_voice_slider);
        value_obj = lv_obj_get_child_form_id(slider_cont, setting_volume_obj_id_touch_notification_voice_slider_text);
        cur_volume = user_data_get()->audio.touch_notification_voice;

        sprintf(value_str, "%02d", cur_volume);
        lv_bar_set_value(slider_obj, cur_volume, LV_ANIM_OFF);
        lv_label_set_text(value_obj, value_str);
}

static lv_obj_t *setting_volume_slider_obj_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 970, 70, setting_volume_obj_id_buzzer_cont, setting_volume_obj_id_buzzer_label, -1, SOUND_XLS_LANG_ID_BUZZER, lang_str_get, -1, NULL, setting_buzzer_volume_slider_change_cb, -1},
            {0, 70, 970, 140, setting_volume_obj_id_entrance_cont, setting_volume_obj_id_entrance_label, -1, SOUND_XLS_LANG_ID_FRONT_DOOR, lang_str_get, -1, NULL, setting_entrance_volume_slider_change_cb, -1},
            {0, 70 + 140, 970, 140, setting_volume_obj_id_common_entrance_cont, setting_volume_obj_id_common_entrance_label, -1, SOUND_XLS_LANG_ID_COMMON_ENTRANCE, lang_str_get, -1, NULL, setting_common_entrance_volume_slider_change_cb, -1},
            {0, 70 + 140 * 2, 970, 140, setting_volume_obj_id_guard_station_cont, setting_volume_obj_id_guard_station_label, -1, SOUND_XLS_LANG_ID_GUARD_STATION, lang_str_get, -1, NULL, setting_guard_station_volume_slider_change_cb, -1},
            {0, 70 + 140 * 3, 970, 140, setting_volume_obj_id_extension_cont, setting_volume_obj_id_extension_label, -1, SOUND_XLS_LANG_ID_EXTENSION, lang_str_get, -1, NULL, setting_extension_volume_slider_change_cb, -1},
            {0, 70 + 140 * 4, 970, 70, setting_volume_obj_id_touch_notification_cont, setting_volume_obj_id_touch_notification_label, -1, SOUND_XLS_LANG_ID_TOUCH_NOTIFICATION, lang_str_get, -1, NULL, setting_touch_notification_volume_slider_change_cb, -1},
        };
        int slider_volume_group[][6] = {

            {setting_volume_obj_id_buzzer_volume_img,
             setting_volume_obj_id_buzzer_volume_slider_cont,
             setting_volume_obj_id_buzzer_volume_slider_text,
             setting_volume_obj_id_buzzer_volume_slider_left_btn,
             setting_volume_obj_id_buzzer_volume_slider_right_btn,
             setting_volume_obj_id_buzzer_volume_slider},

            {setting_volume_obj_id_entrance_volume_img,
             setting_volume_obj_id_entrance_volume_slider_cont,
             setting_volume_obj_id_entrance_volume_slider_text,
             setting_volume_obj_id_entrance_volume_slider_left_btn,
             setting_volume_obj_id_entrance_volume_slider_right_btn,
             setting_volume_obj_id_entrance_volume_slider},

            {setting_volume_obj_id_common_entrance_volume_img,
             setting_volume_obj_id_common_entrance_volume_slider_cont,
             setting_volume_obj_id_common_entrance_volume_slider_text,
             setting_volume_obj_id_common_entrance_volume_slider_left_btn,
             setting_volume_obj_id_common_entrance_volume_slider_right_btn,
             setting_volume_obj_id_common_entrance_volume_slider},

            {setting_volume_obj_id_guard_station_volume_img,
             setting_volume_obj_id_guard_station_volume_slider_cont,
             setting_volume_obj_id_guard_station_volume_slider_text,
             setting_volume_obj_id_guard_station_volume_slider_left_btn,
             setting_volume_obj_id_guard_station_volume_slider_right_btn,
             setting_volume_obj_id_guard_station_volume_slider},

            {setting_volume_obj_id_extension_volume_img,
             setting_volume_obj_id_extension_volume_slider_cont,
             setting_volume_obj_id_extension_volume_slider_text,
             setting_volume_obj_id_extension_volume_slider_left_btn,
             setting_volume_obj_id_extension_volume_slider_right_btn,
             setting_volume_obj_id_extension_volume_slider},

            {setting_volume_obj_id_touch_notification_volume_img,
             setting_volume_obj_id_touch_notification_volume_slider_cont,
             setting_volume_obj_id_touch_notification_volume_slider_text,
             setting_volume_obj_id_touch_notification_volume_slider_left_btn,
             setting_volume_obj_id_touch_notification_volume_slider_right_btn,
             setting_volume_obj_id_touch_notification_volume_slider},
        };
        int slider_voice_group[][6] = {

            {-1, -1, -1, -1, -1, -1},
            {setting_volume_obj_id_entrance_voice_img,
             setting_volume_obj_id_entrance_voice_slider_cont,
             setting_volume_obj_id_entrance_voice_slider_text,
             setting_volume_obj_id_entrance_voice_slider_left_btn,
             setting_volume_obj_id_entrance_voice_slider_right_btn,
             setting_volume_obj_id_entrance_voice_slider},

            {setting_volume_obj_id_common_entrance_voice_img,
             setting_volume_obj_id_common_entrance_voice_slider_cont,
             setting_volume_obj_id_common_entrance_voice_slider_text,
             setting_volume_obj_id_common_entrance_voice_slider_left_btn,
             setting_volume_obj_id_common_entrance_voice_slider_right_btn,
             setting_volume_obj_id_common_entrance_voice_slider},

            {setting_volume_obj_id_guard_station_voice_img,
             setting_volume_obj_id_guard_station_voice_slider_cont,
             setting_volume_obj_id_guard_station_voice_slider_text,
             setting_volume_obj_id_guard_station_voice_slider_left_btn,
             setting_volume_obj_id_guard_station_voice_slider_right_btn,
             setting_volume_obj_id_guard_station_voice_slider},

            {setting_volume_obj_id_extension_voice_img,
             setting_volume_obj_id_extension_voice_slider_cont,
             setting_volume_obj_id_extension_voice_slider_text,
             setting_volume_obj_id_extension_voice_slider_left_btn,
             setting_volume_obj_id_extension_voice_slider_right_btn,
             setting_volume_obj_id_extension_voice_slider},

            {-1, -1, -1, -1, -1, -1},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_volume_obj_id_list);
        lv_common_style_set_common(list, setting_volume_obj_id_list, 27, 88, 970, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        void *left_src = resource_ui_src_alloc("btn_control_minus.png", 42, 42);
        void *right_src = resource_ui_src_alloc("btn_control_plus.png", 42, 42);
        int j = 0;
        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                // if (((user_data_get()->system_mode & 0xF0) != 0x10) && (i == 2 || i == 3))
                // {
                //         continue;
                // }
                lv_obj_t *item = lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[i].w, main_list_group[i].h,
                                                                                 NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                                 0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                                 0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                                 0, (i == 0 || i == 5) ? 17 : 52, 576, 43, main_list_group[i].title_id,
                                                                                 main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                 0, 42, 576, 29, main_list_group[i].sub_id,
                                                                                 main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                                 0, 0, 0, 0, -1,
                                                                                 NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                 0, 0, 0, 0, -1,
                                                                                 NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_btn_create(item, slider_volume_group[i][0], 297, 14, 42, 42,
                                         NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("volume.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_slider_create(item, slider_volume_group[i][1], 255, 8, 678, 54,
                                        main_list_group[i].click_cb, LV_OPA_TRANSP, 0X00,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        137, 8, 40, 38, slider_volume_group[i][2],
                                        "50", 0Xffffff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                        223, 22, 328, 10, slider_volume_group[i][5], LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                        165, 6, 42, 42, slider_volume_group[i][3],
                                        left_src, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                        567, 6, 42, 42, slider_volume_group[i][4],
                                        right_src, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                        360, 9, 0Xffffff, LV_OPA_COVER, NULL,
                                        0, 100, 50);

                if (slider_voice_group[i][0] > 0)
                {
                        lv_common_img_btn_create(item, slider_voice_group[i][0], 297, 84, 42, 42,
                                                 NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("voice.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                        lv_common_slider_create(item, slider_voice_group[i][1], 255, 78, 678, 54,
                                                main_list_group[i].click_cb, LV_OPA_TRANSP, 0X00,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                137, 8, 40, 38, slider_voice_group[i][2],
                                                "50", 0Xffffff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                223, 22, 328, 10, slider_voice_group[i][5], LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                                165, 6, 42, 42, slider_voice_group[i][3],
                                                left_src, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                                567, 6, 42, 42, slider_voice_group[i][4],
                                                right_src, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                                360, 9, 0Xffffff, LV_OPA_COVER, NULL,
                                                0, 100, 50);
                }
                j++;
        }
        setting_buzzer_volume_slider_display(list);
        setting_entrance_volume_slider_display(list);
        setting_common_entrance_volume_slider_display(list);
        setting_guard_station_slider_display(list);
        setting_extension_volume_slider_display(list);
        setting_touch_notification_slider_display(list);

        resouce_file_src_free(left_src);
        resouce_file_src_free(right_src);

        return list;
}

static bool setting_volume_ring_play_callback(int arg)
{
        is_setting_volume_ring_play_runing = arg == 0 ? true : false;
        return true;
}
static void layout_setting_touch_callback(lv_event_t *e)
{
        standby_timer_restart(false, true);
}
static void sat_layout_enter(setting_volume)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_volume_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SOUND_XLS_LANG_ID_VOLUME), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 上一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_volume_obj_id_cancel, 35, 15, 48, 48,
                                         setting_volume_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        lv_obj_pressed_func = layout_setting_touch_callback;
        setting_volume_slider_obj_create();

        ring_play_event_cmd_register(setting_volume_ring_play_callback);
}
static void sat_layout_quit(setting_volume)
{
        ring_play_event_cmd_register(NULL);
        sat_linphone_audio_play_stop();
        is_setting_volume_ring_play_runing = false;
        lv_obj_pressed_func = lv_layout_touch_callback;
}

sat_layout_create(setting_volume);