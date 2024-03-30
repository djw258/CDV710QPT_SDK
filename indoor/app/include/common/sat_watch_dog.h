#ifndef _WATCH_DOG_H_
#define _WATCH_DOG_H_
// 启动看门狗
bool watch_dog_start(int timeout);

// 喂狗
void watch_dog_alive(void);

// 关闭看门狗
bool watch_dog_close(void);
#endif