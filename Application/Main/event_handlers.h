/** File Name : event_handlers.h
	*
	* @Brief	This file declares all the event handlers used in the main.c program.
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
	* @Req			This module requires the following modules:
	*	@Req			- General Data Definitions (Included in "data_general_config.h")
	*	@Req			-	Initialization Functions (Included in "initialization.h")
	* @Req			- Application Functions    (Included in "operations.h")
	*
	*	@Func			
	* @Func
	* @Func
	* @Func
	* @Func
	* @Func
	*
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __EVENT_HANDLERS_H__
#define __EVENT_HANDLERS_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Included Modules */
#include "data_general_config.h"
#include "initialization.h"
#include "operations.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* Event Handler Declarations */

/** @Func 	Function for handling Peer Manager events
	*
	* @Para 	p_evt  Peer Manager event
	*
*/
void pm_evt_handler(pm_evt_t const * p_evt);

/**	@Func 	Function for handling the Connection Parameters Module.
	*
	* @Brief 	This function will be called for all events in the Connection Parameters Module which
	*         are passed to the application.
	*
	* @Note 	All this function does is to disconnect. This could have been done by simply
	*         setting the disconnect_on_fail config parameter, but instead we use the event
	*         handler mechanism to demonstrate its use.
	*
	* @Para 	p_evt  Event received from the Connection Parameters Module.
	*
 */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt);

/**	@Func		Function for handling a Connection Parameters error.
 *
 * 	@Para 	nrf_error  Error code containing information about what went wrong.
 *
*/
void conn_params_error_handler(uint32_t nrf_error);

/**	@Func 	Function for handling advertising events.
	*
	* @Brief 	This function will be called for advertising events which are passed to the application.
	*
	* @Para 	ble_adv_evt  Advertising event.
	*
*/
void on_adv_evt(ble_adv_evt_t ble_adv_evt);

/**	@Func 	Function for handling the Application's BLE Stack events
	*
	* @Para 	p_ble_evt  Bluetooth stack event
	*
*/
void on_ble_evt(ble_evt_t * p_ble_evt);

/**	@Func 	Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
	*
	* @Brief 	This function is called from the BLE Stack event interrupt handler after a BLE stack
	*         event has been received.
	*
	* @Para 	p_ble_evt  Bluetooth stack event.
	*
*/
void ble_evt_dispatch(ble_evt_t * p_ble_evt);

/**	@Func 	Function for dispatching a system event to interested modules.
 *
 * 	@Brief 	This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * 	@Para 	sys_evt  System stack event
 *
 */
void sys_evt_dispatch(uint32_t sys_evt);

/**	@Func 	Function for handling events from the BSP module.
	*
	* @Para   event   Event generated when button is pressed.
*/
void board_event_handler(board_event_t event);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
#ifdef __cplusplus
}
#endif //__cplusplus

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__EVENT_HANDLERS_H__
