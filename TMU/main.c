/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"





int main(void)
{
	/*-------------[ BCM Initialization ]-------------*/
	BCM_cfg_s BCM1;
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = UART_Protocol;
	BCM_Init(&BCM1);
    
	UART_Write_String("OMG Plzzz Report My Noob Team! \r\n");
	
	while (1) 
    {
		
    }
}

