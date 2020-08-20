/*
 * BlockingQueue.c
 *
 *  Created on: Aug 20, 2020
 *      Author: owen
 */

#include"BlockingQueue.h"
#include<stdlib.h>

void initBlockingQueue(BlockingQueue * queue){
	queue->head = NULL;
	queue->tail = NULL;
	queue->previous = NULL;
	queue->current = NULL;
	queue->count =0;
}
