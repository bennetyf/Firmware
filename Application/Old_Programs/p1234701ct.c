#include "p1234701ct.h"

//#include <stdint.h>
//#include <string.h>
#include "nrf_delay.h"

#include "app_twi.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define Post_Sample_Wait_Time_Margin  1.2  //1.05 some zero-readings
// no longer require 
// it was used to polling to detect actual sampling time.
// #define Pre_Sample_Wait_Time_Margin  0.8   //0.9 was fine

// Red LED - 9.6mA
#define Red_Current 			 	(P1234701CT_RED_LED_CURRENT_SELECTION_64MA \
													| P1234701CT_RED_LED_CURRENT_SELECTION_32MA)
													
// Green LED - 12mA
#define Green_Current 			(P1234701CT_GREEN_LED_CURRENT_SELECTION_64MA	\
													| P1234701CT_GREEN_LED_CURRENT_SELECTION_32MA	\
													| P1234701CT_GREEN_LED_CURRENT_SELECTION_16MA	\
													| P1234701CT_GREEN_LED_CURRENT_SELECTION_8MA)

// Blue LED - 2.4mA													
#define Blue_Current				(P1234701CT_BLUE_LED_CURRENT_SELECTION_16MA \
													| P1234701CT_BLUE_LED_CURRENT_SELECTION_8MA)
													
#define LED_ON_Command		(P1234701CT_COLOR_LED_RESET_OPERATION \
													| P1234701CT_COLOR_LED_DC_MODE_DC_MODE \
													| P1234701CT_COLOR_LED_ONE_TENTH_MODE_ONE_TENTH_MODE)	

#define LED_OFF_Command		(P1234701CT_COLOR_LED_RESET_RESET \
													| P1234701CT_COLOR_LED_SLEEP_FUNCTION_SLEEP \
													| P1234701CT_COLOR_LED_DC_MODE_DC_MODE \
													| P1234701CT_COLOR_LED_ONE_TENTH_MODE_ONE_TENTH_MODE)	


//Integration Time Settings 
#define Integration_Time 		P1234701CT_INTEGRATION_TIME_SETTING_32US


#define Manual_Timing				1875u			//32us X 1875 = 60ms

#define Sensor_Setup 				(P1234701CT_COLOR_SENSOR_RESET_RESET \
													| P1234701CT_COLOR_SENSOR_REGISTER_RESET_ADDRESS_03_TO_0A \
													| P1234701CT_GAIN_SELECTION_LOW_GAIN \
													| P1234701CT_INTEGRATION_MODE_MANUAL_SETTING_MODE)

#define Start_Sampling 			(P1234701CT_COLOR_SENSOR_RESET_OPERATION	\
													| P1234701CT_COLOR_SENSOR_SLEEP_FUNCTION_OPERATION \
													| P1234701CT_COLOR_SENSOR_REGISTER_RESET_RESET_RELEASE\
													| P1234701CT_GAIN_SELECTION_LOW_GAIN \
													| P1234701CT_INTEGRATION_MODE_MANUAL_SETTING_MODE \
													| P1234701CT_INTEGRATION_TIME_SETTING_32US)

#define Sensor_Sleep				(P1234701CT_COLOR_SENSOR_SLEEP_FUNCTION_SLEEP \
													| P1234701CT_GAIN_SELECTION_LOW_GAIN \
													| P1234701CT_INTEGRATION_MODE_MANUAL_SETTING_MODE)


// Function to set LED current
bool P1234701CT_led_current(app_twi_t *m_app_twi, uint8_t *current, uint8_t byte_cnt){
		
		uint32_t err_code;		
		
		app_twi_transfer_t const red_current_transfers[] = {
			APP_TWI_WRITE(P1234701CT_ADDRESS, current, byte_cnt, 0)
		};
			
		//work out the number of transfers
		uint8_t number_of_transfers = sizeof(red_current_transfers)/sizeof(red_current_transfers[0]);
		//send data on I2C channal (Blocking/Synchronous Approach)
		err_code = app_twi_perform(m_app_twi, red_current_transfers, number_of_transfers, NULL);

		if (err_code == 0x00){
			return true;
		}
		else {
			NRF_LOG_INFO("RED LED CURRENT SETTING RETURNS: %d !\r\n",err_code);
			NRF_LOG_FLUSH();
		}

		return false;
}


