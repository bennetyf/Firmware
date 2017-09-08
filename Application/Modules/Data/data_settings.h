/* File Name: data_settings.h
 * Author		: Feng Yuan
 * Time			: 01/08/2017
 * Version	: 1.0
 * Functions:
 * Store the information of the settings data set (indices, the number of different data blocks).
 * Set the default values of these data.
 * Provide accessors to get the default data sizes and data values.
*/

#ifndef __SET_DATATINGS_H__
#define __SET_DATATINGS_H__

#include <stdint.h>

/* Settings Data Basic Info */

// Device Information
#define SET_DATA_DEVINFO_START_INDEX			1
#define SET_DATA_DEVINFO_END_INDEX				9
#define SET_DATA_DEVINFO_NUM							SET_DATA_DEVINFO_END_INDEX-SET_DATA_DEVINFO_START_INDEX+1
// DFU System Information
#define SET_DATA_DFU_START_INDEX		 		 	10
#define SET_DATA_DFU_END_INDEX					 	13
#define SET_DATA_DFU_NUM								 	SET_DATA_DFU_END_INDEX-SET_DATA_DFU_START_INDEX+1
// Device Settings
#define SET_DATA_DEVSET_START_INDEX			 	14
#define SET_DATA_DEVSET_END_INDEX				 	16
#define SET_DATA_DEVSET_NUM							 	SET_DATA_DEVSET_END_INDEX-SET_DATA_DEVSET_START_INDEX+1
// System Status
#define SET_DATA_SYS_START_INDEX				 	17
#define SET_DATA_SYS_END_INDEX					 	20
#define SET_DATA_SYS_NUM								 	SET_DATA_SYS_END_INDEX-SET_DATA_SYS_START_INDEX+1
// Color Sensor Settings 
#define SET_DATA_SENR_START_INDEX 			 	21
#define SET_DATA_SENR_END_INDEX					 	29
#define SET_DATA_SENR_NUM								 	SET_DATA_SENR_END_INDEX-SET_DATA_SENR_START_INDEX+1

#define SET_DATA_NUM										  SET_DATA_DEVINFO_NUM \
																				+ SET_DATA_DFU_NUM \
																				+ SET_DATA_DEVSET_NUM \
																				+ SET_DATA_SYS_NUM \
																				+ SET_DATA_SENR_NUM

// The range of 32-bit settings data
#define SET_DATA_ONE_WORD_START_INDEX   	10
#define SET_DATA_ONE_WORD_END_INDEX 			29
#define SET_DATA_ONE_WORD_NUM							SET_DATA_DFU_NUM \
																				+ SET_DATA_DEVSET_NUM \
																				+ SET_DATA_SYS_NUM \
																				+ SET_DATA_SENR_NUM

/* Size Information of This Data Set */

// Device Information
#define SET_DATA_DEVINFO_VMI_SIZE_BYTE		4
#define SET_DATA_DEVINFO_SN_SIZE_BYTE			8				
#define SET_DATA_DEVINFO_MAC_SIZE_BYTE		6
#define SET_DATA_DEVINFO_NAME_SIZE_BYTE		8		
#define SET_DATA_DEVINFO_MODN_SIZE_BYTE		8		
#define SET_DATA_DEVINFO_MANN_SIZE_BYTE		8
#define SET_DATA_DEVINFO_HR_SIZE_BYTE			8
#define SET_DATA_DEVINFO_FR_SIZE_BYTE			8
#define SET_DATA_DEVINFO_FCDT_SIZE_BYTE	 40

/* Default Values of This Data Set 
 * Note: The device information default values are set as a pointer array in the "data_settings.c" file.
 * 			 All other default values of this data set are set here.
*/

// DFU System Information
#define SET_DATA_DFU_APPVER_DEFAULT			 0x00000000
#define SET_DATA_DFU_BLVER_DEFAULT			 0x00000000
#define SET_DATA_DFU_SDVER_DEFAULT			 0x00000000
#define SET_DATA_DFU_HDVER_DEFAULT			 0x00000000
//Device Settings
#define SET_DATA_DEVSET_IDLE_DEFAULT		 0x00000000
#define SET_DATA_DEVSET_REGC_DEFAULT		 0x00000000
#define SET_DATA_DEVSET_BLSEL_DEFAULT		 0x00000000
//System Status
#define SET_DATA_SYS_MEM_DEFAULT				 0x00000000
#define SET_DATA_SYS_CALIB_DEFAULT			 0x00000000
#define SET_DATA_SYS_POWNUM_DEFAULT			 0x00000000
#define SET_DATA_SYS_CHRNUM_DEFAULT			 0x00000000
//Color Sensor Settings
#define SET_DATA_SENR_GSEL_DEFAULT			 0x00000000
#define SET_DATA_SENR_INTMOD_DEFAULT		 0x00000000
#define SET_DATA_SENR_INTSET_DEFAULT		 0x00000000
#define SET_DATA_SENR_MANT_DEFAULT			 0x00000000
#define SET_DATA_SENR_DCMOD_DEFAULT			 0x00000000
#define SET_DATA_SENR_OTMOD_DEFAULT			 0x00000000
#define SET_DATA_SENR_RLEDCUR_DEFAULT		 0x00000000
#define SET_DATA_SENR_GLEDCUR_DEFAULT		 0x00000000
#define SET_DATA_SENR_BLEDCUR_DEFAULT	   0x00000000

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Declare setings data type
 * This type is a union of 4 bytes (or 32 bits) to represent any data in this data set.
 * An IEEE-754 float number
 * A byte array of 4 bytes
 * A 32-bit integer
 * The data value is set as a float number and manipulated as a byte array in the flash storage program.
*/

union data_set_t{
	float 		flt;
	uint8_t 	byte[4];
	uint32_t 	word;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Utility Functions
 * These functions act as the accessors of this data set.
*/

// Get the size array of the data block of device information
const uint8_t * getDevSizeInfo(void);

// Get the default value array of device information
const uint8_t ** getDevInfo(void);

// Get the default value array of the 32-bit settings data
const union data_set_t * getOwSetInfo(void);

#endif //__SET_DATATINGS_H__
