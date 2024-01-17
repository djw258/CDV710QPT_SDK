#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
        setting_motion_timer_obj_id_title,
        setting_motion_timer_obj_id_cancel,

        setting_motion_timer_obj_id_time_label,

        setting_motion_timer_obj_id_schedule_setting_cont,

        setting_motion_timer_obj_id_timer_cont,

        setting_motion_timer_obj_id_start_year_roller,
        setting_motion_timer_obj_id_start_month_roller,
        setting_motion_timer_obj_id_start_day_roller,
        setting_motion_timer_obj_id_start_hour_roller,
        setting_motion_timer_obj_id_start_min_roller,

        setting_motion_timer_obj_id_end_year_roller,
        setting_motion_timer_obj_id_end_month_roller,
        setting_motion_timer_obj_id_end_day_roller,
        setting_motion_timer_obj_id_end_hour_roller,
        setting_motion_timer_obj_id_end_min_roller,

        setting_motion_timer_obj_id_arrow_1,
        setting_motion_timer_obj_id_arrow_2,
        setting_motion_timer_obj_id_arrow_3,
        setting_motion_timer_obj_id_arrow_4,
        setting_motion_timer_obj_id_arrow_5,
        setting_motion_timer_obj_id_arrow_6,
        setting_motion_timer_obj_id_arrow_7,
        setting_motion_timer_obj_id_arrow_8,
        setting_motion_timer_obj_id_arrow_9,
        setting_motion_timer_obj_id_arrow_10,

        setting_motion_timer_obj_id_1_obj,
        setting_motion_timer_obj_id_2_obj,
        setting_motion_timer_obj_id_3_obj,

        setting_motion_timer_obj_id_4_obj,
        setting_motion_timer_obj_id_5_obj,
        setting_motion_timer_obj_id_6_obj,

        setting_motion_timer_obj_id_start_labelj,
        setting_motion_timer_obj_id_end_label,
};
static int modify;
static void setting_motion_timer_cancel_click(lv_event_t *e)
{
        sat_layout_goto(setting_motion, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}

static void setting_motion_timer_schedule_setting_display(void)
{
        lv_obj_t *item = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_schedule_setting_cont);
        lv_obj_t *obj = lv_obj_get_child_form_id(item, 2);
        if (user_data_get()->motion.timer_en == true)
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
        }
}

/************************************************************
** 函数说明: 移动侦测时间初始化
** 作者: xiaoxiao
** 日期：2023-10-14 16:50:59
** 参数说明:
** 注意事项：
************************************************************/
static void setting_motion_time_param_init(void)
{
        lv_obj_t *year_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_year_roller);
        lv_obj_t *month_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_month_roller);
        lv_obj_t *day_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_day_roller);
        lv_obj_t *hour_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_hour_roller);
        lv_obj_t *minute_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_min_roller);

        lv_obj_t *year_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_year_roller);
        lv_obj_t *month_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_month_roller);
        lv_obj_t *day_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_day_roller);
        lv_obj_t *hour_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_hour_roller);
        lv_obj_t *minute_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_min_roller);

        if (user_data_get()->motion.timer_en)
        {
                lv_roller_set_selected(year_start, user_data_get()->motion.start.tm_year - 2023, false);
                lv_roller_set_selected(month_start, user_data_get()->motion.start.tm_mon - 1, false);
                lv_roller_set_selected(day_start, user_data_get()->motion.start.tm_mday - 1, false);
                lv_roller_set_selected(hour_start, user_data_get()->motion.start.tm_hour - 0, false);
                lv_roller_set_selected(minute_start, user_data_get()->motion.start.tm_min - 0, false);
                lv_roller_set_selected(year_end, user_data_get()->motion.end.tm_year - 2023, false);
                lv_roller_set_selected(month_end, user_data_get()->motion.end.tm_mon - 1, false);
                lv_roller_set_selected(day_end, user_data_get()->motion.end.tm_mday - 1, false);
                lv_roller_set_selected(hour_end, user_data_get()->motion.end.tm_hour - 0, false);
                lv_roller_set_selected(minute_end, user_data_get()->motion.end.tm_min - 0, false);
        }
        else
        {
                struct tm tm;
                user_time_read(&tm);
                lv_roller_set_selected(year_start, tm.tm_year - 2023, false);
                lv_roller_set_selected(month_start, tm.tm_mon - 1, false);
                lv_roller_set_selected(day_start, tm.tm_mday - 1, false);
                lv_roller_set_selected(hour_start, tm.tm_hour - 0, false);
                lv_roller_set_selected(minute_start, tm.tm_min - 0, false);
                lv_roller_set_selected(year_end, tm.tm_year - 2023, false);
                lv_roller_set_selected(month_end, tm.tm_mon - 1, false);
                lv_roller_set_selected(day_end, tm.tm_mday - 1, false);
                lv_roller_set_selected(hour_end, tm.tm_hour - 0, false);
                lv_roller_set_selected(minute_end, tm.tm_min - 0, false);
        }
}

