/** Library Name: app_adc.h
 * @Brief 	This library declares functions for ADC (sampling from the battery)
 * @Brief   This library only supports sampling triggered by Timer0 connected using PPI (non-blocking mode)
 *
 * @Auther 	Feng Yuan
 * @Time 		12/09/2017
 * @Version	1.0
 *
 * @Req			This library requires the following modules to function
 * @Req			- SAADC Module for nRF52 Series						(Include the header file "nrf_drv_saadc.h" and configure in "sdk_config.h")
 * @Req			- PPI Module															(Include the header file "nrf_drv_ppi.h" and configure in "sdk_config.h")
 * @Req			- Timer Module														(Include the header file "nrf_drv_timer.h" and configure in "sdk_config.h".The module uses timer 0.)
 * @Req			- GPIO Module															(Used to control the ADC enable pin. Do not need to configure in "sdk_config.h")
 *
 * @Macro		ADC_BUFFER_SIZE														(The default buffer size for the ADC module)
 * @Macro 	ADC_FIRST_CONVERSION_DELAY_MS							(The delay time before the first ADC conversion, waiting for the circuit to become stable)
 * @Macro		ADC_ENABLE_PIN_ACTIVE_STATE								(The ADC enable pin active state)
 *
 * @Type		adc_evt_handler_t													(Function Pointer Type for the ADC Completion Event Handler)
 * @Type		adc_filter_t															(Function Pointer Type for the ADC Sample Filter)
 * @Type		adc_config_t															(Data Type for the ADC Configuration Structure)
 *
 * @Func		adcBufferConfig														(Configuration of ADC Buffer Information)
 * @Func		adcSamplingConfig													(Configuration of ADC Sampling Rate and Post Processing Function)
 * @Func		adcChannelConfig													(Configuration of the ADC Peripheral)
 * @Func		adcEnPinConfig														(Configure the ADC enable pin as output)
 * @Func		adcPPIConfig															(Configuration of the PPI Peripheral)
 * @Func		adcPPIEnable															(Enable the PPI Channel)
 * @Func		adcPPIDisable															(Disable the PPI Channel)
 * @Func		adcSampleStart														(Start Taking a Sample [Single Shot])
 * @Func		adcSampleStop															(Stop Sampling [Single Shot])
 * @Func		adcSampleRead															(Read a Sample Data Out)
 * @Func		adcResultWrite														(Write the ADC Final Result into the Internal Variable)
 * @Func		adcGetConfig															(Get the Address of the ADC Configuration Variable)
 * @Func		adcIsResultReady													(Check whether the ADC processing is finished)
 * @Func		adcChannelDestruct												(Destruct the ADC Channel)
 * @Func		adcPPIDestruct														(Destruct the PPI Channel)
 *
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APP_ADC_H__
#define __APP_ADC_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** System Modules */

#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"
#include "nrf_gpio.h"
#include "nrf_drv_ppi.h"
#include "nrf_delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Macro Definitions */

/** @Macro The Size of the Default ADC Buffer */
#define ADC_BUFFER_SIZE									10

/** @Macro The Delay Before the First ADC Conversion */
#define ADC_FIRST_CONVERSION_DELAY_MS 	5

/** @Macro The ADC Enable Pin Active State */
#define ADC_ENABLE_PIN_ACTIVE_STATE			true
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/** @Type Function Pointer Type for the ADC Completion Event Handler */
typedef void (* adc_evt_handler_t)(nrf_drv_saadc_evt_t const *);

/** @Type Function Pointer Type for the ADC Sample Filter */
typedef uint16_t (* adc_filter_t)(uint16_t *, uint16_t);

/** @Type Data Type for the ADC Sampling Mode */
typedef enum{
	ADC_PPI_TRIGGERED,
	ADC_CPU_TRIGGERED
}adc_sa_mode_t;

/** @Type Data Type for the ADC Configuration Structure */
typedef struct{
	/* ADC channel settings */
  uint8_t										 	adc_channel_no;			 		// ADC channel number in use
	nrf_saadc_channel_config_t 	adc_channel_config;	 		// ADC channel configuration
	/* ADC buffer settings */
	uint16_t * 									adc_buffer_1_addr;			// Address of ADC buffer one
	uint16_t * 									adc_buffer_2_addr;			// Address of ADC buffer two
	uint16_t										adc_buffer_size;				// Buffer size
	/* ADC resolution and reference settings */
	uint16_t										adc_res_val;				 		// The resolution of the ADC in its real value
	float												adc_ref_voltage_v;			// The reference voltage used in ADC conversion
	/* ADC gain settings */
	float												adc_gain_factor;				// ADC gain factor
	/* ADC sampling rate settings */
	uint32_t										adc_sa_period_us;			  // Sampling period in us
	/* ADC post processing function (DSP) */
	adc_filter_t								adc_filter_ptr;					// The function pointer to the post-processing filter
}adc_config_t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Function Declarations for ADC Operations */

