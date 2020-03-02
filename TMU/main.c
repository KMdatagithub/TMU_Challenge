/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"
#include "util/softwareDelay.h"

void omgplzzz(enum_BcmStatus n){}

int main(void)
{
	//uint8_t rxBuffer[50];
	uint8_t MSG[7] = {1, 2, 2, 5, 5, 3, 8};
	/*-------------[ BCM Initialization ]-------------*/
	BCM_cfg_s BCM1;
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
	BCM_Send(MSG, 7, &BCM1, omgplzzz);

	while (1) 
    {
		BCM_Tx_Dispatcher();
    }
}

