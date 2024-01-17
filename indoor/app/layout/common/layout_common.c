
#include "layout_define.h"
#include "ring_common.h"
#include "user_gpio.h"
#include "common/commax_websocket.h"
#define RESOURCE_FILE_PATH_MAX 128

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 获取唯一的字符串
 * @return:
 */
static char *resource_src_get(const char *path, const char *file, int w, int h, bool alloc)
{
        int file_length = 0;

        if (path != NULL)
        {
                file_length += strlen(path);
        }

        if (file != NULL)
        {
                file_length += strlen(file);
        }

        if (file_length > RESOURCE_FILE_PATH_MAX)
        {

                LV_LOG_ERROR("recource path length too long");
                return NULL;
        }

        int resouce_string_size = file_length + 1 + sizeof(uint32_t) * 2 + 1;

        char *resource_string = NULL;

        if (alloc == false)
        {
                static char string[RESOURCE_FILE_PATH_MAX] = {
                    0};
                resource_string = string;
        }
        else
        {
                resource_string = lv_mem_alloc(resouce_string_size);
        }

        lv_memset_00(resource_string, resouce_string_size);

        if (path != NULL)
        {
                strcpy(resource_string, path);
        }

        if (file != NULL)
        {
                strcat(resource_string, file);
        }

        uint32_t *p_param = (uint32_t *)&resource_string[strlen(resource_string) + 2];

        *p_param = w;
        p_param++;
        *p_param = h;

        return resource_string;
}

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 获取ui资源的路径
 * @return:
 */
void *resource_ui_src_get(const char *file)
{
        return (void *)resource_src_get(RESOURCE_UI_PATH, file, 0, 0, false);
}

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 获取背景图片
 * @return:
 */
void *resource_wallpaper_src_get(char *file, uint32_t w, uint32_t h)
{
        return (void *)resource_src_get(RESOURCE_WALLPAPER_PATH, file, w, h, false);
}

/*
 * @日期: 2022-08-12
 * @作者: leo.liu
 * @功能: 文件路径获取
 * @return:
 */
void *file_path_src_get(char *path, const char *file, uint32_t w, uint32_t h)
{
        return (void *)resource_src_get(path, file, w, h, false);
}

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 释放文件
 * @return:
 */
bool resouce_file_src_free(void *pstr)
{
        if (pstr == NULL)
        {
                return false;
        }

        lv_mem_free(pstr);
        return true;
}

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 创建一个ui资源文件
 * @return:
 */
void *resource_ui_src_alloc(char *file, uint32_t w, uint32_t h)
{
        return (void *)resource_src_get(RESOURCE_UI_PATH, file, w, h, true);
}

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 创建一个壁纸资源文件
 * @return:
 */
