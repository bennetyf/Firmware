/** Library Name: "drv_pwm.c" 
	* @Brief 		This library implements the functions for low-level pwm drivers
	*
	* @Auther 	Feng Yuan
	* @Time 		25/08/2017
	* @Version	1.0
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Module Used in This Library */

#include "drv_pwm.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Internal Variable Definitions */

/** @Variable Internal variables for the pwm instances in the RGB channels */
static pwm_instance_rgb_t				pwm_obj;
/** @Variable Internal variables for the pwm sequences in the RGB channels */
static pwm_sequence_rgb_t				pwm_seq;
/** @Variable Internal variables for the pwm configurations in the RGB channels */
static pwm_config_rgb_t 				pwm_config;
/** @Variable Internal variables for the pwm to restore the values before pwm is started */
static pwm_pin_val_rgb_t 				pwm_pin_values;
/** @Variable Internal variables to flag the running stauts of the pwm modules */
static pwm_status_flags_rgb_t		pwm_flags =
{
	.pwm_running_flag_red		=	false,
	.pwm_running_flag_green	=	false,
	.pwm_running_flag_blue	=	false
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* PWM Driver Function Implementations */

/** @Func Setup the Duty Cycle Sequence in One Channel */
void pwmConfig(const nrf_pwm_clk_t clk, const uint16_t cycle_top, const uint16_t * const seq_data, const uint16_t seq_length, const uint8_t pin, const char channel)
{
	// Setup the Configuration Structure and Sequence Structure of the Selected Channel
	switch (channel){
		// Red LED Channel
		case 'r':
		case 'R':
		{
			// Store the Pin Value
			nrf_gpio_cfg_input(pin,NRF_GPIO_PIN_NOPULL);
		  pwm_pin_values.pwm_pin_val_red	=	nrf_gpio_pin_read(pin);
			
			// Assign Red Channel to PWM0
			pwm_obj.pwm_inst_red.p_registers		=	CONCAT_2(NRF_PWM, 0);
			pwm_obj.pwm_inst_red.drv_inst_idx		=	CONCAT_3(PWM, 0, _INSTANCE_INDEX);
			
			// Setup the Configuration Structure for Red Channel
			pwm_config.pwm_config_red.output_pins[0] =	pin | NRF_DRV_PWM_PIN_INVERTED;
			pwm_config.pwm_config_red.output_pins[1] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_red.output_pins[2] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_red.output_pins[3] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_red.irq_priority	 =	APP_IRQ_PRIORITY_LOWEST;
			pwm_config.pwm_config_red.count_mode		 =	NRF_PWM_MODE_UP;
			pwm_config.pwm_config_red.load_mode			 = 	NRF_PWM_LOAD_COMMON;
			pwm_config.pwm_config_red.step_mode			 = 	NRF_PWM_STEP_AUTO;
			pwm_config.pwm_config_red.base_clock		 = 	clk;
			pwm_config.pwm_config_red.top_value			 = 	cycle_top;
			
			// Setup the Sequence Structure
			pwm_seq.pwm_seq_red.values.p_common			 =	seq_data;
			pwm_seq.pwm_seq_red.length							 = 	seq_length;
			pwm_seq.pwm_seq_red.repeats							 =	0;
			pwm_seq.pwm_seq_red.end_delay						 =	0;
			
			return;
		}
		
		// Green LED Channel
		case 'g':
		case 'G':
		{
			// Store the Pin Value
			nrf_gpio_cfg_input(pin,NRF_GPIO_PIN_NOPULL);
			pwm_pin_values.pwm_pin_val_green	=	nrf_gpio_pin_read(pin);
			
			// Assign Green Channel to PWM1
			pwm_obj.pwm_inst_green.p_registers	=	CONCAT_2(NRF_PWM, 1);
			pwm_obj.pwm_inst_green.drv_inst_idx	=	CONCAT_3(PWM, 1, _INSTANCE_INDEX);
			
			// Setup the Configuration Structure for Green Channel
			pwm_config.pwm_config_green.output_pins[0] =	pin | NRF_DRV_PWM_PIN_INVERTED;
			pwm_config.pwm_config_green.output_pins[1] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_green.output_pins[2] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_green.output_pins[3] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_green.irq_priority	 =	APP_IRQ_PRIORITY_LOWEST;
			pwm_config.pwm_config_green.count_mode		 =	NRF_PWM_MODE_UP;
			pwm_config.pwm_config_green.load_mode			 = 	NRF_PWM_LOAD_COMMON;
			pwm_config.pwm_config_green.step_mode			 = 	NRF_PWM_STEP_AUTO;
			pwm_config.pwm_config_green.base_clock		 = 	clk;
			pwm_config.pwm_config_green.top_value			 = 	cycle_top;
			
			// Setup the Sequence Structure
			pwm_seq.pwm_seq_green.values.p_common			 =	seq_data;
			pwm_seq.pwm_seq_green.length							 = 	seq_length;
			pwm_seq.pwm_seq_green.repeats							 =	0;
			pwm_seq.pwm_seq_green.end_delay						 =	0;
			
			return;
		}
		
		// Blue LED Channel
		case 'b':
		case 'B':
		{
			// Store the Pin Value
			nrf_gpio_cfg_input(pin,NRF_GPIO_PIN_NOPULL);
			pwm_pin_values.pwm_pin_val_blue	=	nrf_gpio_pin_read(pin);
			
			// Assign Blue Channel to PWM2
			pwm_obj.pwm_inst_blue.p_registers		=	CONCAT_2(NRF_PWM, 2);
			pwm_obj.pwm_inst_blue.drv_inst_idx	=	CONCAT_3(PWM, 2, _INSTANCE_INDEX);
			
			// Setup the Configuration Structure for Blue Channel
			pwm_config.pwm_config_blue.output_pins[0] =	pin | NRF_DRV_PWM_PIN_INVERTED;
			pwm_config.pwm_config_blue.output_pins[1] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_blue.output_pins[2] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_blue.output_pins[3] =	NRF_DRV_PWM_PIN_NOT_USED;
			pwm_config.pwm_config_blue.irq_priority	 	=	APP_IRQ_PRIORITY_LOWEST;
			pwm_config.pwm_config_blue.count_mode		 	=	NRF_PWM_MODE_UP;
			pwm_config.pwm_config_blue.load_mode			= NRF_PWM_LOAD_COMMON;
			pwm_config.pwm_config_blue.step_mode			= NRF_PWM_STEP_AUTO;
			pwm_config.pwm_config_blue.base_clock		 	= clk;
			pwm_config.pwm_config_blue.top_value		 	= cycle_top;
			
			// Setup the Sequence Structure
			pwm_seq.pwm_seq_blue.values.p_common			=	seq_data;
			pwm_seq.pwm_seq_blue.length								= seq_length;
			pwm_seq.pwm_seq_blue.repeats							=	0;
			pwm_seq.pwm_seq_blue.end_delay						=	0;
			
			return;
		}
	}
}

/** @Func Start the Sequence in One Channel */
void pwmRun(const char channel, const pwm_run_mode_t run_mode, const uint16_t loop_times)
{
	switch (channel){
		// Red LED Channel
		case 'r':
		case 'R':
		{
			// Initialize the PWM instance for red LED
			APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm_obj.pwm_inst_red, &pwm_config.pwm_config_red, NULL));
			
			if(run_mode == PWM_RUN_MODE_FINITE_LOOP){// Finite Loops
				nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_red, &pwm_seq.pwm_seq_red, loop_times, NRF_DRV_PWM_FLAG_STOP);
				pwm_flags.pwm_running_flag_red=true;
			}
			else if(run_mode == PWM_RUN_MODE_INFINITE_LOOP){// Infinite Loops
			  nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_red, &pwm_seq.pwm_seq_red, 1, NRF_DRV_PWM_FLAG_LOOP);
				pwm_flags.pwm_running_flag_red=true;
			}
			else{// By default, the sequence runs only once
				nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_red, &pwm_seq.pwm_seq_red, 1, NRF_DRV_PWM_FLAG_STOP);
				pwm_flags.pwm_running_flag_red=true;
			}
			return;
		}
		// Green LED Channel
		case 'g':
		case 'G':
		{
			// Initialize the PWM instance for green LED
			APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm_obj.pwm_inst_green, &pwm_config.pwm_config_green, NULL));
			
			if(run_mode == PWM_RUN_MODE_FINITE_LOOP){// Finite Loops
				nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_green, &pwm_seq.pwm_seq_green, loop_times, NRF_DRV_PWM_FLAG_STOP);
				pwm_flags.pwm_running_flag_green=true;
			}
			else if(run_mode == PWM_RUN_MODE_INFINITE_LOOP){// Infinite Loops
			  nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_green, &pwm_seq.pwm_seq_green, 1, NRF_DRV_PWM_FLAG_LOOP);
				pwm_flags.pwm_running_flag_green=true;
			}
			else{// By default, the sequence runs only once
				nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_green, &pwm_seq.pwm_seq_green, 1, NRF_DRV_PWM_FLAG_STOP);
				pwm_flags.pwm_running_flag_green=true;
			}
			return;
		}
		// Blue LED Channel
		case 'b':
		case 'B':
		{
			// Initialize the PWM instance for blue LED
			APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm_obj.pwm_inst_blue, &pwm_config.pwm_config_blue, NULL));
			
			if(run_mode == PWM_RUN_MODE_FINITE_LOOP){// Finite Loops
				nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_blue, &pwm_seq.pwm_seq_blue, loop_times, NRF_DRV_PWM_FLAG_STOP);
				pwm_flags.pwm_running_flag_blue=true;
			}
			else if(run_mode == PWM_RUN_MODE_INFINITE_LOOP){// Infinite Loops
				nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_blue, &pwm_seq.pwm_seq_blue, 1, NRF_DRV_PWM_FLAG_LOOP);
				pwm_flags.pwm_running_flag_blue=true;
			}
			else{// By default, the sequence runs only once
			  nrf_drv_pwm_simple_playback(&pwm_obj.pwm_inst_blue, &pwm_seq.pwm_seq_blue, 1, NRF_DRV_PWM_FLAG_STOP);
				pwm_flags.pwm_running_flag_blue=true;
			}
			return;
		}
	}
}

