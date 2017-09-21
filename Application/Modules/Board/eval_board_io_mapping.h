/** Library Name: "eval_board_io_mapping.h" 
 * @Brief 	This library defines all the IO utilities on the board
 *
 * @Macro		-	LED Pin Definitions
 * @Macro		-	Button Pin Definitions
 * @Macro		-	UART Pin Definitions
 * @Macro		- SPI Pin Definitions
 * @Macro		- Serialization Definitions
 * @Macro		- Arduino Board Mappings
 * @Macro		-	Low Frequency Clock Source(Softdevice)
 *
 * 
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __EVAL_BOARD_IO_MAPPING_H__
#define __EVAL_BOARD_IO_MAPPING_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ Library Header */

#ifdef __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/* System Modules */
#include "nrf_gpio.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro LEDs definitions */
	
#define LEDS_NUMBER    3										// Total number of LEDs to be used

#define LED_START      17										// Start pin number of LEDs
#define LED_0        	 17					  				// LED_0 pin number
#define LED_1      		 18										// LED_1 pin number
#define LED_2       	 19										// LED_2 pin number
#define LED_4          20										// LED_3 pin number
#define LED_STOP       19										// Stop pin number of LEDs

#define LEDS_ACTIVE_STATE 0									// The active state of the LED "on" state

#define LED_RED				LED_0									// The pin number of the red LED
#define LED_GREEN			LED_1									// The pin number of the green LED
#define LED_BLUE			LED_2									// The pin number of the blue LED
	
#define LEDS_LIST { LED_RED, LED_GREEN, LED_BLUE } // The LED list

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Button definitions */

#define BUTTONS_NUMBER 4										// Total number of on-board buttons

#define BUTTON_START   13										// Start pin number of buttons
#define BUTTON_0       13										// Button_0 pin number
#define BUTTON_1       14										// Button_1 pin number
#define BUTTON_2       15										// Button_2 pin number
#define BUTTON_3       16										// Button_3 pin number
#define BUTTON_STOP    16										// Stop pin number of buttons
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP 	// Button pin input mode configuration

#define BUTTONS_ACTIVE_STATE 0							// The active state of the button "pushed" state

#define BOARD_BUTTON_0   BUTTON_0						// The pin number of the app button 0
#define BOARD_BUTTON_1   BUTTON_1						// The pin number of the app button 1
#define BOARD_BUTTON_2   BUTTON_2						// The pin number of the app button 2
#define BOARD_BUTTON_3   BUTTON_3						// The pin number of the app button 3

// The app button list
#define BUTTONS_LIST { BOARD_BUTTON_0, BOARD_BUTTON_1, BOARD_BUTTON_2, BOARD_BUTTON_3 }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro UART Pins */

#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6
#define CTS_PIN_NUMBER 7
#define RTS_PIN_NUMBER 5
#define HWFC           true

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro SPI Pins */

#define SPIS_MISO_PIN   28  // SPI MISO signal.
#define SPIS_CSN_PIN    12  // SPI CSN signal.
#define SPIS_MOSI_PIN   25  // SPI MOSI signal.
#define SPIS_SCK_PIN    29  // SPI SCK signal.

#define SPIM0_SCK_PIN   29  // SPI clock GPIO pin number.
#define SPIM0_MOSI_PIN  25  // SPI Master Out Slave In GPIO pin number.
#define SPIM0_MISO_PIN  28  // SPI Master In Slave Out GPIO pin number.
#define SPIM0_SS_PIN    12  // SPI Slave Select GPIO pin number.

#define SPIM1_SCK_PIN   2   // SPI clock GPIO pin number.
#define SPIM1_MOSI_PIN  3   // SPI Master Out Slave In GPIO pin number.
#define SPIM1_MISO_PIN  4   // SPI Master In Slave Out GPIO pin number.
#define SPIM1_SS_PIN    5   // SPI Slave Select GPIO pin number.

