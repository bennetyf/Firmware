/** Library Name : app_twi_sensor.c
	*
	* @Brief This library defines color sensor applications through the I2C(also known as TWI) interface
	*
	* @Auther 	Feng Yuan
	* @Time 		25/09/2017
	* @Version	1.0
	*
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* System Modules */

#include "app_twi_sensor.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Variable Definintions */

/** @Variable TWI Instance */
static nrf_drv_twi_t 		twi_obj = NRF_DRV_TWI_INSTANCE(0);

/** @Variable Sensor Configuration */
static sensor_config_t	sensor_config;

/** @Varialbe TWI Transmission Completion Flags */
static sensor_flag_t 		sensor_flags;

/** @Variable Definition of an instance of nrf_drv_rtc for RTC0 */
const nrf_drv_rtc_t 		rtc_obj = NRF_DRV_RTC_INSTANCE(0);

/** @Variable Definition of a PPI Channel for Connection between RTC0 and TWI0 */
static nrf_ppi_channel_t sensor_ppi_channel;

/** @Variable The Variable to Record the LED On/Off Status */
static sensor_led_flags_t sensor_led_status =
{
	.is_led_red_on 		= false,
	.is_led_green_on 	= false,
	.is_led_blue_on 	= false
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Implementations */

/** @Func Internal Default Function as TWI Event Handler */
static void twi_evt_handler_default(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
	if(p_event->type == NRF_DRV_TWI_EVT_DONE){
		switch(p_event->xfer_desc.type){
			case NRF_DRV_TWI_XFER_TX:
			case NRF_DRV_TWI_XFER_TXTX:
			{
				sensor_flags.twi_tx_flag = true;
				break;
			}
			case NRF_DRV_TWI_XFER_RX:
			{
				sensor_flags.twi_rx_flag = true;
				break;
			}
			case NRF_DRV_TWI_XFER_TXRX:
			{
				sensor_flags.twi_tx_flag = true;
				sensor_flags.twi_rx_flag = true;
				
				// Stop the RTC
				nrf_drv_rtc_counter_clear(&rtc_obj);
				nrf_drv_rtc_disable(&rtc_obj);
				
				// Put the Sensor into Sleep
				APP_ERROR_CHECK(sensorSleep());
				
				// Turn off the LEDs
				if(sensor_led_status.is_led_red_on){//If LED Red is on, then turn it off
					sensorTurnOffLed(RED);
				}
				if(sensor_led_status.is_led_green_on){//If LED Green is on, then turn it off
					sensorTurnOffLed(GREEN);
				}
				if(sensor_led_status.is_led_blue_on){//If LED Blue is on, then turn it off
					sensorTurnOffLed(BLUE);
				}
				break;
			}
		}
	}
}

/** @Func Internal Function for Forcing the TWI Transfer into Blocking Mode */
static uint8_t force_xfer_blocking(uint8_t code,uint8_t mode)
{
	if(code != NRF_SUCCESS){
		return code;
	}
	else{
		switch (mode){
			case 't':case 'T':// Force blocking mode
				while(sensor_flags.twi_tx_flag == false){__WFE();}
				return code;
			case 'r':case 'R':// Force blocking mode
				while(sensor_flags.twi_rx_flag == false){__WFE();}
				return code;
			default:
				return code;
		}
	}
}

/** @Func RTC0 Event Handler */
static void rtc_event_handler(nrf_drv_rtc_int_type_t int_type)
{
	// RTC0 Event Handler Does Nothing
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func TWI Configuration */
void sensorTWIConfig(const uint8_t scl_pin_no, const uint8_t sda_pin_no, nrf_drv_twi_evt_handler_t twi_evt_handler, bool is_blocking)
{
	/* Set the TWI configuration structure */
	sensor_config.twi_config.scl 								= scl_pin_no;
	sensor_config.twi_config.sda 								= sda_pin_no;
	/* Settings byt the sdk_config.h file */
	sensor_config.twi_config.frequency 					= (nrf_twi_frequency_t)TWI_DEFAULT_CONFIG_FREQUENCY;
	sensor_config.twi_config.interrupt_priority	=	TWI_DEFAULT_CONFIG_IRQ_PRIORITY;
	sensor_config.twi_config.clear_bus_init			=	TWI_DEFAULT_CONFIG_CLR_BUS_INIT;
	sensor_config.twi_config.hold_bus_uninit		= TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT;
	
	/* Set the TWI event handler */
	if(is_blocking){
		twi_evt_handler = NULL;
	}
	else{
		if(twi_evt_handler == NULL){ // If TWI event handler is not specified, use the default event handler
			sensor_config.twi_evt_handler = twi_evt_handler_default;
		}
		else{
			sensor_config.twi_evt_handler = twi_evt_handler;
		}
	}
	
	/* Set the Color Sensor LED Currents to Defaults */
	sensor_config.red_current 	= RED_CURRENT;
	sensor_config.green_current = GREEN_CURRENT;
	sensor_config.blue_current 	= BLUE_CURRENT;
	
	/* Set the flags */
	sensor_flags.twi_tx_flag = false;
	sensor_flags.twi_rx_flag = false;
	
	/* Initialize the TWI instance */
	APP_ERROR_CHECK(nrf_drv_twi_init(&twi_obj, &sensor_config.twi_config, sensor_config.twi_evt_handler, NULL));
	
	/* Enable the TWI instance */
	nrf_drv_twi_enable(&twi_obj);
}

/** @Func Set Sensor LED Current */
void sensorLedCurrentConfig(const uint8_t current, const led_type_t led_type)
{
	switch (led_type){
		case RED:
			sensor_config.red_current 	= current;
			break;
		case GREEN:
			sensor_config.green_current = current;
			break;
		case BLUE:
			sensor_config.blue_current 	= current;
			break;
	}
}

/** @Func Set up the RTC Peripheral */
void sensorRTCConfig(void)
{
  //Initialize RTC instance
  nrf_drv_rtc_config_t rtc_config = NRF_DRV_RTC_DEFAULT_CONFIG;
	
	// Setting the Prescaler to 32 means the resolution of the counter is 1ms.
	// This precision is high enough for the color sensor delay.
  rtc_config.prescaler = 32;
	
	/* Initialize the RTC Instance */
  APP_ERROR_CHECK(nrf_drv_rtc_init(&rtc_obj, &rtc_config, rtc_event_handler));
}

/** @Func Set up the PPI Channel */
void sensorPPIConfig(void)
{
	// Initialize the PPI peripheral
	nrf_drv_ppi_init();
	
	// Allocate a free PPI channel
	APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&sensor_ppi_channel));
	
	// Get the trigger event (RTC0 Compare Event) and the task (TWI Register Reading) for PPI channel
	uint32_t sensor_rtc_event_addr 	= nrf_drv_rtc_event_address_get(&rtc_obj,NRF_RTC_EVENT_COMPARE_1);
	uint32_t sensor_twi_task_addr 	=	nrf_drv_twi_start_task_get(&twi_obj,NRF_DRV_TWI_XFER_TXRX);// RTC0 only triggers the register reading operation
	
	// Setup ppi channel so that timer compare event is triggering sample task in SAADC
	APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(sensor_ppi_channel, sensor_rtc_event_addr, sensor_twi_task_addr));
	
	//Enable the PPI Channel Between RTC0 and TWI0
	APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(sensor_ppi_channel));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Write the Data in A Specified Internal Register */
