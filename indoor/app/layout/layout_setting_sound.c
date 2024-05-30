#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
        setting_sound_obj_id_title,
        setting_sound_obj_id_cancel,
        setting_sound_obj_id_main_list,

        setting_sound_obj_id_sub_list,

        setting_sound_obj_id_volume_cont,
        setting_sound_obj_id_volume_title,
        setting_sound_obj_id_volume_sub,

        setting_sound_obj_id_ring_repeat_cont,
        setting_sound_obj_id_ring_repeat_title,
        setting_sound_obj_id_ring_repeat_sub,

        setting_sound_obj_id_buzzer_cont,
        setting_sound_obj_id_buzzer_title,
        setting_sound_obj_id_buzzer_sub,

        setting_sound_obj_id_front_door_cont,
        setting_sound_obj_id_front_door_title,
        setting_sound_obj_id_front_door_sub,

        setting_sound_obj_id_common_entrance_cont,
        setting_sound_obj_id_common_entrance_title,
        setting_sound_obj_id_common_entrance_sub,

        setting_sound_obj_id_security_office_cont,
        setting_sound_obj_id_security_office_title,
        setting_sound_obj_id_security_office_sub,

        setting_sound_obj_id_extension_cont,
        setting_sound_obj_id_extension_title,
        setting_sound_obj_id_extension_sub,

        setting_sound_obj_id_msgbox_parent,
        setting_sound_obj_id_msgbox_cont,
        setting_sound_obj_id_msgbox_titile,

        setting_sound_obj_id_msgbox_list,

        setting_sound_obj_id_msgbox_cancel,
        setting_sound_obj_id_msgbox_confirm,

};
static int select_id;
int layout_setting_sound_select_id_get()
{
        return select_id;
}

void layout_setting_sound_select_id_set(int id)
{
        select_id = id;
}

static void setting_sound_volume_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_volume, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void setting_sound_ring_msgbox_list_click(lv_event_t *ev)
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
        if (1 /*strncmp(item_img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png")))*/)
        {
                layout_setting_sound_select_id_get();
                int id = layout_setting_sound_select_id_get();
                if (id == setting_sound_obj_id_buzzer_cont)
                {
                        ring_buzzer_play(item->id + 1);
                }
                else if (id == setting_sound_obj_id_front_door_cont)
                {
                        ring_door_call_play(item->id + 1, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                }
                else if (id == setting_sound_obj_id_common_entrance_cont)
                {
                        ring_common_door_play(item->id + 1, user_data_get()->audio.ring_repeat == 0 ? 1 : 0xfffff);
                }
                else if (id == setting_sound_obj_id_security_office_cont)
                {
                        ring_guard_play(item->id + 1, 1);
                }
                else if (id == setting_sound_obj_id_extension_cont)
                {
                        ring_intercom_play(item->id + 1, 1);
                }
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
static void setting_sound_ring_msgbox_cancel_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_sound_obj_id_msgbox_parent);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}

