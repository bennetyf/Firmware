/** Library Name : app_sensor.c
	*
	* @Brief
	*
	* @Auther 	Feng Yuan
	* @Time 		18/09/2017
	* @Version	1.0
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* System Modules */
#include "app_sensor.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Variable Definitions */

/** @Variable TWI Instance */
static app_twi_t twi_obj = APP_TWI_INSTANCE(0);

/** @Variable Sensor Configuration */
static sensor_led_current_t	sensor_led_current;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Implementations */

/** @Func TWI Configuration */
void twiConfig(uint8_t scl_pin_no, uint8_t sda_pin_no)
{
  nrf_drv_twi_config_t const config =
	{
		.scl                = scl_pin_no,
    .sda                = sda_pin_no,
    .frequency          = NRF_TWI_FREQ_400K,
    .interrupt_priority = APP_IRQ_PRIORITY_LOW
  };
	uint8_t err_code;
	APP_TWI_INIT(&twi_obj, &config, MAX_TWI_QUEUE_SIZE, err_code);
	APP_ERROR_CHECK(err_code);
}

/** @Func Sensor Configuration */
void sensorSetLedCurrent(uint8_t current, led_type_t led_type)
{
	// Set up the led current
	sensor_led_current.led_red_current 		= RED_CURRENT;
	sensor_led_current.led_green_current 	= GREEN_CURRENT;
	sensor_led_current.led_blue_current		=	BLUE_CURRENT;
	
	switch (led_type){
		case RED:
			sensor_led_current.led_red_current = current;
			break;
		case GREEN:
			sensor_led_current.led_green_current = current;
			break;
		case BLUE:
			sensor_led_current.led_blue_current = current;
			break;
	}
}

/** @Func Configuration of the Color Sensor */
uint8_t sensorSetup(uint16_t value)
{
	uint8_t setup_data[4] = {SENSOR_REG_CONTROL, SENSOR_SETUP, (value >> 8) & 0xff, value & 0xff};
	return twiWriteByteArray(setup_data,4,SENSOR_ADDRESS);
}

/** @Func Write a Byte Array into the Sensor Register from the Specified Address */
uint8_t twiWriteByteArray(uint8_t * byte_array, uint8_t array_length, uint8_t address)
{
	// Set up the data to be transmitted
	app_twi_transfer_t const data[] = {
		APP_TWI_WRITE(address, byte_array, array_length, 0)
	};
	
	// Work out the number of transfers
	uint8_t number_of_bytes = sizeof(data)/sizeof(data[0]);
	
	// Send data on I2C channal (Blocking/Synchronous Approach)
	return app_twi_perform(&twi_obj, data, number_of_bytes, NULL);
}

/** @Func Read Out the Register Data into a Byte Array from the Specified Address */
uint8_t twiReadByteArray(uint8_t * byte_array, uint8_t array_length, uint8_t address)
{
	// Set up the data to be transmitted
	app_twi_transfer_t const data[] = {
		APP_TWI_WRITE(address, &address, sizeof(address), APP_TWI_NO_STOP),\
		APP_TWI_READ (address, byte_array, array_length, 0)
	};
	
	// Work out the number of transfers
	uint8_t number_of_bytes = sizeof(data)/sizeof(data[0]);
	
	// Send data on I2C channal (Blocking/Synchronous Approach)
	return app_twi_perform(&twi_obj, data, number_of_bytes, NULL);
}

