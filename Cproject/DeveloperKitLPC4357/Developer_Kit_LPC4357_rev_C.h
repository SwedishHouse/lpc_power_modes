#ifndef _Developer_Kit_LPC4357_rev_C_
#define _Developer_Kit_LPC4357_rev_C_
#include <stdint.h>
#include <math.h>
#include <string.h>
// USART part
#ifdef DEV_KIT_USART
#include "samsat_uart.h"
#define thisUART (LPC4357_UART_DEBUG)
void Print_Uart(const char *message);
#endif

// I2C part
#ifdef DEV_KIT_I2C
#include "samsat_i2c.h"  
#include "LM75.h"  // Temperature sensor
#include "PCA9532.h"//GPIO expander with LEDs
/* LED defines	*/
//#define LED_RED 	(PCA9532_LS2)
//#define LED_GREEN (PCA9532_LS3)

#define I2C_ADDR_MMA7455 (0x3A)// Accelerometer sensor address
#define I2C_ADDR_E2PROM (0xAE)

#define LPC4357_I2C0 LPC4357_I2C_INT

enum Led_Colors
{
	LED_RED=PCA9532_LS2,
	LED_GREEN
};

enum LED_State{
	LED_OFF,
	LED_ON,
	LED_PWM0,
	LED_PWM1
};

enum LED_Nums{
	LED_0=0,
	LED_1,
	LED_2,
	LED_3
};


typedef struct valActionLED
{
	uint8_t color;
	uint8_t number;
	uint8_t state;
}valActionLED_t;

uint8_t Dev_Kit_LED(uint8_t *data);


/*! 
	Red colors connected to LED output 8,9, 10, 11
	Green LED connected to LED output 12, 13, 14, 15 
	\param led_num in range  from 0 to 3
	\param led_selector Must be equal LS2 or LS3, because LED connected only to them
					LS2 corresponds red color
					LS3 corresponds green color
	\param state - 1 is turn up, 0 - turn down
*/
uint8_t Dev_Kit_LED_Color(uint8_t led_color,uint8_t led_num, uint8_t state);
int16_t Dev_Kit_LM75_Get_Data();

#endif

#endif
