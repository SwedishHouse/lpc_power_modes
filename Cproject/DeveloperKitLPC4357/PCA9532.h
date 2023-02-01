// This is I2C-GPIO expander

#ifndef _PCA9532_
#define _PCA9532_

/*		Address I2C periph
without read/write operation
*/

#define I2C_ADDR_PCA9532 (0xC0>>1)//

/*
The information about PCA9532
**************************************
**************************************
6.3.7		LS0-LS3 - LED selector register
The LSn LED selector registers determine the source of the LED data.
00 = output is set high-impedance (LED off; default)
01 = output is set LOW (LED on)
10 = output blinks at PWM0 rate
11 = output blinks at PWM1 rate
*************************************
LS0 - LED0 to LED3 selector
Register	|Bit	|Value 	|Description

LS0	 			7:6 	00* 		LED3 selected
					5:4 	00* 		LED2 selected
					3:2 	00* 		LED1 selected
					1:0 	00* 		LED0 selected
*************************************
LS1 - LED4 to LED7 selector
LS2 - LED8 to LED11 selector
LS3 - LED12 to LED15 selector
**************************************
**************************************
6.4		 Pins used as GPIOs
LEDn pins not used to control LEDs can be used as General Purpose I/Os (GPIOs).
For use as input, set LEDn to high-impedance (00) and then read the pin state via the
INPUT0 or INPUT1 register.
For use as output, connect external pull-up resistor to the pin and size it according to the
DC recommended operating characteristics. LEDn output pin is HIGH when the output is
programmed as high-impedance, and LOW when the output is programmed LOW through
the ‘LED selector’ register. The output can be pulse-width controlled when PWM0 or
PWM1 are used.
*/

/*
Control Register PCA9532
*/
#define PCA9532_R_INPUT0 	(0x0) //Read only
#define PCA9532_R_INPUT1 	(0x1)	//Read only
#define PCA9532_R_PSC0		(0x2)
#define PCA9532_R_PWM0 		(0x3)
#define PCA9532_R_PSC1		(0x4)
#define PCA9532_R_PWM2 		(0x5)
/*LS*/
#define PCA9532_LS0 			(0x6)
#define PCA9532_LS1 			(0x7)
#define PCA9532_LS2 			(0x8)
#define PCA9532_LS3 			(0x9)
#define PCA9532_AI_SET		(0x1<<4)
#define PCA9532_AI_OFF		(~PCA9532_AI_SET)

/*Next for LED MODE Select
6.3.7 LS0 to LS3 - LED selector registers
The LSn LED selector registers determine the source of the LED data.
00 = output is set high-impedance (LED off; default)
01 = output is set LOW (LED on)
10 = output blinks at PWM0 rate
11 = output blinks at PWM1 rate
*/
#define PCA9532_LED_IMPEDANCE (0x0)
#define PCA9532_LED_LOW (0x1)
#define PCA9532_LED_PWM0 (0x2)
#define PCA9532_LED_PWM1 (0x3)
/*Turn on*/
#define PCA9532_ALL_LED_ON (0x55)
#define PCA9532_ALL_LED_OFF (0x0)

#define PCA9532_BIT0 (1<<0)
#define PCA9532_BIT1 (1<<1)
#define PCA9532_BIT2 (1<<2)
#define PCA9532_BIT3 (1<<3)
#define PCA9532_BIT4 (1<<4)
#define PCA9532_BIT5 (1<<5)
#define PCA9532_BIT6 (1<<6)
#define PCA9532_BIT7 (1<<7)

//enum LED_Number
//{
//	LED_0=0
//};

#endif
