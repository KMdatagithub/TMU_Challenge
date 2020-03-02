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

/* BCM ID */

#define BCM_ID					65

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
	uint8_t* Buffer;
	uint16_t Buf_Len;
	uint16_t Count;
	Notify_FunPtr   BCM_notify_cbf;
}BCM_EXcfg_s;


/*===================================================================================*/
/*--------------------------------[ BCM Static Globals ]-----------------------------*/
/*===================================================================================*/

// BCM_EXcfg_s g_BCM_EXcfg[BCM_MAX_NUM];	    /* Can Be Array In The Future To Support Multiple Instance OF BCM */
//static volatile uint8_t g_BCM_Index = ZERO;

BCM_EXcfg_s g_BCM_EXcfg = {ZERO};
static uint8_t g_RX_Buffer[Rx_Buffer_Size] = {ZERO};
static uint8_t g_RX_Buffer_Index = ZERO;

/*===================================================================================*/
/*----------------------------[ BCM Functions' Definitions ]-------------------------*/
/*===================================================================================*/

/*------------[ BCM CallBacks]------------*/
/* BCM Transmit ISR Call-Back Function */
static void BCM_Tx_ISR_cbf(void)
{
	
	g_BCM_EXcfg.Count++;
	g_BCM_EXcfg.FSM_State = SendComplete_State ;
	
	
}
/* BCM Receive ISR Call-Back Function */
static void BCM_Rx_ISR_cbf(void)
{
	/* LOL */
	g_BCM_EXcfg.Lock_State = Buffer_Locked;
	g_BCM_EXcfg.FSM_State = ReceivingByte_State;
	g_RX_Buffer[g_BCM_EXcfg.Count] = UDR;
	g_BCM_EXcfg.Count++;
}

/*------------------------------------*/
/*---------[ BCM Dispatchers]---------*/
/* RX Dispatcher */
void BCM_RxDispatcher(void)
{
	/* The BCM ID Is Received */
	if(g_BCM_EXcfg.Count == 1)
	{
		if(g_RX_Buffer[0] == BCM_ID)
		{
			g_BCM_EXcfg.FSM_State = ReceiveComplete_State;
		}
		else
		{
			g_RX_Buffer[0] = ZERO;
			g_BCM_EXcfg.Count = ZERO;
			
		}
	}
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
	/* SPI config as well... */
	
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
						//g_BCM_EXcfg.BCM_ISR_cbf = BCM_Tx_ISR_cbf;
						UART_SetTX(BCM_Tx_ISR_cbf);
						break;
					}
					case BCM_Rx_Mode:
					{
						a_BCM_UART.mode = UART_RX;
						a_BCM_UART.uartInterrupts = OnRx;
						/*--------[ Set The RX ISR CallBack Function ]--------*/
						//g_BCM_EXcfg.BCM_ISR_cbf = BCM_Rx_ISR_cbf;
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
	/* Needs So Much Improvements & Error Checking & More... */
	
	//ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	
	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL)
	{
		if(a_Buffer_Len <= Rx_Buffer_Size)
		{
			g_BCM_EXcfg.Buf_Len = a_Buffer_Len;
		}
		else
		{
			/* OMG plzzz */
		}
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		//errorStatus = NULL_PTR + BCM_ERROR;
		//return errorStatus;
	}
	//return errorStatus;
	
	return g_RX_Buffer;
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


ERROR_STATUS BCM_Send(uint8_t* Buffer, uint16_t Buf_Len, BCM_cfg_s* My_BCM, Notify_FunPtr Notify_Ptr ){
	
	/* lock the buffer so user can't chance on it  */
	
	g_BCM_EXcfg.Lock_State = Buffer_Locked ;
	
	/*set the buffer address , length and notification function*/
	
	g_BCM_EXcfg.Buffer = Buffer;
	g_BCM_EXcfg.Buf_Len = Buf_Len;
	g_BCM_EXcfg.BCM_notify_cbf = Notify_Ptr ;
	
	switch(g_BCM_EXcfg.Protocol){
		
		case UART_Protocol :
			UART_Write(g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count]);
		break;
		case  SPI_Protocol :
			_SPITrancevier(& (g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count]));
		break;
		
	}
	g_BCM_EXcfg.FSM_State = SendingByte_State ;	
	return 0 ;
}

void BCM_Tx_Dispatcher(void){
	
	switch(g_BCM_EXcfg.FSM_State){
		case IDLE_State :
		break;
		case SendingByte_State :
		break;
		case SendComplete_State :
			if (g_BCM_EXcfg.Count<g_BCM_EXcfg.Buf_Len)
			{
				switch(g_BCM_EXcfg.Protocol){
					g_BCM_EXcfg.CheckSum+=g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count-1];
					case UART_Protocol :
					UART_Write(g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count]);
					break;
					case  SPI_Protocol :
					_SPITrancevier(& (g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count]));
					break;
				}
				g_BCM_EXcfg.FSM_State = SendingByte_State ;

			}else if (g_BCM_EXcfg.Count == g_BCM_EXcfg.Buf_Len){
				
				switch(g_BCM_EXcfg.Protocol){
					g_BCM_EXcfg.CheckSum+=g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count-1];
					case UART_Protocol :
					UART_Write(g_BCM_EXcfg.CheckSum);
					break;
					case  SPI_Protocol :
					_SPITrancevier(& (g_BCM_EXcfg.CheckSum));
					break;
				}
				g_BCM_EXcfg.FSM_State = SendingByte_State ;		
					
			}else{
				g_BCM_EXcfg.FSM_State = IDLE_State ;	
			}
		break;	
		case OFF_State :
		break;
		
	}
	
	
}