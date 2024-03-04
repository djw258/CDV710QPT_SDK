

#include "common/user_network.h"
#include "common/sat_main_event.h"
#include "common/sat_user_common.h"
#include "common/sat_user_time.h"
#include "common/user_data.h"
#include "layout_define.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "mxml-3.3.1/mxml.h"
#include "base64/include/libbase64.h"
#include "onvif.h"
#include "sha1/sha1.h"
#define DOOR_CAMERA_RECEIVE_BUFFER_MAX (1024 * 1024)
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 16:55:3
** 说明: 所有设备的组播IP
***********************************************/
#define USER_NETWORK_MULTICAST_IP "239.255.255.250"
#define USER_NETWORK_MULTICAST_PORT 3702
#define USER_NETWORK_TCP_SYNC_SERVER_PORT 80
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 16:55:3
** 说明: 所有设备的组播IP
***********************************************/
#define S200_OK_REQUEST_PATH ONVIF_XML_PATH "200_ok_requeset.html"
#define BAD_REQUEST_PATH ONVIF_XML_PATH "bad_requset.html"
#define DISCONVER_DEVICE_FILE_PATH ONVIF_XML_PATH "device_discovery_feedback.xml"
#define SYSTEM_TIME_RESPONSE_PATH ONVIF_XML_PATH "response_get_system_date_and_time.xml"
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 16:53:57
** 说明: 组播套接字
***********************************************/
static int user_linphone_multicast_fd = -1;

/*j解析设备*/
static bool discover_devices_data_parsing(const char *buf, const char *type, char *data, int size)
{
        bool reslut = false;
        char *pxml = strstr(buf, "<"); //?xml version=\"1.0\"
        if (pxml == NULL)
        {
                printf("%s\n", buf);
                return false;
        }

        mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);
        if (root != NULL)
        {
                mxml_node_t *probe = mxmlFindElementSub(root, root, type, NULL, NULL, MXML_DESCEND);
                if (probe != NULL)
                {
                        const char *text = mxmlGetText(probe, NULL);
                        if (text != NULL)
                        {
                                strncpy(data, text, size);
                        }
                        reslut = true;
                }
        }
        else
        {
                SAT_DEBUG("mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);")
        }
        mxmlDelete(root);
        return reslut;
}

/*向对端发送设备信息*/
static bool indoor_device_discover_processing(struct sockaddr_in *client_addr, const char *username)
{
        bool reslut = true;
        static long msg_count = 10000;
        char ip[32] = {0};
        char local_uuid[128] = {0};
        const char *local_usernmae = getenv("SIP");

        /*客户需求即搜索分机，用户为空则直接返回，
        若后续需求所有的室内机，则可以将用户为空作为
        全局搜索的条件。
        */
        if (username == NULL)
        {
                SAT_DEBUG("username is null");
                return false;
        }
        /*房号相同*/

        if (strncmp(local_usernmae, username, 12) == 0)
        {
                SAT_DEBUG("discover usernmae same ");
                return false;
        }

        /*不属于同一个房间内的*/
        if (strncmp(local_usernmae, username, 11))
        {
                SAT_DEBUG("discover usernmae diffrent house:\nlocal usernmae:%s\n dst username:%s", local_usernmae, username);
                return false;
        }

        char *xml_buffer = malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        char *xml_buffer_fmt = malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);

        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        size_t xml_len = sat_file_read(DISCONVER_DEVICE_FILE_PATH, xml_buffer, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (xml_len <= 0)
        {
                SAT_DEBUG("read %s failed ", DISCONVER_DEVICE_FILE_PATH);
                reslut = false;
                goto finish;
        }

        sprintf(local_uuid, "00010010-0001-1020-8000-%s", local_usernmae);
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);
        sprintf(xml_buffer_fmt, xml_buffer, msg_count++, local_uuid, local_uuid, local_uuid, local_usernmae, local_usernmae, local_usernmae, ip);
        //    SAT_DEBUG("%s", xml_buffer_fmt);
        /****************************************************************
        2022-09-20 author:leo.liu 说明:发送探测消息
        *****************************************************************/
        xml_len = sendto(user_linphone_multicast_fd, xml_buffer_fmt, strlen(xml_buffer_fmt), 0, (const struct sockaddr *)client_addr, sizeof(struct sockaddr_in));
        if (xml_len < 0)
        {
                SAT_DEBUG(" xml_len = sendto(user_linphone_multicast_fd, xml_buffer, xml_len, 0, (const struct sockaddr *)client_addr, sizeof(struct sockaddr_in));");
        }

