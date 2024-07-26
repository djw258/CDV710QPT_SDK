#ifndef __AK_VPSS_H__
#define __AK_VPSS_H__

#define VPSS_MD_DIMENSION_H_MAX 32
#define VPSS_MD_DIMENSION_V_MAX 24

#define VPSS_OD_AF_STATICS_MAX 5
#define VPSS_OD_RGB_HIST_MAX 256

#define VPSS_MASK_AREA_MAX 4

#define NIGHT_ARRAY_NUM 5
#define DAY_ARRAY_NUM 10

enum mode_state

{
  STATE_DAY,
  STATE_NIGHT,

};

enum vpss_mask_color_type

{
  VPSS_MASK_ORIGINAL_COLOR = 0, /* masked with original color*/
  VPSS_MASK_MOSAIC_VIDEO,       /* masked with mosaic video data*/
  VPSS_MASK_MOSAIC_WHITE_VIDEO, /* masked with mosaic & white 50% video data*/
  VPSS_MASK_MOSAIC_BLACK_VIDEO, /* masked with mosaic & black 50% video data*/

};

enum vpss_effect_type

{
  /* HUE to SHARP, value: [-50, 50], 0 means use the value in ISP config file */
  VPSS_EFFECT_HUE = 0x00,
  VPSS_EFFECT_BRIGHTNESS,
  VPSS_EFFECT_SATURATION,
  VPSS_EFFECT_CONTRAST,
  VPSS_EFFECT_SHARP,
  VPSS_EFFECT_WDR,

  VPSS_STYLE_ID,   //[0, 2]
  VPSS_POWER_HZ,   // 50 or 60
  VPSS_FLIP_MIRROR //[0, 3]

};

enum vpss_gain_stat

{
  VPSS_GAIN_NO_CHANGE_STAT = 0,
  VPSS_GAIN_LOW_STAT,
  VPSS_GAIN_HIGH_STAT,
  VPSS_GAIN_MID_STAT,

};

struct vpss_md_info

{
  unsigned short stat[VPSS_MD_DIMENSION_V_MAX][VPSS_MD_DIMENSION_H_MAX];
};

struct vpss_od_info

{
  unsigned int af_statics[VPSS_OD_AF_STATICS_MAX];
  unsigned int rgb_total;
  unsigned int rgb_hist[VPSS_OD_RGB_HIST_MAX];
};

struct vpss_af_stat_info

{
  unsigned int af_statics[5];
  // �~���~�g
};

struct vpss_af_attr

{
  unsigned short af_win0_left;
  //[0, 4095]
  unsigned short af_win0_right;
  //[0, 4095]
  unsigned short af_win0_top;
  //[0, 4095]
  unsigned short af_win0_bottom;
  //[0, 4095]

  unsigned short af_win1_left;
  //[0, 4095]
  unsigned short af_win1_right;
  //[0, 4095]
  unsigned short af_win1_top;    //[[0, 4095]
  unsigned short af_win1_bottom; //[0, 4095]

  unsigned short af_win2_left;   //[0, 4095]
  unsigned short af_win2_right;  //[0, 4095]
  unsigned short af_win2_top;    //[0, 4095]
  unsigned short af_win2_bottom; //[0, 4095]

  unsigned short af_win3_left;   //[0, 4095]
  unsigned short af_win3_right;  //[0, 4095]
  unsigned short af_win3_top;    //[0, 4095]
  unsigned short af_win3_bottom; //[0, 4095]

  unsigned short af_win4_left;   //[0, 4095]
  unsigned short af_win4_right;  //[0, 4095]
  unsigned short af_win4_top;    //[0, 4095]
  unsigned short af_win4_bottom; //[0, 4095]

  unsigned short af_th; //[0, 128],     af_th����������8��h���!��8��h���sharp#��58�����
};

struct vpss_mask_color_info

{
  /* defined by enum vpss_mask_color_type*/
  unsigned char color_type;

  /* range [0, 0xff]. if 0 mask video opaquely, then can not see image */
  unsigned char mk_alpha;

  /* range [0, 0xff] */
  unsigned char r_mk_color;
  /* range [0, 0xff] */
  unsigned char g_mk_color;
  /* range [0, 0xff] */
  unsigned char b_mk_color;
};

