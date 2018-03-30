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
 * @file    Practica_1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "MK64F12.h"
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "queue.h"
#include "TeraTERM_UART.h"

#define LCD_DATA 1
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

QueueHandle_t xQueue;

typedef enum{eco_lcd, time_lcd} data_type_t;

typedef struct
{
	data_type_t data_type;
	uint8_t value;
} lcd_msg_t;




void LCD_ECO_TASK()
{
	lcd_msg_t *message;
	message = pvPortMalloc(sizeof(lcd_msg_t));
	static uint8_t print_second= 0;

	for(;;)
	{
		LCDNokia_gotoXY(0, 0);
		print_second = message->value;
		xQueueReceive(xQueue,&message,1000);
		LCDNokia_sendChar(print_second);
		LCDNokia_sendChar(':');
		LCDNokia_sendChar(print_second);
		LCDNokia_sendChar(':');
		LCDNokia_sendChar(print_second);

		vPortFree(message);
	}
}

void SEND_DATA()
{
	lcd_msg_t *message;
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(1000);
	xLastWakeTime = xTaskGetTickCount();
	uint8_t seconds = 48;
	for(;;)
	{
		message = pvPortMalloc(sizeof(lcd_msg_t));
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		seconds++;
		message->data_type = eco_lcd;
		message->value = seconds;
		xQueueSend( xQueue,&message,portMAX_DELAY);
	}

}
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	xQueue = xQueueCreate(2,sizeof(lcd_msg_t));

	NVIC_SetPriority(UART0_RX_TX_IRQn,5);


	LCDNokia_init();
	LCDNokia_clear();
	UART_TeraTerm_Init();
	xTaskCreate(LCD_ECO_TASK, "LCD_ECO_Task", 200, NULL, configMAX_PRIORITIES-4, NULL);
	xTaskCreate(SEND_DATA, "Send_data", 200, NULL, configMAX_PRIORITIES-4, NULL);
	xTaskCreate(menu_Task, "menu_task", 250, NULL, configMAX_PRIORITIES, NULL);//canale
	vTaskStartScheduler();





	/* Force the counter to be placed into memory. */
	volatile static int i = 0 ;
	/* Enter an infinite loop, just incrementing a counter. */
	while(1) {
		i++ ;
	}
	return 0 ;
}
