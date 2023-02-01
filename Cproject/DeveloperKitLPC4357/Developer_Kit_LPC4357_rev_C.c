#include "Developer_Kit_LPC4357_rev_C.h"
#include <stdint.h>

#ifdef DEV_KIT_USART
#define LENGTH_OF_STRING 150
char string_to_print[LENGTH_OF_STRING];




void Print_Uart(const char *message)
{
	uint32_t size = strlen(message);
	memcpy(string_to_print,message, size);
	
	samsat_uart_write(thisUART,string_to_print, size);
}

#endif

#ifdef DEV_KIT_I2C
uint8_t Dev_Kit_LED(uint8_t *data)
{
	return samsat_i2c_master_write((void *)LPC4357_I2C0,I2C_ADDR_PCA9532,data,2);
}

uint8_t Dev_Kit_LED_Color(uint8_t led_color,uint8_t led_num, uint8_t state)
{
	if (led_color<PCA9532_LS2 ||led_color>PCA9532_LS3)
		return 0xFF;
	uint8_t send[2];
	send[0]=led_color;
	switch(led_num)
	{
		case PCA9532_ALL_LED_ON:
			send[1]=PCA9532_ALL_LED_ON;
		break;
		case PCA9532_ALL_LED_OFF:
			send[1]=PCA9532_ALL_LED_OFF;
		break;
		default:
			if(led_num>0&&led_num<5)
				send[1]=(state<<(2*led_num));
			else
				return 0xFF;
			break;
	}
	return	Dev_Kit_LED(send);
}

void Dev_Kit_I2C_Buttons()
{


}

int16_t Dev_Kit_LM75_Get_Data()
{
	uint8_t data[2];
	uint8_t status[2];
	uint8_t write_op=LM75_REG_TEMP;
	status[0]=samsat_i2c_master_read((void*)LPC4357_I2C0,I2C_ADDR_LM75,&write_op,1);
	if (status[0]!=0)
		return -1000;
	status[1] = samsat_i2c_master_read((void*)LPC4357_I2C0,I2C_ADDR_LM75,data,2);
	if (status[1]!=0)
		return -1000;
	int16_t temperature=0;
	temperature |= (data[1]>>5);
	temperature |= (data[0]<<3);
//	temperature |= ((data[0]|~(1<<7))<<5);
//	temperature |= (data[0]|(1<<7))<<8;
	
	return temperature;
}
#endif