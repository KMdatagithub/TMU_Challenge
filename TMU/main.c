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

void omgplzzz(enum_BcmStatus n)
{
	//TCNT0 = 5;
}

int main(void)
{
	uint8_t rxBuffer[50];
	//uint8_t Arr[7] = {3, 3, 3, 3, 1, 5, 8};
	//uint8_t* MSG = (uint8_t*)"GG izi project\r\n";
	/*-------------[ BCM Initialization ]-------------*/
	/*
	BCM_cfg_s BCM1;
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Rx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
	*/
	uint8_t data;
	SPI_cfg_s mySPI;
	mySPI.clockSPI = Fosc128;
	mySPI.dataorder = MSB;
	mySPI.phasePolarityMode = mode0;
	_SPIInitMaster(&mySPI);
	
	//softwareDelayMs(2500);
	
	//BCM_Setup_RxBuffer(&BCM1, 20, rxBuffer, omgplzzz);
	//BCM_Send(Arr, 7, &BCM1, omgplzzz);
	
//
	//softwareDelayMs(1500);
	//_SPIRead(&data);
	//
	//
	//TCNT0 = data;
	//softwareDelayMs(1500);
	//_SPIRead(&data);
	//
	//TCNT0 = data;
	//softwareDelayMs(1500);
	//_SPIRead(&data);
	//
	//TCNT0 = data;
	//softwareDelayMs(1500);
	//_SPIRead(&data);
	//
	//TCNT0 = data ;
	//
	while (1) 
    {
		//BCM_Tx_Dispatcher();
		//BCM_Rx_Dispatcher();
	//	_SPIRead(&data);
		TCNT0 = data;
		softwareDelayMs(1500);
		_SPIRead(&data);
    }
}

