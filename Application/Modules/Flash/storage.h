/** File Name: storage.h
	*
	*	@Brief	Provide functions for the application layer of the flash storage
	*
	* @Author		: Feng Yuan
	* @Time			: 01/08/2017
	* @Version	: 1.0
	*******************************************************************************************************************************
	* This file includes four layers of functions:
	* Basic Utilities (bytes2words,words2bytes)-Essential uitility functions used in this library.
	* First Layer (setByteArray,getByteArray,delByteArray)-Interface of the FDS driver layer
	* Second Layer (setOneByteArrayData,getOneByteArrayData)-Add the status tag in each record
	* Third Layer (getDataSection,getOneRecordStatus,initOneRecord,setOneRecord,getOneRecord,delOneRecord)-Basic application functions
	* Fourth Layer (initAllRecords)- Application-oriented functions
	********************************************************************************************************************************
	* Note: 	1.Before using this module, initialization of the FDS system is required (Using the fdsInit function in "drv_storage.h").
	* 			 	2.The FDS system needs some configuration. The configuration can be set in "sdk_config.h" file or include a separate "fds_config.h" file.
	*
	*	@Macro	FLASH_RECTAG_DEFAULT_BYTE 	(Record Tag for Default Record Value)
	*	@Macro	FLASH_RECTAG_CHANGED_BYTE 	(Record Tag for Record Value that Has Been Changed from its Default Value)
	*	@Macro	FLASH_FILEID_SETTINGS				(File ID for Settings Data)
	*	@Macro	FLASH_FILEID_COLOR					(File ID for Color Data)
	*	@Macro	FLASH_FILEID_EXTENDED				(File ID for Extended Data)
	*	@Macro	CAL_WORD_ARRAY_SIZE					(Procedure for Calculation of the Word-Array Size)
	* @Macro	MAP_INDEX_TO_REC_ID					(Precedure for Mapping An Index into A Record Key)
	*
	*	@Type		flash_status_t							(Flash Operation Status)
	*	@Type		flash_section_t							(Data Section)
	*
	*	@Func		bytes2words									(Byte-array to Word-array Conversion)
	*	@Func		words2bytes									(Word-array to Byte-array Conversion)
	*
	*	@Func		setByteArray								(Write a Byte-array Record into the Flash)
	*	@Func		getByteArray								(Read a Byte-array Record out of the Flash)
	*	@Func		delByteArray								(Delete a Byte-array Record in the Flash)
	*
	*	@Func		setOneByteArrayData					(Write a Byte-array Record into the Flash with Record Tag)
	*	@Func		getOneByteArrayData					(Read a Byte-array Record out of the Flash with Record Tag)
	*
	*	@Func		getDataSection							(Check the Data Section of the Specified Data Index)
	*	@Func		getOneRecordStatus					(Read out the Record Status According to Its Tag)
	*
	*	@Func		initOneRecord								(Initialize One Record - Write Its Value to Defaults)
	*	@Func		setOneRecord								(Write One Record into the Flash)
	* @Func		getOneRecord								(Read One Record out of the Flash)
	*	@Func		delOneRecord								(Delete One Record from the Flash)
	*
	*	@Func   initAllRecords							(Initialize All Records)
	*	@Func		isAllRecordValid						(Check the Validity of the Record)
	*
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __STORAGE_H__
#define __STORAGE_H__
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Including essential modules */

// FDS Driver Module
#include "drv_storage.h"

// Application-oriented Data Files
#include "data_colors.h"
#include "data_settings.h"
#include "data_extended.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Define the flag for each record (8 bits)
	* This flag is used to show whether an record has been changed from its default value or not.
	* This flag is only used for development purpose.
	* In the first-time initialization of a record, the tag is automatically set to 0xAA.
	* In the following updating operations of this record, the tag is automatically set to 0xBB.
	* At any time, this tag (including the record value) can be reset to its default using the initOneRecord function.
	* The status of a record can be obtained by the getOneRecordStatus function.
*/
#define FLASH_RECTAG_DEFAULT_BYTE                    0xAA
#define FLASH_RECTAG_CHANGED_BYTE										 0xBB

