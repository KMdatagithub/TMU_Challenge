/*
 * BCM.c
 *
 * Created: 3/1/2020 9:30:03 PM
 *  Author: Khaled Magdy
 */ 

#include "BCM.h"
#include "BCM_cfg.h"

/*======================================*/
/*-----[ BCM Internal Definitions ]-----*/
/*======================================*/

/* Finite State Machine States */

#define OFF_State				0
#define IDLE_State				1

#define SendingByte_State		2
#define SendComplete_State		3

#define ReceivingByte_State		4
#define ReceiveComplete_State	5

/*=========================================*/
/*-----[ BCM Internal Configurations ]-----*/
/*=========================================*/

typedef struct BCM_EXcfg_s{
	uint8_t Mode;
	uint8_t BCM_CH_ID;
	uint8_t Protocol;
	uint8_t Lock_State;
	uint8_t FSM_State;
	uint8_t CheckSum;
	uint16_t Buf_Len;
	uint16_t Count;
	FunPtr BCM_ISR_cbf;
}BCM_EXcfg_s;


/*================================*/
/*-----[ BCM Static Globals ]-----*/
/*================================*/

BCM_EXcfg_s g_BCM_EXcfg;	/* Can Be Array In The Future To Support Multiple Instance OF BCM */

/*========================================*/
/*-----[ BCM Functions' Definitions ]-----*/
/*========================================*/

static void BCM_Tx_ISR_cbf(void)
{
	/* LOL */
	TCNT2 = 0x05;
}

static void BCM_Rx_ISR_cbf(void)
{
	/* LOL */
}


ERROR_STATUS BCM_Init(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	UART_cfg a_BCM_UART;
	
	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL)
	{
		/*--------[ Add The BCM CFG To The Working List ]--------*/
		g_BCM_EXcfg.BCM_CH_ID = a_BCM->BCM_CH_ID;
		g_BCM_EXcfg.Mode      = a_BCM->Mode;
		g_BCM_EXcfg.Protocol  = a_BCM->Protocol;
		g_BCM_EXcfg.FSM_State = IDLE_State;
		g_BCM_EXcfg.Count     = ZERO;
		g_BCM_EXcfg.CheckSum  = ZERO;
		
		/*--------[ Check The BCM HW Communication Protocol ]--------*/
		switch(g_BCM_EXcfg.Protocol)
		{
			case UART_Protocol:
			{
				a_BCM_UART.baudrate   = BCM_UART_BaudRate;
				a_BCM_UART.dataBits   = UART_Bits_8;
				a_BCM_UART.stopBits   = StopBits_1;
				a_BCM_UART.parity     = No_Parity;
				a_BCM_UART.sampleEdge = RisingEdg;
				a_BCM_UART.uartSync   = UART_Async;
				
				/*--------[ Check The BCM Mode OF Operation ]--------*/
				switch(g_BCM_EXcfg.Mode)
				{
					case BCM_Tx_Mode:
					{
						a_BCM_UART.mode = UART_TX;
						a_BCM_UART.uartInterrupts = OnTx;
						/*--------[ Set The TX ISR CallBack Function ]--------*/
						UART_SetTX(BCM_Tx_ISR_cbf);
						break;
					}
					case BCM_Rx_Mode:
					{
						a_BCM_UART.mode = UART_RX;
						a_BCM_UART.uartInterrupts = OnRx;
						/*--------[ Set The RX ISR CallBack Function ]--------*/
						UART_SetRX(BCM_Rx_ISR_cbf);
						break;
					}
					default:
						break;
				}
				break;
			}
			case SPI_Protocol:
				break;
			default:
				break;
		}	
		
		/*--------[ Initialize The UART Hardware ]--------*/
		UART_Init(&a_BCM_UART);
		
		errorStatus = BCM_ERROR + E_OK;
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		errorStatus = NULL_PTR + BCM_ERROR;
		return errorStatus;
	}
	return errorStatus;
}






ERROR_STATUS BCM_DeInit(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	/*--------[ Search For That BCM CFG In The Working List ]--------*/
	/* Unimplemented Yet */
	
	/*--------[ Set That BCM To OFF ]--------*/
	
	g_BCM_EXcfg.FSM_State = OFF_State;
	
	return errorStatus;
}
