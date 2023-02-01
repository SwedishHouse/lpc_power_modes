/*! \file samsat_uart.h
    \brief Driver for uart on LPC4357
*/
#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include "USART_LPC43xx.h"

extern ARM_DRIVER_USART 		Driver_USART0;
extern ARM_DRIVER_USART 		Driver_USART3;

#define LPC4357_UART_ICC 		((ARM_DRIVER_USART*)(&Driver_USART0)) /*!< Hardware module Driver_USART0 */
#define LPC4357_UART_DEBUG 	((ARM_DRIVER_USART*)(&Driver_USART3)) /*!< Hardware module Driver_USART1 */

typedef enum
{
	SAMSAT_UART_EC_OK = 0,
	SAMSAT_UART_EC_UNSPECIFIED_ERROR,
	SAMSAT_UART_EC_BUSY,
	SAMSAT_UART_EC_TIMEOUT,
	SAMSAT_UART_EC_OPERATION_UNSUPPORTED,
	SAMSAT_UART_EC_PARAMETER_ERROR,
	SAMSAT_UART_EC_MODE,
	SAMSAT_UART_EC_BAUDRATE,
	SAMSAT_UART_EC_DATA_BITS,
	SAMSAT_UART_EC_PARITY,
	SAMSAT_UART_EC_STOP_BITS,
	SAMSAT_UART_EC_FLOW_CONTROL,
	SAMSAT_UART_EC_CPOL,
	SAMSAT_UART_EC_CPHA,
	SAMSAT_UART_EC_SEND_INCOMPLETE,
	SAMSAT_UART_EC_RECEIVE_INCOMPLETE,
	SAMSAT_UART_EC_DATA_COUNT_INCORRECT,
	SAMSAT_UART_EC_DRIVER_NULL_ERROR
} SAMSAT_UART_EC_t;

typedef enum
{
	SAMSAT_UART_KEYWORD_M0_TEST	=	0x30,
	SAMSAT_UART_KEYWORD_M4_TEST	=	0x34,
	
	SAMSAT_UART_KEYWORD_ACK			= 0x06,
	SAMSAT_UART_KEYWORD_NACK		= 0x15,
	
	SAMSAT_UART_KEYWORD_START		=	0x02,
	SAMSAT_UART_KEYWORD_STOP		=	0x03,
} SAMSAT_UART_KEYWORD_t;


#define LPC4357_UART_BAUDRATE											115200

/*! \fn uart_init(void *uart_drv)
    \brief Init uart
	\param uart_drv UART driver UART0 or UART1
*/
uint8_t samsat_uart_init(void *uart_drv);

/*! \fn uart_send(void *uart_drv, const void *data)
	\brief Send data by uart
	\param uart_drv UART driver UART0 or UART1
	\param data Input buffer to send
*/
uint8_t samsat_uart_write(void *uart_drv, const void *data, uint16_t length);

/*! \fn samsat_uart_read(void *uart_drv, void *data)
	\brief Receive data by uart
	\param uart_drv UART driver UART0 or UART1
	\param data Output buffer to receive
*/
uint8_t samsat_uart_read(void *uart_drv, void *data, uint16_t length);

uint8_t samsat_uart_debug(void *data);

#endif  /* __UART_H */
