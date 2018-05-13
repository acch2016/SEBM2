/*
 * i2c.c
 *
 *  Created on: Mar 16, 2018
 *      Author: Cursos
 */
#include "i2c.h"

i2c_master_handle_t g_m_handle;
SemaphoreHandle_t mutex;
EventGroupHandle_t event;

static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle,
        status_t status, void * userData)
{
//	PRINTF(" i2c_master_callback \n\r");

	BaseType_t reschedule;
	reschedule = pdFALSE;

	if (status == kStatus_Success)
	{
//		  xSemaphoreGiveFromISR(semaphoreISR, &reschedule); /* TODO: de aqui no pasa */
		xEventGroupSetBitsFromISR(event, EVENT_BIT, &reschedule);
	}
  //    xSemaphoreGiveFromISR(semaphoreISR_, &reschedule_); /* TODO: de aqui no pasa */
//    xEventGroupSetBits(event,EVENT_BIT);
//    portYIELD_FROM_ISR(pdTRUE == reschedule || pdTRUE == reschedule_ ? pdTRUE : pdFALSE);
    portYIELD_FROM_ISR(reschedule);
}


void init_i2c()
{
    CLOCK_EnableClock(kCLOCK_I2c0);
	CLOCK_EnableClock(kCLOCK_PortB);

	port_pin_config_t config_i2c =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAlt2,
	        kPORT_UnlockRegister, };

	PORT_SetPinConfig(PORTB, 2, &config_i2c);
	PORT_SetPinConfig(PORTB, 3, &config_i2c);

	i2c_master_config_t masterConfig;
	I2C_MasterGetDefaultConfig(&masterConfig);

	masterConfig.baudRate_Bps = I2C_BAUDRATE;

	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));

//	memset(&g_m_handle, 0, sizeof(g_m_handle));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle, i2c_master_callback, NULL);

	NVIC_EnableIRQ(I2C0_IRQn);
	//NVIC_EnableIRQ(PORTB_IRQn);
	NVIC_SetPriority(I2C0_IRQn,5);
	//NVIC_SetPriority(PORTB_IRQn,5);
    mutex = xSemaphoreCreateMutex();
    event = xEventGroupCreate();
}

void ST_RTC()
{

	i2c_master_transfer_t masterXfer;
//    memset(&masterXfer, 0, sizeof(masterXfer));

	uint8_t data_buffer = 0x80;
	masterXfer.slaveAddress = 0x6F;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = (uint32_t)0x00;
	masterXfer.subaddressSize = 1U;
	masterXfer.data = &data_buffer;
	masterXfer.dataSize = 1U;
	masterXfer.flags = kI2C_TransferDefaultFlag;

    xSemaphoreTake(mutex, portMAX_DELAY);
	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
//	while (!g_MasterCompletionFlag)
//	{
//	}
//	g_MasterCompletionFlag = false;
//	xSemaphoreTake(semaphoreISR, portMAX_DELAY);
	xEventGroupWaitBits(event, EVENT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
	xSemaphoreGive(mutex);
}

void Read_RTC(void *arg)
{
	uint8_t status;
	EventBits_t uxBits;
	PRINTF(" Read_RTC \n\r");
//	i2c_master_handle_t g_m_handle_2;
//	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle_2, i2c_master_callback_2, NULL);

	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	uint8_t buffer;
//	uint8_t *buffer;

	while(1)
	{

		masterXfer.slaveAddress = 0x6F;
		masterXfer.direction = kI2C_Read;
		masterXfer.subaddress = (uint32_t)0x00;
		masterXfer.subaddressSize = 1U;
		masterXfer.data = &buffer;
		masterXfer.dataSize = 1U;
		masterXfer.flags = kI2C_TransferDefaultFlag;

		xSemaphoreTake(mutex, portMAX_DELAY);
		status = I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);

		if (status != kStatus_Success)
		    {
			xEventGroupSetBits(event, EVENT_BIT);
		    }
	//	while (!g_MasterCompletionFlag)
	//	{
	//	}
	//	g_MasterCompletionFlag = false;
//		xSemaphoreTake(semaphoreISR, portMAX_DELAY);
		uxBits = xEventGroupWaitBits(event, EVENT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

		if ((uxBits & EVENT_BIT) != 0)
		{
			/* xEventGroupWaitBits() returned because just EVENT_BIT was set. */
			PRINTF("EVENT_BIT was set\n\r");
		}


//		delay(1000);

		xSemaphoreGive(mutex);

		PRINTF("%X\n\r", buffer);
//		PRINTF("%X\n\r", (uint8_t)buffer);
	}

}
