/* 	This is a library for flash munipulation using FDS
 *	Author: Feng Yuan
 *	Time: 17/07/2017 		(V2.0)
 *	Revision:19/07/2017	(V3.0)
 *  Revision:31/07/2017 (V4.0)
*/

/* Include the library header */
#include "drv_storage.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set two internal variables and one default async operation function */

/** @Variable Internal variables definition */
static fds_state_t	fds_state; //Internal variable to store the fds state.
static fds_flags_t	fds_flags; //Internal variable to store all fds flags.

/** @Func Power Management Function (Entering low power mode while waiting for async operations */
static void fds_wait(uint8_t* flag){
	while(*flag == 0) {
		APP_ERROR_CHECK(sd_app_evt_wait());
	}
}

/** @Func Default FDS Event Handler */
static void fds_event_handler_default(fds_evt_t const * const p_fds_evt){
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Initialize All Internal Variables */
void fdsResetVars(void){
	// Set all flags to zero
	fds_flags.fds_init_flag 			= 0;
	fds_flags.fds_write_flag			= 0;
	fds_flags.fds_update_flag			=	0;
	fds_flags.fds_del_flag				=	0;
	fds_flags.fds_gc_flag					=	0;
	fds_flags.fds_log_flag				=	0;
	//Default Async Operation(Enter low power mode and wait till operation is completed)
	fds_state.async_fun		=	fds_wait;
	//Always zero-initialize the token before using it.
	memset(&fds_state.ftok, 0x00, sizeof(fds_find_token_t));
}

/** @Func Setup an FDS Record */
bool fdsRecConfig (const uint16_t file_id, const uint16_t rec_key, const uint32_t* const data_array, const uint16_t data_length){
	//Check file_id and rec_key
	if(file_id > 0xBFFF || rec_key == 0x0000 || rec_key > 0xBFFF){
		NRF_LOG_INFO("FDS_ERROR : <fdsRecSetup> File ID: 0x%4x or RecordKey: 0x%4x Out of Range!\r\n", file_id, rec_key);
		NRF_LOG_FLUSH();
		return false;
	}
	
	//Check the data length
	if(data_length > FDS_VIRTUAL_PAGE_SIZE-5){
		NRF_LOG_INFO("FDS_ERROR : <fdsRecSetup> Data Length is : %d (Record Too Large)!\r\n", data_length);
		NRF_LOG_FLUSH();
		return false;
	}
	
	//Set up data
	fds_state.record.rec_chunk.p_data					=	data_array;
	fds_state.record.rec_chunk.length_words		=	data_length;
	
	//Set up record
	fds_state.record.rec_info.file_id					=	file_id;
	fds_state.record.rec_info.key 						=	rec_key;
	fds_state.record.rec_info.data.p_chunks		=	&fds_state.record.rec_chunk;
	fds_state.record.rec_info.data.num_chunks	=	1;

	return true;
}

/** @Func Initialization of FDS */
ret_code_t fdsInit (fds_evt_handler_t event_handler){
	//	Initializing essential fields
	fdsResetVars();
	
	// Register FDS Event Handler
	if(event_handler != NULL){
		fds_state.ret 		 =	fds_register(event_handler);
	}
	else{
		fds_state.ret 		 =	fds_register(fds_event_handler_default);
	}
	
	// Handle Handler Registration Error
	if (fds_state.ret != FDS_SUCCESS){
		NRF_LOG_ERROR("FDS_ERROR [Error Code:0x%2x] : <fdsInit> FDS Event Handler Registration Failed\r\n", fds_state.ret);
		NRF_LOG_FLUSH();
		APP_ERROR_CHECK(fds_state.ret);
		return fds_state.ret;
	}
	
	// Initialize FDS
	fds_state.ret = fds_init();	
	
	//	Wait till FDS is correctly initialized
	fds_wait(&fds_flags.fds_init_flag);									//Must wait till fds initialization is completed.
	
	//	Handle Initialization Error
	if (fds_state.ret != FDS_SUCCESS){
		NRF_LOG_INFO("FDS_ERROR [Error Code:0x%2x] : <fdsInit> FDS Initialization Failed\r\n", fds_state.ret);
		NRF_LOG_FLUSH();
		APP_ERROR_CHECK(fds_state.ret);
		return fds_state.ret;
	}
	
	// fds_verify_crc_on_writes(1); //Possible CRC checking.
	return fds_state.ret;
}

/** @Func Write a Record */
ret_code_t fdsRecWrite(void){
	// Write into Flash
	fds_flags.fds_write_flag =	0; //Set the Flag
	fds_state.ret						 = fds_record_write(&fds_state.rec_desc, &fds_state.record.rec_info);
	
	// Handle Errors
	if(fds_state.ret != FDS_SUCCESS){
		if(fds_flags.fds_log_flag){//Error happens and print the log if logging is allowed.
			NRF_LOG_INFO("FDS_LOG [Error Code:0x%2x] : <fdsRecWrite> Writing Failed @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.ret, fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
			NRF_LOG_FLUSH();
		}
		return fds_state.ret;
	}
	
	// Async Operation
	fds_state.async_fun(&fds_flags.fds_write_flag);
	
	// Write record result check and Logging
	if(fds_flags.fds_log_flag){
		NRF_LOG_INFO("FDS_LOG: <fdsRecWrite> Writing Succeeded @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
		NRF_LOG_FLUSH();
	}
	
	return fds_state.ret;
}

/** @Func Locate an FDS record */
ret_code_t fdsRecFind(void){
	// 	Zero-initialize the token before usage.
	memset(&(fds_state.ftok), 0x00, sizeof(fds_find_token_t));
	
	//	Search for the record
	fds_state.ret	=	fds_record_find(fds_state.record.rec_info.file_id, fds_state.record.rec_info.key, &fds_state.rec_desc, &fds_state.ftok);
	
	//	Print the logs if logging is allowed
	if(fds_flags.fds_log_flag){
		if(fds_state.ret != FDS_SUCCESS){
			NRF_LOG_INFO("FDS_LOG [Error Code:0x%2x] : <fdsRecFind> Finding Failed @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.ret, fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
			NRF_LOG_FLUSH();
		}
		else{
			NRF_LOG_INFO("FDS_LOG: <fdsRecFind> Finding Succeeded @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n",fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
			NRF_LOG_FLUSH();
		}
	}
	
	return fds_state.ret;
}
	
/** @Func Read a record */
ret_code_t fdsRecRead(uint32_t* data_read){
	// Search all the records according to the file_id and record_key
	if(fdsRecFind() != FDS_SUCCESS){//Search Failed
		return fds_state.ret;
	}
	else{//Search Succeeded
		// Try to open the located record.
		fds_flash_record_t  flash_record;
		fds_state.ret = fds_record_open(&fds_state.rec_desc, &flash_record);
		
		// Handle the errors
		if (fds_state.ret != FDS_SUCCESS){
			NRF_LOG_INFO("FDS_ERROR [Error Code:0x%2x] : <fdsRecRead> Record Open Failed!\r\n", fds_state.ret);
			NRF_LOG_FLUSH();
			return fds_state.ret;
		}
		
		// Access the record through the flash_record structure.
		uint32_t* data_tmp 											= (uint32_t*) flash_record.p_data;
		uint16_t	data_length										= flash_record.p_header->tl.length_words;
		
		// Update the length of words actually read in the global FDS variable.
		fds_state.record.rec_chunk.length_words	=	data_length;
		for (uint16_t i=0; i<data_length; i++){
			data_read[i] = data_tmp[i];									//Write the data in the record (flash) out to memory.
		}
		
		// Close the record when done.
		fds_state.ret = fds_record_close(&fds_state.rec_desc);	
		
		// Handle the errors.
		if (fds_state.ret != FDS_SUCCESS){
			NRF_LOG_INFO("FDS_ERROR [Error Code:0x%2x] : <fdsRecRead> Record Close Failed!\r\n", fds_state.ret);
			NRF_LOG_FLUSH();
			return fds_state.ret;
		}

		//	Print the logs if logging is allowed
		if(fds_flags.fds_log_flag){
			NRF_LOG_INFO("FDS_LOG: <fdsRecRead> Reading Succeeded @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n",fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
			NRF_LOG_FLUSH();
			if(data_length <= 2){// Print all the data read.
				NRF_LOG_INFO("FDS_LOG: <fdsRecRead> Displaying All %d Data.\r\n",data_length);
				for(uint16_t i=0; i<data_length; i++){
					NRF_LOG_INFO(" DATA[%d] --> 0x%8x\r\n",i,data_read[i]);			//Display the flash data.
				}
				NRF_LOG_FLUSH();
			}
			else{ // Print all the data read.
				NRF_LOG_INFO("FDS_LOG: <fdsRecRead> Data Length is %d. Displaying only the First and Last Data.\r\n",data_length);
				NRF_LOG_INFO(" DATA[%d] --> 0x%8x\r\n",0,data_read[0]);														//Display the flash data.
				NRF_LOG_INFO(" DATA[%d] --> 0x%8x\r\n",data_length-1,data_read[data_length-1]);		//Display the flash data.
				NRF_LOG_FLUSH();
			}
		}
		
		return fds_state.ret;
	}
}

/** @Func Update a Record */
ret_code_t fdsRecUpdate(void){
	// Search all the records according to the file_id and record_key
	if(fdsRecFind() != FDS_SUCCESS){// Search failed
		return fds_state.ret;
	}
	else{
		// Update the record
		fds_flags.fds_update_flag = 0;
		fds_state.ret 		 	 			= fds_record_update(&fds_state.rec_desc, &fds_state.record.rec_info);
		
		// Handle errors
		if(fds_state.ret != FDS_SUCCESS){
			if(fds_flags.fds_log_flag){
				NRF_LOG_INFO("FDS_LOG [Error Code:0x%2x] : <fdsRecUpdate> Updating Failed @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.ret, fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
				NRF_LOG_FLUSH();
			}
			return fds_state.ret;
		}
		
		//	Async Operation
		fds_state.async_fun(&fds_flags.fds_update_flag);
		
		// Update logs if logging is allowed.
		if(fds_flags.fds_log_flag){
			NRF_LOG_INFO("FDS_LOG: <fdsRecUpdate> Updating Succeeded @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
			NRF_LOG_FLUSH();
		}
		
		return fds_state.ret;
	}
}

/** @Func Delete a Record */
ret_code_t fdsRecDelete(void){
	// Search all the records according to the file_id and record_key
	if(fdsRecFind() != FDS_SUCCESS){// Search failed
		return fds_state.ret;
	}
	else{// Delete the record
		fds_flags.fds_del_flag	=	0;
		fds_state.ret 					= fds_record_delete(&fds_state.rec_desc);
		
		// Handle errors
		if(fds_state.ret != FDS_SUCCESS){
			if(fds_flags.fds_log_flag){
				NRF_LOG_INFO("FDS_LOG [Error Code:0x%2x] : <fdsRecDelete> Deleting Failed @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.ret, fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
				NRF_LOG_FLUSH();
			}
			return fds_state.ret;
		}
		
		//	Async Operation
		fds_state.async_fun(&fds_flags.fds_del_flag);
		
		// Print the logs if logging is allowed
		if(fds_flags.fds_log_flag){
			NRF_LOG_INFO("FDS_LOG: <fdsRecDelete> Deleting Succeeded @ Record(FileID: 0x%4x, RecordKey: 0x%4x)\r\n", fds_state.record.rec_info.file_id, fds_state.record.rec_info.key);
			NRF_LOG_FLUSH();
		}
		
		return fds_state.ret;
	}
}

/** @Func Garbage Collection */
ret_code_t fdsGC(void){
	fds_flags.fds_gc_flag = 0;
	
	//	Start garbage collection
	fds_state.ret	 = fds_gc();
	
	// Handle the errors
	if(fds_state.ret != FDS_SUCCESS){
		if(fds_flags.fds_log_flag){
			NRF_LOG_INFO("FDS_LOG [Error Code:0x%2x] : <fdsGC> Garbage Collection Failed\r\n", fds_state.ret);
			NRF_LOG_FLUSH();
		}
		return fds_state.ret;
	}
	
	//	Async Operation
	fds_state.async_fun(&fds_flags.fds_gc_flag);
	
	return fds_state.ret;
}

/** @Func Set Async Operation */
void fdsSetAsynFun(fds_fptr_t func){
	fds_state.async_fun = func;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Get FDS Status */
ret_code_t fdsGetState(void)
{
	return fds_state.ret;
}

/** @Func Get FDS Flags */
fds_flags_t fdsGetFlags(void)
{
	return fds_flags;
}

/** @Func Set FDS Flags */
void fdsSetInitFlag(uint8_t flag)
{
	fds_flags.fds_init_flag = flag;
}

void fdsSetWriteFlag(uint8_t flag)
{
	fds_flags.fds_write_flag	= flag;
}

void fdsSetUpdateFlag(uint8_t flag)
{
	fds_flags.fds_update_flag	= flag;
}

void fdsSetDelFlag(uint8_t flag)
{
	fds_flags.fds_del_flag	=	flag;
}

void fdsSetGcFlag(uint8_t flag)
{
	fds_flags.fds_gc_flag	= flag;
}

void fdsSetLogFlag(uint8_t flag)
{
	fds_flags.fds_log_flag	=	flag;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