/** @Macro Define file IDs (16 bits)
	* To use the FDS system, file IDs and record keys are unique names for each record.
	* Indices are mapped into record keys using the mapIndexToRecID function.
	* File IDs are pre-defined values.
	* Here, all records are organised into 3 different files (each corresponds to a data file)
	* Note: If you want to change these values, the file IDs should be within the range: 0x0000 - 0xBFFF 
	* (ref: http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v12.2.0%2Flib_flash.html&cp=4_0_4_3_12)
*/
// Define File IDs. In each file, different data is distinguished using the record id
#define FLASH_FILEID_SETTINGS												 0xAAAA		//File ID for All Settings Data
#define	FLASH_FILEID_COLOR											 		 0xABBB		//File ID for All Color Calibration Data(Temperature Data Included)
#define FLASH_FILEID_EXTENDED												 0xACCC		//File ID for All Extended Data

/** @Macro Define a macro to calculate the word array size from the given byte array size 
	* In essence, this is just a ceiling function
*/
#define CAL_WORD_ARRAY_SIZE(A,B) (((A)+(B)-1)/(B))

/** @Macro Define a macro to map the index to record key
	* The starting record key is 0x1000
*/
#define MAP_INDEX_TO_REC_ID(A) (0x1000+A)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define Data Types Used in This Module
 * In this storage module, we define two data types: flash_status_t and flash_section_t.
 * flash_status_t defines the operation status returned by module functions,including errors and record status.
 * flash_section_t defines the separate data sections. 
 * These sections are manually defined according to the purposes of these data and should be transparent to users of this module.
 * The definitions of flash_status_t and flash_section_t can be amended and extended for future usage.
*/
	
/** @Type Flash Status */
typedef enum {
	FLASH_STATUS_SUCCESS,								//Flash Operation Succeeded
	FLASH_STATUS_SETUP_ERR,							//Flash Record Setup Error
	FLASH_STATUS_LOOKUP_ERR,						//Flash Record Lookup Error
	FLASH_STATUS_READ_ERR,							//Flash Record Read Error
	FLASH_STATUS_WRITE_ERR,							//Flash Record Write Error
	FLASH_STATUS_DEL_ERR,								//Flash Record Delete Error
	FLASH_STATUS_GC_ERR,								//Flash Garbage Collection Error
	FLASH_STATUS_NOT_FOUND_ERR,					//Flash Record Not Found Error
	FLASH_STATUS_OUT_OF_RANGE_ERR,    	//Flash Index Out of Range Error
	FLASH_STATUS_REC_UNCHANGED,					//Flash Record Exists And Unchanged(Has the Default Value)
	FLASH_STATUS_REC_CHANGED,						//Flash Record Exists But Was Changed
	FLASH_STATUS_RECTAG_ERR							//Flash Record Flag Error(Something Happened and the Flag Does Not Equal to Valid or Invalid)		
}flash_status_t;

/** @Type Flash Data Section */
typedef enum {
	FLASH_SECTION_OUT_OF_RANGE,					//Out of the data section range
	FLASH_SECTION_DEVICE_INFO,					//Data Section of the Device Information
	FLASH_SECTION_DFU_INFO,							//Data Section of the DFU Information
	FLASH_SECTION_DEVICE_SETTINGS,			//Data Section of the Device Settings
	FLASH_SECTION_SYS_STATUS,						//Data Section of the System Status
	FLASH_SECTION_SENSOR_SETTINGS,			//Data Section of the Sensor Settings
	FLASH_SECTION_COLORS,								//Data Section of the Colors
	FLASH_SECTION_MANUFACTURE,					//Data Section of the Manufacturer Data
	FLASH_SECTION_USER,									//Data Section of the User Data
	FLASH_SECTION_RESERVED							//Data Section of the Reserved Data
}flash_section_t;

/* Module Functions Declarations */
//Essential Bricks: Utility Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Bytes to Words and Words to Bytes Conversion
	*
	* @Brief		The following two functions are used to perform conversions between byte arrays and word arrays.
	* 			 		These two functions are internally used and should be transparent to the user of this module.
	* 					Note: the maximum size of the byte array is 255(including the status tag).Exceeding this value will generate undefined errors.
	*
	*	@Para	 		bytes_size 	[uint8_t]		:	the length of the byte array
	*	@Para	 		words_size	[uint8_t]		:	the length of the word array
	*	@Para	 		bytes_array [uint8_t*]	:	the address of the byte array
	*	@Para  		words_array	[uint32_t*]	:	the address of the word array
	*
	*	@Return 	the length of the word array
	*
*/
uint8_t		bytes2words(const uint8_t bytes_size, const uint8_t* const bytes_array, uint32_t* words_array);
uint8_t 	words2bytes(const uint8_t words_size, uint8_t* bytes_array, const uint32_t* const words_array);