/** @Func Configuration of ADC Buffer Information
	*	
	*	@Brief This function allocates the buffers for ADC sampling
	* 
	* @Para adc_buffer_1 [uint16_t*] : the first adc buffer address
	* @Para adc_buffer_2 [uint16_t*] : the second adc buffer address
	* @Para buffer_size  [uint16_t]  : the size of the buffer
	* 
	* @Note The addresses of the two ADC buffers can be set to NULL, so that the internal ADC buffers are used.
	* @Note If only one buffer address is set to non-NULL value, then only one buffer is used for ADC. (double buffer function is automatically closed)
	*
*/
void adcBufferConfig(uint16_t * adc_buffer_1, uint16_t * adc_buffer_2, uint16_t buffer_size);

/** @Func Configuration of ADC Sampling Rate and Post Processing Function
  *
	* @Brief This function sets the gain factor and sampling rate of the ADC peripheral
	* @Brief This function also sets the post-processing filter for the acquired data
	* @Brief The ADC filter can be set to NULL so that the internal filter is employed
	* @Brief The internal filter automatically converts the ADC data into the real voltage value in milivolt. If external filter is used, then the acquired adc data is untouched.
	*
	* @Para adc_gain_factor 	[nrf_saadc_gain_t] 	: the ADC gain factor choice, which must be of the enumerate type defined in "nrf_drv_saadc.h" file
	* @Para adc_sampling_rate [uint32_t] 					: the sampling rate of ADC in Hz
	* @Para adc_filter 				[adc_filter_t] 			: the ADC post-processing
	*
*/
void adcSamplingConfig(nrf_saadc_gain_t adc_gain_factor, uint32_t adc_sampling_rate, adc_filter_t adc_filter);

/** @Func Configuration of the ADC Channel and Event Handler
	*
	* @Brief This function configures the ADC analog channel, setting the channel number used and the event handler for the ADC done event
	*
	* @Para adc_channel_no 			[uint8_t] 					: the pin number of the analog channel
	* @Para adc_evt_handler_ptr [adc_evt_handler_t]	: the event handler of the ADC done event
	*
	* @Note The ADC event handler can be set to NULL so that the ADC is only performed once
	*
*/
void adcChannelConfig(uint8_t adc_channel_no, adc_evt_handler_t adc_evt_handler_ptr);

/** @Func Configure the ADC enable pin as output
	*
	* @Brief This function configures the ADC enabling pin as output and initializes its value as 0 (ADC disabled)
	*
	* @Para pin_no [uint8_t] : the pin number of the ADC enabling pin
	*
*/
void adcEnPinConfig(uint8_t pin_no);

/** @Func Configure the ADC sampling timer
	*
	* @Brief This function configures the sampling rate control timer for the ADC
	* @Brief This function uses the Timer0
	*
*/
void adcTimerConfig(void);
	
/** @Func Configuration of the PPI Peripheral
	*
	* @Brief This function configures the PPI channel that connects the sampling rate control timer and the ADC
	*
*/
void adcPPIConfig(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Start Taking a Sample (Single Shot)
	*
	* @Brief This function starts ADC sampling
	* @Brief This function sets the ADC enabling pin, starts the sampling timer, enables the ppi channel and starts the first time ADC conversion
	*
*/
void adcSampleStart(void);

/** @Func Stop Taking a Sample
	*
	* @Brief This function stops the entire ADC sampling
	* @Brief This function clears the ADC enabling pin, stops the sampling timer, disables the ppi channel
	*
*/
void adcSampleStop(void);

/** @Func Read a Sample From the Buffer
	*
	* @Brief This function reads out the final ADC result which has already been processed by the filter
	*
	* @Return [uint16_t]: the final ADC value in milivolt
	*
*/
uint16_t adcResultRead(void);

/** @Func Write the ADC Final Result into the Internal Variable
	*
	* @Brief This function manually writes the final ADC result
	*
	* @Para res_val [uint16_t] : the value to be written into the internal final ADC result variable.
	*
*/
void adcResultWrite(uint16_t res_val);

/** @Func Change the ADC processing completion flag
	*
	* @Brief This function sets the internal final result ready flag
	*
	* @Para flag [bool] : the bool value to be set into the internal result ready flag
	*
*/
void adcSetResultReady(bool flag);

/** @Func Destruct the ADC Channel
	*
	* @Brief This function destroys the ADC analog channel
	* @Brief This function uninitializes the channel and the ADC peripheral
	*
*/
void adcChannelDestruct(void);

/** @Func Destruct the PPI Channel
	*
	* @Brief This function destroys the PPI channel
	* @Brief This function uninitializes the ppi channel and the ADC sampling timer
	*
*/
void adcPPIDestruct(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Get the Address of the ADC Configuration Variable
	*
	* @Brief This function returns the address of the internal ADC configuration variable
	*
	*	@Return [adc_config_t *] : the address of the internal ADC configuration variable 
	*
*/
adc_config_t * adcGetConfig(void);

/** @Func Check whether the ADC processing is finished
	*
	* @Brief This function checks whether the final ADC result is ready or not
	*
	* @Return [bool] : the state of the internal final ADC result ready flag
	*
*/
bool adcIsResultReady(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //__APP_ADC_H__
