#include "common/user_network.h"
#include "common/sat_main_event.h"
#include "common/sat_user_common.h"
#include "common/user_gpio.h"
#include "common/user_data.h"
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

#include "common/sat_user_time.h"
#include "common/user_data.h"
#include "common/user_key.h"
#include "common/user_linphone.h"
#include "common/sat_user_time.h"
#include "anyka/ak_common.h"
#include "anyka/ak_vi.h"
#include "anyka/ak_vpss.h"
#include "mxml-3.3.1/mxml-private.h"
#include "mxml-3.3.1/mxml.h"
#include "onvif/onvif.h"
#include "base64/include/libbase64.h"
#include "sha1/sha1.h"

#define DISCONVER_DEVICE_FILE_PATH ONVIF_XML_PATH "device_discovery_feedback.xml"
#define BAD_REQUEST_PATH ONVIF_XML_PATH "bad_requset.html"
#define S200_OK_REQUEST_PATH ONVIF_XML_PATH "200_ok_requeset.html"
#define GET_ONVIF_MEDIA_PROFILE_PATH ONVIF_XML_PATH "onvif_media_get_profile.html"
#define DEVICE_SERVICE_GET_INFORMATION_PATH ONVIF_XML_PATH "device_service_get_information.html"
#define DEVICE_SERVICE_GET_CAPABILITIES_PATH ONVIF_XML_PATH "device_serverce_get_capabilities.html"
#define DEVICE_SERVICE_GET_DNS_PATH ONVIF_XML_PATH "device_service_get_dns.html"
#define DEVICE_SERVICE_SET_DNS_PATH ONVIF_XML_PATH "device_service_set_dns.html"
#define DEVICE_SERVICE_GET_NETWORKINTERFACE_PATH ONVIF_XML_PATH "device_service_get_networkinterfaces.html"
#define DEVICE_SERVICE_SET_NETWORKINTERFACE_PATH ONVIF_XML_PATH "device_service_set_networkinterfaces.html"
#define DEVICE_SERVICE_GET_INCLUDECAPABILITY_PATH ONVIF_XML_PATH "device_service_get_IncludeCapability.html"
#define DEVICE_SERVICE_GET_SYSTEM_TIME_AND_TIME_PATH ONVIF_XML_PATH "device_service_get_systemtime.html"
#define DEVICE_SERVICE_GET_SCOPES_PATH ONVIF_XML_PATH "device_serverce_get_scopes.html"
#define DEVICE_SERVICE_SET_SCOPES_PATH ONVIF_XML_PATH "set_device_service_scopes.html"
#define DEVICE_SERVICE_GET_USERS_PATH ONVIF_XML_PATH "get_device_service_users.html"
#define DEVICE_SERVICE_SET_USERS_PATH ONVIF_XML_PATH "set_device_service_users.html"
#define DEVICE_SERVICE_GET_NTP_PATH ONVIF_XML_PATH "set_device_service_get_ntp.html"
#define DEVICE_SERVICE_SET_NTP_PATH ONVIF_XML_PATH "set_device_service_set_ntp.html"
#define DEVICE_SERVICE_GET_HOSTNAME_PATH ONVIF_XML_PATH "set_device_service_get_hostname.html"
#define DEVICE_SERVICE_GET_DISCOVERY_PATH ONVIF_XML_PATH "set_device_service_get_discovery.html"
#define DEVICE_SERVICE_GET_GATEWAY_PATH ONVIF_XML_PATH "set_device_service_get_gateway.html"
#define DEVICE_SERVICE_GET_NETWORK_PROTOCOLS_PATH ONVIF_XML_PATH "set_device_service_get_network_protocols.html"
#define GET_ONVIF_STREAM_URI_PATH ONVIF_XML_PATH "get_streamuri.html"
#define GET_ONVIF_SNAPSHOT_URI_PATH ONVIF_XML_PATH "get_snapshoturi.html"
#define GET_ONVIF_VIDEO_SOURCE_PATH ONVIF_XML_PATH "onvif_media_get_videosources.html"
#define GET_ONVIF_INITIALTERMINATIONTIME_PATH ONVIF_XML_PATH "event_get_initialterminationtime.html"
#define GET_ONVIF_SNAPSHOT_JPG_PATH ONVIF_XML_PATH "push_snapshot_jpeg.xml"
#define FILE_ONVIF_SNAPSHOT_JPG_PATH ONVIF_XML_PATH "snapshot.jpg"
#define GET_ONVIF_PROFILE_1_PATH ONVIF_XML_PATH "get_media_profile1.html"
#define GET_ONVIF_MEDIA_VIDEO_SOURCE_CONFIG_PATH ONVIF_XML_PATH "get_media_video_source_config.html"
#define GET_ONVIF_IMAGEING_GET_OPTIONS_RESPONSE_PATH ONVIF_XML_PATH "post_onvif_imaging_get_options_response.xml"
#define GET_ONVIF_IMAGEING_GET_MOVE_OPTIONS_RESPONSE_PATH ONVIF_XML_PATH "post_onvif_imaging_get_move_options_response.xml"
#define GET_ONVIF_IMAGEING_GET_IMAGING_SETTING_RESPONSE_PATH ONVIF_XML_PATH "post_onvif_imaging_get_imaging_setting_response.xml"
#define GET_ONVIF_IMAGEING_SET_IMAGING_SETTING_RESPONSE_PATH ONVIF_XML_PATH "post_onvif_imaging_set_imaging_setting_response.xml"

#define DEVICE_SERVICE_SET_GATEWAY_PATH ONVIF_XML_PATH "post_onvif_service_set_gateway_response.xml"

#define DOOR_CAMERA_RECEIVE_BUFFER_MAX (256 * 1024)

static int user_linphone_multicast_fd = -1;
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 16:55:3
** 说明: 所有设备的组播IP
***********************************************/
#define USER_NETWORK_MULTICAST_IP "239.255.255.250"
#define USER_NETWORK_MULTICAST_PORT 3702
#define USER_NETWORK_TCP_SYNC_SERVER_PORT 80

