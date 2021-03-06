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
 * @file    p1_sbm2017.c
 * @brief   Application entry point.
 */
#include <string.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*#include "fsl_debug_console.h"*/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "user.h"
#include "UART.h"

//TaskHandle_t tareahand;
/* Task priorities. */
#define time_task_PRIORITY (configMAX_PRIORITIES - 1)

/*!
 * @brief Application entry point.
 */
//int main(void) {
//  /* Init board hardware. */
//
//
//  BOARD_InitPins();
//  BOARD_BootClockRUN();
//  BOARD_I2C_ConfigurePins();
//  BOARD_InitDebugConsole();
//  i2c_init();
//  uart_init();
//
//  NVIC_SetPriority(UART0_RX_TX_IRQn, 5U);
//  NVIC_SetPriority(UART3_RX_TX_IRQn, 6U);
// // NVIC_SetPriority(UART4_RX_TX_IRQn, 6U);
//
//  createQueues();
////vTaskSuspend();
//  void * handle = getHandleBlueTerm();
//  //xTaskCreate(getTime_task, "Time_Task", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
//  //xTaskCreate(sendTime, "UART_Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
// // xTaskCreate(uart_init_Task, "uart_init_Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
//
//  xTaskCreate(menu_Task,"menu_task", 250,NULL, 1, NULL);
//  xTaskCreate(menu_Task_b,"menu_task_b", 250,NULL, 1, NULL);
//
////  xTaskCreate(chat_Task, "chat_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
////  xTaskCreate(chat_Task_b, "chat_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
//
//
//  vTaskStartScheduler();
//
//  for(;;) { /* Infinite loop to avoid leaving the main function */
//    __asm("NOP"); /* something to use as a breakpoint stop while looping */
//  }
//}

int main(void) {
  /* Init board hardware. */


  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_I2C_ConfigurePins();
  BOARD_InitDebugConsole();
  i2c_init();
  uart_handle_config * terminal_tt;
  uart_init(UART0,terminal_tt);
  uart_handle_config * terminal_bt;
  uart_init(UART3,terminal_bt);

  xTaskCreate(menu_Task,"menu_task", 250,NULL, 1, terminal_tt);
  xTaskCreate(menu_Task,"menu_task_b", 250,NULL, 1, terminal_bt);

  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
