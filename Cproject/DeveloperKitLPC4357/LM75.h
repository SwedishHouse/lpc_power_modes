//Temperature sensor. I2C_interface
#ifndef _LM75_
#define _LM75_
#include <stdint.h>
//For calculating sign of temperature
#define BIT7 (0x1<<0x7)
#define I2C_ADDR_LM75 (0x91>>1) //Temperature sensor address
/*	7.4 Register list	*/
#define LM75_REG_CONF 	(0x1)//Configuration register R/W
#define LM75_REG_TEMP 	(0x0)//Temperature register, read only
#define LM75_REG_TOS		(0x3)//Overtemperature shutdown treshold register, R/W
#define LM75_REG_THYST	(0x2)//Hysteresis register

/*7.4.2 Configuration register*/




#endif
