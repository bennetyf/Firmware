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
	boardLedEffect(LED_EFFECT_FIRST);
	if(isLoggerEnabled()){
		NRF_LOG_INFO("Application Started...\r\n");
		NRF_LOG_FLUSH();
	}
  APP_ERROR_CHECK(ble_advertising_start(BLE_ADV_MODE_FAST)); 	// Start the BLE Advertising
	// Main Loop
	while(true){
		if(NRF_LOG_PROCESS() == false){powerManage();}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
