#include "layout_define.h"
#include "layout_setting_general.h"
#include "common/language.h"
enum
{
        setting_recording_obj_id_title,
        setting_recording_obj_id_cancel,
        setting_recording_obj_id_main_list,

        setting_recording_obj_id_sub_list,

        setting_recording_obj_id_auto_cont,
        setting_recording_obj_id_auto_title,
        setting_recording_obj_id_auto_sub,

        setting_recording_obj_id_motion_cont,
        setting_recording_obj_id_motion_title,
        setting_recording_obj_id_motion_sub,

        setting_recording_obj_id_always_cont,
        setting_recording_obj_id_always_title,
        setting_recording_obj_id_always_sub,

        setting_recording_obj_id_msgbox_cont,
        setting_recording_obj_id_msgbox_parent,
        setting_recording_obj_id_msgbox_title,
        setting_recording_obj_id_msgbox_check_1,
        setting_recording_obj_id_msgbox_check_1_text,
        setting_recording_obj_id_msgbox_check_1_img,
        setting_recording_obj_id_msgbox_check_2,
        setting_recording_obj_id_msgbox_check_2_text,
        setting_recording_obj_id_msgbox_check_2_img,
        setting_recording_obj_id_msgbox_check_2_cover,
        setting_recording_obj_id_msgbox_check_3,
        setting_recording_obj_id_msgbox_check_3_text,
        setting_recording_obj_id_msgbox_check_3_img,
        setting_recording_obj_id_msgbox_check_4,
        setting_recording_obj_id_msgbox_check_4_text,
        setting_recording_obj_id_msgbox_check_4_img,
        setting_recording_obj_id_msgbox_confirm,
        setting_recording_obj_id_msgbox_confirm_img,
        setting_recording_obj_id_msgbox_cancel,
        setting_recording_obj_id_msgbox_cancel_img,
};
extern void always_record_time_set(int mode);
static void setting_recording_msgbox_del(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_recording_obj_id_msgbox_cont);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}

