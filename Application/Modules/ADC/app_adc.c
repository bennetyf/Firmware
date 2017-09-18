/** Library Name: app_adc.h
 * @Brief 	This library declares functions for ADC (sampling from the battery)
 * @Brief   This library only supports sampling triggered by Timer0 connected using PPI (non-blocking mode)
 *
 * @Auther 	Feng Yuan
 * @Time 		12/09/2017
 * @Version	1.0
 *
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** System Modules */

#include "app_adc.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Variable Definitions */

/** @Variable ADC Default Buffers */
// Use the double buffer function in the ADC peripheral
static uint16_t adc_default_buffer1[ADC_BUFFER_SIZE];
static uint16_t adc_default_buffer2[ADC_BUFFER_SIZE];

/** @Variable The Variable to Store the Final ADC Result */
// Result after data post-processing
static uint16_t adc_final_result;

/** @Variable ADC Configuration Variable */
static adc_config_t adc_config;

/** @Variable ADC Enable Pin Number */
static uint8_t adc_en_pin_no;

/** @Variable Sampling Timer Instance Definition */
// Use Timer0 as the sampling timer to control the sampling rate
static const nrf_drv_timer_t adc_sa_timer = NRF_DRV_TIMER_INSTANCE(0);

/** @Variable PPI Channel */
// This PPI channel is used to connect the sampling timer(Timer0) to the ADC peripheral
static nrf_ppi_channel_t adc_ppi_channel;

/** @Variable ADC Final Result Ready Flag */
static bool adc_result_ready;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Function Implementation for ADC Operations */

/** @Func Convert the ADC Value into the Real Voltage in Milivolt */
static uint16_t adc_val_to_voltage(uint32_t adc_val)
{
	uint16_t tmp_voltage;
	float tmp 	= adc_val/((adc_config.adc_gain_factor / adc_config.adc_ref_voltage_v) * adc_config.adc_res_val);
  tmp_voltage = (uint16_t) (tmp* 1000); 	// Change the value into milivolts
	return ((tmp_voltage + 5) / 10) * 10;  	// Round the value.
}

/** @Func Default ADC Filter (Averaging) */
static uint16_t adc_filter_default(uint16_t * buffer_addr, uint16_t buffer_size)
{
	uint32_t sum = 0;
	for(int16_t i = 0; i < buffer_size; i++){
		sum += buffer_addr[i];
	}
	return (uint16_t)(adc_val_to_voltage(sum)/buffer_size);
}

/** @Func Default Timer Handler */
static void adc_sa_timer_handler(nrf_timer_event_t event_type, void * p_context)
{
}

/** @Func Default Event Handler for ADC EventDone */
// Only perform one ADC conversion
static void adc_evt_handler(nrf_drv_saadc_evt_t const * event)
{
	switch (event->type){
		case NRF_DRV_SAADC_EVT_DONE:
		{
			// Post-process the ADC data
			adc_final_result = adc_config.adc_filter_ptr((uint16_t *)(event->data.done.p_buffer),adc_config.adc_buffer_size);
			
			// Do nothing about the final result
			adc_result_ready = true;
			
			// Stop the ADC sampling
			adcSampleStop();
		}
	}
}

/** @Func Configuration of ADC Buffer Information */
void adcBufferConfig(uint16_t * adc_buffer_1, uint16_t * adc_buffer_2, uint16_t buffer_size)
{
	// Set the buffer information to default
	adc_config.adc_buffer_1_addr 		= NULL;
	adc_config.adc_buffer_2_addr 		= NULL;
	adc_config.adc_buffer_size 			= 0;
	
	// Configure the ADC buffer settings
	if(((adc_buffer_1 == NULL) && (adc_buffer_2 == NULL)) || (buffer_size == 0)){
		// Two ADC buffers are set to default
		adc_config.adc_buffer_1_addr 	= adc_default_buffer1;
		adc_config.adc_buffer_2_addr 	= adc_default_buffer2;
		adc_config.adc_buffer_size 		= ADC_BUFFER_SIZE;
		return;
	}
	if(adc_buffer_1 != NULL && buffer_size != 0){
		adc_config.adc_buffer_1_addr 	= adc_buffer_1;
		adc_config.adc_buffer_size 		= buffer_size;
	}
	if(adc_buffer_2 != NULL && buffer_size != 0){
		adc_config.adc_buffer_2_addr 	= adc_buffer_2;
		adc_config.adc_buffer_size 		= buffer_size;
	}
}

