/*
 * Memalloc.c
 *
 *  Created on: Aug 20, 2020
 *      Author: owen
 */

#include<stdlib.h>
#include "Mutex.h"
#include "Kernel.h"

extern Mutex * libMutex;

void memAllocInit(){
	libMutex = mutexCreate();
}
void * memAllocMemory(int size){
	void * mallocMemory;
	acquireMutex(libMutex);
	mallocMemory = malloc(size);
	releaseMutex(libMutex);
	return mallocMemory;
}