finish:
        if (xml_buffer != NULL)
        {
                free(xml_buffer);
        }
        if (xml_buffer_fmt != NULL)
        {
                free(xml_buffer_fmt);
        }
        return reslut;
}
/*接受数据解析处理*/
static bool discover_receive_data_parsing_processing(struct sockaddr_in *client_addr, const char *buf, int size)
{
        char data[128] = {0};
        //   SAT_DEBUG("discover :%s", buf);
        if (discover_devices_data_parsing(buf, "Types", data, sizeof(data)) == true)
        {
                //  SAT_DEBUG("discover :%s", data);
                /* 设备搜索*/
                char *p = strstr(data, "CIP70QPT:");
                if (p != NULL)
                {
                        indoor_device_discover_processing(client_addr, p + 9);
                }
        }

        return true;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 17:26:43
** 说明: 用户网络任务
***********************************************/
static void *user_network_common_multicast_task(void *arg)
{
        int recv_len = 0;
        /*存在缺陷，如果两帧都是buffer_size大小的包，并且间隔小于500ms，会被丢失之前的包*/
        char *buffer = malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        struct sockaddr_in client_addr;
        usleep(3 * 1000 * 1000);
        sat_socket_udp_open(&user_linphone_multicast_fd, USER_NETWORK_MULTICAST_PORT, false);
        sat_socket_multicast_join(user_linphone_multicast_fd, USER_NETWORK_MULTICAST_IP);
        SAT_DEBUG("multicast join:%s:%d", USER_NETWORK_MULTICAST_IP, USER_NETWORK_MULTICAST_PORT);
        while (1)
        {

                memset(buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
                memset(&client_addr, 0, sizeof(struct sockaddr_in));
                recv_len = sat_socket_udp_receive(user_linphone_multicast_fd, buffer, DOOR_CAMERA_RECEIVE_BUFFER_MAX, &client_addr, 1000);
                if (recv_len > 0)
                {
                        discover_receive_data_parsing_processing(&client_addr, buffer, recv_len);
                }
        }

        return NULL;
}

/*********************************************** 		tcp   		***********************************************/
#define POST_ONVIF_DEVICE_HTML_TEXT "POST /onvif/device_service"
static bool tcp_device_serverce_xml_200_ok_requeset(int tcp_socket_fd, const char *string)
{
        int html_size = sat_file_size_get(S200_OK_REQUEST_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(S200_OK_REQUEST_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(S200_OK_REQUEST_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(S200_OK_REQUEST_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }

        char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_buffer == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                free(html_fmt);
                return false;
        }

        char *xml_fmt = strstr(html_fmt, "<?xml version=\"1.0\"");
        if (xml_fmt == NULL)
        {
                SAT_DEBUG("char *xml_fmt = strstr(html_fmt, \"<?xml version=1.0\n%s\n", html_fmt);
                free(html_buffer);
                free(html_fmt);
                return false;
        }

        char *xml_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (xml_buffer == NULL)
        {
                SAT_DEBUG("char *xml_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                free(html_buffer);
                free(html_fmt);
                return false;
        }

        sprintf(xml_buffer, xml_fmt, string);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        free(html_fmt);
        free(xml_buffer);
        sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);

        return true;
}
#define SYNC_FILE_DATA_MAX (1 * 64 * 1024)
static bool tcp_device_serverce_xml_get_userdata(int tcp_socket_fd, char *recv_string)
{
        bool reslut = false;
        /*base64解码*/
        size_t base64_decode_size = 0;
        char *base64_decode_buffer = (char *)malloc(SYNC_FILE_DATA_MAX);
        if (base64_decode_buffer == NULL)
        {
                SAT_DEBUG("malloc fail");
                return false;
        }
        base64_decode(recv_string, strlen(recv_string), base64_decode_buffer, &base64_decode_size, 0);
        // user_data_info * p = (user_data_info *)base64_decode_buffer;
        // if(p !=NULL)
        // {

        //         printf("=======call time is %d\n",p->etc.call_time);
        //         printf("=======system_mode is %d\n",p->system_mode);

        // }
        int send_len = 0;
        int remain = sizeof(user_data_info);
        while (remain > 0)
        {
                int read_len = remain > 1024 ? 1024 : remain;
                sat_msg_send_cmd_data(MSG_EVENT_CMD_SYNC_DATA, 0x00, &base64_decode_buffer[send_len], read_len, send_len, sizeof(user_data_info));
                remain -= read_len;
                send_len += read_len;
        }

        free(base64_decode_buffer);

        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "CIP-70QPT");
        return reslut;
}
static bool tcp_device_serverce_xml_get_networkdata(int tcp_socket_fd, char *recv_string)
{
        bool reslut = false;
        /*base64解码*/
        size_t base64_decode_size = 0;
        char *base64_decode_buffer = (char *)malloc(SYNC_FILE_DATA_MAX);
        if (base64_decode_buffer == NULL)
        {
                SAT_DEBUG("malloc fail");
                return false;
        }
        base64_decode(recv_string, strlen(recv_string), base64_decode_buffer, &base64_decode_size, 0);
        // user_network_info * p = (user_network_info *)base64_decode_buffer;
        // if(p !=NULL)
        // {

        //         printf("=======sip_user is %s\n",p->sip_user);
        //         printf("=======mask is %s\n",p->mask);
        //         printf("=======ip is %s\n",p->ip);

        // }
        int send_len = 0;
        int remain = sizeof(user_network_info);
        while (remain > 0)
        {
                int read_len = remain > 1024 ? 1024 : remain;
                sat_msg_send_cmd_data(MSG_EVENT_CMD_SYNC_DATA, 0x01, &base64_decode_buffer[send_len], read_len, send_len, sizeof(user_network_info));
                send_len += read_len;
                remain -= read_len;
        }

        free(base64_decode_buffer);
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "CIP-70QPT");
        return reslut;
}

static bool tcp_device_serverce_xml_get_asteriskdata(int tcp_socket_fd, char *recv_string)
{
        bool reslut = false;
        /*base64解码*/
        size_t base64_decode_size = 0;
        char *base64_decode_buffer = (char *)malloc(SYNC_FILE_DATA_MAX);
        if (base64_decode_buffer == NULL)
        {
                SAT_DEBUG("malloc fail");
                return false;
        }
        base64_decode(recv_string, strlen(recv_string), base64_decode_buffer, &base64_decode_size, 0);

        int send_len = 0;
        int remain = sizeof(asterisk_register_info) * 20;
        while (remain > 0)
        {
                int read_len = remain > 1024 ? 1024 : remain;
                sat_msg_send_cmd_data(MSG_EVENT_CMD_SYNC_DATA, 0x02, &base64_decode_buffer[send_len], read_len, send_len, sizeof(asterisk_register_info) * 20);
                send_len += read_len;
                remain -= read_len;
        }

        free(base64_decode_buffer);
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "CIP-70QPT");
        return reslut;
}
static bool tcp_device_serverce_xml_process_shellcmd(int tcp_socket_fd, char *recv_string)
{
        size_t base64_decode_size = strlen(recv_string);
        char *base64_decode_buffer = (char *)malloc(base64_decode_size);
        if (base64_decode_buffer == NULL)
        {
                SAT_DEBUG("malloc fail");
                return false;
        }
        base64_decode(recv_string, strlen(recv_string), base64_decode_buffer, &base64_decode_size, 0);

        printf("%s", base64_decode_buffer);
        system(base64_decode_buffer);
        free(base64_decode_buffer);
        return tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "CIP-70QPT");
}

