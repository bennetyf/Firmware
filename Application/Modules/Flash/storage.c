/** File Name: storage.c
	*
	* @Author		: Feng Yuan
	* @Time			: 01/08/2017
	* @Version	: 1.0
	* @Brief		:	Provide functions for the application layer of the flash storage.
	*******************************************************************************************************************************
	* This file includes four layers of functions:
	* Basic Utilities (mapIndexToRecID,calWordsSize,bytes2words,words2bytes)-Essential uitility functions used in this library.
	* First Layer (setByteArray,getByteArray,delByteArray)-Interface of the FDS driver layer
	* Second Layer (setOneByteArrayData,getOneByteArrayData)-Add the status tag in each record
	* Third Layer (getDataSection,getOneRecordStatus,initOneRecord,setOneRecord,getOneRecord,delOneRecord)-Basic application functions
	* Fourth Layer (initAllRecords)- Application-oriented functions
	********************************************************************************************************************************
	* Note: 1.Before using this module, initialization of the FDS system is required (Using the fdsInit function in "drv_storage.h").
	* 			 2.The FDS system needs some configuration. The configuration can be set in "sdk_config.h" file or include a separate "fds_config.h" file.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "storage.h"
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Essential Bricks: Utility Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Byte Array to Word Array Conversion */
uint8_t	bytes2words(const uint8_t bytes_size, const uint8_t* const bytes_array, uint32_t* words_array)
{
	uint8_t words_size	= CAL_WORD_ARRAY_SIZE(bytes_size,sizeof(uint32_t));
	uint8_t residue 		= bytes_size % 4;
	
	if(residue == 0){
		// For each word, compress each 4 bytes into one word.
		for(int i=0,j=0; i<words_size; i++,j+=4){
			words_array[i] = bytes_array[j] | bytes_array[j+1]<<8 | bytes_array[j+2]<<16 | bytes_array[j+3]<<24;
		}
		return words_size;
	}
	else{
		// For each word, compress each 4 bytes into one word.
		for(int i=0,j=0; i<words_size-1; i++,j+=4){
			words_array[i] = bytes_array[j] | bytes_array[j+1]<<8 | bytes_array[j+2]<<16 | bytes_array[j+3]<<24;
		}
		// For the last few bytes, compress the residue bytes into the lower bits of the last word.
		switch(residue){
			case 1:
				words_array[words_size-1] =	bytes_array[bytes_size-1];
				break;
			case 2:
				words_array[words_size-1] = bytes_array[bytes_size-2] | bytes_array[bytes_size-1]<<8;
				break;
			case 3:
				words_array[words_size-1] = bytes_array[bytes_size-3] | bytes_array[bytes_size-2]<<8 | bytes_array[bytes_size-1]<<16;
				break;
		}
		return words_size;
	}
}

/** @Func Word Array to Byte Array Conversion */
uint8_t words2bytes(const uint8_t bytes_size, uint8_t* bytes_array, const uint32_t* const words_array)
{
	uint8_t words_size	=	CAL_WORD_ARRAY_SIZE(bytes_size,sizeof(uint32_t));
	uint8_t residue 		= bytes_size % 4;
	
	if (residue == 0){
		// For each word, de-compress it into separate bytes and store them into the byte array.
		for(int i=0,j=0;i<words_size;i++,j+=4){
			bytes_array[j] 		= 	words_array[i] 			& 0x000000FF;
			bytes_array[j+1]	=		(words_array[i]>>8) &	0x000000FF;
			bytes_array[j+2]	=		(words_array[i]>>16)& 0x000000FF;
			bytes_array[j+3]	=		(words_array[i]>>24)&	0x000000FF;
		}
		return words_size;
	}
	else{
		// For each word, de-compress it into separate bytes and store them into the byte array.
		for(int i=0,j=0;i<words_size-1;i++,j+=4){
			bytes_array[j] 		= 	words_array[i] 			& 0x000000FF;
			bytes_array[j+1]	=		(words_array[i]>>8) &	0x000000FF;
			bytes_array[j+2]	=		(words_array[i]>>16)& 0x000000FF;
			bytes_array[j+3]	=		(words_array[i]>>24)&	0x000000FF;
		}
		switch(residue){
			case 1:
				bytes_array[bytes_size-1] 	= 	words_array[words_size-1] & 0x000000FF;
				break;
			case 2:
				bytes_array[bytes_size-2]		= 	words_array[words_size-1] 		 & 0x000000FF;
				bytes_array[bytes_size-1]		= 	(words_array[words_size-1]>>8) & 0x000000FF;
				break;
			case 3:
				bytes_array[bytes_size-3]		= 	words_array[words_size-1] 		 	& 0x000000FF;
				bytes_array[bytes_size-2]		= 	(words_array[words_size-1]>>8) 	& 0x000000FF;
				bytes_array[bytes_size-1]		= 	(words_array[words_size-1]>>16) & 0x000000FF;
		}
		return words_size;
	}
}
//First Layer: Basic Byte Array Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Basic Functions to Operate Byte Arrays */

