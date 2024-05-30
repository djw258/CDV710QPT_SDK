#include "layout_define.h"
#include "layout_setting_general.h"
#define GOOLE_APP "https://play.google.com/store/apps/details?id=com.commaxcall.ipvideophone2"
#define APPLE_APP "https://apps.apple.com/kr/app/commax-hey-call/id1585757748"
#include "tuya/tuya_ipc_api.h"
#include "tuya/tuya_api.h"
enum
{

        setting_download_obj_id_title,
        setting_download_obj_id_cancel,
        setting_download_obj_id_google_play_store,
        setting_download_obj_id_apple_app_store,
        setting_download_obj_id_google_play_qrcode,
        setting_download_obj_id_apple_app_qrcode,
        setting_download_obj_id_video_call_with_text,
        setting_download_obj_id_binding_device_qrcode,
};
static void setting_download_cancel_obj_click(lv_event_t *e)
{
        sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
}
static void sat_layout_enter(setting_download)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: 标题显示
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_download_obj_id_title, 0, 20, 1024, 40,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_GENERAL_XLS_LANG_ID_DOWNLOAD_MOBILE_APP), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:25
        ** 说明: 上一步
        ***********************************************/
        {
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_download_obj_id_cancel, 35, 15, 48, 48,
                                         setting_download_cancel_obj_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                         resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
        }

        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明: google store
         ***********************************************/
        {
                lv_common_text_create(sat_cur_layout_screen_get(), setting_download_obj_id_google_play_store, 107, 94, 298, 35,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_DOWNLOAD_XLS_LANG_ID_GOOGLE_PLAY_STORE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);

                lv_common_text_create(sat_cur_layout_screen_get(), setting_download_obj_id_apple_app_store, 630, 94, 298, 35,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_DOWNLOAD_XLS_LANG_ID_GOOGLE_APP_STORE), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);

                lv_common_text_create(sat_cur_layout_screen_get(), setting_download_obj_id_apple_app_qrcode, 299, 428, 426, 120,
                                      NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                      lang_str_get(SETTING_DOWNLOAD_XLS_LANG_ID_VIDEO_CALL_WITH_MOBILE_APP), 0x808080, 0x808080, LV_TEXT_ALIGN_CENTER, lv_font_normal);
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 二维码
        ***********************************************/
        {
                lv_obj_t *qr = lv_qrcode_create(sat_cur_layout_screen_get(), 165, lv_color_hex(0x00000), lv_color_hex(0xFFFFFFF));
                lv_qrcode_update(qr, GOOLE_APP, strlen(GOOLE_APP) + 1);
                lv_common_style_set_common(qr, setting_download_obj_id_google_play_qrcode, 156, 149, 210, 210, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                lv_common_style_set_event(qr, NULL, false, LV_OPA_COVER, 0xFFFFFF, LV_PART_MAIN, LV_OPA_COVER, 0xFFFFFF, LV_PART_MAIN);
                lv_common_style_set_boader(qr, 10, LV_OPA_COVER, 1, LV_BORDER_SIDE_FULL, 0xFFFFFF, LV_PART_MAIN);

                qr = lv_qrcode_create(sat_cur_layout_screen_get(), 165, lv_color_hex(0x00000), lv_color_hex(0xFFFFFFF));
                lv_qrcode_update(qr, APPLE_APP, strlen(APPLE_APP) + 1);
                lv_common_style_set_common(qr, setting_download_obj_id_apple_app_qrcode, 669, 149, 210, 210, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                lv_common_style_set_event(qr, NULL, false, LV_OPA_COVER, 0xFFFFFF, LV_PART_MAIN, LV_OPA_COVER, 0xFFFFFF, LV_PART_MAIN);
                lv_common_style_set_boader(qr, 10, LV_OPA_COVER, 1, LV_BORDER_SIDE_FULL, 0xFFFFFF, LV_PART_MAIN);

                // for (int i = 0; i < 100; i++)
                // {
                //         if (1)
                //         {
                //                 char tuya_qrc_info[256] = {0};
                //                 tuya_ipc_get_qrcode(NULL, tuya_qrc_info, sizeof(tuya_qrc_info));
                //                 SAT_DEBUG("tuya_qrc_info is %s", tuya_qrc_info);
                //                 qr = lv_qrcode_create(sat_cur_layout_screen_get(), 165, lv_color_hex(0x00000), lv_color_hex(0xFFFFFFF));
                //                 lv_qrcode_update(qr, tuya_qrc_info, strlen(tuya_qrc_info) + 1);
                //                 lv_common_style_set_common(qr, setting_download_obj_id_binding_device_qrcode, 418, 149, 210, 210, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                //                 lv_common_style_set_event(qr, NULL, false, LV_OPA_COVER, 0xFFFFFF, LV_PART_MAIN, LV_OPA_COVER, 0xFFFFFF, LV_PART_MAIN);
                //                 lv_common_style_set_boader(qr, 10, LV_OPA_COVER, 1, LV_BORDER_SIDE_FULL, 0xFFFFFF, LV_PART_MAIN);
                //                 break;
                //         }
                //         else
                //         {
                //                 SAT_DEBUG("tuya offline");
                //                 usleep(100 * 1000);
                //         }
                // }
        }
}
static void sat_layout_quit(setting_download)
{
}

sat_layout_create(setting_download);