void *resource_wallpaper_src_alloc(char *file, uint32_t w, uint32_t h)
{
        return (void *)resource_src_get(RESOURCE_WALLPAPER_PATH, file, w, h, true);
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 控件被按下
***********************************************/
void lv_layout_touch_callback(lv_event_t *e)
{
        standby_timer_restart(false);
        ring_touch_play();
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 获取编译日期
***********************************************/
bool platform_build_date_get(struct tm *tm)
{
#define MONTH_PER_YEAR 12   // 一年12月
#define YEAR_MONTH_DAY 20   // 年月日缓存大小
#define HOUR_MINUTES_SEC 20 // 时分秒缓存大小

        const char year_month[MONTH_PER_YEAR][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        char compile_date[YEAR_MONTH_DAY] = {0};
        char compile_time[HOUR_MINUTES_SEC] = {0};
        char str_month[4] = {0};

        sprintf(compile_date, "%s", __DATE__); // "Aug 23 2016"
        sprintf(compile_time, "%s", __TIME__); // "10:59:19"

        sscanf(compile_date, "%s %d %d", str_month, &(tm->tm_mday), &(tm->tm_year));
        sscanf(compile_time, "%d:%d:%d", &(tm->tm_hour), &(tm->tm_min), &(tm->tm_sec));

        for (int i = 0; i < MONTH_PER_YEAR; ++i)
        {
                if (strncmp(str_month, year_month[i], 3) == 0)
                {
                        tm->tm_mon = i + 1;
                        return true;
                }
        }
        return false;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: sip账号获取其IP和房号
***********************************************/
bool sip_user_get_number_and_ip(const char *user, char *ip, char *number)
{
        char user_temp[128] = {0};
        if (user == NULL)
        {
                return false;
        }
        strcpy(user_temp, user);

        char *ptr = strchr(user_temp, '@');
        if (ptr == NULL)
        {
                return false;
        }
        *ptr = 0;
        if (number != NULL)
        {
                strcpy(number, user_temp);
        }

        ptr += 1;
        if (ptr == NULL)
        {
                return false;
        }

        if (ip != NULL)
        {
                strcpy(ip, ptr);
        }
        return true;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 音频回调处理函数
***********************************************/
void audio_output_event_default_process(int arg)
{
        // SAT_DEBUG("AMP %s", arg ? "enable" : "disable");
        amp_enable_set(arg ? true : false);
}
static int alarm_ch;
/************************************************************
** 函数说明: 获取警报通道
** 作者: xiaoxiao
** 日期: 2023-05-04 17:06:40
** 参数说明:
** 注意事项:
************************************************************/
int layout_alarm_alarm_channel_get(void)
{
        return alarm_ch;
}

/************************************************************
** 函数说明: 设置警报通道
** 作者: xiaoxiao
** 日期: 2023-05-04 17:09:12
** 参数说明:
** 注意事项:
************************************************************/
void layout_alarm_alarm_channel_set(int ch)
{
        alarm_ch = ch;
}

/************************************************************
** 函数说明: 警报处理函数
** 作者: xiaoxiao
** 日期: 2023-04-28 17:07:39
** 参数说明:
** 注意事项:
************************************************************/
void layout_alarm_trigger_default(int arg1, int arg2)
{
        if (user_data_get()->is_device_init == false)
        {
                return;
        }
        if ((arg1 == 7) && (arg2 < ALM_LOW))
        {
                if (user_data_get()->alarm.buzzer_alarm == false)
                {
                        user_data_get()->alarm.buzzer_alarm = true;
                        user_data_save(true, true);
                        buzzer_call_trigger_check();
                }
        }
        else
        {
                // 没有传感器被选中
                if ((!(user_data_get()->alarm.away_alarm_enable_list & (0x01 << arg1))) && (!(user_data_get()->alarm.security_alarm_enable_list & (0x01 << arg1))))
                {
                        return;
                }
                // 非总是检测情况下，没有使能警报
                if ((user_data_get()->alarm.alarm_enable_always[0][arg1] == false) && (user_data_get()->alarm.alarm_enable_always[1][arg1] == false))
                {
                        if (user_data_get()->alarm.away_alarm_enable == false && user_data_get()->alarm.security_alarm_enable == false)
                        {
                                return;
                        }
                }
                if ((user_data_get()->alarm.alarm_enable[arg1] == 1 && arg2 < ALM_LOW) || (user_data_get()->alarm.alarm_enable[arg1] == 2 && arg2 > ALM_HIGHT))
                {
                        layout_alarm_alarm_channel_set(arg1);
                        user_data_get()->alarm.alarm_trigger[arg1] = true;
                        user_data_get()->alarm.emergency_mode = 1;
                        user_data_get()->alarm.alarm_ring_play = true;
                        user_data_save(true, true);
                        layout_common_call_log(security_emergency, arg1);
                        sat_linphone_handup(0xFFFF);
                        sat_layout_goto(alarm, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
                }
        }
}

/************************************************************
** 函数说明: 检测是否需要触发警报
** 作者: xiaoxiao
** 日期: 2023-05-08 09:07:36
** 参数说明:
** 注意事项:
************************************************************/
bool alarm_trigger_check(void)
{
        bool alarm_occur = false;
        for (int i = 0; i < 8; i++)
        {
                if (i != 7)
                {
                        if (user_data_get()->alarm.alarm_enable[i] == 0)
                        {
                                user_data_get()->alarm.alarm_trigger[i] = false;
                                user_data_get()->alarm.alarm_trigger_enable[i] = false;
                                user_data_save(true, true);
                                continue;
                        }
                        if (((!user_data_get()->alarm.away_alarm_enable_list) & (0x01 << i)) && (!user_data_get()->alarm.security_alarm_enable_list) & (0x01 << i))
                        {
                                continue;
                        }
                        if ((user_data_get()->alarm.alarm_trigger[i]))
                        {
                                if ((user_data_get()->alarm.alarm_trigger_enable[i] && user_data_get()->alarm.away_alarm_enable) || user_data_get()->alarm.security_alarm_enable || user_data_get()->alarm.alarm_enable_always[0][i] || user_data_get()->alarm.alarm_enable_always[1][i])
                                {
                                        alarm_occur = true;
                                        user_data_get()->alarm.emergency_mode = 1;
                                }
                        }
                        else
                        {
                                user_data_get()->alarm.alarm_trigger_enable[i] = false;
                                user_data_save(true, true);
                        }
                }
                else if (user_data_get()->alarm.alarm_trigger[i])
                {
                        alarm_occur = true;
                        user_data_get()->alarm.emergency_mode = 0;
                }
                if (sat_cur_layout_get() == sat_playout_get(alarm) && (user_data_get()->alarm.alarm_trigger[layout_alarm_alarm_channel_get()] == false) && (i == layout_alarm_alarm_channel_get()))
                {
                        layout_common_call_log(i == 7 ? emergency_return : security_emergency_return, i);
                }
                if ((alarm_occur) && (sat_cur_layout_get() != sat_playout_get(alarm) || (user_data_get()->alarm.alarm_trigger[layout_alarm_alarm_channel_get()] == false)))
                {
                        user_data_save(true, true);
                        layout_common_call_log(i == 7 ? emergency_occur : security_emergency, i);
                        layout_alarm_alarm_channel_set(i);
                        sat_linphone_handup(0xFFFF);
                        sat_layout_goto(alarm, LV_SCR_LOAD_ANIM_FADE_IN, alarm_occur);
                }
        }
        return alarm_occur;
}
/***********************************************************************/

/************************************************************
** 函数说明: 设置last_call_log new flag
** 作者: xiaoxiao
** 日期: 2023-05-17 16:11:26
** 参数说明:
** 注意事项:
************************************************************/
void layout_last_call_new_flag_set(bool new)
{
        user_data_get()->last_call_new = new;
        user_data_save(false, false);
}
/************************************************************
** 函数说明: 获取是否有未查看的通话记录标志
** 作者: xiaoxiao
** 日期: 2023-05-17 16:09:19
** 参数说明:
** 注意事项:
************************************************************/
bool layout_last_call_new_flag_get(void)
{
        return user_data_get()->last_call_new;
}

/***********************************************************************/

// 警报界面输入安全密码公共函数
/***********************************************************************/
enum
{
        common_obj_id_number_keyboard_btn,
        common_password_input_obj_id_tips,
        common_obj_id_passwd_input_tx1,
        common_obj_id_passwd_input_tx2,
        common_obj_id_passwd_input_tx3,
        common_obj_id_passwd_input_tx4,
        common_password_input_obj_id_cancel,
};

/***********************************************
** 作者: leo.liu
** 日期: 2023-2-1 8:51:49
** 说明: 输入数字键盘
***********************************************/
static lv_obj_t *common_password_input_textarea_focused_get(lv_obj_t *parent)
{
        lv_obj_t *textarea = NULL;

        for (int i = 0; i < 2; i++)
        {

                for (int j = 0; j < 4; j++)
                {
                        textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + j);
                        if (lv_obj_get_state(textarea) == LV_STATE_FOCUSED)
                        {
                                return textarea;
                        }
                }
        }
        return textarea;
}

/************************************************************
** 函数说明: 密码输入错误屏幕闪烁
** 作者: xiaoxiao
** 日期: 2023-08-05 14:28:11
** 参数说明:
** 注意事项:
************************************************************/
static void passwd_incorrect_timer(lv_timer_t *ptimer)
{
        lv_obj_t *parent = ptimer->user_data;
        lv_obj_set_style_bg_color(parent, lv_color_hex(0x0), LV_PART_MAIN);
        lv_timer_del(ptimer);
}

// 密码校验成功回调指针
static void (*corrrect_passwd_cb)(void) = NULL;

/************************************************************
** 函数说明: 字符删除动画
** 作者: xiaoxiao
** 日期: 2023-08-05 14:29:54
** 参数说明:
** 注意事项:
************************************************************/
static void common_passwd_input_text_prev_foucued(lv_obj_t *parent)
{
        lv_obj_t *textarea = NULL;

        for (int j = 0; j < 4; j++)
        {
                textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + j);
                if ((lv_obj_get_state(textarea) == LV_STATE_FOCUSED) && (j != 0))
                {
                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                        textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + j - 1);
                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                        return;
                }
        }
}

/************************************************************
** 函数说明: 字符输入完成回调
** 作者: xiaoxiao
** 日期: 2023-08-05 14:27:10
** 参数说明:
** 注意事项:
************************************************************/
static void common_passwd_input_text_next_foucued(lv_obj_t *parent)
{
        lv_obj_t *textarea = NULL;

        for (int j = 0; j < 4; j++)
        {
                textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + j);
                if (lv_obj_get_state(textarea) == LV_STATE_FOCUSED)
                {
                        if (j == 3)
                        {
                                char buffer[4];
                                memset(buffer, 0, sizeof(buffer));

                                for (int k = 0; k < 4; k++)
                                {
                                        strcat(buffer, lv_textarea_get_text(lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + k)));
                                }
                                if (strncmp(user_data_get()->etc.password, buffer, 4) == 0)
                                {
                                        if (corrrect_passwd_cb != NULL)
                                        {
                                                corrrect_passwd_cb();
                                                return;
                                        }
                                }
                                lv_obj_set_style_bg_color(parent, lv_color_hex(0x5E0000), LV_PART_MAIN);
                                lv_sat_timer_create(passwd_incorrect_timer, 500, parent);
                                for (int k = 0; k < 4; k++)
                                {
                                        textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + k);
                                        lv_textarea_del_char(textarea);
                                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                                }
                                textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1);
                                lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                                return;
                        }
                        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
                        textarea = lv_obj_get_child_form_id(parent, common_obj_id_passwd_input_tx1 + j + 1);
                        lv_obj_add_state(textarea, LV_STATE_FOCUSED);
                        return;
                }
        }
}

