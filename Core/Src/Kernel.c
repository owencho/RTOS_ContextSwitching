#include "Kernel.h"
#include <stdint.h>
#include "Common.h"
#include "BaseAddress.h"
#include "List.h"
#include "ThreadContext.h"
#include "Scb.h"
#include "Irq.h"
TimerEvent * timerEvent;
BlockingEvent * blockingEvent;
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

void initMutex(Mutex *mut){
	mut->owner = NULL;
	mut->count = 1;
}

void acquireMutex(Mutex* mut){
	disableIRQ();
	Tcb * currentTcb = peepHeadTcb();
	if(currentTcb == mut->owner){
		enableIRQ();
		return;
	}
	else if(mut->count == 0){
		triggerContextSwitch((PostTcbHandler)storeTcbInBlockingQueue,&mut->blockingQueue);
	}
	else{
		mut->owner = currentTcb;
		mut->count = 0 ;
	}

	enableIRQ();
}

void releaseMutex(Mutex* mut){
	disableIRQ();
	Tcb * currentTcb = peepHeadTcb();
	Tcb * deQueueMutexTcb;
	if(currentTcb != mut->owner || mut->count != 0){
		enableIRQ();
		return;
	}
	deQueueMutexTcb = deleteHeadListItem((List*)&mut->blockingQueue);
	listAddItemToTail((List*)&readyQueue, (ListItem*)deQueueMutexTcb);
	mut->owner = NULL;
	mut->count = 1;
	enableIRQ();
}

void initSemaphore(Semaphore* sema,int count){
	sema->count = count;
}

void semaphoreDown(Semaphore* sema,int count){
	disableIRQ();
	if(sema->count == 0){
		triggerContextSwitch((PostTcbHandler)storeTcbInBlockingQueue,&sema->blockingQueue);
	}
	else{
		sema->count=sema->count-count;
		if(sema->count <0){
			sema->count = 0;
		}
	}
	enableIRQ();
}

void semaphoreUp(Semaphore* sema,int count){
	disableIRQ();
	Tcb * deQueueSemaTcb;
	int i = 0;
	deQueueSemaTcb = deleteHeadListItem((List*)&sema->blockingQueue);
	while(deQueueSemaTcb != NULL && i < (count-1)){
		listAddItemToTail((List*)&readyQueue, (ListItem*)deQueueSemaTcb);
		deQueueSemaTcb = deleteHeadListItem((List*)&sema->blockingQueue);
	}
	sema->count = sema->count + count;
	enableIRQ();
}


void storeTcbInReadyQueue(Tcb* tcb){
	disableIRQ();
	listAddItemToTail((List*)&readyQueue, (ListItem*)tcb);
	enableIRQ();
}

void storeTcbInTimerQueue(Tcb* tcb){
	disableIRQ();
	timerEvent = (TimerEvent*)dataForPostTcbHandler;
	timerEvent->data = tcb;
	timerEventEnqueue(&timerEventQueue,timerEvent);
	postTcbHandler = (PostTcbHandler)storeTcbInReadyQueue;
	enableIRQ();
}

void storeTcbInBlockingQueue(Tcb* tcb){
	disableIRQ();
	listAddItemToTail((List*)dataForPostTcbHandler, (ListItem*)tcb);
	postTcbHandler = (PostTcbHandler)storeTcbInReadyQueue;
	enableIRQ();
}


Tcb * peepHeadTcb(){
	disableIRQ();
	resetCurrentListItem((List*)&readyQueue);
	nextTcb =(Tcb *) getCurrentListItem((List*)&readyQueue);
	enableIRQ();
	return nextTcb;
}

void triggerContextSwitch(PostTcbHandler callback , void*data){
	disableIRQ();
	postTcbHandler = callback;
	dataForPostTcbHandler = data; //unkown data pass in when we need
	scbSetPendSV();
	enableIRQ();
}