/** @Func Write one Byte Array Record into the Flash */
flash_status_t 	setByteArray(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, const uint8_t* const bytes_array)
{
	uint8_t   	words_size		=	CAL_WORD_ARRAY_SIZE(bytes_size, sizeof(uint32_t));
	uint32_t		*words_array	=	(uint32_t*)malloc(sizeof(uint32_t)*words_size);
	ret_code_t 	ret_code 			= 0;
	
	// Turn into words array
	bytes2words(bytes_size, bytes_array, words_array);
	
	// Set Up the Flash Record
  if(!fdsRecSetup(file_id, MAP_INDEX_TO_REC_ID(index), words_array, words_size)){
		free(words_array); // Clean up the temporary variable if failed
		return FLASH_STATUS_SETUP_ERR;
	}
	
	// Look Up the Flash Record
	ret_code = fdsRecFind();
	
	// First Time Writing (Create A New Record)
	if(ret_code == FDS_ERR_NOT_FOUND){
		if(fdsRecWrite() != FDS_SUCCESS){// Write the Record
			if(fdsGC() != FDS_SUCCESS){	//Try Garbage Collection Once
				free(words_array);// Clean up the temporary variable if failed
				return FLASH_STATUS_GC_ERR;
			}
			if(fdsRecWrite() != FDS_SUCCESS){// Re-write the Record Again
				free(words_array);// Clean up the temporary variable if failed
				return FLASH_STATUS_WRITE_ERR;
			}
		}
		// Clean Up and Return
		free(words_array);
		return FLASH_STATUS_SUCCESS;
	}
	
	// Not First Time Writing (Update the Existing Record)
	else if(ret_code == FDS_SUCCESS){
		if(fdsRecUpdate() != FDS_SUCCESS){// Update the Record
			if(fdsGC() != FDS_SUCCESS){	//Try Garbage Collection Once
				free(words_array);
				return FLASH_STATUS_GC_ERR;
			}
			if(fdsRecUpdate() != FDS_SUCCESS){// Update the Record Again
				free(words_array);
				return FLASH_STATUS_WRITE_ERR;
			}
		}
		// Clean Up and Return
		free(words_array);
		return FLASH_STATUS_SUCCESS;
	}
	
	// Flash Look Up Error
	else{
		return FLASH_STATUS_LOOKUP_ERR;
	}
}

/** @Func Read a Flash Record into the Destination Byte Array */
flash_status_t	getByteArray(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, uint8_t* bytes_array)
{
	// Set Up Record Meta Data
	if(!fdsRecSetup(file_id, MAP_INDEX_TO_REC_ID(index), NULL, 0)){
		return FLASH_STATUS_SETUP_ERR;
	}
	
	// Temporary Variables for Record Reading
	uint8_t   	words_size		= CAL_WORD_ARRAY_SIZE(bytes_size,sizeof(uint32_t));
	uint32_t 	 	*words_array	=	(uint32_t*)malloc(sizeof(uint32_t)*words_size);
	ret_code_t  ret_code			=	0;
	
	// Try to Read the Record Once
	ret_code = fdsRecRead(words_array);
	
	// Record Not Found
	if(ret_code == FDS_ERR_NOT_FOUND){
		free(words_array); // Clean Up the Temporary Variable if Failed
		return FLASH_STATUS_NOT_FOUND_ERR;
	}
	
	// Flash Read Error
	else if(ret_code != FDS_SUCCESS){
		free(words_array);// Clean Up the Temporary Variable if Failed
		return FLASH_STATUS_READ_ERR;
	}
	
	// Reading Succeeded
	else{
		// Turn words array to bytes array
		words2bytes(bytes_size, bytes_array, words_array);
		// Clean Up and Return
		free(words_array);
		return FLASH_STATUS_SUCCESS;
	}
}

