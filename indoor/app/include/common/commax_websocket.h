#ifndef _COMMAX_WEBSOKET_H_
#define _COMMAX_WEBSOKET_H_
/*ip传输*/
const char *commax_transport_ip_get(void);
/*commax incoming receive process */
bool commax_sip_incoming_receive_process(const char *ip, int timeout);
/*asterisk init*/
bool commax_sip_asterisk_server_init(const char *from_sip_conf, const char *to_sip_conf, const char *from_extensions_conf, const char *to_extensions_conf, const char *commax_server_ip, const char *asterisk_register_usernmae);
/*修改服务器lobbyphone的密码*/
bool commax_https_server_lobbyphone_change_password(const char *server_ip, const char *port, char *lo_dong, char *lo_ho, char *pwd, char *reslut_msg, int msg_max, int timeout);
/*大厅机开锁*/
bool commax_https_lobbyphone_open_the_door(const char *ip, const char *port, int timeout);
/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期：2023-12-27 09:40:40
** 参数说明: event;1代表境安全紧、,2,燃气紧急,等等（暂时只需要上报安全紧急);event_state:1紧急发生、2紧急取消
** 注意事项：
************************************************************/
bool commax_emergency_event_report(const char *ip, int port, char *lo_dong, char *lo_ho, struct tm *tm, int event, int event_state, int timeout);
/************************************************************
** 函数说明: 上传pis信息
** 作者: xiaoxiao
** 日期：2024-01-09 08:18:32
** 参数说明:
** 注意事项：
************************************************************/
bool commax_pis_information_report(const char *ip, int port, char *lo_dong, char *lo_ho, char *prd, int type, char *ver, int timeout);
#endif