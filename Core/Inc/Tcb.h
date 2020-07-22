/*
 * Tcb.h
 *
 *  Created on: Jul 2, 2020
 *      Author: academic
 */

#ifndef INC_TCB_H_
#define INC_TCB_H_

#include<stdint.h>
#include"ThreadContext.h"

void (*Task)(void);

Tcb * tcbCreate(int mallocNum ,void (*Task)(void),char * name );
Tcb * tcbCreateMain(void);
#endif /* INC_TCB_H_ */
