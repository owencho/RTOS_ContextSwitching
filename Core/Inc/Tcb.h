/*
 * Tcb.h
 *
 *  Created on: Jul 2, 2020
 *      Author: academic
 */

#ifndef INC_TCB_H_
#define INC_TCB_H_

#include<stdint.h>
typedef enum{
	 BLOCKING_STATE,RUNNING_STATE,WAITING_STATE,SLEEP_STATE
} ThreadState;

typedef struct Tcb Tcb;
struct Tcb {
	Tcb * next;
	int8_t * stackPtr;
	int8_t * stackBaseAddr;
	ThreadState threadState;
};
void (*Task)(void);



typedef struct ThreadContext ThreadContext;
struct ThreadContext {
	uint32_t  R4;
	uint32_t  R5;
	uint32_t  R6;
	uint32_t  R7;
	uint32_t  R8;
	uint32_t  R9;
	uint32_t  R10;
	uint32_t  R11;
	uint32_t  R0;
	uint32_t  R1;
	uint32_t  R2;
	uint32_t  R3;
	uint32_t  R12;
	uint32_t  Lr;
	uint32_t  returnAddress;
	uint32_t  xpsr;
};

Tcb * tcbCreate(int mallocSize ,void (*Task)(void));
Tcb * tcbCreateMain(void);
#endif /* INC_TCB_H_ */