/** @Func Configuration of ADC Sampling Rate and Post Processing Function */
void adcSamplingConfig(nrf_saadc_gain_t adc_gain_factor, uint32_t adc_sampling_rate, adc_filter_t adc_filter)
{
	// Set the data resolution information
	switch (SAADC_CONFIG_RESOLUTION){
		case 0:
			adc_config.adc_res_val = 256;
			break;
		case 1:
			adc_config.adc_res_val = 1024;
			break;
		case 2:
			adc_config.adc_res_val = 4096;
			break;
		case 3:
			adc_config.adc_res_val = 16384;
			break;
	}
	
	// Configure the channel gain factor
	adc_config.adc_channel_config.gain = adc_gain_factor;
	
	switch (adc_gain_factor){//convert the enumerate gain factor into the real gain value (float type)
		case NRF_SAADC_GAIN1_6:
			adc_config.adc_gain_factor = 1/6;
			break;
		case NRF_SAADC_GAIN1_5:
			adc_config.adc_gain_factor = 0.2;
			break;
		case NRF_SAADC_GAIN1_4:
			adc_config.adc_gain_factor = 0.25;
			break;
		case NRF_SAADC_GAIN1_3:
			adc_config.adc_gain_factor = 1/3;
			break;
		case NRF_SAADC_GAIN1_2:
			adc_config.adc_gain_factor = 0.5;
			break;
		case NRF_SAADC_GAIN1:
			adc_config.adc_gain_factor = 1.0;
			break;
		case NRF_SAADC_GAIN2:
			adc_config.adc_gain_factor = 2.0;
			break;
		case NRF_SAADC_GAIN4:
			adc_config.adc_gain_factor = 4.0;
			break;
	}
	
	// Configure the ADC acquire time
	// Calculate the sampling period of the ADC peripheral
	adc_config.adc_sa_period_us = (uint32_t)(1.0e+6/adc_sampling_rate); //Convert the float value into the integer
	
	if(adc_config.adc_sa_period_us >= 50){
		adc_config.adc_channel_config.acq_time = NRF_SAADC_ACQTIME_40US;
	}
	else if(adc_config.adc_sa_period_us >= 30){
		adc_config.adc_channel_config.acq_time = NRF_SAADC_ACQTIME_20US;
	}
	else if(adc_config.adc_sa_period_us >= 20){
		adc_config.adc_channel_config.acq_time = NRF_SAADC_ACQTIME_10US;
	}
	else if(adc_config.adc_sa_period_us >= 3){
		adc_config.adc_channel_config.acq_time = NRF_SAADC_ACQTIME_3US;
	}
	else{
		adc_config.adc_sa_period_us = 0;
	}
	
	// Assign the post filter of the ADC acquired data
	if(adc_filter != NULL){// filter provided outside the module
		adc_config.adc_filter_ptr = adc_filter;
	}
	else{ // use the default adc post filter
		adc_config.adc_filter_ptr = adc_filter_default;
	}
}

