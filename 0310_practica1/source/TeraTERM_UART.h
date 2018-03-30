/*
 * TeraTERM_UART.h
 *
 *  Created on: Mar 29, 2018
 *      Author: edson
 */

#ifndef TERATERM_UART_H_
#define TERATERM_UART_H_


#include "fsl_uart.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "FreeRTOS.h"
#include "task.h"
void UART_IRQHandler();
void UART_TeraTerm_Init();

void menu_Task(void *parameter);

#endif /* TERATERM_UART_H_ */
