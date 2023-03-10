#ifndef _SAMSAT_QUEUE_
#define _SAMSAT_QUEUE_

#include <string.h>

#include <stdint.h>
#include "LPC43xx.h"

#ifdef CORE_M4

#define IPC_IRQHandler				M0APP_IRQHandler
#define IPC_IRQn							M0APP_IRQn

/*! \fn void IPC_setM0RespFlag(uint8_t flag)
		\brief set the value of variable that defined as M0_EventResp
		\return none
*/
void IPC_setM0RespFlag(uint8_t flag);

/*! \fn uint32_t IPC_getM0RespFlag(void)
		\brief get the value of variable that defined as M0_EventResp
		\return M0_EventResp value
*/
uint8_t IPC_getM0RespFlag(void);

/*! \fn void IPC_setM4Flag(uint8_t flag)
		\brief set the value of variable that defined as M4_Event
		\return none
*/
void IPC_setM4Flag(uint8_t flag);

/*! \fn uint32_t IPC_getM4Flag(void)
		\brief get the value of variable that defined as M4_Event
		\return M4_Event value
*/
uint8_t IPC_getM4Flag(void);

#elif defined(CORE_M0)

#define IPC_IRQHandler			M0_M4CORE_IRQHandler
#define IPC_IRQn						M0_M4CORE_IRQn

/*! \fn void IPC_setM4Flag(uint8_t flag)
		\brief set the value of variable that defined as M4_Event
		\return none
*/
void IPC_setM4Flag(uint8_t flag);

/*! \fn uint32_t IPC_getM4Flag(void)
		\brief get the value of variable that defined as M4_Event
		\return M4_Event value
*/
uint8_t IPC_getM4Flag(void);

#endif

/* Queue status macro */
/**
 * \def QUEUE_DATA_COUNT(q)
 * This macro will get the number of items pending in \a q
 */
#define QUEUE_DATA_COUNT(q) ((uint32_t) ((q)->head - (q)->tail))

/**
 * \def QUEUE_IS_FULL(q)
 * This macro will evaluate to 1 if queue \a q is full, 0 if it is not
 */
#define QUEUE_IS_FULL(q)    (QUEUE_DATA_COUNT(q) >= (q)->count)

/**
 * \def QUEUE_IS_EMPTY(q)
 * This macro will evaluate to 1 if queue \a q is empty, 0 if it is not
 */
#define QUEUE_IS_EMPTY(q)   ((q)->head == (q)->tail)

/**
 * \def QUEUE_IS_VALID(q)
 * This macro will evaluate to 1 if queue \a q is initialized & valid, 0 if it is not
 */
#define QUEUE_IS_VALID(q)   ((q)->valid == QUEUE_MAGIC_VALID)

/**
 * @brief IPC Queue Structure used for sync between M0 and M4.
 *
 * This structure provides head and tail indexes
 * to the uint32_t array of messages.
 */
typedef struct
{
	uint8_t* data;				/*!< Pointer to the data */
	int32_t size;				/*!< Size of a single item in queue */
	int32_t count;				/*!< Total number of elements that can be stored in the queue */
	volatile uint32_t head;		/*!< Head index of the queue */
	volatile uint32_t tail;		/*!< Tail index of the queue */
	uint32_t valid;             /*!< Queue is valid only if this is #QUEUE_MAGIC_VALID */
	uint32_t reserved[2];		/*!< Reserved entry to keep the structure aligned */
} ipc_queue_t;

/* IPC Function return values */
/**
 * \def QUEUE_VALID
 * Returned by dualcore IPC function when an item is 
 * successfully fetched from Message Queue
 */
#define QUEUE_VALID      1

/**
 * \def QUEUE_INSERT
 * Returned by dualcore IPC function when an item is
 * successfully inserted into the Message Queue
 */
#define QUEUE_INSERT     0

/**
 * \def QUEUE_FULL
 * Returned by dualcore IPC function when an attempt is
 * made to insert item into the Message Queue when the
 * queue is full.
 */
#define QUEUE_FULL      -1

/**
 * \def QUEUE_EMPTY
 * Returned by dualcore IPC function when an attempt is
 * made to fetch/pop an item from the Message Queue when the
 * queue is empty.
 */
#define QUEUE_EMPTY     -2

/**
 * \def QUEUE_ERROR
 * Returned by dualcore IPC function when an push/pop is made
 * to a queue which was not initialized using IPC_initMsgQueue()
 */
#define QUEUE_ERROR     -3

/**
 * \def QUEUE_TIMEOUT
 * Returned by dualcore IPC function when a push/pop operation
 * did not complete within a given time.
 */
#define QUEUE_TIMEOUT   -4

/**
 * \def QUEUE_MAGIC_VALID
 * Macro used to identify if a queue is valid
 */
#define QUEUE_MAGIC_VALID   0xCAB51053

