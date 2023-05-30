#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_setting_motion.h"
#include "layout_setting_recording.h"
enum
{
        setting_motion_obj_id_title,
        setting_motion_obj_id_cancel,
        setting_motion_obj_id_list,

        setting_motion_obj_id_use_motion_cont,

        setting_motion_obj_id_select_camera_cont,

        setting_motion_obj_id_storage_format_cont,

        setting_motion_obj_id_storage_sensitivity_cont,

        setting_motion_obj_id_motion_detecting_schedule_cont,

        setting_motion_obj_id_motion_lcd_on_cont,

        setting_motion_obj_id_msgbox_cont,
        setting_motion_obj_id_msgbox_parent,
        setting_motion_obj_id_msgbox_title,
        setting_motion_obj_id_msgbox_check_1,
        setting_motion_obj_id_msgbox_check_1_text,
        setting_motion_obj_id_msgbox_check_1_img,
        setting_motion_obj_id_msgbox_check_2,
        setting_motion_obj_id_msgbox_check_2_text,
        setting_motion_obj_id_msgbox_check_2_img,
        setting_motion_obj_id_msgbox_check_3,
        setting_motion_obj_id_msgbox_check_3_text,
        setting_motion_obj_id_msgbox_check_3_img,
        setting_motion_obj_id_msgbox_check_4,
        setting_motion_obj_id_msgbox_check_4_text,
        setting_motion_obj_id_msgbox_check_4_img,
        setting_motion_obj_id_msgbox_confirm,
        setting_motion_obj_id_msgbox_confirm_img,
        setting_motion_obj_id_msgbox_cancel,
        setting_motion_obj_id_msgbox_cancel_img,
};
static void setting_motion_cancel_obj_click(lv_event_t *e)
{
        sat_layout_goto(setting_recording, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}
static void setting_motion_use_motion_detection_display(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_list);
        lv_obj_t *item = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_use_motion_cont);
        lv_obj_t *img = lv_obj_get_child_form_id(item, 2);
        lv_obj_set_style_bg_img_src(img, resource_ui_src_get(user_data_get()->motion.enable == true ? "btn_switch_on.png" : "btn_switch_off.png"), LV_PART_MAIN);
        if (user_data_get()->motion.enable == true)
        {
                lv_obj_clear_flag(list, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(list, LV_OBJ_FLAG_HIDDEN);
        }
}

static lv_obj_t *setting_motion_list_item_sub_get(int cont_id)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_list);
        if (list == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(sat_cur_layout_screen_get(),setting_motion_obj_id_list) faild");
                return NULL;
        }

        lv_obj_t *item = lv_obj_get_child_form_id(list, cont_id);
        if (item == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(list,cont_id); ");
                return NULL;
        }

        lv_obj_t *sub = lv_obj_get_child_form_id(item, 1);
        if (sub == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(item, 1); ");
                return NULL;
        }
        return sub;
}

static bool setting_motion_select_camera_display(void)
{
        lv_obj_t *sub = setting_motion_list_item_sub_get(setting_motion_obj_id_select_camera_cont);
        if (sub == NULL)
        {
                SAT_DEBUG(" lv_obj_t*sub = setting_motion_list_item_sub_get(setting_motion_obj_id_select_camera_cont);");
                return false;
        }

        lv_label_set_text(sub, language_common_ch_string_get(user_data_get()->motion.select_camera));
        return true;
}

static bool setting_motion_storage_foramt_display(void)
{
        lv_obj_t *sub = setting_motion_list_item_sub_get(setting_motion_obj_id_storage_format_cont);
        if (sub == NULL)
        {
                SAT_DEBUG(" lv_obj_t*sub = setting_motion_list_item_sub_get(setting_motion_obj_id_storage_format_cont);");
                return false;
        }

        lv_label_set_text(sub, language_common_string_get(user_data_get()->motion.saving_fmt == 0 ? LANG_COMMON_ID_VIDEO : LANG_COMMON_ID_PHOTO));
        return true;
}