/*j解析设备*/
static bool discover_devices_data_parsing(const char *buf, const char *type, char *data, int size)
{
        bool reslut = false;
        const char *pxml = strstr(buf, "<"); //"<?xml version=\"1.0\"");
        if (pxml == NULL)
        {
                // printf("%s\n", buf);
                // return false;
                pxml = buf;
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
        /*  else
         {
                 SAT_DEBUG("mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);")
         } */
        mxmlDelete(root);
        return reslut;
}

/*j解析设备*/
static bool discover_devices_data_parsing_form_parent(const char *buf, const char *parent, const char *type, char *data, int size)
{
        bool reslut = false;
        const char *pxml = strstr(buf, "<"); //"<?xml version=\"1.0\"");
        if (pxml == NULL)
        {
                // printf("%s\n", buf);
                // return false;
                pxml = buf;
        }

        mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);
        if (root != NULL)
        {
                mxml_node_t *node = mxmlFindElementSub(root, root, parent, NULL, NULL, MXML_DESCEND);
                if (parent != NULL)
                {
                        mxml_node_t *probe = mxmlFindElementSub(node, node, type, NULL, NULL, MXML_DESCEND);
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
        }

        /*  else
         {
                 SAT_DEBUG("mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);")
         } */
        mxmlDelete(root);
        return reslut;
}
static bool discover_devices_data_uuid_get(const char *buf, int size, char *uuid, int uuid_size)
{
        bool reslut = false;
        const char *pxml = strstr(buf, "<"); //<?xml version=\"1.0\"");
        if (pxml == NULL)
        {
                //    printf("%s\n", buf);
                //  return false;
                pxml = buf;
        }

        mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);
        if (root != NULL)
        {
                mxml_node_t *probe = mxmlFindElementSub(root, root, "MessageID", NULL, NULL, MXML_DESCEND);
                if (probe != NULL)
                {
                        const char *text = mxmlGetText(probe, NULL);
                        if (text != NULL)
                        {
                                strncpy(uuid, text, uuid_size);
                                reslut = true;
                        }
                }
        }
        else
        {
                SAT_DEBUG("%s", buf);
        }
        mxmlDelete(root);
        return reslut;
}
/*向对端发送设备信息*/
static bool doorcamera_device_discover_processing(struct sockaddr_in *client_addr, const char *relatesto_uuid)
{
        bool reslut = true;
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
        static long msg_count = 1;
        char ip[32] = {0};
        char local_uuid[128] = {0};
        const char *username = getenv("SIP");
        char regist_id[9] = {0};
        for (int i = 6, j = 0; i < 12; i += 2, j += 3)
        {
                strncat(&regist_id[j], &username[i], 2);
                if ((j + 2) < (sizeof(regist_id) - 3))
                {
                        regist_id[j + 2] = '-';
                }
        }

        sprintf(local_uuid, "urn:uuid:00010010-0001-1020-8000-%s", username);
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);
        sprintf(xml_buffer_fmt, xml_buffer, msg_count++, local_uuid, relatesto_uuid, local_uuid, regist_id, username, user_data_get()->device.name, ip);
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
        if (discover_devices_data_parsing(buf, "Types", data, sizeof(data)) == true)
        {
                // SAT_DEBUG("discover :%s", type);
                /* 设备搜索*/
                if ((strstr(data, "Device") != NULL) || (strstr(data, "NetworkVideoTransmitter") != NULL) || (strstr(data, "NetworkVideoDisplay")))
                {
                        char relatesto_uuid[128] = {0};
                        if (discover_devices_data_uuid_get(buf, size, relatesto_uuid, sizeof(relatesto_uuid)) == true)
                        {
                                if ((strstr(relatesto_uuid, "18fafa95-0721-0823-0108-a98aa2dda8f7")) && (user_key_state_get() != KEY_STATE_LONG_DOWN))
                                {
                                        SAT_DEBUG("CIP70QPT is searching for a device, please hold down the call button");
                                        return false;
                                }
                                return doorcamera_device_discover_processing(client_addr, relatesto_uuid);
                        }
                }
                SAT_DEBUG("%s", buf);
        }
        return true;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 17:26:43
** 说明: 用户网络任务
***********************************************/
static void *user_network_task(void *arg)
{
        int recv_len = 0;
        /*存在缺陷，如果两帧都是buffer_size大小的包，并且间隔小于500ms，会被丢失之前的包*/
        char *buffer = malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        struct sockaddr_in client_addr;

        sat_socket_udp_open(&user_linphone_multicast_fd, USER_NETWORK_MULTICAST_PORT, true);
        sat_socket_multicast_join(user_linphone_multicast_fd, USER_NETWORK_MULTICAST_IP);
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
#define POST_ONVIF_MEDIA_TEXT "POST /onvif/media"
#define POST_SUBSCRIPTION_TEXT "POST /Subscription"
#define POST_ANALYTICS_TEXT "POST /onvif/analytics"
#define POST_EVENT_TEXT "POST /onvif/event"
#define GET_SNAPSHOT_JPG_TEXT "GET /images/snapshot.jpg"
#define GET_ONVIF_IMAGEING_TEXT "POST /onvif/imaging"
static bool tcp_device_serverce_xml_parsing(const char *xml,
                                            char *username, int username_size,
                                            char *password_digest, int passowrd_digest_size,
                                            char *nonce, int nonce_size,
                                            char *created, int created_size)
{
        bool reslut = false;
        const char *pxml = strstr(xml, "<"); // "<?xml version=\"1.0\"");
        if (pxml == NULL)
        {
                // printf("%s\n", xml);
                // return false;
                pxml = xml;
        }

        mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);
        if (root != NULL)
        {
                mxml_node_t *node = mxmlFindElementSub(root, root, "Username", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        const char *text = mxmlGetText(node, NULL);
                        if (text != NULL)
                        {
                                //  SAT_DEBUG("Username:%s\n\n%s\n\n", text,pxml);
                                strncpy(username, text, username_size);
                                reslut = true;
                        }
                }

                node = mxmlFindElementSub(root, root, "Password", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        const char *text = mxmlGetText(node, NULL);
                        if (text != NULL)
                        {
                                //  SAT_DEBUG("Password:%s", text);
                                strncpy(password_digest, text, passowrd_digest_size);
                                reslut = true;
                        }
                }

                node = mxmlFindElementSub(root, root, "Nonce", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        const char *text = mxmlGetText(node, NULL);
                        if (text != NULL)
                        {
                                //   SAT_DEBUG("Nonce:%s", text);
                                strncpy(nonce, text, nonce_size);
                                reslut = true;
                        }
                }

                node = mxmlFindElementSub(root, root, "Created", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        const char *text = mxmlGetText(node, NULL);
                        if (text != NULL)
                        {
                                //  SAT_DEBUG("created:%s", text);
                                strncpy(created, text, created_size);
                                reslut = true;
                        }
                }
        }
        else
        {
                SAT_DEBUG("mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);")
        }
        mxmlDelete(root);

        return reslut;
}

static bool onvif_username_token_check(const char *admin, const char *from_token, const char *nonce, const char *created)
{
        // base64decode(nonce);
        char nonce_decode[128] = {0};
        size_t nonce_decode_len = 0;
        base64_decode(nonce, strlen(nonce), (char *)nonce_decode, &nonce_decode_len, 0);

        // sha1(base64decode(nonce) + date +password);
        SHA1_CTX ctx;
        SHA1Init(&ctx);
        SHA1Update(&ctx, (unsigned char *)nonce_decode, nonce_decode_len);
        SHA1Update(&ctx, (unsigned char *)created, strlen(created));
        SHA1Update(&ctx, (unsigned char *)user_data_get()->device.password, strlen(user_data_get()->device.password));

        // 输出sha1后的数据
        char hash2[20];
        SHA1Final((unsigned char *)hash2, &ctx);

        // base64编码
        char src_token[128] = {0};
        struct base64_state state;
        base64_stream_encode_init(&state, 0);
        size_t encode_len = 0;
        base64_stream_encode(&state, hash2, 20, src_token, &encode_len);
        size_t final_len = 0;
        base64_stream_encode_final(&state, &src_token[encode_len], &final_len);

        if (strcmp(from_token, src_token))
        {
                SAT_DEBUG("password error");
                return false;
        }
        SAT_DEBUG("password check success");
        return true;
}

