/*
 * BCM.c
 *
 * Created: 3/1/2020 9:30:03 PM
 *  Author: Khaled Magdy
 */ 

#include "BCM.h"
#include "BCM_cfg.h"

/*===================================================================================*/
/*-----------------------------[ BCM Internal Definitions ]--------------------------*/
/*===================================================================================*/

/* Finite State Machine States */

#define OFF_State				0
#define IDLE_State				1

#define SendingByte_State		2
#define SendComplete_State		3

#define ReceivingByte_State		4
#define ReceiveComplete_State	5

/* The BCM Channel Definitions */

#define TX_CH					0
#define RX_CH					1

/*===================================================================================*/
/*---------------------------[ BCM Internal Configurations ]-------------------------*/
/*===================================================================================*/

typedef struct BCM_EXcfg_s{
	uint8_t  Mode;
	uint8_t  BCM_CH_ID;
	uint8_t  Protocol;
	uint8_t  Lock_State;
	uint8_t  FSM_State;
	uint8_t  CheckSum;
	uint16_t Buf_Len;
	uint16_t Count;
	FunPtr   BCM_ISR_cbf;
}BCM_EXcfg_s;


/*===================================================================================*/
/*--------------------------------[ BCM Static Globals ]-----------------------------*/
/*===================================================================================*/

// BCM_EXcfg_s g_BCM_EXcfg[BCM_MAX_NUM];	    /* Can Be Array In The Future To Support Multiple Instance OF BCM */
//static volatile uint8_t g_BCM_Index = ZERO;

BCM_EXcfg_s g_BCM_EXcfg = {0};
static volatile uint8_t g_RX_Buffer[Rx_Buffer_Size] = {0};

/*===================================================================================*/
/*----------------------------[ BCM Functions' Definitions ]-------------------------*/
/*===================================================================================*/

/*------------[ BCM CallBacks]------------*/
/* BCM Transmit ISR Call-Back Function */
static void BCM_Tx_ISR_cbf(void)
{
	/* LOL */
	TCNT2 = 0x05;
}
/* BCM Receive ISR Call-Back Function */
static void BCM_Rx_ISR_cbf(void)
{
	/* LOL */
	
}

/*------------------------------------*/
/*---------[ BCM Dispatchers]---------*/
/* RX Dispatcher */
void BCM_RxDispatcher(void)
{
	
}
/* TX Dispatcher */
void BCM_TxDispatcher(void)
{
	
}
/*------------------------------------*/

/* BCM Initialization Routine */
ERROR_STATUS BCM_Init(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	UART_cfg a_BCM_UART;
	/* spi config as well... */
	
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
						g_BCM_EXcfg.BCM_ISR_cbf = BCM_Tx_ISR_cbf;
						UART_SetTX(BCM_Tx_ISR_cbf);
						break;
					}
					case BCM_Rx_Mode:
					{
						a_BCM_UART.mode = UART_RX;
						a_BCM_UART.uartInterrupts = OnRx;
						/*--------[ Set The RX ISR CallBack Function ]--------*/
						g_BCM_EXcfg.BCM_ISR_cbf = BCM_Rx_ISR_cbf;
						UART_SetRX(BCM_Rx_ISR_cbf);
						break;
					}
					default:
						break;
				}
				/*--------[ Initialize The UART Hardware ]--------*/
				UART_Init(&a_BCM_UART);
				
				errorStatus = BCM_ERROR + E_OK;
				break;
			}
			case SPI_Protocol:
			{
				/* SPI Init Code Goes Here... */
				
				errorStatus = BCM_ERROR + E_OK;
				break;	
			}
			default:
				errorStatus = BCM_ERROR + INVALID_IN;
				return errorStatus;
		}	
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		errorStatus = NULL_PTR + BCM_ERROR;
		return errorStatus;
	}
	return errorStatus;
}




/* BCM Setup RX Buffer */

uint8_t* BCM_Setup_RxBuffer(BCM_cfg_s* a_BCM, uint16_t a_Buffer_Len)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	
	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL)
	{
		
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		errorStatus = NULL_PTR + BCM_ERROR;
		return errorStatus;
	}
	return errorStatus;
}


/* BCM DeInit */
ERROR_STATUS BCM_DeInit(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	
	/*--------[ Search For That BCM CFG In The Working List ]--------*/

	/*--------[ Set That BCM To OFF ]--------*/
	g_BCM_EXcfg.FSM_State = OFF_State;
	
	errorStatus= BCM_ERROR + E_OK;

	return errorStatus;
	
	
}
