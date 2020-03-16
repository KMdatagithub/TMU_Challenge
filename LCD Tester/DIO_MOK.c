/*
 * DIO.c
 *
 * Created: 16-Feb-20 9:49:54 PM
 *  Author: ahmed
 */

/************************************************************************/
/*						INCLUDES                                        */
/************************************************************************/
#include "DIO.h"
#include "std_types.h"


 /************************************************************************/
 /*						APIS                                             */
 /************************************************************************/
 /*
 *Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
 *Output: No output
 *In/Out:
 *Description: This function can set the direction of a full port, a nibble
 * 			  or even one pin.
 */
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info){


	return E_OK;
}

ERROR_STATUS DIO_Write(uint8_t GPIO, uint8_t pins, uint8_t value)
{

	return E_OK;
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data){

	return E_OK;
}
/*
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle(uint8_t GPIO, uint8_t pins)
{

	return E_OK;
}
