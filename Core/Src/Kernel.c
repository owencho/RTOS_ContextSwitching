#include "Kernel.h"
#include <stdint.h>
#include "Common.h"
#include "BaseAddress.h"
#include "List.h"
#include "Tcb.h"
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
	scbSetPendSV();
}

void peepHeadTcb(){
	resetCurrentListItem((List*)&readyQueue);
	nextTcb =(Tcb *) getCurrentListItem((List*)&readyQueue);
}

void waitForEvent(){
	while(!isEvent){
		__asm("wfe");
	}
	scbSetPendSV();
	isEvent = 0 ;
}

void contextSwitchingISR(QueueTask task){
	__asm("stmdb sp!,{r4-r11,lr}"); //push r4-r11 and lr
	if(!isWaitForEvent){
		deQueueEnqueue();
		__asm("ldr r0,=deQueueTcb"); //r0 points to deQueueTcb pointer
		__asm("ldr r1 , [r0]");      //r1 has address inside deQueueTcb Pointer
		__asm("str sp ,[r1,#4]");	   // sp are now store inside deQueueTcb
		task(&timerEventQueue,(void*)event);
	}
	isWaitForEvent = 0 ;
	if(readyQueue.count != 0){
		peepHeadTcb();
		__asm("ldr r0,=nextTcb");
		__asm("ldr r1 , [r0]");
		__asm("ldr r2 , [r1,#4]");
		__asm("mov sp ,r2");
		__asm("ldmia sp!,{r4-r11,lr}");
		__asm("bx lr");
	}
	else{
		isWaitForEvent = 1;
		callWaitForEvent();
	}
}

void callWaitForEvent(void){
	//get sp from mainTCB stackPtr
	__asm("ldr r0,=tcMain");
	__asm("ldr r1,[r0]");
	__asm("ldr r2 , [r1,#4]");
	__asm("mov sp ,r2");
	//assign value to push
	__asm("ldr r1 ,=waitForEvent");
	__asm("ldr r0 ,=0xFFFFFFF9");
	__asm("ldr r2 ,=0x01000000");
	__asm("push {r2}"); //push xpsr
	__asm("push {r1}"); //push returnAdd
	__asm("push {r0}"); //push lr
	__asm("ldr r0,=0x10000000");
	__asm("ldr r1,=0x15555555");
	__asm("ldr r2,=0x16666666");
	__asm("ldr r3,=0x13333333");
	__asm("ldr r12,=0x1CCCCCC");
	__asm("push {r0,r1,r2,r3,r12}");
	__asm("ldr r0 ,=0xFFFFFFF9");
	__asm("push {r0}"); //push execReturn
	__asm("pop {lr}");
	__asm("bx lr");
}
