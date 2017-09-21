/** Library Name: app_board_btn_ble.c
	*
	* @Brief		This library defines functions related to button-controlled BLE applications
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "app_board_btn_ble.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define Variables */

/** @Variable Error handler registered by the user */
static btn_ble_error_handler_t error_handler = NULL;

/** @Variable Number of connections the device is currently in */
static uint32_t num_connections	= 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Implemetations (Internal Functions) */

/** @Func Function for configuring the buttons for connection */
static uint8_t connection_btn_config(void)
{
	uint8_t err_code;
	
  err_code = boardButtonEventAssign(BTN_PIN_SLEEP, BTN_ACTION_SLEEP, BOARD_EVENT_DEFAULT);
  RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);

  err_code = boardButtonEventAssign(BTN_PIN_WHITELIST_OFF, BTN_ACTION_WHITELIST_OFF, BOARD_EVENT_WHITELIST_OFF);
  RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);

  err_code = boardButtonEventAssign(BTN_PIN_DISCONNECT, BTN_ACTION_DISCONNECT, BOARD_EVENT_DISCONNECT);
  RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);
	
  return NRF_SUCCESS;
}

/** @Func Function for configuring the buttons for advertisement */
static uint8_t advertising_btn_config(void)
{
	uint8_t err_code;

  err_code = boardButtonEventAssign(BTN_PIN_DISCONNECT, BTN_ACTION_DISCONNECT, BOARD_EVENT_DEFAULT);
  RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);

  err_code = boardButtonEventAssign(BTN_PIN_WHITELIST_OFF, BTN_ACTION_WHITELIST_OFF, BOARD_EVENT_WHITELIST_OFF);
  RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);

  err_code = boardButtonEventAssign(BTN_PIN_SLEEP, BTN_ACTION_SLEEP, BOARD_EVENT_SLEEP);
  RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);
	
  return NRF_SUCCESS;
}

/** @Func Function for extracting the board event valid at startup */

static void startup_event_extract(board_event_t * p_startup_event)
{
	// React to button states
	if(boardButtonIsPressed(BTN_PIN_WAKEUP_BOND_DELETE)){
		*p_startup_event = BOARD_EVENT_CLEAR_BONDING_DATA;
  }
  else if(boardButtonIsPressed(BTN_PIN_WAKEUP)){
		*p_startup_event = BOARD_EVENT_WAKEUP;
  }
  else{
		*p_startup_event = BOARD_EVENT_NOTHING;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Implementations */

/** @Func Setting up Wakeup Buttons Before Going into Sleep Mode */
uint8_t btnBleSetWakeUpButton(void)
{
	uint8_t err_code;
	
	err_code = boardWakeupButtonEnable(BTN_PIN_WAKEUP); 
  RETURN_ON_ERROR_NOT_NOT_SUPPORTED(err_code);

  err_code = boardWakeupButtonEnable(BTN_PIN_WAKEUP_BOND_DELETE); 
  RETURN_ON_ERROR_NOT_NOT_SUPPORTED(err_code);
	
	return NRF_SUCCESS;
}

/** @Func Handling the Application's BLE Stack Events */
void btnBleEventHandler(ble_evt_t * p_ble_evt)
{
	uint8_t err_code;
	
	switch (p_ble_evt->header.evt_id){
		case BLE_GAP_EVT_CONNECTED:
		{
			if(num_connections == 0){
				err_code = connection_btn_config();
        CALL_HANDLER_ON_ERROR(error_handler, err_code);
			}
			num_connections++;
      break;
		}
		case BLE_GAP_EVT_DISCONNECTED:
		{
			num_connections--;
			if (num_connections == 0){
				err_code = advertising_btn_config();
        CALL_HANDLER_ON_ERROR(error_handler, err_code);
      }
      break;
		}
    default:
			break;
	}
}

/** @Func Initializing the BLE Button Module */
uint8_t btnBleInit(btn_ble_error_handler_t err_handler_in, board_event_t * p_startup_board_evt)
{
	uint8_t err_code = NRF_SUCCESS;
	
  error_handler = err_handler_in;
	
	if(p_startup_board_evt != NULL){
		startup_event_extract(p_startup_board_evt);
  }

  if(num_connections == 0){
		err_code = advertising_btn_config();
  }
	
  return err_code;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
