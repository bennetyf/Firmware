/** Library name : "drv_storage.h"
	* @Brief This is a library for flash munipulation using FDS
	*
	*	@Author: Feng Yuan
	*	@Time: 17/07/2017 		(V2.0)
	*	@Revision:19/07/2017	(V3.0)
	*	@Revision:31/07/2017 (V4.0)
	*
	*	This module provides a basic interface to the FDS system.
	*************************************************************************************************************************************************
	* Four new data types are declared in this module,including fds_rec_t (Description of the record data)
	*	fds_state_t (Record metadata),fds_flags_t (FDS flags storage) and fds_fprt_t (Async function pointer).
	*************************************************************************************************************************************************
	*	Main FDS operation functions include : 
	*	fdsInit(FDS system initialization),fdsRecWrite(Write a record)
	*	fdsRecRead(read a record),fdsRecUpdate(update an existing record),fdsRecDelete(delete an existing record),fdsGC(perform garbage collection).
	*	Utility functions :
	*	fdsResetVars(reset all the internal variables to zero values),fdsRecSetup(fill in the metadata of a record)
	*	fdsRecFind(look up an FDS record),fdsSetAsynFun(change the async function).
	*	Accessors :
	*	fdsGetState(return the internal FDS state),fdsGetFlags(return an object of type fds_flags_t)
	*	fdsSetInitFlag(change the initialization flag),fdsSetWriteFlag(change the writing flag),fdsSetUpdateFlag(change the update flag)
	*	fdsSetDelFlag(change the deletion flag), fdsSetGcFlag(change the garbage collection flag), fdsSetLogFlag(change the logging flag)
	*************************************************************************************************************************************************
	*	This module defines two internal variables, noted by "static", and one default async function. These are defined in the "drv_storage.c" file.
	*	The two variables include fds_state and fds_flags which store the fds internal states and all the fds internal flags respectively.
	*	The default async function is fds_wait which put the MCU into sleeping mode and wait until the flash operation is done.
	*************************************************************************************************************************************************
	*	@Type	fds_rec_t					(Record Data Description Type)
	*	@Type	fds_state_t				(Record State Type)
	*	@Type fds_flags_t				(FDS Operation Flags Type)
	*	@Type fds_fptr_t				(Function Pointer for Waiting FDS Operation to Complete)
	*
	*	@Func	fdsResetVars			(Zero-initialize the Internal Variables)
	*	@Func fdsRecConfig			(Setup an FDS Record)
	*	@Func fdsInit						(Initialization of FDS System)
	* @Func fdsRecWrite				(Write a Record)
	* @Func fdsRecFind				(Locate a Record)
	*	@Func fdsRecRead				(Read a Record)
	* @Func	fdsRecUpdate			(Update a Record)
	* @Func fdsRecDelete			(Delete a Record)
	*	@Func fdsGC							(Garbage Collection)
	*	@Func fdsSetAsynFun			(Set Async Operation)
	*	@Func fdsGetState				(Get FDS Status)
	* @Func fdsGetFlags				(Get FDS Flags)
	*
	*	@Func fdsSetInitFlag		(Set FDS Flags)
	*	@Func	fdsSetWriteFlag		(Set FDS Flags)
	*	@Func fdsSetUpdateFlag	(Set FDS Flags)
	*	@Func fdsSetDelFlag			(Set FDS Flags)
	* @Func fdsSetGcFlag			(Set FDS Flags)
	* @Func fdsSetLogFlag			(Set FDS Flags)
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRV_STORAGE_H__
#define __DRV_STORAGE_H__
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Include Essential Headers */
#include "app_error.h"
#include "fds.h"

/* Include NRF Libraries */
#include "nrf_log.h"
#include "nrf_delay.h"
#include "nrf_log_ctrl.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Declare the Basic Data Types */
	
/** @Type The type to describe the record data	*/
typedef struct
{
	fds_record_chunk_t  				rec_chunk;						//Data chunk in the record
	fds_record_t        				rec_info;							//The record
}fds_rec_t;

/** @Type The type to store the record states (Metadata) */
typedef struct
{
	fds_record_desc_t   				rec_desc;			//Record descriptor
	fds_find_token_t    				ftok;					//Record Token. Important, make sure you zero init the ftok token
	ret_code_t 									ret;        	//FDS Event Code
	fds_rec_t										record;				//An FDS record Instance
	void (*async_fun)(uint8_t*);							//A function pointer pointed to the function to be called while waiting for FDS operations to complete
}fds_state_t;

/** @Type The type to store the FDS flags */
typedef struct
{
	//FDS EVENT FLAGS
	uint8_t 										fds_init_flag; 	//FDS intialization flag
	uint8_t 										fds_write_flag;	//FDS writing flag 	
	uint8_t 										fds_update_flag;//FDS updating flag
	uint8_t 										fds_del_flag;		//FDS deletion flag
	uint8_t 										fds_gc_flag;		//FDS garbage collection flag
	//PRINT FDS LOGS
	uint8_t 										fds_log_flag;		//FDS logging flag
}fds_flags_t;

