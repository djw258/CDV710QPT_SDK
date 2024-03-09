#ifndef _LAYOUT_MONITOR_H_
#define _LAYOUT_MONITOR_H_
#include <stdbool.h>
bool monitor_doorcamera_call_extern_func(char *arg);
bool monitor_doorcamera_call_inside_func(char *arg);
bool monitor_other_call_busy_inside_func(char *arg);
bool monitor_other_call_busy_extern_func(char *arg);
void layout_call_log_create(CALL_LOG_TYPE type, int call_duration, int ch);
void layout_linphone_current_call_id_set(int id);
int layout_linphone_current_call_id_get(void);
void layout_monitor_goto_layout_process(bool active);
bool monitor_talk_call_end_callback(char *arg);
bool monitor_talk_call_failed_callback(char *arg);
bool layout_monitor_outgoing_arly_media_register(char *arg);
#endif