// --------- Red LED Control -----------
bool P1234701CT_red_on(app_twi_t *m_app_twi){
		
		uint8_t current[] = { P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, \
													LED_OFF_Command | Red_Current, \
													P1234701CT_GREEN_LED_CURRENT_SELECTION_0MA | P1234701CT_BLUE_LED_CURRENT_SELECTION_0MA };	
													
		if (P1234701CT_led_current(m_app_twi, current, sizeof(current))){
			
			uint32_t err_code;
			uint8_t  command[] = {P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, LED_ON_Command | Red_Current};

			app_twi_transfer_t const red_current_transfers[] = {
				APP_TWI_WRITE(P1234701CT_ADDRESS, command, sizeof(command), 0)
			};
			//work out the number of transfers
			uint8_t number_of_transfers = sizeof(red_current_transfers)/sizeof(red_current_transfers[0]);
			//send data on I2C channal (Blocking/Synchronous Approach)
			err_code = app_twi_perform(m_app_twi, red_current_transfers, number_of_transfers, NULL);
			
			if (err_code == 0x00){
				return true;
			}
			else{
				NRF_LOG_INFO("<Command>Red LED Command Sending Failed !\r\n");
				NRF_LOG_FLUSH();
			}
		}
		else{
			NRF_LOG_INFO("<Current>Red LED Current Setting Failed !\r\n");
			NRF_LOG_FLUSH();
		}
		return false;
		
}


bool P1234701CT_red_off(app_twi_t *m_app_twi){
		//set all current to 0mA and switch off operation
		uint8_t current[] = { P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, \
													LED_OFF_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA, \
													P1234701CT_GREEN_LED_CURRENT_SELECTION_0MA | P1234701CT_BLUE_LED_CURRENT_SELECTION_0MA };
						
		return P1234701CT_led_current(m_app_twi, current, sizeof(current));
}




// --------- Green LED Control -----------
bool P1234701CT_green_on(app_twi_t *m_app_twi){
		
		uint8_t current[] = { P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, \
													LED_OFF_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA, \
													Green_Current | P1234701CT_BLUE_LED_CURRENT_SELECTION_0MA };
						
		if (P1234701CT_led_current(m_app_twi, current, sizeof(current))){
			
			uint32_t err_code;
			uint8_t  command[] = {P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, LED_ON_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA};

			app_twi_transfer_t const red_current_transfers[] = {
				APP_TWI_WRITE(P1234701CT_ADDRESS, command, sizeof(command), 0)
			};
			//work out the number of transfers
			uint8_t number_of_transfers = sizeof(red_current_transfers)/sizeof(red_current_transfers[0]);
			//send data on I2C channal (Blocking/Synchronous Approach)
			err_code = app_twi_perform(m_app_twi, red_current_transfers, number_of_transfers, NULL);
			
			if (err_code == 0x00){
				return true;
			}
			
		}
		
		return false;
		
}


bool P1234701CT_green_off(app_twi_t *m_app_twi){
		//set all current to 0mA and switch off operation
		uint8_t current[] = { P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, \
													LED_OFF_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA, \
													P1234701CT_GREEN_LED_CURRENT_SELECTION_0MA | P1234701CT_BLUE_LED_CURRENT_SELECTION_0MA };
						
		return P1234701CT_led_current(m_app_twi, current, sizeof(current));
}



// --------- Blue LED Control -----------
bool P1234701CT_blue_on(app_twi_t *m_app_twi){
		
		uint8_t current[] = { P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, \
													LED_OFF_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA, \
													P1234701CT_GREEN_LED_CURRENT_SELECTION_0MA | Blue_Current };
						
		if (P1234701CT_led_current(m_app_twi, current, sizeof(current))){
			
			uint32_t err_code;
			uint8_t  command[] = {P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, LED_ON_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA};

			app_twi_transfer_t const red_current_transfers[] = {
				APP_TWI_WRITE(P1234701CT_ADDRESS, command, sizeof(command), 0)
			};
			//work out the number of transfers
			uint8_t number_of_transfers = sizeof(red_current_transfers)/sizeof(red_current_transfers[0]);
			//send data on I2C channal (Blocking/Synchronous Approach)
			err_code = app_twi_perform(m_app_twi, red_current_transfers, number_of_transfers, NULL);
			
			if (err_code == 0x00){
				return true;
			}
			
		}
		
		return false;
		
}


