#ifndef _LAYOUT_INTERCOM_CALL_H_
#define _LAYOUT_INTERCOM_CALL_H_

#include "common/language.h"

/*0:空闲，1：call outgoing 2:incomming 3:talk*/
void intercom_call_status_setting(int state);
bool intercom_call_username_setting(const char *user);
bool layout_intercom_talk_current_call_end_log(void);
#endif