
#include "lv_common.h"

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-30 8:30:59
** 说明: 通用按钮创建
***********************************************/
lv_obj_t *lv_common_img_btn_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                   lv_event_cb_t cb, bool click, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                   int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                   int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                   const char *img_src, lv_opa_t press_img_recolor_opa, uint32_t press_img_recolor, lv_align_t align)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);

        if (obj)
        {
                return obj;
        }

        obj = lv_obj_create(parent);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLLABLE);

        lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);
        lv_common_style_set_boader(obj, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN);
        lv_common_style_set_boader(obj, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_press_color, LV_STATE_PRESSED);
        lv_common_style_set_img(obj, img_src, press_img_recolor_opa, press_img_recolor, align, LV_PART_MAIN, LV_STATE_PRESSED);
        lv_common_style_set_event(obj, cb, click, default_bg_opa, default_bg_color, LV_PART_MAIN, press_bg_opa, press_bg_color, LV_STATE_PRESSED);
        return obj;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-30 8:31:26
** 说明: 通用标题文本显示
***********************************************/
lv_obj_t *lv_common_text_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                const char *string, uint32_t font_default_color, uint32_t font_press_color, lv_text_align_t align, const lv_font_t *font)
{

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);

        if (obj != NULL)
        {

                return obj;
        }

        obj = lv_label_create(parent);
        lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);
        lv_common_style_set_text(obj, string, font_default_color, font_press_color, align, font, LV_STATE_PRESSED, LV_PART_MAIN);
        lv_common_style_set_boader(obj, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN);
        lv_common_style_set_boader(obj, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_default_color, LV_STATE_PRESSED);
        lv_common_style_set_event(obj, cb, cb, default_bg_opa, default_bg_color, LV_PART_MAIN, press_bg_opa, press_bg_color, LV_STATE_PRESSED);
        return obj;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-2 14:8:52
