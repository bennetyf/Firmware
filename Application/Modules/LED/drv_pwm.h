/** Library Name: "drv_pwm.h" 
 * @Brief 	This library declares functions for low-level pwm drivers
 *
 * @Auther 	Feng Yuan
 * @Time 		25/08/2017
 * @Version	1.0
 *
 * @Req 		This library requires the following modules to function
 * @Req 		- PWM Driver SDK Module 											(Configured in sdk_config.h)
 * @Req			- GPIO Driver SDK Module											(Configured in sdk_config.h)
 * @Req			- APP Utility SDK Module 											(Defined in app_util_platform.c)
 *
 * @Type 		- pwm_instance_rgb_t													(PWM Instance Type)
 * @Type		-	pwm_sequence_rgb_t													(PWM Sequence Type)
 * @Type		-	pwm_config_rgb_t														(PWM Configuration Structure Type)
 * @Type		- pwm_pin_val_rgb_t														(PWM Pin Value Storage Type)
 * @Type		- pwm_status_flags_rgb_t											(PWM Running Status Flag Type)
 * @Type		- pwm_run_mode_t															(PWM Running Mode Type)
 * @Type		- pwm_stop_mode_t															(PWM Stom Mode Type)
 *
 * @Func		- pwmSetup																		(Set up the PWM configuration structure)
 * @Func		- pwmRun																			(Run the PWM sequence on one channel)
 * @Func		- pwmUpdateSeq																(Update the PWM sequence on an existing instance)
 * @Func		-	pwmStop																			(Stop PWM sequence)
 *
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Modules Used in This Library */

#include "nrf_drv_pwm.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_gpio.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Declare the Basic Data Types */
	
/** @Type Declare the data type to store PWM instances */

typedef struct
{
	nrf_drv_pwm_t pwm_inst_red;
	nrf_drv_pwm_t pwm_inst_green;
	nrf_drv_pwm_t	pwm_inst_blue;
}pwm_instance_rgb_t;

/** @Type Declare the data type to store the PWM sequences */
typedef struct
{
	nrf_pwm_sequence_t pwm_seq_red;
	nrf_pwm_sequence_t pwm_seq_green;
	nrf_pwm_sequence_t pwm_seq_blue;
}pwm_sequence_rgb_t;

/** @Type Declare the data type to store the PWM configuration structures */
typedef struct
{
	nrf_drv_pwm_config_t pwm_config_red;
	nrf_drv_pwm_config_t pwm_config_green;
	nrf_drv_pwm_config_t pwm_config_blue;
}pwm_config_rgb_t;

/** @Type Declare the data type to store/restore the pin values */
typedef struct
{
	uint32_t pwm_pin_val_red;
	uint32_t pwm_pin_val_green;
	uint32_t pwm_pin_val_blue;
}pwm_pin_val_rgb_t;

/** @Type Declare the data type to store the PWM runing status flags */
typedef struct
{
	uint8_t pwm_running_flag_red;
	uint8_t pwm_running_flag_green;
	uint8_t pwm_running_flag_blue;
}pwm_status_flags_rgb_t;

/** @Type Declare the data type to represent the PWM sequence running mode (finite loop or infinite loop) */
typedef enum
{
	PWM_RUN_MODE_FINITE_LOOP = 0,
	PWM_RUN_MODE_INFINITE_LOOP
}pwm_run_mode_t;

/** @Type Declare the data type to represent the PWM sequence stop mode(stop,restore previous LED state or stop,turn off the LED)*/
typedef enum
{
	PWM_STOP_MODE_TURNOFF	=	0,
	PWM_STOP_MODE_RESTORE
}pwm_stop_mode_t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* PWM Driver Functions */

/** @Func Setup the Duty Cycle Sequence in One Channel
	*
	* @Brief This function is used to setup the PWM configuration structures and the sequences in each channel
	* 
	* @Para clk 				[nrf_pwm_clk_t clk]: 	The basic clock frequency of the PWM module
	* @Para cycle_top		[uint16_t]:						The top value in each duty cycle
	*																					(this value together with the clock frquency determines the pulse width for each pwm wave)
	* @Para seq_data   	[uint16_t*]: 					The pointer to the data sequence of the PWM wave in the designated channel
	* @Para seq_length  [uint16_t]:						The length of the data sequence for the designated channel
	* @Para pin					[uint8_t]: 						The output pin number of the PWM channel
	* @Para channel 		[char]: 							The selection of one of the RGB channels (must be 'r','g','b',or 'R','G','B')
	*
*/
void pwmSetup(const nrf_pwm_clk_t clk, const uint16_t cycle_top, const uint16_t * const seq_data, const uint16_t seq_length, const uint8_t pin, const char channel);


/** @Func Run the Sequence in One Channel
	*
	* @Brief This function is used to initialize the PWM peripheral for the designated channel
	* 
	* @Para channel			[char]:								The selection of one of the RGB channels (must be 'r','g','b',or 'R','G','B')
	* @Para run_mode		[pwm_run_mode_t]:			The PWM sequence running mode (PWM_RUN_MODE_FINITE_LOOP,PWM_RUN_MODE_INFINITE_LOOP)
	* @Para loop_times	[uint16_t]:						The total number of loops to be executed when running the sequence
	*
*/
void pwmRun(const char channel, const pwm_run_mode_t run_mode, const uint16_t loop_times);


/** @Func Update the Data Sequence of One Channel
	*
	* @Brief This function is used to update the sequence of one channel
	*
	* @Para seq_data   	[uint16_t*]:					The pointer to the data sequence of the PWM wave in the designated channel
	* @Para seq_length  [uint16_t]:						The length of the data sequence for the designated channel
	* @Para channel 		[char]:								The selection of one of the RGB channels (must be 'r','g','b',or 'R','G','B')
	*
*/
void pwmUpdateSeq(const uint16_t * const seq_data, const uint16_t seq_length, const char channel);


/** @Func Stop the Sequence in One Channel
	*
	* @Brief This function is used to stop the PWM periperal for the designated channel
	* @Brief This function also releases the PWM resource
	* 
	* @Para  channel		[char]: 							The selection of one of the RGB channels (must be 'r','g','b',or 'R','G','B')
	* @Para  mode				[uint8_t]:						PWM_STOP_MODE_TURNOFF is for turning off the LED after stopping the PWM, 
	*																					and PWM_STOP_MODE_RESTORE is for restoring the LED previous state after stopping the PWM
	*	@Para  led_active_state [bool]:					The active state of the LED(true for active high, false for active low)
	*
*/
void pwmStop(const char channel, const pwm_stop_mode_t mode, const bool led_active_state);


/** @Func Get PWM Status
	*
	* @Brief This function is used to check whether a PWM channel is running
	*
	* @Para  channel		[char]: 							The selection of one of the RGB channels (must be 'r','g','b',or 'R','G','B')
	*
*/
bool pwmIsRunning(const char channel);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DRV_PWM_H__
