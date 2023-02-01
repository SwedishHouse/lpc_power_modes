#ifndef _SAMSAT_IPC_CONFIG_
#define _SAMSAT_IPC_CONFIG_

#include <stdint.h>

#define SAMSAT_IPC_SHARED_MEM_M0        	0x10089F80 	/*!< From  0x10089F80 to 0x10089FC0 - 64 bytes length */
#define SAMSAT_IPC_SHARED_MEM_M4        	0x10089FC0 	/*!< From  0x10089FC0 to 0x1008A000 - 64 bytes length */

#define SAMSAT_IPC_QUEUE_SIZE        			64  				/*!< Maximum number of queue elements */
#define SAMSAT_IPC_IRQ_PRIO         			7

#define SAMSAT_IPC_DATA_TEMPERATURE_SIZE	10					/* From SamSatIonConfig_ADCS.h */
#define SAMSAT_IPC_DATA_CURRENTS_SP_SIZE	8						/* From SamSatIonConfig_ADCS.h */
#define SAMSAT_IPC_DATA_LIGHT_COLOR_SIZE	48					/* From SamSatIonConfig_ADCS.h */
#define SAMSAT_IPC_DATA_LIGHT_IR_SIZE 		32					/* From SamSatIonConfig_ADCS.h */
#define SAMSAT_IPC_DATA_CURRENTS_PWM_SIZE	6						/* From SamSatIonConfig_ADCS.h */
#define SAMSAT_IPC_DATA_MAGN_MMC_SIZE			14					/* From samsat_MMC5983.h */
#define SAMSAT_IPC_DATA_MPU_SIZE					60					/* From samsat_MPU9255.h */
#define SAMSAT_IPC_DATA_GYROSCOPE_SIZE		12					/* From samsat_ADXRS290.h */
#define SAMSAT_IPC_DATA_QW_EKF_SIZE				10
#define SAMSAT_IPC_DATA_QW_QUEST_SIZE			10
#define SAMSAT_IPC_DATA_BDOT_SIZE					3						/* From SamSatIonConfig_ADCS.h */
#define SAMSAT_IPC_DATA_TIME_SIZE					4
#define SAMSAT_IPC_DATA_HIL_SIZE					1

#define SAMSAT_IPC_MSG_ID_SHIFT						20
#define SAMSAT_IPC_COMMAND								40

#define SAMSAT_IPC_DATA_SIZES_LIST (uint8_t[]){	SAMSAT_IPC_DATA_TEMPERATURE_SIZE, \
																								SAMSAT_IPC_DATA_CURRENTS_SP_SIZE, \
																								SAMSAT_IPC_DATA_LIGHT_COLOR_SIZE, \
																								SAMSAT_IPC_DATA_LIGHT_IR_SIZE, \
																								SAMSAT_IPC_DATA_CURRENTS_PWM_SIZE, \
																								SAMSAT_IPC_DATA_MAGN_MMC_SIZE, \
																								SAMSAT_IPC_DATA_MPU_SIZE, \
																								SAMSAT_IPC_DATA_GYROSCOPE_SIZE, \
																								SAMSAT_IPC_DATA_QW_EKF_SIZE, \
																								SAMSAT_IPC_DATA_QW_QUEST_SIZE, \
																								SAMSAT_IPC_DATA_BDOT_SIZE, \
																								SAMSAT_IPC_DATA_TIME_SIZE }

/* CM4 message structure */
typedef struct
{
	uint8_t id;
	uint32_t *data;
} samsat_ipc_msg_t;

/* CM4 command IDs */
typedef enum
{
	SAMSAT_IPC_COMMAND_ID_NONE = -1,
	
	SAMSAT_IPC_COMMAND_ID_GET_DATA_TEMPERATURE = 0,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_CURRENTS_SP = 1,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_LIGHT_COLOR = 2,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_LIGHT_IR = 3,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_CURRENTS_PWM = 4,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_MAGN_MMC = 5,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_MPU = 6,
	SAMSAT_IPC_COMMAND_ID_GET_DATA_GYROSCOPE = 7,
	
	SAMSAT_IPC_COMMAND_ID_POST_DATA_QW_EKF = 8,
	SAMSAT_IPC_COMMAND_ID_POST_DATA_QW_QUEST = 9,
	SAMSAT_IPC_COMMAND_ID_POST_DATA_BDOT = 10,
	
	SAMSAT_IPC_COMMAND_ID_POST_DATA_TIME = 11,
	
	SAMSAT_IPC_COMMAND_ID_POST_HIL_TEST = 12
	
} SAMSAT_IPC_COMMAND_ID_t;