static bool setting_motion_sensivitity_display(void)
{
        lv_obj_t *sub = setting_motion_list_item_sub_get(setting_motion_obj_id_storage_sensitivity_cont);
        if (sub == NULL)
        {
                SAT_DEBUG(" lv_obj_t*sub = setting_motion_list_item_sub_get(setting_motion_obj_id_storage_sensitivity_cont);");
                return false;
        }

        lv_label_set_text(sub, layout_setting_motion_language_get(user_data_get()->motion.sensivity == 0 ? SETTING_MOTION_LANG_ID_HIGH : user_data_get()->motion.sensivity == 1 ? SETTING_MOTION_LANG_ID_MIDDLE
                                                                                                                                                                                : SETTING_MOTION_LANG_ID_LOW));
        return true;
}

static bool setting_motion_timer_diplay(void)
{
        lv_obj_t *sub = setting_motion_list_item_sub_get(setting_motion_obj_id_motion_detecting_schedule_cont);
        if (sub == NULL)
        {
                SAT_DEBUG(" lv_obj_t*sub = setting_motion_list_item_sub_get(setting_motion_obj_id_motion_detecting_schedule_cont);");
                return false;
        }

        lv_label_set_text(sub, language_common_string_get(user_data_get()->motion.timer_en == 0 ? LANG_COMMON_ID_OFF:LANG_COMMON_ID_ON));
        return true;
}

static bool setting_motion_lcd_display(void)
{
        lv_obj_t *sub = setting_motion_list_item_sub_get(setting_motion_obj_id_motion_detecting_schedule_cont);
        if (sub == NULL)
        {
                SAT_DEBUG(" lv_obj_t*sub = setting_motion_list_item_sub_get(setting_motion_obj_id_motion_detecting_schedule_cont);");
                return false;
        }

        lv_label_set_text(sub, language_common_string_get(user_data_get()->motion.lcd_en == 0 ? LANG_COMMON_ID_OFF:LANG_COMMON_ID_ON));
        return true;
}

static void setting_motion_msgbox_del(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_msgbox_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}

