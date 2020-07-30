    .type  add2Integers, %function
    .type  dummy, %function
    .type  switchThreadContext, %function
//week 3
/*
switchThreadContext:
	ldr r0 ,=0x00ffff00 // load 0x00ffff00 into r0
	ldr r1 ,=0x11111111 // load 0x11111111 into r1
	ldr r2 ,=0x22222222 // load 0x22222222 into r2
	ldr r3 ,=0x33333333 // load 0x33333333 into r3
	ldr r12 ,=0xcccccccc // load 0xcccccccc into r12
	//push {r0} // to show is there any padding to become odd
	//push {r1}
*/
//waitHere4Interrupt:

	//bal waitHere4Interrupt //loop this function
/*
//week5
//CS with linked List
checkReadyQueue:
	ldr r0,=readyQueue   //r0 has address of readyQueue
	ldr r2 , [r0,#8]	// r2 has value in readyQueue+8 means is the count of ready queue
	cmp r2 , #0        //compare is readyqueue zero
	beq returnToOriginal //branch to return IF COUNT IS ZERO
	bx lr
/*
contextSwitchingISR:
	stmdb sp!,{r4-r11,lr} //push
	bl 		checkReadyQueue //check is the ready queue null if null will jump to returnToOriginal

	//checkIsHeadOfReadyQueueComeFromTimerQueue
	bl 		peepHeadTcb   // check currentItem
	ldr r0,=nextTcb      //r0 points to address of nextTcb
	ldr r1 , [r0]       // r1 has address value of nextTcb
	ldr r2 , [r1,#20]	// r2 has value of address in nextTcb+20
	cmp r2 , #1        //compare if the Tcb came from TimerQueue
	beq peepHeadForNextHeadItem //branch to Tcb if item came from TimerQueue

	//Dequeue from ready queue and load current stack pointer into Tcb
	//and Enqueue into timerQueue
	bl 		deQueueEnqueue  // deQueue from ready Queue head
	// load sp into deQueueTCB->stackPTR
	ldr r0,=deQueueTcb
	ldr r1 , [r0]
	str sp ,[r1,#8]
	// add dequeue Tcb into timerQueue
	bl pushIntoTimerQueue
	bl checkReadyQueue //check is the ready queue if not 0 COUNT then continue

peepHeadForNextHeadItem:
	bl 		peepHeadTcb
	bl 		resetComeFromTimerEvent
	//load sp in tcb into pc sp
	//must do like this for pointer because nextTcb is a pointer
	ldr r0,=nextTcb      //r0 points to address of nextTcb
	ldr r1 , [r0]       // r1 has address value of nextTcb
	ldr r2 , [r1,#8]	// r2 has address in nextTcb+8
	mov sp ,r2			// move value of r2 into sp

returnToOriginal:
	//pop r4-r11 and lr
	ldmia sp!,{r4-r11,lr}
	bx lr   // return to lr address
*/
/*
contextSwitchingISR:
	stmdb sp!,{r4-r11,lr} //push
	bl 		deQueueEnqueue
	// load sp into deQueueTCB->stackPTR
	ldr r0,=deQueueTcb
	ldr r1 , [r0]
	str sp ,[r1,#4]
	//mov r1 ,sp
	bl pushIntoTimerQueue
	//load sp in tcb into pc sp
	bl 		peepHeadTcb
	ldr r0,=nextTcb      //r0 points to nextTcb address
	ldr r1 , [r0]       // r1 has address of nextTcb
	ldr r2 , [r1,#4]	// r2 has address in nextTcb+4
	mov sp ,r2
	//pop r4-r11 and lr
	ldmia sp!,{r4-r11,lr}
	bx lr   // return to lr address
*/
//CS without linked List
/*
	mov r0,sp      //load sp value into r0
	mov sp,r1	  //load r1 value into sp
contextSwitchingISR:
	push {lr}
	//bl SysTick_Handler
	// Do thread context switching
	stmdb sp!,{r4-r11} //push
	//ldr r0,=tcMain //load tcMain into r0
	ldr r2,=thread1	  //r2 has address of the pointer
	ldr r1,[r2]  //r1 has the value address of the pointer
	mov r0,sp      //load sp value into r0
	mov sp,r1	  //load r1 value into sp
	ldmia sp!,{r4-r11} //pop r4-r11
	//pop {lr} //pop lr
	bx lr   // return to lr address
*/

//Week 2
add2Integers:
	// if s is added on the operand
	add R0,r1
	adds R0,R1 // this one will update if zero and carry bit
	//conditional////////////////////////
	add R0,R1
	ldr R0, =123
	ldr R1, =124
	// if (r0<= r1)
	//	r2 = 34;
	//else r2 = 56;
	cmp r0,r1
	ite		gt //if else statement
	//just to find out we do correctly anot like a (if else statement)
	movgt r2 ,#34 		//===> nop is condition
	movle r2, #56

	//jumping
	cmp r0,r1
	//ble lessEqual
	mov r2,#34
	b done


done:
//earlier one////////////////////////
	add 	R0,R1
	ldr 	R3 , =0x12345678 //use this if mov fail
	mov r0, 0x100000
	//push 	{r0,r1,r2,r8}		//
	stmdb sp,{r0,r1,r2,r8}		//push for reg >7
	ldmia sp,{r0,r1,r2,r8}		//pop for reg >7
	bl 		dummy		// branch(call) to dummy
	pop 	{r0}
	pop 	{r1}
	pop 	{r2}
	pop 	{r8}
	//pop		{LR}		//return the original LR
	bx 		LR           //branch (jump) with exchange to address in LR  (return with LR)

dummy:
	mov		R2,R1
	bx		LR			//return


void contextSwitchingISR(QueueTask task){
	__asm("stmdb sp!,{r4-r11,lr}"); //push r4-r11 and lr
	if(!isWaitForEvent){
		deQueueEnqueue();
		__asm("ldr r0,=deQueueTcb"); //r0 points to deQueueTcb pointer
		__asm("ldr r1 , [r0]");      //r1 has address inside deQueueTcb Pointer
		__asm("str sp ,[r1,#4]");	   // sp are now store inside deQueueTcb
		task(&timerEventQueue,(void*)event);
	}
	isWaitForEvent = 0 ;
	if(readyQueue.count != 0){
		peepHeadTcb();
		__asm("ldr r0,=nextTcb");
		__asm("ldr r1 , [r0]");
		__asm("ldr r2 , [r1,#4]");
		__asm("mov sp ,r2");
		__asm("ldmia sp!,{r4-r11,lr}");
		__asm("bx lr");
		}
		else{
			isWaitForEvent = 1;
			callWaitForEvent();
		}
}