/** @Func Configuration of the ADC Analog Channel */
void adcChannelConfig(uint8_t adc_channel_no, adc_evt_handler_t adc_evt_handler_ptr)
{
	uint8_t err_code;
	
	// Configure the common ADC channel peripheral settings
	adc_config.adc_channel_config.resistor_p 	=	NRF_SAADC_RESISTOR_DISABLED;
	adc_config.adc_channel_config.resistor_n 	=	NRF_SAADC_RESISTOR_DISABLED;
	adc_config.adc_channel_config.reference		=	NRF_SAADC_REFERENCE_INTERNAL;//Internal 0.6v reference
	adc_config.adc_channel_config.burst				= NRF_SAADC_BURST_DISABLED;
	adc_config.adc_channel_config.mode				= NRF_SAADC_MODE_SINGLE_ENDED;
	adc_config.adc_channel_config.pin_n				= NRF_SAADC_INPUT_DISABLED;
	
	// Set the ADC channel number
	adc_config.adc_channel_no									= adc_channel_no;
	
	// Set the reference voltage in the adc configuration structure
	// Internal voltage is 0.6 V
	adc_config.adc_ref_voltage_v							= 0.6;
	
	// Configure the ADC peripheral to the designated channel number
	switch (adc_channel_no){
		case 0:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN0;
			break;
		}
		case 1:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN1;
			break;
		}
		case 2:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN2;
			break;
		}
		case 3:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN3;
			break;
		}
		case 4:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN4;
			break;
		}
		case 5:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN5;
			break;
		}
		case 6:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN6;
			break;
		}
		case 7:
		{
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN7;
			break;
		}
		default:
		{// Use AIN6 as default due to the PCB design
			adc_config.adc_channel_config.pin_p		= NRF_SAADC_INPUT_AIN6;
			break;
		}
	}
	
	// Initialize the ADC peripheral
	if(adc_evt_handler_ptr != NULL){
		err_code = nrf_drv_saadc_init(NULL, adc_evt_handler_ptr);
	}
	else{
		err_code = nrf_drv_saadc_init(NULL, adc_evt_handler);
	}
	APP_ERROR_CHECK(err_code);
	
	// Initialize the ADC channel
  err_code = nrf_drv_saadc_channel_init(adc_channel_no, &adc_config.adc_channel_config);
  APP_ERROR_CHECK(err_code);
}

/** @Func Configure the ADC enable pin as output */
void adcEnPinConfig(uint8_t pin_no)
{
	// Set the internal variable
	adc_en_pin_no = pin_no;
	
	// Set the adc enable pin as output
	nrf_gpio_cfg_output(adc_en_pin_no);
	
	// Set the adc enable pin number to 0 (disable ADC)
	nrf_gpio_pin_write(adc_en_pin_no, (ADC_ENABLE_PIN_ACTIVE_STATE?false:true));
}

/** @Func Configure the ADC sampling timer */
void adcTimerConfig(void)
{
	uint8_t err_code;
	
	// Set up the timer peripheral
	nrf_drv_timer_config_t	adc_sa_timer_cfg	=	NRF_DRV_TIMER_DEFAULT_CONFIG;
	adc_sa_timer_cfg.bit_width 								= NRF_TIMER_BIT_WIDTH_32;
	
	err_code = nrf_drv_timer_init(&adc_sa_timer, &adc_sa_timer_cfg, adc_sa_timer_handler);
	APP_ERROR_CHECK(err_code);
	
	// Setup adc_sa_timer for compare event
	uint32_t adc_sa_ticks = nrf_drv_timer_us_to_ticks(&adc_sa_timer, adc_config.adc_sa_period_us);
	nrf_drv_timer_extended_compare(&adc_sa_timer, NRF_TIMER_CC_CHANNEL0, adc_sa_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);
}

/** @Func Configuration of the PPI Peripheral (Including the PPI Between GPIO and Timer, the PPI Between Timer and ADC) */
void adcPPIConfig(void)
{
	uint8_t err_code;
	// Initialize the PPI peripheral
	err_code = nrf_drv_ppi_init();
	APP_ERROR_CHECK(err_code);
	
	// Allocate a free PPI channel
	err_code = nrf_drv_ppi_channel_alloc(&adc_ppi_channel);
	APP_ERROR_CHECK(err_code);
	
	// Get the trigger event (timer time-out) and the task (adc conversion) for PPI channel
	uint32_t adc_sa_event_addr 	= nrf_drv_timer_compare_event_address_get(&adc_sa_timer, NRF_TIMER_CC_CHANNEL0);
	uint32_t adc_sa_task_addr   = nrf_drv_saadc_sample_task_get();

	// Setup ppi channel so that timer compare event is triggering sample task in SAADC
	err_code = nrf_drv_ppi_channel_assign(adc_ppi_channel, adc_sa_event_addr, adc_sa_task_addr);
	APP_ERROR_CHECK(err_code);
}