** 说明: 图片按钮创建
***********************************************/
lv_obj_t *lv_common_img_text_btn_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                        lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                        int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                        int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                        int title_x, int title_y, int title_w, int title_h, int title_id,
                                        const char *title_string, uint32_t title_default_color, uint32_t title_press_color, lv_text_align_t title_align, lv_font_t *title_font,
                                        int img_x, int img_y, int img_w, int img_h, int img_id,
                                        const char *img_src, lv_opa_t press_img_recolor_opa, uint32_t press_img_recolor, lv_align_t img_align)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);

        if (obj != NULL)
        {
                return obj;
        }

        lv_obj_t *title_obj = NULL, *img_obj = NULL;

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置属性
        ***********************************************/
        {

                obj = lv_obj_create(parent);

                lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLLABLE);

                lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);
                lv_common_style_set_boader(obj, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN);
                lv_common_style_set_boader(obj, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_default_color, LV_STATE_PRESSED);
                lv_common_style_set_event(obj, cb, true, default_bg_opa, default_bg_color, LV_PART_MAIN, press_bg_opa, press_bg_color, LV_STATE_PRESSED);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置img
        ***********************************************/
        {
                img_obj = lv_common_img_btn_create(obj, img_id, img_x, img_y, img_w, img_h,
                                                   NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                   img_src, press_img_recolor_opa, press_img_recolor, img_align);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置title
        ***********************************************/
        {
                title_obj = lv_common_text_create(obj, title_id, title_x, title_y, title_w, title_h,
                                                  NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                  title_string, title_default_color, title_press_color, title_align, title_font);
        }
        lv_obj_set_state_bind(obj, title_obj);
        lv_obj_set_state_bind(title_obj, img_obj);
        return obj;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 8:19:24
** 说明: 数字输入键盘键盘创建
***********************************************/
lv_obj_t *lv_common_number_input_keyboard_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                                 lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                                 int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                                 int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                                 uint32_t font_default_color, uint32_t font_press_color, lv_text_align_t align, const lv_font_t *font,
                                                 int row, int column)
{
        lv_obj_t *btnmatrix = lv_obj_get_child_form_id(parent, id);

        if (btnmatrix != NULL)
        {

                return btnmatrix;
        }

        btnmatrix = lv_btnmatrix_create(parent);
        lv_common_style_set_common(btnmatrix, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-1 8:32:8
        ** 说明: 设置按钮属性
        ***********************************************/
        {
                static const char *btnm_map[] = {
                    "1", "2", "3", "\n",
                    "4", "5", "6", "\n",
                    "7", "8", "9", "\n",
                    ".", "0", " ", ""};

                static const char *btnm_img_map[] = {
                    NULL, NULL, NULL,
                    NULL, NULL, NULL,
                    NULL, NULL, NULL,
                    NULL, NULL, NULL};

                lv_btnmatrix_set_map(btnmatrix, btnm_map);

                lv_obj_set_style_pad_row(btnmatrix, row, LV_PART_MAIN);
                lv_obj_set_style_pad_column(btnmatrix, column, LV_PART_MAIN);

                lv_common_style_set_text(btnmatrix, NULL, font_default_color, font_press_color, align, font, LV_STATE_PRESSED, LV_PART_MAIN);
                lv_common_style_set_event(btnmatrix, cb, true, default_bg_opa, default_bg_color, LV_PART_ITEMS, press_bg_opa, press_bg_color, LV_PART_ITEMS | LV_STATE_PRESSED);

                lv_common_style_set_boader(btnmatrix, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_ITEMS);
                lv_common_style_set_boader(btnmatrix, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_default_color, LV_PART_ITEMS | LV_STATE_PRESSED);

                if (btnm_img_map[11] == NULL)
                {
                        btnm_img_map[11] = resource_ui_src_alloc("btn_numb_delete.png", 48, 48);
                }

                lv_obj_clear_flag(btnmatrix, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                lv_btnmatrix_set_btn_bg_map(btnmatrix, btnm_img_map);
        }

        return btnmatrix;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 11:43:3
** 说明: 键盘创建
***********************************************/
lv_obj_t *lv_common_keyboard_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                    lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                    int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                    int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                    int defalut_item_radius, int defalut_item_boarder_width, lv_border_side_t default_item_boarder_side, int boarder_item_default_opa, uint32_t boarde_item_default_color,
                                    int press_item_radius, int press_item_boarder_width, lv_border_side_t press_item_boarder_side, int boarder_item_press_opa, uint32_t boarder_item_press_color,
                                    uint32_t font_default_color, uint32_t font_press_color, lv_text_align_t align, const lv_font_t *font,
                                    int pad_top, int pad_left, int pad_right, int pad_bottom, int pad_column, int pad_row,
                                    lv_opa_t default_parent_bg_opa, uint32_t default_parent_bg_color, lv_opa_t press_parent_bg_opa, uint32_t press_parent_bg_color)
{
        lv_obj_t *keyboard = lv_obj_get_child_form_id(parent, id);

        if (keyboard != NULL)
        {

                return keyboard;
        }

        keyboard = lv_keyboard_create(parent);

        lv_common_style_set_common(keyboard, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);
        lv_common_style_set_event(keyboard, cb, cb, default_bg_opa, default_bg_color, LV_PART_ITEMS | LV_STATE_DEFAULT, press_bg_opa, press_bg_color, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_common_style_set_boader(keyboard, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_common_style_set_boader(keyboard, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_default_color, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_common_style_set_boader(keyboard, defalut_item_radius, boarder_item_default_opa, defalut_item_boarder_width, default_item_boarder_side, boarde_item_default_color, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_common_style_set_boader(keyboard, press_item_radius, boarder_item_press_opa, press_item_boarder_width, press_item_boarder_side, boarder_item_press_color, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_common_style_set_text(keyboard, NULL, font_default_color, font_press_color, align, font, LV_STATE_PRESSED, LV_PART_MAIN);
        lv_common_style_set_pad(keyboard, pad_top, pad_left, pad_bottom, pad_right, pad_column, pad_row, LV_PART_MAIN);

        lv_common_style_set_event(keyboard, NULL, false, default_parent_bg_opa, default_parent_bg_color, LV_PART_MAIN | LV_STATE_DEFAULT, press_parent_bg_opa, press_parent_bg_color, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_CLICK_FOCUSABLE);

        static const char *btnm_img_map[3][45] = {NULL};
        if (btnm_img_map[0][29] == NULL)
        {
                btnm_img_map[0][29] = resource_ui_src_alloc("btn_keypad_delete.png", 64, 64);
                btnm_img_map[1][29] = btnm_img_map[0][29];
                btnm_img_map[2][30] = btnm_img_map[0][29];
        }
        if (btnm_img_map[0][30] == NULL)
        {
                btnm_img_map[0][30] = resource_ui_src_alloc("keyboadr_ad.png", 28, 27);
                btnm_img_map[1][30] = btnm_img_map[0][30];
        }
        if (btnm_img_map[0][40] == NULL)
        {
                btnm_img_map[0][40] = resource_ui_src_alloc("keyboard_40.png", 224, 13);
                btnm_img_map[1][40] = btnm_img_map[0][40];
                btnm_img_map[2][42] = btnm_img_map[0][40];
        }
        if (btnm_img_map[0][41] == NULL)
        {
                btnm_img_map[0][41] = resource_ui_src_alloc("Vector45.png", 27, 28);
                btnm_img_map[1][41] = btnm_img_map[0][41];
                btnm_img_map[2][43] = btnm_img_map[0][41];
        }

        lv_keyboard_update_img_map(keyboard, btnm_img_map);

        return keyboard;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 11:43:3
** 说明: 文本输入框
***********************************************/
lv_obj_t *lv_common_textarea_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                    lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                    lv_opa_t default_cursor_opa, uint32_t default_cursor_color, lv_opa_t press_cursor_opa, uint32_t press_cursor_color,
                                    int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                    int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                    const char *string, uint32_t font_default_color, uint32_t font_focused_color, lv_text_align_t align, const lv_font_t *font, int max_char,
                                    int text_offset_y, int anim_time, uint32_t cursor_color)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);

        if (obj != NULL)
        {

                return obj;
        }

        obj = lv_textarea_create(parent);
        lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);

        lv_common_style_set_textarea(obj, string, font_default_color, font_focused_color, align, font, LV_STATE_FOCUSED, LV_PART_MAIN, max_char);
        lv_common_style_set_event(obj, cb, true, default_bg_opa, default_bg_color, LV_PART_MAIN, press_bg_opa, press_bg_color, LV_STATE_FOCUSED);
        lv_common_style_set_event(obj, NULL, true, default_cursor_opa, default_cursor_color, LV_PART_CURSOR, press_cursor_opa, press_cursor_color, LV_STATE_FOCUSED | LV_PART_CURSOR);
        lv_common_style_set_boader(obj, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN);
        lv_common_style_set_boader(obj, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_press_color, LV_STATE_FOCUSED);

        lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_pad_top(obj, text_offset_y, LV_PART_MAIN);
        lv_obj_set_style_anim_time(obj, anim_time, LV_STATE_FOCUSED | LV_PART_CURSOR);

        return obj;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 11:43:3
** 说明: 按钮显示
***********************************************/
lv_obj_t *lv_common_setting_btn_title_sub_info_img_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                                          lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                                          int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                                          int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                                          int title_x, int title_y, int title_w, int title_h, int title_id,
                                                          const char *title_string, uint32_t title_default_color, uint32_t title_press_color, lv_text_align_t title_align, lv_font_t *title_font,
                                                          int sub_x, int sub_y, int sub_w, int sub_h, int sub_id,
                                                          const char *sub_string, uint32_t sub_default_color, uint32_t sub_press_color, lv_text_align_t sub_align, lv_font_t *sub_font,
                                                          int info_x, int info_y, int info_w, int info_h, int info_id,
                                                          const char *info_string, uint32_t info_default_color, uint32_t info_press_color, lv_text_align_t info_align, lv_font_t *info_font,
                                                          int img_x, int img_y, int img_w, int img_h, int img_id,
                                                          const char *img_src, lv_opa_t press_img_recolor_opa, uint32_t press_img_recolor, lv_align_t img_align)
{

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);

        if (obj != NULL)
        {

                return obj;
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置属性
        ***********************************************/
        {
                obj = lv_obj_create(parent);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLLABLE);

                lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);

                lv_common_style_set_event(obj, cb, cb, default_bg_opa, default_bg_color, LV_PART_MAIN, press_bg_opa, press_bg_color, LV_STATE_PRESSED);
                lv_common_style_set_boader(obj, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN);
                lv_common_style_set_boader(obj, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_press_color, LV_STATE_PRESSED);
        }
        lv_obj_t *title_obj = NULL, *sub_obj = NULL, *info_obj = NULL, *img_obj = NULL;

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置title
        ***********************************************/
        if ((title_string != NULL) && (title_id >= 0))
        {
                title_obj = lv_common_text_create(obj, title_id, title_x, title_y, title_w, title_h,
                                                  NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                  0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                  title_string, title_default_color, title_press_color, title_align, title_font);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置sub
        ***********************************************/
        if (sub_id >= 0)
        {
                sub_obj = lv_common_text_create(obj, sub_id, sub_x, sub_y, sub_w, sub_h,
                                                NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                sub_string, sub_default_color, sub_press_color, sub_align, sub_font);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置info
        ***********************************************/
        if (info_id >= 0)
        {
                info_obj = lv_common_text_create(obj, info_id, info_x, info_y, info_w, info_h,
                                                 NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 info_string, info_default_color, info_press_color, info_align, info_font);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置img
        ***********************************************/
        if (/*(img_src != NULL) &&*/ (img_id >= 0))
        {

                img_obj = lv_common_img_btn_create(obj, img_id, img_x, img_y, img_w, img_h,
                                                   NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                   img_src, press_img_recolor_opa, press_img_recolor, img_align);
        }

        lv_obj_t *bind_obj = obj;

        if (title_obj != NULL)
        {
                lv_obj_set_state_bind(bind_obj, title_obj);
                bind_obj = title_obj;
        }

        if (sub_obj != NULL)
        {
                lv_obj_set_state_bind(bind_obj, sub_obj);
                bind_obj = sub_obj;
        }

        if (info_obj != NULL)
        {
                lv_obj_set_state_bind(bind_obj, info_obj);
                bind_obj = info_obj;
        }

        if (img_obj != NULL)
        {
                lv_obj_set_state_bind(bind_obj, img_obj);
                bind_obj = img_obj;
        }

        return obj;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 11:43:3
** 说明: 滚筒
***********************************************/
lv_obj_t *lv_common_roller_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                  lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color, lv_opa_t press_bg_opa, uint32_t press_bg_color,
                                  int defalut_radius, int defalut_boarder_width, lv_border_side_t default_boarder_side, int boarder_default_opa, uint32_t boarder_default_color,
                                  int press_radius, int press_boarder_width, lv_border_side_t press_boarder_side, int boarder_press_opa, uint32_t boarder_press_color,
                                  int row_cont, int line_space, int min, int max, uint32_t default_text_color, uint32_t select_text_color, lv_text_align_t text_align, lv_font_t *text_font,
                                  const char *img_src)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);

        if (obj != NULL)
        {

                return obj;
        }

        obj = lv_roller_create(parent);

        lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);

        lv_common_style_set_event(obj, cb, cb, default_bg_opa, default_bg_color, LV_PART_MAIN, press_bg_opa, press_bg_color, LV_STATE_PRESSED);
        lv_common_style_set_boader(obj, defalut_radius, boarder_default_opa, defalut_boarder_width, default_boarder_side, boarder_default_color, LV_PART_MAIN);
        lv_common_style_set_boader(obj, press_radius, boarder_press_opa, press_boarder_width, press_boarder_side, boarder_press_color, LV_STATE_PRESSED);
        lv_common_style_set_text(obj, NULL, default_text_color, select_text_color, text_align, text_font, LV_PART_SELECTED, LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(obj, img_src, LV_PART_MAIN);
        lv_obj_set_style_anim_time(obj, 500, LV_PART_MAIN);
        lv_obj_set_style_text_line_space(obj, line_space, LV_PART_MAIN);
        lv_roller_set_visible_row_count(obj, row_cont);
        lv_obj_set_ext_click_area(obj, 54);
        char opt[512] = {0};
        for (int i = min; i <= max; i++)
        {
                char buf[8] = {0};
                sprintf(buf, "%02d%s", i, i == max ? "" : "\n");
                strcat(opt, buf);
        }
        lv_roller_set_options(obj, opt, LV_ROLLER_MODE_NORMAL);

        return obj;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 11:43:3
** 说明: 按钮显示
***********************************************/
static void lv_common_slider_change_obj_cb(lv_event_t *ev)
{
        lv_obj_t *slider = lv_event_get_current_target(ev);
        lv_obj_t *label = lv_event_get_user_data(ev);
        int value = lv_slider_get_value(slider);
        // printf("value is %d\n",value);
        // if(value > 99)
        // {
        //         lv_label_set_text_fmt(label, "%d", value);
        // }else
        // {
        //         lv_label_set_text_fmt(label, "%02d", value);
        // }
        lv_label_set_text_fmt(label, "%02d", value);
}
static void lv_common_slider_left_obj_click(lv_event_t *ev)
{
        lv_obj_t *slider = (lv_obj_t *)lv_event_get_user_data(ev);
        char key = LV_KEY_LEFT;
        lv_event_send(slider, LV_EVENT_KEY, &key);
}
static void lv_common_slider_right_obj_click(lv_event_t *ev)
{
        lv_obj_t *slider = (lv_obj_t *)lv_event_get_user_data(ev);

        char key = LV_KEY_RIGHT;
        lv_event_send(slider, LV_EVENT_KEY, &key);
}
lv_obj_t *lv_common_slider_create(lv_obj_t *parent, int id, int x, int y, int w, int h,
                                  lv_event_cb_t cb, lv_opa_t default_bg_opa, uint32_t default_bg_color,
                                  int bg_radius, int bg_boarder_width, lv_border_side_t bg_boarder_side, int bg_boarder_opa, uint32_t bg_boarder_color,
                                  int slider_radius, int slider_boarder_width, lv_border_side_t slider_boarder_side, int slider_boarder_opa, uint32_t slider_boarder_color,

                                  int text_x, int text_y, int text_w, int text_h, int text_id,
                                  const char *text_string, uint32_t text_font_color, lv_text_align_t text_align, lv_font_t *text_font,

                                  int slider_x, int slider_y, int slider_w, int slider_h, int slider_id,
                                  lv_opa_t slider_bg_opa, uint32_t slider_bg_color, lv_opa_t slider_foure_opa, uint32_t slider_foure_color,

                                  int left_btn_x, int left_btn_y, int left_btn_w, int left_btn_h, int left_btn_id,
                                  const char *left_btn_img_src, lv_opa_t left_btn_press_img_recolor_opa, uint32_t left_btn_press_img_recolor, lv_align_t left_btn_img_align,

                                  int right_btn_x, int right_btn_y, int right_btn_w, int right_btn_h, int right_btn_id,
                                  const char *right_btn_img_src, lv_opa_t right_btn_press_img_recolor_opa, uint32_t right_btn_press_img_recolor, lv_align_t right_btn_img_align,

                                  int knob_radius, int knob_size, uint32_t knob_bg_color, lv_opa_t knob_opa, const char *knob_img_src,
                                  int min, int max, int cur_value)
{

        lv_obj_t *obj = lv_obj_get_child_form_id(parent, id);
        if (obj != NULL)
        {
                return obj;
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置属性
        ***********************************************/
        {
                obj = lv_obj_create(parent);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLLABLE);

                lv_common_style_set_common(obj, id, x, y, w, h, LV_ALIGN_DEFAULT, LV_PART_MAIN);
                lv_common_style_set_event(obj, NULL, false, default_bg_opa, default_bg_color, LV_PART_MAIN, default_bg_opa, default_bg_color, LV_STATE_PRESSED);
                lv_common_style_set_boader(obj, bg_radius, bg_boarder_opa, bg_boarder_width, bg_boarder_side, bg_boarder_color, LV_PART_MAIN);
        }
        lv_obj_t *text_obj = NULL, *slider_obj = NULL, *left_btn_obj = NULL, *right_btn_obj = NULL;

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 设置title
        ***********************************************/
        if ((text_id >= 0))
        {
                text_obj = lv_common_text_create(obj, text_id, text_x, text_y, text_w, text_h,
                                                 NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 text_string, text_font_color, text_font_color, text_align, text_font);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-31 16:43:19
        ** 说明: 绘制滑块
        ***********************************************/
        {
                slider_obj = lv_obj_get_child_form_id(obj, slider_id);
                if (slider_obj == NULL)
                {
                        slider_obj = lv_slider_create(obj);
                        lv_common_style_set_common(slider_obj, slider_id, slider_x, slider_y, slider_w, slider_h, LV_ALIGN_DEFAULT, LV_PART_MAIN);
                        lv_common_style_set_boader(slider_obj, slider_radius, slider_boarder_opa, slider_boarder_width, slider_boarder_side, slider_boarder_color, LV_PART_MAIN);
                        lv_common_style_set_event(slider_obj, NULL, true, slider_bg_opa, slider_bg_color, LV_PART_MAIN, slider_foure_opa, slider_foure_color, LV_PART_INDICATOR);
                        lv_obj_set_ext_click_area(slider_obj, 30);
                        if (text_obj != NULL)
                        {
                                lv_obj_add_event_cb(slider_obj, lv_common_slider_change_obj_cb, LV_EVENT_VALUE_CHANGED, text_obj);
                        }
                        if (cb != NULL)
                        {
                                lv_obj_add_event_cb(slider_obj, cb, LV_EVENT_VALUE_CHANGED, NULL);
                        }

                        lv_bar_set_range(slider_obj, min, max);
                        lv_slider_set_mode(slider_obj, LV_SLIDER_MODE_NORMAL);
                        lv_bar_set_value(slider_obj, cur_value, LV_ANIM_OFF);

                        lv_obj_set_style_radius(slider_obj, knob_radius, LV_PART_KNOB);
                        lv_obj_set_style_pad_all(slider_obj, knob_size, LV_PART_KNOB);
                        lv_obj_set_style_bg_opa(slider_obj, knob_opa, LV_PART_KNOB);
                        lv_obj_set_style_bg_color(slider_obj, lv_color_hex(knob_bg_color), LV_PART_KNOB);
                        if (knob_img_src != NULL)
                        {
                                lv_obj_set_style_bg_img_src(slider_obj, knob_img_src, LV_PART_KNOB);
                        }
                }
        }

        if (left_btn_id >= 0)
        {
                left_btn_obj = lv_common_img_btn_create(obj, left_btn_id, left_btn_x, left_btn_y, left_btn_w, left_btn_h,
                                                        NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        left_btn_img_src, left_btn_press_img_recolor_opa, left_btn_press_img_recolor, left_btn_img_align);

                lv_obj_add_event_cb(left_btn_obj, lv_common_slider_left_obj_click, LV_EVENT_CLICKED, slider_obj);
                lv_obj_set_ext_click_area(left_btn_obj, 10);
        }

        if (right_btn_id >= 0)
        {
                right_btn_obj = lv_common_img_btn_create(obj, right_btn_id, right_btn_x, right_btn_y, right_btn_w, right_btn_h,
                                                         NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         right_btn_img_src, right_btn_press_img_recolor_opa, right_btn_press_img_recolor, right_btn_img_align);

                lv_obj_add_event_cb(right_btn_obj, lv_common_slider_right_obj_click, LV_EVENT_CLICKED, slider_obj);
                lv_obj_set_ext_click_area(right_btn_obj, 10);
        }
        return obj;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 11:43:3
** 说明: 视频模式选择
***********************************************/
void lv_common_video_mode_enable(bool en)
{
        lv_disp_set_bg_opa(NULL, en == false ? LV_OPA_COVER : LV_OPA_TRANSP);
        lv_disp_t *disp = lv_disp_get_default();
        disp->driver->screen_transp = en == true ? 1 : 0;
}

/************************************************************
** 函数说明: 消息框的背景创建
** 作者: xiaoxiao
** 日期: 2023-05-13 09:04:12
** 参数说明:
** 注意事项:
************************************************************/
lv_obj_t *setting_msgdialog_msg_bg_create(int parent_id, int id, int x, int y, int w, int h)
{
        lv_obj_t *cont = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), parent_id);
        if (cont != NULL)
        {
                lv_obj_del(cont);
        }
        cont = lv_common_img_btn_create(sat_cur_layout_screen_get(), parent_id, 0, 0, 1024, 600,
                                        NULL, true, LV_OPA_80, 0, LV_OPA_80, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);
        cont = lv_common_img_btn_create(cont, id, x, y, w, h,
                                        NULL, false, LV_OPA_COVER, 0X242526, LV_OPA_COVER, 0X242526,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        NULL, LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);

        return cont;
}

/************************************************************
** 函数说明: 消息框标语创建
** 作者: xiaoxiao
** 日期: 2023-05-13 09:04:12
** 参数说明:
** 注意事项:
************************************************************/
lv_obj_t *setting_msgdialog_msg_create(lv_obj_t *parent, int id, const char *msg_string, int x, int y, int w, int h, bool title)
{
        int pos_x = 0;
        int pos_y = 0;
        int width = lv_obj_get_style_width(parent, LV_PART_MAIN);
        int height = title ? 80 : lv_obj_get_style_height(parent, LV_PART_MAIN) - 72;
        lv_obj_t *obj = lv_common_text_create(parent, id, pos_x, pos_y, width, height,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 2, title ? LV_BORDER_SIDE_BOTTOM : LV_BORDER_SIDE_NONE, LV_OPA_COVER, 0x323237,
                                              0, 2, title ? LV_BORDER_SIDE_BOTTOM : LV_BORDER_SIDE_NONE, LV_OPA_COVER, 0x323237,
                                              msg_string, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER_CENTER, lv_font_normal);
        // lv_obj_align(obj, LV_ALIGN_CENTER, 0, -80);
        return obj;
}

/************************************************************
** 函数说明: 消息框的确认按键创建
** 作者: xiaoxiao
** 日期: 2023-05-13 09:06:35
** 参数说明:
** 注意事项:
************************************************************/
void setting_msgdialog_msg_confirm_btn_create(lv_obj_t *parent, int id, lv_event_cb_t cb)
{
        int w = lv_obj_get_style_width(parent, LV_PART_MAIN);
        int h = lv_obj_get_style_height(parent, LV_PART_MAIN);
        lv_common_img_btn_create(parent, id, 0, h - 72, w, 72,
                                 cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);
}

/************************************************************
** 函数说明: 消息框的取消确认按键创建
** 作者: xiaoxiao
** 日期: 2023-05-18 11:59:41
** 参数说明:
** 注意事项:
************************************************************/
void setting_msgdialog_msg_confirm_and_cancel_btn_create(lv_obj_t *parent, int confirm_id, int cancel_id, lv_event_cb_t con_cb, lv_event_cb_t can_cb)
{

        int w = lv_obj_get_style_width(parent, LV_PART_MAIN);
        int h = lv_obj_get_style_height(parent, LV_PART_MAIN);
        lv_common_img_text_btn_create(parent, confirm_id, 0 + w / 2, h - 72, w / 2, 72,
                                      con_cb, LV_OPA_COVER, 0x00A8FF, LV_OPA_COVER, 0x0076cf,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 20, w / 2, 72, -1,
                                      "", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                      0, 0, w / 2, 77, 1,
                                      resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_img_text_btn_create(parent, cancel_id, 0, h - 72, w / 2, 72,
                                      can_cb, LV_OPA_COVER, 0x47494a, LV_OPA_COVER, 0x404040,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 20, w / 2, 72, -1,
                                      "", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                      0, 0, w / 2, 77, 1,
                                      resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
}
/************************************************************
** 函数说明: 消息框删除
** 作者: xiaoxiao
** 日期: 2023-05-13 09:04:12
** 参数说明:
** 注意事项:
************************************************************/
void setting_msgdialog_msg_del(int id)
{
        lv_obj_t *msg_bg = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), id);
        if (msg_bg != NULL)
        {
                lv_obj_del(msg_bg);
        }
}
