#ifndef _ONVIF_USER_TOKEN_H_
#define _ONVIF_USER_TOKEN_H_
#include <stdbool.h>
#include <stdlib.h>

/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能:获取密码令牌
*****************************************************************/
bool onvif_usertoken_crate(const unsigned char *base64dec_romdom, size_t base64dec_romdom_len,
                           const unsigned char *time, size_t time_len,
                           const unsigned char *password, size_t pssword_len,
                           char *token, size_t *token_len);
/****************************************************************
 **@日期: 2022-09-20
 **@作者: leo.liu
 **@功能:格式化rtspxml
 *****************************************************************/
size_t onvif_stream_url_xml_format(char *xml, const char *ip, const char *user, const char *password, const char *profile_token);
/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:profile token
*****************************************************************/
size_t onvif_user_token_xml_format(char *file, const char *ip, const char *user, const char *password);
/****************************************************************
**@日期: 2022-09-20
**@作者: leo.liu
**@功能: 编辑类型： register/delete
*****************************************************************/
size_t onvif_edit_url_xml_format(char *file, const char *ip, const char *user, const char *password, const char *loc_sip_uri, const char *type);
#endif