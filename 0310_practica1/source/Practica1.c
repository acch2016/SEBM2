/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Practica1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "LCDNokia5110.h"

#define BUFFER_LEN (64)
/* TODO: insert other include files here. */




volatile bool isFinished = false;
uint8_t string1[]="ITESO"; /*! String to be printed in the LCD*/
uint8_t string2[]="uMs y DSPs";
uint8_t ITESO[504];

uint8_t receiveBuff[BUFFER_LEN];

void LCD_PRINT_TASK()
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(65000);
	xLastWakeTime = xTaskGetTickCount();
	uint8_t string1[]="ITESO"; /*! String to be printed in the LCD*/
	uint8_t string2[]="uMs y DSPs"; /*! String to be printed in the LCD*/
	LCDNokia_init(); /*! Configuration function for the LCD */
	for(;;) {
		LCDNokia_clear();/*! It clears the information printed in the LCD*/
		LCDNokia_bitmap(&ITESO[0]); /*! It prints an array that hold an image, in this case is the initial picture*/
		vTaskDelayUntil(&xLastWakeTime,xPeriod);
		LCDNokia_clear();
		vTaskDelayUntil(&xLastWakeTime,xPeriod);
		LCDNokia_clear();
		LCDNokia_gotoXY(25,0); /*! It establishes the position to print the messages in the LCD*/
		LCDNokia_sendString(string1); /*! It print a string stored in an array*/
		vTaskDelayUntil(&xLastWakeTime,xPeriod);
		LCDNokia_gotoXY(10,1);
		LCDNokia_sendString(string2); /*! It print a string stored in an array*/
		vTaskDelayUntil(&xLastWakeTime,xPeriod);
		LCDNokia_gotoXY(25,2);
		LCDNokia_sendChar('2'); /*! It prints a character*/
		LCDNokia_sendChar('0'); /*! It prints a character*/
		LCDNokia_sendChar('1'); /*! It prints a character*/
		LCDNokia_sendChar('5'); /*! It prints a character*/
		vTaskDelayUntil(&xLastWakeTime,xPeriod);

	}
}


int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	xTaskCreate(LCD_PRINT_TASK,"LCD_PRINT_TASK", configMINIMAL_STACK_SIZE,NULL,configMAX_PRIORITIES,NULL);

	for(;;)
	{
	}
}