/** @Type The function pointer type to store the async function */
typedef void (*fds_fptr_t)(uint8_t*);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Zero-initialize the Internal Variables
	*
	* @Brief 		This function is used to reset all the internal variables to zero or NULL.
	* @Details	The variables are defined as static in the file "drv_storage.c", including fds_state and fds_flags
	* 					All FDS flags are set to zeros. The async function is set to be waiting in low power mode by default.
	* 					The async function is set to be the default low power waiting function.
	*
*/
void fdsResetVars (void);

/** @Func Setup an FDS record
	* 
	* @Brief 		This function is used to set up the default meta-data of an record.
	* @Details 	This function only needs file ID, record key, the word array of the data and the length of the word array.
	* 					Before calling any FDS operation defined in this library, a record must be set up using this function.
	* 					This fucntion sets all the essential meta-data of the record and returns true (if setting up the record is successful) or false (if failed).
	*
	*	@Para			file_id 		[uint16_t]	: File ID
	*	@Para			rec_key 		[uint16_t]	: Record Key
	*	@Para			data_array	[uint32_t*]	:	Address of the data array to be stored.(When locating or reading a record, set it to NULL)
	*	@Para			data_length [uint16_t]	: Array length of the data array.(When locating a record, set it to 0)
	*
	*	@Return		false	: Record set up failed (invalid input parameters)
	*	@Return		true	: Record set up succeeded
	*
*/
bool fdsRecConfig (const uint16_t file_id, const uint16_t rec_key, const uint32_t* const data_array, const uint16_t data_length);

/** @Func Initialization of FDS
	*
	* @Brief 	This function initializes the FDS system and must be called before using the FDS system.
	* 				This function zero-initializes all the internal variables and registers the FDS event handler.
	* 				The async function is set to be the default low power waiting function.
	*
	* @Return Propogate internal errors
	*
*/
ret_code_t fdsInit (void);
	
/** @Func Write a Record
	*
	* @Brief 	This function writes a record into the flash using the FDS system.
	* 				Before calling this function, fdsRecSetup must be called and FDS system initialized.
	*
	*	@Return Propogate internal errors
	*
*/
ret_code_t fdsRecWrite(void);

/** @Func Locate a Record
	*
	* @Brief 	This function looks for a record according to its file ID and record key.
	* 				Before calling this function, fdsRecSetup must be called and FDS system initialized.
	* 				Note: This function only locates the record it first finds in the flash. This function DOES NOT support multiple records with the same file IDs and record keys.
	*
	*	@Return	Propogate internal errors
	*
*/
ret_code_t fdsRecFind(void);
	
/** @Func Read a Record
	*
	* @Brief 	This function reads an existing record in the flash into the destination word array.
	* 				Before calling this function, fdsRecSetup must be called and FDS system initialized.
	* 				Note: This function only locates the record it first finds in the flash. This function DOES NOT support multiple records with the same file IDs and record keys.
	*
	* @Para 	data_read [uint32_t*] : the array address to store the data read from the FDS system
	*
	*	@Return	Propogate internal errors
	*
*/
ret_code_t fdsRecRead(uint32_t* data_read);

/** @Func Update a Record
	*
	* @Brief 	This function updates an existing record in the flash.
	* 				Before calling this function, fdsRecSetup must be called and FDS system initialized.
	* 				Note: This function only locates the record it first finds in the flash. This function DOES NOT support multiple records with the same file IDs and record keys.
	*
	*	@Return Propogate internal errors
	*
*/
ret_code_t fdsRecUpdate(void);

/** @Func Delete a Record
	*
	* @Brief	This function deletes an existing record in the flash.
	* 				Before calling this function, fdsRecSetup must be called and FDS system initialized.
	* 				Note: This function only locates the record it first finds in the flash. This function DOES NOT support multiple records with the same file IDs and record keys.
	*
	*	@Return Propogate internal errors
	*
*/
ret_code_t fdsRecDelete(void);

/** @Func Run Garbage Collection
	*
	* @Brief This function performs garbage collection operation of the flash.
	*
	*	@Return Propogate internal errors
	*
*/
ret_code_t fdsGC(void);

/** @Func Set Async Operation
	*
	* @Brief 	This function sets the internal function pointer to the desired async function.
	* 				Note that the type fds_fptr_t needs an input parameter of type uint8_t.
	*
	*	@Para		func [fds_fptr_t]	: the input function pointer for the waiting operation
	*
*/
void fdsSetAsynFun(fds_fptr_t func);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Interface for Accessing Internal States of this Module */

/** @Func Get FDS Status
	*
	*	@Brief 	This function returns the internal return state value
	*
	* @Return	Propogate internal errors
	*
*/
ret_code_t fdsGetState(void);

/** @Func Get FDS Flags
	*
	* @Brief This function returns the internal flags
	*
*/
fds_flags_t fdsGetFlags(void);

/** @Func Set FDS Flags
	*
	* @Brief This set of functions are used to set the internal flags
*/
void fdsSetInitFlag(uint8_t flag);
void fdsSetWriteFlag(uint8_t flag);
void fdsSetUpdateFlag(uint8_t flag);
void fdsSetDelFlag(uint8_t flag);
void fdsSetGcFlag(uint8_t flag);
void fdsSetLogFlag(uint8_t flag);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //_FDS_STORAGE_H_
