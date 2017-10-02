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

/** @Variable Definition of an instance of nrf_drv_rtc for RTC0 */
static const nrf_drv_rtc_t rtc_obj = NRF_DRV_RTC_INSTANCE(2);

/** @Variable The Variable to Record the Internal RTC Event Handler */
static nrf_drv_rtc_handler_t sensor_rtc_event_handler_default = NULL;

/** @Variable Sensor Configuration */
static sensor_led_current_t	sensor_led_current =
{
	.led_red_current = RED_CURRENT,
	.led_green_current = GREEN_CURRENT,
	.led_blue_current = BLUE_CURRENT
};

/** @Variable The Temporary Data Structure to Store the Byte Array MetaData */
static sensor_data_t sensor_data = 
{
	.array 		= NULL,
	.length		= 0,
	.reg_name	= 0
};

/** @Variable The Variable to Record the LED On/Off Status */
static sensor_led_flags_t sensor_led_status =
{
	.is_led_red_on 		= false,
	.is_led_green_on 	= false,
	.is_led_blue_on 	= false
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Implementations (Internal Functions) */

/** @Func RTC Event Handler */
static void sensor_rtc_event_handler(nrf_drv_rtc_int_type_t int_type)
{
	if(int_type == NRF_DRV_RTC_INT_COMPARE0){
		// Stop the RTC
		nrf_drv_rtc_counter_clear(&rtc_obj);
		nrf_drv_rtc_disable(&rtc_obj);
		
		// Read the Acquired Data	
		APP_ERROR_CHECK(sensorReadByteArray(sensor_data.array, sensor_data.length, sensor_data.reg_name));

		// Put the Sensor into Sleep
		APP_ERROR_CHECK(sensorSleep());
	
		// Turn off LEDs
		if(sensor_led_status.is_led_red_on){//If LED Red is on, then turn it off
			sensorTurnOffLed(RED);
		}
		if(sensor_led_status.is_led_green_on){//If LED Green is on, then turn it off
			sensorTurnOffLed(GREEN);
		}
		if(sensor_led_status.is_led_blue_on){//If LED Blue is on, then turn it off
			sensorTurnOffLed(BLUE);
		}
	}
}

/** @Func Start the RTC2 Timer to Delay Specified Time(in milliseccond) */
static void sensor_delay_ms(uint32_t delay_time_ms)
{
	// Set up the comparator for the sensor(use the internal rtc_obj and comparator 0)
	APP_ERROR_CHECK(nrf_drv_rtc_cc_set(&rtc_obj,0,delay_time_ms,true));
	
	// Power on RTC instance
  nrf_drv_rtc_enable(&rtc_obj);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func TWI Configuration */
void sensorConfig(const uint8_t scl_pin_no, const uint8_t sda_pin_no, nrf_drv_rtc_handler_t rtc_event_handler, bool is_blocking_mode)
{
	// Configure the TWI Peripheral Parameters
	// Set the SCL and SDA pin numbers, basic TWI frequency and the interrupt priority
  nrf_drv_twi_config_t const config =
	{
		.scl                = scl_pin_no,
    .sda                = sda_pin_no,
    .frequency          = NRF_TWI_FREQ_400K,
    .interrupt_priority = APP_IRQ_PRIORITY_LOW
  };
	
	// Initialize the TWI Transaction Manager
	uint8_t err_code;
  APP_TWI_INIT(&twi_obj, &config, MAX_TWI_QUEUE_SIZE, err_code);
	APP_ERROR_CHECK(err_code);
	
	// Configuration Variable for the RTC Instance (Use RTC2 Here)
  nrf_drv_rtc_config_t rtc_config = NRF_DRV_RTC_DEFAULT_CONFIG;
	
	// Setting the Prescaler to 32 means the resolution of the counter is 1ms.
	// This precision is high enough for the color sensor delay.
  rtc_config.prescaler = 32;
	
	// Set Up the RTC Timer Time-out Event Handler
	if(is_blocking_mode){//If blocking mode is used, then no event handler is required
		sensor_rtc_event_handler_default = NULL;
	}
	else{
		if(rtc_event_handler == NULL){// No event handler is provided, then use the default one
			sensor_rtc_event_handler_default = sensor_rtc_event_handler;
		}
		else{// Use the externally provided event handler
			sensor_rtc_event_handler_default = rtc_event_handler;
		}
		
		// Create the RTC Timer for Sensor Delay
		APP_ERROR_CHECK(nrf_drv_rtc_init(&rtc_obj, &rtc_config, sensor_rtc_event_handler_default));
	}
}

/** @Func Sensor Configuration */
void sensorLedCurrentConfig(const uint8_t current, const led_type_t led_type)
{
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

/** @Func Write a Byte Array into the Sensor Register from the Specified Address */
uint8_t sensorWriteByteArray(uint8_t * byte_array, const uint8_t array_length, const uint8_t address)
{
	// Set up the data to be transmitted
	app_twi_transfer_t const data[] = {APP_TWI_WRITE(address, byte_array, array_length, 0)};
	
	// Work out the number of transfers
	uint8_t number_of_transfers = sizeof(data)/sizeof(data[0]);
	
	// Send data on I2C channal (Blocking/Synchronous Approach)
	return app_twi_perform(&twi_obj, data, number_of_transfers, NULL);
}

/** @Func Read Out the Register Data into a Byte Array from the Specified Address */
uint8_t sensorReadByteArray(uint8_t * byte_array, const uint8_t array_length, const uint8_t address)
{
	// Set up the data to be transmitted
	app_twi_transfer_t const data[] = {
		APP_TWI_WRITE(SENSOR_ADDRESS, &address, sizeof(address), APP_TWI_NO_STOP),\
		APP_TWI_READ(SENSOR_ADDRESS, byte_array, array_length, 0)
	};
	
	// Work out the number of transfers
	uint8_t number_of_transfers = sizeof(data)/sizeof(data[0]);
	
	// Send data on I2C channal (Blocking/Synchronous Approach)
	return app_twi_perform(&twi_obj, data, number_of_transfers, NULL);
}

/** @Func Run-time Configuration of the Color Sensor */
uint8_t sensorSetup(const uint16_t value)
{
	uint8_t setup_data[4] = {SENSOR_REG_CONTROL, SENSOR_SETUP, (value >> 8) & 0xff, value & 0xff};
	return sensorWriteByteArray(setup_data,4,SENSOR_ADDRESS);
}

/** @Func Turn On A Specified LED */
uint8_t sensorTurnOnLed(const led_type_t led_type)
{
	uint8_t err_code;
	uint8_t	current[3];
	uint8_t command[2];
	
	// Set the Common Register Address of the Color Sensor
	current[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
	command[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
	
	switch (led_type){
		case RED:
		{
			// Set the Commands
			current[1] = LED_OFF_COMMAND|sensor_led_current.led_red_current;
			current[2] = SENSOR_LED_GREEN_CURRENT_0MA|SENSOR_LED_BLUE_CURRENT_0MA;
			command[1] = LED_ON_COMMAND|sensor_led_current.led_red_current;
			// Write into the sensor registors through the TWI interface
			if((err_code = sensorWriteByteArray(current,3,SENSOR_ADDRESS)) == NRF_SUCCESS){
				if((err_code = sensorWriteByteArray(command,2,SENSOR_ADDRESS)) == NRF_SUCCESS){
					sensor_led_status.is_led_red_on = true;
				}
			}
			return err_code;
		}
		case GREEN:
		{
			// Set the Commands
			current[1] = LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			current[2] = sensor_led_current.led_green_current|SENSOR_LED_BLUE_CURRENT_0MA;
			command[1] = LED_ON_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			// Write into the sensor registors through the TWI interface
			if((err_code = sensorWriteByteArray(current,3,SENSOR_ADDRESS)) == NRF_SUCCESS){
				if((err_code = sensorWriteByteArray(command,2,SENSOR_ADDRESS)) == NRF_SUCCESS){
					sensor_led_status.is_led_green_on = true;
				}
			}
			return err_code;
		}
		case BLUE:
		{
			// Set the Commands
			current[1] = LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			current[2] = SENSOR_LED_GREEN_CURRENT_0MA|sensor_led_current.led_blue_current;
			command[1] = LED_ON_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			// Write into the sensor registors through the TWI interface
			if((err_code = sensorWriteByteArray(current,3,SENSOR_ADDRESS)) == NRF_SUCCESS){
				if((err_code = sensorWriteByteArray(command,2,SENSOR_ADDRESS)) == NRF_SUCCESS){
					sensor_led_status.is_led_blue_on = true;
				}
			}
			return err_code;
		}
		default:
			return NRF_ERROR_INVALID_PARAM;
	}
}

/** @Func Turn Off A Specified LED */
uint8_t sensorTurnOffLed(const led_type_t led_type)
{
	// Set the LED command
	uint8_t current[3] = 
	{ 
		SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1, \
		LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA, \
		SENSOR_LED_GREEN_CURRENT_0MA|SENSOR_LED_BLUE_CURRENT_0MA
	};
	
	// Write the byte array into the color sensor through TWI interface
	uint8_t err_code = sensorWriteByteArray(current, 3, SENSOR_ADDRESS);
	
	// Set the LED on/off status
	if(err_code == NRF_SUCCESS){
		switch(led_type){
			case RED:
				sensor_led_status.is_led_red_on = false;
				break;
			case GREEN:
				sensor_led_status.is_led_green_on = false;
				break;
			case BLUE:
				sensor_led_status.is_led_blue_on = false;
				break;
			default:
				return NRF_ERROR_INVALID_PARAM;
		}
	}
	return err_code;
}

/** @Func Start Color Sampling */
uint8_t sensorSampleStart(void)
{
	uint8_t data[2]  = {SENSOR_REG_CONTROL, START_SAMPLING};
	return sensorWriteByteArray(data,2,SENSOR_ADDRESS);
}

/** @Func Put the Sensor into Sleeping */
uint8_t sensorSleep(void)
{
	uint8_t data[2]  = {SENSOR_REG_CONTROL, SENSOR_SLEEP};
	return sensorWriteByteArray(data,2,SENSOR_ADDRESS);
}

/** @Func Read Out the Acquired Sensor Data */
uint8_t sensorReadData(uint8_t * byte_array, const uint8_t array_length)
{
	uint16_t timing = MANUAL_TIMING;
	uint8_t err_code;
	
	// Configure the Color Sensor into Manual Timing Mode
	if((err_code = sensorSetup(timing)) == NRF_SUCCESS){
		
		// Initial Delay
		// nrf_delay_ms(2);
		
		// Send the Sensor Sampling Command
		if((err_code = sensorSampleStart()) == NRF_SUCCESS){
			
			// Determine the Waiting Time
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
			
			// Non-blocking Mode
			if (sensor_rtc_event_handler_default != NULL){
				
				// Memorize the Temporary Data to be Passed into the Timer Event Handler
				sensor_data.array 		= byte_array;
				sensor_data.length 		= array_length;
				sensor_data.reg_name	=	SENSOR_REG_RED_DATA_HIGH_BYTE;
			
				// Start the Timer
				sensor_delay_ms(wait_time);
			}
			
			// Blocking Mode
			else{
				
				// Waiting till Sampling is Finished
				nrf_delay_ms(wait_time);
			
				// Read the Acquired Data
				err_code = sensorReadByteArray(byte_array,array_length,SENSOR_REG_RED_DATA_HIGH_BYTE);
			
				if(err_code == NRF_SUCCESS){
					return sensorSleep();	//Put the sensor into sleep
				}
			}
		}
	}
	return err_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Top Layer Functions */

/** @Func Sampling From All Three Channels */
uint8_t sensorSampleColor(uint8_t * byte_array, uint8_t array_length)
{
	// Check the Input Array Length
	if(array_length < 18){
		return NRF_ERROR_INVALID_LENGTH;
	}
	
	uint8_t err_code = NRF_SUCCESS;
	
	/* Non-blocking Mode */
	if(sensor_rtc_event_handler_default != NULL){
		
		// Sampling On the Red Channel
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		if((err_code = sensorTurnOnLed(RED)) != NRF_SUCCESS){return err_code;} 																										// Turn on Red LED
		if((err_code = sensorReadData(&byte_array[0],6)) != NRF_SUCCESS){return err_code;} 																				// Sample the Red Channel
	
		// Sampling On the Green Channel
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		if((err_code = sensorTurnOnLed(GREEN)) != NRF_SUCCESS){return err_code;} 																									// Turn on Green LED
		if((err_code = sensorReadData(&byte_array[6],6)) != NRF_SUCCESS){return err_code;} 																				// Sample the Green Channel

		// Sampling On the Blue Channel
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		if((err_code = sensorTurnOnLed(BLUE)) != NRF_SUCCESS){return err_code;} 																									// Turn on Blue LED
		if((err_code = sensorReadData(&byte_array[12],6)) != NRF_SUCCESS){return err_code;} 																			// Sample the Blue Channel
	
		// Wait Till All Sampling is Completed
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		return err_code;
	}
	
	/* Blocking Mode */
	else{
		
		// Sampling On the Red Channel
		if((err_code = sensorTurnOnLed(RED)) != NRF_SUCCESS){return err_code;} // Turn on the Red LED
		if((err_code = sensorReadData(&byte_array[0],6)) != NRF_SUCCESS){return err_code;} // Sample the Red Channel
		if((err_code = sensorTurnOffLed(RED)) != NRF_SUCCESS){return err_code;} // Turn off the Red LED
		
		// Sampling On the Green Channel
		if((err_code = sensorTurnOnLed(GREEN)) != NRF_SUCCESS){return err_code;} // Turn on the Green LED
		if((err_code = sensorReadData(&byte_array[6],6)) != NRF_SUCCESS){return err_code;} // Sample the Green Channel
		if((err_code = sensorTurnOffLed(GREEN)) != NRF_SUCCESS){return err_code;} // Turn off the Green LED
		
		// Sampling On the Blue Channel
		if((err_code = sensorTurnOnLed(BLUE)) != NRF_SUCCESS){return err_code;} // Turn on the Blue LED
		if((err_code = sensorReadData(&byte_array[12],6)) != NRF_SUCCESS){return err_code;} // Sample the Blue Channel
		if((err_code = sensorTurnOffLed(BLUE)) != NRF_SUCCESS){return err_code;} // Turn off the Blue LED
		
		return err_code;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Accessor Functions */

/** @Func Get the Address of the TWI Object */
app_twi_t * sensorGetInstance(void)
{
	return &twi_obj;
}

/** @Func Test Whether the Red LED is Currently On */
bool sensorIsRedOn(void)
{
	return sensor_led_status.is_led_red_on;
}

/** @Func Test Whether the Green LED is Currently On */
bool sensorIsGreenOn(void)
{
	return sensor_led_status.is_led_green_on;
}

/** @Func Test Whether the Blue LED is Currently On */
bool sensorIsBlueOn(void)
{
	return sensor_led_status.is_led_blue_on;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
