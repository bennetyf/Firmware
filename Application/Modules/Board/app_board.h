/** Library Name: app_board.h
 * @Brief 	This library declares functions for board utilities
 * @Brief 	- Button Apps
 * @Brief 	- LED Effect Apps
 *
 * @Auther 	Feng Yuan
 * @Time 	25/08/2017
 * @Version	1.0
 *
 * @Req 		This library requires the following modules to function
 * @Req 		- GPIO Module 											(Configured in sdk_config.h)
 * @Req			- Clock Driver Module								(Configured in sdk_config.h)
 * @Req 		- Timer Module											(Configured in sdk_config.h)
 * @Req 		- Button Module 										(Configured in sdk_config.h)
 * @Req 		- Board IO Interface Definitions		(Defined in "data_board.h")
 * @Req 		- LED Effects Module								(Defined in "app_led_effects.h")
 *
 * @Macro 	This module defines the following macros that can be referenced from outside
 * @Macro 	-	BOARD_INIT_NONE										(Initialize Nothing on the Board)
 * @Macro 	- BOARD_INIT_LEDS										(Initialize All LEDS on the Board)
 * @Macro   - BOARD_INIT_BUTTONS								(Initialize All Buttons on the Board)
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
 * @Type		- board_button_action_t							(Button Action Type)
 * @Type		-	board_button_event_t							(Button Event Type)
 * @Type		-	board_event_callback_t						(Callback Function Type for Board Events)
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
 * @Func		- boardWakeupButtonEnable						(Enable the Wakeup Button)
 * @Func		- boardWakeupButtonDisable					(Disable the Wakeup Button)
 *
 * @Func 		- boardConfig												(Configure Board Resources)
 *
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __APP_BOARD_H__
#define __APP_BOARD_H__
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** System Modules */
#include <stdint.h>
#include <stdbool.h>

/** SDK Modules */
/* GPIO Modules */
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"				//Using the GPIO Module
/* Timer Modules */
#include "app_timer.h"						//Timer module
/* Button Modules */
#include "app_button.h"						//Button module

/** User-defined Modules */
#include "board_select.h"					//Select the correct board IO mappings

#include "app_led_effects.h"			//LED effects module

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Macro Definitions */

/** @Macro Define Options For Board Resource Selection */
#define BOARD_INIT_NONE    												0        	// This define specifies the type of initialization without support for LEDs and buttons (@ref boardInit).
#define BOARD_INIT_LEDS    												(1 << 0) 	// This bit enables LEDs during initialization (@ref boardInit).
#define BOARD_INIT_BUTTONS 												(1 << 1) 	// This bit enables buttons during initialization (@ref boardInit).

/** @Macro Define Button Push Timer Settings */
#define BOARD_BUTTON_LONG_PUSH_TIMEOUT_MS      		(2000)
#define BOARD_BUTTON_DETECTION_DELAY_MS						(50)

/** @Macro Change the Number of Ticks for an Original Time into the Number of Ticks for any Target Time */
#define BOARD_TICKS_CONVERSION(org_time_ms, org_time_ticks, obj_time_ms, obj_time_ticks) {obj_time_ticks = (float)obj_time_ms/org_time_ms * org_time_ticks;}

/** @Macro Define Sub-procedures for Converting Pin No into Index */
#define BOARD_PIN_TO_IDX(list,length,pin_no,index) {for(uint8_t i = 0; i < length; i++){ 	\
																											if (list[i] == pin_no){							\
																												index = i;												\
																												break;}}}

/** @Macro Define Sub-procedures for Converting Index into Pin No */
#define BOARD_IDX_TO_PIN(list,length,pin_no,index) {ASSERT(index<length); \
																										pin_no = list[index];}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** C++ Header */
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Declare Types Used in This Module */

/** @Type Declare General Board Event Types */
typedef enum
{
	BOARD_EVENT_NOTHING = 0,                  // Assign this event to an action to prevent the action from generating an event (disable the action).
  BOARD_EVENT_DEFAULT,                      // Assign this event to an action to assign the default event to the action.
	BOARD_EVENT_CLEAR_BONDING_DATA,
	BOARD_EVENT_ALERT,
	BOARD_EVENT_DISCONNECT,
	BOARD_EVENT_ADVERTISING_START,
	BOARD_EVENT_ADVERTISING_STOP,
	BOARD_EVENT_WHITELIST_OFF,
	BOARD_EVENT_BOND,
  BOARD_EVENT_RESET,                        // The device should reset.
  BOARD_EVENT_SLEEP,                        // The device should enter sleep mode.
  BOARD_EVENT_WAKEUP,                       // The device should wake up from sleep mode.
  BOARD_EVENT_SYSOFF,                       // The device should enter system off mode (without wakeup).
	BOARD_EVENT_DFU,                          // The device should enter DFU mode.
	BOARD_EVENT_PUSH_BUTTON_0,								// Button 0 push event.
	BOARD_EVENT_PUSH_BUTTON_1,								// Button 1 push event.
	BOARD_EVENT_PUSH_BUTTON_2,								// Button 2 push event.
	BOARD_EVENT_PUSH_BUTTON_3,								// Button 3 push event.
	BOARD_EVENT_PUSH_BUTTON_4,								// Button 4 push event.
	BOARD_EVENT_PUSH_BUTTON_5,								// Button 5 push event.
	BOARD_EVENT_PUSH_BUTTON_6,								// Button 6 push event.
	BOARD_EVENT_PUSH_BUTTON_7,								// Button 7 push event.
	BOARD_TEST_EVENT_0,												// Reserved event 0 for testing.
	BOARD_TEST_EVENT_1,												// Reserved event 1 for testing.
	BOARD_TEST_EVENT_2,												// Reserved event 2 for testing.
	BOARD_TEST_EVENT_3,												// Reserved event 3 for testing.
	BOARD_TEST_EVENT_4,												// Reserved event 4 for testing.
	BOARD_TEST_EVENT_5,												// Reserved event 5 for testing.
	BOARD_TEST_EVENT_6,												// Reserved event 6 for testing.
	BOARD_TEST_EVENT_7												// Reserved event 7 for testing.
}board_event_t;

