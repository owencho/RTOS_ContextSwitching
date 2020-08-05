/*
 * AssemblyInstruction.c
 *
 *  Created on: Aug 2, 2020
 *      Author: owen
 */

#include "stm32f4xx_hal.h"
#include "AssemblyInstruction.h"
void callSev(){
	__SEV();
}
