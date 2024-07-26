#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_
#include <stdbool.h>
/*
 * @日期: 2022-08-08
 * @作者: leo.liu
 * @功能: 创建消息队列句柄
 * @return:
 */
bool sat_msg_queue_create(int *handle_id);
/*
 * @日期: 2022-08-08
 * @作者: leo.liu
 * @功能: 发送一个消息
 * @return:
 */
bool sat_msg_queue_send(int handle_id, void *data, int size);
/*
 * @日期: 2022-08-08
 * @作者: leo.liu
 * @功能: 接收一个消息
 * @return:
 */
bool sat_msg_queue_recv(int handle_id, void *data, int data_size);
#endif