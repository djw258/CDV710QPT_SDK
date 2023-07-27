

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
#include "common/user_key.h"
#include "common/sat_linphone_event.h"
#include "common/sat_main_event.h"
#include "common/user_data.h"
#include "common/user_linphone.h"
#include "common/sat_user_common.h"
#include "common/user_network.h"
#include "common/sat_user_time.h"
#include "common/user_gpio.h"
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
int main(int argc, char **argv)
{
      //  system("umount /etc/config");
      //  system("mtd_debug erase /dev/mtd7 0x0 0x19000");

        int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (mem_fd == -1)
        {
                perror("Failed to open /dev/mem");
                return 1;
        }

        // 根据具体Flash起始地址计算偏移量
        off_t flash_offset = 0x4ab000;

        // 读取JFFS2文件系统数据
        FILE *jffs2_file = fopen("/tmp/nfs/config.jffs2", "rb"); // 以二进制读取方式打开文件
        if (jffs2_file == NULL)
        {
                perror("Failed to open JFFS2 file");
                close(mem_fd);
                return 1;
        }

        // 定位到文件末尾，获取文件大小
        fseek(jffs2_file, 0, SEEK_END);
        long jffs2_file_size = ftell(jffs2_file);
        rewind(jffs2_file);

        // 根据计算的偏移量，使用lseek进行偏移
        off_t offset = lseek(mem_fd, flash_offset, SEEK_SET);
        if (offset == -1)
        {
                perror("Failed to seek to flash offset");
                fclose(jffs2_file);
                close(mem_fd);
                return 1;
        }

        // 创建缓冲区，读取JFFS2文件系统的数据
        unsigned char jffs2_data[jffs2_file_size];
        fread(jffs2_data, sizeof(unsigned char), jffs2_file_size, jffs2_file);

        // 将JFFS2文件系统的数据写入Flash
        ssize_t bytes_written = write(mem_fd, jffs2_data, jffs2_file_size);
        if (bytes_written == -1)
        {
                perror("Failed to write JFFS2 data to flash");
                fclose(jffs2_file);
                close(mem_fd);
                return 1;
        }

        // 关闭文件和/dev/mem设备
        fclose(jffs2_file);
        close(mem_fd);

        while (1)
                ;
        return 0;
}
#else

#include "anyka/ak_common.h"
#define SIP_CALL_QURY_TIMER 1000

static unsigned long long sip_call_timestamp = 0;
static bool sip_call_status = false;
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:7:35
** 说明: 初始化内核
***********************************************/
#define MAC_TXT_PATH "/etc/config/mac.txt"
/* static bool network_mac_get(char *buf)
{
        int i = 0, j = 0;
        char temp[128] = {0};
        if (access(MAC_TXT_PATH, F_OK))
        {
                return false;
        }
        if (sat_file_read(MAC_TXT_PATH, temp, sizeof(temp)) < 1)
        {
                return false;
        }

        for (i = j = 0; temp[i] != '\0'; i++)
        {
                if (isalnum(temp[i]))
                {
                        buf[j++] = temp[i];
                }
        }
        buf[j] = '\0';
        SAT_DEBUG("%s->%s\n", temp, buf);
        return true;
} */
static void linux_kerner_init(void)
{

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2022-12-9 13:53:54
        ** 说明: 暂时处理上电打开功放
        ***********************************************/
        // system("ifconfig eth0 txqueuelen 10000");
        system("echo 10485760 > /proc/sys/net/core/wmem_default");
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

                                bit[7]=1:分机(门口机/室内分机)
                                        bit[6:5]=00:室内分机
                                        bit[6:5]=10:户外机)

                BYTE3. 001 楼层号
                BYTE4. 001 千分位和百分位 - 房号
                           个位 -BYTE2 bit[7]=0
                                                1:室内机
                                                3:Gateway IP system
                                                7:Lobby IP system
                                                9:Guard Phone IP system
                                                5:KIOSK IP system
                                   -BYTE2 bit[7]=1
                                                个位分机ID号
        ***********************************************/
        /*读取mac地址*/
        /*     char mac[128] = {0};
            if (network_mac_get(mac) == true)
            {
                    setenv("SIP", mac, 1);
            }
            else
            {
                    SAT_DEBUG("not find mac addres,will restart in 3 seconds\n");
                    sleep(3);
                    system("reboot");
            } */
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 13:47:49
** 说明: 门铃呼叫
***********************************************/
static void key_call_process(unsigned int code, unsigned int state)
{
        char call[64] = {0};
        sprintf(call, "sip:100@%s:5066", user_data_get()->server_ip);
        SAT_DEBUG("CALL:%s", call);

        sat_linphone_call(call, true, true, NULL);
}

