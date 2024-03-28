#include "layout_define.h"
#include "layout_setting_general.h"
enum
{
        setting_screen_clean_obj_id_title,
        setting_screen_clean_obj_id_endtime,
        setting_screen_clean_obj_id_value,
};
static int screen_clean_value = 30;
static void setting_screen_clean_timer(lv_timer_t *ptimer)
{
        if (screen_clean_value == 0)
        {
                sat_layout_goto(setting_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
        }
        else
        {
                lv_obj_t *obj = (lv_obj_t *)ptimer->user_data;
                lv_label_set_text_fmt(obj, "%02d:%02d", screen_clean_value / 60, screen_clean_value % 60);
        }
        screen_clean_value--;
}
static void sat_layout_enter(setting_screen_clean)
{
        standby_timer_close();
        screen_clean_value = 30;
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 标题创建
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_screen_clean_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SCREEN_XLS_LANG_ID_LCD_SCREEN_CLEANING_MODE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 结束时间
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_screen_clean_obj_id_endtime, 414, 172, 196, 59,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_STANDBY_SCREEN_XLS_LANG_ID_END_TIME), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large_s);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-3 14:13:25
         ** 说明: 结束时间
         ***********************************************/
        {
                lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), setting_screen_clean_obj_id_value, 322, 250, 381, 176,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_plus);

                lv_timer_ready(lv_sat_timer_create(setting_screen_clean_timer, 1000, obj));
        }
}
static void sat_layout_quit(setting_screen_clean)
{
        standby_timer_restart(true, true);
}

sat_layout_create(setting_screen_clean);