struct vpss_mask_area

{
  unsigned short start_xpos;
  unsigned short end_xpos;
  unsigned short start_ypos;
  unsigned short end_ypos;
  unsigned char enable;
};

struct vpss_mask_area_info

{
  struct vpss_mask_area main_mask[VPSS_MASK_AREA_MAX];
};

struct vpss_isp_ae_run_info

{
  unsigned char current_calc_avg_lumi;
  // ���<�
  unsigned char current_calc_avg_compensation_lumi;
  // ��9�,���

  unsigned char current_darked_flag;
  // �����1

  long current_a_gain;
  // #���
  long current_d_gain;
  // ���
  long current_isp_d_gain;
  // isp���
  long current_exp_time;
  // ��9������

  unsigned long current_a_gain_step;
  // �����#2��
  unsigned long current_d_gain_step;
  // 2��
  unsigned long current_isp_d_gain_step;
  // isp2��
  unsigned long current_exp_time_step;
  // ��9���2��
};

struct vpss_isp_ae_init_info

{
  long a_gain;
  // #���
  long d_gain;
  // ���
  long isp_d_gain;
  // isp���
  long exp_time;
  // ��9������
};

struct vpss_isp_ae_attr

{
  unsigned int exp_time_max;
  // ��9����
  unsigned int exp_time_min;
  // ��9�����!�
  unsigned int d_gain_max;
  // �
  unsigned int d_gain_min;
  // ��!�
  unsigned int isp_d_gain_min;
  // isp��!
  unsigned int isp_d_gain_max;
  // isp�
  unsigned int a_gain_max;
  // #�
  unsigned int a_gain_min;
  // #��!�
  unsigned int exp_step;
  // ���9
  unsigned int exp_stable_range;
  // ��6���
  unsigned int target_lumiance[10];
  // ?
  unsigned int envi_gain_range[10][2];
  unsigned int hist_weight[16];
  unsigned int BC_en;
  // ���,�
  unsigned int BC_detect_scope;
  // �|�-�
  unsigned int BC_rate_max;
  // �|
  unsigned int BC_rate_min;
  // �|!
};

struct vpss_isp_me_attr

{
  unsigned int exp_time;
  unsigned int a_gain;
  unsigned int d_gain;
  unsigned int isp_d_gain;
};

struct vpss_isp_awb_stat_info

{
  // ������s���������0���s���
  unsigned long total_R[10];
  // 10k��k�R�
  unsigned long total_G[10];
  // 10k��k�G�
  unsigned long total_B[10];
  // 10k��k�B�
  unsigned long total_cnt[10];
  // ������s������������

  // ��6������=0����
  unsigned short r_gain;
  // k�
  unsigned short g_gain;
  // k�
  unsigned short b_gain;
  // k�
  unsigned short r_offset;
  // k��
  unsigned short g_offset;
  // k��
  unsigned short b_offset;
  // k��
  unsigned short current_colortemp_index;
  // k����n���Nk�
  unsigned short colortemp_stable_cnt[10];
  // ��k��6���
};

struct ak_auto_day_night_threshold

{
  int day_to_night_lum;
  // day to night lum value
  int night_to_day_lum;
  // night to day lum value
  int night_cnt[NIGHT_ARRAY_NUM];
  // awb night cnt array
  int day_cnt[DAY_ARRAY_NUM];
  // awb day cnt array
  int lock_time;
  // locke night status time
  int quick_switch_mode;
  // quick switch mode
  int day2night_sleep_time;
  // day change to night sleep time
  int night2day_sleep_time;
  // night change to day sleep time
};

struct vpss_isp_wdr

{
  unsigned short hdr_uv_adjust_level;
  // uv�te z� ^, [0,31]
  unsigned short hdr_cnoise_suppress_slop;
  // �b6R�e�s
  unsigned short wdr_enable;

  unsigned short wdr_th1;
  // 0-1023
  unsigned short wdr_th2;
  // 0-1023
  unsigned short wdr_th3;
  // 0-1023
  unsigned short wdr_th4;
  // 0-1023
  unsigned short wdr_th5;
  // 0-1023

  // unsigned short wdr_light_weight;