/** @Func Update the Data Sequence of One Channel */
void pwmUpdateSeq(const uint16_t * const seq_data, const uint16_t seq_length, const char channel)
{
	switch (channel){
		// Red LED Channel
		case 'r':
		case 'R':
		{
			// Setup the Sequence Structure
			pwm_seq.pwm_seq_red.values.p_common			 =	seq_data;
			pwm_seq.pwm_seq_red.length							 = 	seq_length;
			pwm_seq.pwm_seq_red.repeats							 =	0;
			pwm_seq.pwm_seq_red.end_delay						 =	0;
			return;
		}
		// Green LED Channel
		case 'g':
		case 'G':
		{
			// Setup the Sequence Structure
			pwm_seq.pwm_seq_green.values.p_common			 =	seq_data;
			pwm_seq.pwm_seq_green.length							 = 	seq_length;
			pwm_seq.pwm_seq_green.repeats							 =	0;
			pwm_seq.pwm_seq_green.end_delay						 =	0;
			return;
		}
		// Blue LED Channel
		case 'b':
		case 'B':
		{
			// Setup the Sequence Structure
			pwm_seq.pwm_seq_blue.values.p_common			=	seq_data;
			pwm_seq.pwm_seq_blue.length								= seq_length;
			pwm_seq.pwm_seq_blue.repeats							=	0;
			pwm_seq.pwm_seq_blue.end_delay						=	0;
			return;
		}
	}
}