// 输入一个字符
static bool common_passwd_inpu_textbox_add(lv_obj_t *parent, const char *string)
{
        lv_obj_t *textarea = common_password_input_textarea_focused_get(parent);

        if (textarea == NULL)
        {
                return false;
        }

        lv_textarea_add_text(textarea, string);
        common_passwd_input_text_next_foucued(parent);
        return true;
}

// 删除一个字符
static bool common_password_input_textbox_del(lv_obj_t *parernt)
{
        common_passwd_input_text_prev_foucued(parernt);
        lv_obj_t *textarea = common_password_input_textarea_focused_get(parernt);

        if (textarea == NULL)
        {
                return false;
        }
        lv_textarea_del_char(textarea);

        return true;
}

/************************************************************
** 函数说明: 密码键盘输入
** 作者: xiaoxiao
** 日期: 2023-08-05 14:26:00
** 参数说明:
** 注意事项:
************************************************************/
static void password_input_keyboard_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_target(ev);
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        if (lv_btnmatrix_has_btn_ctrl(obj, id, LV_BTNMATRIX_CTRL_HIDDEN) == true)
        {
                return;
        }
        if (id == 11)
        {
                common_password_input_textbox_del(lv_obj_get_parent(obj));
        }
        else
        {
                const char *text = lv_btnmatrix_get_btn_text(obj, id);

                if (text != NULL)
                {

                        common_passwd_inpu_textbox_add(lv_obj_get_parent(obj), text);
                }
        }
}

