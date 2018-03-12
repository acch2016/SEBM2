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
 * @file    0310_practica1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_port.h"
#include "fsl_i2c.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* TODO: insert other definitions and declarations here. */
typedef long BaseType_t;
//volatile bool g_MasterCompletionFlag = false;
i2c_master_handle_t g_m_handle;

SemaphoreHandle_t semaphoreISR;
SemaphoreHandle_t mutex;
/*
 * @brief   Application entry point.
 */


static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

void i2c_ReleaseBus()
{
	uint8_t i = 0;
	gpio_pin_config_t pin_config;
	port_pin_config_t i2c_pin_config =
	{ 0 };

	/* Config pin mux as gpio */
	i2c_pin_config.pullSelect = kPORT_PullUp;
	i2c_pin_config.mux = kPORT_MuxAsGpio;

	pin_config.pinDirection = kGPIO_DigitalOutput;
	pin_config.outputLogic = 1U;
	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinConfig(PORTE, 24, &i2c_pin_config);
	PORT_SetPinConfig(PORTE, 25, &i2c_pin_config);

	GPIO_PinInit(GPIOE, 24, &pin_config);
	GPIO_PinInit(GPIOE, 25, &pin_config);

	GPIO_PinWrite(GPIOE, 25, 0U);
	i2c_release_bus_delay();

	for (i = 0; i < 9; i++)
	{
		GPIO_PinWrite(GPIOE, 24, 0U);
		i2c_release_bus_delay();

		GPIO_PinWrite(GPIOE, 25, 1U);
		i2c_release_bus_delay();

		GPIO_PinWrite(GPIOE, 24, 1U);
		i2c_release_bus_delay();
		i2c_release_bus_delay();
	}

	GPIO_PinWrite(GPIOE, 24, 0U);
	i2c_release_bus_delay();

	GPIO_PinWrite(GPIOE, 25, 0U);
	i2c_release_bus_delay();

	GPIO_PinWrite(GPIOE, 24, 1U);
	i2c_release_bus_delay();

	GPIO_PinWrite(GPIOE, 25, 1U);
	i2c_release_bus_delay();
}

static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle,
        status_t status, void * userData)
{
//	PRINTF(" i2c_master_callback \n\r");

	SemaphoreHandle_t *h_semaphoreISR = (SemaphoreHandle_t *)userData;

	BaseType_t reschedule;
	reschedule = pdFALSE;

//	if (status == kStatus_Success)
//	{
//		g_MasterCompletionFlag = true;
//	}
    xSemaphoreGiveFromISR(*h_semaphoreISR, &reschedule); /* TODO: de aqui no pasa */
    portYIELD_FROM_ISR(reschedule);
}

void ST_RTC(void *arg)
{
	PRINTF(" ST_RTC \n\r");

	i2c_master_transfer_t masterXfer;
	uint8_t data_buffer = 0x80;
	masterXfer.slaveAddress = 0x6F;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0x00;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &data_buffer;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

    xSemaphoreTake(mutex, portMAX_DELAY);
	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
//	while (!g_MasterCompletionFlag)
//	{
//	}
//	g_MasterCompletionFlag = false;
	xSemaphoreTake(semaphoreISR, portMAX_DELAY);
	xSemaphoreGive(mutex);

}

void Read_RTC(void *arg)
{
	PRINTF(" Read_RTC \n\r");

	while(1)
	{
		i2c_master_transfer_t masterXfer;
		uint8_t *buffer;
		masterXfer.slaveAddress = 0x6F;
		masterXfer.direction = kI2C_Read;
		masterXfer.subaddress = 0x00;
		masterXfer.subaddressSize = 1;
		masterXfer.data = buffer;
		masterXfer.dataSize = 1;
		masterXfer.flags = kI2C_TransferDefaultFlag;

		xSemaphoreTake(mutex, portMAX_DELAY);
		I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
	//	while (!g_MasterCompletionFlag)
	//	{
	//	}
	//	g_MasterCompletionFlag = false;
		xSemaphoreTake(semaphoreISR, portMAX_DELAY);
		xSemaphoreGive(mutex);

		PRINTF("%d\n\r", &buffer);
	}

}

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

	CLOCK_EnableClock(kCLOCK_PortE);
	CLOCK_EnableClock(kCLOCK_I2c0);

	port_pin_config_t config_i2c =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAlt5,
	        kPORT_UnlockRegister, };

	PORT_SetPinConfig(PORTE, 24, &config_i2c);
	PORT_SetPinConfig(PORTE, 25, &config_i2c);

	i2c_master_config_t masterConfig;
	I2C_MasterGetDefaultConfig(&masterConfig);

	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));

	semaphoreISR = xSemaphoreCreateBinary();
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle, i2c_master_callback, &semaphoreISR);

//	NVIC_EnableIRQ(PORTE_IRQn);

    mutex = xSemaphoreCreateMutex();

	xTaskCreate(ST_RTC, "ST_RTC", configMINIMAL_STACK_SIZE+210, NULL, configMAX_PRIORITIES, NULL);
	xTaskCreate(Read_RTC, "Read_RTC", configMINIMAL_STACK_SIZE+210, NULL, configMAX_PRIORITIES, NULL);
	vTaskStartScheduler();

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}