//First Layer: Basic Byte Array Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Basic Functions to Operate Byte Arrays
	*
	* @Brief	These functions act as wrappers of the bottom fds drivers.
	* 			 	All these functions do not manage flash errors and only return the flash operation status.
	*******************************************************************************************************************************
	* @Brief 	setByteArray writes an record into the flash memory using FDS system.
	* 				By default, setByteArray creates a new record on first-time writing and updates a record if it already exists.
	* 				Garbage collection is automatically performed by the setByteArray function when the flash is full.
	*******************************************************************************************************************************
	* @Brief	getByteArray reads an existing record from the flash.
	* 				getByteArray stores the read data in the parameter bytes_array which is a pointer to a byte array.
	* 				The space to store the data should be created outside this function.
	*******************************************************************************************************************************
	* @Brief	delByteArray deletes an existing record from the flash.
	* 				delByteArray only deletes the record and DOES NOT perform garbage collection to release the space.
	*
	*	@Para		file_id 		[uint16_t]	:	the file id of the record
	*	@Para		index				[uint16_t]	:	the index of the record
	*	@Para		bytes_size	[uint8_t]		:	the length of the byte array of the record
	*	@Para		bytes_array	[uint8_t*]	:	the address of the byte array
	*
	*	@Return	FLASH_STATUS_SETUP_ERR			: Record Setup Failed	
	*	@Return	FLASH_STATUS_GC_ERR					:	Garbage Collection Failed
	*	@Return FLASH_STATUS_WRITE_ERR			:	Record Writing Failed
	*	@Return FLASH_STATUS_SUCCESS				:	Operation Succeeded
	*	@Return	FLASH_STATUS_LOOKUP_ERR			:	Record Look Up Failed
	*	@Return FLASH_STATUS_NOT_FOUND_ERR 	:	Record Not Found
	*	@Return FLASH_STATUS_READ_ERR				:	Record Reading Failed
	*	@Return FLASH_STATUS_DEL_ERR				:	Record Deletion Failed
	*
*/
flash_status_t 	setByteArray(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, const uint8_t* const bytes_array);
flash_status_t	getByteArray(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, uint8_t* bytes_array);
flash_status_t	delByteArray(const uint16_t file_id, const uint16_t index);

//Second Layer: Add Record Status Tag////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Func Functions to Add the Record Status Tags
	*
	* @Brief These functions only write or read one record data from the flash with their status tags.
	*******************************************************************************************************************************
	* @Brief 	setOneByteArrayData adds one-byte(8 bits) tag onto one byte array and writes it into the flash.
	* 				The value of the tag is given in the parameter "tag". The function does not automatically decide which tag to be added on.
	*******************************************************************************************************************************
	* @Brief	getOneByteArrayData reads one record from the flash.
	* 				This function DOES NOT check the value of the tag. Instead, it only returns the tag value using the parameter "*tag_ptr".
	* 				If *tag_ptr is set to NULL, this function discards the tag value and only read the payload of the record.
	*
	* @Para		file_id 		[uint16_t]	:	the file id of the record
	*	@Para		index				[uint16_t]	: the record index
	*	@Para		bytes_size 	[uint8_t]		:	the length of the byte array
	*	@Para		bytes_array [uint8_t*]	: the address of the byte array
	*	@Para		tag					[uint8_t]		: the record tag value to be added
	*	@Para		tag_ptr			[uint8_t*]	: the pointer of the record tag value read from the flash
	*
	*	@Return Propagate the internal errors
	*
*/
flash_status_t	setOneByteArrayData(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, const uint8_t * const bytes_array, const uint8_t tag);
flash_status_t	getOneByteArrayData(const uint16_t file_id, const uint16_t index, const uint8_t bytes_size, uint8_t * bytes_array, uint8_t *tag_ptr);

//Third Layer(High-Level Interface): Record Operation Functions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Functions in this part are used as API interfaces for the upper layer applications */

/** @Func Functions to Determine the Data Section
	*
	* @Brief 	This fucntion is used to return the corresponding data section given the index number of the record.
	* 				This function returns the flash_section_t type.
	* 				This function should be used internally and be transparent to the user.
	*
	*	@Para		index [uint16_t]	:	the index of the record
	*
	*	@Return	FLASH_SECTION_DEVICE_INFO 			:	Device information section
	*	@Return	FLASH_SECTION_DFU_INFO					:	DFU information section
	*	@Return	FLASH_SECTION_DEVICE_SETTINGS		:	Device settings information section
	*	@Return	FLASH_SECTION_SYS_STATUS				:	System status information section
	*	@Return	FLASH_SECTION_SENSOR_SETTINGS		:	Sensor settings section
	* @Return FLASH_SECTION_COLORS						:	Colors section
	*	@Return	FLASH_SECTION_MANUFACTURE				:	Manufacture section
	*	@Return	FLASH_SECTION_USER							:	User section
	*	@Return FLASH_SECTION_RESERVED					:	Reserved section
	*	@Return	FLASH_SECTION_OUT_OF_RANGE			:	Index out of range
	*
*/
flash_section_t	getDataSection(const uint16_t index);