static void layout_setting_sound_sub_title_display(lv_obj_t *parent)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, setting_sound_obj_id_ring_repeat_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_t *sub_obj = lv_obj_get_child_form_id(obj, setting_sound_obj_id_ring_repeat_sub);
        if (sub_obj == NULL)
        {
                return;
        }
        lv_label_set_text(sub_obj, lang_str_get(SOUND_XLS_LANG_ID_RINGTONE_ONETIME + user_data_get()->audio.ring_repeat));
        obj = lv_obj_get_child_form_id(parent, setting_sound_obj_id_buzzer_cont);
        if (obj == NULL)
        {
                return;
        }
        sub_obj = lv_obj_get_child_form_id(obj, setting_sound_obj_id_buzzer_sub);
        if (sub_obj == NULL)
        {
                return;
        }
        lv_label_set_text(sub_obj, lang_str_get(SOUND_XLS_LANG_ID_RINGTONE1 - 1 + user_data_get()->audio.buzzer_tone));
        obj = lv_obj_get_child_form_id(parent, setting_sound_obj_id_front_door_cont);
        if (obj == NULL)
        {
                return;
        }
        sub_obj = lv_obj_get_child_form_id(obj, setting_sound_obj_id_front_door_sub);
        if (sub_obj == NULL)
        {
                return;
        }
        lv_label_set_text(sub_obj, lang_str_get(SOUND_XLS_LANG_ID_RINGTONE1 - 1 + user_data_get()->audio.door_tone));
        obj = lv_obj_get_child_form_id(parent, setting_sound_obj_id_common_entrance_cont);
        if (obj == NULL)
        {
                return;
        }
        sub_obj = lv_obj_get_child_form_id(obj, setting_sound_obj_id_common_entrance_sub);
        if (sub_obj == NULL)
        {
                return;
        }
        lv_label_set_text(sub_obj, lang_str_get(SOUND_XLS_LANG_ID_RINGTONE1 - 1 + user_data_get()->audio.common_entrance_tone));
        obj = lv_obj_get_child_form_id(parent, setting_sound_obj_id_security_office_cont);
        if (obj == NULL)
        {
                return;
        }
        sub_obj = lv_obj_get_child_form_id(obj, setting_sound_obj_id_security_office_sub);
        if (sub_obj == NULL)
        {
                return;
        }
        lv_label_set_text(sub_obj, lang_str_get(SOUND_XLS_LANG_ID_RINGTONE1 - 1 + user_data_get()->audio.securirty_office_tone));
        obj = lv_obj_get_child_form_id(parent, setting_sound_obj_id_extension_cont);
        if (obj == NULL)
        {
                return;
        }
        sub_obj = lv_obj_get_child_form_id(obj, setting_sound_obj_id_extension_sub);
        if (sub_obj == NULL)
        {
                return;
        }
        lv_label_set_text(sub_obj, lang_str_get(SOUND_XLS_LANG_ID_RINGTONE1 - 1 + user_data_get()->audio.extenion_tone));
}