static lv_obj_t *setting_recording_msgbox_create(const char *title, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb, lv_event_cb_t checkbox_cb, const char *item[3], int n_item, int select_item)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_recording_obj_id_msgbox_cont, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, setting_recording_obj_id_msgbox_parent, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, setting_recording_obj_id_msgbox_title, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        if (n_item == 3)
        {
                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_1, 48, 89, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get(select_item == 0 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_2, 48, 145, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get(select_item == 1 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                if (((media_sdcard_insert_check() == SD_STATE_ERROR) || (media_sdcard_insert_check() == SD_STATE_UNPLUG)))
                {
                        lv_common_img_btn_create(msgbox, setting_recording_obj_id_msgbox_check_2_cover, 48, 145, 365, 48,
                                                 NULL, true, LV_OPA_60, 0x242526, LV_OPA_60, 0x242526,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                }

                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_3, 48, 201, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_3_text,
                                              item[2], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_3_img,
                                              (const char *)resource_ui_src_get(select_item == 2 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        else
        {

                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_1, 48, 61, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_1_text,
                                              item[0], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_1_img,
                                              (const char *)resource_ui_src_get(select_item == 0 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_2, 48, 117, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_2_text,
                                              item[1], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_2_img,
                                              (const char *)resource_ui_src_get(select_item == 1 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_3, 48, 173, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_3_text,
                                              item[2], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_3_img,
                                              (const char *)resource_ui_src_get(select_item == 2 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_text_btn_create(msgbox, setting_recording_obj_id_msgbox_check_4, 48, 228, 365, 48,
                                              checkbox_cb, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              48, 8, 365 - 94, 32, setting_recording_obj_id_msgbox_check_4_text,
                                              item[3], 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 32, 32, setting_recording_obj_id_msgbox_check_4_img,
                                              (const char *)resource_ui_src_get(select_item == 3 ? "btn_radio_s.png" : "btn_radio_n.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }

        lv_common_img_btn_create(msgbox, setting_recording_obj_id_msgbox_cancel, 0, 281, 230, 62,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, setting_recording_obj_id_msgbox_confirm, 230, 281, 230, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}
static void setting_recording_msgbox_cancel_click(lv_event_t *e)
{
        setting_recording_msgbox_del();
}

static int setting_recording_checkbox_mode_get(void)
{
        lv_obj_t *msgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_recording_obj_id_msgbox_cont);
        if (msgbox == NULL)
        {
                SAT_DEBUG("lv_event_get_current_target(e) is nullpter");
                return -1;
        }

        lv_obj_t *parent = lv_obj_get_child_form_id(msgbox, setting_recording_obj_id_msgbox_parent);
        if (parent == NULL)
        {
                SAT_DEBUG("et_parent(obj); is nullpter");
                return -1;
        }

        lv_obj_t *checkbox = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_1);
        if (checkbox == NULL)
        {
                SAT_DEBUG("et_parent(obj); is nullpter");
                return -1;
        }

        lv_obj_t *img_obj = lv_obj_get_child_form_id(checkbox, setting_recording_obj_id_msgbox_check_1_img);
        if (img_obj == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_1_img) is nullpter");
                return -1;
        }

        if (strncmp((const char *)img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
        {

                return 0;

                return -1;
        }

        checkbox = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_2);
        if (checkbox == NULL)
        {
                SAT_DEBUG("et_parent(obj); is nullpter");
                return -1;
        }

        img_obj = lv_obj_get_child_form_id(checkbox, setting_recording_obj_id_msgbox_check_2_img);
        if (img_obj == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_2_img) is nullpter");
                return -1;
        }

        if (strncmp((const char *)img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
        {

                return 1;

                return -1;
        }

        checkbox = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3);
        if (checkbox == NULL)
        {
                SAT_DEBUG("et_parent(obj); is nullpter");
                return -1;
        }

        img_obj = lv_obj_get_child_form_id(checkbox, setting_recording_obj_id_msgbox_check_3_img);
        if (img_obj == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3_img) is nullpter");
                return -1;
        }

        if (strncmp((const char *)img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
        {
                return 2;

                return -1;
        }
        checkbox = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_4);
        if (checkbox == NULL)
        {
                SAT_DEBUG("et_parent(obj); is nullpter");
                return -1;
        }

        img_obj = lv_obj_get_child_form_id(checkbox, setting_recording_obj_id_msgbox_check_4_img);
        if (img_obj == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3_img) is nullpter");
                return -1;
        }

        if (strncmp((const char *)img_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))) == 0)
        {
                return 3;

                return -1;
        }

        return -1;
}

static lv_obj_t *setting_record_sub_list_item_sub_get(int cont, int sub_id)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_recording_obj_id_sub_list);
        if (list == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(sat_cur_layout_screen_get(),setting_recording_obj_id_sub_list) faild");
                return NULL;
        }

        lv_obj_t *item = lv_obj_get_child_form_id(list, cont);
        if (item == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(list,cont); ");
                return NULL;
        }

        lv_obj_t *sub = lv_obj_get_child_form_id(item, sub_id);
        if (sub == NULL)
        {
                SAT_DEBUG("lv_obj_get_child_form_id(item, sub_id); ");
                return NULL;
        }

        return sub;
}

static bool setting_recording_auto_sub_display(void)
{
        lv_obj_t *sub = setting_record_sub_list_item_sub_get(setting_recording_obj_id_auto_cont, setting_recording_obj_id_auto_sub);
        if (sub == NULL)
        {
                SAT_DEBUG("lv_obj_t* sub = setting_record_auto_item_sub_get(setting_recording_obj_id_auto_sub);");
                return false;
        }
        lv_label_set_text(sub, lang_str_get(user_data_get()->auto_record_mode == 1 ? RECORDING_XLS_LANG_ID_SAVE_VIDEO : user_data_get()->auto_record_mode == 2 ? RECORDING_XLS_LANG_ID_SAVE_PICTURE
                                                                                                                                                               : RECORDING_XLS_LANG_ID_SAVE_OFF));
        return true;
}
static bool setting_recording_motion_sub_display(void)
{
        lv_obj_t *sub = setting_record_sub_list_item_sub_get(setting_recording_obj_id_motion_cont, setting_recording_obj_id_motion_sub);
        if (sub == NULL)
        {
                SAT_DEBUG("lv_obj_t* sub = setting_record_auto_item_sub_get(setting_recording_obj_id_motion_cont,setting_recording_obj_id_motion_sub);");
                return false;
        }
        if (user_data_get()->motion.enable)
        {
                if (monitor_valid_channel_check(user_data_get()->motion.select_camera))
                {
                        char name[64] = {0};
                        if (user_data_get()->motion.select_camera > MON_CH_DOOR8)
                        {
                                sprintf(name, network_data_get()->cctv_device[(int)user_data_get()->motion.select_camera - MON_CH_CCTV1].door_name);
                        }
                        else
                        {
                                sprintf(name, network_data_get()->door_device[(int)user_data_get()->motion.select_camera].door_name);
                        }
                        lv_label_set_text(sub, name);
                }
                else
                {
                        lv_label_set_text(sub, lang_str_get(SETTING_MOTION_XLS_LANG_ID_CAMERA_NOT_SET));
                }
        }
        else
        {
                lv_label_set_text(sub, lang_str_get(SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED));
        }

        return true;
}

static bool setting_recording_always_monitoring_sub_display(void)
{
        lv_obj_t *sub = setting_record_sub_list_item_sub_get(setting_recording_obj_id_always_cont, setting_recording_obj_id_always_sub);
        if (sub == NULL)
        {

                return false;
        }
        // const char * string = NULL;
        // if(always_record_mode_get() == 0)
        // {
        //        string =  lang_str_get(RECORDING_XLS_LANG_ID_SAVE_OFF);
        // }
        // else if(always_record_mode_get() == 1)
        // {
        //        string =  lang_str_get(RECORDING_XLS_LANG_ID_10_SEC_PER_CAMERA);
        // }
        // else if(always_record_mode_get() == 2)
        // {
        //        string =  lang_str_get(RECORDING_XLS_LANG_ID_30_SEC_PER_CAMERA);
        // }
        // else if(always_record_mode_get() == 3)
        // {
        //        string =  lang_str_get(RECORDING_XLS_LANG_ID_60_SEC_PER_CAMERA);
        // }

        lv_label_set_text(sub, lang_str_get(RECORDING_XLS_LANG_ID_SAVE_OFF));

        return true;
}

static void ssetting_recording_auto_msgbox_confirm_click(lv_event_t *e)
{
        int mode = setting_recording_checkbox_mode_get();
        if (mode >= 0)
        {
                if ((mode == 1) && ((media_sdcard_insert_check() == SD_STATE_ERROR) || (media_sdcard_insert_check() == SD_STATE_UNPLUG)))
                {
                        return;
                }

                user_data_get()->auto_record_mode = (char)mode;
                user_data_save(true, true);
                setting_recording_auto_sub_display();
        }
        setting_recording_msgbox_del();
}
static void setting_recording_auto_msgbox_item_click(lv_event_t *e)
{

        lv_obj_t *parent = lv_event_get_current_target(e);
        if (parent == NULL)
        {
                return;
        }
        lv_obj_t *check_obj, *discheck1_obj, *discheck2_obj;
        lv_obj_t *discheck3_obj = NULL;
        if (parent->id == setting_recording_obj_id_msgbox_check_1)
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_2);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_3);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_4);
                if (parent != NULL)
                {
                        discheck3_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_4_img);
                }
        }
        else if (parent->id == setting_recording_obj_id_msgbox_check_2)
        {
                check_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_1);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_3);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_4);
                if (parent != NULL)
                {
                        discheck3_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_4_img);
                }
        }
        else if (parent->id == setting_recording_obj_id_msgbox_check_3)
        {

                check_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_1);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_2);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_4);
                if (parent != NULL)
                {
                        discheck3_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_4_img);
                }
        }
        else
        {

                check_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_4_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_1);
                discheck1_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_1_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_2);
                discheck2_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_2_img);
                parent = lv_obj_get_child_form_id(lv_obj_get_parent(parent), setting_recording_obj_id_msgbox_check_3);
                discheck3_obj = lv_obj_get_child_form_id(parent, setting_recording_obj_id_msgbox_check_3_img);
        }
        if ((check_obj == NULL) || (discheck1_obj == NULL) || (discheck2_obj == NULL) || (check_obj->bg_img_src == NULL) || (discheck1_obj->bg_img_src == NULL) || (discheck2_obj->bg_img_src == NULL))
        {
                return;
        }

        if (strncmp((const char *)check_obj->bg_img_src, resource_ui_src_get("btn_radio_s.png"), strlen(resource_ui_src_get("btn_radio_s.png"))))
        {
                lv_obj_set_style_bg_img_src(check_obj, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(discheck1_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                lv_obj_set_style_bg_img_src(discheck2_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                if (discheck3_obj != NULL)
                {
                        lv_obj_set_style_bg_img_src(discheck3_obj, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
                }
        }
}

static void layout_settting_record_auto_record_sd_state_change_callback(void)
{
        setting_recording_msgbox_del();
        sd_state_channge_callback_register(sd_state_change_default_callback);
}
static void setting_recording_auto_obj_click(lv_event_t *ev)
{
        const char *item[3] = {0};
        item[0] = lang_str_get(RECORDING_XLS_LANG_ID_SAVE_OFF);
        item[1] = lang_str_get(RECORDING_XLS_LANG_ID_SAVE_VIDEO);
        item[2] = lang_str_get(RECORDING_XLS_LANG_ID_SAVE_PICTURE);
        setting_recording_msgbox_create(lang_str_get(RECORDING_XLS_LANG_ID_AUTO_RECORIDNG_CALLS),
                                        setting_recording_msgbox_cancel_click, ssetting_recording_auto_msgbox_confirm_click, setting_recording_auto_msgbox_item_click,
                                        item, 3, user_data_get()->auto_record_mode);
        sd_state_channge_callback_register(layout_settting_record_auto_record_sd_state_change_callback);
}
static void setting_recording_motion_obj_click(lv_event_t *ev)
{
        sat_layout_goto(setting_motion, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}

static void ssetting_recording_always_msgbox_confirm_click(lv_event_t *e)
{
        int mode = setting_recording_checkbox_mode_get();
        user_data_get()->always_monitoring = mode;

        user_data_save(true, true);
        if (mode > 0)
        {
                sat_layout_goto(always_record, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
        }
        sat_layout_goto(setting_recording, LV_SCR_LOAD_ANIM_MOVE_LEFT, SAT_VOID);
}
static void setting_recording_always_obj_click(lv_event_t *ev)
{
        if (monitor_door_registered_status_get() == false)
        {
                return;
        }
        const char *item[4] = {0};

        item[0] = lang_str_get(RECORDING_XLS_LANG_ID_SAVE_OFF);
        item[1] = lang_str_get(RECORDING_XLS_LANG_ID_10_SEC_PER_CAMERA);
        item[2] = lang_str_get(RECORDING_XLS_LANG_ID_30_SEC_PER_CAMERA);
        item[3] = lang_str_get(RECORDING_XLS_LANG_ID_60_SEC_PER_CAMERA);
        setting_recording_msgbox_create(lang_str_get(RECORDING_XLS_LANG_ID_ALWAYS_MONITORING),
                                        setting_recording_msgbox_cancel_click, ssetting_recording_always_msgbox_confirm_click, setting_recording_auto_msgbox_item_click,
                                        item, 4, 0);
}

static lv_obj_t *setting_recording_sub_list_create(void)
{
        setting_list_info_t main_list_group[] = {

            {0, 0, 622, 72,
             setting_recording_obj_id_auto_cont, setting_recording_obj_id_auto_title, setting_recording_obj_id_auto_sub,
             RECORDING_XLS_LANG_ID_AUTO_RECORIDNG_CALLS, lang_str_get,
             -1, NULL,
             setting_recording_auto_obj_click},
            {0, 72, 622, 72,
             setting_recording_obj_id_motion_cont, setting_recording_obj_id_motion_title, setting_recording_obj_id_motion_sub,
             RECORDING_XLS_LANG_ID_MOTION_DETECTION, lang_str_get,
             -1, NULL,
             setting_recording_motion_obj_click},
            {0, 72 * 2, 622, 72,
             setting_recording_obj_id_always_cont, setting_recording_obj_id_always_title, setting_recording_obj_id_always_sub,
             RECORDING_XLS_LANG_ID_ALWAYS_MONITORING, lang_str_get,
             -1, NULL,
             setting_recording_always_obj_click},
        };

        lv_obj_t *list = setting_list_create(sat_cur_layout_screen_get(), setting_recording_obj_id_sub_list);
        lv_common_style_set_common(list, setting_recording_obj_id_sub_list, 354, 88, 622, 512, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
        int j = 0;
        for (int i = 0; i < sizeof(main_list_group) / sizeof(setting_list_info_t); i++)
        {
                /*此处请斟酌*/
                if ((((user_data_get()->system_mode & 0x0F) != 0x01) && (i == 1)) || i == 2)
                {
                        continue;
                }
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

        setting_recording_auto_sub_display();
        setting_recording_motion_sub_display();
        setting_recording_always_monitoring_sub_display();
        return list;
}

static void sat_layout_enter(setting_recording)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 左边主列表
         ***********************************************/
        setting_main_list_create(4);
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 右边副列表
        ***********************************************/
        setting_recording_sub_list_create();
}
static void sat_layout_quit(setting_recording)
{
        sd_state_channge_callback_register(sd_state_change_default_callback);
}

sat_layout_create(setting_recording);