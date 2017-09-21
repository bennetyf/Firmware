/** File Name : event_handlers.h
	*
	* @Brief	This file declares all the event handlers used in the main.c program.
	*
	* @Auther 	Feng Yuan
	* @Time 		20/09/2017
	* @Version	1.0
	*
	* @Req			This module requires the following modules:
	*	@Req			- General Data Definitions (Included in "data_general_config.h")
	*	@Req			-	Initialization Functions (Included in "initialization.h")
	* @Req			- Application Functions    (Included in "operations.h")
	*
*/
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "event_handlers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Variable Handle of the current connection */
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function Implementations */

/** @Func Function for handling Peer Manager events */
void pm_evt_handler(pm_evt_t const * p_evt)
{
    ret_code_t err_code;

    switch (p_evt->evt_id)
    {
        case PM_EVT_BONDED_PEER_CONNECTED:
        {
            NRF_LOG_INFO("Connected to a previously bonded device.\r\n");
        } break;

        case PM_EVT_CONN_SEC_SUCCEEDED:
        {
            NRF_LOG_INFO("Connection secured. Role: %d. conn_handle: %d, Procedure: %d\r\n",
                         ble_conn_state_role(p_evt->conn_handle),
                         p_evt->conn_handle,
                         p_evt->params.conn_sec_succeeded.procedure);
        } break;

        case PM_EVT_CONN_SEC_FAILED:
        {
            /* Often, when securing fails, it shouldn't be restarted, for security reasons.
             * Other times, it can be restarted directly.
             * Sometimes it can be restarted, but only after changing some Security Parameters.
             * Sometimes, it cannot be restarted until the link is disconnected and reconnected.
             * Sometimes it is impossible, to secure the link, or the peer device does not support it.
             * How to handle this error is highly application dependent. */
        } break;

        case PM_EVT_CONN_SEC_CONFIG_REQ:
        {
            // Reject pairing request from an already bonded peer.
            pm_conn_sec_config_t conn_sec_config = {.allow_repairing = false};
            pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
        } break;

        case PM_EVT_STORAGE_FULL:
        {
            // Run garbage collection on the flash.
            err_code = fds_gc();
            if (err_code == FDS_ERR_BUSY || err_code == FDS_ERR_NO_SPACE_IN_QUEUES)
            {
                // Retry.
            }
            else
            {
                APP_ERROR_CHECK(err_code);
            }
        } break;

        case PM_EVT_PEERS_DELETE_SUCCEEDED:
        {
            startAdvertising();
        } break;

        case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
        {
            // The local database has likely changed, send service changed indications.
            pm_local_database_has_changed();
        } break;

        case PM_EVT_PEER_DATA_UPDATE_FAILED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peer_data_update_failed.error);
        } break;

        case PM_EVT_PEER_DELETE_FAILED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peer_delete_failed.error);
        } break;

        case PM_EVT_PEERS_DELETE_FAILED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peers_delete_failed_evt.error);
        } break;

        case PM_EVT_ERROR_UNEXPECTED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.error_unexpected.error);
        } break;

        case PM_EVT_CONN_SEC_START:
        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
        case PM_EVT_PEER_DELETE_SUCCEEDED:
        case PM_EVT_LOCAL_DB_CACHE_APPLIED:
        case PM_EVT_SERVICE_CHANGED_IND_SENT:
        case PM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
        default:
            break;
    }
}

/** @Func Function for handling the Connection Parameters Module */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

/** @Func Function for handling a Connection Parameters error */
void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/** @Func Function for handling advertising events */
void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
//    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
					if(isLoggerEnabled()){
						NRF_LOG_INFO("Fast Advertising!!\r\n");
						NRF_LOG_FLUSH();
					}
//            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
//            APP_ERROR_CHECK(err_code);
          break;

        case BLE_ADV_EVT_IDLE:
					enterSleepMode();
          break;

        default:
          break;
    }
}


/**	@Func Function for handling the Application's BLE Stack events */
void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code = NRF_SUCCESS;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected.\r\n");
//            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
//            APP_ERROR_CHECK(err_code);
            break; // BLE_GAP_EVT_DISCONNECTED

        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected.\r\n");
//            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
//            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break; // BLE_GAP_EVT_CONNECTED

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.\r\n");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTC_EVT_TIMEOUT

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.\r\n");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_TIMEOUT

        case BLE_EVT_USER_MEM_REQUEST:
            err_code = sd_ble_user_mem_reply(p_ble_evt->evt.gattc_evt.conn_handle, NULL);
            APP_ERROR_CHECK(err_code);
            break; // BLE_EVT_USER_MEM_REQUEST

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        {
            ble_gatts_evt_rw_authorize_request_t  req;
            ble_gatts_rw_authorize_reply_params_t auth_reply;

            req = p_ble_evt->evt.gatts_evt.params.authorize_request;

            if (req.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
            {
                if ((req.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ)     ||
                    (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) ||
                    (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
                {
                    if (req.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                    }
                    else
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
                    }
                    auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
                    err_code = sd_ble_gatts_rw_authorize_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                               &auth_reply);
                    APP_ERROR_CHECK(err_code);
                }
            }
        } break; // BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST

#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
#endif

        default:
            // No implementation needed.
            break;
    }
}