static void sys_timer_callback(void)
{
        static unsigned long long pre_timestamp = 0;
        unsigned long long timestamp = user_timestamp_get();

        /*每500ms跑一次*/
        if ((timestamp - pre_timestamp) < SIP_CALL_QURY_TIMER)
        {
                return;
        }
        pre_timestamp = timestamp;

        /*如果5秒内 sip_call_timestamp还是没有更新，则判断sip无连接， 需要主动挂断设备了*/
        if ((sip_call_status == true) && ((timestamp - sip_call_timestamp) > SIP_CALL_QURY_TIMER * 10))
        {
                //   SAT_DEBUG("query online timeout");
                //  sip_call_status = false;
                //    sat_linphone_handup(0xFF);
                return;
        }

        /*每隔500ms发送一次在线询问*/
        //   sat_linphone_calls_cmd_send();
        if ((user_data_get()->server_ip[0] != 0) && (user_data_get()->device.number[0] != 0))
        {
                char domain[32] = {0};
                sprintf(domain, "%s:5066", user_data_get()->server_ip);
                sat_linphone_register(user_data_get()->device.number, NULL, domain);
        }
}
/*连接设备发送正在通话的命令*/
static void sip_call_online_callback(void)
{
        sip_call_status = true;
        sip_call_timestamp = user_timestamp_get();
}

/*通过sat_linphone_call_status_query查询后返回状态的回调函数*/
static void sip_call_status_query_callback(bool state)
{
        //  SAT_DEBUG("-----------call status:%d", state);
        if (state == sip_call_status)
        {
                return;
        }
        if (state == true)
        {
                sip_call_timestamp = user_timestamp_get();
        }
        sip_call_status = state;
}

static void *media_server_task(void *arg)
{

        while (1)
        {
                extern int live555_media_server_start(const char *username, const char *password);
                live555_media_server_start("admin", user_data_get()->device.password);
                usleep(1000 * 1000);
        }
        return NULL;
}

/*视频流状态显示*/
static void video_stream_status_callback(bool en)
{
        SAT_DEBUG("==========>>en=%d", en);

        led_ctrl_enable(((en == false) || (ir_feed_read() == GPIO_LEVEL_LOW)) ? false : true);
        return;
}
/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 *
 * @功能: 主函数入口
 * @return:
 */
#include "anyka/ak_its.h"
int main(int argc, char *argv[])
{
        signal(SIGPIPE, SIG_IGN);
        remove("/tmp/.linphonerc");
        printf("*****************************************************\n");
        printf("*****        project: CDV810QPT(outdoor)        *****\n");
        printf("*****        author:  leo                       *****\n");
        printf("*****        date:   %s               *****\n",COMPILE_DATE_TIME());
        printf("*****************************************************\n");
        usleep(1000 * 1000);
        sdk_run_config config = {0};
        ak_sdk_init(&config);

        //  ak_its_start(8765);
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-5 11:38:19
        ** 说明: 用户数据初始化
        ***********************************************/
        user_data_init();

        linux_kerner_init();

        user_network_init();

        user_linphone_init();

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2022-12-28 11:47:47
        ** 说明: 呼叫按钮检测
        ***********************************************/
        user_key_init();
        user_gpio_init();
        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2022-12-28 13:47:0
        ** 说明: call事件处理
        ***********************************************/
        key_call_callback_register(key_call_process);
        system_timer_callback_register(sys_timer_callback);
        sip_call_online_func_register(sip_call_online_callback);
        sip_call_status_qury_register(sip_call_status_query_callback);
        video_stream_status_callback_register(video_stream_status_callback);

        if ((user_data_get()->server_ip[0] != 0) && (user_data_get()->device.number[0] != 0))
        {
                usleep(100 * 1000);
                char domain[32] = {0};
                sprintf(domain, "%s:5066", user_data_get()->server_ip);
                sat_linphone_register(user_data_get()->device.number, NULL, domain);
        }
        pthread_t thread_id;
        pthread_create(&thread_id, sat_pthread_attr_get(), media_server_task, NULL);
        /*
         * @日期: 2022-08-08
         * @作者: leo.liu
         * @注释: 主任务初始化
         */
        sat_mian_task_init();
        return 0;
}
#endif