/** @Func Delete a Byte-Array Flash Record */
flash_status_t	delByteArray(const uint16_t file_id, const uint16_t index)
{
	// Set Up the Record
	if(!fdsRecSetup(file_id, MAP_INDEX_TO_REC_ID(index), NULL, 0)){
		return FLASH_STATUS_SETUP_ERR;
	}
	
	// Flash Deletion Failed
	if(fdsRecDelete() != FDS_SUCCESS){
		return FLASH_STATUS_DEL_ERR;
	}
	
	// Flash Deletion Succeeded
	return FLASH_STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Functions to Add the Record Status Tags */

/** @Func Write a Single Byte-array Data into a Flash Record */
flash_status_t setOneByteArrayData(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, const uint8_t * const bytes_array, const uint8_t tag)
{
	uint8_t 				*rec_data				=	(uint8_t*)malloc(bytes_size+1);// Plus one extra byte as the validation tag
	flash_status_t 	flash_ret_code	=	FLASH_STATUS_SUCCESS;
	
	// Add the First Validation Tag
	rec_data[0]=tag;
	
	// Add the Rest of the Data
	for(uint16_t i=1;i<bytes_size+1;i++){
			rec_data[i]=bytes_array[i-1];
	}
	
	// Set the Byte Array
	flash_ret_code=setByteArray(file_id,index,bytes_size+1,rec_data);
	
	// Clean Up
	free(rec_data);
	return flash_ret_code;
}

/** @Func Read a Single Byte-array Record into the Flash */
flash_status_t	getOneByteArrayData(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, uint8_t* bytes_array, uint8_t *tag_ptr)
{
	uint8_t 				*rec_data				=	(uint8_t*)malloc(bytes_size+1); // Add one extra byte to store the first validation tag
	flash_status_t  flash_ret_code	=	FLASH_STATUS_SUCCESS;
	
	// Read in the entier byte array
	flash_ret_code = getByteArray(file_id,index,bytes_size+1,rec_data);
	
	// Record Reading Failed
	if(flash_ret_code != FLASH_STATUS_SUCCESS){
		return flash_ret_code;
	}
	
	// Check the First Byte(the Flag) in the Record
	else{
		if(tag_ptr != NULL){//Get the Tag Out
			*tag_ptr = rec_data[0];
		}
		
		// Move the read data into destination memory address
		for(uint16_t i = 0;i<bytes_size;i++){
			bytes_array[i] = rec_data[i+1];
		}
		
		// Clean Up
		free(rec_data);
		return flash_ret_code;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Functions to Check the Data Section */
flash_section_t getDataSection(const uint16_t index)
{
	if(index >= SET_DATA_DEVINFO_START_INDEX && index <= SET_DATA_DEVINFO_END_INDEX){
		return FLASH_SECTION_DEVICE_INFO;
	}
	else if(index >= SET_DATA_DFU_START_INDEX && index <= SET_DATA_DFU_END_INDEX){
		return FLASH_SECTION_DFU_INFO;
	}
	else if(index >= SET_DATA_DEVSET_START_INDEX && index <= SET_DATA_DEVSET_END_INDEX){
		return FLASH_SECTION_DEVICE_SETTINGS;
	}
	else if(index >= SET_DATA_SYS_START_INDEX && index <= SET_DATA_SYS_END_INDEX){
		return FLASH_SECTION_SYS_STATUS;
	}
	else if(index >= SET_DATA_SENR_START_INDEX  && index <= SET_DATA_SENR_END_INDEX){
		return FLASH_SECTION_SENSOR_SETTINGS;
	}
	else if(index >= COLOR_DATA_START_INDEX && index <= COLOR_DATA_END_INDEX){
		return FLASH_SECTION_COLORS;
	}
	else if(index >= EXT_DATA_MAN_START_INDEX && index <= EXT_DATA_MAN_END_INDEX){
		return FLASH_SECTION_MANUFACTURE;
	}
	else if(index >= EXT_DATA_USER_START_INDEX && index <= EXT_DATA_USER_END_INDEX){
		return FLASH_SECTION_USER;
	}
	else if(index >= EXT_DATA_RESERVED_START_INDEX && index <= EXT_DATA_RESERVED_END_INDEX){
		return FLASH_SECTION_RESERVED;
	}
	else{
		return FLASH_SECTION_OUT_OF_RANGE;
	}
}

/** @Func Functions to Operate Any Record */
flash_status_t getOneRecordStatus(const uint16_t index)
{
	// Try to Read the First Byte of the Record Once
	const uint8_t  	*default_sizes	=	NULL; 									// Pointer to the default size array
	uint8_t 				tag_byte				=	0; 											// Temporary byte to store the tag value.
	uint8_t					*bytes_array		= NULL; 									// Byte array to store the read data.
	flash_status_t 	flash_ret_code	=	FLASH_STATUS_SUCCESS;		// Flash operation return code.
	flash_section_t flash_sec_code	=	getDataSection(index);	// Data section code.
	
	// Switch Data Sections
	switch(flash_sec_code){
		case FLASH_SECTION_DEVICE_INFO:
		{
			default_sizes		=	getDevSizeInfo();
			bytes_array			=	(uint8_t*)malloc(default_sizes[index-SET_DATA_DEVINFO_START_INDEX]);
			flash_ret_code	=	getOneByteArrayData(FLASH_FILEID_SETTINGS,index,default_sizes[index-SET_DATA_DEVINFO_START_INDEX],bytes_array,&tag_byte);
			free(bytes_array);
			bytes_array			=	NULL;
			break;
		}
		
		// One-word (32-bit) Data Section
		case FLASH_SECTION_DFU_INFO:
		case FLASH_SECTION_DEVICE_SETTINGS:
		case FLASH_SECTION_SYS_STATUS:
		case FLASH_SECTION_SENSOR_SETTINGS:
		{
			bytes_array			=	(uint8_t*)malloc(4);
			flash_ret_code	=	getOneByteArrayData(FLASH_FILEID_SETTINGS,index,4,bytes_array,&tag_byte);
			free(bytes_array);
			bytes_array			=	NULL;
			break;
		}
		
		// Color Data Section 
		case FLASH_SECTION_COLORS:
		{
			bytes_array			=	(uint8_t*)malloc(4);
			flash_ret_code	=	getOneByteArrayData(FLASH_FILEID_COLOR,index,4,bytes_array,&tag_byte);
			free(bytes_array);
			bytes_array			=	NULL;
			break;
		}
		
		// Manufacturer Data Section
		case FLASH_SECTION_MANUFACTURE:
		{
			default_sizes		=	getManSizeInfo();
			bytes_array			=	(uint8_t*)malloc(default_sizes[index-EXT_DATA_MAN_START_INDEX]);
			flash_ret_code	=	getOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_MAN_START_INDEX],bytes_array,&tag_byte);
			free(bytes_array);
			bytes_array			=	NULL;
			break;
		}
		
		// User Data Section
		case FLASH_SECTION_USER:
		{
			default_sizes		=	getUsrSizeInfo();
			bytes_array			=	(uint8_t*)malloc(default_sizes[index-EXT_DATA_USER_START_INDEX]);
			flash_ret_code	=	getOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_USER_START_INDEX],bytes_array,&tag_byte);
			free(bytes_array);
			bytes_array			=	NULL;
			break;
		}
		
		// Reserved Data Section
		case FLASH_SECTION_RESERVED:
		{
			default_sizes		=	getReservedSizeInfo();
			bytes_array			=	(uint8_t*)malloc(default_sizes[index-EXT_DATA_RESERVED_START_INDEX]);
			flash_ret_code	=	getOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_RESERVED_START_INDEX],bytes_array,&tag_byte);
			free(bytes_array);
			bytes_array			=	NULL;
			break;
		}
		default:
			return FLASH_STATUS_OUT_OF_RANGE_ERR;
	}
	
	// Record Reading Failed
	if(flash_ret_code != FLASH_STATUS_SUCCESS){
		return flash_ret_code;
	}
	
	// Check the First Byte(the Flag) in the Record
	// Record Has Been Changed From its Default Value
	else if(tag_byte == FLASH_RECTAG_DEFAULT_BYTE){
		return FLASH_STATUS_REC_UNCHANGED;
	}
	
	// Record Data Has Not Been Changed From its Default Value
	else if(tag_byte == FLASH_RECTAG_CHANGED_BYTE){
		return FLASH_STATUS_REC_CHANGED;
	}
	
	// Fatal Error: Record Flag Is Wrong
	else{
		return FLASH_STATUS_RECTAG_ERR;
	}
}

