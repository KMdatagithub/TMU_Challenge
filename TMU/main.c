/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"
#include "util/softwareDelay.h"
#include "util/registers.h"



int main(void)
{
	uint8_t* rxBuffer;
	/*-------------[ BCM Initialization ]-------------*/
	BCM_cfg_s BCM1;
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Rx_Mode;
	BCM1.Protocol = UART_Protocol;
	BCM_Init(&BCM1);
    rxBuffer = BCM_Setup_RxBuffer(&BCM1, 1);
	
	//UART_Write_String((uint8_t*)"OMG Plzzz Report My Noob Team! \r\n");
	
	while (1) 
    {
		softwareDelayMs(5000);
		TCNT0 = rxBuffer[0];
    }
}

