/*
 * TimerEvent.h
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */

#ifndef INC_TIMEREVENT_H_
#define INC_TIMEREVENT_H_

#include <stdint.h>
#include <stddef.h>
#include "ThreadContext.h"
typedef struct TimerEvent TimerEvent;
struct TimerEvent {
		TimerEvent * next;
		int time;
		int8_t * stackPtr;
		int8_t * stackBaseAddr;
		ThreadState threadState;
		void * data;
};

#endif /* INC_TIMEREVENT_H_ */
