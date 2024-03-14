
#include "user_data.h"

#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "stdio.h"
#include "stdlib.h"
#include "common/sat_user_time.h"
#include "common/sat_user_common.h"
#include "layout_common.h"
#define USER_DATA_PATH "/app/data/user_data.cfg"
#define NETWORK_DATA_PATH "/app/data/network_data.cfg"

static user_data_info user_data =
    {
        0};

static const user_data_info user_data_default =
    {
        .is_device_init = 0,
        .auto_record_mode = 2,
        .wifi_enable = true,
        .motion = {
            .enable = false,
            .select_camera = 0,
            .saving_fmt = 0,
            .sensivity = 1,
            .timer_en = false,
            .lcd_en = false,
            .start = {
                .tm_year = 2023,
                .tm_mon = 1,
                .tm_mday = 1,
                .tm_hour = 0,
                .tm_min = 0,
                .tm_sec = 0},
            .end = {.tm_year = 2023, .tm_mon = 1, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0}},
        .audio = {
            .key_sound = true,
            .ring_mute = false,
            .door_tone = 1,

            .extenion_tone = 1,

            .buzzer_tone = 1,
            .buzzer_volume = 50,
            .common_entrance_tone = 1,
            .common_entrance_volume = 50,
            .common_entrance_voice = 50,

            .securirty_office_tone = 1,

            .entrance_volume = 50,
            .entrance_voice = 50,

            .guard_station_volume = 50,
            .guard_station_voice = 50,
            .extension_volume = 50,
            .extension_voice = 50,
            .touch_notification_volume = 50,
            .touch_notification_voice = 50,
            .ring_repeat = 0,
        },

        .display = {
            .standby_mode = 0,
            .screen_off_time = 30,
            .lcd_brigtness = 100,
            .wait_display_time = 15,
            .frame_time_en = 0,
            .frame_time_start = 0,
            .frame_time_end = 0,
            .night_mode = false,
            .night_time_start = 0,
            .night_time_end = 0,
            .frame_list = 0x03,
            .frame_background = false,

        },
        .etc = {
            .language = 0,
            .deive_id = 0,
            .open_the_door = 0,
            .door1_open_door_mode = 0,
            .door_lock_num = 0,
            .password = {"1234"},
            .comm_ent_password = {"1234"},
            .time_automatically = 1,
            .call_time = 1,
            .tuya_connect_mode = 0,
        },
        .alarm = {
            .away_alarm_enable_list = 0x00,     // 离家设防列表
            .security_alarm_enable_list = 0x00, // 安全设防列表
            .away_alarm_enable = false,         // 离家设防使能状态
            .security_alarm_enable = false,     // 安全设防状态显示
            .emergency_mode = 0,                // 0:手动触发警报 1:安防系统自动触发警报

            .alarm_enable[0] = 0, // 0不使能，1：NC低电平触发2：NO高电平触发
            .alarm_enable[1] = 0,
            .alarm_enable[2] = 0,
            .alarm_enable[3] = 0,
            .alarm_enable[4] = 0,
            .alarm_enable[5] = 0,
            .alarm_enable[6] = 0,
            .alarm_enable[7] = 0,
            .alarm_trigger[0] = 0, // 0非触发状态，1触发状态
            .alarm_trigger[1] = 0,
            .alarm_trigger[2] = 0,
            .alarm_trigger[3] = 0,
            .alarm_trigger[4] = 0,
            .alarm_trigger[5] = 0,
            .alarm_trigger[6] = 0,
            .alarm_trigger[7] = 0,
            .alarm_trigger_enable[0] = 0,
            .alarm_trigger_enable[1] = 0,
            .alarm_trigger_enable[2] = 0,
            .alarm_trigger_enable[3] = 0,
            .alarm_trigger_enable[4] = 0,
            .alarm_trigger_enable[5] = 0,
            .alarm_trigger_enable[6] = 0,
            .alarm_trigger_enable[7] = 0,
            .alarm_enable_always[0] = false,
            .alarm_enable_always[1] = false,
            .alarm_enable_always[2] = false,
            .alarm_enable_always[3] = false,
            .alarm_enable_always[4] = false,
            .alarm_enable_always[5] = false,
            .alarm_enable_always[6] = false,
            .alarm_enable_always[7] = false,

            .away_sensor_enable[0] = false,
            .away_sensor_enable[1] = false,
            .away_sensor_enable[2] = false,
            .away_sensor_enable[3] = false,
            .away_sensor_enable[4] = false,
            .away_sensor_enable[5] = false,
            .away_sensor_enable[6] = false,
            .away_sensor_enable[7] = false,

            .security_sensor_enable[0] = false,
            .security_sensor_enable[1] = false,
            .security_sensor_enable[2] = false,
            .security_sensor_enable[3] = false,
            .security_sensor_enable[4] = false,
            .security_sensor_enable[5] = false,
            .security_sensor_enable[6] = false,
            .security_sensor_enable[7] = false,

            .away_setting_time = 1,
            .away_release_time = 30,

            .away_save_photo = false,

            .bypass_call = false,

            .away_auto_record = false,

            .security_auto_record = false,

            .alarm_gpio_value_group[0] = 0,
            .alarm_gpio_value_group[1] = 0,
            .alarm_gpio_value_group[2] = 0,
            .alarm_gpio_value_group[3] = 0,
            .alarm_gpio_value_group[4] = 0,
            .alarm_gpio_value_group[5] = 0,
            .alarm_gpio_value_group[6] = 0,
            .alarm_gpio_value_group[7] = 0,

            .buzzer_alarm = 0,

            .cctv_sensor[0] = 0,
            .cctv_sensor[1] = 0,
            .cctv_sensor[2] = 0,
            .cctv_sensor[3] = 0,
            .cctv_sensor[4] = 0,
            .cctv_sensor[5] = 0,
            .cctv_sensor[6] = 0,
            .cctv_sensor[7] = 0,

            .away_setting_countdown = false,
        },
        .system_mode = 0x01,
        .always_monitoring = 0,
        .last_call_new = false,
};
// {"010193001012@172.16.0.104", "010193001013@172.16.0.104", "010193001014@172.16.0.185", "010193001015@172.16.0.104", "010193001016@172.16.0.104", "010193001017@172.16.0.104", "010193001018@172.16.0.104"},

