/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/BCM.h"
#include "MCAL/UART.h"
#include "util/softwareDelay.h"

/**=========================================================================*
 *								Defines & Globals							*
 *==========================================================================*/

#define TRANSMIT_MAX 255
#define RECEIVE_MAX  255
#define ENTER_KEY	 0x0D

BCM_cfg_s BCM1;
UART_cfg UART1;

uint8_t rxBuffer[RECEIVE_MAX];
uint8_t txBuffer[TRANSMIT_MAX];

volatile uint8_t a_index        = ZERO;
volatile uint8_t msg_len        = ZERO;
volatile uint8_t g_TxBuffer_Len = ZERO;
volatile uint8_t g_UART_TXindex = ZERO;
volatile uint8_t BCM_sending    = FALSE;

/**=========================================================================*
 *							BCM Notification Functions						*
 *==========================================================================*/

/* TX Completion Notification Routine */
void txnotify(enum_BcmStatus st)
{
	BCM_sending = FALSE ;
	g_UART_TXindex = ZERO;
	BCM_DeInit(&BCM1);
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
}
	
/* RX Completion Notification Routine */
void rxnotify(enum_BcmStatus st)
{	
	msg_len = BCM_Get_msgLEN();
	UART_Write(rxBuffer[a_index++]);
}

/**=========================================================================*
 *							UART TX / RX ISR Handlers						*
 *==========================================================================*/

/* UART Reception Completion ISR Handler*/
void UART_ISR_RXcbf(void)
{
	if (BCM_sending==FALSE)
	{
		if(g_UART_TXindex < TRANSMIT_MAX)
		{
			txBuffer[g_UART_TXindex++] = UART_Read();
			if(txBuffer[g_UART_TXindex-1] == ENTER_KEY)
			{
				BCM_Send(txBuffer, g_UART_TXindex, &BCM1, txnotify);
				g_UART_TXindex = ZERO;
			}
		}
		else
		{
			BCM_Send(txBuffer, g_UART_TXindex, &BCM1, txnotify);
			BCM_sending = TRUE;
			g_UART_TXindex = ZERO;	
		}
	}
	
}

/* UART Transmission Completion ISR Handler */
void UART_ISR_TXcbf(void)
{
	if(a_index < msg_len)
	{	
		UART_Write(rxBuffer[a_index++]);	
	}
	else if(a_index == msg_len)
	{
		UART_Write('\n');
		a_index++;
	}
	else
	{
		a_index = ZERO;
	}
}

/**=========================================================================*
 *					BCM Applications For ECU1 & ECU2						*
 *==========================================================================*/

void ECU1_Application(void)
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
	UART_SetRX(UART_ISR_RXcbf);
	UART_Init(&UART1);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1)
	{
		BCM_Tx_Dispatcher();
	}
}

void ECU2_Application(void)
{
	/*-------------[ BCM Initialization ]-------------*/
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Rx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
	BCM_Setup_RxBuffer(&BCM1, RECEIVE_MAX, rxBuffer, rxnotify);
	
	/*-------------[ UART Initialization ]-------------*/
	UART1.baudrate   = 9600;
	UART1.dataBits   = UART_Bits_8;
	UART1.stopBits   = StopBits_1;
	UART1.parity     = No_Parity;
	UART1.sampleEdge = RisingEdg;
	UART1.uartSync   = UART_Async;
	UART1.mode       = UART_TX;
	UART1.uartInterrupts = OnTx;
	UART_SetTX(UART_ISR_TXcbf);
	UART_Init(&UART1);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1)
	{
		BCM_Rx_Dispatcher();
	}
}

/**=========================================================================*
 *								Main Function								*
 *==========================================================================*/

int main(void)
{
	ECU2_Application();
	//ECU1_Application();
}
