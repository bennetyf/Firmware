/** Library Name : app_sensor.h
	*
	* @Brief 		This module implements the application layer for accessing color sensor through the TWI interface
	*
	* @Auther 	Feng Yuan
	* @Time 		18/09/2017
	* @Version	1.0
	*
	*
	* @Macro		MAX_TWI_QUEUE_SIZE									(Maximum Queue Size for the TWI Module)
	*	@Macro		POST_SAMPLE_WAIT_TIME_MARGIN				(Post Sample Waiting Time Margin)
	* @Macro		RED_CURRENT													(Red LED Current - 9.6mA)
	* @Macro		GREEN_CURRENT												(GREEN_CURRENT - 12mA)
	*	@Macro		BLUE_CURRENT												(BLUE_CURRENT - 2.4mA)
	*	@Macro		LED_ON_COMMAND											(Turn On LED)
	* @Macro 		LED_OFF_COMMAND											(Turn Off LED)
	*	@Macro		INTEGRATION_TIME										(Integration Time Settings)
	*	@Macro 		MANUAL_TIMING												(Manual Timing)
	* @Macro		SENSOR_SETUP												(Sensor Setup Command)
	* @Macro		START_SAMPLING											(Sensor Sampling Command)
	* @Macro		SENSOR_SLEEP												(Sensor Sleeping Command)
	*
	* @Type			led_type_t													(LED Types)
	*	@Type			reg_type_t													(Register Types)
	*	@Type			sensor_led_current_t								(Data Type to Store the LED Current Settings)
	*
	*	@Func			twiConfig														(Configuration of TWI Peripheral)
	* @Func			sensorSetLedCurrent									(Set Sensor LED Current)
	*	@Func			sensorSetup													(Configuration of the Color Sensor)
	* @Func			twiWriteByteArray										(Write a Byte Array into the Sensor Register from the Specified Address)
	* @Func			twiReadByteArray										(Read Out the Register Data into a Byte Array from the Specified Address)
	* @Func			sensorTurnOnLed											(Turn On A Specified LED)
	* @Func			sensorTurnOffLed										(Turn Off A Specified LED)
	* @Func			sensorReadDataAll										(Read Out the Acquired Sensor Data)
	* @Func			sensorReadRegister									(Read the Settings in A Specified Internal Register)
	* @Func			sensorSampleStart										(Start Color Sampling)
	* @Func			sensorSleep													(Put the Sensor into Sleeping)
	*	@Func			sensorSampleColor										(Sampling From All Three Channels)
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APP_SENSOR_H__
#define __APP_SENSOR_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* System Modules */

#include "data_sensor_config.h"
#include "app_twi.h"
#include "app_util_platform.h"
#include "nrf_delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Macro Definitions */

/** @Macro Maximum Queue Size for the TWI Module */
#define MAX_TWI_QUEUE_SIZE						5

/** @Macro Post Sample Waiting Time Margin */
#define POST_SAMPLE_WAIT_TIME_MARGIN  1.2  										//1.05 some zero-readings

/* LED Current Settings */
/** @Macro Red LED - 9.6mA */
#define RED_CURRENT 			 						(SENSOR_LED_RED_CURRENT_64MA|SENSOR_LED_RED_CURRENT_32MA)
													
/** @Macro Green LED - 12mA */
#define GREEN_CURRENT 								(SENSOR_LED_GREEN_CURRENT_64MA|SENSOR_LED_GREEN_CURRENT_32MA \
																			|SENSOR_LED_GREEN_CURRENT_16MA|SENSOR_LED_GREEN_CURRENT_8MA)

/** @Macro Blue LED - 2.4mA */
#define BLUE_CURRENT									(SENSOR_LED_BLUE_CURRENT_16MA|SENSOR_LED_BLUE_CURRENT_8MA)

/** @Macro LED Operation Commands */
#define LED_ON_COMMAND								(SENSOR_LED_RESET_OPERATION|SENSOR_LED_DC_MODE_DC|SENSOR_LED_ONE_TENTH_MODE_ONE_TENTH)

