

/*******************************************************************************
 * @file		 user_main.c
 * @version	 v1.0.0
 * @copyright	 COPYRIGHT &copy; 2022 CSG
 * @author 	 kayshi
 * @date		 2022-10-8
 * @brief
 * @attention
 * Modification History
 * DATE		 DESCRIPTION
 * ------------------------
 * - 2022-10-8  SYK Created
 *******************************************************************************/
#include <stdlib.h>
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <mqueue.h>
#include <ctype.h>
#include <signal.h>

#include "common/ssd20x_common.h"
#include "common/sat_main_event.h"
#include "layout_define.h"
#include "lang_xls.h"

#include <fcntl.h>
#include <sys/ioctl.h>

// #include "dds/topic_table.h"
#if 0
dds_inherit_listener
#endif

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-13 8:7:34
** 说明: 编译日期显示
***********************************************/
// #include "dmalloc.h"

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:7:35
** 说明: 初始化内核
***********************************************/
static void linux_kerner_init(void)
{
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2022-12-9 13:53:54
        ** 说明: 暂时处理上电打开功放
        ***********************************************/
        system("echo 12 > /sys/class/gpio/export");
        system("echo out > /sys/class/gpio/gpio12/direction");
        system("echo 1 > /sys/class/gpio/gpio12/value");

        system("ifconfig eth0 txqueuelen 10000");
        system("echo 10485760 > /proc/sys/net/core/wmem_max");
        system("echo 10485760 > /proc/sys/net/core/rmem_max");

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-5 14:2:17
        ** 说明: 设置环境变量
        ex :"010001001011" -> 010.001.001.011
                BYTE1. 010(固定部分).
                BYTE2. 001 -> 0x01
                                bit[7]=0:
                                bit[6:0] 栋号

                                bit[7]=1:
                                        分机(门口机/室内分机)
                                        bit[6:5]=00:室内分机 1000 0000(128) ~ 1001 1111(159)
                                        bit[6:5]=10:户外机 1100 0000(192) ~ 1101 1111(223)
                                        bit[6:5]=01:CCTV 1010 0000(160) ~ 1011 1111(191)

                BYTE3. 001 楼层号SS
                BYTE4. 001 百分位和十分位 - 房号
                           个位 -BYTE2 bit[7]=0
                                                1:室内机
                                                3:Gateway IP systemD
                                                7:Lobby IP system
                                                9:Guard Phone IP system
                                                5:KIOSK IP system
                                   -BYTE2 bit[7]=1
                                                个位分机ID号
        ***********************************************/
        //   setenv("SIP", network_data_get()->sip_user, 1);
}

/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: lvgl 心跳任务
 * @return:
 */
static void *lv_sys_tick_task(void *arg)
{
        struct timeval tv1, tv2;
        gettimeofday(&tv2, NULL);

        while (1)
        {
                gettimeofday(&tv1, NULL);
                lv_tick_inc(tv1.tv_sec * 1000 + tv1.tv_usec / 1000 - tv2.tv_sec * 1000 - tv2.tv_usec / 1000);
                gettimeofday(&tv2, NULL);
                usleep(1000);
        }

        return NULL;
}

/***
** 日期: 2022-04-25 14:22
** 作者: leo.liu
** 函数作用：lvgl的任务调度以及心跳包线程
** 返回参数说明：
***/
static void lv_task_scheduling_start(void)
{
        pthread_t task_id;

        _sat_layout_goto(sat_playout_get(logo), LV_SCR_LOAD_ANIM_FADE_OUT);
        pthread_create(&task_id, sat_pthread_attr_get(), lv_sys_tick_task, NULL);

        while (1)
        {
                lv_task_handler();
                usleep(1000);
        }
}

static bool is_asterisk_server_sync_data_force = false;
void asterisk_server_sync_data_force(bool is_sync)
{
        is_asterisk_server_sync_data_force = is_sync;
}
/*使用线程异步发送*/
static void *asterisk_server_sync_task(void *arg)
{

        static bool is_registers_online[20] = {0};

        const asterisk_register_info *p_register_info = asterisk_register_info_get();
        while (1)
        {

                for (int i = 0; i < ASTERISK_REIGSTER_DEVICE_MAX; i++)
                {

                        /*主机或者门口机过滤*/
                        if ((p_register_info[i].name[0] == '\0') || (strncmp(p_register_info[i].name, "501", 3) == 0) || (strncmp(p_register_info[i].name, "20", 2) == 0))
                        {
                                if ((p_register_info[i].name[0] == '\0') && (is_registers_online[i] == true))
                                {
                                        is_registers_online[i] = false;
                                }
                                continue;
                        }

                        unsigned long long timestamp = user_timestamp_get();

                        /*上次设备不在线，这次在线，状态发生改变*/
                        if (((is_registers_online[i] == false) || (is_asterisk_server_sync_data_force == true)) && (abs(timestamp - p_register_info[i].timestamp) < (10 * 1000)))
                        {
                                is_registers_online[i] = true;
                                is_asterisk_server_sync_data_force = false;

                                sat_ipcamera_data_sync(0x00, 0x01, (char *)user_data_get(), sizeof(user_data_info), 10, 100);
                                sat_ipcamera_data_sync(0x01, 0x01, (char *)network_data_get(), sizeof(user_network_info), 10, 100);                               
                                sat_ipcamera_data_sync(0x02, 0x01, (char *)asterisk_register_info_get(), sizeof(asterisk_register_info)*ASTERISK_REIGSTER_DEVICE_MAX, 10, 100);

                        }
                        else if ((is_registers_online[i] == true) && (abs(timestamp - p_register_info[i].timestamp) > (10 * 1000)))
                        {
                                is_registers_online[i] = false;
                                printf("%s %s offline \n", p_register_info[i].name, p_register_info[i].name);
                        }
                        usleep(1000 * 1000);
                }
        }
        return NULL;
}

/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: 主函数入口
 * @return:
 */
int main(int argc, char *argv[])
{
        signal(SIGPIPE, SIG_IGN);
        /*先干掉asterik服务器*/
        remove("/tmp/.linphonerc");
        sat_kill_task_process("{safe_asterisk} /bin/sh /app/asterisk/sbin/safe_asterisk");
        sat_kill_task_process("/app/asterisk/sbin/asterisk -f -vvvg -c");

        user_data_init();
        network_data_init();

        linux_kerner_init();

        /*
         * @日期: 2022-08-06
         * @作者: leo.liu
         * @注释: 平台sdk初始化
         */
        platform_sdk_init();

        init_language_xls_info();

        /*lv_task_handler
         * @日期: 2022-08-08
         * @作者: leo.liu
         * @注释: lvgl core初始化
         */
        lv_init();

        /*
         * @日期: 2022-08-08
         * @作者: leo.liu
         * @注释:注册显示设备
         */
        lv_port_disp_init();

        /*
         * @日期: 2022-08-08
         * @作者: leo.liu
         * @注释: 注册输入设备
         */
        lv_port_indev_init();

        /*
         * @日期: 2022-08-08
         * @作者: leo.liu
         * @注释: 主任务初始化
         */
        sat_mian_task_init();

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2022-11-9 10:15:48
        ** 说明: 创建信令任务
        ***********************************************/
        if ((user_data_get()->system_mode & 0x0F) == 0x01)
        {
                pthread_t task_id;
                pthread_create(&task_id, sat_pthread_attr_get(), asterisk_server_sync_task, NULL);
        }
        /*
         * @日期: 2022-08-08
         * @作者: leo.liu
         * @注释: 开启任务调度
         */
        lv_task_scheduling_start();

        return 0;
}
