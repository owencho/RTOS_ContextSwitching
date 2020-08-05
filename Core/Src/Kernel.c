#include "Kernel.h"
#include <stdint.h>
#include "Common.h"
#include "BaseAddress.h"
#include "List.h"
#include "ThreadContext.h"
#include "Scb.h"
#include "Irq.h"

volatile Tcb * nextTcb;
volatile Tcb * deQueueTcb;
volatile int isWaitForEvent = 0;
volatile int isEvent = 0;
PostTcbHandler postTcbHandler = (PostTcbHandler)storeTcbInReadyQueue;
void * dataForPostTcbHandler;
extern TimerEventQueue timerEventQueue;
extern TcbQueue readyQueue;

void deQueueEnqueue(){
	disableIRQ();
	resetCurrentListItem((List*)&readyQueue);
	deQueueTcb =(Tcb *) deleteHeadListItem((List*)&readyQueue);
	enableIRQ();
}

void kernelSleep(TimerEvent* evt,int time){
	disableIRQ();
	evt->time = time;
	isWaitForEvent = 0;
	triggerContextSwitch((PostTcbHandler)storeTcbInTimerQueue,evt);
	enableIRQ();
}


void storeTcbInReadyQueue(Tcb* tcb){
	disableIRQ();
	listAddItemToTail((List*)&readyQueue, (ListItem*)tcb);
	enableIRQ();
}

void storeTcbInTimerQueue(Tcb* tcb){
	disableIRQ();
	TimerEvent * event = (TimerEvent*)dataForPostTcbHandler;
	event->data = tcb;
	timerEventEnqueue(&timerEventQueue,event);
	postTcbHandler = (PostTcbHandler)storeTcbInReadyQueue;
	enableIRQ();
}
/*
void storeTcbInBlockingQueue(Tcb* tcb){
	listAddItemToTail((List*)dataForPostTcbHandler, (ListItem*)event);
	postTcbHandler = (PostTcbHandler)storeTcbInReadyQueue;
}
*/

void peepHeadTcb(){
	disableIRQ();
	resetCurrentListItem((List*)&readyQueue);
	nextTcb =(Tcb *) getCurrentListItem((List*)&readyQueue);
	enableIRQ();
}

void triggerContextSwitch(PostTcbHandler callback , void*data){
	disableIRQ();
	postTcbHandler = callback;
	dataForPostTcbHandler = data; //unkown data pass in when we need
	scbSetPendSV();
	enableIRQ();
}

