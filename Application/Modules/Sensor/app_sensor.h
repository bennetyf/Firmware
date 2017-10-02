/** Library Name : app_sensor.h
	*
	* @Brief 		This module implements the application layer for accessing color sensor through the TWI interface
	*
	* @Auther 	Feng Yuan
	* @Time 		18/09/2017
	* @Version	1.0
	*
	* @Req			This module requires the following modules to be enabled
	* @Req			- TWI Transaction Manager 					(Configured in "sdk_config.h")
	* @Req		  - RTC Driver												(Configured in "sdk_config.h")
	* @Req		  - NRF Delay													(Included in "nrf_delay.h")
	*
	* @Type			led_type_t													(LED Types)
	*	@Type			reg_type_t													(Register Types)
	*	@Type			sensor_led_current_t								(Data Type to Store the LED Current Settings)
	*	@Type			sensor_data_t												(Data Type to Store the Byte Array Address and Array Length)
	* @Type			sensor_led_flags_t									(Data Type to Store the LED Status)
	*
	*	@Func			sensorConfig												(Configuration of the Sensor Module including the TWI Peripheral)
	* @Func			sensorSetLedCurrent									(Set Sensor LED Current)
	*
	* @Func			sensorWriteByteArray								(Write a Byte Array into the Sensor Register from the Specified Address)
	* @Func			sensorReadByteArray									(Read Out the Register Data into a Byte Array from the Specified Address)
	* @Func			sensorTurnOnLed											(Turn On A Specified LED)
	* @Func			sensorTurnOffLed										(Turn Off A Specified LED)
	*	@Func			sensorSetup													(Configuration of the Color Sensor)
	* @Func			sensorSampleStart										(Start Color Sampling)
	* @Func			sensorSleep													(Put the Sensor into Sleeping)
	*
	* @Func			sensorReadData											(Read Out the Acquired Sensor Data)
	*	@Func			sensorSampleColor										(Sampling From All Three Channels)
	*
	* @Func			sensorGetInstance										(Get the Address of the Internal TWI Instance)
	* @Func			sensorIsRedOn												(Test Whether the Red LED is Currently On)
	* @Func			sensorIsGreenOn											(Test Whether the Green LED is Currently On)
	* @Func			sensorIsBlueOn											(Test Whether the Blue LED is Currently On)
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APP_SENSOR_H__
#define __APP_SENSOR_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* System Modules */

#include "data_sensor_config.h"
#include "app_util_platform.h"
#include "app_twi.h"
#include "nrf_drv_rtc.h"
#include "nrf_delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Header */

#ifdef __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* Type Declarations */

/** @Type LED Types
	* 
	* @Brief This is an enumerate type representing the three LEDs
	*
*/
typedef enum{
	RED,
	GREEN,
	BLUE
}led_type_t;

/** @Type Register Types
	*
	* @Brief This is an enumerate type representing the internal color sensor registers
	*
*/
typedef enum{
	LED_CTRL_REG,
	SENSOR_CTRL_REG
}reg_type_t;

/** @Type 	Data Type to Store the LED Current Settings
	*
	*	@Brief 	This structure is for storing the LED currents of the color sensor
	*
*/
typedef struct{
	// LED Current Settings
	uint8_t					led_red_current;
	uint8_t 				led_green_current;
	uint8_t					led_blue_current;
}sensor_led_current_t;

/** @Type 	Data Type to Store the Byte Array Address and Array Length
	*
	* @Brief 	This structure is for temporarily storing the byte array address, array length and sensor register name
	* @Brief 	This usage is because the sample-reading function returns immediately in non-blocking mode.
	* @Brief 	The left work is to be completed by the event handler. So, the following information should be passed to the event handler in some way.
	*
*/
typedef struct{
	uint8_t					* array;
	uint8_t					length;
	uint8_t					reg_name;
}sensor_data_t;

/** @Type 	Data Type to Store the LED Status (LED Flags)
	*
	* @Brief 	This structure is for storing the LED on/off states, which is introduced as a requirement in non-blocking mode
	*
*/
typedef struct{
	bool		is_led_red_on;
	bool 		is_led_green_on;
	bool 		is_led_blue_on;
}sensor_led_flags_t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Declarations */

/** @Func		Configuration of TWI Peripheral
	*
	* @Brief	This function configures the TWI peripheral by setting the pin numbers of SCL and SDA
	*	@Brief  The TWI frequency and interrupt priority are set to 400K and low respectively
	* @Brief  The Maximum TWI queue size is set in the macro MAX_TWI_QUEUE_SIZE
	*
	*	@Para 	scl_pin_no [uint8_t] : the SCL pin number of TWI interface
	* @Para		sda_pin_no [uint8_t] : the SDA pin number of TWI interface
	*
*/
void sensorConfig(const uint8_t scl_pin_no, const uint8_t sda_pin_no, nrf_drv_rtc_handler_t rtc_event_handler, bool is_blocking_mode);