bool P1234701CT_blue_off(app_twi_t *m_app_twi){
		//set all current to 0mA and switch off operation
		uint8_t current[] = { P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1, \
													LED_OFF_Command | P1234701CT_RED_LED_CURRENT_SELECTION_0MA, \
													P1234701CT_GREEN_LED_CURRENT_SELECTION_0MA | P1234701CT_BLUE_LED_CURRENT_SELECTION_0MA };
						
		return P1234701CT_led_current(m_app_twi, current, sizeof(current));
}






// ------ Get LED Control Status --------
bool P1234701CT_get_LED_control_setting(app_twi_t *m_app_twi, uint8_t *p_buffer, uint8_t byte_cnt){

		uint32_t err_code;
		uint8_t reg_add = P1234701CT_REG_COLOR_LED_DRIVE_CONTROL_1;

		app_twi_transfer_t const read_status_transfers[] = {
			
			APP_TWI_WRITE(P1234701CT_ADDRESS, &reg_add, sizeof(reg_add), APP_TWI_NO_STOP),\
			APP_TWI_READ (P1234701CT_ADDRESS, p_buffer, byte_cnt, 0)
				
		};
		//work out the number of transfers
		uint8_t number_of_transfers = sizeof(read_status_transfers)/sizeof(read_status_transfers[0]);
		//send data on I2C channal (Blocking/Synchronous Approach)
		err_code = app_twi_perform(m_app_twi, read_status_transfers, number_of_transfers, NULL);

		if (err_code == 0x00){
			return true;
		}

		return false;
		
}


// ------ Get Sensor Control Status --------
bool P1234701CT_get_sensor_control_status(app_twi_t *m_app_twi, uint8_t *p_buffer, uint8_t byte_cnt){

		uint32_t err_code;
		uint8_t reg_add = P1234701CT_REG_RGB_SENSOR_CONTROL;

		app_twi_transfer_t const read_status_transfers[] = {
			
			APP_TWI_WRITE(P1234701CT_ADDRESS, &reg_add, sizeof(reg_add), APP_TWI_NO_STOP),\
			APP_TWI_READ (P1234701CT_ADDRESS, p_buffer, byte_cnt, 0)
				
		};
		//work out the number of transfers
		uint8_t number_of_transfers = sizeof(read_status_transfers)/sizeof(read_status_transfers[0]);
		//send data on I2C channal (Blocking/Synchronous Approach)
		err_code = app_twi_perform(m_app_twi, read_status_transfers, number_of_transfers, NULL);

		if (err_code == 0x00){
			return true;
		}

		return false;
		
}