/** @Func Functions to check the status of one record
	*
	* @Brief 	This function is used to get the status of a single record.
	* 				This function reads the entire record data but only checks its tag value and returns the record status.
	*
	*	@Para		index [uint16_t]	:	the index of the record
	*
	*	@Return	FLASH_STATUS_REC_UNCHANGED	:	Record data is its default value
	*	@Return	FLASH_STATUS_REC_CHANGED		: Record data has been changed
	*	@Return	FLASH_STATUS_RECTAG_ERR			: Tag reading is error
	*
*/
flash_status_t	getOneRecordStatus(const uint16_t index);


/** @Func Functions to Operate Any Record
	*
	* @Brief 	These functions are interfaces to upper layer applications
	* 				These functions automatically decides which section the index should belong to.
	* 				All the information that you need to provide is the record index number and the byte array that stores the data.
	*******************************************************************************************************************************
	* initOneRecord sets the record to its default value that is pre-stored in the corresponding data files.
	* initOneRecord always sets the record status tag to 0xAA. If the record already exists, it still sets the tag to 0xAA, regardless of its current tag.
	* initOneRecord returns the flash operation status.
	*	@Para		index [uint16_t]	: the index of the record
	*
	*	@Return	Propagate internal errors
	*******************************************************************************************************************************
	* setOneRecord sets the record payload to any given value in the input byte array.
	* setOneRecord always sets the record status tag to 0xBB. If the record does not exist, it still sets the tag to 0xBB.
	* Therefore, setOneRecords should not be used without calling the initOneRecord first. Otherwise, the record status tag is wrong.
	* setOneRecord returns the flash operation status.
	* @Para		index 			[uint16_t]	: the index of the record
	*	@Para		bytes_array	[uint8_t*]	:	the address of the byte array to be stored into the flash
	*
	*	@Return	Propagate internal errors
	*******************************************************************************************************************************
	* getOneRecord reads the record payload from the flash into the parameter bytes_array.
	* getOneRecord always discards the record status tag and reads its payload only.(If you want to check the record status, use getOneRecordStatus function instead.)
	* getOneRecord returns the flash operation status.
	*	@Para		index 			[uint16_t]	: the index of the record
	*	@Para		bytes_array	[uint8_t*]	:	the address of the byte array to be read from the flash
	*
	*	@Return	Propagate internal errors
	*******************************************************************************************************************************
	* delOneRecord deletes the given record from the flash.
	* delOneRecord does not perform garbage collection after deletion.
	* delOneRecord returns the flash operation status.
	*	@Para		index 			[uint16_t]	: the index of the record
	*
	*	@Return	Propagate internal errors
	*******************************************************************************************************************************
*/
flash_status_t	initOneRecord(const uint16_t index);
flash_status_t	setOneRecord(const uint16_t index, const uint8_t* const bytes_array);
flash_status_t	getOneRecord(const uint16_t index, uint8_t* bytes_array);
flash_status_t	delOneRecord(const uint16_t index);

//Fourth Layer: Application Functions/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Application Functions that Use the Above Interfaces */

/** @Func Initialize All Data to the Default Values
	*
	* @Brief 	This function intializes all the records to their default values stored in the data files.
	* 				This function returns the flash operation status.
	*	@Return	Propagate internal errors
	*
*/
flash_status_t	initAllRecords(void);

/** @Func The Function to Check the Tags in All Records
	*
	* @Brief 	This function is used to check whether all the records in the flash have the default values.
	* 				If all the records in the flash are of their default values, this function returns true. Otherwise, returns false.
	*	@Return	false		: the record value is its default(has not been changed)
	*	@Return	true		:	the record value is not its default (has been changed)
	*
*/
bool isAllRecordValid(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Functions for Device Information Data */
/* To be Defined */

/* Functions for One-word Settings Data */
/* To be Defined */

/* Functions for Color Data*/
/* To be Defined */

/* Functions for Extended Data */
/* To be Defined */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif //__cplusplus
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //__STORAGE_H__
