/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"
#include "util/softwareDelay.h"


BCM_cfg_s BCM1;
UART_cfg UART1;
uint8_t txBuffer[50];
volatile uint8_t g_TxBuffer_Len = ZERO;
volatile uint8_t g_UART_TXindex = ZERO;

	
/* TX Completion Notification Routine */
void omgplzzz(enum_BcmStatus st)
{
	/* Debug Point */
	TCNT1L = 0xFF;
	/* Debug Point */
	
	g_UART_TXindex = ZERO;
	BCM_DeInit(&BCM1);
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
}


void UART_ISR_cbf(void)
{
	txBuffer[g_UART_TXindex++] = UART_Read();
	if(txBuffer[g_UART_TXindex-1] == 0x0D)
	{
		BCM_Send(txBuffer, g_UART_TXindex, &BCM1, omgplzzz);
	}
}

int main(void)
{
	/*-------------[ BCM Initialization ]-------------*/
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
		
	/*-------------[ UART Initialization ]-------------*/
	UART1.baudrate   = 9600;
	UART1.dataBits   = UART_Bits_8;
	UART1.stopBits   = StopBits_1;
	UART1.parity     = No_Parity;
	UART1.sampleEdge = RisingEdg;
	UART1.uartSync   = UART_Async;
	UART1.mode       = UART_RX;
	UART1.uartInterrupts = OnRx;
	UART_SetRX(UART_ISR_cbf);
	UART_Init(&UART1);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1) 
    {
		BCM_Tx_Dispatcher();
    }
}