/** @Func Turn On A Specified LED */
uint8_t sensorTurnOnLed(led_type_t led_type)
{
	uint8_t err_code;
	uint8_t	current[3];
	uint8_t command[2];
	
	switch (led_type){
		case RED:
		{
			// LED Current Settings
			current[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
			current[1] = LED_OFF_COMMAND|sensor_led_current.led_red_current;
			current[2] = SENSOR_LED_GREEN_CURRENT_0MA|SENSOR_LED_BLUE_CURRENT_0MA;
			// Command
			command[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
			command[1] = LED_ON_COMMAND|sensor_led_current.led_red_current;
			break;
		}
		case GREEN:
		{
			// LED Current Settings
			current[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
			current[1] = LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			current[2] = sensor_led_current.led_green_current|SENSOR_LED_BLUE_CURRENT_0MA;
			// Command
			command[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
			command[1] = LED_ON_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			break;
		}
		case BLUE:
		{
			// LED Current Settings
			current[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
			current[1] = LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			current[2] = SENSOR_LED_GREEN_CURRENT_0MA|sensor_led_current.led_blue_current;
			// Command
			command[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
			command[1] = LED_ON_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			break;
		}
	}
	// Write into the sensor registors through the TWI interface
	if((err_code = twiWriteByteArray(current,3,SENSOR_ADDRESS)) == NRF_SUCCESS){
		return twiWriteByteArray(command,2,SENSOR_ADDRESS);
	}
	return err_code;
}

/** @Func Turn Off A Specified LED */
uint8_t sensorTurnOffLed(void)
{
	uint8_t current[] = { SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1, \
												LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA, \
												SENSOR_LED_GREEN_CURRENT_0MA|SENSOR_LED_BLUE_CURRENT_0MA};
	return twiWriteByteArray(current, sizeof(current), SENSOR_ADDRESS);
}

/** @Func Read Out the Acquired Sensor Data */
uint8_t sensorReadDataAll(uint8_t * byte_array, uint8_t array_length)
{
	uint16_t timing = MANUAL_TIMING;
	uint8_t err_code;
	if((err_code = sensorSetup(timing)) == NRF_SUCCESS){
		// Initial Delay
		nrf_delay_ms(2);
		
		if((err_code = sensorSampleStart()) == NRF_SUCCESS){
			// Determine the waiting time
			uint32_t wait_time;
			switch(INTEGRATION_TIME){
				case SENSOR_CTRL_INT_TIME_SETTING_32US:
					wait_time = (uint32_t)(0.032 * MANUAL_TIMING * 4 * POST_SAMPLE_WAIT_TIME_MARGIN);
					break;
				case SENSOR_CTRL_INT_TIME_SETTING_1MS:
					wait_time = (uint32_t)(1 * MANUAL_TIMING * 4 * POST_SAMPLE_WAIT_TIME_MARGIN);
					break;
				case SENSOR_CTRL_INT_TIME_SETTING_16MS:
					wait_time = (uint32_t)(16.0 * MANUAL_TIMING * 4 * POST_SAMPLE_WAIT_TIME_MARGIN);
					break;
				case SENSOR_CTRL_INT_TIME_SETTING_131MS:
					wait_time = (uint32_t)(131.0 * MANUAL_TIMING * 4 * POST_SAMPLE_WAIT_TIME_MARGIN);
					break;
				default:
					wait_time = (uint32_t)(0.032 * MANUAL_TIMING * 4 * POST_SAMPLE_WAIT_TIME_MARGIN);
					break;
			}
			
			// Waiting till Sampling is Finished
			nrf_delay_ms(wait_time);
			
			// Read the Acquired Data
			err_code = twiReadByteArray(byte_array,array_length,SENSOR_REG_RED_DATA_HIGH_BYTE);
			
			if(err_code == NRF_SUCCESS){
				return sensorSleep();	//Put the sensor into sleep
			}
		}
	}
	return err_code;
}

/** @Func Read the Settings in A Specified Internal Register */
uint8_t sensorReadRegister(uint8_t * byte_array, uint8_t array_length, reg_type_t reg_type)
{
	switch (reg_type){
		case LED_CTRL_REG:
			return twiReadByteArray(byte_array,array_length,SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1);
		case SENSOR_CTRL_REG:
			return twiReadByteArray(byte_array,array_length,SENSOR_REG_CONTROL);
		default:
			return NRF_ERROR_INVALID_PARAM;
	}
}

/** @Func Start Color Sampling */
uint8_t sensorSampleStart(void)
{
	uint8_t data[2] 	= {SENSOR_REG_CONTROL, START_SAMPLING};
	return twiWriteByteArray(data,2,SENSOR_ADDRESS);
}

/** @Func Put the Sensor into Sleeping */
uint8_t sensorSleep(void)
{
	uint8_t data[2]  = {SENSOR_REG_CONTROL,SENSOR_SLEEP};
	return twiWriteByteArray(data,2,SENSOR_ADDRESS);
}

/** @Func Sampling From All Three Channels */
uint8_t sensorSampleColor(uint8_t * byte_array, uint8_t array_length)
{
	if(array_length < 18){
		return NRF_ERROR_INVALID_LENGTH;
	}
	
	uint8_t err_code = NRF_SUCCESS;
	
	// Sampling On the Red Channel
	if((err_code = sensorTurnOnLed(RED)) != NRF_SUCCESS){return err_code;}
	if((err_code = sensorReadDataAll(&byte_array[0],6)) != NRF_SUCCESS){return err_code;}
	if((err_code = sensorTurnOffLed()) != NRF_SUCCESS){return err_code;}
	
	// Sampling On the Green Channel
	if((err_code = sensorTurnOnLed(GREEN)) != NRF_SUCCESS){return err_code;}
	if((err_code = sensorReadDataAll(&byte_array[6],6)) != NRF_SUCCESS){return err_code;}
	if((err_code = sensorTurnOffLed()) != NRF_SUCCESS){return err_code;}
	
	// Sampling On the Blue Channel
	if((err_code = sensorTurnOnLed(BLUE)) != NRF_SUCCESS){return err_code;}
	if((err_code = sensorReadDataAll(&byte_array[12],6)) != NRF_SUCCESS){return err_code;}
	if((err_code = sensorTurnOffLed()) != NRF_SUCCESS){return err_code;}
	
	return err_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
