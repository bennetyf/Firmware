/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup ble_lls Link Loss Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Link Loss Service module.
 *
 * @details This module implements the Link Loss Service with the Alert Level characteristic.
 *          During initialization it adds the Link Loss Service and Alert Level characteristic
 *          to the BLE stack database.
 *
 *          The application must supply an event handler for receiving Link Loss Service
 *          events. Using this handler, the service will notify the application when the
 *          link has been lost, and which Alert Level has been set.
 *
 *          The service also provides a function for letting the application poll the current
 *          value of the Alert Level characteristic.
 *
 * @note The application must propagate BLE stack events to the Link Loss Service
 *       module by calling ble_lls_on_ble_evt() from the @ref softdevice_handler callback.
 *
 * @note Attention!
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile
 *  qualification listings, this section of source code must not be modified.
*/

#ifndef BLE_LLS_H__
#define BLE_LLS_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Link Loss Service event type. */
typedef enum
{
    BLE_LLS_EVT_LINK_LOSS_ALERT                         /**< Alert Level Updated event. */
} ble_lls_evt_type_t;

/**@brief Link Loss Service event. */
typedef struct
{
    ble_lls_evt_type_t evt_type;                        /**< Type of event. */
    union
    {
        uint8_t alert_level;                            /**< New Alert Level value. */
    } params;
} ble_lls_evt_t;

// Forward declaration of the ble_lls_t type.
typedef struct ble_lls_s ble_lls_t;

/**@brief Link Loss Service event handler type. */
typedef void (*ble_lls_evt_handler_t) (ble_lls_t * p_lls, ble_lls_evt_t * p_evt);

/**@brief Link Loss Service init structure. This contains all options and data needed for initialization of the service. */
typedef struct
{
    ble_lls_evt_handler_t     evt_handler;              /**< Event handler to be called for handling events in the Link Loss Service. */
    ble_srv_error_handler_t   error_handler;            /**< Function to be called in case of an error. */
    uint8_t                   initial_alert_level;      /**< Initial value of the Alert Level characteristic. */
    ble_srv_security_mode_t   lls_attr_md;              /**< Initial Security Setting for Link Loss Service Characteristics. */
} ble_lls_init_t;

/**@brief Link Loss Service structure. This contains various status information for the service. */
struct ble_lls_s
{
    ble_lls_evt_handler_t     evt_handler;              /**< Event handler to be called for handling events in the Link Loss Service. */
    ble_srv_error_handler_t   error_handler;            /**< Function to be called in case of an error. */
    uint16_t                  service_handle;           /**< Handle of Link Loss Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t  alert_level_handles;      /**< Handles related to the Alert Level characteristic. */
    uint16_t                  conn_handle;              /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
};

/**@brief Function for initializing the Link Loss Service.
 *
 * @param[out]  p_lls       Link Loss Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_lls_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_lls_init(ble_lls_t * p_lls, const ble_lls_init_t * p_lls_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Link Loss Service.
 *
 * @param[in]   p_lls      Link Loss Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_lls_on_ble_evt(ble_lls_t * p_lls, ble_evt_t * p_ble_evt);

/**@brief Function for getting current value of the Alert Level characteristic.
 *
 * @param[in]   p_lls          Link Loss Service structure.
 * @param[out]  p_alert_level  Current Alert Level value.
 */
uint32_t ble_lls_alert_level_get(ble_lls_t * p_lls, uint8_t * p_alert_level);


#ifdef __cplusplus
}
#endif

#endif // BLE_LLS_H__

/** @} */