#define LED_OFF_COMMAND								(SENSOR_LED_RESET_RESET|SENSOR_LED_SLEEP_SLEEP \
																			|SENSOR_LED_DC_MODE_DC|SENSOR_LED_ONE_TENTH_MODE_ONE_TENTH)	

/** @Macro Integration Time Settings */
#define INTEGRATION_TIME 							SENSOR_CTRL_INT_TIME_SETTING_32US

/** @Macro Manual Timing */
#define MANUAL_TIMING									1875u			//32us X 1875 = 60ms

/* Sensor Configurations */
/** @Macro Sensor Setup Command */
#define SENSOR_SETUP 									(SENSOR_CTRL_RESET_OPERATION|SENSOR_CTRL_REG_RESET_03_TO_0A \
																			|SENSOR_CTRL_GAIN_SELECT_LOW|SENSOR_CTRL_INT_MODE_MANUAL_SETTING)
																			
/** @Macro Sensor Sampling Command */
#define START_SAMPLING 								(SENSOR_CTRL_RESET_OPERATION|SENSOR_CTRL_SLEEP_OPERATION \
																			|SENSOR_CTRL_REG_RESET_RESET_RELEASE|SENSOR_CTRL_GAIN_SELECT_LOW \
																			|SENSOR_CTRL_INT_MODE_MANUAL_SETTING|SENSOR_CTRL_INT_TIME_SETTING_32US)

/** @Macro Sensor Sleep Command */
#define SENSOR_SLEEP									(SENSOR_CTRL_SLEEP_SLEEP|SENSOR_CTRL_GAIN_SELECT_LOW|SENSOR_CTRL_INT_MODE_MANUAL_SETTING)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Header */

#ifdef __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Type Declarations */

/** @Type LED Types */
typedef enum{
	RED,
	GREEN,
	BLUE
}led_type_t;

/** @Type Register Types */
typedef enum{
	LED_CTRL_REG,
	SENSOR_CTRL_REG
}reg_type_t;

/** @Type Data Type to Store the LED Current Settings */
typedef struct{
	// LED Current Settings
	uint8_t					led_red_current;
	uint8_t 				led_green_current;
	uint8_t					led_blue_current;
}sensor_led_current_t;

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
void twiConfig(uint8_t scl_pin_no, uint8_t sda_pin_no);

/** @Func 	Set Sensor LED Current
	*
	* @Brief	This function sets the LED current to the specified LED	
	*
	*	@Para		current 	[uint8_t] 		: the LED current choices which should be of the size of a byte
	* @Para		led_type	[led_type_t] 	:	the LED type which can be choosen from RED,GREEN,BLUE
	*
*/
void sensorSetLedCurrent(uint8_t current, led_type_t led_type);

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
uint8_t sensorSetup(uint16_t value);

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
uint8_t twiWriteByteArray(uint8_t * byte_array, uint8_t array_length, uint8_t address);

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
uint8_t twiReadByteArray(uint8_t * byte_array, uint8_t array_length, uint8_t address);

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
uint8_t sensorTurnOnLed(led_type_t led_type);

/** @Func 	Turn Off A Specified LED
	*
	*	@Breif	This function turns off any LED by setting all LED currents into zeros
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorTurnOffLed(void);

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
uint8_t sensorReadDataAll(uint8_t * byte_array, uint8_t array_length);

/** @Func 	Read the Settings in A Specified Internal Register
	*
	* @Breif	This function reads out the specified register contents into the byte array
	*
	* @Para		byte_array 		[uint8_t*] 		: the address of the byte array to be read into
	* @Para		array_length 	[uint8_t]	 		: the length of the byte array to be read into
	* @Para		reg_type			[reg_type_t] 	:	the register selection which should be choosen from the enumerator type reg_type_t
	*
	* @Return NRF_SUCCESS : The operation succeeded
	*	@Return Propagate the TWI operation errors
	*
*/
uint8_t sensorReadRegister(uint8_t * byte_array, uint8_t array_length, reg_type_t reg_type);

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
uint8_t sensorSampleColor(uint8_t * byte_array, uint8_t array_length);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* C++ Library Header */
	
#ifdef __cplusplus
} 
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#endif //__APP_SENSOR_H__
