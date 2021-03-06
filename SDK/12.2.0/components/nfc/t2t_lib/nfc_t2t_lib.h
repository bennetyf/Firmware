/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#ifndef NFC_T2T_LIB_H__
#define NFC_T2T_LIB_H__

/** @file
 *
 * @addtogroup nfc_api
 *
 * @defgroup nfc_t2t NFC Type 2 Tag
 * @ingroup nfc_api
 * @brief Implementation of NFC Type 2 Tag.
 *
 * @defgroup nfc_t2t_lib NFC tag 2 type emulation library
 * @{
 * @ingroup nfc_t2t
 * @brief The T2T emulation library interface.
 */

#include <string.h>
#include <sdk_errors.h>

#define NFC_T2T_SIZEOF_INTERNAL_BYTES 10    ///< T2T internal byte size.
#define NFC_T2T_MAX_PAYLOAD_SIZE      988   ///< Maximum NDEF message size.
#define NFC_T2T_MAX_PAYLOAD_SIZE_RAW  1008  ///< No NDEF-TLV and no implicit lock bytes at the end.

/** @brief Events passed to the callback function. */
typedef enum
{
    NFC_T2T_EVENT_NONE,
    ///< Not used.

    NFC_T2T_EVENT_FIELD_ON,
    ///< NFC tag has detected external NFC field and was selected by an NFC polling device.

    NFC_T2T_EVENT_FIELD_OFF,
    ///< External NFC field has been removed.

    NFC_T2T_EVENT_DATA_READ,
    ///< NFC polling device has read all tag data.
    /**<
     * Repeated reading in the same session i.e. before @ref NFC_T2T_EVENT_FIELD_OFF event,
     * will not trigger another @ref NFC_T2T_EVENT_DATA_READ event.
     */

    NFC_T2T_EVENT_STOPPED
    ///< Reference to the application NFC callback has been released using @ref nfc_t2t_done.
} nfc_t2t_event_t;

typedef enum
{
    NFC_T2T_PARAM_TESTING       ///< Used for unit tests.
} nfc_t2t_param_id_t;

/** @brief Callback to pass events from NFC T2T Library to application.
 *
 * @param[in] p_context    Application context for callback execution.
 * @param[in] event        The event that occurred.
 * @param[in] p_data       Data to send to the application (event specific).
 * @param[in] data_length  Length of the data.
 */
typedef void (*nfc_t2t_callback_t)(void          * p_context,
                                   nfc_t2t_event_t event,
                                   const uint8_t * p_data,
                                   size_t          data_length);

/** @brief Function for registering the application callback for event signaling.
 *
 * The callback will be called by NFC T2T Library to notify the application of relevant
 * events. It will be called from the HAL_NFC callback context.
 *
 * @param[in] callback     Function pointer to the callback.
 * @param[in] p_context    Pointer to a memory area used by the callback for execution (optional).
 *
 * @retval NRF_SUCCESS If the application callback was registered successfully. If one 
 *                     of the arguments was invalid, an error code is returned.
 */
ret_code_t nfc_t2t_setup(nfc_t2t_callback_t callback, void * p_context);

/** @brief Function for setting an NFC parameter.
 *
 * @note Not implemented. For future use.
 *
 * This function allows to set any parameter defined as available by HAL_NFC.
 *
 * @param[in] id           ID of the parameter to set.
 * @param[in] p_data       Pointer to a buffer containing the data to set.
 * @param[in] data_length  Size of the buffer containing the data to set.
 *
 * @retval NRF_SUCCESS If the parameter was set successfully. If one of the arguments
 *                     was invalid (for example, a wrong data length), an error code
 *                     is returned.
 */
ret_code_t nfc_t2t_parameter_set(nfc_t2t_param_id_t id, void * p_data, size_t data_length);

/** @brief Function for querying an NFC parameter value.
 *
 * @note Not implemented. For future use.
 *
 * The queried value will be placed into the passed data buffer. If the buffer
 * is too small, p_max_data_length will contain the required buffer size. If the
 * buffer is big enough, p_max_data_length will contain the actual size of the
 * data.
 *
 * @param[in]      id                ID of the parameter to query.
 * @param[in]      p_data            Pointer to a buffer receiving the queried data.
 * @param[in, out] p_max_data_length Size of the buffer, receives actual size of queried data.
 *
 * @retval NRF_SUCCESS If the parameter was received successfully. If one of the arguments
 *                     was invalid (for example, the buffer was too small), an error code
 *                     is returned.
 */
