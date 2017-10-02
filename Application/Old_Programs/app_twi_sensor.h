/** Library Name : "app_twi_sensor.h"
	*
	* @Brief 		This library defines color sensor applications through the I2C(also known as TWI) interface
	*
	* @Auther 	Feng Yuan
	* @Time 		25/09/2017
	* @Version	1.0
	*
	* @Req			The required 
	* @Req
	*
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APP_TWI_SENSOR_H__
#define __APP_TWI_SENSOR_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* System Modules */

#include "data_sensor_config.h"
#include "app_twi.h"
#include "app_util_platform.h"
#include "nrf_delay.h"

#include "nrf_drv_rtc.h"
#include "nrf_drv_ppi.h"

//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Header */

#ifdef __cplusplus
extern "C" {
#endif
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Type Declarations */

/** @Type LED Types */
typedef enum{
	RED,
	GREEN,
	BLUE
}led_type_t;

/** @Type Register Types */
typedef enum{
	LED_DATA_REG,
	LED_CTRL_REG,
	SENSOR_CTRL_REG
}reg_type_t;

/** @Type Data Type to Store the Color Sensor Settings */
typedef struct{
	// TWI Configurations
	nrf_drv_twi_config_t twi_config;
	
	// TWI Event Handler
	nrf_drv_twi_evt_handler_t twi_evt_handler;
	
	// LED Current Settings
	uint8_t					red_current;
	uint8_t 				green_current;
	uint8_t					blue_current;
}sensor_config_t;

/** @Type Data Type to Store the Color Sensor Flags */
typedef struct{
	bool twi_tx_flag;
	bool twi_rx_flag;
}sensor_flag_t;

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
void sensorTWIConfig(const uint8_t scl_pin_no, const uint8_t sda_pin_no, nrf_drv_twi_evt_handler_t twi_evt_handler, bool is_blocking);

/** @Func 	Set Sensor LED Current
	*
	* @Brief	This function sets the LED current to the specified LED	
	*
	*	@Para		current 	[uint8_t] 		: the LED current choices which should be of the size of a byte
	* @Para		led_type	[led_type_t] 	:	the LED type which can be choosen from RED,GREEN,BLUE
	*
*/
void sensorLedCurrentConfig(const uint8_t current, const led_type_t led_type);

/** @Func Set up the RTC Peripheral */
void sensorRTCConfig(void);

/** @Func Set up the PPI Channel */
void sensorPPIConfig(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Write the Data in A Specified Internal Register */
uint8_t sensorWriteByteArray(uint8_t * byte_array, const uint8_t array_length);

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
uint8_t sensorReadByteArray(uint8_t reg_name, uint8_t * byte_array, const uint8_t array_length);

/** @Func Read the Data in A Specified Internal Register After A Specified Time Delay */
uint8_t sensorReadByteArrayRTC(uint8_t reg_name, uint8_t * byte_array, const uint8_t array_length, const uint32_t delay_ms);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
uint8_t sensorReadColorData(uint8_t * byte_array, const uint8_t array_length);

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Get the Address of the Internal TWI Object */
nrf_drv_twi_t * sensorGetTWIObj(void);

/** @Func Accessors and Setters */
bool sensorGetTxFlag(void);
bool sensorGetRxFlag(void);
void sensorSetTxFlag(bool tx_flag);
void sensorSetRxFlag(bool rx_flag);
/** @Func Test Whether the Red LED is Currently On */
bool sensorIsRedOn(void);
/** @Func Test Whether the Green LED is Currently On */
bool sensorIsGreenOn(void);
/** @Func Test Whether the Blue LED is Currently On */
bool sensorIsBlueOn(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */
	
#ifdef __cplusplus
} 
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__APP_TWI_SENSOR_H__
