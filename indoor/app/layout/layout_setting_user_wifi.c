#include "layout_define.h"
#include "layout_setting_general.h"
#include "layout_home.h"
#include "layout_setting_wifi.h"
#include "layout_setting_language.h"
#include "layout_setting_user_wifi.h"
#include "layout_wifi_input.h"
#include "layout_wifi_info.h" // 临时用默认代替
static wifi_info wifi_connected_info = {0};
static bool wifi_connected_status = false;
enum
{
        setting_user_wifi_obj_id_wifi_wireless_title,
        setting_user_wifi_obj_id_cancel,
        setting_user_wifi_obj_id_main_list,

        setting_user_wifi_obj_id_wifi_cont,

        setting_user_wifi_obj_id_wifi_add_cont,

        setting_user_wifi_obj_id_wifi_connected_user_cont,

        setting_user_wifi_obj_id_wifi_connected_user_delete,

        setting_user_wifi_obj_id_wifi_discovered_user_cont,

        setting_user_wifi_obj_id_list,
};

static void wifi_setting_user_wifi_enable_display(lv_obj_t *obj)
{
        if (user_data_get()->wifi_enable == true)
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_on.png"), LV_PART_MAIN);
        }
        else
        {
                lv_obj_set_style_bg_img_src(obj, resource_ui_src_get("btn_switch_off.png"), LV_PART_MAIN);
        }
}
static void setting_user_wifi_disable_obj_hidden(void)
{
        lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_add_cont);
        if (obj)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_connected_user_cont);
        if (obj)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_discovered_user_cont);
        if (obj)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_list);
        if (obj)
        {
                lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
}
static void setting_user_wifi_setting_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_cont);
        lv_obj_t *img = lv_obj_get_child_form_id(parent, 2);

        user_data_get()->wifi_enable = user_data_get()->wifi_enable ? false : true;
        user_data_save();
        wifi_setting_user_wifi_enable_display(img);

        if (user_data_get()->wifi_enable == false)
        {
                //     setting_user_wifi_disable_obj_hidden();
                wifi_device_close();
        }
        else
        {
                wifi_device_open();
        }
        sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}
static void setting_user_wifi_discover_click(lv_event_t *e)
{
        lv_obj_t *parent = lv_event_get_current_target(e);
        lv_obj_t *obj = lv_obj_get_child_form_id(parent, 0);
        if (obj == NULL)
        {
                SAT_DEBUG("  lv_obj_t* obj = lv_obj_get_child_form_id(parent,0);");
                return;
        }
        wifi_input_user_setting(lv_label_get_text(obj));

        sat_layout_goto(wifi_input, LV_SCR_LOAD_ANIM_MOVE_TOP, SAT_VOID);
}
static const char *wifi_setting_user_wifi_free_icon_get(const wifi_info *info)
{
        if (info->db < -100)
        {
                return resource_ui_src_get("11_list_wifi_01.png");
        }
        else if (info->db < -88)
        {
                return resource_ui_src_get("12_list_wifi_02.png");
        }
        else if (info->db < -66)
        {
                return resource_ui_src_get("13_list_wifi_03.png");
        }
        else if (info->db < -55)
        {
                return resource_ui_src_get("14_list_wifi_04.png");
        }
        return resource_ui_src_get("15_list_wifi_05.png");
}
static const char *wifi_setting_user_wifi_psk_icon_get(const wifi_info *info)
{
        if (info->db < -100)
        {
                return resource_ui_src_get("21_list_wifi_s_01.png");
        }
        else if (info->db < -88)
        {
                return resource_ui_src_get("22_list_wifi_s_02.png");
        }
        else if (info->db < -66)
        {
                return resource_ui_src_get("23_list_wifi_s_03.png");
        }
        else if (info->db < -55)
        {
                return resource_ui_src_get("24_list_wifi_s_04.png");
        }
        return resource_ui_src_get("25_list_wifi_s_05.png");
}
static const char *wifi_setting_user_wifi_icon_get(const wifi_info *info)
{
        if (info->free == true)
        {
                return wifi_setting_user_wifi_free_icon_get(info);
        }

        return wifi_setting_user_wifi_psk_icon_get(info);
}
static void settign_wifi_connected_click(lv_event_t *e)
{
        connected_wifi_info_setting((void *)&wifi_connected_info);
        sat_layout_goto(wifi_info, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
static lv_obj_t *setting_user_wifi_discovered_network_list_create(void)
{
        lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_list);
        if (list != NULL)
        {
                return list;
        }
        list = lv_list_create(sat_cur_layout_screen_get());
        return list;
}


static void setting_wifi_delete_connected_wifi_info(lv_event_t *e)
{
        wifi_api_reset_default();
        wifi_device_conneting();
        wifi_device_close();
        wifi_device_open();
        
        sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
}

static void settign_wifi_add_click(lv_event_t *ev)
{
        wifi_input_user_setting(NULL);
        sat_layout_goto(wifi_input, LV_SCR_LOAD_ANIM_MOVE_TOP, SAT_VOID);
}
static void setting_user_wifi_discovered_network_display(void)
{
        int list_item_y = 216;
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明: 加载连接的设备
        ***********************************************/
        unsigned char ip[32] = {0};
        wifi_connected_status = false;
        wifi_device_connection_stauts((unsigned char *)(wifi_connected_info.name), &wifi_connected_info.free, ip, &wifi_connected_status, NULL);
        wifi_info *p_wifi_info_group;
        int total = 0;
        if (wifi_device_scanf_info_get(&p_wifi_info_group, &total) == true)
        {
                for (int i = 0; i < total; i++)
                {
                        if (strcmp(p_wifi_info_group[i].name, wifi_connected_info.name) == 0)
                        {
                                wifi_connected_info.db = p_wifi_info_group[i].db;
                        }
                }
        }
        if (wifi_connected_status == true)
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_connected_user_cont, 354, 216, 622, 72,
                                              settign_wifi_connected_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                              0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                              58, 20, 558, 64, 0,
                                              wifi_connected_info.name, 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                              0, 8, 48, 48, 2,
                                              wifi_setting_user_wifi_icon_get(&wifi_connected_info), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_connected_user_delete,920, 216 + 12, 48, 48,
                                                setting_wifi_delete_connected_wifi_info, true, LV_OPA_TRANSP, 0x0096FF, LV_OPA_TRANSP, 0x0096FF,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                resource_ui_src_get("btn_list_delete.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                list_item_y += 72;
        }

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:46:56
        ** 说明:显示搜索的文本
        ***********************************************/
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_discovered_user_cont, 306, list_item_y + 10, 718, 48,
                                        settign_wifi_add_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                        16, 13, 300, 27, 0,
                                        layout_setting_wifi_language_get(WIFI_SETTING_ID_LANG_DISCOVERED_NETWORKS), 0x00a8ff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                        590, 16, 80, 32, 2,
                                        (const char *)resource_ui_src_get("wifi_add.png"), LV_OPA_50, 0x00a8ff, LV_ALIGN_CENTER);
                list_item_y += 58;
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:46:56
         ** 说明:显示搜索的wifi
         ***********************************************/
        {
                lv_obj_t *list = setting_user_wifi_discovered_network_list_create();
                lv_common_style_set_common(list, setting_user_wifi_obj_id_list, 354, list_item_y, 622, (600 - list_item_y), LV_ALIGN_TOP_LEFT, LV_PART_MAIN);
                for (int i = 0; i < total; i++)
                {
                        lv_common_img_text_btn_create(list, i, 0, list_item_y, 622, 72,
                                                      setting_user_wifi_discover_click, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                                      0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                      0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                      58, 20, 564, 64, 0,
                                                      p_wifi_info_group[i].name, 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                      0, 8, 48, 48, 2,
                                                      (const char *)wifi_setting_user_wifi_icon_get(&p_wifi_info_group[i]), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);

                        list_item_y += 72;
                }
        }
}

