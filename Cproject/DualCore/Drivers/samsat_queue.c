#include "samsat_queue.h"
#include "samsat_ipc_config.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* * IRQ priority of IPC interrupt
   * Currently this has the highest priority
 */ 
#define IPC_IRQ_Priority    IRQ_PRIO_IPC

#ifdef CORE_M4

static volatile uint8_t M0RespFlag, M4Flag;
	
static volatile ipc_queue_t *qrd = (ipc_queue_t *)SAMSAT_IPC_SHARED_MEM_M0;		/*!< Shared memory area of M0. Used for reading*/
static volatile ipc_queue_t *qwr = (ipc_queue_t *)SAMSAT_IPC_SHARED_MEM_M4;		/*!< Shared memory area of M4. Used for writing*/

uint8_t IPC_getM0RespFlag()
{
	return M0RespFlag;
}

void IPC_setM0RespFlag(uint8_t flag)
{
	M0RespFlag = flag;
}

uint8_t IPC_getM4Flag()
{
	return M4Flag;
}

void IPC_setM4Flag(uint8_t flag)
{
	M4Flag = flag;
}

#elif defined(CORE_M0)

static volatile uint8_t M4Flag;

static volatile ipc_queue_t *qrd = (ipc_queue_t *)SAMSAT_IPC_SHARED_MEM_M4;		/*!< Shared memory area of M4. Used for writing*/
static volatile ipc_queue_t *qwr = (ipc_queue_t *)SAMSAT_IPC_SHARED_MEM_M0;		/*!< Shared memory area of M0. Used for reading*/

uint8_t IPC_getM4Flag()
{
	return M4Flag;
}

void IPC_setM4Flag(uint8_t flag)
{
	M4Flag = flag;
}

#else
#error "For LPC43XX, CORE_M0 or CORE_M4 must be defined!"
#endif

/**
 * @brief	Interrupt handler for IPC interrupts
 * @return	None
 */
void IPC_IRQHandler(void)
{
	#ifdef CORE_M4
		LPC_CREG->M0APPTXEVENT = 0;
		IPC_setM0RespFlag(1);
	#elif defined(CORE_M0)
		LPC_CREG->M4TXEVENT = 0;
		IPC_setM4Flag(1);
	#endif
}

/* Wait for the event */
#define ipc_wait_event(evt) while ((evt))

#define ipc_wait_event_tout(evt, tout)	\
	do { \
		uint32_t cnt = LPC_RITIMER->COUNTER + (tout * (SystemCoreClock / 1000)); \
		if (cnt + 5000 < cnt) {cnt += 5000; } \
		while ((evt) && LPC_RITIMER->COUNTER < cnt) {}	 \
		if (evt) {tout = 0; } \
	} while (0)

/*
 * Initiate interrupt on other processor
 * Upon calling this function generates and interrupt on the other
 * core. Ex. if called from M0 core it generates interrupt on M4 core
 * and vice versa.
 */
static void ipc_send_signal(void)
{
	__DSB();
	__SEV();
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Function to send notificaton interrupt */
void IPC_msgNotify(void)
{
	ipc_send_signal();
}

/* Function to initialize the IPC message queue */
void IPC_initMsgQueue(void *data, int size, int count){
	/* Sanity Check */
	if (!size || !count || !data) {
		/* Handle error */
		while (1) {}
	}

	/* Check if size is a power of 2 */
	if (count & (count - 1)) {
		/* Handle error */ 
		while (1){}/* BUG: Size must always be power of 2 */
		}	
	
	memset(qwr,0, sizeof(*qwr));	
	qwr->count = count;
	qwr->size = size;
	qwr->data = data;
	qwr->valid = QUEUE_MAGIC_VALID;
		
	NVIC_SetPriority(IPC_IRQn, SAMSAT_IPC_IRQ_PRIO);
	NVIC_EnableIRQ(IPC_IRQn);
}

/* Function to push a message into queue with timeout */
int IPC_pushMsgTout(const void *data, int tout)
{
	/* Check if write queue is initialized */
	if (!QUEUE_IS_VALID(qwr)) {
		return QUEUE_ERROR;
	}

	if (tout == 0) {
		/* Check if queue is full */
		if (QUEUE_IS_FULL(qwr)) {
			return QUEUE_FULL;
		}
	}
	else if (tout < 0) {
		/* Wait for read queue to have some data */
		ipc_wait_event(QUEUE_IS_FULL(qwr));
	}
	else {
		/* Wait for read queue to have some data */
		ipc_wait_event_tout(QUEUE_IS_FULL(qwr), tout);
		if (tout == 0) {
			return QUEUE_TIMEOUT;
		}
	}
	
	/* Push the queue Item */
	memcpy(qwr->data + ((qwr->head & (qwr->count - 1)) * qwr->size), data, qwr->size);
	
	qwr->head++;

	return QUEUE_INSERT;
}

/* Function to read a message from queue with timeout */
int IPC_popMsgTout(void *data, int tout)
{
	if (!QUEUE_IS_VALID(qrd)) {
		return QUEUE_ERROR;
	}

	if (tout == 0) {
		/* Check if read queue is empty */
		if (QUEUE_IS_EMPTY(qrd)) {
			return QUEUE_EMPTY;
		}
	}
	else if (tout < 0) {
		/* Wait for read queue to have some data */
		ipc_wait_event(QUEUE_IS_EMPTY(qrd));
	}
	else {
		/* Wait for event or timeout */
		ipc_wait_event_tout(QUEUE_IS_EMPTY(qrd), tout);
		if (tout == 0) {
			return QUEUE_TIMEOUT;
		}
	}

	/* Pop the queue Item */
	memcpy(data, qrd->data + ((qrd->tail & (qrd->count - 1)) * qrd->size), qrd->size);
	
	qrd->tail++;

	return QUEUE_VALID;
}

/* Get number of pending items in queue */
int IPC_msgPending(int queue_write)
{
	ipc_queue_t *q = queue_write ? qwr : qrd;
	if (!QUEUE_IS_VALID(q))
		return QUEUE_ERROR;

	return QUEUE_DATA_COUNT(q);
}

/* Function to convert IPC error number to string */
const char *IPC_strerror(int errnum)
{
	static const char *ipc_errstr[] = {
		"Queue Insert OK",
		"Queue Pop OK/Valid",
		"Queue is Full",
		"Queue is Empty",
		"Queue Error/Not initialized",
		"Queue operation timed out",
	};
	if (errnum < 0) {
		errnum = 1 - errnum;
	}
	return ipc_errstr[errnum];
}