static bool tcp_device_serverce_xml_bad_request(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(BAD_REQUEST_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(BAD_REQUEST_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(BAD_REQUEST_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(BAD_REQUEST_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }

        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_200_ok_requeset(int *tcp_socket_fd, const char *string)
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
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_get_information(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_INFORMATION_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_INFORMATION_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }

        free(html_fmt);
        return true;
}

static bool tcp_device_servrce_xml_get_capabilities(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_CAPABILITIES_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_CAPABILITIES_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_CAPABILITIES_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        char ip[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, ip, ip, ip, ip, ip, ip, ip, ip, ip, ip, ip);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_servrce_xml_get_scopes(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_SCOPES_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_SCOPES_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_SCOPES_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        const char *username = getenv("SIP");
        char regist_id[9] = {0};
        for (int i = 6, j = 0; i < 12; i += 2, j += 3)
        {
                strncat(&regist_id[j], &username[i], 2);
                if ((j + 2) < (sizeof(regist_id) - 3))
                {
                        regist_id[j + 2] = '-';
                }
        }
        char mac[64] = {0};
        sat_ip_mac_addres_get("eth0", NULL, mac, NULL);

        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, regist_id, mac, user_data_get()->device.name);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_servrce_xml_get_register(int *tcp_socket_fd, const char *xml)
{
        char sip_uri[128] = {0};
        if (discover_devices_data_parsing(xml, "Register", sip_uri, sizeof(sip_uri)) == false)
        {
                return false;
        }
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, sip_uri);
        sat_socket_close(*tcp_socket_fd);
        *tcp_socket_fd = -1;

        bool is_reboot = false;
        char uri[128] = {0};
        strcpy(uri, sip_uri);
        char *s = strstr(uri, "sip:");
        if (s == NULL)
        {
                SAT_DEBUG("parsing sip error ");
                return false;
        }
        char *p = strchr(s, '@');
        if (p == NULL)
        {
                SAT_DEBUG("parsing sip error ");
                return false;
        }
        *p = '\0';
        p++;
        if ((strncmp(user_data_get()->device.number, s + 4, sizeof(user_data_get()->device.number))) || (strncmp(user_data_get()->server_ip, p, sizeof(user_data_get()->server_ip))))
        {
                memset(user_data_get()->device.number, 0, sizeof(user_data_get()->device.number));
                memset(user_data_get()->server_ip, 0, sizeof(user_data_get()->server_ip));
                strncpy(user_data_get()->device.number, s + 4, sizeof(user_data_get()->device.number));
                strncpy(user_data_get()->server_ip, p, sizeof(user_data_get()->server_ip));
                SAT_DEBUG("NUMBER:%s,domain:%s", user_data_get()->device.number, user_data_get()->server_ip);
                is_reboot = true;
        }
        if (is_reboot == true)
        {
                user_data_save();
                led_ctrl_blink(3);

                system("reboot");
        }
        return true;
}

static bool tcp_device_servrce_xml_get_delete(int *tcp_socket_fd, const char *xml)
{
        char sip_uri[128] = {0};
        if (discover_devices_data_parsing(xml, "Delete", sip_uri, sizeof(sip_uri)) == false)
        {
                return false;
        }
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, "delete");
        sat_socket_close(*tcp_socket_fd);
        *tcp_socket_fd = -1;

        memset(user_data_get()->device.number, 0, sizeof(user_data_get()->device.number));
        memset(user_data_get()->server_ip, 0, sizeof(user_data_get()->server_ip));
        user_data_save();
        led_ctrl_blink(3);
        system("reboot");
        return true;
}

static bool tcp_device_servrce_xml_get_device_name(int *tcp_socket_fd, const char *xml)
{
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, user_data_get()->device.name);
        return true;
}

static bool tcp_device_servrce_xml_set_device_name(int *tcp_socket_fd, const char *xml)
{
        char sip_uri[128] = {0};
        if (discover_devices_data_parsing(xml, "SetName", sip_uri, sizeof(sip_uri)) == false)
        {
                return false;
        }
        strncpy(user_data_get()->device.name, sip_uri, sizeof(user_data_get()->device.name));
        user_data_save();
        SAT_DEBUG("change name:%s", sip_uri);
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, sip_uri);
        return true;
}

static bool tcp_device_servrce_xml_get_version_name(int *tcp_socket_fd, const char *xml)
{
        char mac[64] = {0};
        sat_ip_mac_addres_get("eth0", NULL, mac, NULL);
        char version[128] = {0};
        struct tm tm;
        if (platform_build_date_get(&tm) == true)
        {
                sprintf(version, "%04d-%02d-%02d(%02d:%02d:%02d)%s-MAC:%s", 2024, 03, 29, 20, 00, 36 /* tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec */, SYSTEM_VERSION, mac);
                tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, version);
        }
        else
        {
                tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }

        return true;
}
static bool tcp_device_servrce_xml_channge_device_password(int *tcp_socket_fd, const char *xml)
{
        char sip_uri[128] = {0};
        if (discover_devices_data_parsing(xml, "ChangePassword", sip_uri, sizeof(sip_uri)) == false)
        {
                return false;
        }
        strncpy(user_data_get()->device.password, sip_uri, sizeof(user_data_get()->device.password));
        user_data_save();
        SAT_DEBUG("change password:%s", sip_uri);
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, sip_uri);
        sat_socket_close(*tcp_socket_fd);
        *tcp_socket_fd = -1;

        led_ctrl_blink(3);
        system("reboot");
        return true;
}

static bool tcp_device_serverce_xml_get_dns(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_DNS_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_DNS_PATH); \n");
                return false;
        }
        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }
        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_DNS_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        } // mac  ip
          //   sat_ip_mac_addres_get("eth0", ip, mac);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, user_data_get()->network.dns[0] == '\0' ? "0.0.0.0" : user_data_get()->network.dns);
        int xml_size = strlen(xml_buffer);
        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);
        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);
        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_get_networkinterface(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_NETWORKINTERFACE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_NETWORKINTERFACE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_NETWORKINTERFACE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        } // mac  ip
        char mac[64] = {0};
        char ip[32] = {0};
        char mask[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, mac, mask);
        int mask_vol = convert_subnet_mask(mask);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, mac, ip, mask_vol, user_data_get()->network.udhcp == true ? "true" : "false");
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_get_includecapability(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_INCLUDECAPABILITY_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_INCLUDECAPABILITY_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_INCLUDECAPABILITY_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        } // mac  ip
        char mac[64] = {0};
        char ip[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, mac, NULL);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, ip, ip, ip, ip, ip, ip, ip, ip, ip, ip, ip);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_get_systemdataandtime(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_SYSTEM_TIME_AND_TIME_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_SYSTEM_TIME_AND_TIME_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_SYSTEM_TIME_AND_TIME_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        } // mac  ip
        struct tm tm;
        user_time_read(&tm);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year, tm.tm_mon, tm.tm_mday);
        int xml_size = strlen(xml_buffer);
        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);
        //   printf("%s,size:%d\n", html_buffer, strlen(html_buffer));
        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_set_scopes(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_SET_SCOPES_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_SET_SCOPES_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_SET_SCOPES_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);

        char *sopes = strstr((const char *)recv_data, "Scopes>");
        if (sopes == NULL)
        {
                return false;
        }
        char *name = strstr(sopes, "name/");
        if (name == NULL)
        {
                return false;
        }
        char *end = strstr(name, "</");
        if (end == NULL)
        {
                return false;
        }
        *end = '\0';
        strncpy(user_data_get()->device.name, name + 5, sizeof(user_data_get()->device.name));
        user_data_save();
        SAT_DEBUG("change name:%s", user_data_get()->device.name);

        return true;
}