  unsigned short area_tb1[65];
  // 10bit
  unsigned short area_tb2[65];
  // 10bit
  unsigned short area_tb3[65];
  // 10bit
  unsigned short area_tb4[65];
  // 10bit
  unsigned short area_tb5[65];
  // 10bit
  unsigned short area_tb6[65];
  // 10bit

  unsigned short area1_key[16];
  unsigned short area2_key[16];
  unsigned short area3_key[16];
  unsigned short area4_key[16];
  unsigned short area5_key[16];
  unsigned short area6_key[16];

  unsigned short hdr_uv_adjust_enable;
  // uv�teO��
  unsigned short hdr_cnoise_suppress_yth1;
  // r�i_jV�X�N� ^�< P1
  unsigned short hdr_cnoise_suppress_yth2;
  // r�i_jV�X�N� ^�< P2
  unsigned short hdr_cnoise_suppress_gain; // r��]�b6R
};

struct vpss_wdr_attr

{
  unsigned short wdr_mode;
  // !j_	��b�Kb�Rb�T��R

  struct vpss_isp_wdr manual_wdr;
  struct vpss_isp_wdr linkage_wdr[9];
};

struct vpss_isp_weight_attr

{
  unsigned short zone_weight[8][16];
};

struct vpss_isp_wb_type_attr

{
  unsigned short wb_type;
};

struct vpss_isp_3d_nr

{
  unsigned short tnr_enable;
  // default:1
  unsigned short updata_ref;
  // y/uv,default:1
  unsigned short tnr_refFrame_format;
  // ��������8���default:2

  unsigned short t_mf_th1;
  //[0, 8191],default:300
  unsigned short t_mf_th2;
  //[0, 8191],default:500
  unsigned short statScale[129];
  //[0, 15],default:8
  unsigned short statScale_key[16];
  unsigned short t_k_scale;
  //[0, 63],default:34
  unsigned short minstep;
  //[0-15],default:2
  unsigned short update_th;
  //[0-15],default:8

  unsigned short uvnr_k;
  //[0, 15],default:8
  unsigned short uvlp_k1;
  //[0, 15],default:7
  unsigned short uvlp_k2;
  //[0, 15],default:0
  unsigned short t_uv_th1;
  //[0, 255],default:48
  unsigned short t_uv_th2;
  //[0, 255],default:16
  unsigned short t_uv_k;
  //[0, 127],default:120
  unsigned short t_uv_kslop;
  //[0, 127],default:32

  unsigned short ynr_weight_tbl[17];
  // ynr_strength
  unsigned short ynr_calc_k;
  unsigned short ylp_k1;
  //[0, 15],default:7
  unsigned short ylp_k2;
  //[0, 15],default:0
  unsigned short t_y_th1;
  //[0, 255],default:48
  unsigned short t_y_th2;
  //[0, 255],default:16
  unsigned short t_y_k;
  //[0, 127],default:120
  unsigned short t_y_kslop;
  //[0, 127],default:32

  unsigned int md_th;
  //[0, 65535]  ����[0-127],default:0
};

struct vpss_isp_3d_nr_attr

{
  unsigned short _3d_nr_mode;
  struct vpss_isp_3d_nr manual_3d_nr;
  struct vpss_isp_3d_nr linkage_3d_nr[9];
};

struct vpss_isp_mwb_attr

{
  unsigned short r_gain;
  unsigned short g_gain;
  unsigned short b_gain;
  short r_offset;
  short g_offset;
  short b_offset;
};

struct vpss_isp_awb_attr

{
  unsigned short g_weight[16];
  unsigned short y_low;
  // y_low<=y_high
  unsigned short y_high;
  unsigned short err_est;
  unsigned short gr_low[10];
  // gr_low[i]<=gr_high[i]
  unsigned short gb_low[10];
  // gb_low[i]<=gb_high[i]
  unsigned short gr_high[10];
  unsigned short gb_high[10];
  unsigned short rb_low[10];
  // rb_low[i]<=rb_high[i]
  unsigned short rb_high[10];

  // awb����2��
  unsigned short auto_wb_step;
  // ����,
  unsigned short total_cnt_thresh;
  // ��8���
  unsigned short colortemp_stable_cnt_thresh;
  // ��6������k���1��
  unsigned short colortemp_envi[10];
};

