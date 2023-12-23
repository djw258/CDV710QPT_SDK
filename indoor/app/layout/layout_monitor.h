#ifndef _LAYOUT_MONITOR_H_
#define _LAYOUT_MONITOR_H_
#include <stdbool.h>
bool monitor_doorcamera_call_extern_func(char *arg);
bool monitor_doorcamera_call_inside_func(char *arg);
bool monitor_other_call_busy_inside_func(char *arg);
bool monitor_other_call_busy_extern_func(char *arg);
void layout_call_log_create(CALL_LOG_TYPE type, int call_duration, int ch);
#endif
