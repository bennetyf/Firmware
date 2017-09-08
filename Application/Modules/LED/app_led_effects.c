/** Library Name: "app_led_effects.c" 
	* @Brief 	This library implements LED effect functions declared in "app_led_effects.h"
	*
	* @Auther 	Feng Yuan
	* @Time 		25/08/2017
	* @Version	1.0
	*
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Modules Included */
#include "app_led_effects.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Internal Variable Definitions */

/** @Variable Store the current LED effect */
static led_effect_t current_led_effect 	= LED_EFFECT_NONE;

/** @Variable Flag for noting whether the effect is a PWM effect */
static bool is_pwm_effect								= false;

/** @Variable Flag for noting whether the effect is a simple GPIO effect */
static bool is_gpio_effect							= false;

/** @Variable Flag for noting whether there is an effect currently running */
static bool is_led_effect_running				= false;

/** @Variable Store the pin numbers of the three RGB LEDs */
#if LEDS_NUMBER > 2
static const uint8_t leds_rgb_list[LEDS_NUMBER] 	= LEDS_LIST;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* PWM Wave Sequence Data Definitions */

/** @Variable Define variables for the first LED effect */
static const nrf_pwm_clk_t 	clk_led_eff_1_r 			= NRF_PWM_CLK_1MHz;		//PWM basic clock
static const uint16_t 			top_val_led_eff_1_r 	= 10000;							//Top value for each time-out cycle(this defines the period of the PWM wave)
static const uint16_t			 	seq_len_led_eff_1_r 	= 300;								//The length of one sequence data(this defines the total time of sequence execution)
static uint16_t 						seq_data_led_eff_1_r[seq_len_led_eff_1_r];  //The data array to store the sequence data

static const nrf_pwm_clk_t 	clk_led_eff_1_g 			= NRF_PWM_CLK_1MHz;
static const uint16_t 			top_val_led_eff_1_g 	= 10000;
static const uint16_t			 	seq_len_led_eff_1_g 	= 300;
static uint16_t 						seq_data_led_eff_1_g[seq_len_led_eff_1_g];

static const nrf_pwm_clk_t 	clk_led_eff_1_b 			= NRF_PWM_CLK_1MHz;
static const uint16_t 			top_val_led_eff_1_b 	= 10000;
static const uint16_t			 	seq_len_led_eff_1_b 	= 300;
static uint16_t 						seq_data_led_eff_1_b[seq_len_led_eff_1_b];

/** @Variable Define variables for the second LED effect */
static const nrf_pwm_clk_t 	clk_led_eff_2_r 			= NRF_PWM_CLK_1MHz;		//PWM basic clock
static const uint16_t 			top_val_led_eff_2_r 	= 10000;							//Top value for each time-out cycle(this defines the period of the PWM wave)
static const uint16_t			 	seq_len_led_eff_2_r 	= 200;								//The length of one sequence data(this defines the total time of sequence execution)
static uint16_t 						seq_data_led_eff_2_r[seq_len_led_eff_2_r];  //The data array to store the sequence data

static const nrf_pwm_clk_t 	clk_led_eff_2_g 			= NRF_PWM_CLK_1MHz;
static const uint16_t 			top_val_led_eff_2_g 	= 10000;
static const uint16_t			 	seq_len_led_eff_2_g 	= 200;
static uint16_t 						seq_data_led_eff_2_g[seq_len_led_eff_2_g];

static const nrf_pwm_clk_t 	clk_led_eff_2_b 			= NRF_PWM_CLK_1MHz;
static const uint16_t 			top_val_led_eff_2_b 	= 10000;
static const uint16_t			 	seq_len_led_eff_2_b 	= 200;
static uint16_t 						seq_data_led_eff_2_b[seq_len_led_eff_2_b];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Function Implementations for LED Effect Operations */

