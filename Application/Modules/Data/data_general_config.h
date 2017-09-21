/** File Name : data_general_config.h
	*
	*	@Brief	This file includes all the required modules by the main.c program
	* @Brief	This file defines all the macros used in initialization functions, event handlers and operation functions
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
	* @Macro
	*	@Macro
	* @Macro
	* @Macro
	* @Macro
	* @Macro
	* @Macro
	* @Macro
	* @Macro
	* @Macro
	* @Macro
	*
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DATA_GENERAL_CONFIG_H__
#define __DATA_GENERAL_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Included Modules */

/** @Modules System Modules */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** @Modules Common Modules */
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"

/** @Modules Bluetooth Modules */
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"

/** @Module Softdevice Module */
#include "softdevice_handler.h"

/** @Module SDK Application Modules */
#include "nrf_gpio.h"
#include "app_timer.h"
#include "fstorage.h"
#include "fds.h"
#include "peer_manager.h"
#include "sensorsim.h"

/** @Modules Logging Modules */
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

/** @Modules Custom Application Modules */
#include "board_select.h"
#include "app_board.h"
#include "app_board_btn_ble.h"
#include "app_sensor.h"
#include "app_storage.h"
#include "app_uart_comm.h"
#include "app_adc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Macro Definitions (Initialization Functions) */

/* Manufacture Information */
/** @Macro Name of device. Will be included in the advertising data */
#define DEVICE_NAME                     														"Nordic_Template"
/** @Macro Manufacturer. Will be passed to Device Information Service */
#define MANUFACTURER_NAME               														"NordicSemiconductor"

/* Timer Macros */
/** @Macro Value of the RTC1 PRESCALER register */
#define APP_TIMER_PRESCALER             														0
/** @Macro Size of timer operation queues */
#define APP_TIMER_OP_QUEUE_SIZE         														4

/* GAP Parameters */
/** @Macro Minimum acceptable connection interval (0.1 seconds) */
#define MIN_CONN_INTERVAL               														MSEC_TO_UNITS(100, UNIT_1_25_MS)
/** @Macro Maximum acceptable connection interval (0.2 second) */
#define MAX_CONN_INTERVAL               														MSEC_TO_UNITS(200, UNIT_1_25_MS)
/** @Macro Slave latency */
#define SLAVE_LATENCY                   														0
/** @Macro Connection supervisory timeout (4 seconds) */
#define CONN_SUP_TIMEOUT                														MSEC_TO_UNITS(4000, UNIT_10_MS)

/* Connection Parameters */
/** @Macro Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds) */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  														APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)
/** @Macro Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds) */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   														APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)
/** @Macro Number of attempts before giving up the connection parameter negotiation */
#define MAX_CONN_PARAMS_UPDATE_COUNT    														3

/* Bluetooth Paramenters */
/** @Macro Number of central links used by the application. When changing this number remember to adjust the RAM settings */
#define CENTRAL_LINK_COUNT              														0
/** Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings */
#define PERIPHERAL_LINK_COUNT           														1
/** @Macro MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event */
#if (NRF_SD_BLE_API_VERSION == 3)
#define NRF_BLE_MAX_MTU_SIZE            														GATT_MTU_SIZE_DEFAULT
#endif

/* Peer Manager Parameters */
#define SEC_PARAM_BOND                  														1                                   // Perform bonding
#define SEC_PARAM_MITM                  														0                                   // Man In The Middle protection not required
#define SEC_PARAM_LESC                  														0                                   // LE Secure Connections not enabled
#define SEC_PARAM_KEYPRESS              														0                                   // Keypress notifications not enabled
#define SEC_PARAM_IO_CAPABILITIES       														BLE_GAP_IO_CAPS_NONE                // No I/O capabilities
#define SEC_PARAM_OOB                   														0                                   // Out Of Band data not available
#define SEC_PARAM_MIN_KEY_SIZE          														7                                   // Minimum encryption key size
#define SEC_PARAM_MAX_KEY_SIZE          														16                                  // Maximum encryption key size

/* Bluetooth Advertising Parameters */
/** @Macro The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms) */
#define APP_ADV_INTERVAL                														300
/** @Macro The advertising timeout in units of seconds */
#define APP_ADV_TIMEOUT_IN_SECONDS      														180

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Macro Definitions (Event Handlers) */

/** @Macro Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device */
#define IS_SRVC_CHANGED_CHARACT_PRESENT 														1
/** @Macro Reply when unsupported features are requested */
#define APP_FEATURE_NOT_SUPPORTED       														BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2
/** @Macro Value used as error code on stack dump, can be used to identify stack location on stack unwind */
#define DEAD_BEEF                       														0xDEADBEEF

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DATA_GENERAL_CONFIG_H__
