/*! \file samsat_i2c.h
    \brief Driver I2C for LPC4357
*/

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include "I2C_LPC43xx.h"

extern ARM_DRIVER_I2C Driver_I2C0;
extern ARM_DRIVER_I2C Driver_I2C1;

#define LPC4357_I2C_INT 	((ARM_DRIVER_I2C*)(&Driver_I2C0)) /*!< Hardware module I2C0 */
#define LPC4357_I2C_EXT 	((ARM_DRIVER_I2C*)(&Driver_I2C1)) /*!< Hardware module I2C1 */

typedef enum
{
	SAMSAT_I2C_EC_OK = 0,
	SAMSAT_I2C_EC_UNSPECIFIED_ERROR,
	SAMSAT_I2C_EC_BUSY,
	SAMSAT_I2C_EC_TIMEOUT,
	SAMSAT_I2C_EC_OPERATION_UNSUPPORTED,
	SAMSAT_I2C_EC_PARAMETER_ERROR,
	SAMSAT_I2C_EC_TRANSFER_INCOMPLETE,
	SAMSAT_I2C_EC_DATA_COUNT_INCORRECT,
	SAMSAT_I2C_EC_DRIVER_NULL_ERROR
} SAMSAT_I2C_EC_t;

/*! \fn samsat_i2c_init(void *i2c_drv, uint8_t onOff)
    \brief Init I2C 
		\param void* *i2c_drv - it is refer to Driver I2C0 or I2C1 
    \param uint8_t onOff Init or deinit parameter (1 or 0)
		\return 0, if all operations is ok, or 0xFF.
*/
uint8_t samsat_i2c_init(void *i2c_drv, uint8_t onOff);


uint8_t samsat_i2c_clear_bus(void *i2c_drv);

/*! \fn samsat_i2c_master_read(void *i2c_drv, const uint8_t address, const void *buffer, const uint16_t length)
    \brief I2C Master Read
		\param void* i2c_drv - it is needs refer to bus driver Driver_I2C0 or Driver_I2C1
    \param const uint8_t addr - I2C address of device
		\param const void* buffer - refer to data, that return from device
		\param const uint8_t length - it is a number of bytes, that needed to transaction
		\return 0, if all operations is ok, or 0xFE,if data count isn't correct, of oxFF, if adress of i2c drivers isn't correct
*/
uint8_t samsat_i2c_master_read (void *i2c_drv, const uint8_t address, const void *buffer, const uint16_t length);

/*! \fn samsat_i2c_master_write(void *i2c_drv, const uint8_t address, const void *buffer, const uint16_t length)
    \brief I2C Master Transmit
		\param void* i2c_drv i2c - it is needs refer to bus driver Driver_I2C0 or Driver_I2C1
    \param const uint8_t addr - I2C address of device
		\param const void* buffer - refer to data, that return from device
		\param const uint8_t length - it is a number of bytes, that needed to transaction
		\return 0, if all operations is ok, or 0xFE,if data count isn't correct, of oxFF, if adress of i2c drivers isn't correct
*/
uint8_t samsat_i2c_master_write(void *i2c_drv, const uint8_t address, const void *buffer, const uint16_t length);

#endif  /* __I2C_H */