struct vpss_isp_exp_type

{
  unsigned short exp_type;
};

struct vpss_isp_sensor_reg_info

{
  unsigned short reg_addr;
  unsigned short value;
};

struct vpss_lens_coef

{
  unsigned short coef_b[10];
  //[0,255]
  unsigned short coef_c[10];
  //[0,1023]
};

struct vpss_lsc_attr

{
  unsigned short enable;
  // the reference point of lens correction
  unsigned short xref;
  //[0,4096]
  unsigned short yref;
  //[0,4096]
  unsigned short lsc_shift;
  //[0,15]
  struct vpss_lens_coef lsc_r_coef;
  struct vpss_lens_coef lsc_gr_coef;
  struct vpss_lens_coef lsc_gb_coef;
  struct vpss_lens_coef lsc_b_coef;
  // the range of ten segment
  unsigned short range[10];
  //[0,1023]
  unsigned short lsc_mode;
  unsigned char linkage_strength[10];
  //[0,1023]
};

struct vpss_fps_level

{
  int high_fps;
  int high_fps_exp_time;
  int high_fps_to_lower_fps_gain;

  int mid_fps;
  // set 0 means no use middle fps
  int mid_fps_exp_time;
  // set 0 means no use middle fps
  int mid_fps_to_low_fps_gain;
  // set 0 means no use middle fps

  int low_fps;
  int low_fps_exp_time;
  int low_fps_to_higher_fps_gain;
};

/********************** public *******************************/

/**
 * ak_vpss_get_version - get vpss version
 * return: version string
 */
const char *ak_vpss_get_version(void);

/**
 * ak_vpss_get_ispsdk_version - get ispsdk version
 * return: version string
 * notes:
 */
const char *ak_vpss_get_ispsdk_version(void);

/********************** md *******************************/

/**
 * ak_vpss_hw_md_get_stat: get motion detection stat params
 * @dev[IN]: dev id
 * @md[OUT]: md params
 * return: 0 success, otherwise error code
 * notes:
 */
int ak_vpss_md_get_stat(int dev, struct vpss_md_info *md);

/********************** od *******************************/

/**
 * brief: get occlusion detection params
 * @dev[IN]: dev id
 * @od[OUT]: od params
 * return: 0 success, otherwise error code
 * notes:
 */
int ak_vpss_od_get(int dev, struct vpss_od_info *od);

/********************** af stat *******************************/

/**
 * ak_vpss_af_get_stat: get af stat info
 * @dev[IN]: dev id
 * @af_stat[OUT]: af_stat info
 * return: 0 success, otherwise error code
 * notes:
 */
int ak_vpss_af_get_stat(int dev, struct vpss_af_stat_info *af_stat);

/**
 * ak_vpss_af_set_attr: set af attr
 * @dev[IN]: dev id
 * @af_stat[IN]: af_attr
 * return: 0 success, otherwise error code;
 * notes:
 */
int ak_vpss_af_set_attr(int dev, struct vpss_af_attr *af_attr);

/**
 * ak_vpss_af_get_attr: set af attr
 * @dev[IN]: dev id
 * @af_stat[OUT]: af_attr
 * return: 0 success, otherwise error code;
 * notes:
 */
int ak_vpss_af_get_attr(int dev, struct vpss_af_attr *af_attr);

/********************** effect *******************************/

/**
 * ak_vpss_effect_get - get isp effect param.
 * @dev[IN]: dev id
 * @type[IN]:   effect type name
 * @value[OUT]: effect value(s)
 * return: 0 - success;
 otherwise error code
 */
int ak_vpss_effect_get(int dev, enum vpss_effect_type type, int *value);

/**
 * ak_vpss_effect_set - set isp effect param.
 * @dev[IN]: dev id
 * @type[IN]:   effect type name
 * @value[IN]: effect value
 * return: 0 - success;
 otherwise error code
 */
int ak_vpss_effect_set(int dev, enum vpss_effect_type type, const int value);

/********************** mask *******************************/

/**
 * brief: set main & sub channel mask area
 * @dev[IN]: dev id
 * @area[IN]: main channel mask area paramters
 * return: 0 success, other error code
 * notes:
 */
