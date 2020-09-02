/*
 * Serial.c
 *
 *  Created on: Aug 13, 2020
 *      Author: academic
 */
#include "Irq.h"
#include "Nvic.h"
#include "Usart.h"
#include "Serial.h"
#include "Rcc.h"
#include "Gpio.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

volatile int usartTurn = 0;
UsartRegs * sharedUsart;
char * messageToSend;

void configureUart5(){
	  enableGpio(PORT_C);
	  gpioSetMode(gpioC, PIN_12, GPIO_ALT);  //set GpioC as alternate mode
	  gpioSetPinSpeed(gpioC,PIN_12,HIGH_SPEED);

	  enableGpio(PORT_D);
	  gpioSetMode(gpioD, PIN_2, GPIO_ALT);  //set GpioC as alternate mode
	  gpioSetPinSpeed(gpioD,PIN_2,HIGH_SPEED);

	  //set alternate function
	  gpioSetAlternateFunction(gpioC ,PIN_12 ,AF8); //set PC12 as USART5_TX
	  gpioSetAlternateFunction(gpioD ,PIN_2 ,AF8); //set PD2 as USART5_RX

	  enableUART5();
	  nvicEnableInterrupt(53);
	  //usartEnableInterrupt(uart5,TRANS_COMPLETE);
	  setUsartOversamplingMode(uart5,OVER_16);
	  usartSetBaudRate(uart5,115200);
	  setUsartWordLength(uart5,DATA_8_BITS);
	  usartEnableParityControl(uart5);
	  setUsartParityMode(uart5,ODD_PARITY);
	  usartSetStopBit(uart5,STOP_BIT_2);
	  usartEnableTransmission(uart5);
	  enableUsart(uart5);
}

void freeMessage(char * msg){
    if(msg)
        free(msg);
}

void serialSend(UsartRegs * usart,char *message,...){
	disableIRQ();
    int actualLength;
    char* buffer;


    va_list arg;
    va_start(arg, message);

    actualLength = vsnprintf(NULL,0, message, arg);
    buffer =malloc(actualLength + 1);
    vsnprintf(buffer,actualLength + 1, message, arg);
    va_end(arg);
    usartSendMessage(usart,buffer);
    freeMessage(buffer);
    enableIRQ();
}


void usartSendMessage(UsartRegs * usart,char *message){
	if(!usartTurn){
		usartEnableInterrupt(uart5,TRANS_COMPLETE);
		sharedUsart = usart;
		messageToSend =message;
		usartTurn = 1;
	}
}