/** @Func Stop the Sequence in One Channel */
void pwmStop(const char channel, const pwm_stop_mode_t mode, const bool led_active_state)
{
	// The PWM wave plays for once only, so here the function only restores the pin states before the PWM operation
	switch (channel){
		// Red LED Channel	
		case 'r':
		case 'R':
		{
			// Release the PWM Resources
			nrf_drv_pwm_uninit(&pwm_obj.pwm_inst_red);
			// Set the flag
			pwm_flags.pwm_running_flag_red=false;
			// Restore the value before PWM starts
			uint8_t pin	=	pwm_config.pwm_config_red.output_pins[0] & (~NRF_DRV_PWM_PIN_INVERTED); // Get the pin number
			nrf_gpio_cfg_output(pin);			// Configure the pin to output mode
			// Check the mode option
			switch (mode){
				case PWM_STOP_MODE_TURNOFF:
				{// Turn off the LED
					if(led_active_state){
						nrf_gpio_pin_clear(pin);
					}
					else{
						nrf_gpio_pin_set(pin);
					}
					return;
				}
				case PWM_STOP_MODE_RESTORE:
				default:
				{
					nrf_gpio_pin_write(pin,pwm_pin_values.pwm_pin_val_red);
					return;
				}
			}
		}
		// Green LED
		case 'g':
		case 'G':
		{
			// Release the PWM Resources
			nrf_drv_pwm_uninit(&pwm_obj.pwm_inst_green);
			// Set the flag
			pwm_flags.pwm_running_flag_green=false;
			// Restore the value before PWM starts
			uint8_t pin	=	pwm_config.pwm_config_green.output_pins[0] & (~NRF_DRV_PWM_PIN_INVERTED); // Get the pin number
			nrf_gpio_cfg_output(pin);			// Configure the pin to output mode
			// Check the mode option
			switch (mode){
				case PWM_STOP_MODE_TURNOFF:
				{// Turn off the LED
					if(led_active_state){
						nrf_gpio_pin_clear(pin);
					}
					else{
						nrf_gpio_pin_set(pin);
					}
					return;
				}
				case PWM_STOP_MODE_RESTORE:
				default:
				{
					nrf_gpio_pin_write(pin,pwm_pin_values.pwm_pin_val_green);
					return;
				}
			}
		}
		// Blue LED
		case 'b':
		case 'B':
		{
			// Release the PWM Resources
			nrf_drv_pwm_uninit(&pwm_obj.pwm_inst_blue);
			// Set the flag
			pwm_flags.pwm_running_flag_blue=false;
		  // Restore the value before PWM starts
			uint8_t pin	=	pwm_config.pwm_config_blue.output_pins[0] & (~NRF_DRV_PWM_PIN_INVERTED); // Get the pin number
			nrf_gpio_cfg_output(pin);			// Configure the pin to output mode
			// Check the mode option
			switch (mode){
				case PWM_STOP_MODE_TURNOFF:
				{// Turn off the LED
					if(led_active_state){
						nrf_gpio_pin_clear(pin);
					}
					else{
						nrf_gpio_pin_set(pin);
					}
					return;
				}
				case PWM_STOP_MODE_RESTORE:
				default:
				{
					nrf_gpio_pin_write(pin,pwm_pin_values.pwm_pin_val_blue);
					return;
				}
			}
		}
	}
}

/** @Func Get PWM Status */
bool pwmIsRunning(const char channel)
{
	switch (channel){
		case 'r':
		case 'R':
		{
			return pwm_flags.pwm_running_flag_red;
		}
		case 'g':
		case 'G':
		{
			return pwm_flags.pwm_running_flag_green;
		}
		case 'b':
		case 'B':
		{
			return pwm_flags.pwm_running_flag_blue;
		}
		default:
			return false;
	}
}
