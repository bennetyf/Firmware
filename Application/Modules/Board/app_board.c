/** Library Name: app_board.c
	*
	* @Brief This File Implements the Functions Declared in "app_board.h"
	*
	* @Auther 	Feng Yuan
	* @Time 		25/08/2017
	* @Version	1.0
	*
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "app_board.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Definitions of Internal Variables */

/* Variables Used in Button Applications */
#if BUTTONS_NUMBER > 0

/** @Variable The Callback Function Pointer for All Board Events */
static board_event_callback_t  		board_callback         									 = NULL;

/** @Variable The Ticks for Long Push Time Out */
static uint32_t										long_push_time_out_ticks								 = 0;

/** @Variable The Ticks for Button Detection Time Out */
static uint32_t										button_detection_time_out_ticks					 = 0;

/** @Variable Button Event List (Store the event for each button) */
static board_button_event_t   		board_button_events_list[BUTTONS_NUMBER] =
{//By default, for each button action (short push, long push and release), the default event is nothing.
	{BOARD_EVENT_NOTHING, BOARD_EVENT_NOTHING, BOARD_EVENT_NOTHING}
};

/** @Func Declare the Button Event Handler */
static void board_button_event_handler(uint8_t pin_no, uint8_t button_action);

/** @Variable Button List */
static const uint8_t board_button_list[BUTTONS_NUMBER] 					= BUTTONS_LIST; //The button list to store all the button pin numbers.