bool user_data_save(bool data_sync, bool activve)
{
        int fd = open(USER_DATA_PATH, O_WRONLY | O_CREAT);

        if (fd < 0)
        {
                printf("write open %s fail \n", USER_DATA_PATH);
                return false;
        }
        if ((user_data_get()->system_mode & 0x0F) == 0x01 && data_sync) // 主机是否需要同步数据
        {
                asterisk_server_sync_user_data_force(true);
        }
        if (activve) // 是否是主动更改用户数据
        {
                user_data.sync_timestamp = user_timestamp_get();
        }
        write(fd, &user_data, sizeof(user_data_info));

        close(fd);
        system("sync");

        return true;
}

#define user_data_check_range_out(cur, min, max)                                                  \
        if ((user_##data.cur < min) || (user_##data.cur > max))                                   \
        {                                                                                         \
                printf("user data error %d(%d,%d) \n", (int)user_##data.cur, (int)min, (int)max); \
                user_##data.cur = user_##data##_default.cur;                                      \
        }

#define user_data_motion_check_range_out(x, min, max) user_data_check_range_out(motion.x, min, max)

#define user_data_audio_check_range_out(x, min, max) user_data_check_range_out(audio.x, min, max)

#define user_data_display_check_range_out(x, min, max) user_data_check_range_out(display.x, min, max)

#define user_data_etc_check_range_out(x, min, max) user_data_check_range_out(etc.x, min, max)

#define user_data_alarm_check_range_out(x, min, max) user_data_check_range_out(alarm.x, min, max)

/***
** 日期: 2022-05-05 08:47
** 作者: leo.liu
** 函数作用：检验数据是否合法
** 返回参数说明：
***/
static void user_data_check_valid(void)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-12 14:35:9
        ** 说明: 初始值设置
        ***********************************************/
        user_data_check_range_out(is_device_init, 0, 1);

        /***** 自动记录模式 *****/
        user_data_check_range_out(auto_record_mode, 0, 2);
        user_data_check_range_out(wifi_enable, 0, 1);

        /***** 移动侦测 *****/
        user_data_motion_check_range_out(enable, 0, 1);
        user_data_motion_check_range_out(select_camera, 0, 15);
        user_data_motion_check_range_out(saving_fmt, 0, 1);
        user_data_motion_check_range_out(sensivity, 0, 2);
        user_data_motion_check_range_out(timer_en, 0, 1);
        user_data_motion_check_range_out(lcd_en, 0, 1);

        user_data_motion_check_range_out(start.tm_year, 2021, 2037);
        user_data_motion_check_range_out(start.tm_mon, 1, 12);
        user_data_motion_check_range_out(start.tm_mday, 1, 31);
        user_data_motion_check_range_out(start.tm_hour, 0, 23);
        user_data_motion_check_range_out(start.tm_min, 0, 59);
        user_data_motion_check_range_out(start.tm_sec, 0, 59);

        user_data_motion_check_range_out(end.tm_year, 2021, 2037);
        user_data_motion_check_range_out(end.tm_mon, 1, 12);
        user_data_motion_check_range_out(end.tm_mday, 1, 31);
        user_data_motion_check_range_out(end.tm_hour, 0, 23);
        user_data_motion_check_range_out(end.tm_min, 0, 59);
        user_data_motion_check_range_out(end.tm_sec, 0, 59);

        /***** audio *****/
        user_data_audio_check_range_out(key_sound, 0, 1);
        user_data_audio_check_range_out(ring_mute, 0, 1);
        user_data_audio_check_range_out(door_tone, 1, 6);
        user_data_audio_check_range_out(extenion_tone, 0, 4);

        user_data_audio_check_range_out(buzzer_tone, 1, 6);
        user_data_audio_check_range_out(buzzer_volume, 0, 100);

        user_data_audio_check_range_out(common_entrance_tone, 1, 6);
        user_data_audio_check_range_out(common_entrance_volume, 0, 100);
        user_data_audio_check_range_out(common_entrance_voice, 0, 100);

        user_data_audio_check_range_out(securirty_office_tone, 1, 6);
        user_data_audio_check_range_out(entrance_volume, 0, 100);
        user_data_audio_check_range_out(entrance_voice, 0, 100);

        user_data_audio_check_range_out(guard_station_volume, 0, 100);
        user_data_audio_check_range_out(guard_station_voice, 0, 100);

        user_data_audio_check_range_out(touch_notification_voice, 0, 100);
        user_data_audio_check_range_out(touch_notification_volume, 0, 100);

        user_data_audio_check_range_out(extension_volume, 0, 100);
        user_data_audio_check_range_out(extension_voice, 0, 100);
        user_data_audio_check_range_out(ring_repeat, 0, 1);

        /***** display *****/
        user_data_display_check_range_out(standby_mode, 0, 1);
        user_data_display_check_range_out(lcd_brigtness, 0, 100);
        user_data_display_check_range_out(wait_display_time, 15, 180);

        user_data_display_check_range_out(frame_time_en, 0, 1);
        user_data_display_check_range_out(frame_time_start, 0, 1440);
        user_data_display_check_range_out(screen_off_time, 15, 180);
        user_data_display_check_range_out(frame_time_end, 0, 1440);
        user_data_display_check_range_out(night_mode, 0, 1);
        user_data_display_check_range_out(night_time_start, 0, 1440);
        user_data_display_check_range_out(night_time_end, 0, 1440);

        user_data_display_check_range_out(frame_list, 0, 0x1F);
        user_data_display_check_range_out(frame_background, 0, 1);

        /***** etc *****/
        user_data_etc_check_range_out(language, 0, 9);
        user_data_etc_check_range_out(deive_id, 0, 3);
        user_data_etc_check_range_out(open_the_door, 0, 1);
        user_data_etc_check_range_out(door1_open_door_mode, 0, 1);
        user_data_etc_check_range_out(door_lock_num, 0, 0xff);
        user_data_etc_check_range_out(password[0], '0', '9');
        user_data_etc_check_range_out(password[1], '0', '9');
        user_data_etc_check_range_out(password[2], '0', '9');
        user_data_etc_check_range_out(password[3], '0', '9');
        user_data_etc_check_range_out(time_automatically, 0, 1);
        user_data_etc_check_range_out(call_time, 1, 3);
        user_data_etc_check_range_out(tuya_connect_mode, 0, 1);

        /*****	alarm *****/
        user_data_alarm_check_range_out(away_save_photo, 0, 1);

        user_data_alarm_check_range_out(away_alarm_enable, 0, 2);
        user_data_alarm_check_range_out(security_alarm_enable, 0, 1);
        user_data_alarm_check_range_out(emergency_mode, 0, 1);

        user_data_alarm_check_range_out(away_alarm_enable_list, 0, 0x7f);
        user_data_alarm_check_range_out(security_alarm_enable_list, 0, 0x7f);

        for (int i = 0; i < 8; i++)
        {
                user_data_alarm_check_range_out(alarm_enable[i], 0, 2);

                user_data_alarm_check_range_out(alarm_trigger[i], 0, 1);

                user_data_alarm_check_range_out(away_sensor_enable[i], 0, 1);

                user_data_alarm_check_range_out(cctv_sensor[i], 0, 8);

                user_data_alarm_check_range_out(security_sensor_enable[i], 0, 1);

                for (int i = 0; i < 8; i++)
                {
                        user_data_alarm_check_range_out(alarm_enable_always[i], 0, 1);
                }
                user_data_alarm_check_range_out(alarm_gpio_value_group[i], 0, 4);
        }

        user_data_alarm_check_range_out(away_setting_time, 1, 3);
        user_data_alarm_check_range_out(away_release_time, 0, 90);
        user_data_alarm_check_range_out(away_auto_record, 0, 1);
        user_data_alarm_check_range_out(security_auto_record, 0, 1);
        user_data_alarm_check_range_out(buzzer_alarm, 0, 1);

        user_data_check_range_out(system_mode, 1, 29);
}

bool user_data_init(void)
{
        int fd = open(USER_DATA_PATH, O_RDONLY);

        if (fd < 0)
        {
                printf("read open %s fail \n", USER_DATA_PATH);
                user_data = user_data_default;
                return false;
        }

        read(fd, &user_data, sizeof(user_data_info));

        close(fd);
        user_data_check_valid();
        return true;
}

user_data_info *user_data_get(void)
{
        return &user_data;
}

void user_data_reset(void)
{
        char language = user_data.etc.language;

        user_data = user_data_default;
        user_data.etc.language = language;
        user_data_save(true, true);
        system("sync");
}

/*************************************************************************************************************************************************
*******************************											                                                                                              *******************************
*******************************					                                                        楚河汉界					                             *******************************
*******************************											                                                                                              *******************************
*************************************************************************************************************************************************/
static user_network_info network_data = {0};

static const user_network_info network_data_default = {
    .network = {
        .udhcp = true,
        .ipaddr = {0},
        .mask = {"255.0.0.0"},
        .gateway = {"10.0.0.1"},
        .dns = {"8.8.8.8"}},
    .common_entrance_ip = {"10.0.0.2"},
    .sip_user = {"12345678"},
    .local_server = {"10.0.0.2"},
    .sip_server = {"10.0.0.2"},
    .cctv_server = {"10.0.0.2"},
    .guard_number = {"7000010050"},
};

#define network_data_check_range_out(cur, min, max)                                      \
        if ((network_##data.cur < min) || (network_##data.cur > max))                    \
        {                                                                                \
                printf("network data error %d(%d,%d) \n", network_##data.cur, min, max); \
                network_##data.cur = network_##data##_default.cur;                       \
        }

bool network_data_save(void)
{
        int fd = open(NETWORK_DATA_PATH, O_WRONLY | O_CREAT);

        if (fd < 0)
        {
                printf("write open %s fail \n", NETWORK_DATA_PATH);
                return false;
        }

        write(fd, &network_data, sizeof(user_network_info));

        close(fd);
        system("sync");

        if ((user_data_get()->system_mode & 0x0F) == 0x01)
        {
                asterisk_server_sync_network_data_force(true);
        }

        return true;
}

static void printf_register_device(void)
{

        printf("\n\n#############################################\n");
        for (int i = 0; i < DEVICE_MAX; i++)
        {
                printf("door camera :%d\n", i);
                printf("accout:%s\n", network_data.door_device[i].username);
                printf("password:%s\n", network_data.door_device[i].password);
                printf("authr:%d\n", network_data.door_device[i].auther_flag);
                printf("ipaddr:%s\n", network_data.door_device[i].ipaddr);
                printf("port:%d\n", network_data.door_device[i].port);
                printf("sip_url:%s\n", network_data.door_device[i].sip_url);
                printf("name:%s\n", network_data.door_device[i].door_name);
                printf("door camera rtsp profile token num%d\n", network_data.door_device[i].profile_token_num);
                // for (int j = 0; j < network_data.door_device[i].profile_token_num; j++)//数据错乱，profile_token_num可能会变成一个很大的数，导致启动以后一直停留在打印阶段，导致无法正常进入系统
                // {
                //         printf("---profile token:%s\n", network_data.door_device[i].rtsp[j].profile_token);
                //         printf("---rtsp url:%s\n", network_data.door_device[i].rtsp[j].rtsp_url);
                // }
        }
        printf("\n");
        for (int i = 0; i < DEVICE_MAX; i++)
        {
                printf("CCTV  :%d\n", i);
                printf("accout:%s\n", network_data.cctv_device[i].username);
                printf("password:%s\n", network_data.cctv_device[i].password);
                printf("authr:%d\n", network_data.cctv_device[i].auther_flag);
                printf("ipaddr:%s\n", network_data.cctv_device[i].ipaddr);
                printf("port:%d\n", network_data.cctv_device[i].port);
                printf("sip_url:%s\n", network_data.cctv_device[i].sip_url);
                printf("name:%s\n", network_data.cctv_device[i].door_name);
                printf("door camera rtsp profile token num%d\n", network_data.cctv_device[i].profile_token_num);
                // for (int j = 0; j < network_data.cctv_device[i].profile_token_num; j++)//数据错乱，profile_token_num可能会变成一个很大的数，导致启动以后一直停留在打印阶段，导致无法正常进入系统
                // {
                //         printf("---profile token:%s\n", network_data.cctv_device[i].rtsp[j].profile_token);
                //         printf("---rtsp url:%s\n", network_data.cctv_device[i].rtsp[j].rtsp_url);
                // }
        }
        printf("#############################################\n\n");
}
/***
** 日期: 2022-05-05 08:47
** 作者: leo.liu
** 函数作用：检验数据是否合法
** 返回参数说明：
***/
static void network_data_check_valid(void)
{
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-1-5 14:33:3
         ** 说明: sip
         ***********************************************/
        network_data_check_range_out(network.udhcp, 0, 1);

        for (int i = 0; i < strlen(network_data.sip_user); i++)
        {

                network_data_check_range_out(sip_user[i], '0', '9');
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-5 14:33:3
        ** 说明: 本机IP,和掩码
        ***********************************************/
        for (int i = 0; i < strlen(network_data.network.ipaddr); i++)
        {

                if ((network_data.network.ipaddr[i] != '.') && (network_data.network.ipaddr[0] != '\0'))
                {
                        network_data_check_range_out(network.ipaddr[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.network.mask); i++)
        {
                if (network_data.network.mask[i] != '.')
                {
                        network_data_check_range_out(network.mask[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.network.gateway); i++)
        {
                if (network_data.network.gateway[i] != '.')
                {
                        network_data_check_range_out(network.gateway[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.network.dns); i++)
        {
                if (network_data.network.dns[i] != '.')
                {
                        network_data_check_range_out(network.dns[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.local_server); i++)
        {
                if (network_data.local_server[i] != '.')
                {
                        network_data_check_range_out(local_server[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.sip_server); i++)
        {
                if (network_data.sip_server[i] != '.')
                {
                        network_data_check_range_out(sip_server[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.cctv_server); i++)
        {
                if (network_data.cctv_server[i] != '.')
                {
                        network_data_check_range_out(cctv_server[i], '0', '9');
                        break;
                }
        }
        for (int i = 0; i < strlen(network_data.guard_number); i++)
        {
                if (network_data.guard_number[i] != '.')
                {
                        network_data_check_range_out(guard_number[i], '0', '9');
                        break;
                }
        }
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-10 9:40:52
        ** 说明:	 检测分机IP组
        ***********************************************/
        //   network_data_check_range_out(door_device_count, 0, DEVICE_MAX);

        // network_data_check_range_out(cctv_device_count, 0, DEVICE_MAX);
        printf_register_device();
}

bool network_data_init(void)
{
        int fd = open(NETWORK_DATA_PATH, O_RDONLY);

        if (fd < 0)
        {
                printf("read open %s fail \n", NETWORK_DATA_PATH);
                network_data = network_data_default;
                return false;
        }

        read(fd, &network_data, sizeof(user_network_info));

        close(fd);
        network_data_check_valid();
        network_data_save();
        return true;
}

user_network_info *network_data_get(void)
{
        return &network_data;
}

void network_data_reset(void)
{
        network_data = network_data_default;
        network_data_save();
        system("sync");
}

user_network_info const *network_defauld_data_get(void)
{
        return &network_data_default;
}

user_data_info const *user_default_data_get(void)
{
        return &user_data_default;
}