/** @Func Functions to Set Any Record to Its Default Value */
flash_status_t 	initOneRecord(const uint16_t index)
{
	const uint8_t			 				tag								=	FLASH_RECTAG_DEFAULT_BYTE;
	const uint8_t						 	*default_sizes		=	NULL;
	const uint8_t 					 	**default_values	=	NULL;
	const union data_set_t 	 	*default_settings	=	NULL;
	const union data_color_t 	*default_colors		=	NULL;
	flash_section_t 					flash_sec_code		=	getDataSection(index);
	
	// Switch Data Sections
	switch(flash_sec_code){
		case FLASH_SECTION_DEVICE_INFO:
		{
			default_sizes=getDevSizeInfo();	// Get the Data Size at the Index
			default_values=getDevInfo();		//Get the Default Values at the Index 
			return setOneByteArrayData(FLASH_FILEID_SETTINGS,index,default_sizes[index-SET_DATA_DEVINFO_START_INDEX],default_values[index-SET_DATA_DEVINFO_START_INDEX],tag);
		}
		
		// One-word (32-bit) Data Section
		case FLASH_SECTION_DFU_INFO:
		case FLASH_SECTION_DEVICE_SETTINGS:
		case FLASH_SECTION_SYS_STATUS:
		case FLASH_SECTION_SENSOR_SETTINGS:
		{
			default_settings=getOwSetInfo();
			return setOneByteArrayData(FLASH_FILEID_SETTINGS,index,4,default_settings[index-SET_DATA_ONE_WORD_START_INDEX].byte,tag);
		}
		
		// Color Data Section
		case FLASH_SECTION_COLORS:
		{
			default_colors=getColorInfo();
			return setOneByteArrayData(FLASH_FILEID_COLOR,index,4,default_colors[index-COLOR_DATA_START_INDEX].byte,tag);
		}
		
		// Manufacturer Data Section
		case FLASH_SECTION_MANUFACTURE:
		{
			default_sizes=getManSizeInfo();
			default_values=getManInfo();
			return setOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_MAN_START_INDEX],default_values[index-EXT_DATA_MAN_START_INDEX],tag);
		}
		
		// User Data Section
		case FLASH_SECTION_USER:
		{
			default_sizes=getUsrSizeInfo();
			default_values=getUsrInfo();
			return setOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_USER_START_INDEX],default_values[index-EXT_DATA_USER_START_INDEX],tag);
		}
		
		// Reserved Data Section
		case FLASH_SECTION_RESERVED:
		{
			default_sizes=getReservedSizeInfo();
			default_values=getReservedInfo();
			return setOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_RESERVED_START_INDEX],default_values[index-EXT_DATA_RESERVED_START_INDEX],tag);
		}
		default:
			return FLASH_STATUS_OUT_OF_RANGE_ERR;
	}
}

