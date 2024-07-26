

#ifndef _SAT_USER_QUEUE_H_
#define _SAT_USER_QUEUE_H_
#include <stdbool.h>

typedef void *	sat_queue_handle;
typedef void *	sat_queue_user;

typedef struct sat_queue_node {
	unsigned char * data;
	int 			size;
	unsigned long long pts;
} sat_queue_node;


/***********************************************
** 作者: leo.liu
** 日期: 2022-11-22 16:58:20
** 说明: 销毁队列
***********************************************/
void sat_queue_end(sat_queue_handle qq);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-22 16:58:39
** 说明: 停止队列读写
***********************************************/
void sat_queue_abort(sat_queue_handle qq);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-23 11:24:1
** 说明:	
***********************************************/
void sat_queue_flush(sat_queue_handle qq);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-22 15:12:41
** 说明: 删除一个可读用户
***********************************************/
bool sat_queue_user_del(sat_queue_user user);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-22 15:12:41
** 说明: 增加一个可读用户
***********************************************/
sat_queue_user sat_queue_user_create(sat_queue_handle qq);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-22 15:9:1
** 说明: 创建一个队列
***********************************************/
sat_queue_handle sat_queue_create(int queue_max);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-22 15:9:1
** 说明: 标记释放节点
***********************************************/
bool sat_queue_free(sat_queue_user user, sat_queue_node * *pkt);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-23 11:27:12
** 说明: 获取一个数据 
***********************************************/
bool sat_queue_get(sat_queue_user user, sat_queue_node * *pkt, int block);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-23 11:27:42
** 说明: 送入队列 
***********************************************/
bool sat_queue_put(sat_queue_handle qq, sat_queue_node * pkt);

#endif