#define SPIM2_SCK_PIN   12  // SPI clock GPIO pin number.
#define SPIM2_MOSI_PIN  13  // SPI Master Out Slave In GPIO pin number.
#define SPIM2_MISO_PIN  14  // SPI Master In Slave Out GPIO pin number.
#define SPIM2_SS_PIN    15  // SPI Slave Select GPIO pin number.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Serialization Pins */

// serialization APPLICATION board - temp. setup for running serialized MEMU tests
#define SER_APP_RX_PIN              23    // UART RX pin number.
#define SER_APP_TX_PIN              24    // UART TX pin number.
#define SER_APP_CTS_PIN             2     // UART Clear To Send pin number.
#define SER_APP_RTS_PIN             25    // UART Request To Send pin number.

#define SER_APP_SPIM0_SCK_PIN       27     // SPI clock GPIO pin number.
#define SER_APP_SPIM0_MOSI_PIN      2      // SPI Master Out Slave In GPIO pin number
#define SER_APP_SPIM0_MISO_PIN      26     // SPI Master In Slave Out GPIO pin number
#define SER_APP_SPIM0_SS_PIN        23     // SPI Slave Select GPIO pin number
#define SER_APP_SPIM0_RDY_PIN       25     // SPI READY GPIO pin number
#define SER_APP_SPIM0_REQ_PIN       24     // SPI REQUEST GPIO pin number

// serialization CONNECTIVITY board
#define SER_CON_RX_PIN              24    // UART RX pin number.
#define SER_CON_TX_PIN              23    // UART TX pin number.
#define SER_CON_CTS_PIN             25    // UART Clear To Send pin number. Not used if HWFC is set to false.
#define SER_CON_RTS_PIN             2     // UART Request To Send pin number. Not used if HWFC is set to false.


#define SER_CON_SPIS_SCK_PIN        27    // SPI SCK signal.
#define SER_CON_SPIS_MOSI_PIN       2     // SPI MOSI signal.
#define SER_CON_SPIS_MISO_PIN       26    // SPI MISO signal.
#define SER_CON_SPIS_CSN_PIN        23    // SPI CSN signal.
#define SER_CON_SPIS_RDY_PIN        25    // SPI READY GPIO pin number.
#define SER_CON_SPIS_REQ_PIN        24    // SPI REQUEST GPIO pin number.

#define SER_CONN_CHIP_RESET_PIN     11    // Pin used to reset connectivity chip

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Arduino Board Mappings */

#define ARDUINO_SCL_PIN             27    // SCL signal pin
#define ARDUINO_SDA_PIN             26    // SDA signal pin
#define ARDUINO_AREF_PIN            2     // Aref pin
#define ARDUINO_13_PIN              25    // Digital pin 13
#define ARDUINO_12_PIN              24    // Digital pin 12
#define ARDUINO_11_PIN              23    // Digital pin 11
#define ARDUINO_10_PIN              22    // Digital pin 10
#define ARDUINO_9_PIN               20    // Digital pin 9
#define ARDUINO_8_PIN               19    // Digital pin 8

#define ARDUINO_7_PIN               18    // Digital pin 7
#define ARDUINO_6_PIN               17    // Digital pin 6
#define ARDUINO_5_PIN               16    // Digital pin 5
#define ARDUINO_4_PIN               15    // Digital pin 4
#define ARDUINO_3_PIN               14    // Digital pin 3
#define ARDUINO_2_PIN               13    // Digital pin 2
#define ARDUINO_1_PIN               12    // Digital pin 1
#define ARDUINO_0_PIN               11    // Digital pin 0

#define ARDUINO_A0_PIN              3     // Analog channel 0
#define ARDUINO_A1_PIN              4     // Analog channel 1
#define ARDUINO_A2_PIN              28    // Analog channel 2
#define ARDUINO_A3_PIN              29    // Analog channel 3
#define ARDUINO_A4_PIN              30    // Analog channel 4
#define ARDUINO_A5_PIN              31    // Analog channel 5

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
