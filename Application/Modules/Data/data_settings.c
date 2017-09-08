/** File Name: data_settings.c
	*
	* @Author		: Feng Yuan
	* @Time			: 01/08/2017
	* @Version	: 1.0
	* @Brief
	*
	* Store the information of the settings data set (indices, the number of different data blocks).
	* Set the default values of these data.
	* Provide accessors to get the default data sizes and data values.
	*
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DATA_SETTINGS_H__
#define __DATA_SETTINGS_H__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "data_settings.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/** @Variable Default Values of Device Information */
	
// 32 bits (4 bytes)
const static uint8_t default_devinfo_vmi[SET_DATA_DEVINFO_VMI_SIZE_BYTE]={0x00};
// 64 bits (8 bytes)
const static uint8_t default_devinfo_sn[SET_DATA_DEVINFO_SN_SIZE_BYTE]={0x00};
// 48 bits (6 bytes)
const static uint8_t default_devinfo_mac[SET_DATA_DEVINFO_MAC_SIZE_BYTE]={0x00};
// 64 bits (8 bytes)
const static uint8_t default_devinfo_name[SET_DATA_DEVINFO_NAME_SIZE_BYTE]={0x00};
// 64 bits (8 bytes)
const static uint8_t default_devinfo_modn[SET_DATA_DEVINFO_MODN_SIZE_BYTE]={0x00};
// 64 bits (8 bytes)
const static uint8_t default_devinfo_mann[SET_DATA_DEVINFO_MANN_SIZE_BYTE]={0x00};
// 64 bits (8 bytes)
const static uint8_t default_devinfo_hr[SET_DATA_DEVINFO_HR_SIZE_BYTE]={0x00};
// 64 bits (8 bytes)
const static uint8_t default_devinfo_fr[SET_DATA_DEVINFO_FR_SIZE_BYTE]={0x00};
// 160 bits (40 bytes)
const static uint8_t default_devinfo_fcdt[SET_DATA_DEVINFO_FCDT_SIZE_BYTE]={0x00};

/** @Variable Organise the above arrays into a pointer array */
const static uint8_t * default_devinfo[SET_DATA_DEVINFO_NUM]=
{
	default_devinfo_vmi,
	default_devinfo_sn,
	default_devinfo_mac,
	default_devinfo_name,
	default_devinfo_modn,
	default_devinfo_mann,
	default_devinfo_hr,
	default_devinfo_fr,
	default_devinfo_fcdt
};

/** @Variable Size Values of Device Information */
const static uint8_t default_devinfo_size[SET_DATA_NUM]=
{
	SET_DATA_DEVINFO_VMI_SIZE_BYTE,
	SET_DATA_DEVINFO_SN_SIZE_BYTE,
	SET_DATA_DEVINFO_MAC_SIZE_BYTE,
	SET_DATA_DEVINFO_NAME_SIZE_BYTE,
	SET_DATA_DEVINFO_MODN_SIZE_BYTE,
	SET_DATA_DEVINFO_MANN_SIZE_BYTE,
	SET_DATA_DEVINFO_HR_SIZE_BYTE,
	SET_DATA_DEVINFO_FR_SIZE_BYTE,
	SET_DATA_DEVINFO_FCDT_SIZE_BYTE,
};

/** @Variable Default Values of 4-byte Settings Data */
const static union data_set_t default_oneword_settings[SET_DATA_ONE_WORD_NUM]=
{
	// DFU System Information
	SET_DATA_DFU_APPVER_DEFAULT,
	SET_DATA_DFU_BLVER_DEFAULT,
	SET_DATA_DFU_SDVER_DEFAULT,
	SET_DATA_DFU_HDVER_DEFAULT,
	
	// Device Settings
	SET_DATA_DEVSET_IDLE_DEFAULT,
	SET_DATA_DEVSET_REGC_DEFAULT,
	SET_DATA_DEVSET_BLSEL_DEFAULT,
	
	// System Status
	SET_DATA_SYS_MEM_DEFAULT,
	SET_DATA_SYS_CALIB_DEFAULT,
	SET_DATA_SYS_POWNUM_DEFAULT,
	SET_DATA_SYS_CHRNUM_DEFAULT,
	
	// Color Sensor Settings
	SET_DATA_SENR_GSEL_DEFAULT,
	SET_DATA_SENR_INTMOD_DEFAULT,
	SET_DATA_SENR_INTSET_DEFAULT,
	SET_DATA_SENR_MANT_DEFAULT,
	SET_DATA_SENR_DCMOD_DEFAULT,
	SET_DATA_SENR_OTMOD_DEFAULT,
	SET_DATA_SENR_RLEDCUR_DEFAULT,
	SET_DATA_SENR_GLEDCUR_DEFAULT,
	SET_DATA_SENR_BLEDCUR_DEFAULT
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*	Utility Functions
 * These functions act as the accessors of this data set.
*/

/** @Func Get the size array of the data block of device information */
const uint8_t * getDevSizeInfo(void)
{
	return default_devinfo_size;
}

/** @Func Get the default value array of device information */
const uint8_t ** getDevInfo(void)
{
	return default_devinfo;
}

/** @Func Get the default value array of the 32-bit settings data */
const union data_set_t * getOwSetInfo(void)
{
	return default_oneword_settings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif //__cplusplus
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //__DATA_SETTINGS_H__
