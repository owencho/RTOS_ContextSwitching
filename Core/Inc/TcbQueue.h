#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stddef.h>
#include "ThreadContext.h"

typedef struct TcbQueue TcbQueue;
struct TcbQueue {
	Tcb * head;
	Tcb * tail;
	int count;
	Tcb * current;
	Tcb * previous;
};

#endif // EVENT_H
