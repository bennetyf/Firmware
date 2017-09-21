/** File Name : initialization.c
	*	
	*	@Brief This file declares all the functions for initialization in the main.c program.
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
	*	@Note		All the initialization functions for modules are implemented as "static", meaning internal functions.
	*
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "initialization.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Variable Universally unique service identifiers */
static ble_uuid_t m_adv_uuids[] = {{BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Initialization Function Implementations */

/*===========================================================================================================================*/

/** @Func Function for the Timer initialization */
static void timerInit(void)
{
	// Initialize timer module.
  APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
}

/*===========================================================================================================================*/

/** @Func Function for the GAP initialization */
static void gapParamsInit(void)
{
	ble_gap_conn_params_t   gap_conn_params;
  ble_gap_conn_sec_mode_t sec_mode;

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	APP_ERROR_CHECK(sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)DEVICE_NAME, strlen(DEVICE_NAME)));

  /* YOUR_JOB: Use an appearance value matching the application's use case.
     err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_);
		 APP_ERROR_CHECK(err_code); */

  memset(&gap_conn_params, 0, sizeof(gap_conn_params));

  gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
  gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
  gap_conn_params.slave_latency     = SLAVE_LATENCY;
  gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

  APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&gap_conn_params));
}

/*===========================================================================================================================*/

/** @Func Function for initializing the Connection Parameters module */
static void connParamsInit(void)
{
  ble_conn_params_init_t cp_init;
	
  memset(&cp_init, 0, sizeof(cp_init));
	
  cp_init.p_conn_params                  = NULL;
  cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
  cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
  cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
  cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
  cp_init.disconnect_on_fail             = false;
  cp_init.evt_handler                    = on_conn_params_evt;
  cp_init.error_handler                  = conn_params_error_handler;

  APP_ERROR_CHECK(ble_conn_params_init(&cp_init));
}

/*===========================================================================================================================*/

/** @Func Function for initializing the BLE stack */
static void bleStackInit(void)
{
  ble_enable_params_t ble_enable_params;
	
	// Setup the Low Frequency Clock
  nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
	
	// Initialize the SoftDevice handler module.
  SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

  APP_ERROR_CHECK(softdevice_enable_get_default_config(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT, &ble_enable_params));
	
  // Check the ram settings against the used number of links
  CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);

  // Enable BLE stack
#if (NRF_SD_BLE_API_VERSION == 3)
	ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif
	
	APP_ERROR_CHECK(softdevice_enable(&ble_enable_params));

	// Register with the SoftDevice handler module for BLE events.
  APP_ERROR_CHECK(softdevice_ble_evt_handler_set(ble_evt_dispatch));

	// Register with the SoftDevice handler module for BLE events.
	APP_ERROR_CHECK(softdevice_sys_evt_handler_set(sys_evt_dispatch));
}

/*===========================================================================================================================*/

/** @Func Function for the Peer Manager initialization */
static void peerManagerInit(bool erase_bonds)
{
	ble_gap_sec_params_t 	sec_param;

  APP_ERROR_CHECK(pm_init());
	
  if(erase_bonds){
		APP_ERROR_CHECK(pm_peers_delete());
  }
	
  memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

  // Security parameters to be used for all security procedures.
  sec_param.bond           = SEC_PARAM_BOND;
  sec_param.mitm           = SEC_PARAM_MITM;
  sec_param.lesc           = SEC_PARAM_LESC;
  sec_param.keypress       = SEC_PARAM_KEYPRESS;
  sec_param.io_caps        = SEC_PARAM_IO_CAPABILITIES;
  sec_param.oob            = SEC_PARAM_OOB;
  sec_param.min_key_size   = SEC_PARAM_MIN_KEY_SIZE;
  sec_param.max_key_size   = SEC_PARAM_MAX_KEY_SIZE;
  sec_param.kdist_own.enc  = 1;
  sec_param.kdist_own.id   = 1;
  sec_param.kdist_peer.enc = 1;
  sec_param.kdist_peer.id  = 1;

  APP_ERROR_CHECK(pm_sec_params_set(&sec_param));
  APP_ERROR_CHECK(pm_register(pm_evt_handler));
}

/*===========================================================================================================================*/

