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

#endif /* INC_SEMAPHORE_H_ */
