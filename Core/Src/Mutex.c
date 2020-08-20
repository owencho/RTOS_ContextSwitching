/*
 * Mutex.c
 *
 *  Created on: Aug 12, 2020
 *      Author: owen
 */


#include "Mutex.h"
#include <stdio.h>
#include <stdlib.h>

Mutex* mutexCreate(){
	Mutex *mutex = malloc(sizeof(Mutex));
	initMutex(mutex);
	return mutex;

}

void initMutex(Mutex *mut){
	mut->owner = NULL;
	mut->count = 1;
	initBlockingQueue(&mut->blockingQueue);
}
