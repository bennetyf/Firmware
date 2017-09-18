/** Library Name: app_uart_comm.h
	*	@Brief		This library is served as the application layer of the uart communication
	*
	* @Auther 	Feng Yuan
	* @Time 		11/09/2017
	* @Version	1.0
	*
	* @Req 			This library requires the following modules to function
	* @Req 			- UART Application Module 								(Configured in sdk_config.h)
	* @Req			- UART FIFO Application Module
	* @Req			- APP FIFO Application Module
	*
	*	@Macro  	UART_COMM_TX_BUF_SIZE											(The Size of TX Buffer in UART Interface)
	*	@Macro		UART_COMM_RX_BUF_SIZE											(The Size of RX Buffer in UART Interface)
	*	@Macro		CTS_PIN_NUMBER														(The Dummy Pin Number of CTS[Hardware Flow Control])
	* @Macro		RTS_PIN_NUMBER														(The Dummy Pin Number of RTS[Hardware Flow Control])
	* @Macro		UART_TRANS_TRY_TIME												(The Number of Times to Try in UART Comm)
	*
	*	@Type			uart_err_handler_t												(The Pointer Type to the UART Error Handler)
	*
	* @Func			uartConfig																(Configure the UART Module)
	* @Func			uartSendByteArray													(Send Out A Byte Array Through the UART Interface)
	* @Func			uartReceiveByteArray											(Receive A Byte Array Through the UART Interface)
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __APP_UART_COMM_H__
#define __APP_UART_COMM_H__
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Modules Included */

#include "app_uart.h"
#include "app_error.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define the UART event handler type */

/** @Macro The Size of the TX Buffer for UART Comm */
#define UART_COMM_TX_BUF_SIZE 256                         //UART TX buffer size

/** @Macro The Size of the RX Buffer for UART Comm*/
#define UART_COMM_RX_BUF_SIZE 256                         //UART RX buffer size

/* The UART Control Flow is not Used, but Still Need to Define two Dummy Pins for CTS and RTS
 * This is Required by the UART Configuration Structure
*/

/** @Macro The CTS Pin Number */
#ifndef CTS_PIN_NUMBER
#define CTS_PIN_NUMBER				2														//UART CTS Pin Number
#endif

/** @Macro The RTS Pin Number */
#ifndef RTS_PIN_NUMBER
#define RTS_PIN_NUMBER				3														//UART RTS Pin Number
#endif

/** @Macro The Number of Times to Try in UART Comm */
#define UART_TRANS_TRY_TIME 	16													//The number of times to try in UART transmission and reception

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Type The Error Handler Function Pointer Type in UART Module */
typedef void (*uart_err_handler_t)(app_uart_evt_t *);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* Function Declarations for UART Applications */
	
/** @Func Configure the UART Module
	*
	*	@Brief This function initializes the UART module
	* 
	*	@Para rx_pin_no							[uint8_t]: 						the number of the rx pin for UART interface
	* @Para tx_pin_no 						[uint8_t]:						the number of the tx pin for UART interface
	*	@Para baud_rate							[uint32_t]: 					the baud rate of the UART interface 
	* @Para uart_err_handler_ptr 	[uart_err_handler_t]:	the pointer to the error handler of the UART interface
	*
*/
void uartConfig(uint8_t rx_pin_no, uint8_t tx_pin_no, uint32_t baud_rate, uart_err_handler_t uart_err_handler_ptr);
	
/** @Func Send A Byte Array Through the UART Interface
	*	
	*	@Brief This function sends out the data stored in a byte array through UART interface
	*
	* @Para byte_array 				[uint8_t*]: the address of the byte array to be sent
	*	@Para array_length 			[uint8_t]:	the length of the byte array
	*
	* @Return err_code 				[uint8_t]:  the error code returned by the uart operations
	*
*/
uint8_t uartSendByteArray(uint8_t * byte_array, uint8_t array_length);

/** @Func Receive A Byte Array Through the UART Interface
	*	
	*	@Brief This function receives the data in a byte stream through UART interface and stores it into a designated byte array address
	*
	* @Para byte_array 				[uint8_t*]: the address of the byte array to be stored
	*	@Para array_length 			[uint8_t]:	the length of the byte array
	*
	* @Return err_code 				[uint8_t]:  the error code returned by the uart operations
	*
*/
uint8_t uartReceiveByteArray(uint8_t * byte_array, uint8_t array_length);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
}
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__UART_COMM_H__