/** @Func Start Taking a Sample (Single Shot) */
void adcSampleStart(void)
{
	uint8_t err_code;
	
	// Pull up the ADC enable GPIO pin
	nrf_gpio_pin_write(adc_en_pin_no,(ADC_ENABLE_PIN_ACTIVE_STATE?true:false));
	
	// Wait till the battery voltage is stable
	nrf_delay_ms(ADC_FIRST_CONVERSION_DELAY_MS);
	
	// Set the ADC result notification flag
	adc_result_ready = false;
	
	// Start the timer
	nrf_drv_timer_clear(&adc_sa_timer);
	nrf_drv_timer_enable(&adc_sa_timer);
	
	// Enable the PPI channel
	err_code = nrf_drv_ppi_channel_enable(adc_ppi_channel);
	APP_ERROR_CHECK(err_code);
	
	// First ADC conversion
	if(adc_config.adc_buffer_1_addr != NULL){
		err_code = nrf_drv_saadc_buffer_convert((nrf_saadc_value_t *)(adc_config.adc_buffer_1_addr), adc_config.adc_buffer_size);
		APP_ERROR_CHECK(err_code);
	}
	if(adc_config.adc_buffer_2_addr != NULL){
		err_code = nrf_drv_saadc_buffer_convert((nrf_saadc_value_t *)(adc_config.adc_buffer_2_addr), adc_config.adc_buffer_size);
		APP_ERROR_CHECK(err_code);
	}
}

/** @Func Stop Taking a Sample */
void adcSampleStop(void)
{
	uint8_t err_code;
	// Disable the PPI channel
	err_code = nrf_drv_ppi_channel_disable(adc_ppi_channel);
	APP_ERROR_CHECK(err_code);
	
	// Stop the timer
	nrf_drv_timer_disable(&adc_sa_timer);
	
	// Pull down the ADC enable GPIO pin
	nrf_gpio_pin_write(adc_en_pin_no,(ADC_ENABLE_PIN_ACTIVE_STATE?false:true));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Destruct the ADC Channel */
void adcChannelDestruct(void)
{
	uint8_t err_code;
	
	// Destruct the ADC channel
	err_code = nrf_drv_saadc_channel_uninit(adc_config.adc_channel_no);
	APP_ERROR_CHECK(err_code);
	
	// Destruct the ADC peripheral
	nrf_drv_saadc_uninit();
}

/** @Func Destruct the PPI Channel */
void adcPPIDestruct(void)
{
	uint8_t err_code;
	// Destruct the PPI channel
	err_code = nrf_drv_ppi_channel_free(adc_ppi_channel);
	APP_ERROR_CHECK(err_code);
	err_code = nrf_drv_ppi_uninit();
	APP_ERROR_CHECK(err_code);
	
	// Destruct the Timer
	nrf_drv_timer_uninit(&adc_sa_timer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Read the ADC Final Result From the Internal Variable */
uint16_t adcResultRead(void)
{
	return adc_final_result;
}

/** @Func Write the ADC Final Result into the Internal Variable */
void adcResultWrite(uint16_t res_val)
{
	adc_final_result = res_val;
}

/** @Func Set the ADC processing completion flag */
void adcSetResultReady(bool flag)
{
	adc_result_ready = flag;
}

/** @Func Get the Address of the ADC Configuration Variable */
adc_config_t * adcGetConfig(void)
{
	return &adc_config;
}

/** @Func Check whether the ADC processing is finished */
bool adcIsResultReady(void)
{
	return adc_result_ready;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