uint8_t sensorWriteByteArray(uint8_t * byte_array, const uint8_t array_length)
{
	nrf_drv_twi_xfer_desc_t 	xfer = NRF_DRV_TWI_XFER_DESC_TX(SENSOR_ADDRESS,byte_array,array_length);
	
	/* Start the transfer */
	sensor_flags.twi_tx_flag 	= false;
	return nrf_drv_twi_xfer(&twi_obj,&xfer,0);
}

/** @Func Read the Data in A Specified Internal Register */
uint8_t sensorReadByteArray(uint8_t reg_name, uint8_t * byte_array, const uint8_t array_length)
{
	nrf_drv_twi_xfer_desc_t 	xfer = NRF_DRV_TWI_XFER_DESC_TXRX(SENSOR_ADDRESS,&reg_name,1,byte_array,array_length);
	
	/* Start the transfer */
	sensor_flags.twi_tx_flag	= false;
	sensor_flags.twi_rx_flag	=	false;
	
	return nrf_drv_twi_xfer(&twi_obj,&xfer,0);
}

/** @Func Read the Data in A Specified Internal Register After A Specified Time Delay */
uint8_t sensorReadByteArrayRTC(uint8_t reg_name, uint8_t * byte_array, const uint8_t array_length, const uint32_t delay_time_ms)
{
	nrf_drv_twi_xfer_desc_t 	xfer 			= NRF_DRV_TWI_XFER_DESC_TXRX(SENSOR_ADDRESS,&reg_name,1,byte_array,array_length);
	uint8_t 									err_code 	= nrf_drv_twi_xfer(&twi_obj, &xfer, NRF_DRV_TWI_FLAG_HOLD_XFER);
	
	if(err_code == NRF_SUCCESS){
		//Prepare the TWI flags
		sensor_flags.twi_tx_flag = false;
		sensor_flags.twi_rx_flag = false;
		
		//Set compare channel to trigger the PPI task after delay_time_ms milliseconds
		APP_ERROR_CHECK(nrf_drv_rtc_cc_set(&rtc_obj,0,delay_time_ms,false));
		
		//Power on RTC instance
		nrf_drv_rtc_enable(&rtc_obj);
		
		return NRF_SUCCESS;
	}
	return err_code;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Turn On A Specified LED */
uint8_t sensorTurnOnLed(const led_type_t led_type)
{
	uint8_t err_code;
	uint8_t current[3];
	uint8_t command[2];
	
	current[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
	command[0] = SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1;
	
	switch (led_type){
		case RED:			// LED Current Settings
		{
			current[1] = LED_OFF_COMMAND|sensor_config.red_current;
			current[2] = SENSOR_LED_GREEN_CURRENT_0MA|SENSOR_LED_BLUE_CURRENT_0MA;
			command[1] = LED_ON_COMMAND|sensor_config.red_current;
			
			if((err_code = force_xfer_blocking(sensorWriteByteArray(current,3),'t')) == NRF_SUCCESS){
				if((err_code = force_xfer_blocking(sensorWriteByteArray(command,2),'t')) == NRF_SUCCESS){
					sensor_led_status.is_led_red_on = true;
				}
			}
			return err_code;
		}
		
		case GREEN:		// LED Current Settings
		{
			current[1] = LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			current[2] = sensor_config.green_current|SENSOR_LED_BLUE_CURRENT_0MA;
			command[1] = LED_ON_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			
			if((err_code = force_xfer_blocking(sensorWriteByteArray(current,3),'t')) == NRF_SUCCESS){
				if((err_code = force_xfer_blocking(sensorWriteByteArray(command,2),'t')) == NRF_SUCCESS){
					sensor_led_status.is_led_green_on = true;
				}
			}
			return err_code;
		}
		
		case BLUE:		// LED Current Settings
		{
			current[1] = LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			current[2] = SENSOR_LED_GREEN_CURRENT_0MA|sensor_config.blue_current;
			command[1] = LED_ON_COMMAND|SENSOR_LED_RED_CURRENT_0MA;
			
			if((err_code = force_xfer_blocking(sensorWriteByteArray(current,3),'t')) == NRF_SUCCESS){
				if((err_code = force_xfer_blocking(sensorWriteByteArray(command,2),'t')) == NRF_SUCCESS){
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
	uint8_t command[3] 	= {SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1,LED_OFF_COMMAND|SENSOR_LED_RED_CURRENT_0MA,SENSOR_LED_GREEN_CURRENT_0MA|SENSOR_LED_BLUE_CURRENT_0MA};
	uint8_t err_code 		= force_xfer_blocking(sensorWriteByteArray(command,3),'t');
	
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

/** @Func Configuration of the Color Sensor */
uint8_t sensorSetup(const uint16_t value)
{
	uint8_t setup_data[4] = {SENSOR_REG_CONTROL,SENSOR_SETUP,(value >> 8) & 0xFF,value & 0xFF};
	return force_xfer_blocking(sensorWriteByteArray(setup_data,4),'t');
}

/** @Func Start Color Sampling */
uint8_t sensorSampleStart(void)
{
	uint8_t command[2]	= {SENSOR_REG_CONTROL,START_SAMPLING};
	return force_xfer_blocking(sensorWriteByteArray(command,2),'t');
}

/** @Func Put the Sensor into Sleeping */
uint8_t sensorSleep(void)
{
	uint8_t command[2] 	= {SENSOR_REG_CONTROL,SENSOR_SLEEP};
	return force_xfer_blocking(sensorWriteByteArray(command,2),'t');
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Read the Sensor Data */
uint8_t sensorReadColorData(uint8_t * byte_array, const uint8_t array_length)
{
	uint16_t 	timing = MANUAL_TIMING;
	uint8_t 	err_code;
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
			
			// Blocking Mode
			if(sensor_config.twi_evt_handler == NULL){
				
				// Waiting till Sampling is Finished
				nrf_delay_ms(wait_time);
			
				// Read the Acquired Data
				err_code = force_xfer_blocking(sensorReadByteArray(SENSOR_REG_RED_DATA_HIGH_BYTE,byte_array,array_length),'r');
				
				//Put the sensor into sleep
				if(err_code == NRF_SUCCESS){
					return sensorSleep();
				}
			}
			
			// Non-blocking Mode
			else{
				return sensorReadByteArrayRTC(SENSOR_REG_RED_DATA_HIGH_BYTE,byte_array,array_length,wait_time);
			}
		}
	}
	return err_code;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Sampling From All Three Channels */
uint8_t sensorSampleColor(uint8_t * byte_array, uint8_t array_length)
{
	if(array_length < 18){
		return NRF_ERROR_INVALID_LENGTH;
	}
	
	uint8_t err_code = NRF_SUCCESS;
	
	/* Blocking Mode */
	if(sensor_config.twi_evt_handler == NULL){
		
		// Sampling On the Red Channel
		if((err_code = sensorTurnOnLed(RED)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorReadColorData(&byte_array[0],6)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorTurnOffLed(RED)) != NRF_SUCCESS){return err_code;}
	
		// Sampling On the Green Channel
		if((err_code = sensorTurnOnLed(GREEN)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorReadColorData(&byte_array[6],6)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorTurnOffLed(GREEN)) != NRF_SUCCESS){return err_code;}
	
		// Sampling On the Blue Channel
		if((err_code = sensorTurnOnLed(BLUE)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorReadColorData(&byte_array[12],6)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorTurnOffLed(BLUE)) != NRF_SUCCESS){return err_code;}
	
		return err_code;
	}
	
	/* Non-blocking Mode */
	else{
		// Sampling On the Red Channel
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		if((err_code = sensorTurnOnLed(RED)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorReadColorData(&byte_array[0],6)) != NRF_SUCCESS){return err_code;}
		
		// Sampling On the Green Channel		
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		if((err_code = sensorTurnOnLed(GREEN)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorReadColorData(&byte_array[6],6)) != NRF_SUCCESS){return err_code;}
		
		// Sampling On the Blue Channel
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		if((err_code = sensorTurnOnLed(BLUE)) != NRF_SUCCESS){return err_code;}
		if((err_code = sensorReadColorData(&byte_array[12],6)) != NRF_SUCCESS){return err_code;}
		
		while(sensor_led_status.is_led_red_on || sensor_led_status.is_led_green_on || sensor_led_status.is_led_blue_on){__WFE();} // Wait until all LEDs are turned off
		return err_code;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Get the Address of the Internal TWI Object */
nrf_drv_twi_t * sensorGetTWIObj(void)
{
	return &twi_obj;
}

/** @Func Test Whether the TWI Tx is Finished */
bool sensorGetTxFlag(void)
{
	return sensor_flags.twi_tx_flag;
}

/** @Func Test Whether the TWI Rx is Finished */
bool sensorGetRxFlag(void)
{
	return sensor_flags.twi_rx_flag;
}

/** @Func Set the TWI Tx Flag */
void sensorSetTxFlag(bool tx_flag)
{
	sensor_flags.twi_tx_flag = tx_flag;
}

/** @Func Set the TWI Rx Flag */
void sensorSetRxFlag(bool rx_flag)
{
	sensor_flags.twi_rx_flag = rx_flag;
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

