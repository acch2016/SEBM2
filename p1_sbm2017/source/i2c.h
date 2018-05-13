/*
 * i2c.h
 *
 *  Created on: Mar 16, 2018
 *      Author: Cursos
 */


#ifndef I2C_H_
#define I2C_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "event_groups.h"
#include "fsl_i2c.h"
#include "fsl_port.h"

#define EVENT_BIT (1<<0)
#define I2C_BAUDRATE 100000U


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
uint8_t i2c_init(void);
bool I2C_Read(I2C_Type *base, uint8_t device_addr, uint8_t reg_addr, uint8_t *rxBuff, uint32_t rxSize);
bool I2C_Write(I2C_Type *base, uint8_t device_addr, uint8_t reg_addr, uint8_t value);




#endif /* I2C_H_ */