static int setting_motion_msgbox_item_select_index_get(int max_item)
{
        int img_obj_id_group[][2] = {
            {setting_motion_obj_id_msgbox_check_1, setting_motion_obj_id_msgbox_check_1_img},
            {setting_motion_obj_id_msgbox_check_2, setting_motion_obj_id_msgbox_check_2_img},
            {setting_motion_obj_id_msgbox_check_3, setting_motion_obj_id_msgbox_check_3_img},
            {setting_motion_obj_id_msgbox_check_4, setting_motion_obj_id_msgbox_check_4_img}};

        if (sizeof(img_obj_id_group) / (2 * sizeof(int)) < max_item)
        {
                SAT_DEBUG("sizeof(img_obj_id_group)/sizeof(int) < max_item");
                return -1;
        }

        lv_obj_t *msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_msgbox_cont);
        if (msgbox == NULL)
        {
                SAT_DEBUG("lv_obj_t* msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_obj_id_msgbox_cont);");
                return -1;
        }

        lv_obj_t *parent = lv_obj_get_child_form_id(msgbox, setting_motion_obj_id_msgbox_parent);
        if (parent == NULL)
        {
                SAT_DEBUG("lv_obj_t* parent = lv_obj_get_child_form_id(msgbox, setting_motion_obj_id_msgbox_cont);");
                return -1;
        }

        for (int i = 0; i < max_item; i++)
        {
                lv_obj_t *checkbox = lv_obj_get_child_form_id(parent, img_obj_id_group[i][0]);
                if (checkbox == NULL)
                {
                        SAT_DEBUG(" lv_obj_t* checkbox = lv_obj_get_child_form_id(parent,img_obj_id_group[i]);");
                        return -1;
                }

                lv_obj_t *img = lv_obj_get_child_form_id(checkbox, img_obj_id_group[i][1]);
                if (img == NULL)
                {
                        SAT_DEBUG("  lv_obj_t* img =  lv_obj_get_child_form_id(checkbox, img_obj_id_group[i][1]);");
                        return -1;
                }

                if (strncmp((const char *)img->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
                {
                        return i;
                }
        }
        return -1;
}

static lv_obj_t *setting_motion_msgbox_create(const char *title, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb, const char *item[3], int n_item, int select_item)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_motion_obj_id_msgbox_cont, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_motion_obj_id_msgbox_parent, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, setting_motion_obj_id_msgbox_title, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        if (n_item == 2)
        {
                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_1, 48, 110, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get(select_item == 0 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_2, 48, 166, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get(select_item == 1 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        else if (n_item == 3)
        {
                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_1, 48, 89, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get(select_item == 0 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_2, 48, 145, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get(select_item == 1 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_3, 48, 201, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_3_text,
                                              item[2], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_3_img,
                                              (const char *)resource_ui_src_get(select_item == 2 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        else if (n_item == 4)
        {
                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_1, 48, 61, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get(select_item == 0 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_2, 48, 116, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get(select_item == 1 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_3, 48, 171, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_3_text,
                                              item[2], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_3_img,
                                              (const char *)resource_ui_src_get(select_item == 2 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_motion_obj_id_msgbox_check_4, 48, 226, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_motion_obj_id_msgbox_check_4_text,
                                              item[3], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_motion_obj_id_msgbox_check_4_img,
                                              (const char *)resource_ui_src_get(select_item == 3 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }

        lv_common_img_btn_create(msgbox, setting_motion_obj_id_msgbox_cancel, 0, 281, 230, 62,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, setting_motion_obj_id_msgbox_confirm, 230, 281, 230, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}

static void setting_motion_msgbox_item_click(lv_event_t *e)
{
        lv_obj_t *item = lv_event_get_current_target(e);
        if (item == NULL)
        {
                return;
        }
        lv_obj_t *check_obj1 = NULL, *discheck_obj2 = NULL, *discheck_obj3 = NULL, *discheck_obj4 = NULL, *parent = NULL;
        if (item->id == setting_motion_obj_id_msgbox_check_1)
        {
                check_obj1 = lv_obj_get_child_form_id(item, setting_motion_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_2);
                if (parent != NULL)
                {
                        discheck_obj2 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_2_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_3);
                if (parent != NULL)
                {
                        discheck_obj3 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_3_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_4);
                if (parent != NULL)
                {
                        discheck_obj4 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_4_img);
                }
        }
        else if (item->id == setting_motion_obj_id_msgbox_check_2)
        {
                check_obj1 = lv_obj_get_child_form_id(item, setting_motion_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_1);
                if (parent != NULL)
                {
                        discheck_obj2 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_1_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_3);
                if (parent != NULL)
                {
                        discheck_obj3 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_3_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_4);
                if (parent != NULL)
                {
                        discheck_obj4 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_4_img);
                }
        }
        else if (item->id == setting_motion_obj_id_msgbox_check_3)
        {
                check_obj1 = lv_obj_get_child_form_id(item, setting_motion_obj_id_msgbox_check_3_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_1);
                if (parent != NULL)
                {
                        discheck_obj2 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_1_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_2);
                if (parent != NULL)
                {
                        discheck_obj3 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_2_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_4);
                if (parent != NULL)
                {
                        discheck_obj4 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_4_img);
                }
        }
        else if (item->id == setting_motion_obj_id_msgbox_check_4)
        {
                check_obj1 = lv_obj_get_child_form_id(item, setting_motion_obj_id_msgbox_check_4_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_1);
                if (parent != NULL)
                {
                        discheck_obj2 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_1_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_2);
                if (parent != NULL)
                {
                        discheck_obj3 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_2_img);
                }
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(item), setting_motion_obj_id_msgbox_check_3);
                if (parent != NULL)
                {
                        discheck_obj4 = lv_obj_get_child_form_id(parent, setting_motion_obj_id_msgbox_check_3_img);
                }
        }
        if (check_obj1 == NULL)
        {
                return;
        }

        if (strncmp((const char *)check_obj1->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                lv_obj_set_style_bg_img_src(check_obj1, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                if (discheck_obj2)
                {
                        lv_obj_set_style_bg_img_src(discheck_obj2, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                }
                if (discheck_obj3)
                {
                        lv_obj_set_style_bg_img_src(discheck_obj3, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                }
                if (discheck_obj4)
                {
                        lv_obj_set_style_bg_img_src(discheck_obj4, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                }
        }
}

static void setting_motion_msgbox_cancel_obj_click(lv_event_t *e)
{
        setting_motion_msgbox_del();
}
static void setting_motion_select_camera_msgbox_confim_click(lv_event_t *e)
{
        int item_n = setting_motion_msgbox_item_select_index_get(4);
        if (item_n < 0)
        {
                return;
        }
        char u_camera_data = item_n == 0 ? MON_CH_DOOR1 : item_n == 1 ? MON_CH_DOOR2
                                                      : item_n == 2   ? MON_CH_CCTV1
                                                                      : MON_CH_CCTV2;

        if (u_camera_data != user_data_get()->motion.select_camera)
        {
                user_data_get()->motion.select_camera = u_camera_data;
                user_data_save();
                setting_motion_select_camera_display();
        }

        setting_motion_msgbox_del();
}

static void setting_motion_storage_foramt_msgbox_confim_click(lv_event_t *e)
{
        int item_n = setting_motion_msgbox_item_select_index_get(2);
        if (item_n < 0)
        {
                return;
        }

        if (item_n != user_data_get()->motion.saving_fmt)
        {
                user_data_get()->motion.saving_fmt = item_n;
                user_data_save();
                setting_motion_storage_foramt_display();
        }

        setting_motion_msgbox_del();
}
static void setting_motion_sensivity_msgbox_confim_click(lv_event_t *e)
{
        int item_n = setting_motion_msgbox_item_select_index_get(3);
        if (item_n < 0)
        {
                return;
        }

        if (item_n != user_data_get()->motion.sensivity)
        {
                user_data_get()->motion.sensivity = item_n;
                user_data_save();
                setting_motion_sensivitity_display();
        }

        setting_motion_msgbox_del();
}
static void setting_motion_lcd_on_msgbox_confim_click(lv_event_t *e)
{
        int item_n = setting_motion_msgbox_item_select_index_get(2);
        if (item_n < 0)
        {
                return;
        }

        if (item_n != user_data_get()->motion.lcd_en)
        {
                user_data_get()->motion.lcd_en = item_n;
                user_data_save();
                setting_motion_lcd_display();
        }
        setting_motion_msgbox_del();
}
static void setting_motion_list_item_click(lv_event_t *e)
{
        lv_obj_t *item = lv_event_get_current_target(e);
        if (item->id == setting_motion_obj_id_use_motion_cont)
        {
                user_data_get()->motion.enable = user_data_get()->motion.enable ? false : true;
                user_data_save();
                setting_motion_use_motion_detection_display();
        }
        else if (item->id == setting_motion_obj_id_select_camera_cont)
        {
                const char *item[4] = {0};
                item[0] = language_common_string_get(LANG_COMMON_ID_DOOR1);
                item[1] = language_common_string_get(LANG_COMMON_ID_DOOR2);
                item[2] = language_common_string_get(LANG_COMMON_ID_CCTV1);
                item[3] = language_common_string_get(LANG_COMMON_ID_CCTV2);
                setting_motion_msgbox_create(layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_SELECT_CAMERA),
                                             setting_motion_msgbox_cancel_obj_click, setting_motion_select_camera_msgbox_confim_click, setting_motion_msgbox_item_click,
                                             item, 4, user_data_get()->motion.select_camera == MON_CH_DOOR1 ? 0 : user_data_get()->motion.select_camera == MON_CH_DOOR2 ? 1
                                                                                                              : user_data_get()->motion.select_camera == MON_CH_CCTV1   ? 2
                                                                                                                                                                        : 3);
        }
        else if (item->id == setting_motion_obj_id_storage_format_cont)
        {
                const char *item[2] = {0};
                item[0] = language_common_string_get(LANG_COMMON_ID_VIDEO);
                item[1] = language_common_string_get(LANG_COMMON_ID_PHOTO);
                setting_motion_msgbox_create(layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_STORAGE_FORMAT),
                                             setting_motion_msgbox_cancel_obj_click, setting_motion_storage_foramt_msgbox_confim_click, setting_motion_msgbox_item_click,
                                             item, 2, user_data_get()->motion.saving_fmt);
        }
        else if (item->id == setting_motion_obj_id_storage_sensitivity_cont)
        {
                const char *item[3] = {0};
                item[0] = layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_HIGH);
                item[1] = layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_MIDDLE);
                item[2] = layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_LOW);
                setting_motion_msgbox_create(layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_SENSITIVTY_SETTING),
                                             setting_motion_msgbox_cancel_obj_click, setting_motion_sensivity_msgbox_confim_click, setting_motion_msgbox_item_click,
                                             item, 3, user_data_get()->motion.sensivity);
        }
        else if (item->id == setting_motion_obj_id_motion_detecting_schedule_cont)
        {
                sat_layout_goto(setting_motion_timer, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
        else if (item->id == setting_motion_obj_id_motion_lcd_on_cont)
        {
                const char *item[2] = {0};
                item[0] = language_common_string_get(LANG_COMMON_ID_OFF);
                item[1] = language_common_string_get(LANG_COMMON_ID_ON);
                setting_motion_msgbox_create(layout_setting_motion_language_get(SETTING_MOTION_LANG_ID_LCD_ON_WHEN_MOTION_DETECTED),
                                             setting_motion_msgbox_cancel_obj_click, setting_motion_lcd_on_msgbox_confim_click, setting_motion_msgbox_item_click,
                                             item, 2, user_data_get()->motion.lcd_en ? 1 : 0);
        }
}

static lv_obj_t *setting_motion_list_create(lv_obj_t *parent)
{
        setting_list_info_t main_list_group[] = {

            {48, 88, 928, 88,
             setting_motion_obj_id_use_motion_cont, 0, -1,
             SETTING_MOTION_LANG_ID_USE_MOTION_DETECTION, layout_setting_motion_language_get,
             -1, NULL,
             setting_motion_list_item_click, 2},
            {0, 88, 928, 88,
             setting_motion_obj_id_select_camera_cont, 0, 1,
             SETTING_MOTION_LANG_ID_SELECT_CAMERA, layout_setting_motion_language_get,
             -1, NULL,
             setting_motion_list_item_click, 2},
            {0, 88 * 2, 928, 88,
             setting_motion_obj_id_storage_format_cont, 0, 1,
             SETTING_MOTION_LANG_ID_STORAGE_FORMAT, layout_setting_motion_language_get,
             LANG_COMMON_ID_PHOTO, language_common_string_get,
             setting_motion_list_item_click, 2},
            {0, 88 * 3, 928, 88,
             setting_motion_obj_id_storage_sensitivity_cont, 0, 1,
             SETTING_MOTION_LANG_ID_SENSITIVTY_SETTING, layout_setting_motion_language_get,
             SETTING_MOTION_LANG_ID_HIGH, language_common_string_get,
             setting_motion_list_item_click, 2},
            {0, 88 * 4, 928, 88,
             setting_motion_obj_id_motion_detecting_schedule_cont, 0, 1,
             SETTING_MOTION_LANG_ID_MOTION_DETECTING_SCHEDULE, layout_setting_motion_language_get,
             LANG_COMMON_ID_OFF, language_common_string_get,
             setting_motion_list_item_click, 2},
            {0, 88 * 5, 928, 88,
             setting_motion_obj_id_motion_lcd_on_cont, 0, 1,
             SETTING_MOTION_LANG_ID_MOTION_DETECTING_SCHEDULE, layout_setting_motion_language_get,
             LANG_COMMON_ID_OFF, language_common_string_get,
             setting_motion_list_item_click, 2},
        };

        lv_obj_t *list = setting_list_create(parent, setting_motion_obj_id_list);
        lv_common_style_set_common(list, setting_motion_obj_id_list, 48, 88 * 2, 928, 424, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                lv_common_setting_btn_title_sub_info_img_create(i == 0 ? sat_cur_layout_screen_get() : list, main_list_group[i].cont_id, main_list_group[i].x, main_list_group[i].y, main_list_group[i].w, main_list_group[i].h,
                                                                main_list_group[i].click_cb, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                0, 30, 576, 43, main_list_group[i].title_id,
                                                                main_list_group[i].title_language_id == -1 ? NULL : main_list_group[i].title_language_cb(main_list_group[i].title_language_id), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 55, 576, 29, main_list_group[i].sub_id,
                                                                main_list_group[i].sub_language_id == -1 ? NULL : main_list_group[i].sub_language_cb(main_list_group[i].sub_language_id), 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                840, 20, 80, 48, main_list_group[i].img_id,
                                                                NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        setting_motion_select_camera_display();
        setting_motion_storage_foramt_display();
        setting_motion_sensivitity_display();
        setting_motion_timer_diplay();
        setting_motion_lcd_display();
        return list;
}
static void sat_layout_enter(setting_motion)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-3 14:13:25
        ** 说明: 标题创建
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_motion_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      layout_setting_recording_language_get(RECORDING_LANG_ID_MOTION_DETECTION), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 返回
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_motion_obj_id_cancel, 35, 15, 48, 48,
                                         setting_motion_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 设置motion状态
         ***********************************************/
        {
                setting_motion_list_create(sat_cur_layout_screen_get());
                setting_motion_use_motion_detection_display();
        }
}

static void sat_layout_quit(setting_motion)
{
}

sat_layout_create(setting_motion);