static void wifi_display_status_timer(lv_timer_t *pt)
{
        bool conneted = false;
        wifi_device_connection_stauts(NULL, NULL, NULL, &conneted, NULL);
        if (wifi_connected_status != conneted)
        {
                sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
        }
}
static void wifi_display_scan_timer(lv_timer_t *pt)
{
        if (user_data_get()->wifi_enable == true)
        {
                wifi_info *info = NULL;
                int total = 0;
                wifi_device_scanf();
                wifi_device_scanf_info_get(&info, &total);
                static wifi_info *p_wifi_info_group = NULL;
                static int wifif_info_total = 0;
                if (wifif_info_total != total)
                {
                        if (p_wifi_info_group != NULL)
                        {
                                lv_mem_free(p_wifi_info_group);
                        }
                        p_wifi_info_group = lv_mem_alloc(sizeof(wifi_info) * total);
                        lv_memcpy_small(p_wifi_info_group, info, sizeof(wifi_info) * total);
                        wifif_info_total = total;
                        sat_layout_goto(setting_user_wifi, LV_SCR_LOAD_ANIM_NONE, SAT_VOID);
                }
        }
        lv_timer_del(pt);
}
static void sat_layout_enter(setting_user_wifi)
{

        setting_main_list_create(1);

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-2-2 13:42:50
        ** 说明: 设置按钮创建
        ***********************************************/
        {
                lv_obj_t *parent = lv_common_setting_btn_title_sub_info_img_create(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_cont, 354, 80, 622, 88,
                                                                                   setting_user_wifi_setting_click, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                                                                   0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
                                                                                   0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                                   0, 8, 400, 50, 0,
                                                                                   layout_setting_wifi_language_get(WIFI_SETTING_ID_LANG_WIFI), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                   0, 45, 542, 50, 1,
                                                                                   "Wi-FI MAC: AA:00:00:BB:FF:88", 0x6d6d79, 0x00484f, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                                                                   0, 0, 0, 0, -1,
                                                                                   NULL, 0xFFFFFF, 0x0078Cf, LV_TEXT_ALIGN_LEFT, lv_font_normal,
                                                                                   540, 20, 80, 48, 2,
                                                                                   resource_ui_src_get("btn_switch_on.png"), LV_OPA_50, 0x00a8ff, LV_ALIGN_CENTER);

                lv_obj_t *obj = lv_obj_get_child_form_id(parent, 2);
                wifi_setting_user_wifi_enable_display(obj);
        }
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-2-2 13:42:50
         ** 说明: 添加wifi
         ***********************************************/
        {
                lv_common_img_text_btn_create(sat_cur_layout_screen_get(), setting_user_wifi_obj_id_wifi_add_cont, 306, 168, 718, 48,
                                              NULL, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              16, 13, 300, 27, 0,
                                              layout_setting_wifi_language_get(WIFI_SETTING_ID_LANG_ADD_NETWORKS), 0x00a8ff, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_small,
                                              590, 16, 80, 32, -1,
                                              NULL, LV_OPA_50, 0x00a8ff, LV_ALIGN_CENTER);
        }
        setting_user_wifi_discovered_network_display();

        if (user_data_get()->wifi_enable == false)
        {
                setting_user_wifi_disable_obj_hidden();
        }
        lv_sat_timer_create(wifi_display_status_timer, 1000, NULL);

        lv_sat_timer_create(wifi_display_scan_timer, 3000, NULL);
}

static void sat_layout_quit(setting_user_wifi)
{
}
sat_layout_create(setting_user_wifi);
