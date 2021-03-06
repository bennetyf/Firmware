#ifndef TCS3472X_H__
#define TCS3472X_H__

#include "app_twi.h"

#include "nrf_gpio.h"

//#define TCS3472x_INIT_DELAY_MS      720     // RGBC Init (2.4 ms) + RGBC ADC (614 ms) = 616.4 ms

#define P1234701CT_ADDRESS	    0x2A   // 7-bit I2C address of P1234701CT is 0x2A

// Registers
#define P1234701CT_REG_RGB_SENSOR_CONTROL 							0x00

#define P1234701CT_REG_MANUAL_TIMING_HIGH_BYTE         	0x01
#define P1234701CT_REG_MANUAL_TIMING_LOW_BYTE						0x02

#define P1234701CT_REG_RED_CHANNEL_DATA_HIGH_BYTE       0x03
#define P1234701CT_REG_RED_CHANNEL_DATA_LOW_BYTE 				0x04
#define P1234701CT_REG_GREEN_CHANNEL_DATA_HIGH_BYTE 		0x05
#define P1234701CT_REG_GREEN_CHANNEL_DATA_LOW_BYTE  		0x06
#define P1234701CT_REG_BLUE_CHANNEL_DATA_HIGH_BYTE	    0x07
#define P1234701CT_REG_BLUE_CHANNEL_DATA_LOW_BYTE       0x08
#define P1234701CT_REG_INFRARED_CHANNEL_DATA_HIGH_BYTE	0x09
#define P1234701CT_REG_INFRARED_CHANNEL_DATA_LOW_BYTE   0x0A

#define P1234701CT_REG_PROXIMITY_SENSOR_CONTROL_1       0x0B
#define P1234701CT_REG_PROXIMITY_SENSOR_CONTROL_2       0x0C
#define P1234701CT_REG_PROXIMITY_SENSOR_THRESHOLD       0x0D

#define P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1       	0x0E
#define P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_2       	0x0F

#define P1234701CT_MONITOR        											0x10


//Settings
//RGB Sensor Control
#define P1234701CT_COLOR_SENSOR_RESET_OPERATION					0x00
#define P1234701CT_COLOR_SENSOR_RESET_RESET							0x80

#define P1234701CT_COLOR_SENSOR_SLEEP_FUNCTION_OPERATION  		0x00
#define P1234701CT_COLOR_SENSOR_SLEEP_FUNCTION_SLEEP	 				0x40

#define P1234701CT_COLOR_SENSOR_REGISTER_RESET_RESET_RELEASE 			0x00
#define P1234701CT_COLOR_SENSOR_REGISTER_RESET_ADDRESS_03_TO_0A		0x10

#define P1234701CT_GAIN_SELECTION_HIGH_GAIN 						0x00
#define P1234701CT_GAIN_SELECTION_LOW_GAIN 							0x08

#define P1234701CT_INTEGRATION_MODE_FIXED_TIME_MODE			0x00
#define P1234701CT_INTEGRATION_MODE_MANUAL_SETTING_MODE 0x04

#define P1234701CT_INTEGRATION_TIME_SETTING_32US 				0x00
#define P1234701CT_INTEGRATION_TIME_SETTING_1MS 		 		0x01
#define P1234701CT_INTEGRATION_TIME_SETTING_16MS 		 		0x02
#define P1234701CT_INTEGRATION_TIME_SETTING_131MS 		 	0x03


// 3-Color LED Driver Control
#define P1234701CT_COLOR_LED_RESET_OPERATION 						0x00
#define P1234701CT_COLOR_LED_RESET_RESET 								0x80

#define P1234701CT_COLOR_LED_SLEEP_FUNCTION_OPERATION 	0x00
#define P1234701CT_COLOR_LED_SLEEP_FUNCTION_SLEEP 			0x40

#define P1234701CT_COLOR_LED_DC_MODE_PULSE_MODE 				0x00
#define P1234701CT_COLOR_LED_DC_MODE_DC_MODE 						0x20

#define P1234701CT_COLOR_LED_ONE_TENTH_MODE_NORMAL_MODE 		0x00
#define P1234701CT_COLOR_LED_ONE_TENTH_MODE_ONE_TENTH_MODE 	0x10

#define P1234701CT_RED_LED_CURRENT_SELECTION_64MA 			0x08
#define P1234701CT_RED_LED_CURRENT_SELECTION_32MA 			0x04
#define P1234701CT_RED_LED_CURRENT_SELECTION_16MA 			0x02
#define P1234701CT_RED_LED_CURRENT_SELECTION_8MA 				0x01
#define P1234701CT_RED_LED_CURRENT_SELECTION_0MA 				0x00

#define P1234701CT_GREEN_LED_CURRENT_SELECTION_64MA 		0x80
#define P1234701CT_GREEN_LED_CURRENT_SELECTION_32MA 		0x40
#define P1234701CT_GREEN_LED_CURRENT_SELECTION_16MA 		0x20
#define P1234701CT_GREEN_LED_CURRENT_SELECTION_8MA 			0x10
#define P1234701CT_GREEN_LED_CURRENT_SELECTION_0MA 			0x00

#define P1234701CT_BLUE_LED_CURRENT_SELECTION_64MA 			0x08
#define P1234701CT_BLUE_LED_CURRENT_SELECTION_32MA 			0x04
#define P1234701CT_BLUE_LED_CURRENT_SELECTION_16MA 			0x02
#define P1234701CT_BLUE_LED_CURRENT_SELECTION_8MA 			0x01
#define P1234701CT_BLUE_LED_CURRENT_SELECTION_0MA 			0x00


//Functions
bool P1234701CT_led_current(app_twi_t *m_app_twi, uint8_t *current, uint8_t byte_cnt);

bool P1234701CT_red_on(app_twi_t *m_app_twi);
bool P1234701CT_red_off(app_twi_t *m_app_twi);
bool P1234701CT_green_on(app_twi_t *m_app_twi);
bool P1234701CT_green_off(app_twi_t *m_app_twi);
bool P1234701CT_blue_on(app_twi_t *m_app_twi);
bool P1234701CT_blue_off(app_twi_t *m_app_twi);

bool P1234701CT_get_LED_control_setting(app_twi_t *m_app_twi, uint8_t *p_buffer, uint8_t byte_cnt);

bool P1234701CT_get_sensor_control_status(app_twi_t *m_app_twi, uint8_t *p_buffer, uint8_t byte_cnt);
bool P1234701CT_get_raw_data(app_twi_t *m_app_twi, uint8_t *p_buffer, uint8_t byte_cnt);
bool P1234701CT_sensor_setup(app_twi_t *m_app_twi, uint16_t value);
bool P1234701CT_start_sampling(app_twi_t *m_app_twi);
bool P1234701CT_sensor_sleep(app_twi_t *m_app_twi);
#endif