static void setting_motion_timer_cont_display(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont);
        if (user_data_get()->motion.timer_en == true)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}
static void setting_motion_timer_schedule_setting_obj_click(lv_event_t *e)
{
        setting_motion_time_param_init();
        user_data_get()->motion.timer_en = user_data_get()->motion.timer_en == true ? false : true;
        user_data_save(true, true);

        setting_motion_timer_schedule_setting_display();
        setting_motion_timer_cont_display();
}
static void setting_moiton_timer_roller_click(lv_event_t *e)
{

        if (e->code == LV_EVENT_VALUE_CHANGED)
        {
                modify = true;
        }
}

static void setting_motion_timer_time_refresh(lv_timer_t *ptimer)
{
        lv_obj_t *label = (lv_obj_t *)ptimer->user_data;
        struct tm tm;
        user_time_read(&tm);
        lv_label_set_text_fmt(label, "%04d-%02d-%02d  %02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

static void layout_setting_motion_time_save_time(void)
{
        if (modify)
        {
                lv_obj_t *year_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_year_roller);
                lv_obj_t *month_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_month_roller);
                lv_obj_t *day_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_day_roller);
                lv_obj_t *hour_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_hour_roller);
                lv_obj_t *minute_start = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_start_min_roller);

                lv_obj_t *year_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_year_roller);
                lv_obj_t *month_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_month_roller);
                lv_obj_t *day_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_day_roller);
                lv_obj_t *hour_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_hour_roller);
                lv_obj_t *minute_end = lv_obj_get_child_form_id(lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont), setting_motion_timer_obj_id_end_min_roller);
                /***** year *****/
                char buffer[8] = {0};

                lv_roller_get_selected_str(year_start, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.start.tm_year));
                lv_roller_get_selected_str(month_start, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.start.tm_mon));
                lv_roller_get_selected_str(day_start, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.start.tm_mday));
                lv_roller_get_selected_str(hour_start, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.start.tm_hour));
                lv_roller_get_selected_str(minute_start, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.start.tm_min));
                lv_roller_get_selected_str(year_end, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.end.tm_year));
                lv_roller_get_selected_str(month_end, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.end.tm_mon));
                lv_roller_get_selected_str(day_end, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.end.tm_mday));
                lv_roller_get_selected_str(hour_end, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.end.tm_hour));
                lv_roller_get_selected_str(minute_end, buffer, 8);
                sscanf(buffer, "%d", &(user_data_get()->motion.end.tm_min));
                user_data_save(true, true);
        }
}

