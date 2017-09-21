/** Library Name: app_board_btn_ble.h
	*
	* @Brief		This library defines functions related to button-controlled BLE applications
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
	*	@Macro		BTN_PIN_WAKEUP																				(Button ID used to wake up the application)
	* @Macro		BTN_PIN_SLEEP																					(Button ID used to put the application into sleep mode)
	* @Macro		BTN_PIN_DISCONNECT																		(Button ID used to gracefully terminate a connection on long press)
	* @Macro		BTN_PIN_WAKEUP_BOND_DELETE														(Button ID used to wake up the application and delete all bonding information)
	*	@Macro		BTN_PIN_WHITELIST_OFF																	(Button ID used to turn off usage of the whitelist)
	*
	*	@Macro		BTN_ACTION_SLEEP																			(Button action used to put the application into sleep mode)
	*	@Macro		BTN_ACTION_DISCONNECT																	(Button action used to gracefully terminate a connection on long press)
	* @Macro		BTN_ACTION_WHITELIST_OFF															(Button action used to turn off usage of the whitelist)
	*
	*	@Macro		RETURN_ON_ERROR(err_code)															(Return from the current function if err_code is not NRF_SUCCESS)
	* @Macro		RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code)						(Return from the current function if err_code is not NRF_SUCCESS or NRF_ERROR_INVALID_PARAM)
	* @Macro		RETURN_ON_ERROR_NOT_NOT_SUPPORTED(err_code)						(Return from the current function if err_code is not NRF_SUCCESS or NRF_ERROR_NOT_SUPPORTED)
	* @Macro		CALL_HANDLER_ON_ERROR(error_handler, err_code)				(Call the registered error handler if err_code is not NRF_SUCCESS and the error handler is not NULL)
	*
	* @Type			btn_ble_error_handler_t																(BLE button module error handler type)
	*
	*	@Func			btnBleSetWakeUpButton																	(Setting up Wakeup Buttons Before Going into Sleep Mode)
	* @Func			btnBleEventHandler																		(Handling the Application's BLE Stack Events)
	* @Func			btnBleInit																						(Initializing the BLE Button Module)
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APP_BOARD_BTN_BLE_H__
#define __APP_BOARD_BTN_BLE_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Included Modules */
#include "ble.h"
#include "app_board.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Macro Definitions */

/** @Macro Button PIN No Definitions */
#define BTN_PIN_WAKEUP             						BOARD_BUTTON_0  							// ID of button used to wake up the application
#define BTN_PIN_SLEEP              						BOARD_BUTTON_0  							// ID of button used to put the application into sleep mode
#define BTN_PIN_DISCONNECT         						BOARD_BUTTON_0  							// ID of button used to gracefully terminate a connection on long press
#define BTN_PIN_WAKEUP_BOND_DELETE 						BOARD_BUTTON_0  							// ID of button used to wake up the application and delete all bonding information
#define BTN_PIN_WHITELIST_OFF      						BOARD_BUTTON_0  							// ID of button used to turn off usage of the whitelist

/** @Macro Buttion Action Assignment */
#define BTN_ACTION_SLEEP          						BOARD_BUTTON_ACTION_RELEASE   // Button action used to put the application into sleep mode
#define BTN_ACTION_DISCONNECT     						BOARD_BUTTON_ACTION_LONG_PUSH // Button action used to gracefully terminate a connection on long press
#define BTN_ACTION_WHITELIST_OFF  						BOARD_BUTTON_ACTION_LONG_PUSH	// Button action used to turn off usage of the whitelist

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Macro-defined Procedures */

/** @Macro 	This macro will return from the current function if err_code is not NRF_SUCCESS */
#define RETURN_ON_ERROR(err_code)  \
do                                 \
{                                  \
    if ((err_code) != NRF_SUCCESS) \
    {                              \
        return err_code;           \
    }                              \
}                                  \
while (0)

/** @Macro 	This macro will return from the current function if err_code is not NRF_SUCCESS or NRF_ERROR_INVALID_PARAM */
#define RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code)                             \
do                                                                              \
{                                                                               \
    if (((err_code) != NRF_SUCCESS) && ((err_code) != NRF_ERROR_INVALID_PARAM)) \
    {                                                                           \
        return err_code;                                                        \
    }                                                                           \
}                                                                               \
while (0)

/** @Macro	This macro will return from the current function if err_code is not NRF_SUCCESS or NRF_ERROR_NOT_SUPPORTED */
#define RETURN_ON_ERROR_NOT_NOT_SUPPORTED(err_code)                             \
do                                                                              \
{                                                                               \
    if (((err_code) != NRF_SUCCESS) && ((err_code) != NRF_ERROR_NOT_SUPPORTED)) \
    {                                                                           \
        return err_code;                                                        \
    }                                                                           \
}                                                                               \
while (0)

/** @Macro	This macro will call the registered error handler if err_code is not NRF_SUCCESS and the error handler is not NULL */
#define CALL_HANDLER_ON_ERROR(error_handler, err_code)                          \
do                                                                							\
{                                                                 							\
    if (((err_code) != NRF_SUCCESS) && (error_handler != NULL)) 								\
    {                                                             							\
        error_handler(err_code);                                								\
    }                                                             							\
}                                                                 							\
while (0)

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
#ifdef __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Type Declarations */

/** @Type BLE Button Module Error Handle Type */
typedef void (* btn_ble_error_handler_t) (uint32_t nrf_error);

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Declarations */

/** @Func Setting up Wakeup Buttons Before Going into Sleep Mode */
uint8_t btnBleSetWakeUpButton(void);

/** @Func Handling the Application's BLE Stack Events */
void btnBleEventHandler(ble_evt_t * p_ble_evt);

/** @Func Initializing the BLE Button Module */
uint8_t btnBleInit(btn_ble_error_handler_t err_handler_in, board_event_t * p_startup_board_evt);

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
#ifdef __cplusplus
}
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__APP_BOARD_BTN_BLE_H__
