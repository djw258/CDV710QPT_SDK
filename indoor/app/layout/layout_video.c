#include "layout_define.h"
#include "layout_playback.h"
#define video_WIDTH (1024)
#define video_HIGHT (592)
enum
{
        video_obj_id_top,
        video_obj_id_play,
        video_obj_id_left,
        video_obj_id_right,
        video_obj_id_buttom,
        video_obj_id_thumb,
        video_obj_id_msgbox,
};
#if 0
static lv_img_dsc_t *video_thumb_img_dsc = NULL;
static lv_point_t *video_thumb_point = NULL;
static void video_thumb_img_dsc_alloc(void)
{
        lv_point_t area = {0, 0};
        if (video_thumb_img_dsc == NULL)
        {
                video_thumb_img_dsc = lv_img_buf_alloc(video_WIDTH, video_HIGHT, LV_IMG_CF_TRUE_COLOR);
                video_thumb_img_dsc->header.reserved = 0x03;
        }
        if (video_thumb_point == NULL)
        {
                video_thumb_point = lv_mem_alloc(sizeof(lv_point_t));
                lv_memcpy_small(video_thumb_point, &area, sizeof(lv_point_t));
        }
}
static void video_thumb_img_dse_destroy(void)
{
        if (video_thumb_img_dsc != NULL)
        {
                lv_img_buf_free(video_thumb_img_dsc);
                video_thumb_img_dsc = NULL;
        }
        if (video_thumb_point != NULL)
        {
                lv_mem_free(video_thumb_point);
                video_thumb_point = NULL;
        }
}
#endif

