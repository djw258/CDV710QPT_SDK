#ifndef _AK_MD_H_
#define _AK_MD_H_


#define MAX_BOX_NUM 	5


enum ak_md_errno {
	ERROR_MD_NOT_INIT					= ( MODULE_ID_MD << 24 ) + 0,
    ERROR_MD_LIB_INIT_FAILED			= ( MODULE_ID_MD << 24 ) + 1,
    ERROR_MD_CREATE_THREAD_FAILED	    = ( MODULE_ID_MD << 24 ) + 2,
    ERROR_MD_NOT_RUN	                = ( MODULE_ID_MD << 24 ) + 3,
};


struct md_box
{
	int left;
	int top;
	int right;
	int bottom;
};

struct md_result_info
{
    int result;
    int md_sec;
	int move_box_num;
	struct md_box boxes[MAX_BOX_NUM];
};



/**
 * ak_md_get_version - get md version
 * return: version string
 * notes: 
 */
const char* ak_md_get_version(void);

/**
 * ak_md_get_mrd_version - get mrd version
 * return: version string
 * notes: 
 */
const char* ak_md_get_mrd_version(void);

/**
 * ak_md_init - md init. set defaut fps ,global sensitivity
 * @dev[IN]:  dev id
 * 
 * return:  0 - success;  other error code;
 */
int ak_md_init(int dev);

/**
 * ak_md_set_fps -  set how much frame to do md in one second
 * @dev[IN]:  dev id
 * @fps[IN]:     how much frame to do md in one second. 
 * return:  0 - success;  other error code;
 * note: this fps should be no more than fps of sensor.
 */
int ak_md_set_fps(int dev, int fps);

/**
 * ak_md_get_fps -  get fps
 * @dev[IN]:  dev id
 * @fps[OUT]:   how much frame that md do  in one second. 
 * return:  0 - success;  other error code;
 */
int ak_md_get_fps(int dev, int *fps);

/**
 * ak_md_set_sensitivity -  set the sensitivity that  md do
 * @dev[IN]:  dev id
 * @sensitivity[IN]:   sensitivity. 
 * return:  0 - success;  other error code;
 */
int ak_md_set_sensitivity(int dev, int sensitivity);

/**
 * ak_md_get_sensitivity -  get sensitivity
 * @dev[IN]:  dev id
 * @sensitivity[OUT]:   how sensitivity that md do   
 * return: 0 - success;  other error code;
 */
int ak_md_get_sensitivity(int dev, int *sensitivity);

/**
 * ak_md_set_filters -  set mrd filters
 * @dev[IN]:  dev id
 * @flt_big[IN]:   flt_big
 * @flt_small[IN]:   flt_small
 * return:  0 - success;  other error code;
 */
int ak_md_set_filters (int dev, unsigned short flt_big, unsigned short flt_small);

/**
 * ak_md_set_day_night_mode -  set how much frame to do md in one second
 * @dev[IN]:  dev id
 * @day_night_mode[IN]:  0  night, 1 day
 * return:  0 - success;  other error code;
 */
int ak_md_set_day_night_mode(int dev, int day_night_mode);

/**
 * ak_md_set_area - set area that md to do.
 * @dev[IN]:  dev id
 * @area_mask[IN]: area mask .
 * return:  0 - success;  other error code;
 */
int ak_md_set_area(int dev, char *area_mask);

/**
 * ak_md_get_dimensionality - get dimensionality.
 * @dev[IN]:  dev id
 * @horizon_num[OUT]:   dimension number of horizontal.
 * @vertical_num[OUT]:   dimension number of vertial.
 * return:  0 - success;  other error code;
 */
int ak_md_get_dimensionality(int dev, int *horizon_num, int *vertical_num);

/**
 * ak_md_get_result - get md result.
 * @dev[IN]:  dev id
 * @ret_info[OUT]:  result info
 * @timeout[IN]:  timeout <0  block mode, ==0 non-block mode, >0 waittime
 * return: 0 - success;  other error code;
 */
int ak_md_get_result(int dev, struct md_result_info *ret_info, int timeout);

/** 
 * ak_md_enable - start or stop md . 
 * @dev[IN]:  dev id
 * @enable[IN]:  [0,1],  0 -> stop md; 1 -> start md 
 * return: 0 - success; other error code;
 */
int ak_md_enable(int dev, int enable);

/* 
 * ak_md_destroy - free md resource and quit md. 
 * @dev[IN]:  dev id
 * return: 0 - success; other error code; 
 */
int ak_md_destroy(int dev);

#endif
