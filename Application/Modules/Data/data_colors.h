/* File Name: data_colors.h
 * Author		: Feng Yuan
 * Time			: 01/08/2017
 * Version	: 1.0
 * Functions:
 * Store the information of the color data set (indices, the number of different data blocks).
 * Set the default values of these data.
 * Provide an accessor to get the default values.
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DATA_COLORS_H__
#define __DATA_COLORS_H__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Macro Color Data Basic Info(Temperature data is also included) */
#define COLOR_DATA_START_INDEX											30
#define COLOR_DATA_END_INDEX											  101
#define COLOR_DATA_NUM															COLOR_DATA_END_INDEX-COLOR_DATA_START_INDEX+1

#define COLOR_DATA_MOTHER_START_INDEX       				30
#define	COLOR_DATA_MOTHER_END_INDEX									62
#define COLOR_DATA_MOTHER_NUM												COLOR_DATA_MOTHER_END_INDEX-COLOR_DATA_MOTHER_START_INDEX+1	

#define COLOR_DATA_FACTORY_START_INDEX      				63
#define COLOR_DATA_FACTORY_END_INDEX								80
#define COLOR_DATA_FACTORY_NUM											COLOR_DATA_FACTORY_END_INDEX-COLOR_DATA_FACTORY_START_INDEX+1

#define COLOR_DATA_GREYSCALE_FACTORY_START_INDEX   	81
#define COLOR_DATA_GREYSCALE_FACTORY_END_INDEX			89
#define COLOR_DATA_GREYSCALE_FACTORY_NUM						COLOR_DATA_GREYSCALE_FACTORY_END_INDEX-COLOR_DATA_GREYSCALE_FACTORY_START_INDEX+1

#define COLOR_DATA_TEMP_START_INDEX									90
#define COLOR_DATA_TEMP_END_INDEX										91
#define COLOR_DATA_TEMP_NUM													COLOR_DATA_TEMP_END_INDEX-COLOR_DATA_TEMP_START_INDEX+1

#define COLOR_DATA_GREYSCALE_USER_START_INDEX      	92
#define COLOR_DATA_GREYSCALE_USER_END_INDEX					100
#define COLOR_DATA_GREYSCALE_USER_NUM								COLOR_DATA_GREYSCALE_USER_END_INDEX-COLOR_DATA_GREYSCALE_USER_START_INDEX+1

#define COLOR_DATA_TEMP_USER_START_INDEX						101
#define COLOR_DATA_TEMP_USER_END_INDEX							101
#define COLOR_DATA_TEMP_USER_NUM										COLOR_DATA_TEMP_USER_END_INDEX-COLOR_DATA_TEMP_USER_START_INDEX+1

#define COLOR_DATA_TOTAL_NUM												COLOR_DATA_MOTHER_NUM  \
																									+	COLOR_DATA_FACTORY_NUM \
																									+	COLOR_DATA_GREYSCALE_FACTORY_NUM \
																									+	COLOR_DATA_GREYSCALE_USER_NUM \
																									+	COLOR_DATA_TEMP_NUM \
																									+	COLOR_DATA_TEMP_USER_NUM

/** @Macro Default Values of this Data Set
	* All default values of this data set are set here.
	* You don't need to change anything in the "data_colors.c" file to change the default values.
*/

/** @Macro Color Mother */
#define COLOR_DATA_MOTHER_0_DEFAULT         				7.7585
#define COLOR_DATA_MOTHER_1_DEFAULT         				0.0
#define COLOR_DATA_MOTHER_2_DEFAULT         				0.0
#define COLOR_DATA_MOTHER_3_DEFAULT         				28.3045
#define COLOR_DATA_MOTHER_4_DEFAULT         				4.2323
#define COLOR_DATA_MOTHER_5_DEFAULT         				-14.6471
#define COLOR_DATA_MOTHER_6_DEFAULT         				0.0
#define COLOR_DATA_MOTHER_7_DEFAULT         				-7.4425
#define COLOR_DATA_MOTHER_8_DEFAULT         				3.0063
#define COLOR_DATA_MOTHER_9_DEFAULT         				7.7208
#define COLOR_DATA_MOTHER_10_DEFAULT        				0.0
#define COLOR_DATA_MOTHER_11_DEFAULT        				0.0
#define COLOR_DATA_MOTHER_12_DEFAULT        				45.2001
#define COLOR_DATA_MOTHER_13_DEFAULT        				9.0369
#define COLOR_DATA_MOTHER_14_DEFAULT        				-8.2718
#define COLOR_DATA_MOTHER_15_DEFAULT        				0.0
#define COLOR_DATA_MOTHER_16_DEFAULT        				-10.9677
#define COLOR_DATA_MOTHER_17_DEFAULT        				3.7684
#define COLOR_DATA_MOTHER_18_DEFAULT        				0.1817
#define COLOR_DATA_MOTHER_19_DEFAULT        				0.0
#define COLOR_DATA_MOTHER_20_DEFAULT        				0.0
#define COLOR_DATA_MOTHER_21_DEFAULT        				-4.2816
#define COLOR_DATA_MOTHER_22_DEFAULT        				-7.2218
#define COLOR_DATA_MOTHER_23_DEFAULT        				28.0945
#define COLOR_DATA_MOTHER_24_DEFAULT        				0.0
#define COLOR_DATA_MOTHER_25_DEFAULT       					-19.3509
#define COLOR_DATA_MOTHER_26_DEFAULT        				15.1156
#define COLOR_DATA_MOTHER_27_DEFAULT        				4234.2494
#define COLOR_DATA_MOTHER_28_DEFAULT        				53965.9691
#define COLOR_DATA_MOTHER_29_DEFAULT        				8225.8053
#define COLOR_DATA_MOTHER_30_DEFAULT        				101305.0207
#define COLOR_DATA_MOTHER_31_DEFAULT        				9817.7002
#define COLOR_DATA_MOTHER_32_DEFAULT        				136506.6339