ret_code_t nfc_t2t_parameter_get(nfc_t2t_param_id_t id, void * p_data, size_t * p_max_data_length);

/** @brief Function for registering the payload to send on reception of a READ request.
 *
 * The payload is considered to only contain the NDEF message to deliver to a
 * reader. The required NDEF TLV will be created implicitly by NFC T2T Library.
 *
 * The pointer to the payload must stay valid for the duration of the library
 * execution, or until it is explicitly released.
 *
 * If the pointer is not NULL, but the length is zero, the paypload is
 * considered to be an empty NDEF message.
 *
 * If a new payload is registered, the previously registered one is considered
 * released.
 *
 * Passing a NULL pointer releases the current payload without registering a
 * new one.
 *
 * If an invalid size is given (too big), the function returns with an error
 * and the currently registered payload is left unchanged.
 *
 * @param[in] p_payload        Pointer to the memory area containing the payload to send.
 * @param[in] payload_length   Size of the payload in bytes.
 *
 * @retval NRF_SUCCESS If the operation was successful. If one 
 *                     of the arguments was invalid, an error code is returned.
 */
ret_code_t nfc_t2t_payload_set(const uint8_t * p_payload, size_t payload_length);

/** @brief Function for registering the raw payload to send on reception of a READ request.
 *
 * The payload will be delivered directly as-is to the reader, without
 * implicitly adding an NDEF TLV container. This can be used if the
 * application wants to define the TLVs itself, for example, to provide a different
 * memory layout.
 *
 * The pointer to the payload must stay valid for the duration of the library
 * execution, or until it is explicitly released.
 *
 * If a new payload is registered, the previously registered one is considered
 * released.
 *
 * Passing a NULL pointer releases the current payload, without registering a
 * new one.
 *
 * If an invalid size is given (too big), the function returns with an error
 * and the currently registered payload is left unchanged.
 *
 * @param[in] p_payload        Pointer to the memory area containing the payload to send.
 * @param[in] payload_length   Size of the payload in bytes.
 *
 * @retval NRF_SUCCESS If the operation was successful. If one 
 *                     of the arguments was invalid, an error code is returned.
 */
ret_code_t nfc_t2t_payload_raw_set(const uint8_t * p_payload, size_t payload_length);

/** @brief Function for registering the sequence of internal bytes.
 *
 * This refers to the first 10 bytes of the tag memory. The library will set
 * a sensible default for these bytes. The application can use this function
 * to override the default.
 *
 * Passing a NULL pointer reverts back to the default sequence.
 * The data will be copied by NFC T2T Library, so the memory does not have to remain valid
 * after the function returns.
 *
 * @note When modifying the internal bytes, remember that they must be consistent
 *       with the NFC hardware register settings (see @ref nfc_t2t_format_internal).
 *
 * @param[in] p_data         Pointer to the memory area containing the data.
 * @param[in] data_length    Size of the data in bytes.
 *
 * @retval NRF_SUCCESS If the operation was successful. If the data was not NULL and the
 *                     data length was not 10, an error code is returned.
 */
ret_code_t nfc_t2t_internal_set(const uint8_t * p_data, size_t data_length);

/** @brief Function for activating the NFC frontend.
 *
 * You must call this function so that events are posted to the application
 * callback.
 *
 * @retval NRF_SUCCESS If the NFC frontend was activated successfully. If the lower layer
 *                     could not be started, an error code is returned.
 */
ret_code_t nfc_t2t_emulation_start(void);

/** @brief Function for deactivating the NFC frontend.
 *
 * After calling this function, no more events will be posted to the
 * application callback.
 *
 * @retval NRF_SUCCESS If the NFC frontend was deactivated successfully.  If the lower layer
 *                     could not be stopped, an error code is returned.
 */
ret_code_t nfc_t2t_emulation_stop(void);

/** @brief Function for releasing the reference to the application callback.
 *
 * After calling this function, the passed callback pointer is no longer
 * considered valid.
 *
 * @retval NRF_SUCCESS This function always succeeds.
 */
ret_code_t nfc_t2t_done(void);

/** @} */
#endif // NFC_T2T_LIB_H__