/** @Func Initialize a pre-defined effect */
void ledEffectFillSeq(led_effect_t effect)
{
	switch (effect){
		/** @Effect First LED effect sequence filling */
		case LED_EFFECT_FIRST:
		{
			uint16_t i,step;
			uint16_t inc_start,inc_end,dec_start,dec_end;
			
			// Channel One
			inc_start	=	0;
			inc_end		=	seq_len_led_eff_1_r/6-1;
			dec_start	=	seq_len_led_eff_1_r/6;
			dec_end		= seq_len_led_eff_1_r/3-1;
			for(i=inc_start,step=200;i<=inc_end;i++){
				seq_data_led_eff_1_r[i] = step*i;
			}
			for(i=dec_start,step=200;i<=dec_end;i++){
				seq_data_led_eff_1_r[i] = seq_data_led_eff_1_r[inc_end]-step*(i-dec_start);
			}
			for(i=dec_end+1;i<seq_len_led_eff_1_r;i++){
				seq_data_led_eff_1_r[i]	= 0;
			}
			
			// Channel Two
			inc_start	=	seq_len_led_eff_1_g/3;
			inc_end		= seq_len_led_eff_1_g/2-1;
			dec_start	=	seq_len_led_eff_1_g/2;
			dec_end		= seq_len_led_eff_1_g/3*2-1;
			for(i=0;i<inc_start;i++){
				seq_data_led_eff_1_g[i]=0;
			}
			for(i=inc_start,step=200;i<=inc_end;i++){
				seq_data_led_eff_1_g[i] = step*(i-inc_start);
			}
			for(i=dec_start,step=200;i<=dec_end;i++){
				seq_data_led_eff_1_g[i] = seq_data_led_eff_1_g[inc_end]-step*(i-dec_start);
			}
			for(i=dec_end+1;i<seq_len_led_eff_1_g;i++){
				seq_data_led_eff_1_g[i]	= 0;
			}
			
			// Channel Three
			inc_start	=	seq_len_led_eff_1_b/3*2;
			inc_end		=	seq_len_led_eff_1_b/6*5-1;
			dec_start	= seq_len_led_eff_1_b/6*5;
			dec_end		= seq_len_led_eff_1_b-1;
			for(i=0;i<=inc_start;i++){
				seq_data_led_eff_1_b[i] = 0;
			}
			for(i=inc_start,step=200;i<=inc_end;i++){
				seq_data_led_eff_1_b[i]	=	step*(i-inc_start);
			}
			for(i=dec_start;i<=dec_end;i++){
				seq_data_led_eff_1_b[i]	= seq_data_led_eff_1_b[inc_end]-step*(i-dec_start);
			}
			return;
		}
		
		/** @Effect Second LED effect sequence filling */
		case LED_EFFECT_SECOND:
		{
			uint16_t i,step;
			
			// Channel One
			for(i=0,step=100;i<seq_len_led_eff_2_r/2;i++){
				seq_data_led_eff_2_r[i] = step * i;
			}
			for(i=seq_len_led_eff_2_r/2,step=100;i<seq_len_led_eff_2_r;i++){
				seq_data_led_eff_2_r[i]	=	seq_data_led_eff_2_r[seq_len_led_eff_2_r/2-1]-step*(i-seq_len_led_eff_2_r/2);
			}
			
			// Channel Two
			for(i=0,step=100;i<seq_len_led_eff_2_g/2;i++){
				seq_data_led_eff_2_g[i] = step * i;
			}
			for(i=seq_len_led_eff_2_g/2,step=100;i<seq_len_led_eff_2_g;i++){
				seq_data_led_eff_2_g[i]	=	seq_data_led_eff_2_g[seq_len_led_eff_2_g/2-1]-step*(i-seq_len_led_eff_2_g/2);
			}
			
			// Channel Three
			for(i=0,step=100;i<seq_len_led_eff_2_b/2;i++){
				seq_data_led_eff_2_b[i] = step * i;
			}
			for(i=seq_len_led_eff_2_b/2,step=100;i<seq_len_led_eff_2_b;i++){
				seq_data_led_eff_2_b[i]	=	seq_data_led_eff_2_b[seq_len_led_eff_2_b/2-1]-step*(i-seq_len_led_eff_2_b/2);
			}
			return;
		}
	}
}

