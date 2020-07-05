#include "Tcb.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f429xx.h"
Tcb * tcbCreateMain(void ){
	Tcb * tcb;
	tcb = malloc(sizeof(Tcb));
	tcb->stackBaseAddr =  0;
	tcb->stackPtr =(int8_t*) __get_MSP();
	return tcb;
}

Tcb * tcbCreate(int mallocNum ,void (*Task)(void) ){
	Tcb * tcb;
	ThreadContext * tc;
	int8_t * baseaddress;
	tcb = malloc(sizeof(Tcb));
	baseaddress = malloc(mallocNum);
	tcb->stackBaseAddr = baseaddress;
	tcb->stackPtr = (baseaddress + mallocNum - sizeof(ThreadContext));
	//& 0xFFFFFFF8
	tc =(ThreadContext *) tcb->stackPtr;
	tc-> R4 = 0x14444444;
	tc-> R5 = 0x15555555;
	tc-> R6 = 0x16666666;
	tc-> R7 = 0x17777777;
	tc-> R8 = 0x18888888;
	tc-> R9 = 0x19999999;
	tc-> R10 = 0x1AAAAAAA;
	tc-> R11= 0x1BBBBBBB;
	tc-> R0 = 0x10000000;
	tc-> R1 = 0x15555555;
	tc-> R2 = 0x16666666;
	tc-> R3 = 0x17777777;
	tc-> R12 = 0x1CCCCCC;
	tc-> returnAddress = (uint32_t)Task;
	tc-> Lr = 0xFFFFF9;
	tc-> xpsr = 0x01000000;
	return tcb;
}
