#ifndef KERNEL_H
#define KERNEL_H
#include "TimerEvent.h"
#include "Tcb.h"
typedef void (*PostTcbHandler)(Tcb* tcb, void *queue);
void contextSwitchingISR(QueueTask task);
void kernelSleep(TimerEvent* evt,int time);
void deQueueEnqueue();
void peepHeadTcb();
#endif // KERNEL_H
