/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"


void test_SPI(){
	TCNT1 = 5 ;
	
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
	SPI_cfg_s cfg ;
	cfg.clockSPI = Fosc4 ;
	cfg.phasePolarityMode = mode0 ;
	cfg.dataorder
	while (1) 
    {

    }
}