#define TCP_RECEIVE_BUFFER_SIZE (128 * 1024)
static bool tcp_device_serverce_xml_process_systemtime(int tcp_socket_fd, char *recv_string)
{
        int html_size = sat_file_size_get(SYSTEM_TIME_RESPONSE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(SYSTEM_TIME_RESPONSE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(TCP_RECEIVE_BUFFER_SIZE);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(TCP_RECEIVE_BUFFER_SIZE);");
                return false;
        }

        memset(html_fmt, 0, TCP_RECEIVE_BUFFER_SIZE);
        int read_len = sat_file_read(SYSTEM_TIME_RESPONSE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }

        char *html_buffer = (char *)malloc(TCP_RECEIVE_BUFFER_SIZE);
        if (html_buffer == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(TCP_RECEIVE_BUFFER_SIZE);");
                free(html_fmt);
                return false;
        }

        char *xml_fmt = strstr(html_fmt, "<?xml version=\"1.0\"");
        if (xml_fmt == NULL)
        {
                SAT_DEBUG("char *xml_fmt = strstr(html_fmt, \"<?xml version=1.0\n%s\n", html_fmt);
                free(html_buffer);
                free(html_fmt);
                return false;
        }

        char *xml_buffer = (char *)malloc(TCP_RECEIVE_BUFFER_SIZE);
        if (xml_buffer == NULL)
        {
                SAT_DEBUG("char *xml_buffer = (char *)malloc(TCP_RECEIVE_BUFFER_SIZE);");
                free(html_buffer);
                free(html_fmt);
                return false;
        } // mac  ip
        struct tm tm;
        user_time_read(&tm);
        memset(xml_buffer, 0, TCP_RECEIVE_BUFFER_SIZE);
        sprintf(xml_buffer, xml_fmt, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year, tm.tm_mon, tm.tm_mday);
        int xml_size = strlen(xml_buffer);
        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, TCP_RECEIVE_BUFFER_SIZE);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);
        // printf("%s,size:%d\n", html_buffer, strlen(html_buffer));
        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_process_syncsystemtime(int tcp_socket_fd, char *recv_string)
{
        bool reslut = false;
        /*base64解码*/
        size_t base64_decode_size = 0;
        char *base64_decode_buffer = (char *)malloc(SYNC_FILE_DATA_MAX);
        if (base64_decode_buffer == NULL)
        {
                SAT_DEBUG("malloc fail");
                return false;
        }
        base64_decode(recv_string, strlen(recv_string), base64_decode_buffer, &base64_decode_size, 0);
        // user_network_info * p = (user_network_info *)base64_decode_buffer;
        // if(p !=NULL)
        // {

        //         printf("=======sip_user is %s\n",p->sip_user);
        //         printf("=======mask is %s\n",p->mask);
        //         printf("=======ip is %s\n",p->ip);

        // }
        int send_len = 0;
        int remain = sizeof(struct tm);
        while (remain > 0)
        {
                int read_len = remain > 1024 ? 1024 : remain;
                sat_msg_send_cmd_data(MSG_EVENT_CMD_SYNC_DATA, 0x03, &base64_decode_buffer[send_len], read_len, send_len, sizeof(struct tm));
                send_len += read_len;
                remain -= read_len;
        }

        free(base64_decode_buffer);
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "CIP-70QPT");
        return reslut;
}

