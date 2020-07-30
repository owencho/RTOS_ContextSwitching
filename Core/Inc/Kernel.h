#ifndef KERNEL_H
#define KERNEL_H
#include "TimerEvent.h"
#include "TimerEventQueue.h"
#include "TcbQueue.h"
#include "Tcb.h"

typedef void (*PostTcbHandler)(void * tcb, void *queue);

void storeTcbInReadyQueue(Tcb* tcb);
void storeTcbInTimerQueue(Tcb* tcb);
void triggerContextSwitch(PostTcbHandler callback , void*data);

void kernelSleep(TimerEvent* evt,int time);
void deQueueEnqueue();
void peepHeadTcb();
#endif // KERNEL_H