/** @Func 	Set Sensor LED Current
	*
	* @Brief	This function sets the LED current to the specified LED	
	*
	*	@Para		current 	[uint8_t] 		: the LED current choices which should be of the size of a byte
	* @Para		led_type	[led_type_t] 	:	the LED type which can be choosen from RED,GREEN,BLUE
	*
*/
void sensorLedCurrentConfig(const uint8_t current, const led_type_t led_type);

/** @Func 	Write a Byte Array into the Sensor Register from the Specified Address
	*
	* @Brief 	This function writes a byte array into the specified register address through the TWI interface
	*
	* @Para		byte_array 		[uint8_t*] :	the address of the byte array to be written into the register 
	* @Para		array_length 	[uint8_t]	 :	the length of the byte array in bytes
	* @Para		address				[uint8_t]	 :	the register address of the color sensor
	*
	* @Return NRF_SUCCESS	: The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorWriteByteArray(uint8_t * byte_array, const uint8_t array_length, const uint8_t address);

/** @Func 	Read Out the Register Data into a Byte Array from the Specified Address
	*
	*	@Brief	This function reads a byte array out of the specified register address through the TWI interface
	*
	* @Para		byte_array 		[uint8_t*]	:	 the address of the byte array to be read into
	* @Para		array_length 	[uint8_t]		:	 the length of the byte array in bytes
	* @Para		address				[uint8_t]		:	 the register address of the color sensor
	*
	* @Return	NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorReadByteArray(uint8_t * byte_array, const uint8_t array_length, const uint8_t address);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func 	Turn On A Specified LED
	*
	* @Brief 	This function turns on the specified LED by setting its current to non-zero value and setting the currents of other LEDs to zero
	*
	* @Para		led_type [led_type_t] 	: 	the LED choice of the LED which should be choosen from RED, GREEN, and BLUE
	*
	*	@Return NRF_SUCCESS : The operation succeeded
	* @Return Propagate the TWI operation errors
	*
*/
uint8_t sensorTurnOnLed(const led_type_t led_type);

/** @Func 	Turn Off A Specified LED
	*
	*	@Breif	This function turns off any LED by setting all LED currents into zeros
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorTurnOffLed(const led_type_t led_type);

/** @Func 	Configuration of the Color Sensor
	*
	*	@Brief	This function sets the sensor settings
	*
	* @Para		value [uint16_t] : the value to be written into the register
	*
	* @Return NRF_SUCCESS : The TWI operation succeeded
	* @Return Propagate the TWI operation errors
	*
*/
uint8_t sensorSetup(const uint16_t value);

/** @Func 	Start Color Sampling
	*
	* @Brief 	This function starts the sampling procedure of the color sensor
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorSampleStart(void);

/** @Func 	Put the Sensor into Sleeping
	*
	*	@Brief	This function puts the color sensor into sleeping mode
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorSleep(void);

/** @Func 	Read Out the Acquired Sensor Data
	*
	* @Brief	This function reads out all the sensor data (6 bytes in total from the output data registers) into the specified array
	*
	*	@Para		byte_array 		[uint8_t*] : the address of the byte array to be read into
	* @Para		array_length 	[uint8_t]	 : the length of the byte array to be read into
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorReadData(uint8_t * byte_array, const uint8_t array_length);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Top Layer Functions */

/** @Func 	Sampling From All Three Channels
	*
	*	@Brief	This function takes the color sensor data for all three RGB channels
	*
	* @Para		byte_array 		[uint8_t*] 	: the address of the byte array to be read into
	* @Para		array_length 	[uint8_t]		: the length of the byte array to be read into
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*	
*/
uint8_t sensorSampleColor(uint8_t * byte_array, const uint8_t array_length);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Accessor Functions */

/** @Func 	Get the Address of the Internal TWI Instance
	*
	* @Brief 	This function returns the address of the internal defined TWI instance
	*
	* @Return	app_twi_t : The TWI instance type
	*
*/
app_twi_t * sensorGetInstance(void);

/** @Func 	Test Whether the Red LED is Currently On
	*
	* @Brief 	This function returns the status of the Red LED
	*
	* @Return true 	[bool] : The Red LED is on
	*	@Return	false	[bool] : The Red LED is off
	*
*/
bool sensorIsRedOn(void);

/** @Func 	Test Whether the Green LED is Currently On
	*
	* @Brief	This function returns the status of the Green LED
	*
	* @Return	true 	[bool] : The Green LED is on
	* @Return false [bool] : The Green LED is off
	*
*/
bool sensorIsGreenOn(void);

/** @Func 	Test Whether the Blue LED is Currently On
	*
	* @Brief	This function returns the status of the Blue LED
	*
	* @Return true 	[bool] : The Blue LED is on
	* @Return	false [bool] : The Blue LED is off	
	*
*/
bool sensorIsBlueOn(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
	
#ifdef __cplusplus
} 
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#endif //__APP_SENSOR_H__
