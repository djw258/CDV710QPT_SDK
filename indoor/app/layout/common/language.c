#include "language.h"
#include "user_data.h"
#include "string.h"
#include "lang_xls.h"
#include "stdio.h"
#include "user_monitor.h"
static char *layout_default_language_string[LANGUAGE_STRING_TOTAL] = // 意义不大，只有xls文件出错才会使用这里的语言
    {
        "영문English",
        "English",
        "Skip",
        "Welcome to the Start Setting.\nFrom the next page, the setting of basic elements for operating the product begins.",
        "Execution",
        "Start setting",
        "Start setting is complete. Items whose values are not specified can be specified again in the setting menu.",
        "Confirm",
        "Setting complete",
        "Model",
        "Version",
        "Language",
        "Operating structure",
        "This is the sub indoor unit setting linked with the master unit.",
        "This is the sub indoor unit setting linked with the master indoor unit",
        "Enabled",
        "Slave settings",
        "Operating with server-based guard station and common entrance",
        "Operating with master indoor unit and door camera (Basic)",
        "Operating network",
        "Operating structure",
        "Single",
        "Server system",
        "Single (Slave) or Server System (Slave)",
        "Default gateway",
        "Product IP",
        "Subnet mask",
        "DNS",
        "SIP server",
        "Household number",
        "Local server",
        "Server system",
        "Guard station number",
        "Building number",
        "Update server",
        "CCTV server",
        "Building-House number",
        "Operating server",
        "Guard station",
        "Slave type setting",
        "Not set",
        "Master indoor unit IP setting",
        "Extension number setting",
        "Discovered networks",
        "Wi-Fi (wireless network)",
        "Wi-Fi",
        "Connected network",
        "Security",
        "IP address",
        "Please enter the account number",
        "Please enter a password",
        "connecting",
        "connect failed",
        "The time is set automatically when connected to the network.",
        "Set date and time automatically",
        "Date/time",
        "Search Door camera",
        "Door camera",
        "If you press the button of front door camera for 5 seconds, camera information is displayed.",
        "Searched door camera",
        "Registered Door camera",
        "Searched CCTV",
        "Registered CCTV",
        "Unable to register camera.",
        "Registration",
        "If you cannot see the video, please enter the correct IP using the IP setting function at the top right. \nIf the video is not visible even after setting the IP, press the reset button on the door camera.",
        "If it is not registered, please enter the correct IP using the IP setting function at the top right.",
        "Operation server IP address",
        "Update server IP",
        "DHCP",
        "Static",
        "A default password is set.  You must change your password before you can proceed to the next procedure.  Do you want to change your password?",
        "Change Password",
        "Enter at least 9 digits of numbers, special characters and alphabet characters",
        "Set the password to a mixture of numbers, letters and special characters to be at least 9 digits.",
        "Confirm Change Password ",
        "Change passwords do not match",
        "Change door camera information ",
        "Change Name",
        "Change the connection password ",
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
        "Mon.",
        "Tue.",
        "Wed.",
        "Thu.",
        "Fri.",
        "Sat.",
        "Sun.",
        "Monitoring",
        "Call",
        "CCTV",
        "Away",
        "Burglar",
        "Elevator",
        "Emergency",
        "Recent call",
        "Recent Video",
        "Currently in use in mobile app.",
        "Common entrance",
        "Door camera",
        "The communication line is being used by another device.",
        "Call",
        "Call channel",
        "Call log",
        "Selected",
        "Extension",
        "Guard",
        "Away",
        "Away mode is running",
        "Detects intruders inside and outside our house in away mode",
        "Outgoing time setting",
        "release time Setting",
        "Away function setting time",
        "Away function release time",
        "Save visitor photos",
        "Bypass Call",
        "0 seconds",
        "10 seconds",
        "20 seconds",
        "30 seconds",
        "40 seconds",
        "50 seconds",
        "60 seconds",
        "70 seconds",
        "80 seconds",
        "90 seconds",
        "1 minute",
        "2 minute",
        "3 minute",
        "Security",
        "Security area \nsensor 1",
        "Security area \nsensor 2",
        "Security area \nsensor 3",
        "Security area \nsensor 4",
        "Security area \nsensor 5",
        "Security area \nsensor 6",
        "Security area \nsensor 7",
        "Detect outside intrusion when you are inside home",
        "CCTV automatic recording",
        "End",
        "Execution",
        "Security mode is running",
        "Enter Password",
        "Stop",
        "Return",
        "Security emergency",
        "Emergency",
        "Household operation",
        "Security sensor 1",
        "Security sensor 2",
        "Security sensor 3",
        "Security sensor 4",
        "Security sensor 5",
        "Security sensor 6",
        "Security sensor 7",
        "Confirm",
        "Would you like to activate an emergency?",
        "Delete all?",
        "Emergency Log",
        "Emergency occurs",
        "Emergency stop",
        "Emergency return",
        "Security emergency",
        "Security emergency stop",
        "Security emergency return(have this status ?)",
        "indoor unit operation",
        "user operation",
        "Security sensor 1",
        "Security sensor 2",
        "Security sensor 3",
        "Security sensor 4",
        "Security sensor 5",
        "Security sensor 6",
        "Calling",
        "Incomming call",
        "Buzzer call",
        "Call connected",
        "Door Camera",
        "Delete",
        "Are you sure you want to delete?",
        "Video playback->playback",
        "Settings",
        "General",
        "Wi-Fi",
        "Sound",
        "Screen",
        "Recording/Monitoring",
        "Installation configuration",
        "Auto setup",
        "Date and time",
        "Language",
        "Change password for security",
        "Password",
        "Storage space",
        "SD card format",
        "Remaining data",
        "Internal storage",
        "External storage",
        "Would you like to format it?",
        "Would you like to delete it?",
        "Door opening method",
        "Open the door when called",
        "Open the door after calling",
        "Door opener module",
        "Door camera #2 : Number of door openings",
        "1 door",
        "2 doors",
        "Number of door openings",
        "Digital door lock",
        "Door camera",
        "Call time",
        "1 minute",
        "3 minute",
        "5 minute",
        "Sensor settings when going out",
        "Download mobile app",
        "Google Play Store",
        "Apple App Store",
        "Video call with mobile app, front door, CCTV monitoring is available.",
        "Initialization user data",
        "Initialize user data",
        "Data cannot be recovered after initialization.",
        "Initialization list",
        "Initialization is complete.",
        "Call log:",
        "Saved video:",
        "Emergency record:",
        "Complete",
        "Reset",
        "Initialization is Failed.",
        "Version information",
        "Current version",
        "Update version",
        "Update",
        "Updating....",
        "Volume",
        "The volume of the calling and receiving tone",
        "Ringtone 1",
        "Ringtone 2",
        "Ringtone 3",
        "Ringtone 4",
        "Ringtone 5",
        "Ringtone 6",
        "Buzzer",
        "Front door",
        "Entrance",
        "Common entrance",
        "Security office",
        "Extension",
        "Guard station",
        "Touch/notification",
        "LCD screen brightness",
        "Standby screen",
        "Off after 15 seconds",
        "Off after 30 seconds",
        "Off after 60 seconds",
        "Off after 180 seconds",
        "Screen Cleaning mode",
        "Automatic recording during calls",
        "Videos",
        "Picture",
        "OFF",
        "ON",
        "Motion detection",
        "Always monitoring",
        "10 seconds per camera",
        "30 seconds per camera",
        "60 seconds per camera",
        "This menu is to operate the product normally. Please note\n you don't need to change setting arbitraily.",
        "*We are not responsible for any situations caused by arbitrary settings.",
        "I have read and understood with your notice.",
        "Confirm",
        "Cancel",
        "Operating structure",
        "Building - House No.",
        "IP address",
        "IP address setting, default gateway, subnet mask, DNS setting",
        "Operation server IP address",
        "Local server, SIP server update server, CCTV server",
        "Common entrance IP",
        "Common entrance IP for interlocking elevator main device",
        "Guard station number",
        "Not used",
        "Sensor",
        "Sensor test",
        "Front door camera",
        "CCTV",
        "Factory reset",
        "Master Wallpad IP",
        "Sensor settings",
        "Sensor contact 1",
        "Sensor contact 2",
        "Sensor contact 3",
        "Sensor contact 4",
        "Sensor contact 5",
        "Sensor contact 6",
        "Sensor contact 7",
        "The outing is currently running.The 'Not used' setting is applied after the end of the outing.",
        "Not used",
        "NO",
        "NC",
        "ALWAYS_DETECT",
        "Sensor usage setting",
        "Sensor settings when going out",
        "Sensor settings for security mode",
        "Sensor for security",
        "Emergency",
        "Security mode (magnetic sensor)",
        "Security mode (PIR sensor)",
        "Alarm",
        "IP settings",
        "Use motion detection",
        "Select camera",
        "Storage format",
        "High",
        "Low",
        "Middle",
        "Sensitivity setting",
        "Motion detecting schedule",
        "LCD On when motion detected",
        "Schedule setting",
        "Start",
        "End",
        "Screen off",
        "Open frame",
        "Always monitoring has been applied. Are you sure you want to stop using daily monitoring and apply screen shutdown time?",
        "Wait display time",
        "Display Time",
        "Always",
        "Time designation",
        "Night mode",
        "Display item",
        "0 item",
        "1 item",
        "2 item",
        "3 item",
        "4 item",
        "5 item",
        "background",
        "Random image",
        "Basic",
        "Use night mode",
        "Adjusts the brightness of the screen to dim when there is no room light.",
        "Hour",
        "Calendar",
        "Recently saved video",
        "Front door video",
        "CCTV footage",
        "End time",
        "Reset household password",
        "Reset common entrance password",
        "New password",
        "Verify password",
        "Password reset",
        "Apply",
        "The password does not match.",
        "old password",
        "Are you sure you want to\ndelete the front door camera?",
        "Are you sure you want to\ndelete the CCTV?",
        "Password modified successfully",
        "Name modified successfully",
        "Name modified fail",
        "Format",
        "Failed",
        "Checking SD card...",
        "Formatting SD card... During formatting, never turn off power or romove SD card.",
        "SD card is not normal status or not inserted.",
        "Format is complete.",

};

