/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"
#include "util/softwareDelay.h"

uint8_t rxBuffer[50];
uint8_t g_TxBuffer_Len = ZERO;
uint8_t g_UART_TXindex = ZERO;

	
/* RX Completion Notification Routine */
void omgplzzz(enum_BcmStatus st)
{
	/* Debug Point */
	TCNT1L = 0xFF;
	/* Debug Point */
	
	g_UART_TXindex = ZERO;
	g_TxBuffer_Len = BCM_Get_msgLEN();
	UART_Write(rxBuffer[g_UART_TXindex++]);
}


void UART_ISR_cbf(void)
{
	if(g_UART_TXindex < g_TxBuffer_Len)
	{
		UART_Write(rxBuffer[g_UART_TXindex++]);
	}
}

int main(void)
{
	BCM_cfg_s BCM1;
	UART_cfg UART1;
	/*-------------[ BCM Initialization ]-------------*/
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Rx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
	BCM_Setup_RxBuffer(&BCM1, 40, rxBuffer, omgplzzz);
	
	/*-------------[ UART Initialization ]-------------*/
	UART1.baudrate   = 9600;
	UART1.dataBits   = UART_Bits_8;
	UART1.stopBits   = StopBits_1;
	UART1.parity     = No_Parity;
	UART1.sampleEdge = RisingEdg;
	UART1.uartSync   = UART_Async;
	UART1.mode       = UART_TX;
	UART1.uartInterrupts = OnTx;
	UART_SetTX(UART_ISR_cbf);
	UART_Init(&UART1);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1) 
    {
		BCM_Rx_Dispatcher();
    }
}