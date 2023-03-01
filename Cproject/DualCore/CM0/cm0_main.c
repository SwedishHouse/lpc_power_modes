#include "LPC43xx.h"                    // Device header
#include "RTE_Components.h"             // Component selection
//#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
//#include "I2C_LPC43xx.h"                // Keil::CMSIS Driver:I2C
#include "SCU_LPC43xx.h"                // Keil::Device:SCU
#include "RTE_Device.h"                 // Keil::Device:Startup

#define DEV_KIT_I2C

/* Custom drivers*/
//#include "samsat_i2c.h"
#include "samsat_queue.h"
#include "samsat_ipc_config.h"
#include "message_types.h"
//#include "Developer_Kit_LPC4357_rev_C.h"


/* Standart files*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>


static volatile uint32_t m0_cycle_counter;
static volatile message_t msg_cmd;
static volatile message_t msg_to_send;
static volatile message_t cmd;

/*------------------------------------------------------------------------------
  Function prototypes
 *----------------------------------------------------------------------------*/
void _delay_ms (uint32_t delay);
void M0_RITIMER_OR_WWDT_IRQHandler (void);
volatile void RIT_Config (uint32_t ticks);

/*------------------------------------------------------------------------------
  Delay 1ms
 *----------------------------------------------------------------------------*/
void _delay_ms(uint32_t delay)
{
	uint32_t startTime = m0_cycle_counter;	
	while ((uint32_t)(m0_cycle_counter - startTime) <= delay);
}

volatile void RIT_Config(uint32_t ticks)
{
	LPC_RITIMER->COMPVAL = ticks-1;	
	NVIC_SetPriority (M0_RITIMER_OR_WWDT_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
	LPC_RITIMER->MASK = 0;
  LPC_RITIMER->COUNTER = 0;
  LPC_RITIMER->CTRL    = (1 << 3) | (1 << 2) | (1 << 1);
	
	NVIC_EnableIRQ (M0_RITIMER_OR_WWDT_IRQn);                          
}

/*------------------------------------------------------------------------------
  Repetitive Interrupt Timer IRQ Handler @ 1ms
 *----------------------------------------------------------------------------*/
void M0_RITIMER_OR_WWDT_IRQHandler (void)
{	
	LPC_RITIMER->CTRL |= 1;
	m0_cycle_counter++;
}

int main()
{
		NVIC_EnableIRQ (IPC_IRQn); /* Enable IRQ from the M4 Core */
		RIT_Config(SystemCoreClock  / 1000);
		
		uint8_t counter = 0;
		const uint32_t message_size = 50;
		char result[message_size];
		uint16_t temperature = 0;
		//samsat_i2c_init(LPC4357_I2C0, 1);
		/* Turn on LEDs*/
		//Dev_Kit_LED_Color(LED_GREEN, PCA9532_ALL_LED_ON, 1);
		//_delay_ms(2000);
		//Dev_Kit_LED_Color(LED_GREEN, PCA9532_ALL_LED_OFF, 0);
	
		IPC_initMsgQueue(msg, sizeof(message_t), MSG_SIZE);
		IPC_msgNotify();
		while(1)
		{
			/* Interprocessor communication */
		if (IPC_getM4Flag() == 1U)
		{
      IPC_setM4Flag(0);
			memset(result, 0xFF, message_size);
			IPC_tryPopMsg(&msg_cmd); /* Try to read command id */
			switch (msg_cmd.id)
			{
				case GREEN_ON: 
					//result[0] = Dev_Kit_LED_Color(LED_GREEN, LED_2, 1);
					sprintf(result, "Tiger\t %c\r\n\0", (char)(0x30+counter%10));
				break;
				case GREEN_OFF:
					//result[0] = Dev_Kit_LED_Color(LED_GREEN, LED_2, 0);
					sprintf(result, "Bunny\t %u\r\n\0", (char)(0x41 + counter%25));
				break;
				case Get_Temperaure: 
					//temperature = Dev_Kit_LM75_Get_Data();
					sprintf(result, "Wolf\t %u\r\n\0", temperature);
				break;
				default:
					sprintf(result, "error %u\r\n\0", (uint8_t)'X');
					break;
			}
			
			counter++;
			counter %= UINT8_MAX;
			msg_to_send.data = result;
			msg_to_send.id = counter;
			IPC_tryPushMsg(&msg_to_send);
			IPC_msgNotify();
				
	
    }
		}
	
}