/***
**   日期:2022-07-07 10:36:31
**   作者: leo.liu
**   函数作用：其他没有显示的字体
**   参数说明:
***/
static LANGUAGE_ID language_cur_id = LANGUAGE_ID_ENGLISH;

/***
** 日期: 2022-04-26 08:55
** 作者: leo.liu
** 函数作用：设置当前语言
** 返回参数说明：
***/
void language_id_set(LANGUAGE_ID id)
{
    language_cur_id = id;
}

/***
** 日期: 2022-04-26 08:56
** 作者: leo.liu
** 函数作用：获取当前语言ID
** 返回参数说明：
***/
LANGUAGE_ID language_id_get(void)
{
    return language_cur_id;
}

const char *lang_str_get(int lang_str_id)
{
    if (lang_xls_file_state_get() == true &&
        lang_xls_language_num_get() > 0)
    {
        if ((strcmp("NULL", lang_xls_str_get(lang_str_id, language_cur_id)) == 0) || (strcmp("", lang_xls_str_get(lang_str_id, language_cur_id)) == 0))
        {
            return lang_xls_str_get(lang_str_id, LANGUAGE_ID_ENGLISH);
        }
        return lang_xls_str_get(lang_str_id, language_cur_id);
    }
    else
    {
        return layout_default_language_string[lang_str_id];
    }
}

