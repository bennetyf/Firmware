/** Library Name: "app_led_effects.h" 
 * @Brief 	This library declares functions for LED effects
 *
 * @Auther 	Feng Yuan
 * @Time 		25/08/2017
 * @Version	1.0
 *
 * @Req 		This library requires the following modules to function
 * @Req 		- PWM Driver Module 											(Configured in sdk_config.h)
 * @Req			- Board IO Interface Definition Module		(Defined in "data_board.h")
 *
 * @Macro 	This module defines the following macros that can be referenced from outside
 * @Macro 	-	BOARD_INIT_NONE										(Initialize Nothing on the Board)
 * @Macro 	- BOARD_INIT_LEDS										(Initialize All LEDS on the Board)
 * @Macro   - BOARD_INIT_BUTTONS								(Initialize All Buttons on the Board)
 * @Macro   - BOARD_BUTTON_ACTION_PUSH  				(Define Button Short Push Action)
 * @Macro 	- BOARD_BUTTON_ACTION_RELEASE 			(Define Button Release Action)
 * @Macro 	- BOARD_BUTTON_ACTION_LONG_PUSH			(Define Button Long Push Action)
 * @Macro 	- BOARD_BUTTON_TIMER_PRESCALER			(Define Button Timer Prescaler)
 * @Macro		- BOARD_BUTTON_TIMER_OP_QUEUE_SIZE	(Define Button Timer Queue Size)
 * @Macro 	- BOARD_BUTTON_LONG_PUSH_TIMEOUT_MS (Define Button Long Push Time in Milliseconds)
 * @Macro		- BOARD_BUTTON_DETECTION_DELAY_MS		(Define Button Push Action Detection Delay Time in Milliseconds)
 * @Macro 	- BOARD_PIN_TO_IDX									(Define Conversion from the Pin Number on Board to List Index)
 *						(list,length,pin_no,index) 				(The List is Internally Defined in "data_board.h" File)
 * @Macro 	- BOARD_IDX_TO_PIN									(Define Conversion from the List Index to the Pin Number on Board)
 *						(list,length,pin_no,index) 				(The List is Internally Defined in "data_board.h" File)
 *
 * @Type 		- board_event_t											(Board Event Type Including All Kinds of Board Events)
 * @Type		-	board_button_event_t							(Button Event Type)
 * @Type		-	board_event_callback_t						(Callback Function Type for Board Events)
 * @Type		- board_button_action_t							(Button Action Type)
 *
 * @Func		- boardReadPinState									(Read the State of a Specified Pin)
 * @Func		- boardLedIsOn											(Check Whether the Specified LED is On)
 * @Func		- boardButtonIsPressed							(Check Whether the Specified Button is Pressed)
 *
 * @Func		- boardLedEffectRun									(Run the Currently Registered LED Effect)
 * @Func		-	boardLedEffectStop								(Stop the Currently Registered LED Effect)
 * @Func		- boardLedEffectClear								(Turn off All LEDs and Unregister the Current LED Effect)
 * @Func		-	boardLedEffect										(Register and Run an LED Effect)		
 *
 * @Func		-	boardButtonEventAssign						(Assign an Event to a Button Action)
 * @Func		- boardButtonEnable									(Enable a Button)
 * @Func		- boardButtonDisable								(Disable a Button)
 * @Func		- boardButtonEnableAll							(Enable All Buttons)
 * @Func		- boardButtonDisableAll							(Disable All Buttons)
 *
 * @Func 		- boardInit													(Initialize Board Resources)
 *
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifndef __APP_LED_EFFECTS_H__
#define __APP_LED_EFFECTS_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Modules Used in This Library */

/* PWM Hardware Driver Module */
#include "drv_pwm.h"

/* Board IO Interface Definition Module */
#ifndef	 USER_BOARD
#include "data_board.h"
#endif

#ifdef  USER_BOARD
#include "io_board.h"
#endif

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
