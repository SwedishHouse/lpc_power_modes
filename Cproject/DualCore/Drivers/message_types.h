#ifndef __MESSAGE_TYPES__
#define  __MESSAGE_TYPES__

#include <stdint.h>



enum LED_Commands
{
	RED_ON = 0,
	RED_OFF,
	GREEN_ON,
	GREEN_OFF,
	Get_Temperaure
};

typedef struct
{
	uint8_t id;
	uint32_t *data;
} message_t;

#define MSG_SIZE 32
volatile message_t msg[MSG_SIZE];

#endif