char *lang_str_get_by_language(xls_str_id_t index, LANGUAGE_ID language)
{
    if (lang_xls_file_state_get() == true &&
        lang_xls_language_num_get() > 0 &&
        lang_xls_str_num_get() == LANGUAGE_STRING_TOTAL)
    {
        if (strcmp("NULL", lang_xls_str_get(index, language)) == 0)
        {
            return lang_xls_str_get(index, LANGUAGE_ID_ENGLISH);
        }

        return lang_xls_str_get(index, language);
    }
    else
    {
        return layout_default_language_string[index];
    }
}

int language_total_get(void)
{
    if (lang_xls_file_state_get() == true &&
        lang_xls_language_num_get() > 0 &&
        /* lang_xls_null_str_num_get() == 0 && */
        lang_xls_str_num_get() == LANGUAGE_STRING_TOTAL)
    {
        return lang_xls_language_num_get();
    }

    return 1;
}

/*************************************************************************
 * @brief  输出全部语言文本到指定文件, 主要用于检查文案是否出错
 * @date   2022-12-02 09:06
 * @author xiaoele
 **************************************************************************/
void write_language_string_to_xls(void)
{
#if (0)
#define LANGUAGE_STRING_XLS "/tmp/nfs/language_default.csv"

    int fd = open(LANGUAGE_STRING_XLS, O_RDWR);
    if (fd < 0)
    {
        LAN_ERR("%s open fail..\n", LANGUAGE_STRING_XLS);
        return;
    }

    /* string */
    for (int i = 0; i < LANGUAGE_STRING_TOTAL; i++)
    {
        char buf[128] = {0};
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s\n", lang_str_get(i));
        write(fd, buf, strlen(buf));
    }

    close(fd);

#endif
}