/** @Func Write a single record into the flash */
flash_status_t setOneRecord(const uint16_t index, const uint8_t* const bytes_array)
{
	const uint8_t 		*default_sizes	=	NULL; // Variable to store the size of the byte array
	const uint8_t 		tag							=	FLASH_RECTAG_CHANGED_BYTE;
	flash_section_t 	flash_sec_code	=	getDataSection(index);
	
	// Switch Data Sections
	switch(flash_sec_code){
		case FLASH_SECTION_DEVICE_INFO:
		{
			default_sizes=getDevSizeInfo();
			return setOneByteArrayData(FLASH_FILEID_SETTINGS,index,default_sizes[index-SET_DATA_DEVINFO_START_INDEX],bytes_array,tag);
		}
		
		// One-word Data Section
		case FLASH_SECTION_DFU_INFO:
		case FLASH_SECTION_DEVICE_SETTINGS:
		case FLASH_SECTION_SYS_STATUS:
		case FLASH_SECTION_SENSOR_SETTINGS:
			return setOneByteArrayData(FLASH_FILEID_SETTINGS,index,4,bytes_array,tag);
		case FLASH_SECTION_COLORS:
			return setOneByteArrayData(FLASH_FILEID_COLOR,index,4,bytes_array,tag);
		
		// Manufacturer Data Section
		case FLASH_SECTION_MANUFACTURE:
		{
			default_sizes=getManSizeInfo();
			return setOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_MAN_START_INDEX],bytes_array,tag);
		}
		
		// User Data Section
		case FLASH_SECTION_USER:
		{
			default_sizes=getUsrSizeInfo();
			return setOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_USER_START_INDEX],bytes_array,tag);
		}
		
		// Reserved Data Section
		case FLASH_SECTION_RESERVED:
		{
			default_sizes=getReservedSizeInfo();
			return setOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_RESERVED_START_INDEX],bytes_array,tag);
		}
		default:
			return FLASH_STATUS_OUT_OF_RANGE_ERR;
	}
}

