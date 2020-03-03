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
#include "util/registers.h"


BCM_cfg_s BCM1;
uint8_t rxBuffer[50];
//uint8_t Arr[7] = {3, 3, 3, 3, 1, 5, 8};
uint8_t* MSG = (uint8_t*)"GG izi project .. Ma3 Ta7eyat Menna & Khaldoon\r\n";


void txnotify(enum_BcmStatus st){}

void omgplzzz(enum_BcmStatus st)
{
	/* Debug Point */
	TCNT1L = 0xFF;
	/* Debug Point */
	
	BCM_DeInit(&BCM1);
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = UART_Protocol;
	BCM_Init(&BCM1);
	BCM_Send(MSG, 50, &BCM1, txnotify);

}

int main(void)
{
	/*-------------[ BCM Initialization ]-------------*/
	
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Rx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
	
	BCM_Setup_RxBuffer(&BCM1, 20, rxBuffer, omgplzzz);
	
	while (1) 
    {
		BCM_Tx_Dispatcher();
		BCM_Rx_Dispatcher();
		softwareDelayMs(100);
    }
}