/** @Type Declare General Button Action Type */
typedef enum
{
	BOARD_BUTTON_ACTION_RELEASE = 0,
	BOARD_BUTTON_ACTION_PUSH,
	BOARD_BUTTON_ACTION_LONG_PUSH,
	BOARD_BUTTON_ACTION_RELEASE_AFTER_LONG_PUSH
}board_button_action_t;

/** @Type Declare Button Action Event Types */
typedef struct
{
	board_event_t push_event;      					// The event to fire on regular button press.
	board_event_t long_push_event; 					// The event to fire on long button press.
	board_event_t release_event;						// The event to fire on button release after a short push.
	board_event_t	release_after_long_push;	// The event to fire on button releas after a long push.
}board_button_event_t;

/** @Type Declare Board Event Callback Function Type.
 *  Upon an event in the module, this callback function will be called to notify
 *  the application about the event.
 */
typedef void (* board_event_callback_t)(board_event_t);

/** @Type The different actions that can be performed on a button */
//typedef uint8_t board_button_action_t; 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Essential Utility Functions (External) */

/** @Func Read the Value of a Specified Pin
	*
	*  @Brief	This function reads in the state of a specified pin.
	*  				This function configures the pin to read-ready state and then performs gpio read operation.
	*
	*	@Para		pin_no[uint8_t]: the pin number of the specified pin.
	*
	*	@Return False -	The pin is in a low state.
	*	@Return Ture	- The pin is in a high state.
*/
bool boardReadPinState(uint8_t pin_no);

/** @Func Get the State of an LED
	*
  * @Brief 	This function returns the state of specified LED.
	*				 	If the LED is on, then it returns true. Otherwise, it returns false.
	*					The input pin number is checked against the pre-defined LED pin list.
	*			
	* @Para	 	led_pin_no [uint8_t]: The pin number of the LED to be read.
	*
	* @Return	False	- The LED is off.
	* @Return True	- The LED is on.
*/
bool boardLedIsOn(uint8_t led_pin_no);

/** @Func Get the State of a Button
	*
	* @Brief	This function tests whether the specified button is pushed or not.
	*					This function simply reads the state of a button.
	*					The input pin number is checked against the pre-defined button pin list.
	*
	*	@Para		button_pin_no [uint8_t]: The pin number of the button to be read.
	*
	* @Return	False - The button is NOT pressed.
	* @Return True	- The button is pressed.
*/
bool boardButtonIsPressed(uint8_t button_pin_no);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Functions for running the pre-defined LED effects (External). */

/** @Func Run a Registered LED Effect
	*
  * @Brief	This function runs a already registered LED effect.
	*					The LED effect is pre-defined in the "app_led_effects.h" file.
*/
void boardLedEffectRun(void);

/** @Func Stop the Registered LED Effect
	*	
	* @Brief	This function stops a currently running registered LED effect.
	*					The LED effect is pre-defined in the "app_led_effects.h" file.
*/
void boardLedEffectStop(void);

/** @Func Unregister the Current LED Effect
	*
  * @Brief This function clears a currently registered LED effect.
	*				 This function turns off all LEDs and then unregisters the LED effect.
*/
void boardLedEffectClear(void);

/** @Func Register and Run an LED Effect
	*
	*	@Brief	This function registers the specified LED effect and then runs it.
	*					This function is async so that it returns immediately after the running function is called.
	*
	* @Para	effect[led_effect_t] : The specified LED effect which is defined in the file "app_led_effects.h"
	*
*/
void boardLedEffect(led_effect_t effect);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Functions for Button Applications */

/** @Func Function for Assigning an Event to a Button.
	*
	*	@Brief This function allows redefinition of standard events assigned to buttons.
	*				 To unassign events, assign the event BOARD_EVENT_NOTHING.
	*
	* @Para	button [uint8_t]:								Button ID to be redefined.
	* @Para action [board_button_action_t]: Button action to assign event to.
	* @Para event	 [board_event_t]:         Event to be assigned to button.
	*
	* @Return	NRF_SUCCESS									-	If the event was successfully assigned to button.
	* @Return NRF_ERROR_INVALID_PARAM  		-	If the button ID or button action was invalid.
*/
uint8_t boardButtonEventAssign(uint8_t button_pin_no, board_button_action_t action, board_event_t event);

