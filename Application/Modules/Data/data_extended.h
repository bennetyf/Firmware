/* File Name: data_extended.h
 * Author		: Feng Yuan
 * Time			: 01/08/2017
 * Version	: 1.0
 * Functions:
 * Store the information of the extended data set (indices, the number of different data blocks).
 * Set the default values of these data.
 * Provide accessors to get the default data sizes and data values.
 * Note: All the default values are NOT set in this header file, instead they are organised as a pointer array in the "data_extended.c" file.
*/

#ifndef __DATA_EXTENDED_H__
#define __DATA_EXTENDED_H__

#include <stdint.h>

/** @Macro Extended Data Set Basic Info */
#define EXT_DATA_START_INDEX													 102
#define EXT_DATA_END_INDEX														 115
#define EXT_DATA_NUM																	 EXT_DATA_END_INDEX-EXT_DATA_START_INDEX+1

/** @Macro Manufacturer Data Basic Info */
#define EXT_DATA_MAN_START_INDEX											 102
#define EXT_DATA_MAN_END_INDEX												 104
#define EXT_DATA_MAN_NUM															 EXT_DATA_MAN_END_INDEX-EXT_DATA_MAN_START_INDEX+1

/** @Macro User Data Basic Info */
#define EXT_DATA_USER_START_INDEX											 105
#define EXT_DATA_USER_END_INDEX                        105
#define EXT_DATA_USER_NUM															 EXT_DATA_USER_END_INDEX-EXT_DATA_USER_START_INDEX+1

/** @Macro Reserved Data Basic Info */
#define EXT_DATA_RESERVED_START_INDEX									 106
#define EXT_DATA_RESERVED_END_INDEX										 115
#define EXT_DATA_RESERVED_NUM													 EXT_DATA_RESERVED_END_INDEX-EXT_DATA_RESERVED_START_INDEX+1

/** @Macro Manufacture Data */
#define EXT_DATA_MAN_UNITN_SIZE_BYTE									   4
#define EXT_DATA_MAN_CALIBT_SIZE_BYTE										40
#define EXT_DATA_MAN_UNITT_SIZE_BYTE										40

/** @Macro User Data */
#define	EXT_DATA_USER_FCDT_SIZE_BYTE										40

/** @Macro Reserved Space */
#define EXT_DATA_RESERVED_1_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_2_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_3_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_4_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_5_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_6_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_7_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_8_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_9_SIZE_BYTE										 4
#define EXT_DATA_RESERVED_10_SIZE_BYTE									 4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Accessors for This Data Module*/

/** @Func Get the Sizes and Default Data Values of the Manufacturer Data Set */
const uint8_t * 	getManSizeInfo(void);
const uint8_t ** 	getManInfo(void);

/** @Func Get the Sizes and Default Data Values of the User Data Set */
const uint8_t * 	getUsrSizeInfo(void);
const uint8_t ** 	getUsrInfo(void);

/** @Func Get the Sizes and Default Data Values of the Reserved Data Set */
const uint8_t *   getReservedSizeInfo(void);
const uint8_t **  getReservedInfo(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif //__cplusplus
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //__DATA_EXTENDED_H__
