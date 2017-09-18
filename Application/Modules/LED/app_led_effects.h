/** Library Name: "app_led_effects.h" 
 * @Brief 	This library declares functions for LED effects
 *
 * @Auther 	Feng Yuan
 * @Time 		25/08/2017
 * @Version	1.0
 *
 * @Req 		This library requires the following modules to function
 * @Req 		- PWM Driver Module 											(Configured in sdk_config.h)
 * @Req			- Board IO Interface Definition Module		(Defined in "board_select.h")
 *
 * @Type 		- led_effect_t											(LED Effect Data Type)
 *
 * @Func		- ledEffectFillSeq									(Fill in the PWM sequence of a pre-defined effect)
 *
 * @Func		- ledEffectRegister									(Register the PWM instance of one channel)
 * @Func		- ledEffectRun											(Run a pre-defined LED effect)
 * @Func		- ledEffectStop											(Stop a pre-defined effect and restore the pin to its initial state)
 * @Func		-	ledEffectClear										(Clear any currently running LED effect)
 * @Func		- ledEffectUnregister								(Release the allocated PWM instance)
 *
 * @Func		-	ledEffectIsRunning								(Check whether there is an effect currently running)		
 * @Func		-	ledEffectIsRegistered							(Check whether there is an effect currently registered)
 *
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __APP_LED_EFFECTS_H__
#define __APP_LED_EFFECTS_H__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Modules Used in This Library */

/* PWM Hardware Driver Module */
#include "drv_pwm.h"

/* Board IO Interface Definition Module */
#include "board_select.h"					//Select the correct board IO mappings

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
extern "C" {
#endif
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Internal Variable Type Declarations */
	
/** @Type Declare LED Effect Data Type */
	
typedef enum
{
	LED_EFFECT_NONE  = 0, 				// No LED Effect
	LED_EFFECT_RED_ON,						// Red LED is On
	LED_EFFECT_GREEN_ON,					// Green LED is On
	LED_EFFECT_BLUE_ON,						// Blue LED is On
	LED_EFFECT_RGB_ON,						// All RGB LEDs are On
	LED_EFFECT_RGB_OFF,						// All RGB LEDs are Off
	LED_EFFECT_FIRST,							// First LED Effect
	LED_EFFECT_SECOND							// Second LED Effect
}led_effect_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Declarations for LED Effect Operations */

/** @Func Fill in the PWM sequence of a pre-defined effect
	*	
	*	@Brief This function fills in the PWM sequence of the specified effect
	* @Brief The data arrays to store the sequences should be defined in "app_led_effects.c"
	*
	* @Para effect [led_effect_t]: The specified LED effect
	*
*/
void ledEffectFillSeq(led_effect_t effect);


/** @Func Register the PWM instance of one channel
  * 
	* @Brief This function registers the specified LED effect
	*	@Brief This function sets up the configuration structures of the three PWM instances
	* @Brief This function initializes the three PWM instances which can be controlled independently
	* @Brief For non-PWM LED effects, this function configures the relevant pins as outputs
	*
	*	@Para  effect [led_effect_t]: The specified LED effect
	*
*/
void ledEffectRegister(led_effect_t effect);


/** @Func Run a pre-defined LED effect
	*
	*	@Brief This function runs the registered LED effect
	* @Brief For PWM LED effects, this function runs the pwm sequence
	* @Brief The PWM sequence running mode should be designated in this function
	* @Brief For different LED effects, not only the PWM sequences filled are different,
	* @Brief but the running mode should also be specified in this function
	*	@Brief For non-PWM effects, this function turns on/off relevant LEDs
	*	
	* @Brief For all new LED effects, this function should be modified
	*
*/
void ledEffectRun(void);


/** @Func Stop a pre-defined effect and restore the pin to its initial state
	*
	* @Brief This function stops the registered LED effect
	* @Brief After calling this function, it does NOT clear all LEDs
	* @Brief Instead, it restore the LED pin states before running the LED effect
	*	@Brief For non-PWM LED effects, it toggles the pin states instead of clear them.
	*
*/
void ledEffectStop(void);


/** @Func Clear any currently running LED effect
	*
	* @Brief This function stops the registered LED effect and also clear all three RGB LEDs
	*
*/
void ledEffectClear(void);


/** @Func Release the allocated PWM instance
	*
	*	@Brief This function unregisters the current LED effect
	* @Brief For PWM LED effect, this function destroys the three PWM instances.
	* @Brief For non-PWM LED effect, this function does nothing.
	*
*/
void ledEffectUnregister(void);


/** @Func Check whether there is an effect currently running
	*
	* @Brief This function checks whether a registered LED effect is currently running or not
	* 
	* @Return False - The current effect is not running
	* @Return True	- The current effect is running
	*
*/
bool ledEffectIsRunning(void);


/** @Func Check whether there is an effect currently registered
	*
	* @Brief This function checks whether there is an LED effect currently registered
	*
	* @Return False - There is NO LED effect currently registered
	* @Return True	-	There is an LED effect currently registered
	*
*/
bool ledEffectIsRegistered(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
}
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DATA_LED_EFFECTS_H__
