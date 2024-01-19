#include "layout_define.h"
enum
{
        setting_building_house_number_obj_id_titile_label,
        setting_building_house_number_obj_id_cancel_btn,
        setting_building_house_number_obj_id_next_btn,

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 8:50:19
        ** 说明: 数字键盘
        ***********************************************/
        setting_building_house_number_obj_id_number_keyboard_btn,

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 13:43:33
        ** 说明: ip文本框栋号
        ***********************************************/
        setting_building_house_number_obj_id_building_number_textbox,
        setting_building_house_number_obj_id_foolr_number_textbox,
        setting_building_house_number_obj_id_household_number_textbox,
        setting_building_house_number_obj_id_extension_number_textbox,
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 13:43:33
        ** 说明: ip文本显示
        ***********************************************/
        setting_building_house_number_obj_id_building_number_label,
        setting_building_house_number_obj_id_foolr_number_label,
        setting_building_house_number_obj_id_household_number_label,
        setting_building_house_number_obj_id_extension_number_label,
};

static void setting_building_house_number_obj_cancel_click(lv_event_t *e)
{
        sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}
static void setting_building_house_number_obj_confirm_click(lv_event_t *e)
{
        lv_obj_t *textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_building_number_textbox);
        const char *building_str = lv_textarea_get_text(textarea);

        // textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_foolr_number_textbox);
        // const char *foolr_str = lv_textarea_get_text(textarea);

        textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_household_number_textbox);
        const char *household_str = lv_textarea_get_text(textarea);

        // textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_extension_number_textbox);
        // const char *extension_str = lv_textarea_get_text(textarea);

        if ((strlen(building_str) >= 1) && (strlen(household_str) >= 1))
        {
                memset(&network_data_get()->sip_user[0], '0', 8);
                strncpy(&network_data_get()->sip_user[4 - strlen(building_str)], building_str, strlen(building_str));
                strncpy(&network_data_get()->sip_user[8 - strlen(household_str)], household_str, strlen(household_str));

                network_data_save();

                backlight_enable(false);
                usleep(100 * 1000);
                system("reboot");
        }

        sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void setting_building_house_number_next_obj_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_next_btn);
        if (obj == NULL)
        {
                return;
        }

        if (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) == true)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 8:51:49
** 说明: 输入数字键盘
***********************************************/
static lv_obj_t *setting_building_house_number_textarea_focused_get(void)
{

        int obj_id[] = {
            setting_building_house_number_obj_id_building_number_textbox,
            setting_building_house_number_obj_id_foolr_number_textbox,
            setting_building_house_number_obj_id_household_number_textbox,
            setting_building_house_number_obj_id_extension_number_textbox};

        lv_obj_t *textarea = NULL;

        for (int i = 0; i < sizeof(obj_id) / sizeof(int); i++)
        {

                textarea = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), obj_id[i]);

                if ((textarea != NULL) && (lv_obj_get_state(textarea) & LV_STATE_FOCUSED))
                {

                        return textarea;
                }
        }

        return textarea;
}

static bool setting_building_house_number_textbox_del(void)
{
        lv_obj_t *textarea = setting_building_house_number_textarea_focused_get();

        if (textarea == NULL)
        {
                return false;
        }

        lv_textarea_del_char(textarea);

        return true;
}
static bool setting_building_house_number_textbox_add(const char *string)
{
        lv_obj_t *textarea = setting_building_house_number_textarea_focused_get();

        if (textarea == NULL)
        {

                return false;
        }

        lv_textarea_add_text(textarea, string);
        return true;
}

static void setting_building_house_number_obj_keyboad_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_event_get_target(e);
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);

        if (id == 11)
        {

                setting_building_house_number_textbox_del();
        }
        else
        {
                const char *text = lv_btnmatrix_get_btn_text(obj, id);

                if (text != NULL)
                {

                        setting_building_house_number_textbox_add(text);
                }
        }
        setting_building_house_number_next_obj_display();
}

static void setting_building_house_number_item_display(void)
{
        lv_obj_t *item1_label = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_building_number_label);
        lv_obj_t *item1_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_building_number_textbox);

        lv_obj_t *item2_label = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_foolr_number_label);

        lv_obj_t *item2_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_foolr_number_textbox);

        lv_obj_t *item3_label = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_household_number_label);
        lv_obj_t *item3_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_household_number_textbox);

        lv_obj_t *item4_label = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_extension_number_label);
        lv_obj_t *item4_txt = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_extension_number_textbox);

        lv_obj_set_style_y(item1_label, 250, LV_PART_MAIN);
        lv_obj_set_style_y(item1_txt, 257, LV_PART_MAIN);
        lv_obj_set_style_y(item3_label, 334, LV_PART_MAIN);
        lv_obj_set_style_y(item3_txt, 341, LV_PART_MAIN);
        lv_obj_add_flag(item2_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(item2_txt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(item4_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(item4_txt, LV_OBJ_FLAG_HIDDEN);
}
static void sat_layout_enter(setting_building_house_number)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_titile_label, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_BUILD_HOUSE_NUMBER), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 上一步下一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_cancel_btn, 35, 15, 48, 48,
                                         setting_building_house_number_obj_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                lv_obj_t *obj = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_next_btn, 912, 15, 80, 48,
                                                         setting_building_house_number_obj_confirm_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_title_check.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:18
        ** 说明: 数字键盘创建
        ***********************************************/
        {
                lv_common_number_input_keyboard_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_number_keyboard_btn, 608, 127, 312, 402,
                                                       setting_building_house_number_obj_keyboad_click, LV_OPA_COVER, 0x808080, LV_OPA_COVER, 0x00a8ff,
                                                       360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                       0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                       18, 24);
        }

        char building[8] = {0};
        char household[8] = {0};
        strncpy(building, &network_data_get()->sip_user[0], 4);
        strncpy(household, &network_data_get()->sip_user[4], 4);

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_building_number_textbox, 298, 166, 250, 50,
                                          NULL, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          building, 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 4,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:48
         ** 说明: 文本显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_building_number_label, 32, 173, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_BUILD_NUMBER), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_foolr_number_textbox, 298, 250, 250, 50,
                                          NULL, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          "floor", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 4,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_foolr_number_label, 32, 257, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      "lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_FOOLR_NUMBER)", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_household_number_textbox, 298, 334, 250, 50,
                                          NULL, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          household, 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 4,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_household_number_label, 32, 341, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_HOUSEHOLD_NUMBER), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:43:29
         ** 说明: 文本输入框显示
         ***********************************************/
        {
                lv_common_textarea_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_extension_number_textbox, 298, 421, 250, 50,
                                          NULL, LV_OPA_TRANSP, 0, LV_OPA_COVER, 0X101010,
                                          LV_OPA_TRANSP, 0Xffffff, LV_OPA_COVER, 0Xffffff,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0X101010,
                                          9, 2, LV_BORDER_SIDE_FULL, LV_OPA_COVER, 0x00a8ff,
                                          "extension", 0Xffffff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal, 4,
                                          5, 500, 0Xffffff);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:43:48
        ** 说明: 文本显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_building_house_number_obj_id_extension_number_label, 32, 428, 200, 50,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      "lang_str_get(SERVER_OPERATION_NETWORK_XLS_LANG_ID_EXTENSION_NUMBER)", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
        }
        setting_building_house_number_item_display();
}
static void sat_layout_quit(setting_building_house_number)
{
}

sat_layout_create(setting_building_house_number);