/** @Func Read One Record Out of the Flash */
flash_status_t		getOneRecord(const uint16_t index, uint8_t * bytes_array)
{
	const uint8_t   *default_sizes					=	NULL;
	uint8_t  			  *tag_ptr								=	NULL;
	flash_section_t flash_sec_code					=	getDataSection(index);
	
	// Switch Data Sections
	switch(flash_sec_code){
		case FLASH_SECTION_DEVICE_INFO:
		{
			default_sizes=getDevSizeInfo();
			return getOneByteArrayData(FLASH_FILEID_SETTINGS,index,default_sizes[index-SET_DATA_DEVINFO_START_INDEX],bytes_array,tag_ptr);
		}
		case FLASH_SECTION_DFU_INFO:
		case FLASH_SECTION_DEVICE_SETTINGS:
		case FLASH_SECTION_SYS_STATUS:
		case FLASH_SECTION_SENSOR_SETTINGS:
			return getOneByteArrayData(FLASH_FILEID_SETTINGS,index,4,bytes_array,tag_ptr);
		case FLASH_SECTION_COLORS:
			return getOneByteArrayData(FLASH_FILEID_COLOR,index,4,bytes_array,tag_ptr);
		case FLASH_SECTION_MANUFACTURE:
		{
			default_sizes=getManSizeInfo();
			return getOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_MAN_START_INDEX],bytes_array,tag_ptr);
		}
		case FLASH_SECTION_USER:
		{
			default_sizes=getUsrSizeInfo();
			return getOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_USER_START_INDEX],bytes_array,tag_ptr);
		}
		case FLASH_SECTION_RESERVED:
		{
			default_sizes=getReservedSizeInfo();
			return getOneByteArrayData(FLASH_FILEID_EXTENDED,index,default_sizes[index-EXT_DATA_RESERVED_START_INDEX],bytes_array,tag_ptr);
		}
		default:
			return FLASH_STATUS_OUT_OF_RANGE_ERR;
	}
}

/** @Func Delete One Record in the Flash */
flash_status_t	delOneRecord(const uint16_t index)
{
	flash_section_t flash_sec_code = getDataSection(index);
	
	// Switch Data Sections
	switch(flash_sec_code){
		case FLASH_SECTION_DEVICE_INFO:
		case FLASH_SECTION_DFU_INFO:
		case FLASH_SECTION_DEVICE_SETTINGS:
		case FLASH_SECTION_SYS_STATUS:
		case FLASH_SECTION_SENSOR_SETTINGS:
			return delByteArray(FLASH_FILEID_SETTINGS,index);
		case FLASH_SECTION_COLORS:
			return delByteArray(FLASH_FILEID_COLOR,index);
		case FLASH_SECTION_MANUFACTURE:
		case FLASH_SECTION_USER:
		case FLASH_SECTION_RESERVED:
			return delByteArray(FLASH_FILEID_EXTENDED,index);
		default:
			return FLASH_STATUS_OUT_OF_RANGE_ERR;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Set All Record Data to Default Values */
flash_status_t	initAllRecords(void)
{
	flash_status_t ret_code	=	FLASH_STATUS_SUCCESS;
	for(uint16_t index = 1; index <= EXT_DATA_RESERVED_END_INDEX; index++){
		ret_code=initOneRecord(index);
		if(ret_code != FLASH_STATUS_SUCCESS){
			return ret_code;
		}
	}
	return ret_code;
}

/** @Func The Function to Check the Tags in All Records */
bool isAllRecordValid(void)
{
	flash_status_t 	flash_ret_code	=	FLASH_STATUS_SUCCESS;		// Flash operation return code.
	for(uint16_t index = 1; index <= EXT_DATA_RESERVED_END_INDEX; index++){
		flash_ret_code	=	getOneRecordStatus(index);
		if(flash_ret_code != FLASH_STATUS_REC_UNCHANGED){
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
