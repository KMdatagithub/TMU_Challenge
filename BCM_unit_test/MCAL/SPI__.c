/*
 * SPI__.c
 *
 * Created: 2/10/2020 6:54:24 PM
 *  Author: MENA
 */

#include "SPI__.h"

 void (*PTR_CALL_BACK)(void);
 volatile uint8_t g_u8_RxFlag = TRUE;

extern void _SPIInitMaster(SPI_cfg_s * a_SPI_cfg){

}

extern void _SPIInitSlave(SPI_cfg_s * a_SPI_cfg ){


}

extern void _SPITrancevier(uint8_t* data){


}

extern ERROR_STATUS _SPISend(uint8_t data)
{
	uint8_t state = E_OK;

	return state;
}

extern ERROR_STATUS _SPIRead(uint8_t* data)
{
	uint8_t state = E_OK;

	return state;
}


void SPI_SetCBF(FunPtr isr_ptr)
{

}

ISR(SPI_STC_vect)
{

}