/** @Func Register one specified LED effect */
void ledEffectRegister(led_effect_t effect)
{
	// Check input parameter and avoid re-register for the same effect
	if(effect == current_led_effect){
		return;
	}
	
	// New LED effect to be registered
	switch (effect){
		
		// None PWM effects
		case LED_EFFECT_RED_ON:
		case LED_EFFECT_GREEN_ON:
		case LED_EFFECT_BLUE_ON:
		case LED_EFFECT_RGB_ON:
		case LED_EFFECT_RGB_OFF:
		{
			// Configure the output pins
			nrf_gpio_cfg_output(leds_rgb_list[0]);
			nrf_gpio_cfg_output(leds_rgb_list[1]);
			nrf_gpio_cfg_output(leds_rgb_list[2]);
			
			// Set the flag to notify that this is a non-PWM effect
			is_gpio_effect		 = true;
			
			// Record the current registered LED effect
			current_led_effect = effect;
			
			return;
		}
		
		// First PWM effect
		case LED_EFFECT_FIRST:
		{
			// Set up the PWM configuration structures
			pwmSetup(clk_led_eff_1_r,top_val_led_eff_1_r,seq_data_led_eff_1_r,seq_len_led_eff_1_r,leds_rgb_list[0],'r');
			pwmSetup(clk_led_eff_1_g,top_val_led_eff_1_g,seq_data_led_eff_1_g,seq_len_led_eff_1_g,leds_rgb_list[1],'g');
			pwmSetup(clk_led_eff_1_b,top_val_led_eff_1_b,seq_data_led_eff_1_b,seq_len_led_eff_1_b,leds_rgb_list[2],'b');	
			
			// Set the flag to notify that this is a PWM effect
			is_pwm_effect			 = true;
			
			// Store Current LED Effect
			current_led_effect = effect;
			
			return;
		}
		
		// Second PWM effect
		case LED_EFFECT_SECOND:
		{
			// Set up the PWM configuration structures
			pwmSetup(clk_led_eff_2_r,top_val_led_eff_2_r,seq_data_led_eff_2_r,seq_len_led_eff_2_r,leds_rgb_list[0],'r');
			pwmSetup(clk_led_eff_2_g,top_val_led_eff_2_g,seq_data_led_eff_2_g,seq_len_led_eff_2_g,leds_rgb_list[1],'g');
			pwmSetup(clk_led_eff_2_b,top_val_led_eff_2_b,seq_data_led_eff_2_b,seq_len_led_eff_2_b,leds_rgb_list[2],'b');
			
			// Set the flag to notify that this is a PWM effect
			is_pwm_effect			 = true;
			
			// Store Current LED Effect
			current_led_effect = effect;
			
			return;
		}
		
		default:
		{
			// Set the flag to notify that this is not an LED effect
			is_pwm_effect			 = false;
			is_gpio_effect		 = false;
			
			// Store the current LED effect
			current_led_effect = LED_EFFECT_NONE;
			
			return;
		}
	}
}

