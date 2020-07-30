#include "Kernel.h"
#include <stdint.h>
#include "Common.h"
#include "BaseAddress.h"
#include "List.h"
#include "ThreadContext.h"
#include "TimerEventQueue.h"
#include "TcbQueue.h"
#include "Scb.h"
TimerEvent * event;
extern TimerEventQueue timerEventQueue;
extern TcbQueue readyQueue;
volatile Tcb * nextTcb;
volatile Tcb * deQueueTcb;
int isWaitForEvent = 0;
volatile int isEvent = 0;
PostTcbHandler postTcbHandler = storeTcbInReadyQueue;
void deQueueEnqueue(){
	resetCurrentListItem((List*)&readyQueue);
	deQueueTcb =(Tcb *) deleteHeadListItem((List*)&readyQueue);
}

void kernelSleep(TimerEvent* evt,int time){
	peepHeadTcb();
	evt->data =(void*)nextTcb;
	evt->time = time;
	event = evt;
	isWaitForEvent = 0;
	triggerContextSwitch(storeTcbInTimerQueue,event);

}

void storeTcbInReadyQueue(Tcb* tcb, TcbQueue *queue){
	listAddItemToTail((List*)queue, (ListItem*)tcb);
}

void peepHeadTcb(){
	resetCurrentListItem((List*)&readyQueue);
	nextTcb =(Tcb *) getCurrentListItem((List*)&readyQueue);
}

void triggerContextSwitch(PostTcbHandler callback , void*data){
	postTcbHandler = callback;
	dataForPostTcbHandler = data;
	scbSetPendSV();
}