static void setting_sound_ring_msgbox_confirm_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_sound_obj_id_msgbox_parent);
        if (parent == NULL)
        {
                return;
        }
        for (int i = 0; i < 6; i++)
        {
                lv_obj_t *list = lv_obj_get_child_form_id(lv_obj_get_child_form_id(parent, setting_sound_obj_id_msgbox_cont), setting_sound_obj_id_msgbox_list);
                if (list == NULL)
                {
                        return;
                }
                lv_obj_t *checkbox = lv_obj_get_child_form_id(lv_obj_get_child_form_id(list, i), 1);
                if (checkbox == NULL)
                {
                        return;
                }
                if (!strncmp((const char *)checkbox->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
                {
                        int id = layout_setting_sound_select_id_get();
                        if (id == setting_sound_obj_id_ring_repeat_cont)
                        {
                                user_data_get()->audio.ring_repeat = i;
                        }
                        else if (id == setting_sound_obj_id_buzzer_cont)
                        {
                                user_data_get()->audio.buzzer_tone = i + 1;
                        }
                        else if (id == setting_sound_obj_id_front_door_cont)
                        {
                                user_data_get()->audio.door_tone = i + 1;
                        }
                        else if (id == setting_sound_obj_id_common_entrance_cont)
                        {
                                user_data_get()->audio.common_entrance_tone = i + 1;
                        }
                        else if (id == setting_sound_obj_id_security_office_cont)
                        {
                                user_data_get()->audio.securirty_office_tone = i + 1;
                        }
                        else if (id == setting_sound_obj_id_extension_cont)
                        {
                                user_data_get()->audio.extenion_tone = i + 1;
                        }
                        user_data_save(false, false);
                        break;
                }
        }
        sat_layout_goto(setting_sound, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void setting_sound_ring_msg_box_create(const char *title_sting, int s_item, int num)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_sound_obj_id_msgbox_parent, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_sound_obj_id_msgbox_cont, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, setting_sound_obj_id_msgbox_titile, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title_sting, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_obj_t *list = setting_list_create(msgbox, setting_sound_obj_id_msgbox_list);
        lv_common_style_set_common(list, setting_sound_obj_id_msgbox_list, 47, 73, 366, 200, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        for (int i = 0; i < num; i++)
        {
                if (layout_setting_sound_select_id_get() == setting_sound_obj_id_ring_repeat_cont)
                {

                        lv_common_img_text_btn_create(list, i, 0, 0, 366, 55,
                                                      setting_sound_ring_msgbox_list_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      46, 8, 366 - 16, 32, 0,
                                                      lang_str_get(SOUND_XLS_LANG_ID_RINGTONE_KEEP_REPEAT), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                      0, 8, 32, 32, 1,
                                                      (const char *)resource_ui_src_get(i == s_item ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                }
                else
                {
                        lv_common_img_text_btn_create(list, i, 0, 0, 366, 55,
                                                      setting_sound_ring_msgbox_list_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      46, 8, 366 - 16, 32, 0,
                                                      lang_str_get(SOUND_XLS_LANG_ID_RINGTONE1 + i), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                      0, 8, 32, 32, 1,
                                                      (const char *)resource_ui_src_get(i + 1 == s_item ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                }
        }

        lv_common_img_btn_create(msgbox, setting_sound_obj_id_msgbox_cancel, 0, 281, 230, 62,
                                 setting_sound_ring_msgbox_cancel_click, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, setting_sound_obj_id_msgbox_confirm, 230, 281, 230, 62,
                                 setting_sound_ring_msgbox_confirm_click, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);
}
static void setting_outdoor_call_ringtone_repeat(lv_event_t *ev)
{
        layout_setting_sound_select_id_set(setting_sound_obj_id_ring_repeat_cont);
        setting_sound_ring_msg_box_create(lang_str_get(SOUND_XLS_LANG_ID_RINGTONE_REPEAT), user_data_get()->audio.ring_repeat, 2);
}

static void setting_sound_buzzer_obj_click(lv_event_t *ev)
{
        layout_setting_sound_select_id_set(setting_sound_obj_id_buzzer_cont);
        setting_sound_ring_msg_box_create(lang_str_get(SOUND_XLS_LANG_ID_BUZZER), user_data_get()->audio.buzzer_tone, 3);
}
static void setting_sound_front_door_obj_click(lv_event_t *ev)
{
        layout_setting_sound_select_id_set(setting_sound_obj_id_front_door_cont);
        setting_sound_ring_msg_box_create(lang_str_get(SOUND_XLS_LANG_ID_FRONT_DOOR), user_data_get()->audio.door_tone, 3);
}
static void setting_sound_common_entrance_obj_click(lv_event_t *ev)
{

        layout_setting_sound_select_id_set(setting_sound_obj_id_common_entrance_cont);
        setting_sound_ring_msg_box_create(lang_str_get(SOUND_XLS_LANG_ID_COMMON_ENTRANCE), user_data_get()->audio.common_entrance_tone, 3);
}
static void setting_sound_security_office_obj_click(lv_event_t *ev)
{
        layout_setting_sound_select_id_set(setting_sound_obj_id_security_office_cont);
        setting_sound_ring_msg_box_create(lang_str_get(SOUND_XLS_LANG_ID_SECURITY_OFFICE), user_data_get()->audio.securirty_office_tone, 3);
}
static void setting_sound_extension_obj_click(lv_event_t *ev)
{

        layout_setting_sound_select_id_set(setting_sound_obj_id_extension_cont);
        setting_sound_ring_msg_box_create(lang_str_get(SOUND_XLS_LANG_ID_EXTENSION), user_data_get()->audio.extenion_tone, 3);
}

/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 右边副列表
 ***********************************************/
static lv_obj_t *setting_sound_sub_list_create(void)
{
        setting_list_info_t main_list_group[] = {
            {0, 0, 622, 72, setting_sound_obj_id_volume_cont, setting_sound_obj_id_volume_title, setting_sound_obj_id_volume_sub, SOUND_XLS_LANG_ID_VOLUME, lang_str_get, SOUND_XLS_LANG_ID_THE_VOLUME_OF_THE_CALLING, lang_str_get, setting_sound_volume_obj_click},
            {0, 72, 622, 72, setting_sound_obj_id_ring_repeat_cont, setting_sound_obj_id_ring_repeat_title, setting_sound_obj_id_ring_repeat_sub, SOUND_XLS_LANG_ID_RINGTONE_REPEAT, lang_str_get, SOUND_XLS_LANG_ID_RINGTONE_ONETIME, lang_str_get, setting_outdoor_call_ringtone_repeat},
            {0, 72 * 2, 622, 72, setting_sound_obj_id_buzzer_cont, setting_sound_obj_id_buzzer_title, setting_sound_obj_id_buzzer_sub, SOUND_XLS_LANG_ID_BUZZER, lang_str_get, SOUND_XLS_LANG_ID_RINGTONE2, lang_str_get, setting_sound_buzzer_obj_click},
            {0, 72 * 3, 622, 72, setting_sound_obj_id_front_door_cont, setting_sound_obj_id_front_door_title, setting_sound_obj_id_front_door_sub, SOUND_XLS_LANG_ID_FRONT_DOOR, lang_str_get, SOUND_XLS_LANG_ID_RINGTONE1, lang_str_get, setting_sound_front_door_obj_click},
            {0, 72 * 4, 622, 72, setting_sound_obj_id_common_entrance_cont, setting_sound_obj_id_common_entrance_title, setting_sound_obj_id_common_entrance_sub, SOUND_XLS_LANG_ID_COMMON_ENTRANCE, lang_str_get, SOUND_XLS_LANG_ID_RINGTONE2, lang_str_get, setting_sound_common_entrance_obj_click},
            {0, 72 * 5, 622, 72, setting_sound_obj_id_security_office_cont, setting_sound_obj_id_security_office_title, setting_sound_obj_id_security_office_sub, SOUND_XLS_LANG_ID_SECURITY_OFFICE, lang_str_get, SOUND_XLS_LANG_ID_RINGTONE3, lang_str_get, setting_sound_security_office_obj_click},
            {0, 72 * 6, 622, 72, setting_sound_obj_id_extension_cont, setting_sound_obj_id_extension_title, setting_sound_obj_id_extension_sub, SOUND_XLS_LANG_ID_EXTENSION, lang_str_get, SOUND_XLS_LANG_ID_RINGTONE4, lang_str_get, setting_sound_extension_obj_click},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_sound_obj_id_sub_list);

        lv_common_style_set_common(list, setting_sound_obj_id_sub_list, 354, 88, 622, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        int j = 0;
        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                // if (((user_data_get()->system_mode & 0xF0) == 0xF0) && (i == 3 || i == 4))
                // {
                //         continue;
                // }
                lv_common_setting_btn_title_sub_info_img_create(list, main_list_group[i].cont_id, main_list_group[j].x, main_list_group[j].y, main_list_group[j].w, main_list_group[j].h,
                                                                main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                0, 17, 576, 43, main_list_group[i].title_id,
                                                                main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 42, 576, 29, main_list_group[i].sub_id,
                                                                main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 0, 0, 0, -1,
                                                                NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                j++;
        }

        layout_setting_sound_sub_title_display(list);

        return list;
}
static void sat_layout_enter(setting_sound)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 左边主列表
         ***********************************************/
        setting_main_list_create(2);
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 右边副列表
        ***********************************************/
        setting_sound_sub_list_create();
}
static void sat_layout_quit(setting_sound)
{
}
sat_layout_create(setting_sound);