/** @Func Function for initializing the Advertising functionality */
static void advInit(void)
{
  ble_advdata_t          advdata;
  ble_adv_modes_config_t options;

  // Build advertising data struct to pass into @ref ble_advertising_init.
  memset(&advdata, 0, sizeof(advdata));

  advdata.name_type               = BLE_ADVDATA_FULL_NAME;
  advdata.include_appearance      = true;
  advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
  advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
  advdata.uuids_complete.p_uuids  = m_adv_uuids;

  memset(&options, 0, sizeof(options));
  options.ble_adv_fast_enabled  = true;
  options.ble_adv_fast_interval = APP_ADV_INTERVAL;
  options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

  APP_ERROR_CHECK(ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL));
}

/*===========================================================================================================================*/

/** @Func Function for initializing buttons and leds */
static void boardInit(bool * p_erase_bonds)
{
	// board_event_t startup_event;
  uint32_t err_code = boardConfig(BOARD_INIT_LEDS | BOARD_INIT_BUTTONS, APP_TIMER_TICKS(1, APP_TIMER_PRESCALER), board_event_handler);
  APP_ERROR_CHECK(err_code);
		
	/*
    err_code = btnBleInit(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    *p_erase_bonds = (startup_event == BOARD_EVENT_CLEAR_BONDING_DATA);
	*/
	
	// Fill in Each LED Effect Data Sequence
	ledEffectFillSeq(LED_EFFECT_FIRST);
	ledEffectFillSeq(LED_EFFECT_SECOND);
	
	/* Assign Some Events Here */
	boardButtonEventAssign(BOARD_BUTTON_0, BOARD_BUTTON_ACTION_PUSH, BOARD_TEST_EVENT_2);
	boardButtonEventAssign(BOARD_BUTTON_0, BOARD_BUTTON_ACTION_RELEASE, BOARD_TEST_EVENT_3);
	boardButtonEventAssign(BOARD_BUTTON_0, BOARD_BUTTON_ACTION_LONG_PUSH, BOARD_TEST_EVENT_0);
	boardButtonEventAssign(BOARD_BUTTON_0, BOARD_BUTTON_ACTION_RELEASE_AFTER_LONG_PUSH, BOARD_TEST_EVENT_1);
}

/*===========================================================================================================================*/

/** @Func Initialize All the Pins Including Correct I/O Direction Configurations */
static void pinInit(void)
{
	nrf_gpio_cfg_output(LED_RED);
	nrf_gpio_cfg_output(LED_GREEN);
	nrf_gpio_cfg_output(LED_BLUE);

	nrf_gpio_cfg_input(BOARD_BUTTON_0, NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_output(VBAT_ADC_EN);
	nrf_gpio_cfg_input(VBAT_ADC, NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_input(USB_PRESENT_INT, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(CHARGE_FULL_INT, NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_input(INT_OPEN, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(INT_CLOSED, NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_output(TX_PIN_NUMBER);
	
	nrf_gpio_cfg_output(I2C_SCL);
	nrf_gpio_cfg_output(I2C_SDA);
	
	nrf_gpio_cfg_output(ENTER_SHIP_MODE);
	nrf_gpio_pin_write(ENTER_SHIP_MODE,0);
}

/*===========================================================================================================================*/

/** @Func Function for initializing services that will be used by the application */
static void serviceInit(void)
{
	/*Your Code Here*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Initialize All the Modules */
void moduleInit(void)
{
	bool	erase_bonds;
	
	// Initialize the Logger
	if(isLoggerEnabled()){
		APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
		NRF_LOG_INFO("Initialization Starting...\r\n");
		NRF_LOG_FLUSH();
	}
	
	// Initialize all the GPIO pins
	pinInit();
	
	// Initialize the Timer
	timerInit();
	
	// Initialize the Board Resources
  boardInit(&erase_bonds);
	
	// Initialize BLE Stack (Including the Low Frequency Clock)
	bleStackInit();
	
	// Initialize the Peer Manager
	peerManagerInit(erase_bonds);
	
	// Initialize GAP
	gapParamsInit();
	
	// Initialize Advertising
  advInit();
	
	// Initialize Services
  serviceInit();
	
	// Initialize Connection Parameters
	connParamsInit();
	
	if(isLoggerEnabled()){
		NRF_LOG_INFO("Initialization Completed...\r\n");
		NRF_LOG_FLUSH();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