int ak_vpss_mask_set_area(int dev, const struct vpss_mask_area_info *area);

/**
 * brief: get main & sub channel mask area
 * @dev[IN]: dev id
 * @area[OUT]: main channel mask area paramters
 * return: 0 success, other error code
 * notes:
 */
int ak_vpss_mask_get_area(int dev, struct vpss_mask_area_info *area);

/**
 * brief: set main & sub channel mask color
 * @dev[IN]: dev id
 * @color[IN]: main & sub channel mask color paramters
 * return: 0 success, other error code
 * notes:
 */
int ak_vpss_mask_set_color(int dev, const struct vpss_mask_color_info *color);

/**
 * brief: get main & sub channel mask color
 * @dev[IN]: dev id
 * @color[OUT]: main & sub channel mask color paramters
 * return: 0 success, other error code
 * notes:
 */
int ak_vpss_mask_get_color(int dev, struct vpss_mask_color_info *color);

/**
 * brief: get fps ctrl stat
 * @dev[IN]: device id
 * @stat[OUT]:gain stat
 * @need_fps[OUT]:need fps
 * return: 0 success, otherwise error code
 * notes:for switch day high light and low light. use with ak_vpss_change_sensor_fps
 */
int ak_vpss_get_fps_ctrl_stat(int dev, enum vpss_gain_stat *stat, int *need_fps);

/**
 * brief: change sensor fps
 * @dev[IN]: device id
 * @need_fps[IN]:need fps out by ak_vpss_get_fps_ctrl_stat
 * return:  0 success, otherwise error code
 * notes:for switch day high light and low light. use with ak_vpss_get_fps_ctrl_stat
 */
int ak_vpss_change_sensor_fps(int dev, int need_fps);

/**
 * brief: set sensor fps level
 * @dev[IN]: device id
 * @fps_level[IN]:fps_level params
 * return:  0 success, otherwise error code
 * notes:
 */
int ak_vpss_set_fps_level(int dev, struct vpss_fps_level *fps_level);

/**
 * brief: get sensor fps level
 * @dev[IN]: device id
 * @fps_level[OUT]:fps_level params
 * return:  0 success, otherwise error code
 * notes:
 */
int ak_vpss_get_fps_level(int dev, struct vpss_fps_level *fps_level);

/**
 * brief: get sensor framerate
 * @dev[IN]: device id
 * @fps[OUT]: fps, sensor framerate
 * return: 0 success, otherwise error code
 * notes:
 */
int ak_vpss_get_sensor_fps(int dev, int *fps);

/**
 * ak_vpss_get_ae_run_info: get ae run info
 * @dev[IN]: device id
 * @ae_run_info[OUT]: ae run info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_ae_run_info(int dev, struct vpss_isp_ae_run_info *ae_run_info);

/**
 * ak_vpss_set_ae_init_info: set ae init info
 * @dev[IN]: device id
 * @ae_init_info[IN]: ae init info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_ae_init_info(int dev, struct vpss_isp_ae_init_info *ae_init_info);

/**
 * ak_vpss_get_sensor_ae_info: get sensor fast ae info
 * @dev[IN]: device id
 * @ae_init_info[OUT]: ae info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_sensor_ae_info(int dev, struct vpss_isp_ae_init_info *ae_init_info);

/**
 * ak_vpss_get_ae_attr: get AE attr
 * @dev[IN]: device id
 * @ae_attr[OUT]: AE attr info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_ae_attr(int dev, struct vpss_isp_ae_attr *ae_attr);

/**
 * ak_vpss_set_ae_attr: set AE attr
 * @dev[IN]: device id
 * @ae_attr[IN]: AE attr info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_ae_attr(int dev, const struct vpss_isp_ae_attr *ae_attr);

/**
 * ak_vpss_set_ae_convergence_rate: set AE convergence rate
 * @dev[IN]: device id
 * @value[IN]: param of convergence rate
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_ae_convergence_rate(int dev, unsigned long value);

/**
 * ak_vpss_get_ae_convergence_rate: get AE convergence rate
 * @dev[IN]: device id
 * @value[OUT]: param of convergence rate
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_ae_convergence_rate(int dev, unsigned long *value);

/**
 * ak_vpss_check_ae_stable: check ae is stable or not
 * @dev[IN]: device id
 * @stable[OUT]: 1 stable, 0 not stable
 * return: 0 success, otherwise error code;
 */
