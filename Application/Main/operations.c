/** File Name : operations.h
	* 
	*	@Brief		This file declares all the application functions used in the main.c program
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "operations.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Variable General Flag for Logging */
static bool logger_enable_flag = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Enable the Logger */
void enableLogger(void)
{
	logger_enable_flag = true;
}

/** @Func Disable the Logger */
void disableLogger(void)
{
	logger_enable_flag = false;
}

/** @Func Check whether the Logger is Enabled */
bool isLoggerEnabled(void)
{
	return logger_enable_flag;
}

/** @Func Callback function for asserts in the SoftDevice */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/** @Func Function for putting the chip into sleep mode */
void enterSleepMode(void)
{
	uint32_t err_code;
//    err_code = bsp_indication_set(BSP_INDICATE_IDLE);

//    APP_ERROR_CHECK(err_code);

	// Prepare wakeup buttons.
	err_code = btnBleSetWakeUpButton();
	APP_ERROR_CHECK(err_code);

	// Go to system-off mode (this function will not return; wakeup will cause a reset).
  err_code = sd_power_system_off();
  APP_ERROR_CHECK(err_code);
}

/** @Func Function for the Power manager */
void powerManage(void)
{
    uint32_t err_code = sd_app_evt_wait();

    APP_ERROR_CHECK(err_code);
}

/** @Func Function for starting advertising */
void startAdvertising(void)
{
    uint32_t err_code = ble_advertising_start(BLE_ADV_MODE_FAST);

    APP_ERROR_CHECK(err_code);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
