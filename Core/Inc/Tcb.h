/*
 * Tcb.h
 *
 *  Created on: Jul 2, 2020
 *      Author: academic
 */

#ifndef INC_TCB_H_
#define INC_TCB_H_

typedef struct Tcb Tcb;
struct Tcb {
	int8_t * stackPtr;
	ThreadState threadState;
};

typedef enum{
	 BLOCKING_STATE,RUNNING_STATE,WAITING_STATE,SLEEP_STATE
} ThreadState;

#endif /* INC_TCB_H_ */