/** @Func Function for Enabling One Specified Button
	*
	* @Brief This function is used for enabling one specified button which will generate events when pressed, and
	*				 it will be able to wake the system up from sleep mode.
	*
	* @Para button_pin_no [uint8_t]:				The button pin number to be enabled.
	*
	* @Return	NRF_SUCCESS									- If the specified button was successfully enabled.	
	* @Return NRF_ERROR_NOT_SUPPORTED			- If the button pin number is not within the button list.
*/
uint8_t boardButtonEnable(uint8_t button_pin_no);

/** @Func Function for Disabling One Specified Button
	*
	* @Brief This function is used for disabling one specified button which will not generate events when pressed, and
	*				 it will not be able to wake the system up from sleep mode.
	*
	* @Para button_pin_no [uint8_t]: 				The button pin number to be enabled.
	*
	* @Return NRF_SUCCESS						 			-	If the specified button was successfully disabled.
	* @Return NRF_ERROR_NOT_SUPPORTED			-	If the button pin number is not within the button list.
*/
uint8_t boardButtonDisable(uint8_t button_pin_no);

/** @Func Function for Enabling All Buttons.
	*
	* @Brief After calling this function, all buttons will generate events when pressed, and
	* 			 all buttons will be able to wake the system up from sleep mode.
	*
	* @Return	NRF_SUCCESS              		-	If the buttons were successfully enabled.
	* @Return	NRF_ERROR_NOT_SUPPORTED  		-	If the board has no buttons.
	* @Return A propagated error.
*/
uint8_t boardButtonEnableAll(void);

/** @Func Function for disabling all buttons.
	*
	* @Brief After calling this function, no buttons will generate events when pressed, and
	* 			 no buttons will be able to wake the system up from sleep mode.
	*
	* @Return	NRF_SUCCESS              - If the buttons were successfully disabled.
	* @Return NRF_ERROR_NOT_SUPPORTED  -	If the board has no buttons.
	* @Return A propagated error.
*/
uint8_t boardButtonDisableAll(void);

/** @Func		Function for Enabling the Wakeup Button
	*
	* @Brief 	This function enables the wakeup button.
	*	@Brief	The difference between this function and the normal button enabling function is that in this function, the button is set to be more sensitive
	*
	* @Para button_pin_no [uint8_t]: 				The button pin number to be enabled
	*
	* @Return NRF_SUCCESS						 			-	If the specified button was successfully enabled
	* @Return NRF_ERROR_NOT_SUPPORTED			-	If the button pin number is not within the button list
	*
*/
uint8_t boardWakeupButtonEnable(uint8_t button_pin_no);

/** @Func		Function for Disabling the Wakeup Button
	*
	* @Brief 	This function disables the wakeup button.
	*	@Brief	The difference between this function and the normal button disabling function is that in this function, the button is set to be more sensitive
	*
	* @Para button_pin_no [uint8_t]: 				The button pin number to be disabled
	*
	* @Return NRF_SUCCESS						 			-	If the specified button was successfully disabled
	* @Return NRF_ERROR_NOT_SUPPORTED			-	If the button pin number is not within the button list
	*
*/
uint8_t boardWakeupButtonDisable(uint8_t button_pin_no);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Function for configuring on-board resources.
	*
	* @Brief The function initializes the board driver to allow LED effects, button reaction and power management.
	* 			 Default events are assigned to buttons.
	*
	* @Brief Before calling this function, you must initiate the following required modules:
	* 			 - nrf_drv_pwm 					for LED effect support
	*				 - nrf_drv_clock				for timer clock support
	*				 - app_timer						for timer support (button used timers to distinguish long push)
	*  			 - app_gpiote 					for button support
	*  			 - app_button 					for button support
	*
	* @Para		init_type [uint32_t]: 								Peripherals used.
	*	@Para		ticks_per_ms [uint32_t]:							Ticks for 1 miliseconds.(Used as the basic number of ticks for timers inside this module)
	*	@Para 	callback 	[board_event_callback_t]:		The callback function for events.
	*																								It is defined externally to determine what function is called when a board event happens.
	*
	* @Return	NRF_SUCCESS               	-	If the module was successfully initialized.
	* @Return NRF_ERROR_INVALID_STATE   	-	If the application timer module has not been initialized.
	* @Return NRF_ERROR_NO_MEM          	-	If the maximum number of timers has already been reached.
	* @Return NRF_ERROR_INVALID_PARAM   	-	If GPIOTE has too many users.
	* @Return NRF_ERROR_INVALID_STATE   	-	If button or GPIOTE has not been initialized.
*/
uint8_t boardConfig(uint32_t init_type, uint32_t ticks_per_ms, board_event_callback_t callback);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* C++ Library Header */
#ifdef __cplusplus
} 
#endif //__cplusplus
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DRV_BOARD_H__
