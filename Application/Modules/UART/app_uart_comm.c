/** Library Name: app_uart_comm.c
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

/* Modules Included */
#include "app_uart_comm.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Variable The UART Configuration Variable */
static app_uart_comm_params_t 	uart_comm_config;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* Function Implementation for UART Applications */

/** @Func Default UART Error Handler 
	* @Brief If the UART error handler is not specified outside this module, the default handler will be used.
	*
*/
static void uart_error_handler(app_uart_evt_t * p_event)
{
	if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR){
		APP_ERROR_HANDLER(p_event->data.error_communication);
  }
  else if (p_event->evt_type == APP_UART_FIFO_ERROR){
    APP_ERROR_HANDLER(p_event->data.error_code);
	}
}

/** @Func Initialize the UART Module */
void uartConfig(uint8_t rx_pin_no, uint8_t tx_pin_no, uint32_t baud_rate, uart_err_handler_t uart_err_handler_ptr)
{
	uint32_t err_code;
	
	// Setting the UART configuration variable
	// Set the pin numbers
	uart_comm_config.rx_pin_no 		= rx_pin_no;
	uart_comm_config.tx_pin_no 		= tx_pin_no;
	uart_comm_config.rts_pin_no 	= RTS_PIN_NUMBER;
	uart_comm_config.cts_pin_no 	= CTS_PIN_NUMBER;
	// Set the options for flow control and parity check
	uart_comm_config.flow_control = APP_UART_FLOW_CONTROL_DISABLED;
	uart_comm_config.use_parity 	= false;
	// Set the UART baud rate
	uart_comm_config.baud_rate		= baud_rate;
	
	// Setting the UART error event handler
	if(uart_err_handler_ptr != NULL){ // Use the uart error handler in the input parameter
		APP_UART_FIFO_INIT(	&uart_comm_config,
                        UART_COMM_RX_BUF_SIZE,
                        UART_COMM_TX_BUF_SIZE,
                        uart_err_handler_ptr,
                        APP_IRQ_PRIORITY_LOWEST,
                        err_code);
	}
	else{	// Use the default uart error handler defined in this module
		APP_UART_FIFO_INIT(	&uart_comm_config,
                        UART_COMM_RX_BUF_SIZE,
                        UART_COMM_TX_BUF_SIZE,
                        uart_error_handler,
                        APP_IRQ_PRIORITY_LOWEST,
                        err_code);
	}
	
	// Check whether the initialization is successful or not
	APP_ERROR_CHECK(err_code);
}

/** @Func Send A Byte Array Through the UART Interface */
uint8_t uartSendByteArray(uint8_t * byte_array, uint8_t array_length)
{
	uint8_t err_code = NRF_SUCCESS;
	uint8_t i,j;
	
	// Loop in in the byte array
	for(i = 0; i < array_length; i++){
		
		// Try UART_TRANS_TRY_TIME times to transmit each byte
		for(j = 0; j < UART_TRANS_TRY_TIME; j++){
			if((err_code = app_uart_put(byte_array[i])) == NRF_SUCCESS){
				break;
			}
		}
		
		// If the transmission of any byte failed, return immediately
		if(err_code != NRF_SUCCESS){
			return err_code;
		}
	}
	
	return err_code;
}

/** @Func Receive A Byte Array Through the UART Interface */
uint8_t uartReceiveByteArray(uint8_t * byte_array, uint8_t array_length)
{
	uint8_t err_code = NRF_SUCCESS;
	uint8_t i,j;
	
	// Loop in in the byte array
	for(i = 0; i < array_length; i++){
		
		// Try UART_TRANS_TRY_TIME times to receive each byte
		for(j = 0; j < UART_TRANS_TRY_TIME; j++){
			if((err_code = app_uart_get(byte_array+i)) == NRF_SUCCESS){
				break;
			}
		}
		
		// If the receiving of any byte failed, return immediately
		if(err_code != NRF_SUCCESS){
			return err_code;
		}
	}
	
	return err_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
