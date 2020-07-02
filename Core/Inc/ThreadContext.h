/*
 * ThreadContext.h
 *
 *  Created on: Jul 2, 2020
 *      Author: academic
 */

#ifndef INC_THREADCONTEXT_H_
#define INC_THREADCONTEXT_H_
typedef struct ThreadContext ThreadContext;
struct ThreadContext {
	int32_t  LrAfterDelay;
	int32_t  R4;
	int32_t  R5;
	int32_t  R6;
	int32_t  R7;
	int32_t  R8;
	int32_t  R9;
	int32_t  R10;
	int32_t  R11;
	int32_t  R0;
	int32_t  R1;
	int32_t  R2;
	int32_t  R3;
	int32_t  R12;
	int32_t  Lr;
	int32_t  returnAddress;
	int32_t  xpsr;
};

#endif /* INC_THREADCONTEXT_H_ */