static lv_obj_t *video_msgbox_create(const char *title, const char *msg, lv_event_cb_t cancel_cb, lv_event_cb_t confirm_cb)
{
        lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_msgbox, 0, 0, 1024, 600,
                                                    NULL, true, LV_OPA_90, 0, LV_OPA_90, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_obj_t *msgbox = lv_common_img_btn_create(parent, 0, 282, 131, 460, 343,
                                                    NULL, false, LV_OPA_COVER, 0x242526, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                    NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

        lv_common_text_create(msgbox, 0, 32, 10, 396, 47,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              0, 2, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                              title, 0XA8A8A8, 0XA8A8A8, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_text_create(msgbox, 1, 32, 90, 396, 200,
                              NULL, LV_OPA_TRANSP, 0x323237, LV_OPA_TRANSP, 0,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0x323237,
                              msg, 0xFFFFFFFF, 0xFFFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_small);

        lv_common_img_btn_create(msgbox, 2, 0, 281, 230, 62,
                                 cancel_cb, true, LV_OPA_COVER, 0x47494A, LV_OPA_COVER, 0x47494A,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_cancel.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        lv_common_img_btn_create(msgbox, 3, 230, 281, 230, 62,
                                 confirm_cb, true, LV_OPA_COVER, 0x0096FF, LV_OPA_COVER, 0x0096FF,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                 resource_ui_src_get("btn_title_confirm.png"), LV_OPA_TRANSP, 0x0096FF, LV_ALIGN_CENTER);

        return parent;
}

static void video_obj_cancel_click(lv_event_t *e)
{
        sat_layout_goto(playback, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}

static void video_info_obj_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_top);
        if (parent == NULL)
        {
                SAT_DEBUG("lv_obj_t* parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_top);");
                return;
        }

        lv_obj_t *label = lv_obj_get_child_form_id(parent, 0);
        if (label == NULL)
        {
                SAT_DEBUG("lv_obj_t *label = lv_obj_get_child_form_id(parent, 0);");
                return;
        }

        const file_info *info = playback_media_info_get();
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_label_set_text_fmt(label, "%s [%05d/%05d]", info->ch, playback_media_total_get() - playback_pview_item_get(), playback_media_total_get());
}
static void video_time_obj_display(void)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_buttom);
        if (parent == NULL)
        {
                SAT_DEBUG("lv_obj_t* parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_buttom);");
                return;
        }

        lv_obj_t *label = lv_obj_get_child_form_id(parent, 0);
        if (label == NULL)
        {
                SAT_DEBUG("lv_obj_t *label = lv_obj_get_child_form_id(parent, 0);");
                return;
        }

        const file_info *info = playback_media_info_get();

        char str[128] = {"2023-04-08  08:30:26"}; // 230408-093026.jpg
        int index_group[6][2] = {
            {2, 0},
            {5, 2},
            {8, 4},
            {12, 7},
            {15, 9},
            {18, 11}};
        for (int i = 0; i < 6; i++)
        {
                str[index_group[i][0]] = info->file_name[index_group[i][1]];
                str[index_group[i][0] + 1] = info->file_name[index_group[i][1] + 1];
        }
        lv_label_set_text(label, str);
}
#if 0
static void video_thumb_refresh_display_callback(void)
{

        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_thumb);
        if (parent == NULL)
        {
                SAT_DEBUG(" lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_thumb);");
                return;
        }
        //    lv_obj_clear_flag(parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_img_src(parent, video_thumb_img_dsc, LV_PART_MAIN);

        /*每次加载完后显示总数*/
        video_time_obj_display();
        video_info_obj_display();
}
#endif
static void video_sd_state_change_callback(void)
{
        if (((media_sdcard_insert_check() == SD_STATE_UNPLUG) || (media_sdcard_insert_check() == SD_STATE_ERROR)) && (playback_pview_type_get() != FILE_TYPE_FLASH_PHOTO))
        {
                sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
}
#if 0
static bool video_thumb_media_display_callback(const char *data, int x, int y, int w, int h)
{
        if ((video_thumb_img_dsc != NULL) && (video_thumb_point != NULL) && (video_thumb_point->x == x) && (video_thumb_point->y == y))
        {
                lv_memcpy_small((uint8_t *)video_thumb_img_dsc->data, data, w * h * 3);
                return true;
        }
        SAT_DEBUG("video display failed(%d,%d,%d,%d)\n", x, y, w, h);
        return false;
}
#endif
static void video_thumb_decode_all_display(void)
{
        /*显示加载的图像*/
        const file_info *pinfo = playback_media_info_get();
        char arry[128] = {0};
        sprintf(arry, "%s%s", playback_pview_path_get(), pinfo->file_name);
        printf("video play :%s\n", arry);
        sat_linphone_video_play_start(arry, 0, 0, video_WIDTH, video_HIGHT);
        //  sat_linphone_media_thumb_display(arry, 1, video_thumb_media_display_callback);
        /*每次加载完后显示总数*/
        video_time_obj_display();
        video_info_obj_display();

        if (pinfo->is_new == true)
        {
                media_file_new_clear(pinfo->type, playback_pview_item_get());
        }
}

static void video_thumb_left_right_arrow_display(void)
{
        lv_obj_t *left = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_left);
        lv_obj_t *right = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_right);
        if ((playback_media_total_get() < 2) || (playback_media_total_get() == (playback_pview_item_get() + 1)))
        {

                lv_obj_add_flag(left, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_clear_flag(left, LV_OBJ_FLAG_HIDDEN);
        }
        if ((playback_media_total_get() < 2) || (playback_pview_item_get() == 0))
        {
                lv_obj_add_flag(right, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_clear_flag(right, LV_OBJ_FLAG_HIDDEN);
        }
}

static void video_media_thumb_obj_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_top);
        if (obj == NULL)
        {
                SAT_DEBUG("lv_obj_t* obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(),video_obj_id_top);");
                return;
        }

        if (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN) == true)
        {
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                video_thumb_left_right_arrow_display();
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_play);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_buttom);
                lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_left);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_right);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_buttom);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_play);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}

static void video_msgbox_del_cancel_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_msgbox);
        if (obj == NULL)
        {
                return;
        }
        lv_obj_del(obj);
}
static void video_del_msgbox_confirm_click(lv_event_t *e)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_msgbox);
        if (obj == NULL)
        {
                return;
        }

        sat_linphone_video_play_stop();
        /*等待视频线程退出*/
        usleep(500 * 1000);
        file_type type = playback_pview_type_get();
        int item = playback_pview_item_get();
        media_file_delete(type, item);

        int total;
        media_file_total_get(type, &total, NULL);
        if (total < 1)
        {
                sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                return;
        }

        playback_media_total_set(total);
        if (item >= total)
        {
                playback_pview_item_set(total - 1);
        }

        int top = playback_media_top_get();
        if (top == 0)
        {
                top = (total > 5) ? 5 : (total - 1);
        }
        else
        {
                top -= 1;
        }
        playback_media_top_set(top);
        lv_obj_del(obj);

        const file_info *info = playback_media_info_get();
        if (info->type != FILE_TYPE_VIDEO)
        {
                sat_layout_goto(photo, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
        }
        else
        {
                video_thumb_decode_all_display();
        }
}