/** @Macro Color Factory */
#define COLOR_DATA_FACTORY_0_DEFAULT        				4838.6179
#define COLOR_DATA_FACTORY_1_DEFAULT        				205.1801
#define COLOR_DATA_FACTORY_2_DEFAULT        				353.4113
#define COLOR_DATA_FACTORY_3_DEFAULT        				828.6841
#define COLOR_DATA_FACTORY_4_DEFAULT        				4726.4440
#define COLOR_DATA_FACTORY_5_DEFAULT        				1065.9442
#define COLOR_DATA_FACTORY_6_DEFAULT        				242.7979
#define COLOR_DATA_FACTORY_7_DEFAULT        				2005.3778
#define COLOR_DATA_FACTORY_8_DEFAULT        				8080.1176
#define COLOR_DATA_FACTORY_9_DEFAULT        				2.2685
#define COLOR_DATA_FACTORY_10_DEFAULT       				0.9983
#define COLOR_DATA_FACTORY_11_DEFAULT       				1.0014
#define COLOR_DATA_FACTORY_12_DEFAULT       				1.5041
#define COLOR_DATA_FACTORY_13_DEFAULT       				1.4465
#define COLOR_DATA_FACTORY_14_DEFAULT       				0.9148
#define COLOR_DATA_FACTORY_15_DEFAULT       				1.4414
#define COLOR_DATA_FACTORY_16_DEFAULT       				0.8635
#define COLOR_DATA_FACTORY_17_DEFAULT       				1.2148

/** @Macro Color Greyscale Factory */
#define COLOR_DATA_GREYSCALE_FACTORY_0_DEFAULT     	13470.0
#define COLOR_DATA_GREYSCALE_FACTORY_1_DEFAULT     	567.0
#define COLOR_DATA_GREYSCALE_FACTORY_2_DEFAULT     	978.0
#define COLOR_DATA_GREYSCALE_FACTORY_3_DEFAULT     	2385.0
#define COLOR_DATA_GREYSCALE_FACTORY_4_DEFAULT     	13904.0
#define COLOR_DATA_GREYSCALE_FACTORY_5_DEFAULT     	3115.0
#define COLOR_DATA_GREYSCALE_FACTORY_6_DEFAULT     	691.0
#define COLOR_DATA_GREYSCALE_FACTORY_7_DEFAULT     	5997.0
#define COLOR_DATA_GREYSCALE_FACTORY_8_DEFAULT     	24399.0

/** @Macro Temperature Data */
#define COLOR_DATA_TEMP_INIT_DEFAULT							 	0x00000000
#define COLOR_DATA_TEMP_FINAL_DEFAULT							 	0x00000000

/** @Macro Color Greyscale User */
#define COLOR_DATA_GREYSCALE_USER_0_DEFAULT        	13470.0
#define COLOR_DATA_GREYSCALE_USER_1_DEFAULT        	567.0
#define COLOR_DATA_GREYSCALE_USER_2_DEFAULT        	978.0
#define COLOR_DATA_GREYSCALE_USER_3_DEFAULT        	2385.0
#define COLOR_DATA_GREYSCALE_USER_4_DEFAULT        	13904.0
#define COLOR_DATA_GREYSCALE_USER_5_DEFAULT        	3115.0
#define COLOR_DATA_GREYSCALE_USER_6_DEFAULT        	691.0
#define COLOR_DATA_GREYSCALE_USER_7_DEFAULT        	5997.0
#define COLOR_DATA_GREYSCALE_USER_8_DEFAULT        	24399.0

/** @Macro Temperature Data (User) */
#define COLOR_DATA_TEMP_USER_DEFAULT							 	0x00000000

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @Type Color Data Type 
	* This type is a union of 4 bytes (or 32 bits) to represent any data in this data set.
	* An IEEE-754 float number
	* A byte array of 4 bytes
	* A 32-bit integer
	* The data value is set as a float number and manipulated as a byte array in the flash storage program.
*/
union data_color_t{
    float 		flt;
    uint8_t  	byte[4];
		uint32_t 	word;
};

/** @Func Get default data
	* This function acts as the accessor of this data set.
	* It returns the address of the array containing all the default values.
	* This array is defined in the "data_colors.c" file.
*/
const union data_color_t* getColorInfo(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif //__cplusplus
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DATA_COLORS_H__