int ak_vpss_check_ae_stable(int dev, int *stable);

/**
 * ak_vpss_get_me_attr: get ME attr
 * @dev[IN]: device id
 * @me_attr[OUT]: ME attr info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_me_attr(int dev, struct vpss_isp_me_attr *me_attr);

/**
 * ak_vpss_set_me_attr: set ME attr
 * @dev[IN]: device id
 * @me_attr[IN]: ME attr info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_me_attr(int dev, const struct vpss_isp_me_attr *me_attr);

/*
 * ak_vpss_set_ae_suspend - set ae suspend flag
 * dev[IN]: dev id
 * ae_suspend_flag[IN]: 1 suspend, 0 resume
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_ae_suspend(int dev, int ae_suspend_flag);

/**
 * ak_vpss_get_awb_stat_info: get awb stat info
 * @dev[IN]: device id
 * @awb_stat_info[OUT]: awb stat info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_awb_stat_info(int dev, struct vpss_isp_awb_stat_info *awb_stat_info);

/**
 * brief: get current lum factor
 * @dev[IN]: device id
 * @lumi[OUT]: lum factor
 * return: 0 success, otherwise error code;
 * notes:
 */
int ak_vpss_get_cur_lumi(int dev, int *lumi);

/**
 * ak_vpss_set_auto_day_night_param: set auto day or night switch threshold
 * @dev[IN]: device id
 * @param[IN]: input param threshold
 * return: 0 - success;
 otherwise error code;
 */
int ak_vpss_set_auto_day_night_param(int dev, struct ak_auto_day_night_threshold *param);

/**
 * ak_vpss_get_auto_day_night_level: get day or night
 * @dev[IN]: device id
 * @pre_ir_level[IN]: pre status, 0 day, 1 night
 * @new_level[OUT]: new status, 0 day, 1 night
 * return: 0 - success;
 otherwise error code;
 */
int ak_vpss_get_auto_day_night_level(int dev, int pre_ir_level, int *new_ir_level);

/**
 * ak_vpss_set_force_anti_flicker_flag: set force anti flicker flag
 * @dev[IN]: device id
 * @force_flag[IN]: force anti flicker flag
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_force_anti_flicker_flag(int dev, int force_flag);

/**
 * ak_vpss_get_force_anti_flicker_flag: get force anti flicker flag
 * @dev[IN]: device id
 * @force_flag[OUT]: force anti flicker flag
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_force_anti_flicker_flag(int dev, int *force_flag);

/**
 * ak_vpss_set_force_anti_flicker_flag_ex: set force anti flicker flag and sub_antiflicker_value
 * @dev[IN]: device id
 * @force_flag[IN]: force anti flicker flag
 * @sub_antiflicker_value[IN]: sub_antiflicker_value suggest [64-90]
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_force_anti_flicker_flag_ex(int dev, int force_flag, int sub_antiflicker_value);

/**
 * ak_vpss_get_force_anti_flicker_flag_ex: get force anti flicker flag and sub_antiflicker_value
 * @dev[IN]: device id
 * @force_flag[OUT]: force anti flicker flag
 * @sub_antiflicker_value[OUT]: sub_antiflicker_value
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_force_anti_flicker_flag_ex(int dev, int *force_flag, int *sub_antiflicker_value);

/**
 * brief: open wdr
 * @dev[IN]: device id
 * return: 0 success, otherwise error code;
 * notes:
 */
int ak_vpss_open_wdr(int dev);

/**
 * brief: close wdr
 * @dev[IN]: device id
 * return: 0 success, otherwise error code;
 * notes:
 */
int ak_vpss_close_wdr(int dev);

/**
 * brief: get wdr attr
 * @dev[IN]: device id
 * @p_wdr[OUT]: wdr attr
 * return: 0 success, otherwise error code;
 * notes:
 */
int ak_vpss_get_wdr_attr(int dev, struct vpss_wdr_attr *p_wdr);

