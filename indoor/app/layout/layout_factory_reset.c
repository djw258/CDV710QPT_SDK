#include "layout_define.h"
#include "tuya/tuya_api.h"
enum
{
        factory_reset_obj_id_title,
        factory_reset_obj_id_cancel,
        factory_reset_obj_id_warn_tips,
        factory_reset_obj_id_param1,
        factory_reset_obj_id_param2,
        factory_reset_obj_id_confirm,
        factory_reset_obj_id_msgbox_bg,
        factory_reset_obj_id_msgbox_cont,
        factory_reset_obj_id_msgbox_text,
        factory_reset_obj_id_format_process
};
static void factory_reset_cancel_click(lv_event_t *ev)
{
        sat_layout_goto(setting_installation, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

/************************************************************
** 函数说明: 恢复出厂设置过程动画场景
** 作者: xiaoxiao
** 日期：2023-09-12 09:50:26
** 参数说明:
** 注意事项：
************************************************************/
static void layout_factory_reset_timer(lv_timer_t *t)
{
        lv_obj_t *obj = (lv_obj_t *)t->user_data;
        lv_obj_t *bar = lv_obj_get_child_form_id(obj, factory_reset_obj_id_format_process);
        char result = 0;
        if (obj && bar)
        {
                result = *(char *)(obj->user_data);
                lv_bar_set_value(bar, result, LV_ANIM_ON);
                if (result == 6)
                {
                        backlight_enable(false);
                        usleep(100 * 1000);
                        system("reboot");
                }
        }
}

/************************************************************
** 函数说明: 恢复出厂设置进程
** 作者: xiaoxiao
** 日期：2023-09-12 09:48:49
** 参数说明:
** 注意事项：
************************************************************/
static void *layout_factory_reset_process(void *arg)
{
        char *reseted = (char *)arg;
        *reseted = 0;
        user_data_reset();
        *reseted += 1;
        usleep(100 * 1000); // 加睡眠，只是为了能给用户展示在格式的过程，因为格式流程太快了。
        network_data_reset();
        *reseted += 1;
        usleep(100 * 1000);
        tuay_api_data_reset();
        *reseted += 1;
        usleep(100 * 1000);
        media_file_delete_all(FILE_TYPE_FLASH_PHOTO, true);
        *reseted += 1;
        usleep(100 * 1000);
        alarm_list_del_all();
        *reseted += 1;
        usleep(100 * 1000);
        call_list_del_all();
        *reseted += 1;
        usleep(100 * 1000);
        wifi_api_reset_default();
        *reseted += 1;
        usleep(100 * 1000);
        return NULL;
}

/************************************************************
** 函数说明: 恢复出厂设置任务创建
** 作者: xiaoxiao
** 日期：2023-09-12 09:48:19
** 参数说明:
** 注意事项：
************************************************************/
static void layout_factory_reset_confirm(lv_event_t *ev)
{
        lv_obj_t *masgbox = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), factory_reset_obj_id_msgbox_bg);
        if (masgbox != NULL)
        {
                setting_msgdialog_msg_del(factory_reset_obj_id_msgbox_bg);
        }
        masgbox = setting_msgdialog_msg_bg_create(factory_reset_obj_id_msgbox_bg, factory_reset_obj_id_msgbox_cont, 282, 143, 460, 283);
        setting_msgdialog_msg_create(masgbox, factory_reset_obj_id_msgbox_text, lang_str_get(INSTALLATION_XLS_LANG_ID_FACTORY_RESET_PROCESS), 0, 120, 460, 120, false);
        static lv_style_t style_bg;
        static lv_style_t style_indic;

        lv_style_init(&style_bg);
        lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_border_width(&style_bg, 2);
        lv_style_set_pad_all(&style_bg, 0); /*To make the indicator smaller*/
        lv_style_set_radius(&style_bg, 0);

        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_radius(&style_indic, 3);

        lv_obj_t *bar = lv_bar_create(masgbox);
        lv_obj_set_id(bar, factory_reset_obj_id_format_process);
        lv_obj_remove_style_all(bar); /*To have a clean start*/
        lv_obj_add_style(bar, &style_bg, 0);
        lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);

        lv_common_style_set_common(bar, factory_reset_obj_id_format_process, 50, 220, 360, 24, LV_ALIGN_DEFAULT, LV_PART_MAIN);
        lv_bar_set_range(bar, 0, 6);
        lv_bar_set_value(bar, 0, LV_ANIM_ON);
        static char reseted = false;
        masgbox->user_data = &reseted;
        lv_timer_ready(lv_sat_timer_create(layout_factory_reset_timer, 100, masgbox));
        pthread_t task_id;
        pthread_create(&task_id, sat_pthread_attr_get(), layout_factory_reset_process, &reseted);
        pthread_detach(task_id);
}

static void sat_layout_enter(factory_reset)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 标题显示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), factory_reset_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(INSTALLATION_XLS_LANG_ID_FACTORY_RESET),
                                      0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: xiaoxiao
        ** 日期：2023-09-12 10:18:24
        ** 说明: 返回上一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), factory_reset_obj_id_cancel, 35, 15, 48, 48,
                                         factory_reset_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
        ** 作者: xiaoxiao
        ** 日期：2023-09-12 08:49:41
        ** 说明: 格式化注意事项提示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), factory_reset_obj_id_warn_tips, 0, 137, 1024, 38,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_INITIALIZE_XLS_LANG_ID_DATA_CANNOT_BE),
                                      0XFF5B5B, 0XFF5B5B, LV_TEXT_ALIGN_CENTER, lv_font_small);
        }
        /***********************************************
        ** 作者: xiaoxiao
        ** 日期：2023-09-12 08:54:10
        ** 说明: 格式化参数提示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), factory_reset_obj_id_param1, 0, 232, 1024, 144,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(FACTORY_RESET_XLS_LANG_ID_ALL_DATA_RESET),
                                      0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
        }
        /***********************************************
        ** 作者: xiaoxiao
        ** 日期：2023-09-12 08:54:10
        ** 说明: 格式化参数提示
        ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), factory_reset_obj_id_param2, 0, 350, 1024, 38,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_INITIALIZE_XLS_LANG_ID_FINISH_AND_REBOOT),
                                      0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);
        }

        /***********************************************
        ** 作者: xiaoxiao
        ** 日期：2023-09-12 09:20:04
        ** 说明:
        ***********************************************/
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), factory_reset_obj_id_confirm, 0, 528, 1024, 72,
                                              layout_factory_reset_confirm, LV_OPA_COVER, 0x00A8FF, LV_OPA_COVER, 0X101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              400, 17, 224, 27, 0,
                                              lang_str_get(SETTING_INITIALIZE_XLS_LANG_ID_RESET), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                              3, 0, 77, 77, -1,
                                              NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
}
static void sat_layout_quit(factory_reset)
{
        if (user_data_get()->is_device_init == true) // 启动设置会有机会进入这里，所以要加判断
        {
                standby_timer_restart(true, true);
        }
        ipcamera_state_callback_register(NULL);
        // for(int i = 0; i < network_data_get()->cctv_device_count; i++)
        // {
        //         char ch[5];
        //         strncpy(ch,network_data_get()->cctv_device[i].door_name,5);
        //         if(strncmp(ch,"CCTV1",5) == 0)
        //         {
        //                 continue;;
        //         }

        // }
}

sat_layout_create(factory_reset);