/************************************************************
** 函数说明: 隐藏键盘
** 作者: xiaoxiao
** 日期: 2023-05-16 21:31:30
** 参数说明:
** 注意事项:
************************************************************/
static void common_passwd_check_close_click(lv_event_t *ev)
{
        lv_obj_t *obj = lv_event_get_target(ev);
        lv_obj_add_flag(lv_obj_get_parent(obj), LV_OBJ_FLAG_HIDDEN);
}

/************************************************************
** 函数说明: 密码输入检验公共函数创建
** 作者: xiaoxiao
** 日期: 2023-08-05 10:50:25
** 参数说明:
** 注意事项:
************************************************************/
void common_passwd_check_func_create(int cont_id, void (*callback)(void))
{
        corrrect_passwd_cb = callback;

        {
                lv_obj_t *parent = lv_common_img_btn_create(sat_cur_layout_screen_get(), cont_id, 0, 0, 1024, 600,
                                                            NULL, false, LV_OPA_90, 0, LV_OPA_90, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                            NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);

                lv_obj_add_flag(parent, LV_OBJ_FLAG_HIDDEN);

                /************************************************************
                ** 函数说明: 数字键盘创建
                ** 作者: xiaoxiao
                ** 日期: 2023-04-27 16:40:03
                ** 参数说明:
                ** 注意事项:
                ************************************************************/
                {
                        lv_obj_t *obj = lv_common_number_input_keyboard_create(parent, common_obj_id_number_keyboard_btn, 128, 90, 312, 402,
                                                                               password_input_keyboard_click, LV_OPA_COVER, 0x808080, LV_OPA_COVER, 0x00a8ff,
                                                                               360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                               360, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                                               0XFFFFFF, 0XFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large,
                                                                               18, 24);
                        lv_btnmatrix_set_btn_ctrl(obj, 9, LV_BTNMATRIX_CTRL_HIDDEN | LV_BTNMATRIX_CTRL_DISABLED);
                        lv_common_img_btn_create(parent, 7, 128, 390, 100, 102,
                                                 NULL, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
                }

                /************************************************************
                ** 函数说明: 密码提示
                ** 作者: xiaoxiao
                ** 日期: 2023-04-27 17:50:33
                ** 参数说明:
                ** 注意事项:
                ************************************************************/
                {
                        lv_common_text_create(parent, common_password_input_obj_id_tips, 644, 198, 217, 43,
                                              NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                              lang_str_get(LAYOUT_SECURITY_XLS_LANG_ID_ENTER_PASSSWORD), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
                }

                /************************************************************
                ** 函数说明: 密码文本输入框创建
                ** 作者: xiaoxiao
                ** 日期: 2023-04-27 17:30:01
                ** 参数说明:
                ** 注意事项:
                ************************************************************/
                for (int j = 0; j < 4; j++)
                {

                        lv_obj_t *obj = lv_common_textarea_create(parent, common_obj_id_passwd_input_tx1 + j, 596 + 84 * j, 262, 60, 76,
                                                                  NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                                                  LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0X101010,
                                                                  0, 4, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0Xffffff,
                                                                  0, 4, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
                                                                  NULL, 0Xffffff, 0Xffffff, LV_TEXT_ALIGN_CENTER, lv_font_large, 1,
                                                                  20, 500, 0Xffffff);

                        lv_textarea_set_password_mode(obj, true);
                        lv_textarea_set_password_show_time(obj, 500);
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        if (j == 0)
                        {
                                lv_obj_add_state(obj, LV_STATE_FOCUSED);
                        }
                }

                /***********************************************
                ** 作者: leo.liu
                ** 日期: 2023-2-3 14:13:25
                ** 说明: 返回按钮
                ***********************************************/
                {
                        lv_common_img_btn_create(parent, common_password_input_obj_id_cancel, 35, 15, 48, 48,
                                                 common_passwd_check_close_click, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
                                                 resource_ui_src_get("btn_close.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
                }
        }
}

/************************************************************
** 函数说明: 判断是否是一个合法的ipv4地址
** 作者: xiaoxiao
** 日期: 2023-04-27 17:30:01
** 参数说明:
** 注意事项:
************************************************************/
bool is_valid_ipv4(const char *s)
{
        char *copy = strdup(s);
        char *token = strtok(copy, ".");
        int count = 0;

        while (token != NULL)
        {
                int num = atoi(token);
                if (num < 0 || num > 255)
                {
                        free(copy);
                        return false;
                }
                count++;
                token = strtok(NULL, ".");
        }
        free(copy);
        return count == 4;
}

/************************************************************
** 函数说明: 是否警备触发状态
** 作者: xiaoxiao
** 日期：2023-10-23 08:21:24
** 参数说明:
** 注意事项：
************************************************************/
bool is_alarm_trigger(void)
{
        for (int i = 0; i < 8; i++)
        {
                if (i != 7)
                {
                        if (user_data_get()->alarm.alarm_trigger[i])
                        {
                                if (user_data_get()->alarm.away_alarm_enable || user_data_get()->alarm.security_alarm_enable || ((user_data_get()->alarm.alarm_enable[i] == 1) && user_data_get()->alarm.alarm_enable_always[0][i]) || ((user_data_get()->alarm.alarm_enable[i] == 2) && user_data_get()->alarm.alarm_enable_always[1][i]))

                                        return true;
                                user_data_get()->alarm.alarm_trigger[i] = false;
                                user_data_save(true, true);
                                return false;
                        }
                }
                else if (user_data_get()->alarm.alarm_trigger[i])
                {
                        return true;
                }
        }
        return false;
}

/************************************************************
** 函数说明: 获取eth0网卡是否正常插入
** 作者: xiaoxiao
** 日期：2023-12-09 15:54:43
** 参数说明:
** 注意事项：
************************************************************/
bool is_eth0_inserted(void)
{
        FILE *fp;
        char buffer[1024];

        // 执行ifconfig eth0命令
        fp = popen("ifconfig eth0", "r");
        if (fp == NULL)
        {
                perror("打开管道时出错");
                exit(EXIT_FAILURE);
        }

        // 读取命令的输出
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
                // 检查输出中是否包含"eth0"
                if (strstr(buffer, "eth0") != NULL)
                {
                        pclose(fp);
                        return 1; // eth0存在
                }
        }

        // 关闭管道，如果未找到"eth0"，则返回0
        pclose(fp);
        return 0;
}

/************************************************************
** 函数说明:警报记录
** 作者: xiaoxiao
** 日期：2023-12-27 21:19:30
** 参数说明:
** 注意事项：
************************************************************/
bool layout_common_call_log(int type, int ch)
{
        bool result = true;
        struct tm tm;
        user_time_read(&tm);
        alarm_list_add(type, ch, &tm);
        if ((user_data_get()->system_mode & 0x0f) == 0x01)
        {
                int event = (user_data_get()->alarm.away_alarm_enable || user_data_get()->alarm.security_alarm_enable) ? 3 : 1;
                char dong[5] = {0};
                char ho[5] = {0};
                strncpy(dong, network_data_get()->sip_user, 4);
                strncpy(ho, &network_data_get()->sip_user[4], 4);
                while (dong[0] == '0')
                {
                        memmove(&dong[0], &dong[1], 4);
                }
                while (ho[0] == '0')
                {
                        memmove(&ho[0], &ho[1], 4);
                }
                result = commax_emergency_event_report(network_data_get()->local_server, 80, dong, ho, &tm, event, (type % 3) + 1, 1000);
        }
        return result;
}