static void sat_layout_enter(setting_motion_timer)
{
        modify = false;
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 标题显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_MOTION_XLS_LANG_ID_MOTION_DETECTING_SCHEDULE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 时间显示
         ***********************************************/
        {
                lv_obj_t *label = lv_common_text_create(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_time_label, 377, 80, 268, 40,
                                                        NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                        NULL, 0x00a8ff, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal);

                lv_timer_ready(lv_sat_timer_create(setting_motion_timer_time_refresh, 1000, label));
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 上一步下一步
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_cancel, 35, 15, 48, 48,
                                         setting_motion_timer_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:50
         ** 说明: 设置按钮创建
         ***********************************************/
        {
                lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_schedule_setting_cont, 48, 113, 928, 68,
                                                                setting_motion_timer_schedule_setting_obj_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                0, 20, 838, 50, 0,
                                                                lang_str_get(SETTING_MOTION_XLS_LANG_ID_SCHEDULE_SETTING), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                0, 45, 838, 50, -1,
                                                                NULL, 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                0, 0, 0, 0, -1,
                                                                NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                840, 5, 80, 48, 2,
                                                                resource_ui_src_get("btn_switch_on.png"), LV_OPA_50, 0x00a8ff, LV_ALIGN_CENTER);

                setting_motion_timer_schedule_setting_display();
        }

        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_motion_timer_obj_id_timer_cont, 0, 200, 1024, 400,
                                                    NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        setting_motion_timer_cont_display();
        {
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: year roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_1, 248, 0, 104, 201, NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_start_year_roller, 248, 0, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 2023, 2037, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: -
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_motion_timer_obj_id_1_obj, 352, 75, 36, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              "-", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: montth roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_2, 388, 0, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_start_month_roller, 388, 0, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 1, 12, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: -
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_motion_timer_obj_id_2_obj, 492, 75, 36, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              "-", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                }
                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:42:50
                ** 说明: day roller
                ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_3, 528, 0, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_start_day_roller, 528, 0, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 1, 31, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: hour roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_4, 672, 0, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_start_hour_roller, 672, 0, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 0, 23, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: -
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_motion_timer_obj_id_3_obj, 776, 75, 36, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              ":", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: min roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_5, 812, 0, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_start_min_roller, 812, 0, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 0, 59, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: year roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_6, 248, 200, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_end_year_roller, 248, 200, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 2023, 2037, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: -
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_motion_timer_obj_id_4_obj, 352, 275, 36, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              "-", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: montth roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_7, 388, 200, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_end_month_roller, 388, 200, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 1, 12, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: -
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_motion_timer_obj_id_5_obj, 492, 275, 36, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              "-", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                }
                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-2 13:42:50
                ** 说明: day roller
                ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_8, 528, 200, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_end_day_roller, 528, 200, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 1, 31, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: hour roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_9, 672, 200, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_end_hour_roller, 672, 200, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 0, 23, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: -
                 ***********************************************/
                {
                        lv_common_text_create(parent, setting_motion_timer_obj_id_6_obj, 776, 275, 36, 40,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              ":", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:42:50
                 ** 说明: min roller
                 ***********************************************/
                {
                        lv_obj_t *arrow = lv_common_img_btn_create(parent, setting_motion_timer_obj_id_arrow_10, 812, 200, 104, 201,
                                                                   NULL, false, LV_OPA_COVER, 0, LV_OPA_COVER, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   resource_ui_src_get("roller_arrow_up_down.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_BOTTOM_MID);
                        lv_obj_add_flag(arrow, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_t *roller = lv_common_roller_create(parent, setting_motion_timer_obj_id_end_min_roller, 812, 200, 104, 201,
                                                                   setting_moiton_timer_roller_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                                                                   0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x00a8ff,
                                                                   3, 30, 0, 59, 0x303030, 0x00a8ff, LV_TEXT_ALIGN_CENTER, lv_font_normal,
                                                                   resource_ui_src_get("roller_icon.png"));
                        lv_obj_add_event_cb(roller, setting_moiton_timer_roller_click, LV_EVENT_VALUE_CHANGED, NULL);
                }

                lv_common_text_create(parent, setting_motion_timer_obj_id_start_labelj, 90, 70, 66, 43,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_MOTION_XLS_LANG_ID_START), 0XFFFFFFFF, 0XFFFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);

                lv_common_text_create(parent, setting_motion_timer_obj_id_end_label, 90, 270, 66, 43,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_MOTION_XLS_LANG_ID_END), 0XFFFFFFFF, 0XFFFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        setting_motion_time_param_init();
}
static void sat_layout_quit(setting_motion_timer)
{
        layout_setting_motion_time_save_time();
}
sat_layout_create(setting_motion_timer);