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
//#include "TeraTERM_UART.h"

#define LCD_DATA 1
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

QueueHandle_t xQueue;
QueueHandle_t UART_Queue;



typedef enum{eco_lcd, time_lcd} data_type_t;

typedef struct
{
	data_type_t data_type;
	uint8_t value;
} lcd_msg_t;


#include "TeraTERM_UART.h"
#include "task.h"
#define UART_BUFFER_SIZE 16
#define READ_MEMORY 49
#define WRITE_MEMORY 50
#define SET_TIME 51
#define SET_DATE 52
#define TIME_FORMAT 53
#define READ_TIME 54
#define READ_DATE 55
#define TERM2_COMM 56
#define ECO_LCD 57

/** menu variables **/
//SemaphoreHandle_t menu_semaphore;
//SemaphoreHandle_t read_semaphore;

TaskHandle_t  menu_handle;



uint8_t UART_Buffer[UART_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

void UART0_RX_TX_IRQHandler()
{
	uint8_t data;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0))
	{
		data = UART_ReadByte(UART0);
		UART_WriteByte(UART0, data);
		xQueueSendFromISR(UART_Queue, &data, xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	}
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
	      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}



void UART_MENU_SELECT()
{
	uint8_t *seleccion;
	seleccion = pvPortMalloc(sizeof(uint8_t));

	for(;;)
	{
		xQueueReceive(UART_Queue,&seleccion,portMAX_DELAY);

		if(seleccion !=0 )
		{
			xTaskCreate(menu_Task, "menu_task", 250, NULL, configMAX_PRIORITIES, NULL);//canale
		}

//		switch(seleccion)
//		{
//
//		case READ_MEMORY:
//			//xTaskCreate(read_Mem_Task, "read_mem_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
//
//			break;
//
//		case WRITE_MEMORY:
//
//			break;
//
//		case SET_TIME:
//
//			break;
//
//		case SET_DATE:
//
//			break;
//
//		case TIME_FORMAT:
//
//			break;
//
//		case READ_TIME:
//
//			break;
//
//		case READ_DATE:
//
//			break;
//
//		case TERM2_COMM:
//
//			break;
//
//		case ECO_LCD:
//
//
//			break;
//
//		default:
//		}
		//vPortFree(seleccion);
	}



}



//void UART_MENU_SELECT(uint8_t data)
void menu_Task(void *parameter)
{
	menu_handle = xTaskGetCurrentTaskHandle();
	uint8_t menu[] = "\033[0;30;47m"
			"\e[ ? 25 l"
			"\033[2J"
			"1) Leer Memoria I2C\r\n"
			"2) Escribir memoria I2C\r\n"
			"3) Establecer Hora\r\n"
			"4) Establecer Fecha\r\n"
			"5) Formato de hora\r\n"
			"6) Leer hora\r\n"
			"7) Leer fecha\r\n"
			"8) Comunicacion con terminal 2\r\n"
			"9) Eco en LCD\r\n";

	for(;;)
	{
		UART_WriteBlocking(UART0, menu, sizeof(menu) / sizeof(menu[0]));
		vTaskDelete(menu_handle);
	}

}

void LCD_ECO_TASK()
{
	lcd_msg_t *message;
	message = pvPortMalloc(sizeof(lcd_msg_t));
	static uint8_t print_second= 0;

	for(;;)
	{
		LCDNokia_gotoXY(0, 0);
		xQueueReceive(xQueue,&message,1000);
		print_second = message->value;
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
	UART_Queue = xQueueCreate(2,sizeof(uint8_t));

	NVIC_SetPriority(UART0_RX_TX_IRQn,5);


	LCDNokia_init();
	LCDNokia_clear();
	UART_TeraTerm_Init();
	xTaskCreate(LCD_ECO_TASK, "LCD_ECO_Task", 200, NULL, configMAX_PRIORITIES-4, NULL);
	xTaskCreate(SEND_DATA, "Send_data", 200, NULL, configMAX_PRIORITIES-4, NULL);
	xTaskCreate(menu_Task, "menu_task", 250, NULL, configMAX_PRIORITIES, NULL);//canale
	xTaskCreate(UART_MENU_SELECT, "UART_Menu_select", 250, NULL, configMAX_PRIORITIES, NULL);//canale
	vTaskStartScheduler();





	/* Force the counter to be placed into memory. */
	volatile static int i = 0 ;
	/* Enter an infinite loop, just incrementing a counter. */
	while(1) {
		i++ ;
	}
	return 0 ;
}
