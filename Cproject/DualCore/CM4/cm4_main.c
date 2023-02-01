#include "LPC43xx.h"                    // Device header
#include "RTE_Components.h"             // Component selection
#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "USART_LPC43xx.h"              // Keil::CMSIS Driver:USART
#include "SCU_LPC43xx.h"                // Keil::Device:SCU
#include "RTE_Device.h"                 // Keil::Device:Startup

#include "message_types.h"
#include "samsat_queue.h"
#include "samsat_ipc_config.h"
#include "Developer_Kit_LPC4357_rev_C.h"
#include <stdio.h>
#include <string.h>
#include "samsat_uart.h"

const                                   /* Place the CM0 image in CM4 ROM     */
#include "M0_Image.c"                  /* Cortex M0 image reference          */
volatile uint32_t m4_cycle_counter;
#define line_size 100
volatile char line[line_size];
volatile message_t msg_m4;
//#define LINE_LENGTH 100
//static char line[LINE_LENGTH];

/* Function prototypes */
void _delay_ms(uint32_t delay);
void Load_CM0_Image (uint32_t DestAddr, const uint8_t *Image, uint32_t Sz);

/* Function definitions*/

void _delay_ms(uint32_t delay)
{
	uint32_t startTime = m4_cycle_counter;
	while ((uint32_t)(m4_cycle_counter - startTime) <= delay);
}

/*------------------------------------------------------------------------------
  SysTick Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
	m4_cycle_counter++;
}

/*------------------------------------------------------------------------------
  Load Cortex M0APP Application Image
 *----------------------------------------------------------------------------*/
void Load_CM0_Image (uint32_t DestAddr, const uint8_t *Image, uint32_t Sz)
{
  uint32_t i;
  uint8_t *dp = (uint8_t *)DestAddr;

  /* Copy application image */
  for (i = 0; i < Sz; i++) {
    dp[i] = Image[i];
  }
  /* Set shadow pointer to beginning of the CM0 application */
  LPC_CREG->M0APPMEMMAP = DestAddr;
}

void Receive_signal_from_CM0()
{
	IPC_setM4Flag(1);
	
	if (IPC_getM0RespFlag() == 1U)
		IPC_setM0RespFlag(0);
}

int main()
{
	SystemInit();
	
	SysTick_Config(SystemCoreClock  / 1000);
	SystemCoreClockUpdate();
	samsat_uart_init(thisUART);
	NVIC_EnableIRQ (IPC_IRQn);
	IPC_setM4Flag(1);
	Print_Uart("M4 starts and \r\n\0");
	/* Download CM0 application */
	
	Print_Uart("Stop M0\r\n\0");
	LPC_RGU->RESET_CTRL1 = (1 << 24);
  Load_CM0_Image (0x10080000, LR0, sizeof (LR0));
	
	/* Init write queue */
	IPC_initMsgQueue(msg, sizeof(message_t), MSG_SIZE);
	
	/* Start CM0 core */
	Print_Uart("M4 starts M0\r\n\0)");
  LPC_RGU->RESET_CTRL1 = 0;
	/* CM0 initializaton complited */
	while (1) { if (IPC_getM0RespFlag() == 1U) { IPC_setM0RespFlag(0); break; } }
	static int i = 0, j =0, counter = 0;

	/* CM0 initializaton complited */
	//IPC_setM4Flag(0);
	uint8_t cmd = 0;
	while (1)
	{ 
		
		switch(i % 3)
		{
			case 0:
				cmd = GREEN_ON;
			break;
			case 1:
				cmd = GREEN_OFF;
			break;
			case 2:
				cmd = Get_Temperaure;
			break;
		}
		msg_m4.id = cmd;
		
		if (IPC_getM4Flag() == 1U)
		{
			IPC_setM4Flag(0);
			IPC_tryPushMsg(&msg_m4);
			IPC_msgNotify();
		}
		
		IPC_setM4Flag(1);
	
		if (IPC_getM0RespFlag() == 1U)
			IPC_setM0RespFlag(0);
		
		//IPC_setM4Flag(0);
		while(IPC_getM0RespFlag() == 0U)
				_delay_ms(1);

		IPC_setM4Flag(1);
		if (IPC_getM0RespFlag() == 1U) 
			{	
				IPC_setM0RespFlag(0);
				//Print_Uart("Response from M0\r\n\0");
				IPC_tryPopMsg(&msg_m4);
				sprintf(line, "Counter = %u\t\0", msg_m4.id);
				Print_Uart(line);
				memset(line, 0, line_size*sizeof(char));
				memcpy(line, msg_m4.data, 50 * sizeof(char));
				Print_Uart(line);
			}
		_delay_ms(800);
		i++;
		counter++;
	}	

}