/** @Func Function for dispatching a BLE stack event to all modules with a BLE stack event handler */
void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    /** The Connection state module has to be fed BLE events in order to function correctly
     * Remember to call ble_conn_state_on_ble_evt before calling any ble_conns_state_* functions. */
    ble_conn_state_on_ble_evt(p_ble_evt);
    pm_on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
    btnBleEventHandler(p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
    /*YOUR_JOB add calls to _on_ble_evt functions from each service your application is using
       ble_xxs_on_ble_evt(&m_xxs, p_ble_evt);
       ble_yys_on_ble_evt(&m_yys, p_ble_evt);
     */
}

/** @Func Function for dispatching a system event to interested modules */
void sys_evt_dispatch(uint32_t sys_evt)
{
    // Dispatch the system event to the fstorage module, where it will be
    // dispatched to the Flash Data Storage (FDS) module.
    fs_sys_event_handler(sys_evt);

    // Dispatch to the Advertising module last, since it will check if there are any
    // pending flash operations in fstorage. Let fstorage process system events first,
    // so that it can report correctly to the Advertising module.
    ble_advertising_on_sys_evt(sys_evt);
}

/** @Func Function for handling events from the BSP module */
void board_event_handler(board_event_t event)
{
	switch (event){
		case BOARD_TEST_EVENT_0:
			boardLedEffect(LED_EFFECT_FIRST);
			NRF_LOG_INFO("BOARD_TEST_EVENT_0!\r\n");
			NRF_LOG_FLUSH();
			break;
		case BOARD_TEST_EVENT_1:
			boardLedEffect(LED_EFFECT_SECOND);
			NRF_LOG_INFO("BOARD_TEST_EVENT_1!\r\n");
			NRF_LOG_FLUSH();
			break;
		case BOARD_TEST_EVENT_2:
			boardLedEffect(LED_EFFECT_RED_ON);
			NRF_LOG_INFO("BOARD_TEST_EVENT_2!\r\n");
			NRF_LOG_FLUSH();
			break;
		case BOARD_TEST_EVENT_3:
			boardLedEffect(LED_EFFECT_GREEN_ON);
			NRF_LOG_INFO("BOARD_TEST_EVENT_3!\r\n");
			NRF_LOG_FLUSH();
			break;
		case BOARD_TEST_EVENT_4:
			boardLedEffect(LED_EFFECT_BLUE_ON);
			NRF_LOG_INFO("BOARD_TEST_EVENT_4!\r\n");
			NRF_LOG_FLUSH();
			break;
		default:
			break;
	}

//	uint32_t err_code;
//  switch (event){
//		case BOARD_EVENT_SLEEP:
//			enterSleepMode();
//      break; // BSP_EVENT_SLEEP
//    case BOARD_EVENT_DISCONNECT:
//      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
//      if(err_code != NRF_ERROR_INVALID_STATE){
//				APP_ERROR_CHECK(err_code);
//      }
//      break; // BSP_EVENT_DISCONNECT
//    case BOARD_EVENT_WHITELIST_OFF:
//			if(m_conn_handle == BLE_CONN_HANDLE_INVALID){
//				err_code = ble_advertising_restart_without_whitelist();
//        if(err_code != NRF_ERROR_INVALID_STATE){
//					APP_ERROR_CHECK(err_code);
//        }
//      }
//			break; // BSP_EVENT_KEY_0
//    default:
//			break;
//	}
}

/* Initialization of the FDS event handler(This can be defined in the "module_scheduler.c" file that aims to manage all the modules */
void fds_event_handler(fds_evt_t const * const p_fds_evt){
	switch (p_fds_evt->id){
		case FDS_EVT_INIT:  	//!< Event for @ref fds_init.
		if (p_fds_evt->result == FDS_SUCCESS){
			fdsSetInitFlag(1);
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: HANDLER INIT EVENT!\r\n");
			}
		}else{
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: HANDLER INIT FAILED!\r\n");
			}
		}
		break;
		case FDS_EVT_WRITE: 		//!< Event for @ref fds_record_write and @ref fds_record_write_reserved.
		if (p_fds_evt->result == FDS_SUCCESS){
			fdsSetWriteFlag(1);
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: WRITE EVENT!\r\n");
			}
		}
		break;
		case FDS_EVT_UPDATE: 		//!< Event for @ref fds_record_update.
		if (p_fds_evt->result == FDS_SUCCESS){
			fdsSetUpdateFlag(1);
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: UPDATE EVENT!\r\n");
			}
		}
		break;
		case FDS_EVT_DEL_RECORD: //!< Event for @ref fds_record_delete.
		if (p_fds_evt->result == FDS_SUCCESS){
			fdsSetDelFlag(1);
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: RECORD DELETED!\r\n");
			}
		}
		break;
		case FDS_EVT_DEL_FILE:   //!< Event for @ref fds_file_delete.
		if (p_fds_evt->result == FDS_SUCCESS){
			fdsSetDelFlag(1);
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: FILE DELETED!\r\n");
			}
		}
		break;
		case FDS_EVT_GC:         //!< Event for @ref fds_gc.
		if (p_fds_evt->result == FDS_SUCCESS){
			fdsSetGcFlag(1);
			if(fdsGetFlags().fds_log_flag) {
				NRF_LOG_INFO("FDS_EVENT: GARBAGE COLLECTION!\r\n");
			}
		}
		default:
		break;
	}
}
