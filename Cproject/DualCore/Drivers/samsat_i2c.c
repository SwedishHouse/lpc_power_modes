#include "samsat_i2c.h"

#define COMMON_EC(res)		(uint8_t)( -res )

#define SAMSAT_I2C_ATTEMPTS_COUNT	100

static volatile uint32_t I2C_Event;

extern void _delay_ms (uint32_t delay);

inline static void I2C_SignalEvent (uint32_t event)
{
  I2C_Event |= event;
}

uint8_t samsat_i2c_init(void* i2c_drv, uint8_t on)
{
	int8_t res=0;
	ARM_DRIVER_I2C* I2Cdrv = (ARM_DRIVER_I2C*)i2c_drv;
	
	if(I2Cdrv != NULL)
	{
		if (on = 1U) /* Branch if we want to init driver*/
		{
			/* Next sequence of actions - init, power on, set speed, clear wire*/
			res = I2Cdrv->Initialize(&I2C_SignalEvent);
			if (res != SAMSAT_I2C_EC_OK) 
				return COMMON_EC(res);
			
			res = I2Cdrv->PowerControl(ARM_POWER_FULL);
			if (res != SAMSAT_I2C_EC_OK)
				return COMMON_EC(res);
			
			res = I2Cdrv->Control(ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_STANDARD);
			if (res != SAMSAT_I2C_EC_OK)
				return COMMON_EC(res);
		}
		else /* Branch to deinit driver*/
		{
			/* Next  order of actions is - power off, uninit */
			res = I2Cdrv->PowerControl(ARM_POWER_OFF);
			if (res != SAMSAT_I2C_EC_OK)
				return COMMON_EC(res);
			
			res = I2Cdrv->Uninitialize();
			if (res != SAMSAT_I2C_EC_OK)
				return COMMON_EC(res);
		}
		
		return SAMSAT_I2C_EC_OK;
	}
	else
	{
		return SAMSAT_I2C_EC_DRIVER_NULL_ERROR;
	}
}

uint8_t samsat_i2c_clear_bus(void *i2c_drv)
{
	int8_t res=0;
	ARM_DRIVER_I2C* I2Cdrv = (ARM_DRIVER_I2C*)i2c_drv;
	
	if(I2Cdrv != NULL)
	{
		res = I2Cdrv->Control(ARM_I2C_BUS_CLEAR,0);
		if (res != SAMSAT_I2C_EC_OK)
			return COMMON_EC(res);
		
		return SAMSAT_I2C_EC_OK;
	}
	else
	{
		return SAMSAT_I2C_EC_DRIVER_NULL_ERROR;
	}
}

uint8_t samsat_i2c_master_write(void *i2c_drv, const uint8_t address, const void *buffer, const uint16_t length)
{
	int8_t res=0, attempts=0;
	ARM_DRIVER_I2C* I2Cdrv = (ARM_DRIVER_I2C*)i2c_drv;
	
	if(I2Cdrv != NULL)
	{
		I2C_Event = 0U;
		
		res = I2Cdrv->Control(ARM_I2C_BUS_CLEAR,0);
		if (res != SAMSAT_I2C_EC_OK)
			return COMMON_EC(res);
		
		res = I2Cdrv->MasterTransmit(address, (uint8_t*)buffer, (uint32_t)length, false);
		if (res != SAMSAT_I2C_EC_OK)
			return COMMON_EC(res);
				
		/* Wait until transfer completed */
		while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U && ++attempts < SAMSAT_I2C_ATTEMPTS_COUNT)
			_delay_ms(1);
		
		/* Check if all data transferred */
		if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U)
			return SAMSAT_I2C_EC_TRANSFER_INCOMPLETE;
	 
		/* Clear event flags before new transfer */
		I2C_Event = 0U;
		
		if(I2Cdrv->GetDataCount() != length)
			return SAMSAT_I2C_EC_DATA_COUNT_INCORRECT;
		
		return SAMSAT_I2C_EC_OK;
	}
	else
	{
		return SAMSAT_I2C_EC_DRIVER_NULL_ERROR;
	}
}

uint8_t samsat_i2c_master_read(void *i2c_drv, const uint8_t address, const void *buffer, const uint16_t length)// Do not need transmitt FID and CS, just array for data from buffer SC18IS602B
{
	int8_t res=0, attempts=0;
	ARM_DRIVER_I2C* I2Cdrv = (ARM_DRIVER_I2C*)i2c_drv;
	
	if(I2Cdrv != NULL)
	{
		I2C_Event = 0U;
		
		res = I2Cdrv->Control(ARM_I2C_BUS_CLEAR,0);// Stop anything on I2C-bus
		if (res != SAMSAT_I2C_EC_OK)
			return COMMON_EC(res);
		
		res = I2Cdrv->MasterReceive(address, (uint8_t *)buffer, (uint32_t)length, false);
		if (res != SAMSAT_I2C_EC_OK)
			return COMMON_EC(res);
		
		/* Wait until transfer completed */
		while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U && ++attempts < SAMSAT_I2C_ATTEMPTS_COUNT)
			_delay_ms(1);
		
		/* Check if all data transferred */
		if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U)
			return SAMSAT_I2C_EC_TRANSFER_INCOMPLETE;
		
		/* Clear event flags before new transfer */
		I2C_Event = 0U;
		
		if(I2Cdrv->GetDataCount() != length)
			return SAMSAT_I2C_EC_DATA_COUNT_INCORRECT;
		
		return SAMSAT_I2C_EC_OK;
	}
	else
	{
		return SAMSAT_I2C_EC_DRIVER_NULL_ERROR;
	}
}
