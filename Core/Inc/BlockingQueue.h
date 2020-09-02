/*
 * BlockingQueue.h
 *
 *  Created on: Aug 5, 2020
 *      Author: owen
 */

#ifndef INC_BLOCKINGQUEUE_H_
#define INC_BLOCKINGQUEUE_H_

typedef struct BlockingEvent BlockingEvent;
struct BlockingEvent {
	BlockingEvent * next;
	void * data;
};

typedef struct BlockingQueue BlockingQueue ;
struct BlockingQueue{
	BlockingEvent * head ;
	BlockingEvent * tail ;
    int count ;
    BlockingEvent * previous;
    BlockingEvent * current ;
};

void initBlockingQueue(BlockingQueue * queue);

#endif /* INC_BLOCKINGQUEUE_H_ */
