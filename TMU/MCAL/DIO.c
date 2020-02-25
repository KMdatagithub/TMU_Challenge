/*
 * DIO.c
 *
 * Created: 2/15/2020 9:32:21 PM
 *  Author: Khaled Magdy
 */ 

#include "DIO.h"
#include "../util/registers.h"

/***********************************************************************
*							Functions' Definitions                     *
***********************************************************************/

/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	ERROR_STATUS errorStatus = E_NOK;
	if(DIO_info != NULL)
	{
		switch(DIO_info->GPIO)
		{
			case GPIOA :
			PORTA_DIR = (DIO_info->dir & DIO_info->pins) | (~(DIO_info->pins) & PORTA_DIR);
			errorStatus = E_OK;
			break;
			case GPIOB :
			PORTB_DIR = (DIO_info->dir & DIO_info->pins) | (~(DIO_info->pins) & PORTB_DIR);
			errorStatus = E_OK;
			break;
			case GPIOC :
			PORTC_DIR = (DIO_info->dir & DIO_info->pins) | (~(DIO_info->pins) & PORTC_DIR);
			errorStatus = E_OK;
			break;
			case GPIOD :
			PORTD_DIR = (DIO_info->dir & DIO_info->pins) | (~(DIO_info->pins) & PORTD_DIR);
			errorStatus = E_OK;
			break;
			default :
			errorStatus = E_NOK;
			break;
		}
	}
	else
	{
		errorStatus = E_NOK;
	}
	return errorStatus;
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
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write(uint8_t GPIO, uint8_t pins, uint8_t value)
{
	ERROR_STATUS errorStatus = E_NOK;
	switch(GPIO)
	{
		case GPIOA :
		PORTA_DATA = (pins & value) | (~pins & PORTA_DATA);
		errorStatus = E_OK;
		break;
		case GPIOB :
		PORTB_DATA = (pins & value) | (~pins & PORTB_DATA);
		errorStatus = E_OK;
		break;
		case GPIOC :
		PORTC_DATA = (pins & value) | (~pins & PORTC_DATA);
		errorStatus = E_OK;
		break;
		case GPIOD :
		PORTD_DATA = (pins & value) | (~pins & PORTD_DATA);
		errorStatus = E_OK;
		break;
		default :
		errorStatus = E_NOK;
		break;
	}
	return errorStatus;
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
ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data)
{

	ERROR_STATUS errorStatus = E_NOK;
	if(data != NULL)
	{
		switch(GPIO)
		{
			case GPIOA :
			*data = (PORTA_PIN & pins);
			errorStatus = E_OK;
			break;
			case GPIOB :
			*data = (PORTB_PIN & pins);
			errorStatus = E_OK;
			break;
			case GPIOC :
			*data = (PORTC_PIN & pins);
			errorStatus = E_OK;
			break;
			case GPIOD :
			*data = (PORTD_PIN & pins);
			errorStatus = E_OK;
			break;
			default :
			errorStatus = E_NOK;
			break;
		}
	}
	else
	{
		errorStatus = E_NOK;
	}
	return errorStatus;
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
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle(uint8_t GPIO, uint8_t pins)
{
	ERROR_STATUS errorStatus = E_NOK;
	switch(GPIO)
	{
		case GPIOA :
		PORTA_DATA = (pins & (~PORTA_DATA)) | (~pins & PORTA_DATA);
		errorStatus = E_OK;
		break;
		case GPIOB :
		PORTB_DATA = (pins & (~PORTB_DATA)) | (~pins & PORTB_DATA);
		errorStatus = E_OK;
		break;
		case GPIOC :
		PORTC_DATA = (pins & (~PORTC_DATA)) | (~pins & PORTC_DATA);
		errorStatus = E_OK;
		break;
		case GPIOD :
		PORTD_DATA = (pins & (~PORTD_DATA)) | (~pins & PORTD_DATA);
		errorStatus = E_OK;
		break;
		default :
		errorStatus = E_NOK;
		break;
	}
	return errorStatus;
}