/**
 * ak_vpss_get_weight_attr: get weight attr
 * @dev[IN]: device id
 * @weight_attr[OUT]: weight_attr info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_weight_attr(int dev, struct vpss_isp_weight_attr *weight_attr);

/**
 * ak_vpss_set_weight_attr: set weight attr
 * @dev[IN]: device id
 * @weight_attr[IN]: weight_attr info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_weight_attr(int dev, struct vpss_isp_weight_attr *weight_attr);

/**
 * ak_vpss_get_rgb_average: get rgb average value
 * @dev[IN]: device id
 * @r_avr[OUT]: r average value
 * @g_avr[OUT]: g average value
 * @b_avr[OUT]: b average value
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_rgb_average(int dev,
                            unsigned int *r_avr, unsigned int *g_avr, unsigned int *b_avr);

/**
 * ak_vpss_get_wb_type: get wb type
 * @dev[IN]: device id
 * @wb_type[OUT]: wb type
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_wb_type(int dev, struct vpss_isp_wb_type_attr *wb_type);

/**
 * ak_vpss_set_wb_type: set wb type
 * @dev[IN]: device id
 * @wb_type[IN]: wb type
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_wb_type(int dev, const struct vpss_isp_wb_type_attr *wb_type);

/**
 * ak_vpss_get_3d_nr_attr: get 3D NR attr
 * @dev[IN]: device id
 * @nr_3d_attr[OUT]: 3D NR attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_3d_nr_attr(int dev, struct vpss_isp_3d_nr_attr *nr_3d_attr);

/**
 * ak_vpss_set_3d_nr_attr: set 3D NR attr
 * @dev[IN]: device id
 * @nr_3d_attr[IN]: 3D NR attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_3d_nr_attr(int dev, const struct vpss_isp_3d_nr_attr *nr_3d_attr);

/**
 * ak_vpss_get_mwb_attr: get mwb attr
 * @dev[IN]: device id
 * @mwb_attr[OUT]: mwb attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_mwb_attr(int dev, struct vpss_isp_mwb_attr *mwb_attr);

/**
 * ak_vpss_set_mwb_attr: set mwb attr
 * @dev[IN]: device id
 * @mwb_attr[IN]: mwb attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_mwb_attr(int dev, const struct vpss_isp_mwb_attr *mwb_attr);

/**
 * ak_vpss_get_awb_attr: get awb attr
 * @dev[IN]: device id
 * @awb_attr[OUT]: awb attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_awb_attr(int dev, struct vpss_isp_awb_attr *awb_attr);

/**
 * ak_vpss_set_awb_attr: set awb attr
 * @dev[IN]: device id
 * @awb_attr[IN]: awb attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_awb_attr(int dev, const struct vpss_isp_awb_attr *awb_attr);

/**
 * ak_vpss_get_exp_type: get exp type
 * @dev[IN]: device id
 * @exp_type[OUT]: exp type
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_exp_type(int dev, struct vpss_isp_exp_type *exp_type);

/**
 * ak_vpss_set_exp_type: set exp type
 * @dev[IN]: device id
 * @exp_type[IN]: exp type
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_exp_type(int dev, const struct vpss_isp_exp_type *exp_type);

/**
 * ak_vpss_get_sensor_reg: get sensor register info
 * @dev[IN]: device id
 * @sensor_reg_info[IN/OUT]: sensor register info
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_sensor_reg(int dev, struct vpss_isp_sensor_reg_info *sensor_reg_info);

/**
 * ak_vpss_get_lsc_attr: get lsc attr
 * @dev[IN]: device id
 * @lsc[OUT]: lsc attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_lsc_attr(int dev, struct vpss_lsc_attr *lsc);

/**
 * ak_vpss_set_lsc_attr: set lsc attr
 * @dev[IN]: device id
 * @lsc[IN]: lsc attr
 * return: 0 success, otherwise error code;
 */
int ak_vpss_set_lsc_attr(int dev, const struct vpss_lsc_attr *lsc);

/**
 * ak_vpss_get_awb_stable: get awb stable flag
 * @dev[IN]: device id
 * @exp_type[OUT]: awb stable flag
 * return: 0 success, otherwise error code;
 */
int ak_vpss_get_awb_stable(int dev, char *awb_stable_flag);

#endif