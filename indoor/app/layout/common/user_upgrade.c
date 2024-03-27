#include "layout_define.h"
#include "user_upgrade.h"

#define MULTICAST_IP "224.0.0.1"
#define MULTICAST_PORT 5000
#define UPGRADE_TCP_PORT 5001
#define NETWORK_PACKAET_MAX (100 * 1024)
static pthread_mutex_t upgrade_outdoor_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool upgrade_outdoor_enable = false;
static char upgrade_outdoor_dst_ip[64] = {0};

enum
{
        NETWORK_CMD_DEVICE_UPGRADE,

        NETWORK_CMD_TOTAL
};
static bool sat_upgrade_firmware_transfer(int socket, const char *upgrade_path)
{
        FILE *fp = fopen(upgrade_path, "rb");
        if (fp == NULL)
        {
                return false;
        }
        int firmware_size = sat_file_size_get(upgrade_path);
        unsigned char *packet = malloc(NETWORK_PACKAET_MAX);
        int len = 0, read_size = 0;
        while ((len = fread(packet, 1, NETWORK_PACKAET_MAX, fp)) > 0)
        {
                if (sat_socket_tcp_send(socket, packet, len, 1000) == false)
                {
                        break;
                }

                read_size += len;
                printf("send size:%.0f%%\n", read_size * 100 / (firmware_size + 0.1)); //
        }
        free(packet);
        fclose(fp);
        return true;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:13:8
** 说明: 升级户外机
***********************************************/
bool upgrade_outdoor_device(int multicast_socket, const char *upgrade_path)
{
        int server_fd = -1;

        /***********************************************
        ** 作者: leo.liu
        ** 日期: 2023-1-7 10:14:26
        ** 说明: 建立tcp服务端
        ***********************************************/
        if (sat_socket_tcp_open(&server_fd, UPGRADE_TCP_PORT, 1) == false)
        {
                printf("[%s:%d] tcp open failed \n", __func__, __LINE__);
                return false;
        }

        printf("Start to query outdoor equipment...\n");
        /***********************************************
         ** 作者: leo.liu
         ** 日期: 2023-1-5 10:13:8
         ** 说明: 查询设备
         ***********************************************/
        char cmd[128] = {0};
        sprintf(cmd, "%d %d", NETWORK_CMD_DEVICE_UPGRADE, UPGRADE_TCP_PORT);
        if (sat_socket_udp_send(multicast_socket, (const char *)cmd, strlen((const char *)cmd), MULTICAST_IP, MULTICAST_PORT, 500) == true)
        {
                SAT_DEBUG("find the device");
                struct sockaddr_in client_addr;
                int client_fd[DEVICE_MAX] = {0};
                char client_ip[DEVICE_MAX][32] = {0};
                memset(client_ip, 0, sizeof(client_ip));
                memset(client_fd, 0, sizeof(client_fd));
                for (int i = 0; i < DEVICE_MAX; i++)
                {
                        memset(&client_addr, 0, sizeof(struct sockaddr_in));
                        client_fd[i] = sat_socket_tcp_accept(server_fd, &client_addr, 3 * 1000);
                        if (client_fd[i] < 0)
                        {
                                printf("client_fd[%d] accept failed\n", i);
                                break;
                        }
                        else if (sat_socket_tcp_receive(client_fd[i], (unsigned char *)client_ip[i], sizeof(client_ip[i]), 500) <= 0)
                        {
                                sat_socket_close(client_fd[i]);
                                client_fd[i] = -1;
                        }

                        SAT_DEBUG("client:%s dstip:%s ", client_ip[i], upgrade_outdoor_dst_ip);
                        if (strcmp(client_ip[i], upgrade_outdoor_dst_ip))
                        {
                                sat_socket_close(client_fd[i]);
                                client_fd[i] = -1;
                        }
                }

                for (int i = 0; i < DEVICE_MAX; i++)
                {
                        if (client_fd[i] > 0)
                        {
                                printf("upgrade ip:%s\n", client_ip[i]);
                                if (strcmp(client_ip[i], upgrade_outdoor_dst_ip) == 0)
                                {
                                        sat_upgrade_firmware_transfer(client_fd[i], upgrade_path);

                                        while (sat_socket_tcp_receive(client_fd[i], (unsigned char *)client_ip[i], sizeof(client_ip[i]), 500) > 0)
                                        {
                                                usleep(1000);
                                        }
                                }
                                sat_socket_close(client_fd[i]);
                                break;
                        }
                }
        }
        sat_socket_close(server_fd);

        return true;
}

static void *outdoor_network_upgrade_task(void *arg)
{
        int multicast_socket = -1;
        if (sat_socket_udp_open(&multicast_socket, MULTICAST_PORT, false) == false)
        {
                SAT_DEBUG("sat_socket_udp_open(&multicast_socket, MULTICAST_PORT, false) == false");
        }

        if (sat_socket_multicast_join(multicast_socket, MULTICAST_IP) == false)
        {
                SAT_DEBUG("sat_socket_multicast_join(network->multicast_socket, MULTICAST_IP) == false");
        }
        SAT_DEBUG("multicase join <<<<< %s:%d>>>>>>", MULTICAST_IP, MULTICAST_PORT);
        upgrade_outdoor_device(multicast_socket, SD_BASE_PATH "/SAT_ANYKA3918OS");

        upgrade_outdoor_enable = false;
        return NULL;
}

bool outdoor_network_upgrade(const char *ip)
{
        pthread_t pthread_id;
        pthread_mutex_lock(&upgrade_outdoor_mutex);
        if (upgrade_outdoor_enable == true)
        {
                SAT_DEBUG("upgrade task runing");
                pthread_mutex_unlock(&upgrade_outdoor_mutex);
                return false;
        }

        memset(upgrade_outdoor_dst_ip, 0, sizeof(upgrade_outdoor_dst_ip));
        strcpy(upgrade_outdoor_dst_ip, ip);
        upgrade_outdoor_enable = true;
        pthread_create(&pthread_id, sat_pthread_attr_get(), outdoor_network_upgrade_task, NULL);
        pthread_detach(pthread_id);

        pthread_mutex_unlock(&upgrade_outdoor_mutex);
        return true;
}

bool outdoor_network_enable_status_get(void)
{
        bool status = false;
        pthread_mutex_lock(&upgrade_outdoor_mutex);
        status = upgrade_outdoor_enable;
        pthread_mutex_unlock(&upgrade_outdoor_mutex);
        return status;
}
