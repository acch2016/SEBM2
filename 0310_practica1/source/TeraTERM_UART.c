/*
 * TeraTERM_UART.c
 *
 *  Created on: Mar 29, 2018
 *      Author: edson
 */

#include "TeraTERM_UART.h"

#define UART_BUFFER_SIZE 16
#define READ_MEMORY 1
#define WRITE_MEMORY 2
#define SET_TIME 3
#define SET_DATE 4
#define TIME_FORMAT 5
#define READ_TIME 6
#define READ_DATE 7
#define TERM2_COMM 8
#define ECO_LCD 9

/** menu variables **/
TaskHandle_t  menu_handle;
//SemaphoreHandle_t menu_semaphore;
//SemaphoreHandle_t read_semaphore;


uint8_t UART_Buffer[UART_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

void UART0_RX_TX_IRQHandler()
{
	uint8_t data;

	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0))
	{
		data = UART_ReadByte(UART0);
		UART_WriteByte(UART0, data);


		/* If ring buffer is not full, add data to ring buffer. */
		if (((rxIndex + 1) % UART_BUFFER_SIZE) != txIndex)
		{
			UART_Buffer[rxIndex] = data;
			rxIndex++;
			rxIndex %= UART_BUFFER_SIZE;
			//UART_WriteByte(UART0, UART_Buffer[txIndex]);
//			UART_MENU_SELECT(data);
		}
	}
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
	      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}

void read_task(void* parameter)
{
	uint8_t data;

for(;;)
{

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
		//xSemaphoreTake(minutes_semaphore,portMAX_DELAY);
		UART_WriteBlocking(UART0, menu, sizeof(menu) / sizeof(menu[0]));
//		UART_WriteBlocking(UART0, UART_Buffer, sizeof(UART_Buffer) / sizeof(UART_Buffer[0]));
		UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
		EnableIRQ(UART0_RX_TX_IRQn);
//		while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0)) && (rxIndex != txIndex))
//				{
//					UART_WriteByte(UART0, UART_Buffer[txIndex]);
//					txIndex++;
//					txIndex %= UART_BUFFER_SIZE;
//				}
		vTaskDelay(portMAX_DELAY);

	}


	void UART_MENU_SELECT(data)
	{
			switch(data)
			{

			case READ_MEMORY:
				xTaskCreate(read_Mem_Task, "read_mem_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
				vTaskSuspend(menu_handle);

				break;

			case WRITE_MEMORY:

				break;

			case SET_TIME:

				break;

			case SET_DATE:

				break;

			case TIME_FORMAT:

				break;

			case READ_TIME:

				break;

			case READ_DATE:

				break;

			case TERM2_COMM:

				break;

			case ECO_LCD:
				uint8_t

				break;

			default:

				break;

			}
	}


}

void UART_TeraTerm_Init()
{
	//menu_semaphore = xSemaphoreCreateBinary();
	//read_semaphore = xSemaphoreCreateBinary();
	uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;
	UART_Init(UART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));


}