/**
 * @brief	Function to push a message into queue with timeout
 *
 * This function will push an message of size \a size, specified by
 * IPC_initMsgQueue. If this function is called from M4 Core the message
 * will be pushed to M4 Queue and will be popped by M0, and vice-versa.
 *
 * @param	data	: Pointer to data to be pushed
 * @param	tout	: non-zero value - timeout value in milliseconds,
 *                      zero value - no blocking,
 *                      negative value - blocking
 * @return  #QUEUE_INSERT on success,
 * @note	#QUEUE_FULL or #QUEUE_ERROR on failure,
 *          #QUEUE_TIMEOUT when there is a timeout
 */
int IPC_pushMsgTout(const void *data, int tout);

/**
 * @brief	Function to read a message from queue with timeout
 *
 * This function will pop a message of size \a size, specified by
 * IPC_initMsgQueue of the other core. If this function is called from
 * M4 Core the message will be popped from M0 Queue and vice-versa.
 *
 * @param	data	: Pointer to store popped data
 * @param	tout	: non-zero value - timeout value in milliseconds,
 *                      zero value - no blocking,
 *                      negative value - blocking
 * @return	#QUEUE_VALID on success,
 * @note	#QUEUE_EMPTY or #QUEUE_ERROR on failure,
 *          #QUEUE_TIMEOUT when there is a timeout
 */
int IPC_popMsgTout(void *data, int tout);

/**
 * @brief	Function to push the message into queue with no wait
 *
 * The function tries to push the \a data to the IPC queue. If the 
 * queue is full, it will return without blocking. It is equivalent
 * to calling IPC_pushMsgTout(data, 0).
 *
 * @param	data	: Pointer to data to be pushed
 * @return	#QUEUE_INSERT on success, #QUEUE_FULL or #QUEUE_ERROR on failure
 */
static inline int IPC_tryPushMsg(const void *data)
{
	return IPC_pushMsgTout(data, 0);
}

/**
 * @brief	Function to pop the message from queue with no wait
 *
 * The function tries to pop the data from the IPC queue. If the queue is empty,
 * it will return without blocking.
 *
 * @param	data	: Pointer to store popped data
 * @return	#QUEUE_VALID on success, #QUEUE_ERROR or #QUEUE_EMPTY on failure
 */
static inline int IPC_tryPopMsg(void *data)
{
	return IPC_popMsgTout(data, 0);
}

/**
 * @brief	Function to push the message into queue with wait
 *
 * The function to push the data to the IPC queue. If the queue is full, it will
 * wait till data is pushed (blocking).
 *
 * @param	data	: Pointer to data to be pushed
 * @return	#QUEUE_INSERT on success, #QUEUE_ERROR on failure
 */
static inline int IPC_pushMsg(const void *data, const int* addr, int size)
{
	return IPC_pushMsgTout(data, -1);
}

/**
 * @brief	Function to pop the message from queue with wait
 *
 * The function will pop the data from the IPC queue. If the queue is empty,
 * it will wait (blocking) till the data is available.
 *
 * @param	data	: Pointer to store popped data
 * @return	status	: Pop status
 */
static inline int IPC_popMsg(void *data)
{
	return IPC_popMsgTout(data, -1);
}

/**
 * @brief	Get number of pending items in queue
 *
 * This function will get the number of pending items in the queue. If
 * this function is called by M4 core with param \a queue_write set to
 * non zero then this function will return the number of messages waiting
 * in M4's Push Queue that is yet to be popped by the M0 core, or vice versa.
 * If this function is called by M4 core with param \a queue_write set to
 * zero then it returns the number of elements that can be popped without
 * waiting (in other words number of messages pending in the M0's Push queue
 * that is yet to be popped by M4 core), or vice versa.
 *
 * @param	queue_write	: 1 - read number of elements in write queue,
 *                        0 - read number of elements in read queue
 * @return	On success - Number of elements in queue (which will be >= 0),
 * @note	On Error   - #QUEUE_ERROR (when queue is not initialized/valid)
 */
int IPC_msgPending(int queue_write);

/**
 * @brief	Function to send notificaton interrupt
 *
 * When called from the M0, it sends the notification interrupt to M4 and vice-versa.
 *
 * @return	None
 */
void IPC_msgNotify(void);

/**
 * @brief	Function to initialize the IPC message queue
 *
 * This function intializes the interprocessor communication message queue.
 * **IMPORTANT NOTE: \a MaxNoOfMsg must always be a power of 2.**
 *
 * @param	data	: Pointer to the array of messages of size \a msgSize
 * @param	msgSize	: Size of the single data element in queue
 * @param	maxNoOfMsg	: Maximum number of items that can be stored in Queue
 * @return	None, will not return if there is error in given arguments
 */
void IPC_initMsgQueue(void *data, int msgSize, int maxNoOfMsg);

/**
 * @brief	Function to convert IPC error number to string
 *
 * This function returns the error string for the error value.
 *
 * @param	errnum : IPC error value
 * @return	pointer to error string
 */
const char *IPC_strerror(int errnum);

#endif