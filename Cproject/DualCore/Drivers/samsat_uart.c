#include "samsat_uart.h"

#define COMMON_EC(res)		(uint8_t)( -res )
#define SPECIFIC_EC(res)	(uint8_t)( res < ARM_DRIVER_ERROR_SPECIFIC ? -res-1 : -res )

#define SAMSAT_UART_ATTEMPTS_COUNT	100

static volatile uint32_t UART_Event;

inline static void myUSART_callback(uint32_t event)
{
	UART_Event |= event;
}

uint8_t samsat_uart_init(void *uart_drv)
{
	int8_t res=0;
	ARM_DRIVER_USART* UARTdrv = (ARM_DRIVER_USART*)uart_drv;

	if(UARTdrv!=NULL)
	{
		res = UARTdrv->Initialize(&myUSART_callback);
		if (res != SAMSAT_UART_EC_OK)
			return COMMON_EC(res);
		
		res = UARTdrv->PowerControl(ARM_POWER_FULL);
		if (res != SAMSAT_UART_EC_OK)
			return COMMON_EC(res);

		res = UARTdrv->Control(	ARM_USART_MODE_ASYNCHRONOUS |
														ARM_USART_DATA_BITS_8 |
														ARM_USART_PARITY_NONE |
														ARM_USART_STOP_BITS_1 |
														ARM_USART_FLOW_CONTROL_NONE,
														LPC4357_UART_BAUDRATE);
		if (res != SAMSAT_UART_EC_OK)
			return SPECIFIC_EC(res);
		
		res = UARTdrv->Control(ARM_USART_CONTROL_TX, 1);
		if (res != SAMSAT_UART_EC_OK)
			return SPECIFIC_EC(res);
		
		res = UARTdrv->Control(ARM_USART_CONTROL_RX, 1);
		if (res != SAMSAT_UART_EC_OK)
			return SPECIFIC_EC(res);
		
		return SAMSAT_UART_EC_OK;
	}
	else
	{
		return SAMSAT_UART_EC_DRIVER_NULL_ERROR;
	}
}

uint8_t samsat_uart_write(void *uart_drv, const void *data_out, uint16_t length) 
{
	int8_t res;
	uint8_t attempts=0;
	ARM_DRIVER_USART* UARTdrv = (ARM_DRIVER_USART*)uart_drv;

	if(UARTdrv!=NULL)
	{
		UART_Event = 0U;
		
		res = UARTdrv->Send(data_out, length);
		if (res != SAMSAT_UART_EC_OK)
			return COMMON_EC(res);
		
		/* Wait until transfer completed */
		while ((UART_Event & ARM_USART_EVENT_SEND_COMPLETE) == 0U && ++attempts < SAMSAT_UART_ATTEMPTS_COUNT)
			_delay_ms(1);
		
		if ((UART_Event & ARM_USART_EVENT_SEND_COMPLETE) == 0U)
			return SAMSAT_UART_EC_SEND_INCOMPLETE;
	 
		/* Clear event flags before new transfer */
		UART_Event = 0U;
		
		if(UARTdrv->GetTxCount() != length)
			return SAMSAT_UART_EC_DATA_COUNT_INCORRECT;
		
		return SAMSAT_UART_EC_OK;
	}
	else
	{
		return SAMSAT_UART_EC_DRIVER_NULL_ERROR;
	}
}

uint8_t samsat_uart_read(void *uart_drv, void *data_in, uint16_t length) 
{
	int8_t res;
	uint8_t attempts=0;
	
	ARM_DRIVER_USART* UARTdrv = (ARM_DRIVER_USART*)uart_drv;

	if(UARTdrv!=NULL)
	{
		UART_Event = 0U;
		
		res = UARTdrv->Receive(data_in, length);
		if (res != SAMSAT_UART_EC_OK)
			return COMMON_EC(res);
		
		/* Wait until receive completed */
		while ((UART_Event & ARM_USART_EVENT_RECEIVE_COMPLETE) == 0U && ++attempts < SAMSAT_UART_ATTEMPTS_COUNT)
			_delay_ms(1);
		
		if ((UART_Event & ARM_USART_EVENT_RECEIVE_COMPLETE) == 0U)
			return SAMSAT_UART_EC_RECEIVE_INCOMPLETE;
	 
		/* Clear event flags before new transfer */
		UART_Event = 0U;
		
		if(UARTdrv->GetRxCount() != length)
			return SAMSAT_UART_EC_DATA_COUNT_INCORRECT;
		
		return SAMSAT_UART_EC_OK;
	}
	else
	{
		return SAMSAT_UART_EC_DRIVER_NULL_ERROR;
	}
}