/** @Variable Button Configurations (Configure the GPIO settings for each button. @ref boardInit) */
static const app_button_cfg_t board_buttons_cfg[BUTTONS_NUMBER] =
{
	// Note that these button definitions should be defined in the "data_board.h" file.
	/** Members of the configuration structure:
		*	@member Pin to be used as a button
		* @member Active level of the button: APP_BUTTON_ACTIVE_HIGH or APP_BUTTON_ACTIVE_LOW
		* @member Pull-up or -down configuration
		* @member Handler to be called when button is pushed
	*/
	
	#ifdef BOARD_BUTTON_0
  {BOARD_BUTTON_0, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_0

  #ifdef BOARD_BUTTON_1
  {BOARD_BUTTON_1, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_1

  #ifdef BSP_BUTTON_2
  {BOARD_BUTTON_2, false, BUTTON_PULL, board_button_event_handler},
  #endif //	BOARD_BUTTON_2

  #ifdef BOARD_BUTTON_3
  {BOARD_BUTTON_3, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_3
	
	#ifdef BOARD_BUTTON_4
  {BOARD_BUTTON_4, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_4
	
	#ifdef BOARD_BUTTON_5
  {BOARD_BUTTON_5, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_5
	
	#ifdef BOARD_BUTTON_6
  {BOARD_BUTTON_6, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_6
	
	#ifdef BOARD_BUTTON_7
  {BOARD_BUTTON_7, false, BUTTON_PULL, board_button_event_handler},
  #endif // BOARD_BUTTON_7
};

/** @Variable Button Initialization Flag */
static bool board_is_button_init = false;	// Flag to note whether on-board buttons are chosen to be initialized

/** @Variable The Button Timer */
APP_TIMER_DEF(button_timer_id);

#endif

/* Variables Used in LED Applications */
#if LEDS_NUMBER > 0

/** @Variable LED List */
static const uint8_t board_led_list[LEDS_NUMBER] = LEDS_LIST; //The LED list to store all the LED pin numbers.

/** @Varialbe LED Initialization Flag */
static bool board_is_led_init = false; // Flag to note whether on-board LEDs are chose to be initialized

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* General Utility Functions */

/** @Func Read the Value of a Specified Pin */
bool boardReadPinState(uint8_t pin_no)
{
	nrf_gpio_cfg_input(pin_no, NRF_GPIO_PIN_NOPULL);
	return nrf_gpio_pin_out_read(pin_no);
}

#if LEDS_NUMBER > 0
/* Utility Functions for LEDs */

/** @Func Get the State of a Specified LED */
bool boardLedIsOn(uint8_t led_pin_no)
{
	// Check the Input Parameter
	uint8_t led_idx = 0xFF;
	BOARD_PIN_TO_IDX(board_led_list,LEDS_NUMBER,led_pin_no,led_idx);
	ASSERT(led_idx < LEDS_NUMBER);
	
	return (boardReadPinState(led_pin_no) == (LEDS_ACTIVE_STATE ? true : false));
}
#endif // LEDS_NUMBER > 0

#if BUTTONS_NUMBER > 0
/* Utility Functions for Buttons*/

/** @Func Get the State of a Specified Button */
bool boardButtonIsPressed(uint8_t button_pin_no)
{
	// Check the Input Parameter
	uint8_t button_idx = 0xFF;
	BOARD_PIN_TO_IDX(board_button_list,BUTTONS_NUMBER,button_pin_no,button_idx);
	ASSERT(button_idx < BUTTONS_NUMBER);
	
  return (boardReadPinState(button_pin_no) == (BUTTONS_ACTIVE_STATE ? true : false));
}
#endif //BUTTONS_NUMBER > 0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Internal Event Handler Functions */

#if BUTTONS_NUMBER > 0

/** @Func Define Button Event Handler */
static void board_button_event_handler(uint8_t button_pin_no, board_button_action_t button_action)
{
	board_event_t      		event  									= BOARD_EVENT_NOTHING;
  uint8_t           		button_idx 							= 0xFF;
  uint8_t           		err_code;
  static uint8_t     		current_long_push_pin_no;              // Pin number of a currently pushed button, that could become a long push if held long enough.
  static board_event_t 	release_event_at_push[BUTTONS_NUMBER]; // Array of what the release event of each button was last time it was pushed, so that no release event is sent if the event was bound after the push of the button.
	static bool						release_after_long_push	=	false;			 // The flag to notify whether the release action is after a long push or not.
	
	// Convert the pin number into button list index.
	BOARD_PIN_TO_IDX(board_button_list, BUTTONS_NUMBER, button_pin_no, button_idx);
	
  if (button_idx < BUTTONS_NUMBER){ // Check whether the button pin number is within the button list
		switch (button_action){
			
			// Short Push Action
			case BOARD_BUTTON_ACTION_PUSH:
			{
				// Read the name of the board event for short button push. (This should be already assigned to the button)
				event = board_button_events_list[button_idx].push_event;
				
				// Check whether long button push action is assigned to a board event.
				if (board_button_events_list[button_idx].long_push_event != BOARD_EVENT_NOTHING){
					// If long button push action is assigned to a board event, start the timer for long button push.
					err_code = app_timer_start(button_timer_id,
																		 long_push_time_out_ticks,
																		 (void*)&current_long_push_pin_no);
					if (err_code == NRF_SUCCESS){
						current_long_push_pin_no = button_pin_no;// Memorize the long-pushed button pin number
          }
        }
				
				// Memorize the release event corresponding to the pushed button.
				release_event_at_push[button_idx] = board_button_events_list[button_idx].release_event;
				break;
		 }
		 
		 // Long Push Action
		 case BOARD_BUTTON_ACTION_LONG_PUSH:
		 {
			 // Read the name of the board event for long push action.(This should be already assigned to the button)
			 event = board_button_events_list[button_idx].long_push_event;
			 release_after_long_push	= true;
			 break;
		 }
		 
		 // Release Action After Short Push
		 case BOARD_BUTTON_ACTION_RELEASE:
		 {
			 // Stop the timer on short release action
			 app_timer_stop(button_timer_id);
			 
			 // Response only to release actions that correspond to the same buttons just pushed.
			 if(release_event_at_push[button_idx] == board_button_events_list[button_idx].release_event){
				 if(release_after_long_push){
					 event = board_button_events_list[button_idx].release_after_long_push;
					 release_after_long_push	=	false;
				 }
				 else{
					// Read the name of the board event for release button action.(This should be already assigned to the button)
					event = board_button_events_list[button_idx].release_event;
				 }
       }
       break;
		 }
		 
		 // Default Button Action
		 default:
			 break;
   }
  }

	// Execute the board event callback function for the button action (short push, release or long push).
  if((event != BOARD_EVENT_NOTHING) && (board_callback != NULL)){
		board_callback(event);
	}
}

/** @Func Handle the time-out event from button timer(to determine the long push action) */
static void board_button_timer_handler(void * p_context)
{
	// Button timer time-out, call long push event action.
	board_button_event_handler(*(uint8_t *)p_context, BOARD_BUTTON_ACTION_LONG_PUSH);
}

#endif //BUTTONS_NUMBER > 0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* External Led Effects Functions */

#if LEDS_NUMBER > 2 // Must have at least three RGB LEDs
/* Function for running the pre-defined LED effects */

/** @Func Run a Registered LED Effect */
void boardLedEffectRun(void)
{
	// Check whether an LED effect is currently registered
	if(board_is_led_init && ledEffectIsRegistered()){
		ledEffectRun();
	}
}

/** @Func Stop Running the Registered LED Effect */
void boardLedEffectStop(void)
{
	// Check whether an LED effect is currently registered
	if(board_is_led_init && ledEffectIsRegistered()){
		ledEffectStop();
	}
}

/** @Func Unregister the Current LED Effect */
void boardLedEffectClear(void)
{
	// Check whether an LED effect is currently registered
	if(board_is_led_init && ledEffectIsRegistered()){
		ledEffectClear();  					// Turn off all the RGB LEDs
		ledEffectUnregister();			// Unregister the current LED effect
	}
}

/** @Func Register and Run an LED Effect */
void boardLedEffect(led_effect_t led_effect)
{
	if(board_is_led_init){
		// Turn off all the RGB LEDs if some effect is currently running
		ledEffectClear();
		// Unregister the current LED effect
		ledEffectUnregister();
		// Register the specified LED effect
		ledEffectRegister(led_effect);
		// Run this effect
		ledEffectRun();
	}
}

#endif // LEDS_NUMBER > 2

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* External Button Functions */

/** @Func Function for assigning a board event to a button action */
uint8_t boardButtonEventAssign(uint8_t button_pin_no, board_button_action_t action, board_event_t event)
{
	uint8_t err_code 		= NRF_SUCCESS;
	uint8_t button_idx 	= 0xFF;
	
#if BUTTONS_NUMBER > 0
	// Convert the pin number into button list index.
	BOARD_PIN_TO_IDX(board_button_list, BUTTONS_NUMBER, button_pin_no, button_idx);
	
	// Check whether the input button pin number is within the button list.
	if (button_idx < BUTTONS_NUMBER){
		
		/*
		// If the input board event is the default event
		if (event == BOARD_EVENT_DEFAULT){
			// Set default action: BOARD_EVENT_PUSH_BUTTON_x for PUSH actions, BOARD_EVENT_NOTHING for RELEASE and LONG_PUSH actions.
      event = (action == BOARD_BUTTON_ACTION_PUSH) ? (board_event_t)(BOARD_EVENT_PUSH_BUTTON_0 + button_idx) : BOARD_EVENT_NOTHING;
    }
		*/
		
		// Assign the board event to different button actions
    switch (action){
			case BOARD_BUTTON_ACTION_PUSH:
				board_button_events_list[button_idx].push_event	= event;
        break;
      case BOARD_BUTTON_ACTION_LONG_PUSH:
				board_button_events_list[button_idx].long_push_event = event;
        break;
      case BOARD_BUTTON_ACTION_RELEASE:
				board_button_events_list[button_idx].release_event = event;
        break;
			case BOARD_BUTTON_ACTION_RELEASE_AFTER_LONG_PUSH:
				board_button_events_list[button_idx].release_after_long_push	=	event;
				break;
      default:
				err_code = NRF_ERROR_INVALID_PARAM;
        break;
    }
  }
  else{// The input button pin number is wrong
		err_code = NRF_ERROR_INVALID_PARAM;
  }
	
#else // There are no buttons
	
	err_code = NRF_ERROR_INVALID_PARAM;
	
#endif // BUTTONS_NUMBER > 0
	
	return err_code;
}

/** @Func Enable One Specific Button */
uint8_t boardButtonEnable(uint8_t button_pin_no)
{
#if BUTTONS_NUMBER > 0
	uint8_t button_idx = 0xFF;
	// Convert the pin number into button list index.
	BOARD_PIN_TO_IDX(board_button_list, BUTTONS_NUMBER, button_pin_no, button_idx);
	
	if (button_idx < BUTTONS_NUMBER){
		nrf_drv_gpiote_in_event_enable(button_pin_no, true);
		return NRF_SUCCESS;
	}
	else{
		return NRF_ERROR_NOT_SUPPORTED;
	}
#else
  return NRF_ERROR_NOT_SUPPORTED;
#endif
}

/** @Func Disable One Specific Button */
uint8_t boardButtonDisable(uint8_t button_pin_no)
{
#if BUTTONS_NUMBER > 0
	uint8_t button_idx = 0xFF;
	// Convert the pin number into button list index.
	BOARD_PIN_TO_IDX(board_button_list, BUTTONS_NUMBER, button_pin_no, button_idx);
	
	if (button_idx < BUTTONS_NUMBER){
		nrf_drv_gpiote_in_event_enable(button_pin_no, true);
		return NRF_SUCCESS;
	}
	else{
		return NRF_ERROR_NOT_SUPPORTED;
	}
#else
  return NRF_ERROR_NOT_SUPPORTED;
#endif
}

/** @Func Function for Enabling All Buttons. */
uint8_t boardButtonEnableAll(void)
{
#if BUTTONS_NUMBER > 0
	return app_button_enable();
#else
  return NRF_ERROR_NOT_SUPPORTED;
#endif
}

/** @Func Function for Disabling All Buttons.*/
uint8_t boardButtonDisableAll(void)
{
#if BUTTONS_NUMBER > 0
   return app_button_disable();
#else
	return NRF_SUCCESS;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Function for Initializing On-Board Resources */
uint8_t boardConfig(uint32_t init_type, uint32_t ticks_per_ms, board_event_callback_t callback)
{
	uint8_t err_code 	= NRF_SUCCESS;
	
	// LEDs Configuration
#if LEDS_NUMBER > 0
	// Tag the flag for led initialization
	board_is_led_init = (bool)(init_type & BOARD_INIT_LEDS);
#endif

	// Button Configuration
#if BUTTONS_NUMBER > 0
	// Assign the callback function for the board event
	// Note: Only one callback function for the entire board,
	// but this function can change its operation depending on the input board_event_t parameter.
	board_callback 		= callback;
	
	// Convert the timer ticks
	BOARD_TICKS_CONVERSION(1, ticks_per_ms, BOARD_BUTTON_LONG_PUSH_TIMEOUT_MS, long_push_time_out_ticks);
	BOARD_TICKS_CONVERSION(1, ticks_per_ms, BOARD_BUTTON_DETECTION_DELAY_MS, button_detection_time_out_ticks);
	
	// Tag the flag for button initialization
	board_is_button_init = (bool)(init_type & BOARD_INIT_BUTTONS);
	
	// The board application package supports buttons and generate events
  if(board_is_button_init){
		// Assign the button push action to each button by default
		// When initializing all the buttons, each button has its all button actions recognized as board event nothing.
    for (uint8_t i = 0; i < BUTTONS_NUMBER; i++){
			err_code = boardButtonEventAssign(board_button_list[i], BOARD_BUTTON_ACTION_PUSH, BOARD_EVENT_NOTHING);
			if(err_code != NRF_SUCCESS) break;
			err_code = boardButtonEventAssign(board_button_list[i], BOARD_BUTTON_ACTION_RELEASE, BOARD_EVENT_NOTHING);
			if(err_code != NRF_SUCCESS) break;
			err_code = boardButtonEventAssign(board_button_list[i], BOARD_BUTTON_ACTION_LONG_PUSH, BOARD_EVENT_NOTHING);
			if(err_code != NRF_SUCCESS) break;
			err_code = boardButtonEventAssign(board_button_list[i], BOARD_BUTTON_ACTION_RELEASE_AFTER_LONG_PUSH, BOARD_EVENT_NOTHING);
			if(err_code != NRF_SUCCESS) break;
    }

		// Initialize All Button Input Pins as Buttons
		// If you want to initialize only some physical button, change the macro definitions in "data_board.h"
    if(err_code == NRF_SUCCESS){
			err_code = app_button_init((app_button_cfg_t *)board_buttons_cfg,
                                  BUTTONS_NUMBER,
                                  button_detection_time_out_ticks);
    }
		
		// Enable All Defined Buttons
		if(err_code == NRF_SUCCESS){
			err_code = app_button_enable();
    }
		
		// Create the Timer for Button Long Push Detection
    if (err_code == NRF_SUCCESS){
			err_code = app_timer_create(&button_timer_id,
                                  APP_TIMER_MODE_SINGLE_SHOT,
                                  board_button_timer_handler);
    }
   }
#else
	 UNUSED_VARIABLE(callback);
	 UNUSED_VARIABLE(ticks_per_ms);
#endif // BUTTONS_NUMBER > 0

	// LED Initialization
#if LEDS_NUMBER > 0
	if (board_is_led_init){
		// Turn off All Three RGB LEDS
		boardLedEffect(LED_EFFECT_RGB_OFF);
  }
#endif // LEDS_NUMBER > 0
	
	return err_code;
}
