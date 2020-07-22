#ifndef KERNEL_H
#define KERNEL_H
#include "TimerEvent.h"
typedef void (*QueueTask)(void* queue, void *event);
void contextSwitchingISR(QueueTask task);
void kernelSleep(TimerEvent* evt,int time);
void deQueueEnqueue();
void callWaitForEvent(void);
void peepHeadTcb();
#endif // KERNEL_H