bool modify_rtsp_url_by_ip(const char *original_url, const char *new_ip_address, char *modified_url)
{
        // 查找原始URL中的IP地址部分
        const char *ip_start = strstr(original_url, "://") + 3;
        if (ip_start == NULL)
        {
                return false;
        }
        const char *ip_end = strchr(ip_start, ':');
        if (ip_end == NULL)
        {
                ip_end = strchr(ip_start, '/');
                if (ip_end == NULL)
                {
                        return false;
                }
        }
        // 构建新的URL
        strncpy(modified_url, original_url, ip_start - original_url);     // 复制 "rtsp://"
        strcpy(modified_url + (ip_start - original_url), new_ip_address); // 添加新IP地址
        strcat(modified_url, ip_end);
        return true; // 添加端口号和路径
}

static bool tcp_device_serverce_xml_process_update_client_ip(int tcp_socket_fd, char *recv_string)
{
        int index = 0;
        char ip[16] = {0};
        sscanf(recv_string, "sip:20%d@%s", &index, ip);
        if ((index < 0) || (index > 8))
        {
                return false;
        }
        index -= 1;
        char newurl[128] = {0};
        for (int i = 0; i < network_data_get()->door_device[index].profile_token_num; i++)
        {
                memset(newurl, 0, sizeof(newurl));
                modify_rtsp_url_by_ip(network_data_get()->door_device[index].rtsp[i].rtsp_url, ip, newurl);

                strncpy(network_data_get()->door_device[index].rtsp[i].rtsp_url, newurl, sizeof(network_data_get()->door_device[index].rtsp[i].rtsp_url));
        }
        strncpy(network_data_get()->door_device[index].ipaddr, ip, sizeof(network_data_get()->door_device[index].ipaddr));
        network_data_save();
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "UpdateClientIP");
        return true;
}

