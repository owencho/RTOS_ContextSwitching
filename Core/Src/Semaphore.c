/*
 * Semaphore.c
 *
 *  Created on: Aug 12, 2020
 *      Author: owen
 */


#include "Semaphore.h"
#include <stdio.h>
#include <stdlib.h>

void initSemaphore(Semaphore* sema,int count){
	sema->count = count;
}

Semaphore* semaphoreCreate(int count){
	Semaphore * semaphore = malloc(sizeof(Semaphore));
	initSemaphore(semaphore,count);
	return semaphore;
}
