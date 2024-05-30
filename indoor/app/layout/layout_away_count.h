#ifndef LAYOUT_AWAY_COUNT
#define LAYOUT_AWAY_COUNT
#include "../share/lvgl-8.3.1/lvgl.h"
typedef struct
{
  lv_timer_t *away_setting_time_countdown_timer;
  int away_count_sec; // 离家设防生效缓冲时间（在缓冲时间过后，离家设防开始）
} layout_away_count_info;

layout_away_count_info *layout_away_count_data_get(void);

#endif