static bool tcp_receive_device_service_html_processing(int tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        bool reslut = false;
        char *ptr = strstr((const char *)recv_data, "<");
        if (ptr == NULL)
        {
                SAT_DEBUG("%s", recv_data);
                return false;
        }
        char *data = (char *)malloc(SYNC_FILE_DATA_MAX);

        if (discover_devices_data_parsing(ptr, "SyncUserData", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SyncUserData\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_userdata(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "SyncNetworkData", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SyncNetworkData\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_networkdata(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "SyncAsteriskData", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SyncAsteriskData\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_asteriskdata(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "SyncDateTime", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SyncDateTime\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_syncsystemtime(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "ShellCmd", data, sizeof(data)) == true)
        {
                printf("[%s:%d] ShellCmd\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_shellcmd(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "GetSystemDateAndTime", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetSystemDateAndTime\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_systemtime(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "UpdateClientIP", data, SYNC_FILE_DATA_MAX) == true)
        {
                SAT_DEBUG("%s", recv_data);
                reslut = tcp_device_serverce_xml_process_update_client_ip(tcp_socket_fd, data);
        }
        else if (strstr((const char *)recv_data, "200 OK") == NULL)
        {
                SAT_DEBUG("%s", recv_data);
        }
        if (data != NULL)
        {
                free(data);
        }

        return reslut;
}
static bool tcp_receive_data_parsing_processing(int tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        if (strncasecmp((const char *)recv_data, POST_ONVIF_DEVICE_HTML_TEXT, strlen(POST_ONVIF_DEVICE_HTML_TEXT)) == 0)
        {
                return tcp_receive_device_service_html_processing(tcp_socket_fd, recv_data, recv_size);
        }
        return true;
}

static void *sat_socket_tcp_receive_task(void *arg)
{
        unsigned char *data = (unsigned char *)arg;
        int client_fd = *((int *)data);
        unsigned char *receive_data = data + sizeof(int);
        memset(receive_data, 0, TCP_RECEIVE_BUFFER_SIZE);
        int read_len = 0;
        int remain_len = TCP_RECEIVE_BUFFER_SIZE;
        int recv_len = 0;
        while ((recv_len = sat_socket_tcp_receive(client_fd, &receive_data[read_len], remain_len, 100)) > 0)
        {
                //  printf("%s\n", receive_data);la
                read_len += recv_len;
                remain_len -= recv_len;
        }
        if (read_len > 0)
        {
                tcp_receive_data_parsing_processing(client_fd, receive_data, recv_len);
        }
        sat_socket_close(client_fd);
        free(data);
        return NULL;
}

static void *user_network_tcp_task(void *arg)
{
        int server_fd = -1;

        struct sockaddr_in client_addr;
        sat_socket_tcp_open(&server_fd, USER_NETWORK_TCP_SYNC_SERVER_PORT, DEVICE_MAX);
        while (1)
        {
                memset(&client_addr, 0, sizeof(struct sockaddr_in));
                int client_fd = sat_socket_tcp_accept(server_fd, &client_addr, 100);
                if (client_fd >= 0)
                {
                        pthread_t thread_id;
                        unsigned char *receive_data = (unsigned char *)malloc(sizeof(int) + TCP_RECEIVE_BUFFER_SIZE);
                        int *p_socket_fd = (int *)receive_data;
                        *p_socket_fd = client_fd;
                        pthread_create(&thread_id, sat_pthread_attr_get(), sat_socket_tcp_receive_task, receive_data);
                        pthread_detach(thread_id);
                }
        }

        return NULL;
}

#define UDHCPC_TIMEOUT_MAX (300)
static bool ipaddr_udhcp_server_get_wait(void)
{
        int count = 0;
        char ip[32] = {0};
        char mac[128] = {0};
        sat_kill_task_process("udhcpc -b -i eth0 -s /etc/init.d/udhcpc.script &");
        system("ifconfig eth0 0.0.0.0");
        system("ifconfig eth0 down");
        usleep(10 * 1000);
        system("ifconfig eth0 up");
        usleep(10 * 1000);
        sat_network_udhcpc_ip("eth0");
        usleep(10 * 1000);
        while ((sat_ip_mac_addres_get("eth0", ip, mac, NULL) == false) || (ip[0] == '\0'))
        {
                usleep(10 * 1000);
                count++;
                if (count > UDHCPC_TIMEOUT_MAX)
                {
                        SAT_DEBUG("udhcpc get ip timeout");
                        break;
                }
        }
        if (count < UDHCPC_TIMEOUT_MAX)
        {
                if (strcmp(ip, "10.0.0.2"))
                {
                        SAT_DEBUG("udhcp ip get successs !(%s)", ip);
                        return true;
                }
        }
        return false;
}

/****************************************************************
**@日期: 2023-09-18
**@作者: leo.liu
**@功能: 手动获取的IP信息
*****************************************************************/
static bool obtain_ipddress_based_on_manual(void)

{
        char cmd[128] = {0};
        sprintf(cmd, "ifconfig eth0 %s netmask %s", network_data_get()->network.ipaddr, network_data_get()->network.mask[0] != 0 ? network_data_get()->network.mask : "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);
        return true;
}
#if 0
/****************************************************************
**@日期: 2023-09-18
**@作者: leo.liu
**@功能: 依据用户名或者网卡获取IP
*****************************************************************/
static bool obtain_ipaddress_based_on_username(void)
{
        // 010193001012
        int buffer[4] = {0};
        char ip[32] = {0};
        memset(buffer, 0, sizeof(buffer));
        char *username = network_data_get()->sip_user;

        buffer[0] = (username[0] - 48) * 100 + (username[1] - 48) * 10 + (username[2] - 48);
        buffer[1] = ((username[3] - 48) * 100 + (username[4] - 48) * 10 + (username[5] - 48));
        buffer[2] = (username[6] - 48) * 100 + (username[7] - 48) * 10 + (username[8] - 48);
        buffer[3] = (username[9] - 48) * 100 + (username[10] - 48) * 10 + (user_data_get()->system_mode & 0x0F);

        sprintf(ip, "%d.%d.%d.%d", buffer[0], buffer[1], buffer[2], buffer[3]);

        char cmd[128] = {0};
        sprintf(cmd, "ifconfig eth0 %s netmask %s", ip, "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);

        SAT_DEBUG("defalut ip:%s,mask:%s", network_data_get()->ip, network_data_get()->mask);
        return true;
}
#endif

static int convert_a_string_to_an_integer_number(const char *str, int len)
{
        int val = 0;
        for (int i = 0; i < len; i++)
        {
                if ((str[i] >= '0') && (str[i] <= '9'))
                {
                        val = val * 16 + (str[i] - '0');
                }
                else if ((str[i] >= 'A') && (str[i] <= 'F'))
                {
                        val = val * 16 + ((str[i] - 'A') + 10);
                }
                else if ((str[i] >= 'a') && (str[i] <= 'f'))
                {
                        val = val * 16 + ((str[i] - 'a') + 10);
                }
        }
        return val;
}

static bool obtain_ipaddress_based_on_mac(void)
{
        char mac[64] = {0};
        if (sat_ip_mac_addres_get("eth0", NULL, mac, NULL) == false)
        {
                SAT_DEBUG("if(sat_ip_mac_addres_get(\"eth0\",NULL,mac) == false)");
                return false;
        }

        /*获取mac的xx:xx:xx:xx:xx:xx 低字节*/
        char *str = mac;
        for (int i = 0; i < 3; i++)
        {
                str = strchr(str, ':');
                if (str == NULL)
                {
                        SAT_DEBUG("char *str = strchar(mac, ':');%d", i);
                        return false;
                }
                str++;
        }
        if (strlen(str) != 8)
        {
                SAT_DEBUG("char *str = %s", str);
                return false;
        }

        int ip_part[3] = {0};
        ip_part[0] = convert_a_string_to_an_integer_number(str, 2);
        ip_part[1] = convert_a_string_to_an_integer_number(str + 3, 2);
        ip_part[2] = convert_a_string_to_an_integer_number(str + 6, 2);

        char ip[32] = {0};
        sprintf(ip, "%d.%d.%d.%d", 10, ip_part[0], ip_part[1], ip_part[2]);
        strcpy(network_data_get()->network.ipaddr, ip);
        char cmd[128] = {0};
        sprintf(cmd, "ifconfig eth0 %s netmask %s", ip, "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);
        return true;
}

static bool local_network_config_get_remote(char *ip, char *mask, int length)
{
        if (network_data_get()->network.mask[0] == '\0')
        {
                strncpy(mask, "255.0.0.0", length);
        }
        else
        {
                strncpy(mask, network_data_get()->network.mask, length);
        }

        if (network_data_get()->network.ipaddr[0] == '\0')
        {
                strncpy(ip, "10.1.1.1", length);
        }
        else
        {
                strncpy(ip, network_data_get()->network.ipaddr, length);
        }
        printf("mask %s\n", mask);
        char tmp[32] = {0};
        strncpy(tmp, mask, sizeof(tmp));
        int num = convert_subnet_mask(tmp) / 8;
        if ((num != 1) && (num != 2) && (num != 3))
        {
                num = 3;
        }
        printf("length:%d\n ", num);

        char *p = ip;
        for (int i = 0; i < num; i++)
        {
                char *dot = strchr(p, '.');
                if ((dot == NULL) || ((dot + 1) == NULL))
                {
                        return false;
                }
                p = dot + 1;
        }

        if (num == 1)
        {
                memset(p, 0, 7);
                strcpy(p, "0.0.0");
                return true;
        }
        if (num == 2)
        {
                memset(p, 0, 5);
                strcpy(p, "0.0");
                return true;
        }
        if (num == 3)
        {
                memset(p, 0, 3);
                strcpy(p, "0");
                return true;
        }
        return false;
}

static bool add_multicase_routing_addres(void)
{
        char cmd[128] = {0};
        sprintf(cmd, "route add -net %s netmask %s eth0", "239.0.0.0", "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);

        sprintf(cmd, "route add -net %s netmask %s eth0", "225.0.0.0", "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);

        /*升级组播路由*/
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, "route add -net %s netmask %s eth0", "224.0.0.0", "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);

        memset(cmd, 0, sizeof(cmd));
        char mask[32] = {0};
        char ip[32] = {0};
        if (local_network_config_get_remote(ip, mask, sizeof(ip)) == true)
        {
                sprintf(cmd, "route add -net %s netmask %s eth0", ip, mask);
                system(cmd);
                SAT_DEBUG("%s ", cmd);
        }
        sprintf(cmd, "ip route add 10.0.0.0/8 via 10.0.0.1 dev eth0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);
        return true;
}

static bool netsta_port_information_kill(const char *info)
{

        char *end = strstr(info, "/");
        if (end == NULL)
        {
                return false;
        }
        *end = '\0';
        char *start = end - 1;
        while ((start != NULL) && !isspace(*start))
        {
                start--;
        }
        if (start != NULL)
        {
                char cmd[128] = {0};
                sprintf(cmd, "kill -s 9 %s", start);
                SAT_DEBUG("%s", cmd);
                system(cmd);
                return true;
        }
        return false;
}
static bool kill_related_port_process(char *port)
{
        return true;

        FILE *fp = popen("netstat -anp", "r");
        if (fp == NULL)
        {
                printf("popen netstat -anp\n");
                return false;
        }
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp) > 0)
        {
                if (strstr(buffer, port) != NULL)
                {
                        netsta_port_information_kill(buffer);
                }
        }
        pclose(fp);
        return true;
}

static bool automatic_ip_setting(void)
{
        /*杀死相关的端口进程*/
        kill_related_port_process("5060");
        /* 在开机脚本已经做了udhcpc后台运行，此处检测3sec，如果没有获取到IP，将执行下一步动作*/
        if ((network_data_get()->network.udhcp == false) || (ipaddr_udhcp_server_get_wait() == false))
        {
                sat_kill_task_process("udhcpc -b -i eth0 -s /etc/init.d/udhcpc.script");
                sat_kill_task_process("udhcpc -i eth0 -s /etc/init.d/udhcpc.script");

                // system("ps");
                network_data_get()->network.udhcp = false;
                if (user_data_get()->is_device_init)
                {
                        network_data_save();
                }
                if (network_data_get()->network.ipaddr[0] != '\0')
                {
                        /*手动设置的IP信息*/
                        obtain_ipddress_based_on_manual();
                }
                else
                {

                        /*默认IP*/
                        obtain_ipaddress_based_on_mac(); // obtain_ipaddress_based_on_username();
                }
                add_multicase_routing_addres();
        }
        return true;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 17:13:48
** 说明: 用户网络处理
***********************************************/
void user_network_init(void)
{
        pthread_t thread_id;
        automatic_ip_setting();
        pthread_create(&thread_id, sat_pthread_attr_get(), user_network_common_multicast_task, NULL);
        pthread_create(&thread_id, sat_pthread_attr_get(), user_network_tcp_task, NULL);
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 15:21:6
** 说明: 获取组播IP地址
***********************************************/
char *user_linphone_local_multicast_get(void)
{
        // ex:户外机:"010129001011" -->>> 225.1.1.10
        // ex:室内机:"010001001011" -->>> 225.1.1.10
        int value[4] = {0};
        static char multicase_ip[32] = {0};
        const char *username = getenv("SIP");
        if ((username == NULL) || (strlen(username) < 12))
        {
                SAT_DEBUG("getenv sip failed");
                return NULL;
        }
        value[0] = 225;
        value[1] = ((username[3] - 48) * 100 + (username[4] - 48) * 10 + (username[5] - 48)) & 0x1F;
        value[2] = (username[6] - 48) * 100 + (username[7] - 48) * 10 + (username[8] - 48);
        value[3] = (username[9] - 48) * 100 + (username[10] - 48) * 10;
        memset(multicase_ip, 0, sizeof(multicase_ip));
        sprintf(multicase_ip, "%d.%d.%d.%d", value[0], value[1], value[2], value[3]);

        //  SAT_DEBUG("sip:%s,multicase ip:%s", username, multicase_ip);
        return multicase_ip;
}