static void video_obj_del_click(lv_event_t *e)
{
        video_msgbox_create(lang_str_get(LAYOUT_CALL_LOG_XLS_LANG_ID_DEL),
                            lang_str_get(SETTING_STORAGE_XLS_LANG_ID_WOULD_YOU_LIKE_DEL),
                            video_msgbox_del_cancel_click, video_del_msgbox_confirm_click);
}

static void video_obj_left_click(lv_event_t *e)
{
        int item = playback_pview_item_get();
        item++;
        if (item >= playback_media_total_get())
        {
                item = 0;
        }
        playback_pview_item_set(item);
        const file_info *info = playback_media_info_get();
        if (info->type != FILE_TYPE_VIDEO)
        {
                sat_layout_goto(photo, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
        }
        else
        {
                sat_layout_goto(video, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
        }
        video_thumb_decode_all_display();
        video_thumb_left_right_arrow_display();
}
static void video_obj_right_click(lv_event_t *e)
{
        int item = playback_pview_item_get();
        item--;
        if (item < 0)
        {
                item = playback_media_total_get() - 1;
        }
        playback_pview_item_set(item);
        const file_info *info = playback_media_info_get();
        if (info->type != FILE_TYPE_VIDEO)
        {
                sat_layout_goto(photo, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
        }
        else
        {
                sat_layout_goto(video, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
        }
        video_thumb_decode_all_display();
        video_thumb_left_right_arrow_display();
}

static void video_thumb_duration_callback(unsigned int cur, unsigned int total)
{

        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_buttom);
        if (parent == NULL)
        {
                SAT_DEBUG("lv_obj_t* parent=  lv_obj_get_child_form_id(sat_cur_layout_screen_get(),video_obj_id_buttom);");
                return;
        }

        parent = lv_obj_get_child_form_id(parent, 1);
        if (parent == NULL)
        {
                SAT_DEBUG(" parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), 1);");
                return;
        }

        lv_obj_t *slider = lv_obj_get_child_form_id(parent, 1);
        if (slider == NULL)
        {
                SAT_DEBUG("lv_obj_t *slider = lv_obj_get_child_form_id(parent, 1);");
                return;
        }
        lv_obj_t *play = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_play);
        if (play == NULL)
        {
                return;
        }

        if ((strncmp(play->bg_img_src, resource_ui_src_get("btn_thumbnail_pause_l.png"), strlen(play->bg_img_src))) && (cur != total))
        {
                return;
        }

        lv_slider_set_value(slider, cur * 100 / total, LV_ANIM_ON);

        lv_obj_t *label = lv_obj_get_child_form_id(parent, 0);
        if (label == NULL)
        {
                SAT_DEBUG("lv_obj_t *slider = lv_obj_get_child_form_id(label, 1);");
                return;
        }
        lv_label_set_text_fmt(label, "%02d:%02d", (total - cur) / 1000 / 60, (total - cur) / 1000 % 60);
        if (cur == total)
        {
                const file_info *info = playback_media_info_get();
                if (info->type != FILE_TYPE_VIDEO)
                {
                        sat_layout_goto(photo, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
                }
                else
                {
                        sat_layout_goto(video, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
                }
        }
}
static void video_thumb_play_state_callback(unsigned int sate)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_play);

        lv_obj_set_style_bg_img_src(obj, resource_ui_src_get(sate == 0x01 ? "btn_thumbnail_pause_l.png" : "btn_thumbnail_play_l.png"), LV_PART_MAIN);
        if (sate == 0x01)
        {
                standby_timer_close();
                video_media_thumb_obj_click(NULL);
        }
        else if (sate == 0x02)
        {
                standby_timer_restart(true);
        }
}

static void video_thumb_duration_timer(lv_timer_t *ptimer)
{
        if (ptimer->period != 1000)
        {
                lv_timer_set_period(ptimer, 1000);
        }
        sat_linphone_video_play_duration_get();
}

static void video_obj_play_click(lv_event_t *e)
{
        sat_linphone_video_play_pause();
}
static void layout_video_touch_callback(lv_event_t *e)
{
        standby_timer_restart(false);
}

static void layout_video_first_frame_callback(int arg1, int arg2)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), video_obj_id_play);
        if (obj != NULL)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
}

