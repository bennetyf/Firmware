/** File Name : operations.h
	* 
	*	@Brief		This file declares all the application functions used in the main.c program
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
	* @Req			This file requires the macro definitions and included SDK modules from the file "data_general_config.h"
	*
	* @Func			isLoggerEnabled														(Check whether the Logger is Enabled or Not)
	*	@Func			enableLogger															(Enable the Logger)
	*	@Func			disableLogger															(Disable the Logger)
	*
	*	@Func			assert_nrf_callback												(Callback function for asserts in the SoftDevice)
	*	@Func			enterSleepMode														(Putting the chip into sleep mode)
	*	@Func			powerManage																(Function for the Power manager)
	* @Func			startAdvertising													(Function for starting advertising)
	*
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Included Modules */
#include "data_general_config.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* Function Declarations */
/*===========================================================================================================================*/
/* Logger Functions */
	
/** @Func 	Check whether the Logger is Enabled or Not
	*
	* @Brief	This function checks whether the logging module is enabled
	* @Brief	It returns a bool value
	*
	*	@Return	True	: The logger is enabled
	* @Return	False :	The logger is disalbed
	*
*/
bool isLoggerEnabled(void);

/** @Func 	Enable the Logger
	*
	* @Brief	This function enables the logging module
	*
*/
void enableLogger(void);

/** @Func Disable the Logger
	*
	*	@Brief This function disables the logging module
	*
*/
void disableLogger(void);

/*===========================================================================================================================*/

/** @Func 		Callback function for asserts in the SoftDevice
 *
 * 	@Brief 		This function will be called in case of an assert in the SoftDevice.
 *
 * 	@Warning 	This handler is an example only and does not fit a final product. You need to analyze
 *          	how your product is supposed to react in case of Assert.
 * 	@Warning 	On assert from the SoftDevice, the system can only recover on reset.
 *
 * 	@Para 		line_num   Line number of the failing ASSERT call.
 * 	@Para 		file_name  File name of the failing ASSERT call.
*/
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);

/** @Func Function for putting the chip into sleep mode
 *
 * 	@Note This function will not return.
*/
void enterSleepMode(void);

/** @Func Function for the power manager */
void powerManage(void);

/** @Func Function for starting advertising */
void startAdvertising(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
#ifdef __cplusplus
}
#endif //__cplusplus

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__OPERATIONS_H__