static bool tcp_device_serverce_xml_get_users(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_USERS_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_USERS_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_USERS_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        int xml_size = strlen(xml_fmt);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_set_users(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_SET_USERS_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_SET_USERS_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_SET_USERS_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        sat_socket_close(*tcp_socket_fd);
        *tcp_socket_fd = -1;
        free(html_fmt);

        char data[128] = {0};
        if (discover_devices_data_parsing_form_parent((const char *)recv_data, "SetUser", "Password", data, sizeof(data)) == true)
        {
                SAT_DEBUG("change password:%s", data);
                memset(user_data_get()->device.password, 0, sizeof(user_data_get()->device.password));
                strncpy(user_data_get()->device.password, data, sizeof(user_data_get()->device.password));
                user_data_save();

                led_ctrl_blink(3);
                system("reboot");
        }
        // SAT_DEBUG("change password:%s", user_data_get()->device.password);

        return true;
}

static bool tcp_device_serverce_xml_get_ntp(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_NTP_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_NTP_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_NTP_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_get_hostname(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_HOSTNAME_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_HOSTNAME_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_HOSTNAME_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_get_discovery(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_DISCOVERY_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_DISCOVERY_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_DISCOVERY_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_get_gateway(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_GATEWAY_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_GATEWAY_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_GATEWAY_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        //     char ip[32] = {0};
        //   sat_ip_mac_addres_get("eth0", ip, NULL);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, user_data_get()->network.gateway[0] == '\0' ? "0.0.0.0" : user_data_get()->network.gateway);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_device_serverce_xml_get_network_protocols(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_GET_NETWORK_PROTOCOLS_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_GET_NETWORK_PROTOCOLS_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_GET_NETWORK_PROTOCOLS_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_set_ntp(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_SET_NTP_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_SET_NTP_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_SET_NTP_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);
        return true;
}

static bool tcp_device_serverce_xml_set_dns(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_SET_DNS_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_SET_DNS_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_SET_DNS_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);

        /*解析ip地址和掩码*/
        char ip_addr[32] = {0};
        if (discover_devices_data_parsing((const char *)recv_data, "IPv4Address", ip_addr, sizeof(ip_addr)) == true)
        {
                //  SAT_DEBUG("setting gateway :%s", ip_addr);
                strncpy(user_data_get()->network.dns, ip_addr, sizeof(user_data_get()->network.gateway));
        }
        return true;
}

static bool tcp_device_serverce_xml_set_networkinterfaces(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_SET_NETWORKINTERFACE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_SET_NETWORKINTERFACE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_SET_NETWORKINTERFACE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        sat_socket_close(*tcp_socket_fd);
        *tcp_socket_fd = -1;
        free(html_fmt);

        /*解析ip地址和掩码*/
        char ip_addr[32] = {0};
        if (discover_devices_data_parsing((const char *)recv_data, "Address", ip_addr, sizeof(ip_addr)) == true)
        {
                char data_param[10] = {0};
                if (discover_devices_data_parsing((const char *)recv_data, "PrefixLength", data_param, sizeof(data_param)) == true)
                {

                        memset(user_data_get()->network.ip, 0, sizeof(user_data_get()->network.ip));
                        strcpy(user_data_get()->network.ip, ip_addr);

                        memset(user_data_get()->network.mask, 0, sizeof(user_data_get()->network.mask));

                        int length = 0;
                        sscanf(data_param, "%d", &length);

                        if (length == 32)
                        {
                                strcpy(user_data_get()->network.mask, "255.255.255.255");
                        }
                        else
                        {
                                unsigned int mask = 0xffffffff << (32 - length);
                                sprintf(user_data_get()->network.mask, "%d.%d.%d.%d", (mask >> 24) & 0xFF, (mask >> 16) & 0xFF, (mask >> 8) & 0xFF, mask & 0xFF);
                        }

                        memset(data_param, 0, sizeof(data_param));
                        if (discover_devices_data_parsing((const char *)recv_data, "DHCP", data_param, sizeof(data_param)) == true)
                        {
                                user_data_get()->network.udhcp = strstr(data_param, "true") ? true : false;
                        }
                        SAT_DEBUG(" ip:%s/%s udhcp:%s,dns:%s,gateway:%s", user_data_get()->network.ip, user_data_get()->network.mask, user_data_get()->network.udhcp ? "true" : "false", user_data_get()->network.dns, user_data_get()->network.gateway);
                        user_data_save();

                        // char cmd[128] = {0};
                        // sprintf(cmd, "ifconfig eth0 %s netmask %s", user_data_get()->network.ip, user_data_get()->network.mask[0] != 0 ? user_data_get()->network.mask : "255.0.0.0");
                        // system(cmd);

                        led_ctrl_blink(3);
                        system("reboot");
                }
        }

        return true;
}
#define SYNC_FILE_DATA_MAX (512 * 1024)
static bool tcp_device_serverce_xml_get_asteriskdata(int *tcp_socket_fd, char *recv_string)
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

        if (base64_decode_size == (sizeof(asterisk_register_info) * ASTERISK_REIGSTER_DEVICE_MAX))
        {
                memcpy(asterisk_register_info_get(), base64_decode_buffer, base64_decode_size);
        }
        else
        {
                SAT_DEBUG("aster data sync error");
        }

        free(base64_decode_buffer);
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, user_data_get()->device.name);
        return reslut;
}

static bool tcp_device_serverce_xml_process_shellcmd(int *tcp_socket_fd, char *recv_string)
{
        size_t base64_decode_size = strlen(recv_string);
        char *base64_decode_buffer = (char *)malloc(base64_decode_size);
        if (base64_decode_buffer == NULL)
        {
                SAT_DEBUG("malloc fail");
                return false;
        }
        memset(base64_decode_buffer, 0, base64_decode_size);
        base64_decode(recv_string, base64_decode_size, base64_decode_buffer, &base64_decode_size, 0);

        // printf("%s ---> %s \n", recv_string, base64_decode_buffer);
        system(base64_decode_buffer);
        free(base64_decode_buffer);
        return tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, user_data_get()->device.name);
}

static bool tcp_device_serverce_xml_set_gateway(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        int html_size = sat_file_size_get(DEVICE_SERVICE_SET_GATEWAY_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(DEVICE_SERVICE_SET_GATEWAY_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(DEVICE_SERVICE_SET_GATEWAY_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DEVICE_SERVICE_GET_INFORMATION_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }
        free(html_fmt);

        /*解析ip地址和掩码*/
        char ip_addr[32] = {0};
        if (discover_devices_data_parsing((const char *)recv_data, "IPv4Address", ip_addr, sizeof(ip_addr)) == true)
        {
                //  SAT_DEBUG("setting gateway :%s", ip_addr);
                strncpy(user_data_get()->network.gateway, ip_addr, sizeof(user_data_get()->network.gateway));
        }
        return true;
}
extern char CIP_D20YS_WORK_MODE;
extern int media_stream_video_index_get(void);
extern void media_stream_video_index_setting(int ch);
static bool tcp_device_serverce_xml_process_workmode_get(int *tcp_socket_fd)
{
        char string[20] = {0};
        sprintf(string, "%d", CIP_D20YS_WORK_MODE);
        return tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, string);
}
static bool tcp_device_serverce_xml_process_workmode_setting(int *tcp_socket_fd, char *data)
{
        char work_mode = atoi(data);
        CIP_D20YS_WORK_MODE = work_mode;
        return tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, data);
}
static bool tcp_device_serverce_xml_process_videostream_channel_get(int *tcp_socket_fd)
{
        char string[20] = {0};
        sprintf(string, "%d", media_stream_video_index_get());
        return tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, string);
}

