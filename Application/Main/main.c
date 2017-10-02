/** The Main Application Functions
	*
	*
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "data_general_config.h"
#include "initialization.h"
#include "event_handlers.h"
#include "operations.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Function for application main entry */
int main(void)
{
	// Initialization
	enableLogger();	
	moduleInit();
	// Main Execution
	
	/* Test LED Effect */
	boardLedEffect(LED_EFFECT_FIRST);
	
	/* Test BLE Advertising */
	if(isLoggerEnabled()){
		NRF_LOG_INFO("Application Started...\r\n");
		NRF_LOG_FLUSH();
	}
  APP_ERROR_CHECK(ble_advertising_start(BLE_ADV_MODE_FAST)); 	// Start the BLE Advertising
	
	/* Test Flash Storage Module */
//	uint8_t flash_data[4];
//	if(getOneRecord(42,flash_data) == 0x00){
//		NRF_LOG_INFO("Printing Flash Storage Data...\r\n");
//		NRF_LOG_FLUSH();
//		for(uint8_t i = 0;i<4;i++){
//			NRF_LOG_INFO("Flash Data[%d] : 0x%2x\r\n",i,flash_data[i]);
//			NRF_LOG_FLUSH();
//		}
//	}
	
	
	/* Test Sensor Reading */
	
//	uint8_t sensor_data[18];
//	for(uint8_t i =0;i<18;i++){
//		sensor_data[i] = i+1;
//	}
//	
//	if(isLoggerEnabled()){
//		NRF_LOG_INFO("Printing Sensor Data(Before Color Sampling)...\r\n");
//		NRF_LOG_FLUSH();
//		for(uint8_t i = 0;i < 2;i++){
//			NRF_LOG_INFO("Data[%d] : 0x%2x\r\n",i,sensor_data[i]);
//			NRF_LOG_FLUSH();
//		}
//	}
	
	uint8_t sensor_data_length;
//	uint8_t * sensor_data = sensorDataAddress(&sensor_data_length);
	uint8_t	* sensor_data;
	
//		uint8_t err_code = sensorSampleColor(sensor_data,18);
//		if(err_code == NRF_SUCCESS){
//	if(sensor_data != NULL){
//			if(isLoggerEnabled()){
//				NRF_LOG_INFO("Printing Sensor Data(After Color Sampling)...\r\n");
//				NRF_LOG_FLUSH();
//				for (uint8_t i = 0;i < 5;i++){
//					NRF_LOG_INFO("Data[%d] : 0x%2x\r\n",i,sensor_data[i]);
//					NRF_LOG_FLUSH();
//				}
//			}
//		}

	// Main Loop
	while(true){
		if(NRF_LOG_PROCESS() == false){
			app_sched_execute();
			sensor_data = sensorDataAddress(&sensor_data_length);
			if(sensor_data != NULL){
				if(isLoggerEnabled()){
					NRF_LOG_INFO("Printing Sensor Data(After Color Sampling)...\r\n");
					NRF_LOG_FLUSH();
					for(uint8_t i = 0;i < 5;i++){
						NRF_LOG_INFO("Data[%d] : 0x%2x\r\n",i,sensor_data[i]);
						NRF_LOG_FLUSH();
					}
				}
			}
			__WFI();
		}
		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
