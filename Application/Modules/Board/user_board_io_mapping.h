/** Library Name: "user_board_io_mapping.h" 
 * @Brief 	This library defines all the IO utilities on the board
 *
 * @Macro		-	LED Pin Definitions
 * @Macro		-	Button Pin Definitions
 * @Macro		-	UART Pin Definitions
 * @Macro		-	Low Frequency Clock Source(Softdevice)
 *
 * 
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __USER_BOARD_IO_MAPPING_H__
#define __USER_BOARD_IO_MAPPING_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* System Modules */
//#include "nrf_gpio.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Define all used pin numbers */

#define PIN_00 									0
#define PIN_01 									1
#define PIN_05 									5
#define PIN_06 									6
#define PIN_08 									8
#define PIN_12 									12
#define PIN_14 									14
#define PIN_15 									15
#define PIN_16 									16
#define PIN_18 									18
#define PIN_21 									21
#define PIN_24 									24
#define PIN_28 									28
#define PIN_29 									29
#define PIN_30 									30

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/** @Macro LEDs definitions */
	
#define LEDS_NUMBER    					3													// Total number of LEDs to be used

#define LEDS_ACTIVE_STATE				0													// The active state of the LED "on" state

#define LED_RED									PIN_00										// The pin number of the red LED
#define LED_GREEN								PIN_12										// The pin number of the green LED
#define LED_BLUE								PIN_24										// The pin number of the blue LED
	
#define LEDS_LIST 							{LED_RED, LED_GREEN, LED_BLUE} 		// The LED list

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Button Definitions */

#define BUTTONS_NUMBER 					1													// Total number of on-board buttons

#define BUTTON_PULL    					NRF_GPIO_PIN_PULLUP 			// Button pin input mode configuration
#define BUTTONS_ACTIVE_STATE 		0													// The active state of the button "pushed" state

#define BOARD_BUTTON_0					PIN_05

#define BUTTONS_LIST 						{BOARD_BUTTON_0}					// The app button list

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Other Pin Definitions */

#define VBAT_ADC_EN 						PIN_01
#define VBAT_ADC								PIN_30

#define USB_PRESENT_INT					PIN_14
#define CHARGE_FULL_INT					PIN_15
#define ENTER_SHIP_MODE					PIN_21

#define INT_OPEN								PIN_28
#define INT_CLOSED							PIN_29

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro UART Pins */

#define RX_PIN_NUMBER  								PIN_08
#define TX_PIN_NUMBER 		 						PIN_06
#define RTS_PIN_NUMBER								2				// Flow control is always disabled, so this is dummy pin number.
#define CTS_PIN_NUMBER							  3				// Flow control is always disabled, so this is dummy pin number.
#define HWFC           								true

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro I2C Pins */

#define I2C_SCL									PIN_16
#define I2C_SDA									PIN_18

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Low Frequency Clock Source Macro Using SoftDevice */

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_SYNTH,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																 
/* C++ Library Header */
																 
#ifdef __cplusplus
}
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __DATA_BOARD_H__
