#ifndef __DATA_SENSOR_CONFIG_H__
#define __DATA_SENSOR_CONFIG_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro TWI Physical Address of Color Sensor P12347-01CT */
#define SENSOR_ADDRESS	    																0x2A   							// 7-bit I2C address of P1234701CT is 0x2A

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Color Sensor Register Addresses (Register Map) */

/** @Macro Sensor General Control Register Address */
#define SENSOR_REG_CONTROL 																	0x00

/** @Macro Sensor Manual Timing Register Address */
#define SENSOR_REG_MANUAL_TIMING_HIGH_BYTE         					0x01
#define SENSOR_REG_MANUAL_TIMING_LOW_BYTE										0x02

/** @Macro Sensor Output Data Register Address */
#define SENSOR_REG_RED_DATA_HIGH_BYTE       								0x03
#define SENSOR_REG_RED_DATA_LOW_BYTE 												0x04
#define SENSOR_REG_GREEN_DATA_HIGH_BYTE 										0x05
#define SENSOR_REG_GREEN_DATA_LOW_BYTE  										0x06
#define SENSOR_REG_BLUE_DATA_HIGH_BYTE	    								0x07
#define SENSOR_REG_BLUE_DATA_LOW_BYTE       								0x08
#define SENSOR_REG_INFRARED_DATA_HIGH_BYTE									0x09
#define SENSOR_REG_INFRARED_DATA_LOW_BYTE   								0x0A

/** @Macro Sensor Proximity Sensor Control Register Address */
#define SENSOR_REG_PROXIMITY_SENSOR_CONTROL_1       				0x0B
#define SENSOR_REG_PROXIMITY_SENSOR_CONTROL_2       				0x0C
#define SENSOR_REG_PROXIMITY_SENSOR_THRESHOLD       				0x0D

/** @Macro Sensor LED Drive Control Register Address */
#define SENSOR_REG_COLOR_LED_DRIVE_CONTROL_1       					0x0E
#define SENSOR_REG_COLOR_LED_DRIVE_CONTROL_2       					0x0F

/** @Macro Sensor Monitor Register Address */
#define SENSOR_REG_MONITOR        													0x10

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Sensor Controls (Operations) */

/** @Macro Sensor Resetting Controls */
#define SENSOR_CTRL_RESET_OPERATION													0x00
#define SENSOR_CTRL_RESET_RESET															0x80

/** @Macro Sensor Sleeping Controls */
#define SENSOR_CTRL_SLEEP_OPERATION  												0x00
#define SENSOR_CTRL_SLEEP_SLEEP 														0x40

/** @Macro Sensor Register Reset Controls */
#define SENSOR_CTRL_REG_RESET_RESET_RELEASE 								0x00
#define SENSOR_CTRL_REG_RESET_03_TO_0A											0x10

/** @Macro Sensor Gain Selection Controls */
#define SENSOR_CTRL_GAIN_SELECT_HIGH 												0x00
#define SENSOR_CTRL_GAIN_SELECT_LOW 												0x08

/** @Macro Sensor Integration Mode Controls */
#define SENSOR_CTRL_INT_MODE_FIXED_TIME											0x00
#define SENSOR_CTRL_INT_MODE_MANUAL_SETTING 								0x04

/** @Macro Sensor Integration Time Settings */
#define SENSOR_CTRL_INT_TIME_SETTING_32US 									0x00
#define SENSOR_CTRL_INT_TIME_SETTING_1MS 		 								0x01
#define SENSOR_CTRL_INT_TIME_SETTING_16MS 		 							0x02
#define SENSOR_CTRL_INT_TIME_SETTING_131MS 		 							0x03

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 3-Color LED Driver Control */

/** @Macro 3-Color LED Driver Resetting Controls */
#define SENSOR_LED_RESET_OPERATION 													0x00
#define SENSOR_LED_RESET_RESET 															0x80

/** @Macro 3-Color LED Driver Sleeping Controls */
#define SENSOR_LED_SLEEP_OPERATION 													0x00
#define SENSOR_LED_SLEEP_SLEEP 															0x40

/** @Macro 3-Color LED Driver DC Mode Controls */
#define SENSOR_LED_DC_MODE_PULSE 														0x00
#define SENSOR_LED_DC_MODE_DC 															0x20

/** @Macro 3-Color LED Driver One-Tenth Mode Controls */
#define SENSOR_LED_ONE_TENTH_MODE_NORMAL 										0x00
#define SENSOR_LED_ONE_TENTH_MODE_ONE_TENTH 								0x10

/** @Macro 3-Color LED Driver LED Current Selection Controls */
/** @Macro RED LED Current */
#define SENSOR_LED_RED_CURRENT_64MA 												0x08
#define SENSOR_LED_RED_CURRENT_32MA 												0x04
#define SENSOR_LED_RED_CURRENT_16MA 												0x02
#define SENSOR_LED_RED_CURRENT_8MA 													0x01
#define SENSOR_LED_RED_CURRENT_0MA 													0x00

/** @Macro GREEN LED Current */
#define SENSOR_LED_GREEN_CURRENT_64MA 											0x80
#define SENSOR_LED_GREEN_CURRENT_32MA 											0x40
#define SENSOR_LED_GREEN_CURRENT_16MA 											0x20
#define SENSOR_LED_GREEN_CURRENT_8MA 												0x10
#define SENSOR_LED_GREEN_CURRENT_0MA 												0x00

/** @Macro BLUE LED Current */
#define SENSOR_LED_BLUE_CURRENT_64MA 												0x08
#define SENSOR_LED_BLUE_CURRENT_32MA 												0x04
#define SENSOR_LED_BLUE_CURRENT_16MA 												0x02
#define SENSOR_LED_BLUE_CURRENT_8MA 												0x01
#define SENSOR_LED_BLUE_CURRENT_0MA 												0x00

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DATA_SENSOR_CONFIG_H__
