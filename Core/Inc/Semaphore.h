/*
 * Semaphore.h
 *
 *  Created on: Aug 5, 2020
 *      Author: owen
 */

#ifndef INC_SEMAPHORE_H_
#define INC_SEMAPHORE_H_

#include "BlockingQueue.h"
#include "Tcb.h"
typedef struct Semaphore Semaphore;
struct Semaphore {
	int count;
	BlockingQueue blockingQueue;
};

void initSemaphore(Semaphore* sema,int count);
Semaphore* semaphoreCreate(int count);
#endif /* INC_SEMAPHORE_H_ */
