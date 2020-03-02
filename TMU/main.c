/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"
#include "MCAL/SPI__.h"
#include "util/softwareDelay.h"

void test_SPI(){
	
	
}



int main(void)
{
	/*-------------[ BCM Initialization ]-------------*/
	/*
	BCM_cfg_s BCM1;
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = UART_Protocol;
	BCM_Init(&BCM1);
    
	UART_Write_String((uint8_t*)"OMG Plzzz Report My Noob Team! \r\n");
	*/
	
	uint8_t test ;
	uint8_t test1 ;
	uint8_t test2 ;


	
	SPI_cfg_s cfg ;
	cfg.clockSPI = Fosc128 ;
	cfg.phasePolarityMode = mode0 ;
	cfg.dataorder = LSB ;
	cfg.ptr_call_back=test_SPI;
	_SPIInitSlave(&cfg);
	softwareDelayMs(1000);
	_SPITrancevier(&test);
		TCNT0 = test ;
		
	softwareDelayMs(1000);
	_SPITrancevier(&test1);
	
		TCNT2 = test1 ;
	softwareDelayMs(1000);
	_SPITrancevier(&test2);
	
	TCNT1L = test2 ;
	while (1) 
    {

    }
}