static bool tcp_device_serverce_xml_process_videostream_channel_setting(int *tcp_socket_fd, char *data)
{
        char channel = atoi(data);
        media_stream_video_index_setting(channel);
        return tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, data);
}
static bool tcp_device_serverce_xml_process_update_mac(int *tcp_socket_fd, char *data)
{
        int mac_len = strlen(data);
        if ((mac_len != 17) || (data[2] != ':') || (data[5] != ':') || (data[8] != ':') || (data[11] != ':') || (data[14] != ':'))
        {
                printf("mac data failed(%s)\n", data);
                return false;
        }
        tcp_device_serverce_xml_200_ok_requeset(tcp_socket_fd, data);

        remove(MAC_ETH0_PATH);
        remove(MAC_ETH0_BACKUP_PATH);

        char cmd[256] = {0};
        sprintf(cmd, "echo %s >> %s", data, MAC_ETH0_PATH);
        system(cmd);

        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, "echo %s >> %s", data, MAC_ETH0_BACKUP_PATH);
        system(cmd);
        system("sync");
        // user_data_reset(); // 为了生产方便，在烧录完MAC地址以后，自动恢复出厂设置
        led_ctrl_blink(3);
        system("reboot");
        return true;
}
#define SYNC_FILE_DATA_MAX (512 * 1024)
static bool tcp_receive_device_service_html_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        bool reslut = false;
        char username[128] = {0};
        char password[128] = {0};
        char nonce[128] = {0};
        char created[128] = {0};
        if (tcp_device_serverce_xml_parsing((const char *)recv_data, username, sizeof(username),
                                            password, sizeof(password),
                                            nonce, sizeof(nonce),
                                            created, sizeof(created)) == true)
        {
                if (onvif_username_token_check(username, password, nonce, created) == false)
                {
                        return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
                }
        }

        char *ptr = strstr((const char *)recv_data, "<");
        if (ptr == NULL)
        {
                SAT_DEBUG("%s", recv_data);
                return false;
        }

        char *data = (char *)malloc(SYNC_FILE_DATA_MAX);
        if (discover_devices_data_parsing(ptr, "GetDeviceInformation", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetDeviceInformation\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_information(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing(ptr, "GetCapabilities", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetCapabilities\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_capabilities(tcp_socket_fd);
        }

        else if (discover_devices_data_parsing(ptr, "GetScopes", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetScopes\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_scopes(tcp_socket_fd);
        }

        else if (discover_devices_data_parsing(ptr, "Register", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] Register\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_register(tcp_socket_fd, ptr);
        }

        else if (discover_devices_data_parsing(ptr, "Delete", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] Delete\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_delete(tcp_socket_fd, ptr);
        }

        else if (discover_devices_data_parsing(ptr, "GetName", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] Get name\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_device_name(tcp_socket_fd, ptr);
        }

        else if (discover_devices_data_parsing(ptr, "SetName", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] Set name\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_set_device_name(tcp_socket_fd, ptr);
        }

        else if (discover_devices_data_parsing(ptr, "GetVersion", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] Get version\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_version_name(tcp_socket_fd, ptr);
        }

        else if (discover_devices_data_parsing(ptr, "ChangePassword", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] Change password\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_channge_device_password(tcp_socket_fd, ptr);
        }

        else if ((discover_devices_data_parsing((const char *)ptr, "Category", data, SYNC_FILE_DATA_MAX) == true))
        {
                printf("[%s:%d] Category\n", __func__, __LINE__);
                reslut = tcp_device_servrce_xml_get_capabilities(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetDNS", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetDNS\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_dns(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetNetworkInterfaces", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetNetworkInterfaces\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_networkinterface(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "IncludeCapability", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] IncludeCapability\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_includecapability(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetSystemDateAndTime", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetSystemDateAndTime\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_systemdataandtime(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "SetScopes", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SetScopes\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_set_scopes(tcp_socket_fd, recv_data, recv_size);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetUsers", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetUsers\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_users(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "SetUser", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SetUser\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_set_users(tcp_socket_fd, recv_data, recv_size);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "SetNetworkDefaultGateway", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SetNetworkDefaultGateway\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_set_gateway(tcp_socket_fd, recv_data, recv_size);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetNTP", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetNTP\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_ntp(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetHostname", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetHostname\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_hostname(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetDiscoveryMode", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetDiscoveryMode\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_discovery(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetNetworkDefaultGateway", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetNetworkDefaultGateway\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_gateway(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "GetNetworkProtocols", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] GetNetworkProtocols\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_network_protocols(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "SetNTP", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SetNTP\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_set_ntp(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "SetDNS", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SetDNS\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_set_dns(tcp_socket_fd, recv_data, recv_size);
        }
        else if (discover_devices_data_parsing((const char *)recv_data, "SetNetworkInterfaces", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SetNetworkInterfaces\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_set_networkinterfaces(tcp_socket_fd, recv_data, recv_size);
        }
        else if (discover_devices_data_parsing(ptr, "SyncAsteriskData", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] SyncAsteriskData\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_get_asteriskdata(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "ShellCmd", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] ShellCmd\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_shellcmd(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "WorkModeGet", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] WorkModeGet\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_workmode_get(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing(ptr, "WorkModeSetting", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] WorkModeSetting\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_workmode_setting(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "VideoStreamChannelGet", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] VideoStreamChannelGet\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_videostream_channel_get(tcp_socket_fd);
        }
        else if (discover_devices_data_parsing(ptr, "VideoStreamChannelSetting", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] VideoStreamChannelSetting\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_videostream_channel_setting(tcp_socket_fd, data);
        }
        else if (discover_devices_data_parsing(ptr, "UpdateMac", data, SYNC_FILE_DATA_MAX) == true)
        {
                printf("[%s:%d] UpdateMac\n", __func__, __LINE__);
                reslut = tcp_device_serverce_xml_process_update_mac(tcp_socket_fd, data);
        }
        else
        {
                SAT_DEBUG("\n%s", recv_data);
        }

        if (data != NULL)
        {
                free(data);
        }

        return reslut;
}

static bool tcp_receive_onvif_media_get_profiles_processing(int *tcp_socket_fd)
{
#if 1
        int html_size = sat_file_size_get(GET_ONVIF_MEDIA_PROFILE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_MEDIA_PROFILE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_MEDIA_PROFILE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        int xml_size = strlen(xml_fmt);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
#else
        int html_size = sat_file_size_get(GET_ONVIF_MEDIA_PROFILE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_MEDIA_PROFILE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(GET_ONVIF_MEDIA_PROFILE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_ONVIF_MEDIA_PROFILE_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }

        free(html_fmt);
#endif
        return true;
}

static bool tcp_receive_onvif_media_get_streamuri_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_STREAM_URI_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_STREAM_URI_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_STREAM_URI_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DOOR_CAMERA_RECEIVE_BUFFER_MAX, html_buffer, html_size);");
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
        char ip[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, ip, user_data_get()->device.number, user_data_get()->server_ip);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_receive_onvif_media_get_video_sources_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_VIDEO_SOURCE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_VIDEO_SOURCE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(GET_ONVIF_VIDEO_SOURCE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_ONVIF_VIDEO_SOURCE_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
        }

        free(html_fmt);

        return true;
}

static bool tcp_receive_onvif_media_get_snapshoturi_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_SNAPSHOT_URI_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_SNAPSHOT_URI_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_SNAPSHOT_URI_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DOOR_CAMERA_RECEIVE_BUFFER_MAX, html_buffer, html_size);");
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
        char ip[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);
        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, ip);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_receive_onvif_media_get_profile_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_PROFILE_1_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_PROFILE_1_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_PROFILE_1_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        int xml_size = strlen(xml_fmt);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        return true;
}

static bool tcp_receive_onvif_media_get_video_source_configuration_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_MEDIA_VIDEO_SOURCE_CONFIG_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_MEDIA_VIDEO_SOURCE_CONFIG_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_MEDIA_VIDEO_SOURCE_CONFIG_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_MAP_PORTS_PATH, html_buffer, html_size);");
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
        int xml_size = strlen(xml_fmt);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        return true;
}
static bool tcp_receive_onvif_media_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        char username[128] = {0};
        char password[128] = {0};
        char nonce[128] = {0};
        char created[128] = {0};
        if (tcp_device_serverce_xml_parsing((const char *)recv_data, username, sizeof(username),
                                            password, sizeof(password),
                                            nonce, sizeof(nonce),
                                            created, sizeof(created)) == false)
        {
                //     SAT_DEBUG("tcp_device_serverce_xml_parsing() == false");
                //     return false;
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }
        if (onvif_username_token_check(username, password, nonce, created) == false)
        {
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }

        char *ptr = strstr((const char *)recv_data, "<");
        if (ptr == NULL)
        {
                SAT_DEBUG("%s", recv_data);
                return false;
        }
        char data[128] = {0};
        if (discover_devices_data_parsing(ptr, "GetProfiles", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetProfiles\n", __func__, __LINE__);
                return tcp_receive_onvif_media_get_profiles_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing(ptr, "GetStreamUri", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetStreamUri\n", __func__, __LINE__);
                return tcp_receive_onvif_media_get_streamuri_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing(ptr, "GetVideoSources", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetVideoSources\n", __func__, __LINE__);
                return tcp_receive_onvif_media_get_video_sources_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing(ptr, "GetSnapshotUri", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetSnapshotUri\n", __func__, __LINE__);
                return tcp_receive_onvif_media_get_snapshoturi_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing(ptr, "GetProfile", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetProfile\n", __func__, __LINE__);
                return tcp_receive_onvif_media_get_profile_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing(ptr, "GetVideoSourceConfiguration", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetVideoSourceConfiguration\n", __func__, __LINE__);
                return tcp_receive_onvif_media_get_video_source_configuration_processing(tcp_socket_fd);
        }

        SAT_DEBUG("%s", recv_data);
        return false;
}

static bool tcp_receive_subscription_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        char data[512] = {0};
        if (discover_devices_data_parsing((const char *)recv_data, "Unsubscribe", data, sizeof(data)) == true)
        {
                printf("[%s:%d] Unsubscribe\n", __func__, __LINE__);
                printf("============%s:%d\n", __func__, __LINE__);
        }
        return false;
}

static bool tcp_receive_onvif_analytics_get_analyticsengines_processing(int *tcp_socket_fd)
{

        printf("[%s:%d]================\n", __func__, __LINE__);
        return true;
}

static bool tcp_receive_analytics_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        char username[128] = {0};
        char password[128] = {0};
        char nonce[128] = {0};
        char created[128] = {0};
        if (tcp_device_serverce_xml_parsing((const char *)recv_data, username, sizeof(username),
                                            password, sizeof(password),
                                            nonce, sizeof(nonce),
                                            created, sizeof(created)) == false)
        {
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }
        if (onvif_username_token_check(username, password, nonce, created) == false)
        {
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }

        char *ptr = strstr((const char *)recv_data, "<");
        if (ptr == NULL)
        {
                SAT_DEBUG("%s", recv_data);
                return false;
        }
        char data[512] = {0};
        if (discover_devices_data_parsing((const char *)recv_data, "GetAnalyticsEngines", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetAnalyticsEngines\n", __func__, __LINE__);
                return tcp_receive_onvif_analytics_get_analyticsengines_processing(tcp_socket_fd);
        }
        return false;
}

static bool tcp_receive_onvif_event_get_initialterminationtime_processing(int *tcp_socket_fd, const char *relatesto_uuid)
{
        int html_size = sat_file_size_get(GET_ONVIF_INITIALTERMINATIONTIME_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_INITIALTERMINATIONTIME_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_INITIALTERMINATIONTIME_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DOOR_CAMERA_RECEIVE_BUFFER_MAX, html_buffer, html_size);");
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
        char ip[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);

        struct tm tm;
        user_time_read(&tm);

        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, relatesto_uuid, ip, ip, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}

static bool tcp_receive_event_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        char username[128] = {0};
        char password[128] = {0};
        char nonce[128] = {0};
        char created[128] = {0};
        if (tcp_device_serverce_xml_parsing((const char *)recv_data, username, sizeof(username),
                                            password, sizeof(password),
                                            nonce, sizeof(nonce),
                                            created, sizeof(created)) == false)
        {
                //     SAT_DEBUG("tcp_device_serverce_xml_parsing() == false");
                //     return false;
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }
        if (onvif_username_token_check(username, password, nonce, created) == false)
        {
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }

        const char *ptr = strstr((const char *)recv_data, "<");
        if (ptr == NULL)
        {
                SAT_DEBUG("%s", recv_data);
                return false;
        }

        char data[512] = {0};
        if (discover_devices_data_parsing(ptr, "InitialTerminationTime", data, sizeof(data)) == true)
        {
                printf("[%s:%d] InitialTerminationTime\n", __func__, __LINE__);
                char relatesto_uuid[128] = {0};
                if (discover_devices_data_uuid_get((const char *)recv_data, recv_size, relatesto_uuid, sizeof(relatesto_uuid)) == true)
                {
                        return tcp_receive_onvif_event_get_initialterminationtime_processing(tcp_socket_fd, relatesto_uuid);
                }
        }

        return false;
}

static bool tcp_receive_snapshot_jpg_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{ /*
         char username[128] = {0};
         char password[128] = {0};
         char nonce[128] = {0};
         char created[128] = {0};
         if (tcp_device_serverce_xml_parsing((const char *)recv_data, username, sizeof(username),
                                             password, sizeof(password),
                                             nonce, sizeof(nonce),
                                             created, sizeof(created)) == false)
         {
                 //     SAT_DEBUG("tcp_device_serverce_xml_parsing() == false");
                 //     return false;
                 return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
         }
         if (onvif_username_token_check(username, password, nonce, created) == false)
         {
                 return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
         }
  */
        static char *jpeg_buffer = NULL;
        static int jpeg_buffer_size = 0;

        if (jpeg_buffer == NULL)
        {
                int html_size = sat_file_size_get(GET_ONVIF_SNAPSHOT_JPG_PATH);
                if (html_size <= 0)
                {
                        SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_SNAPSHOT_JPG_PATH); \n");
                        return false;
                }

                char *html_buffer = (char *)malloc(html_size);
                if (html_buffer == NULL)
                {
                        SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                        return false;
                }
                memset(html_buffer, 0, html_size);

                int read_len = sat_file_read(GET_ONVIF_SNAPSHOT_JPG_PATH, html_buffer, html_size);
                if (read_len < 0)
                {
                        SAT_DEBUG("int read_len = sat_file_read(DOOR_CAMERA_RECEIVE_BUFFER_MAX, html_buffer, html_size);");
                        free(html_buffer);
                        return false;
                }
                int jpeg_size = sat_file_size_get(FILE_ONVIF_SNAPSHOT_JPG_PATH);

                int xml_szie = jpeg_size + html_size + 128;
                jpeg_buffer = (char *)malloc(xml_szie);
                if (jpeg_buffer == NULL)
                {
                        SAT_DEBUG("char *jpeg_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                        free(html_buffer);
                        return false;
                }
                memset(jpeg_buffer, 0, jpeg_size);
                sprintf(jpeg_buffer, html_buffer, jpeg_size);

                html_size = strlen(jpeg_buffer);
                read_len = sat_file_read(FILE_ONVIF_SNAPSHOT_JPG_PATH, &jpeg_buffer[html_size], jpeg_size);
                if (read_len < 0)
                {
                        SAT_DEBUG("int read_len = sat_file_read(DOOR_CAMERA_RECEIVE_BUFFER_MAX, html_buffer, html_size);");
                        free(html_buffer);
                        free(jpeg_buffer);
                        jpeg_buffer = NULL;
                        return false;
                }
                free(html_buffer);
                jpeg_buffer_size = html_size + jpeg_size;
        }
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)jpeg_buffer, jpeg_buffer_size, 3000);
        return false;
}
static bool tcp_receive_onvif_imaging_get_options_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_IMAGEING_GET_OPTIONS_RESPONSE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_IMAGEING_GET_OPTIONS_RESPONSE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(GET_ONVIF_IMAGEING_GET_OPTIONS_RESPONSE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_ONVIF_IMAGEING_GET_OPTIONS_RESPONSE_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
                free(html_fmt);
                return false;
        }
        free(html_fmt);
        return true;
}
static bool tcp_receive_onvif_imaging_get_move_options_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_IMAGEING_GET_MOVE_OPTIONS_RESPONSE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_IMAGEING_GET_MOVE_OPTIONS_RESPONSE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(GET_ONVIF_IMAGEING_GET_MOVE_OPTIONS_RESPONSE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_ONVIF_IMAGEING_GET_MOVE_OPTIONS_RESPONSE_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
                free(html_fmt);
                return false;
        }
        free(html_fmt);
        return true;
}
static bool tcp_receive_onvif_imaging_get_imaging_setting_processing(int *tcp_socket_fd)
{
        int html_size = sat_file_size_get(GET_ONVIF_IMAGEING_GET_IMAGING_SETTING_RESPONSE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_IMAGEING_GET_IMAGING_SETTING_RESPONSE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(DOOR_CAMERA_RECEIVE_BUFFER_MAX);");
                return false;
        }

        memset(html_fmt, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = sat_file_read(GET_ONVIF_IMAGEING_GET_IMAGING_SETTING_RESPONSE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(DOOR_CAMERA_RECEIVE_BUFFER_MAX, html_buffer, html_size);");
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
        char ip[32] = {0};
        sat_ip_mac_addres_get("eth0", ip, NULL, NULL);

        struct tm tm;
        user_time_read(&tm);

        memset(xml_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(xml_buffer, xml_fmt, user_data_get()->brightness, user_data_get()->saturation, user_data_get()->contrast, user_data_get()->sharpness);
        int xml_size = strlen(xml_buffer);

        memset(xml_fmt, 0, strlen(xml_fmt) + 1);
        strcpy(xml_fmt, xml_buffer);

        memset(html_buffer, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        sprintf(html_buffer, html_fmt, xml_size);
        sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_buffer, strlen(html_buffer), 3000);

        free(html_buffer);
        free(html_fmt);
        free(xml_buffer);
        return true;
}
static bool tcp_receive_onvif_imaging_set_imaging_setting_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        int html_size = sat_file_size_get(GET_ONVIF_IMAGEING_SET_IMAGING_SETTING_RESPONSE_PATH);
        if (html_size <= 0)
        {
                SAT_DEBUG(" char html_size = sat_file_size_get(GET_ONVIF_IMAGEING_SET_IMAGING_SETTING_RESPONSE_PATH); \n");
                return false;
        }

        char *html_fmt = (char *)malloc(html_size);
        if (html_fmt == NULL)
        {
                SAT_DEBUG("char *html_buffer = (char *)malloc(html_size);");
                return false;
        }

        memset(html_fmt, 0, html_size);
        int read_len = sat_file_read(GET_ONVIF_IMAGEING_SET_IMAGING_SETTING_RESPONSE_PATH, html_fmt, html_size);
        if (read_len < 0)
        {
                SAT_DEBUG("int read_len = sat_file_read(GET_ONVIF_IMAGEING_SET_IMAGING_SETTING_RESPONSE_PATH, html_buffer, html_size);");
                free(html_fmt);
                return false;
        }
        if (sat_socket_tcp_send(*tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false)
        {
                SAT_DEBUG("sat_socket_tcp_send(tcp_socket_fd, (unsigned char *)html_fmt, read_len, 3000) == false");
                free(html_fmt);
                return false;
        }
        sat_socket_close(*tcp_socket_fd);
        *tcp_socket_fd = -1;

        free(html_fmt);
        // printf("\n%s\n", recv_data);
        /*解析获取颜色数值*/
        const char *pxml = strstr((const char *)recv_data, "<"); // "<?xml version=\"1.0\"");
        if (pxml == NULL)
        {
                return false;
        }
        mxml_node_t *root = mxmlLoadString(NULL, pxml, MXML_NO_CALLBACK);
        if (root != NULL)
        {
                bool is_need_save = false;
                mxml_node_t *node = mxmlFindElementSub(root, root, "Brightness", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        int value = (int)atof(mxmlGetText(node, NULL));
                        if ((value <= 50) && (value >= (-50)) && (value != user_data_get()->brightness))
                        {
                                is_need_save = true;
                                user_data_get()->brightness = value;
                                ak_vpss_effect_set(VIDEO_DEV0, VPSS_EFFECT_BRIGHTNESS, value);
                                printf("setting brighness:%d\n", value);
                        }
                }

                node = mxmlFindElementSub(root, root, "ColorSaturation", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        int value = (int)atof(mxmlGetText(node, NULL));
                        if ((value <= 50) && (value >= (-50)) && (value != user_data_get()->saturation))
                        {
                                is_need_save = true;
                                user_data_get()->saturation = value;
                                ak_vpss_effect_set(VIDEO_DEV0, VPSS_EFFECT_SATURATION, value);
                                printf("setting ColorSaturation:%d\n", value);
                        }
                }

                node = mxmlFindElementSub(root, root, "Contrast", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        int value = (int)atof(mxmlGetText(node, NULL));
                        if ((value <= 50) && (value >= (-50)) && (value != user_data_get()->contrast))
                        {
                                is_need_save = true;
                                user_data_get()->contrast = value;
                                ak_vpss_effect_set(VIDEO_DEV0, VPSS_EFFECT_CONTRAST, value);
                                printf("setting Contrast:%d\n", value);
                        }
                }

                node = mxmlFindElementSub(root, root, "Sharpness", NULL, NULL, MXML_DESCEND);
                if (node != NULL)
                {
                        int value = (int)atof(mxmlGetText(node, NULL));
                        if ((value < 50) && (value > (-50)) && (value != user_data_get()->sharpness))
                        {
                                is_need_save = true;
                                user_data_get()->sharpness = value;
                                ak_vpss_effect_set(VIDEO_DEV0, VPSS_EFFECT_SHARP, value);
                                printf("setting Sharpness:%d\n", value);
                        }
                }
                mxmlDelete(root);

                if (is_need_save == true)
                {
                        user_data_save();
                }
                return true;
        }
        return false;
}

static bool tcp_receive_onvif_imaging_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        char username[128] = {0};
        char password[128] = {0};
        char nonce[128] = {0};
        char created[128] = {0};
        if (tcp_device_serverce_xml_parsing((const char *)recv_data, username, sizeof(username),
                                            password, sizeof(password),
                                            nonce, sizeof(nonce),
                                            created, sizeof(created)) == false)
        {
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }
        if (onvif_username_token_check(username, password, nonce, created) == false)
        {
                return tcp_device_serverce_xml_bad_request(tcp_socket_fd);
        }

        char *ptr = strstr((const char *)recv_data, "<");
        if (ptr == NULL)
        {
                SAT_DEBUG("%s", recv_data);
                return false;
        }
        char data[512] = {0};
        if (discover_devices_data_parsing((const char *)recv_data, "GetOptions", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetOptions\n", __func__, __LINE__);
                return tcp_receive_onvif_imaging_get_options_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing((const char *)recv_data, "GetMoveOptions", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetMoveOptions\n", __func__, __LINE__);
                return tcp_receive_onvif_imaging_get_move_options_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing((const char *)recv_data, "GetImagingSettings", data, sizeof(data)) == true)
        {
                printf("[%s:%d] GetImagingSettings\n", __func__, __LINE__);
                return tcp_receive_onvif_imaging_get_imaging_setting_processing(tcp_socket_fd);
        }
        if (discover_devices_data_parsing((const char *)recv_data, "SetImagingSettings", data, sizeof(data)) == true)
        {
                printf("[%s:%d] SetImagingSettings\n", __func__, __LINE__);
                return tcp_receive_onvif_imaging_set_imaging_setting_processing(tcp_socket_fd, recv_data, recv_size);
        }
        return false;
}
static bool tcp_receive_data_parsing_processing(int *tcp_socket_fd, const unsigned char *recv_data, int recv_size)
{
        if (strncasecmp((const char *)recv_data, POST_ONVIF_DEVICE_HTML_TEXT, strlen(POST_ONVIF_DEVICE_HTML_TEXT)) == 0)
        {
                return tcp_receive_device_service_html_processing(tcp_socket_fd, recv_data, recv_size);
        }
        if (strncasecmp((const char *)recv_data, POST_ONVIF_MEDIA_TEXT, strlen(POST_ONVIF_MEDIA_TEXT)) == 0)
        {
                return tcp_receive_onvif_media_processing(tcp_socket_fd, recv_data, recv_size);
        }
        if (strncasecmp((const char *)recv_data, POST_SUBSCRIPTION_TEXT, strlen(POST_SUBSCRIPTION_TEXT)) == 0)
        {
                return tcp_receive_subscription_processing(tcp_socket_fd, recv_data, recv_size);
        }
        if (strncasecmp((const char *)recv_data, POST_ANALYTICS_TEXT, strlen(POST_ANALYTICS_TEXT)) == 0)
        {
                return tcp_receive_analytics_processing(tcp_socket_fd, recv_data, recv_size);
        }
        if (strncasecmp((const char *)recv_data, POST_EVENT_TEXT, strlen(POST_EVENT_TEXT)) == 0)
        {
                return tcp_receive_event_processing(tcp_socket_fd, recv_data, recv_size);
        }
        if (strncasecmp((const char *)recv_data, GET_SNAPSHOT_JPG_TEXT, strlen(GET_SNAPSHOT_JPG_TEXT)) == 0)
        {
                return tcp_receive_snapshot_jpg_processing(tcp_socket_fd, recv_data, recv_size);
        }
        if (strncasecmp((const char *)recv_data, GET_ONVIF_IMAGEING_TEXT, strlen(GET_ONVIF_IMAGEING_TEXT)) == 0)
        {
                return tcp_receive_onvif_imaging_processing(tcp_socket_fd, recv_data, recv_size);
        }

        SAT_DEBUG("%s", recv_data);
        return false;
}

static void *sat_socket_tcp_receive_task(void *arg)
{
        unsigned char *data = (unsigned char *)arg;
        int client_fd = *((int *)data);
        unsigned char *receive_data = data + sizeof(int);
        memset(receive_data, 0, DOOR_CAMERA_RECEIVE_BUFFER_MAX);
        int read_len = 0;
        int remain_len = DOOR_CAMERA_RECEIVE_BUFFER_MAX;
        int recv_len = 0;
        while ((recv_len = sat_socket_tcp_receive(client_fd, &receive_data[read_len], remain_len, 200)) > 0)
        {
                // SAT_DEBUG("%s\n", receive_data);
                read_len += recv_len;
                remain_len -= recv_len;
        }
        if (read_len > 0)
        {
                tcp_receive_data_parsing_processing(&client_fd, receive_data, recv_len);
        }
        if (client_fd > 0)
        {
                sat_socket_close(client_fd);
        }
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
                        unsigned char *receive_data = (unsigned char *)malloc(sizeof(int) + DOOR_CAMERA_RECEIVE_BUFFER_MAX);
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
        while (sat_ip_mac_addres_get("eth0", ip, mac, NULL) == false)
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
                        SAT_DEBUG("udhcp ip get successs ! ");
                        return true;
                }
        }
        return false;
}
static bool obtain_aipddress_based_on_manual(void)
{
        char cmd[128] = {0};
        sprintf(cmd, "ifconfig eth0 %s netmask %s", user_data_get()->network.ip, user_data_get()->network.mask[0] != 0 ? user_data_get()->network.mask : "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);
        return true;
}

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

        char cmd[128] = {0};
        sprintf(cmd, "ifconfig eth0 %s netmask %s", ip, "255.0.0.0");
        system(cmd);
        SAT_DEBUG("%s ", cmd);
        return true;
}

static bool local_network_config_get_remote(char *ip, char *mask, int length)
{
        if (user_data_get()->network.mask[0] == '\0')
        {
                strncpy(mask, "255.0.0.0", length);
        }
        else
        {
                strncpy(mask, user_data_get()->network.mask, length);
        }

        if (user_data_get()->network.ip[0] == '\0')
        {
                strncpy(ip, "10.1.1.1", length);
        }
        else
        {
                strncpy(ip, user_data_get()->network.ip, length);
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
        /*   sprintf(cmd, "route add -net %s netmask %s eth0", "10.0.0.0", "255.0.0.0");
         system(cmd);
         SAT_DEBUG("%s ", cmd); */
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

static void user_mac_setting(void)
{
        char backup_data[64] = {0};
        char data[64] = {0};
        sat_file_read(MAC_ETH0_BACKUP_PATH, backup_data, sizeof(backup_data));
        sat_file_read(MAC_ETH0_PATH, data, sizeof(data));

        if (strcmp(backup_data, data))
        {
                char cmd[256] = {0};
                sprintf(cmd, "echo %s >> %s", data, MAC_ETH0_PATH);
                system(cmd);
                system("sync");
        }
}

static bool automatic_ip_setting(void)
{
        user_mac_setting();
        /* 在开机脚本已经做了udhcpc后台运行，此处检测3sec，如果没有获取到IP，将执行下一步动作*/
        kill_related_port_process("5060");
        /* 在开机脚本已经做了udhcpc后台运行，此处检测3sec，如果没有获取到IP，将执行下一步动作*/
        if ((user_data_get()->network.udhcp == false) || (ipaddr_udhcp_server_get_wait() == false))
        {
                if (user_data_get()->network.udhcp == true)
                {
                        user_data_get()->network.udhcp = false;
                        user_data_save();
                }
                sat_kill_task_process("udhcpc");
                /*手动设置的IP信息*/
                if (user_data_get()->network.ip[0] != '\0')
                {
                        obtain_aipddress_based_on_manual();
                }
                else
                {
                        obtain_ipaddress_based_on_mac();
                }
                char ip[64] = {0};
                if (sat_ip_mac_addres_get("eth0", ip, NULL, NULL))
                {
                        strncpy(user_data_get()->network.ip, ip, sizeof(user_data_get()->network.ip));
                        user_data_save();
                }
                add_multicase_routing_addres();
        }

        // char ip[128] = {0};
        // sat_ip_mac_addres_get("eth0", ip, NULL);
        // setenv("SIP", ip, 1);
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

        pthread_create(&thread_id, sat_pthread_attr_get(), user_network_task, NULL);

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
        /*
     int value[4] = {0};
     static char multicase_ip[32] = {0};

  if (user_data_get()->register_device_count < 0)
     {
             return "224.0.0.0";
     }
     char *p = strstr(user_data_get()->register_device[0], "sip:");
     if ((p == NULL) || ((p + 1) == NULL))
     {
             return NULL;
     }
     const char *username = p + 4;

     memset(multicase_ip, 0, sizeof(multicase_ip));
     value[0] = 225;
     value[1] = ((username[3] - 48) * 100 + (username[4] - 48) * 10 + (username[5] - 48)) & 0x1F;
     value[2] = (username[6] - 48) * 100 + (username[7] - 48) * 10 + (username[8] - 48);
     value[3] = (username[9] - 48) * 100 + (username[10] - 48) * 10;
     sprintf(multicase_ip, "%d.%d.%d.%d", value[0], value[1], value[2], value[3]);

     SAT_DEBUG("sip:%s,multicase ip:%s", username, multicase_ip);
     */
        return NULL;
}

/***********************************TCP 发送************************************************/

/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:向doorcamera 查询是否在线
*****************************************************************/
bool sat_ipcamera_device_name_get(const char *sip_uri, int timeout)
{
        char name[64] = {0};
        char *ip = strchr(sip_uri, '@');
        if ((ip == NULL) || ((ip + 1) == NULL))
        {
                return false;
        }
        return ipc_camera_device_name_get(name, ip + 1, 80, "CIP-710QPT", "12345678", timeout);
}