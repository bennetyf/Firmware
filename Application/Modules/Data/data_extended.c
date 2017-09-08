/** File Name: data_extended.c
	*
	* @Author		: Feng Yuan
	* @Time			: 01/08/2017
	* @Version	: 1.0
	* @Brief
	* Store the information of the extended data set (indices, the number of different data blocks).
	* Set the default values of these data.
	* Provide accessors to get the default data sizes and data values.
	* Note: All default values of this data set are organised as a pointer array in the "data_extended.c" file.
*/

#include "data_extended.h"

/* Default Manufacturer Data */
// Data Sizes
const static uint8_t default_man_size[EXT_DATA_MAN_NUM]=
{
	EXT_DATA_MAN_UNITN_SIZE_BYTE,
	EXT_DATA_MAN_CALIBT_SIZE_BYTE,
	EXT_DATA_MAN_UNITT_SIZE_BYTE
};

// Default Data Values in Each Data Type
const static uint8_t default_man_unitn[EXT_DATA_MAN_UNITN_SIZE_BYTE]={0x00};
const static uint8_t default_man_calibt[EXT_DATA_MAN_CALIBT_SIZE_BYTE]={0x00};
const static uint8_t default_man_unitt[EXT_DATA_MAN_UNITT_SIZE_BYTE]={0x00};

// The pointer array to store the default values of the manufacturer data set
const static uint8_t * default_man[EXT_DATA_MAN_NUM]=
{
	default_man_unitn,
	default_man_calibt,
	default_man_unitt
};

/* Default User Data Set */
const static uint8_t default_usr_size[EXT_DATA_USER_NUM]=
{
	EXT_DATA_USER_FCDT_SIZE_BYTE
};

// Default Data Values in Each Data Type
const static uint8_t default_usr_fcdt[EXT_DATA_USER_FCDT_SIZE_BYTE]={0x00};

// The pointer array to store the default values of the user data set
const static uint8_t * default_usr[EXT_DATA_USER_NUM]=
{
	default_usr_fcdt
};

/* Default Reserved Data Set */
const static uint8_t default_reserved_size[EXT_DATA_RESERVED_NUM]=
{
	EXT_DATA_RESERVED_1_SIZE_BYTE,
	EXT_DATA_RESERVED_2_SIZE_BYTE,
	EXT_DATA_RESERVED_3_SIZE_BYTE,
	EXT_DATA_RESERVED_4_SIZE_BYTE,
	EXT_DATA_RESERVED_5_SIZE_BYTE,
	EXT_DATA_RESERVED_6_SIZE_BYTE,
	EXT_DATA_RESERVED_7_SIZE_BYTE,
	EXT_DATA_RESERVED_8_SIZE_BYTE,
	EXT_DATA_RESERVED_9_SIZE_BYTE,
	EXT_DATA_RESERVED_10_SIZE_BYTE
};

// Default Data Values in Each Data Type
const static uint8_t default_reserved_1[EXT_DATA_RESERVED_1_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_2[EXT_DATA_RESERVED_2_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_3[EXT_DATA_RESERVED_3_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_4[EXT_DATA_RESERVED_4_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_5[EXT_DATA_RESERVED_5_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_6[EXT_DATA_RESERVED_6_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_7[EXT_DATA_RESERVED_7_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_8[EXT_DATA_RESERVED_8_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_9[EXT_DATA_RESERVED_9_SIZE_BYTE]={0x00};
const static uint8_t default_reserved_10[EXT_DATA_RESERVED_10_SIZE_BYTE]={0x00};

// The pointer array to store the default values of the user data set
const static uint8_t * default_reserved[EXT_DATA_RESERVED_NUM]=
{
	default_reserved_1,
	default_reserved_2,
	default_reserved_3,
	default_reserved_4,
	default_reserved_5,
	default_reserved_6,
	default_reserved_7,
	default_reserved_8,
	default_reserved_9,
	default_reserved_10
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Accessor functions in this data set*/

// Get the Sizes and Default Values of the Manufacturer Data
const uint8_t * getManSizeInfo(void)
{
	return default_man_size;
}

const uint8_t ** getManInfo(void)
{
	return default_man;
}

// Get the Sizes and Default Values of the User Data
const uint8_t * getUsrSizeInfo(void)
{
	return default_usr_size;
}

const uint8_t ** getUsrInfo(void)
{
	return default_usr;
}

// Get the Sizes and Default Values of the Reserved Data
const uint8_t * getReservedSizeInfo(void)
{
	return default_reserved_size;
}

const uint8_t ** getReservedInfo(void)
{
	return default_reserved;
}