// -------- RGB Sensor Control ----------
bool P1234701CT_get_raw_data(app_twi_t *m_app_twi, uint8_t *p_buffer, uint8_t byte_cnt){
				
		uint16_t timing = Manual_Timing	;
	
		if (P1234701CT_sensor_setup(m_app_twi, timing)){
			
			nrf_delay_ms(2);
			
			if (P1234701CT_start_sampling(m_app_twi)){
				uint32_t err_code;
				uint32_t wait_time;
				
				switch(Integration_Time){
					case P1234701CT_INTEGRATION_TIME_SETTING_32US:
						wait_time = (uint32_t)(0.032 * Manual_Timing * 4 * Post_Sample_Wait_Time_Margin);
						break;
					
					case P1234701CT_INTEGRATION_TIME_SETTING_1MS:
						wait_time = (uint32_t)(1 * Manual_Timing * 4 * Post_Sample_Wait_Time_Margin);
						break;
					
					case P1234701CT_INTEGRATION_TIME_SETTING_16MS:
						wait_time = (uint32_t)(16.0 * Manual_Timing * 4 * Post_Sample_Wait_Time_Margin);
						break;
					
					case P1234701CT_INTEGRATION_TIME_SETTING_131MS:
						wait_time = (uint32_t)(131.0 * Manual_Timing * 4 * Post_Sample_Wait_Time_Margin);
						break;
				
					default:
						wait_time = (uint32_t)(0.032 * Manual_Timing * 4 * Post_Sample_Wait_Time_Margin);
						break;
				}
				
				nrf_delay_ms(wait_time);
								
				
				uint8_t reg_add = P1234701CT_REG_RED_CHANNEL_DATA_HIGH_BYTE;

				app_twi_transfer_t const read_data_transfers[] = {
					
					APP_TWI_WRITE(P1234701CT_ADDRESS, &reg_add, sizeof(reg_add), APP_TWI_NO_STOP),\
					APP_TWI_READ (P1234701CT_ADDRESS, p_buffer, byte_cnt, 0)
						
				};
				
				//work out the number of transfers
				uint8_t number_of_transfers = sizeof(read_data_transfers)/sizeof(read_data_transfers[0]);
				//send data on I2C channal (Blocking/Synchronous Approach)
				err_code = app_twi_perform(m_app_twi, read_data_transfers, number_of_transfers, NULL);
				
				
				if (err_code == 0x00){					
					if (P1234701CT_sensor_sleep(m_app_twi)){
						return true;
					}
				}
			}
		}
	
		return false;
}


bool P1234701CT_sensor_setup(app_twi_t *m_app_twi, uint16_t value){
		
		uint32_t err_code;
		uint8_t setup_data[4];
		
		setup_data[0] = P1234701CT_REG_RGB_SENSOR_CONTROL;
		setup_data[1] = Sensor_Setup;
		setup_data[2] = (value >> 8) & 0xff;
		setup_data[3] = value & 0xff;
	
		app_twi_transfer_t const sensor_setup_transfers[] = {
			APP_TWI_WRITE(P1234701CT_ADDRESS, setup_data, sizeof(setup_data), 0)
		};
		//work out the number of transfers
		uint8_t number_of_transfers = sizeof(sensor_setup_transfers)/sizeof(sensor_setup_transfers[0]);
		//send data on I2C channal (Blocking/Synchronous Approach)
		err_code = app_twi_perform(m_app_twi, sensor_setup_transfers, number_of_transfers, NULL);

		if (err_code == 0x00){
			return true;
		}

		return false;
}

bool P1234701CT_start_sampling(app_twi_t *m_app_twi){
		
		uint32_t err_code;
		uint8_t data[2];
		
		data[0] = P1234701CT_REG_RGB_SENSOR_CONTROL;
		data[1] = Start_Sampling;
		
		app_twi_transfer_t const sensor_sampling_transfers[] = {
			APP_TWI_WRITE(P1234701CT_ADDRESS, data, sizeof(data), 0)
		};
		//work out the number of transfers
		uint8_t number_of_transfers = sizeof(sensor_sampling_transfers)/sizeof(sensor_sampling_transfers[0]);
		//send data on I2C channal (Blocking/Synchronous Approach)
		err_code = app_twi_perform(m_app_twi, sensor_sampling_transfers, number_of_transfers, NULL);

		if (err_code == 0x00){
			return true;
		}

		return false;
}


bool P1234701CT_sensor_sleep(app_twi_t *m_app_twi){
		
		uint32_t err_code;
		uint8_t data[2];
		
		data[0] = P1234701CT_REG_RGB_SENSOR_CONTROL;
		data[1] = Sensor_Sleep;
		
		app_twi_transfer_t const sensor_sleep_transfers[] = {
			APP_TWI_WRITE(P1234701CT_ADDRESS, data, sizeof(data), 0)
		};
		//work out the number of transfers
		uint8_t number_of_transfers = sizeof(sensor_sleep_transfers)/sizeof(sensor_sleep_transfers[0]);
		//send data on I2C channal (Blocking/Synchronous Approach)
		err_code = app_twi_perform(m_app_twi, sensor_sleep_transfers, number_of_transfers, NULL);

		if (err_code == 0x00){
			return true;
		}

		return false;
}