/** @Func Run a pre-defined effect */
void ledEffectRun(void)
{
	if(is_led_effect_running){//If there is an effect currently running,then return.
		return;
	}
	
	switch (current_led_effect){
		
		// Turn on the red LED only
		case LED_EFFECT_RED_ON:
		{
			nrf_gpio_pin_write(leds_rgb_list[0], LEDS_ACTIVE_STATE ? 1 : 0);
			nrf_gpio_pin_write(leds_rgb_list[1], LEDS_ACTIVE_STATE ? 0 : 1);
			nrf_gpio_pin_write(leds_rgb_list[2], LEDS_ACTIVE_STATE ? 0 : 1);
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		// Turn on the green LED only
		case LED_EFFECT_GREEN_ON:
		{
			nrf_gpio_pin_write(leds_rgb_list[0], LEDS_ACTIVE_STATE ? 0 : 1);
			nrf_gpio_pin_write(leds_rgb_list[1], LEDS_ACTIVE_STATE ? 1 : 0);
			nrf_gpio_pin_write(leds_rgb_list[2], LEDS_ACTIVE_STATE ? 0 : 1);
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		// Turn on the blue LED only
		case LED_EFFECT_BLUE_ON:
		{
			nrf_gpio_pin_write(leds_rgb_list[0], LEDS_ACTIVE_STATE ? 0 : 1);
			nrf_gpio_pin_write(leds_rgb_list[1], LEDS_ACTIVE_STATE ? 0 : 1);
			nrf_gpio_pin_write(leds_rgb_list[2], LEDS_ACTIVE_STATE ? 1 : 0);
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		// Turn on all three RGB LEDs
		case LED_EFFECT_RGB_ON:
		{
			nrf_gpio_pin_write(leds_rgb_list[0], LEDS_ACTIVE_STATE ? 1 : 0);
			nrf_gpio_pin_write(leds_rgb_list[1], LEDS_ACTIVE_STATE ? 1 : 0);
			nrf_gpio_pin_write(leds_rgb_list[2], LEDS_ACTIVE_STATE ? 1 : 0);
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		// Turn off all three RGB LEDs
		case LED_EFFECT_RGB_OFF:
		{
			nrf_gpio_pin_write(leds_rgb_list[0], LEDS_ACTIVE_STATE ? 0 : 1);
			nrf_gpio_pin_write(leds_rgb_list[1], LEDS_ACTIVE_STATE ? 0 : 1);
			nrf_gpio_pin_write(leds_rgb_list[2], LEDS_ACTIVE_STATE ? 0 : 1);
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		// Run the first LED effect
		case LED_EFFECT_FIRST:
		{
			// Run the PWM effect
			pwmRun('r',PWM_RUN_MODE_FINITE_LOOP,2);
			pwmRun('g',PWM_RUN_MODE_FINITE_LOOP,2);
			pwmRun('b',PWM_RUN_MODE_FINITE_LOOP,2);
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		// Run the second LED effect
		case LED_EFFECT_SECOND:
		{
			// Run the PWM effect				
			pwmRun('r',PWM_RUN_MODE_FINITE_LOOP,3);
			pwmRun('g',PWM_RUN_MODE_FINITE_LOOP,3);
			pwmRun('b',PWM_RUN_MODE_FINITE_LOOP,3);		
			// Set the LED effect running flag
			is_led_effect_running	= true;
			return;
		}
		
		default:
			return;
	}
}

/** @Func Stop a pre-defined effect */
void ledEffectStop(void)
{
	if(!is_led_effect_running){//Current LED effect has already been stopped
		return;
	}
	
	if(is_pwm_effect){//PWM LED effect
		pwmStop('r',PWM_STOP_MODE_RESTORE,LEDS_ACTIVE_STATE);
		pwmStop('g',PWM_STOP_MODE_RESTORE,LEDS_ACTIVE_STATE);
		pwmStop('b',PWM_STOP_MODE_RESTORE,LEDS_ACTIVE_STATE);
		// Clear the LED effect running flag
		is_led_effect_running = false;
	}
	
	if(is_gpio_effect){//Non-PWM LED effect
		nrf_gpio_pin_toggle(leds_rgb_list[0]);
		nrf_gpio_pin_toggle(leds_rgb_list[1]);
		nrf_gpio_pin_toggle(leds_rgb_list[2]);
		// Clear the LED effect running flag
		is_led_effect_running = false;
	}
	return;
}

/** @Func Clear any currently running LED effect */
void ledEffectClear(void)
{
	if(!is_led_effect_running){//Current LED effect has already been stopped
		return;
	}
		
	if(is_pwm_effect){//PWM LED effect
		pwmStop('r',PWM_STOP_MODE_TURNOFF,LEDS_ACTIVE_STATE);
		pwmStop('g',PWM_STOP_MODE_TURNOFF,LEDS_ACTIVE_STATE);
		pwmStop('b',PWM_STOP_MODE_TURNOFF,LEDS_ACTIVE_STATE);
		// Clear the LED effect running flag
		is_led_effect_running = false;
	}
	
	if(is_gpio_effect){//Non-PWM LED effect
		nrf_gpio_pin_write(leds_rgb_list[0], LEDS_ACTIVE_STATE ? 0 : 1);
		nrf_gpio_pin_write(leds_rgb_list[1], LEDS_ACTIVE_STATE ? 0 : 1);
		nrf_gpio_pin_write(leds_rgb_list[2], LEDS_ACTIVE_STATE ? 0 : 1);
		// Clear the LED effect running flag
		is_led_effect_running = false;		
	}
	return;
}

/** @Func Release the allocated PWM instance */
void ledEffectUnregister(void)
{
	if(is_gpio_effect || is_pwm_effect){
		is_gpio_effect				=	false;
		is_pwm_effect					= false;
		is_led_effect_running	= false;
		// Set the currently registered LED effect to NONE
		current_led_effect = LED_EFFECT_NONE;
	}
	return;
}

/** @Func Check whether there is an effect currently running */
bool ledEffectIsRunning(void)
{
	if(is_gpio_effect || is_pwm_effect){//There is a current effect registered
		return is_led_effect_running;
	}
	else{// No LED effect currently registered
		return false;
	}
}

/** @Func Check whether there is an effect currently registered */
bool ledEffectIsRegistered(void)
{
	return is_gpio_effect || is_pwm_effect;
}