/* Queue message IDs */
typedef enum
{
	/* Start request from 0 */
	SAMSAT_IPC_MSG_ID_GET_DATA_TEMPERATURE_REQUEST = 0,
	SAMSAT_IPC_MSG_ID_GET_DATA_CURRENTS_SP_REQUEST = 1,
	SAMSAT_IPC_MSG_ID_GET_DATA_LIGHT_COLOR_REQUEST = 2,
	SAMSAT_IPC_MSG_ID_GET_DATA_LIGHT_IR_REQUEST = 3,
	SAMSAT_IPC_MSG_ID_GET_DATA_CURRENTS_PWM_REQUEST = 4,
	SAMSAT_IPC_MSG_ID_GET_DATA_MAGN_MMC_REQUEST = 5,
	SAMSAT_IPC_MSG_ID_GET_DATA_MPU_REQUEST = 6,
	SAMSAT_IPC_MSG_ID_GET_DATA_GYROSCOPE_REQUEST = 7,
	
	SAMSAT_IPC_MSG_ID_POST_DATA_QW_EKF_REQUEST = 8,
	SAMSAT_IPC_MSG_ID_POST_DATA_QW_QUEST_REQUEST = 9,
	SAMSAT_IPC_MSG_ID_POST_DATA_BDOT_REQUEST = 10,
	
	SAMSAT_IPC_MSG_ID_POST_DATA_TIME_REQUEST = 11,
	
	SAMSAT_IPC_MSG_ID_POST_DATA_HIL_TEST_REQUEST = 12,
	
	/* Start response from 20 (difference between response and request) */
	SAMSAT_IPC_MSG_ID_GET_DATA_TEMPERATURE_RESPONSE = 20,
	SAMSAT_IPC_MSG_ID_GET_DATA_CURRENTS_SP_RESPONSE = 21,
	SAMSAT_IPC_MSG_ID_GET_DATA_LIGHT_COLOR_RESPONSE = 22,
	SAMSAT_IPC_MSG_ID_GET_DATA_LIGHT_IR_RESPONSE = 23,
	SAMSAT_IPC_MSG_ID_GET_DATA_CURRENTS_PWM_RESPONSE = 24,
	SAMSAT_IPC_MSG_ID_GET_DATA_MAGN_MMC_RESPONSE = 25,
	SAMSAT_IPC_MSG_ID_GET_DATA_MPU_RESPONSE = 26,
	SAMSAT_IPC_MSG_ID_GET_DATA_GYROSCOPE_RESPONSE = 27,
	
	SAMSAT_IPC_MSG_ID_POST_DATA_HIL_TEST_RESPONSE = 32,
} SAMSAT_IPC_MSG_ID_t;

/* Message structures */
typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_TEMPERATURE_SIZE];
} SAMSAT_IPC_GET_DATA_TEMPERATURE_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_CURRENTS_SP_SIZE];
} SAMSAT_IPC_GET_DATA_CURRENTS_SP_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_LIGHT_COLOR_SIZE];
} SAMSAT_IPC_GET_DATA_LIGHT_COLOR_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_LIGHT_IR_SIZE];
} SAMSAT_IPC_GET_DATA_LIGHT_IR_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_CURRENTS_PWM_SIZE];
} SAMSAT_IPC_GET_DATA_CURRENTS_PWM_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_MAGN_MMC_SIZE];
} SAMSAT_IPC_GET_DATA_MAGN_MMC_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_MPU_SIZE];
} SAMSAT_IPC_GET_DATA_MPU_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_GYROSCOPE_SIZE];
} SAMSAT_IPC_GET_DATA_GYROSCOPE_RESPONSE_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_QW_EKF_SIZE];
} SAMSAT_IPC_POST_DATA_QW_EKF_REQUEST_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_QW_QUEST_SIZE];
} SAMSAT_IPC_POST_DATA_QW_QUEST_REQUEST_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_BDOT_SIZE];
} SAMSAT_IPC_POST_DATA_BDOT_REQUEST_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_TIME_SIZE];
} SAMSAT_IPC_POST_DATA_TIME_REQUEST_t;

typedef struct
{
	uint8_t data[SAMSAT_IPC_DATA_HIL_SIZE];
} SAMSAT_IPC_POST_DATA_HIL_TEST_REQUEST_t;

/* Command structure */
typedef struct
{
	SAMSAT_IPC_COMMAND_ID_t id;
} SAMSAT_IPC_COMMAND_t;

/* Structure of message structures */
typedef union
{
	struct
	{		
		SAMSAT_IPC_GET_DATA_TEMPERATURE_RESPONSE_t	get_temperature;
		SAMSAT_IPC_GET_DATA_CURRENTS_SP_RESPONSE_t	get_currents_sp;
		SAMSAT_IPC_GET_DATA_LIGHT_COLOR_RESPONSE_t	get_light_color;
		SAMSAT_IPC_GET_DATA_LIGHT_IR_RESPONSE_t			get_light_ir;
		SAMSAT_IPC_GET_DATA_CURRENTS_PWM_RESPONSE_t	get_currents_pwm;
		SAMSAT_IPC_GET_DATA_MAGN_MMC_RESPONSE_t			get_magn_mmc;
		SAMSAT_IPC_GET_DATA_MPU_RESPONSE_t					get_mpu;
		SAMSAT_IPC_GET_DATA_GYROSCOPE_RESPONSE_t		get_gyroscope;
		
		SAMSAT_IPC_POST_DATA_QW_EKF_REQUEST_t				post_qw_ekf;
		SAMSAT_IPC_POST_DATA_QW_QUEST_REQUEST_t			post_qw_quest;
		SAMSAT_IPC_POST_DATA_BDOT_REQUEST_t					post_bdot;
		
		SAMSAT_IPC_POST_DATA_TIME_REQUEST_t					post_time;
		
		SAMSAT_IPC_POST_DATA_HIL_TEST_REQUEST_t			post_hil;
	};
	uint8_t array[1000]; // https://stackoverflow.com/questions/36137050/accessing-a-struct-by-index
} SAMSAT_IPC_MESSAGES_t;

inline static uint16_t samsat_ipc_data_start_index(const uint8_t index)
{
	uint16_t res = 0;
	for (uint8_t i=0; i<index; i++) 
		res += SAMSAT_IPC_DATA_SIZES_LIST[i];
	return res;
}

#endif