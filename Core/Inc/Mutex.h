/*
 * Mutex.h
 *
 *  Created on: Aug 5, 2020
 *      Author: owen
 */

#ifndef INC_MUTEX_H_
#define INC_MUTEX_H_

#include "BlockingQueue.h"

typedef struct Mutex Mutex;
struct Mutex {
	int count;
	Tcb * owner;
	BlockingQueue blockingQueue;
};


#endif /* INC_MUTEX_H_ */
