#ifndef KERNEL_H
#define KERNEL_H
#include "TimerEvent.h"
#include "TimerEventQueue.h"
#include "TcbQueue.h"
#include "Tcb.h"
#include "BlockingQueue.h"
#include "Mutex.h"

typedef void (*PostTcbHandler)(void * tcb, void *queue);

void storeTcbInReadyQueue(Tcb* tcb);
void storeTcbInTimerQueue(Tcb* tcb);
void storeTcbInBlockingQueue(Tcb *tcb);
void triggerContextSwitch(PostTcbHandler callback , void*data);

void kernelSleep(TimerEvent* evt,int time);
void setMutex(Mutex *mut);
void acquireMutex(Mutex * mut);
void releaseMutex(Mutex* mut);
void deQueueEnqueue();
Tcb * peepHeadTcb();
#endif // KERNEL_H