static void sat_layout_enter(video)
{
        lv_obj_pressed_func = layout_video_touch_callback;

        first_refresh_lcd_cmd_callback_register(layout_video_first_frame_callback);

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 缩略图显示相关
        ***********************************************/
        {
                //  video_thumb_img_dsc_alloc();
                //  thumb_display_refresh_register(video_thumb_refresh_display_callback);
                sd_state_channge_callback_register(video_sd_state_change_callback);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 预览图片
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_thumb, (int)0, (int)0, video_WIDTH, video_HIGHT,
                                         video_media_thumb_obj_click, true, LV_OPA_TRANSP, 0x242526, LV_OPA_TRANSP, 0x242526,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 顶部控件
        ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_top, 0, 0, 1024, 80,
                                                            NULL, false, LV_OPA_50, 0, LV_OPA_TRANSP, 0x808080,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                {
                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:46:56
                         ** 说明: 标题显示
                         ***********************************************/
                        {
                                lv_common_text_create(parent, 0, 0, 20, 1024, 40,
                                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                      " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                        }
                        /***********************************************
                        ** 作者: leo.liu
                        ** 日期: 2023-2-2 13:42:25
                        ** 说明: 上一步
                        ***********************************************/
                        {
                                lv_common_img_btn_create(parent, 1, 35, 15, 48, 48,
                                                         video_obj_cancel_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                        }
                        /***********************************************
                         ** 作者: leo.liu
                         ** 日期: 2023-2-2 13:42:25
                         ** 说明: 上一步
                         ***********************************************/
                        {
                                lv_common_img_btn_create(parent, 2, 930, 0, 80, 80,
                                                         video_obj_del_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                         resource_ui_src_get("btn_title_delete.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                        }
                }
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 播放
         ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_play, 435, 224, 152, 152,
                                         video_obj_play_click, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_thumbnail_play_l.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 左右切换
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_left, 24, 300, 80, 80,
                                         video_obj_left_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_thumbnail_arrow_left_n.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_right, 919, 300, 80, 80,
                                         video_obj_right_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_thumbnail_arrow_right_n.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                video_thumb_left_right_arrow_display();
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 底部显示
         ***********************************************/
        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), video_obj_id_buttom, 0, 488, 1024, 112,
                                                            NULL, false, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            resource_ui_src_get("plyabck_buttom_bg.png"), LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: 标题显示
                 ***********************************************/
                {
                        lv_common_text_create(parent, 0, 24, 10, 300, 33,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              " ", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_LEFT, lv_font_normal);
                }
                /***********************************************
                 ** 作者: leo.liu
                 ** 日期: 2023-2-2 13:46:56
                 ** 说明: 进度条显示
                 ***********************************************/
                {
                        lv_obj_t *slider = lv_common_slider_create(parent, 1, 0, 0, 1024, 112,
                                                                   NULL, LV_OPA_TRANSP, 0X00,
                                                                   6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   6, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                   900, 10, 100, 33, 0,
                                                                   "00:00", 0Xffffff, LV_TEXT_ALIGN_RIGHT, lv_font_normal,
                                                                   43, 70, 940, 10, 1, LV_OPA_COVER, 0x666666, LV_OPA_COVER, 0x00a8ff,
                                                                   165, 6, 42, 42, -1,
                                                                   NULL, LV_OPA_TRANSP, 0X00, LV_ALIGN_CENTER,
                                                                   567, 6, 42, 42, -1,
                                                                   NULL, LV_OPA_TRANSP, 0x00, LV_ALIGN_CENTER,
                                                                   360, 9, 0Xffffff, LV_OPA_TRANSP, NULL,
                                                                   0, 100, 0);
                        lv_obj_clear_flag(lv_obj_get_child_form_id(slider, 1), LV_OBJ_FLAG_CLICKABLE);
                }
        }

        video_play_duration_callback_register(video_thumb_duration_callback);
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:25
         ** 说明: 刷新缩略图
         ***********************************************/
        video_thumb_decode_all_display();

        lv_common_video_mode_enable(true);

        video_play_state_callback_register(video_thumb_play_state_callback);

        lv_timer_ready(lv_sat_timer_create(video_thumb_duration_timer, 100, NULL));
}
static void sat_layout_quit(video)
{
        standby_timer_restart(true);
        //   sat_linphone_media_thumb_destroy();
        lv_obj_pressed_func = lv_layout_touch_callback;

        lv_common_video_mode_enable(false);
        sat_linphone_video_play_stop();
        sd_state_channge_callback_register(sd_state_change_default_callback);

        video_play_duration_callback_register(NULL);
        video_play_state_callback_register(NULL);
        first_refresh_lcd_cmd_callback_register(NULL);
        //  video_thumb_img_dse_destroy();
}

sat_layout_create(video);