/***** 公共字符串资源 *****/
static const char *lang_common_language[16][LANGUAGE_ID_TOTAL] =
    {
        /***** LANG_COMMON_ID_DOOR1 *****/
        {"Door1", "Door1", "门口机 1", "Door1", "Door1", "Door1", "Door1"},
        {"Door2", "Door2", "门口机 2", "Door2", "Door2", "Door2", "Door2"},
        {"Door3", "Door3", "门口机 3", "Door3", "Door3", "Door3", "Door3"},
        {"Door4", "Door4", "门口机 4", "Door4", "Door4", "Door4", "Door4"},
        {"Door5", "Door5", "门口机 5", "Door5", "Door5", "Door5", "Door5"},
        {"Door6", "Door6", "门口机 6", "Door6", "Door6", "Door6", "Door6"},
        {"CCTV1", "CCTV1", "CCTV1", "CCTV1", "CCTV1", "CCTV1", "CCTV1"},
        {"CCTV2", "CCTV2", "CCTV2", "CCTV2", "CCTV2", "CCTV2", "CCTV2"},
        {"CCTV3", "CCTV3", "CCTV3", "CCTV3", "CCTV3", "CCTV3", "CCTV3"},
        {"CCTV4", "CCTV4", "CCTV4", "CCTV4", "CCTV4", "CCTV4", "CCTV4"},
        {"CCTV5", "CCTV5", "CCTV5", "CCTV5", "CCTV5", "CCTV5", "CCTV5"},
        {"CCTV6", "CCTV6", "CCTV6", "CCTV6", "CCTV6", "CCTV6", "CCTV6"},
        {"Lobby", "Lobby", "Lobby", "Lobby", "Lobby", "Lobby", "Lobby"},
        {"Guard", "Guard", "Guard", "Guard", "Guard", "Guard", "Guard"},

};

typedef enum
{

    LANG_COMMON_ID_DOOR1,
    LANG_COMMON_ID_DOOR2,
    LANG_COMMON_ID_DOOR3,
    LANG_COMMON_ID_DOOR4,
    LANG_COMMON_ID_DOOR5,
    LANG_COMMON_ID_DOOR6,
    LANG_COMMON_ID_CCTV1,
    LANG_COMMON_ID_CCTV2,
    LANG_COMMON_ID_CCTV3,
    LANG_COMMON_ID_CCTV4,
    LANG_COMMON_ID_CCTV5,
    LANG_COMMON_ID_CCTV6,
    LANG_COMMON_ID_LOBBY,
    LANG_COMMON_ID_GUARD,
    LANG_COMMON_ID_TOTAL
} LANG_COMMON_ID;
/***
** 日期: 2022-04-28 13:55
** 作者: leo.liu
** 函数作用：获取公共字符串的字符串资源
** 返回参数说明：
***/
const char *language_common_string_get(int id)
{
    return lang_common_language[id][language_cur_id];
}

/***
**   日期:2022-05-24 10:04:38
**   作者: leo.liu
**   函数作用：根据通道类型获取字符串
**   参数说明:
***/
const char *language_common_ch_string_get(int ch)
{
    int group[][2] =
        {
            {TUYA_CH_DOOR1, LANG_COMMON_ID_DOOR1},
            {TUYA_CH_DOOR2, LANG_COMMON_ID_DOOR2},
            {TUYA_CH_DOOR3, LANG_COMMON_ID_DOOR3},
            {TUYA_CH_DOOR4, LANG_COMMON_ID_DOOR4},
            {TUYA_CH_DOOR5, LANG_COMMON_ID_DOOR5},
            {TUYA_CH_DOOR6, LANG_COMMON_ID_DOOR6},
            {TUYA_CH_CCTV1, LANG_COMMON_ID_CCTV1},
            {TUYA_CH_CCTV2, LANG_COMMON_ID_CCTV2},
            {TUYA_CH_CCTV3, LANG_COMMON_ID_CCTV3},
            {TUYA_CH_CCTV4, LANG_COMMON_ID_CCTV4},
            {TUYA_CH_CCTV5, LANG_COMMON_ID_CCTV5},
            {TUYA_CH_CCTV6, LANG_COMMON_ID_CCTV6},
            {TUYA_CH_LOBBY, LANG_COMMON_ID_LOBBY},
            {TUYA_CH_GUARD, LANG_COMMON_ID_GUARD},
        };
    for (int i = 0; i < sizeof(group) / sizeof(int); i++)
    {
        if (group[i][0] == ch)
        {
            return language_common_string_get(group[i][1]);
        }
    }
    return NULL;
}
