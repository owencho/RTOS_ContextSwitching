#include "Kernel.h"
#include <stdint.h>
#include "Common.h"
#include "BaseAddress.h"
#include "List.h"
#include "ThreadContext.h"
#include "MemAlloc.h"
#include "Scb.h"
#include "Irq.h"
TimerEvent * timerEvent;
BlockingEvent * blockingEvent;
volatile Tcb * nextTcb;
volatile Tcb * deQueueTcb;
volatile int allowCS = 0;
volatile int isWaitForEvent = 0;
volatile int isEvent = 0;
volatile int hasContextSwitch;
PostTcbHandler postTcbHandler = (PostTcbHandler)storeTcbInReadyQueue;
void * dataForPostTcbHandler;
volatile Tcb * tcMain;


void kernelInit(){
	  tcMain = tcbCreateMain();
	  listAddItemToTail((List*)&readyQueue,(ListItem*)tcMain);
	  memAllocInit();
	  allowThreadContext();
}

void allowThreadContext(){
	disableIRQ();
	allowCS = 1;
	enableIRQ();
}

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


void acquireMutex(Mutex* mut){
	while(1){
		disableIRQ();
		Tcb * currentTcb = peepHeadTcb();
		if(currentTcb == mut->owner){
			mut->count++;
			enableIRQ();
			return;
		}
		else if(mut->owner){
			triggerContextSwitch((PostTcbHandler)storeTcbInBlockingQueue,&mut->blockingQueue);
			enableIRQ();
			while(!hasContextSwitch);
		}
		else{
			mut->owner = currentTcb;
			enableIRQ();
			return;
		}
	}
}

void releaseMutex(Mutex* mut){
	disableIRQ();
	Tcb * currentTcb = peepHeadTcb();
	Tcb * deQueueMutexTcb;
	if(mut->count > 0){
		mut->count --;
	}
	if(mut->count == 0){
		if(currentTcb != mut->owner){
			enableIRQ();
			return;
		}
		deQueueMutexTcb = deleteHeadListItem((List*)&mut->blockingQueue);
		listAddItemToTail((List*)&readyQueue, (ListItem*)deQueueMutexTcb);
		mut->owner = NULL;
		enableIRQ();
	}
}


void semaphoreDown(Semaphore* sema,int count){
	while(1){
		disableIRQ();
		sema->count=sema->count-count;
		if(sema->count < 0){
			sema->count = 0;
			triggerContextSwitch((PostTcbHandler)storeTcbInBlockingQueue,&sema->blockingQueue);
			enableIRQ();
			while(!hasContextSwitch);
		}
		else{
			enableIRQ();
			return;
		}
	}
}

void semaphoreUp(Semaphore* sema,int count){
	disableIRQ();
	Tcb * deQueueSemaTcb;
	int i = 0;
	sema->count = sema->count + count;
	if(sema->count <=0){
		enableIRQ();
		return;
	}
	deQueueSemaTcb = deleteHeadListItem((List*)&sema->blockingQueue);
	while(deQueueSemaTcb != NULL && i < (count-1)){
		listAddItemToTail((List*)&readyQueue, (ListItem*)deQueueSemaTcb);
		deQueueSemaTcb = deleteHeadListItem((List*)&sema->blockingQueue);
	}

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
	hasContextSwitch = 0 ;
	postTcbHandler = callback;
	dataForPostTcbHandler = data; //unkown data pass in when we need
	scbSetPendSV();
	enableIRQ();
}
