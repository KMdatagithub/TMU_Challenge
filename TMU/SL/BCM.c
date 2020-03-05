/*
 * BCM.c
 *
 * Created: 3/1/2020 9:30:03 PM
 *  Author: Khaled Magdy
 */ 

#include "BCM.h"
#include "BCM_cfg.h"
#include "../MCAL/UART.h"
#include "../MCAL/SPI__.h"


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

#define S_NOK					0
#define S_OK					1


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
 	uint16_t MSG_Len;
	uint16_t Count;
	
	Notify_FunPtr BCM_notify_cbf;
}BCM_EXcfg_s;


/*===================================================================================*/
/*--------------------------------[ BCM Static Globals ]-----------------------------*/
/*===================================================================================*/

// BCM_EXcfg_s g_BCM_EXcfg[BCM_MAX_NUM];	    /* Can Be Array In The Future To Support Multiple Instance OF BCM */
//static volatile uint8_t g_BCM_Index = ZERO;

BCM_EXcfg_s g_BCM_EXcfg = {ZERO};
volatile static uint8_t g_RX_Buffer_Index = ZERO;
volatile static uint8_t g_Rx_ID = ZERO;
volatile static uint8_t g_Tx_ID = ZERO;
volatile static uint8_t g_ID_State  = S_NOK;
volatile static uint8_t g_LEN_State = S_NOK;
volatile static uint8_t g_CS_Calculated = ZERO;

/* BCM Static Functions */
static ERROR_STATUS BCM_TxUnlock(BCM_EXcfg_s* a_exBCM);
static ERROR_STATUS BCM_Buffer_Lock(BCM_EXcfg_s* a_exBCM);

/*===================================================================================*/
/*----------------------------[ BCM Functions' Definitions ]-------------------------*/
/*===================================================================================*/

/*------------[ BCM CallBacks]------------*/
/* BCM Transmit ISR Call-Back Function */

static void BCM_Tx_ISR_cbf(void)
{	if (g_BCM_EXcfg.FSM_State == SendingByte_State)
	{
		g_BCM_EXcfg.Count++;
		g_BCM_EXcfg.FSM_State = SendComplete_State;
	}
}

/* BCM Receive ISR Call-Back Function */
static void BCM_Rx_ISR_cbf(void)
{
	uint8_t a_RX_Byte;
	switch(g_BCM_EXcfg.Protocol)
	{
		case UART_Protocol:
		{
			a_RX_Byte = UART_Read();
			break;
		}
		case SPI_Protocol:
		{
			_SPIRead(&a_RX_Byte);
			break;
		}
		default:
			break;
	}
	
	
	/* Read & Check The BCM Received ID */
	if(g_BCM_EXcfg.Count == 0)
	{
		g_Rx_ID = a_RX_Byte;
	
		if(g_Rx_ID == BCM_ID)
		{
			
			g_ID_State = S_OK;
			g_BCM_EXcfg.Count++;
		}
		else
		{
			g_ID_State = S_NOK;
		}	
	}
	/*-------------------[ ID IS CHECKED ]------------------*/
	/*------------------------------------------------------*/
	/*-------------------[ Save & CHECK MSG Len ]------------------*/
	/* Read & Check The BCM Received Data Length */
	else if(g_BCM_EXcfg.Count == 1)
	{
		if(a_RX_Byte > g_BCM_EXcfg.Buf_Len)
		{
			g_LEN_State = S_NOK;
			g_BCM_EXcfg.FSM_State = IDLE_State;
			g_BCM_EXcfg.Count = ZERO;
			g_BCM_EXcfg.BCM_notify_cbf(g_LEN_State);
		}
		else
		{
			g_BCM_EXcfg.MSG_Len = a_RX_Byte;
			g_BCM_EXcfg.Count++;
			g_BCM_EXcfg.FSM_State = ReceivingByte_State;
			BCM_Buffer_Lock(&g_BCM_EXcfg);
		}
	}
	else if(g_BCM_EXcfg.Count-2 < g_BCM_EXcfg.MSG_Len)
	{
		g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count-2] = a_RX_Byte;
		g_CS_Calculated += a_RX_Byte;
		g_BCM_EXcfg.Count++;
	}
	else if(g_BCM_EXcfg.Count-2 == g_BCM_EXcfg.MSG_Len)
	{
		g_BCM_EXcfg.CheckSum = a_RX_Byte;
		g_BCM_EXcfg.Count++;
		g_BCM_EXcfg.FSM_State = ReceiveComplete_State;
	}
	else
	{	}
}

/*------------------------------------*/
/*---------[ BCM Dispatchers]---------*/

/* RX Dispatcher */
void BCM_Rx_Dispatcher(void)
{
	if(g_BCM_EXcfg.FSM_State == ReceiveComplete_State)
	{	
		if(g_BCM_EXcfg.CheckSum == g_CS_Calculated)
		{
		
			g_BCM_EXcfg.Count = ZERO;
			g_CS_Calculated = ZERO;
			g_BCM_EXcfg.CheckSum = ZERO;
			g_BCM_EXcfg.FSM_State = IDLE_State;
			g_BCM_EXcfg.BCM_notify_cbf(E_OK);
		}
	}
}

/* TX Dispatcher */
void BCM_Tx_Dispatcher(void)
{	
	switch(g_BCM_EXcfg.FSM_State)
	{
		case IDLE_State :
			break;
		case SendingByte_State :
			break;
		case SendComplete_State :
		if(g_BCM_EXcfg.Count == 1)
		{
			switch(g_BCM_EXcfg.Protocol){
				case UART_Protocol :
				UART_Write(g_BCM_EXcfg.Buf_Len);
				break;
				case  SPI_Protocol :
				_SPISend(g_BCM_EXcfg.Buf_Len);
				break;
			}
			g_BCM_EXcfg.FSM_State = SendingByte_State ;
			
		}
		else if (g_BCM_EXcfg.Count <= g_BCM_EXcfg.Buf_Len+1)
		{
			g_BCM_EXcfg.CheckSum +=g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count-2];
			
			switch(g_BCM_EXcfg.Protocol){
				case UART_Protocol :
				UART_Write(g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count-2]);
				break;
				case  SPI_Protocol :
				_SPISend(g_BCM_EXcfg.Buffer[g_BCM_EXcfg.Count-2]);	
				break;
			}
			g_BCM_EXcfg.FSM_State = SendingByte_State ;

			}else if (g_BCM_EXcfg.Count == g_BCM_EXcfg.Buf_Len+2){
			switch(g_BCM_EXcfg.Protocol){
				
				case UART_Protocol :
				UART_Write(g_BCM_EXcfg.CheckSum);
				break;
				case  SPI_Protocol :
				_SPISend(g_BCM_EXcfg.CheckSum);
				break;
			}
			g_BCM_EXcfg.FSM_State = SendingByte_State ;
			
			}else{
				BCM_TxUnlock(&g_BCM_EXcfg);
				g_BCM_EXcfg.BCM_notify_cbf(10);
			g_BCM_EXcfg.FSM_State = IDLE_State ;
		}
		break;
		case OFF_State :
		break;
	}	
}
/*------------------------------------*/

/* BCM Initialization Routine */

/**
 * Input: Pointer to a structure contains the information needed to initialize the BCM 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the Basic Communication Module
 * 							
 */


ERROR_STATUS BCM_Init(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	UART_cfg  a_BCM_UART;
	SPI_cfg_s a_BCM_SPI; 
	
	if (g_BCM_EXcfg.FSM_State==IDLE_State)
	{
		 errorStatus =  BCM_ERROR + ALREADY_INIT ;
		 return errorStatus ;
	}
	
	
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
		g_BCM_EXcfg.Buf_Len   = ZERO;
		g_BCM_EXcfg.MSG_Len   = ZERO;
		
		/*--------[ Check The BCM HW Communication Protocol ]--------*/
		switch(g_BCM_EXcfg.Protocol)
		{
			/*----------------------[ UART CASE START ]---------------------*/
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
				/*--------[ Initialize The UART Hardware ]--------*/
				UART_Init(&a_BCM_UART);
				
				errorStatus = BCM_ERROR + E_OK;
				break;
			}
			/*----------------------[ UART CASE END ]---------------------*/
			/*------------------------------------------------------------*/
			/*----------------------[ SPI CASE START ]--------------------*/
			case SPI_Protocol:
			{
				a_BCM_SPI.clockSPI = Fosc128;
				a_BCM_SPI.dataorder = MSB;
				a_BCM_SPI.phasePolarityMode = mode0;
				
				/*--------[ Check The BCM Mode OF Operation ]--------*/
				switch(g_BCM_EXcfg.Mode)
				{
					case BCM_Tx_Mode:
					{
						a_BCM_SPI.ptr_call_back = BCM_Tx_ISR_cbf;
						_SPIInitMaster(&a_BCM_SPI);
						/*--------[ Set The TX ISR CallBack Function ]--------*/
						SPI_SetCBF(BCM_Tx_ISR_cbf);
						break;
					}
					case BCM_Rx_Mode:
					{
						a_BCM_SPI.ptr_call_back = BCM_Rx_ISR_cbf;
						_SPIInitSlave(&a_BCM_SPI);
						/*--------[ Set The RX ISR CallBack Function ]--------*/
						SPI_SetCBF(BCM_Rx_ISR_cbf);
						break;
					}
					default:
					break;
				}				
				errorStatus = BCM_ERROR + E_OK;
				break;
			}
			/*----------------------[ SPI CASE END ]---------------------*/
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


/* BCM DeInit */

/**
 * Input: Pointer to a structure contains the information needed to De-initialize the BCM Unit 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: De-Initialize the Basic Communication Module
 * 							
 */

ERROR_STATUS BCM_DeInit(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	
	if(a_BCM != NULL)
	{	
		
		/*--------[ Search For That BCM CFG In The Working List ]--------*/
		if (g_BCM_EXcfg.FSM_State == IDLE_State)
		{
			/*--------[ Set That BCM To OFF ]--------*/
			g_BCM_EXcfg.FSM_State = OFF_State;	
		}
	} 
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		return BCM_ERROR + NULL_PTR  ;
	}
	
	return errorStatus;
}
/* BCM_send */

/**
 * Input:  Pointer to a structure contains the information of the BCM Unit
 *		   Pointer To The Buffer Of Data To Be Transmitted
 *		   Buffer Length OF The Data To Be Transmitted
 *		   Pointer To The Notification Function To Be Called After Sending Completion
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Sends The Data In TX_buffer Over The BCM Protocol
 * 							
 */


ERROR_STATUS BCM_Send(uint8_t* Buffer, uint16_t Buf_Len, BCM_cfg_s* My_BCM, Notify_FunPtr Notify_Ptr )
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_OK;

	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(My_BCM != NULL && Buffer != NULL && Notify_Ptr != NULL)
	{
		
		/* search for The corresponding BCM Struct ID */ 
		/* lock the buffer so user can't chance on it */
		if(g_BCM_EXcfg.Lock_State == Buffer_Unlocked)
		{
			BCM_Buffer_Lock(&g_BCM_EXcfg);
		}
		else
		{
			/* The Buffer IS Already Locked, Means It's Sending Now.. */
			errorStatus = BCM_ERROR + BUFFER_ALREADY_LOCKED;
		}
		
		/*set the buffer address, length, and notification function*/
		g_BCM_EXcfg.Buffer = Buffer;
		g_BCM_EXcfg.Buf_Len = Buf_Len;
		g_BCM_EXcfg.BCM_notify_cbf = Notify_Ptr;
		
		switch(g_BCM_EXcfg.Protocol)
		{	
			case UART_Protocol :
				UART_Write(BCM_ID);
				break;
			case  SPI_Protocol :
				_SPISend(BCM_ID);
				break;
		}
		g_BCM_EXcfg.FSM_State = SendingByte_State;
		
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

/**
 * Input:  Pointer to a structure contains the information of the BCM Unit
 *		   Pointer To The Buffer Of Data To Be Received
 *		   Buffer Length OF The Data To Be Received
 *		   Pointer To The Notification Function To Be Called After Reception Completion
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Setup The Receive Buffer For The Basic Communication Module
 * 							
 */

ERROR_STATUS BCM_Setup_RxBuffer(BCM_cfg_s* a_BCM, uint16_t a_Buffer_Len, uint8_t* a_buffer, Notify_FunPtr a_notify)
{
	/* Needs So Much Improvements & Error Checking & More... */
	
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	
	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL && a_buffer != NULL && a_notify != NULL)
	{
		
		/*-------------[ Check BCM's state if OFF_state means module not initialized  ]-------------*/
		if (g_BCM_EXcfg.FSM_State==OFF_State )
		{
			errorStatus = BCM_ERROR + NOT_INIT;
			return errorStatus ;
			
		/*-------------[ Check BCM's state if ReceiveComplete_State or ReceivingByte_State means module is in the middle of the processes ]-------------*/
		}else if(g_BCM_EXcfg.FSM_State== ReceiveComplete_State || g_BCM_EXcfg.FSM_State == ReceivingByte_State)
		{
			errorStatus = BCM_ERROR + MULTI_START;
			return errorStatus ;
		}
		
		g_BCM_EXcfg.Buf_Len = a_Buffer_Len;
		g_BCM_EXcfg.Buffer = a_buffer;
		
		g_BCM_EXcfg.BCM_notify_cbf = a_notify;
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		errorStatus = NULL_PTR + BCM_ERROR;
		return errorStatus;
	}
	
	return errorStatus;
}

/**
 * Input:   
 * Output:
 * In/Out: 
 * Return: The Received Message Length			
 * Description: Gets The Message Length From The BCM Receiver
 * 			
 */

ERROR_STATUS BCM_Get_msgLEN(uint8_t* msg_Lentgh)

{
	ERROR_STATUS errorStatus = BCM_ERROR + E_NOK;
	
	*msg_Lentgh = g_BCM_EXcfg.MSG_Len ;
	
	return errorStatus ;
	 
}


/*
 * Input:  Pointer to a structure for the BCM Unit 
 * Output:
 * In/Out: Pointer to a state variable to save the result in
 * Return: The error status of the function.			
 * Description: Gets The Transmit Buffer Lock State For the Basic Communication Module Unit
 * 							
 */

ERROR_STATUS BCM_Get_TxBuf_State(uint8_t* Tx_State, BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_OK;

	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL && Tx_State != NULL)
	{
		/* Search For The Corresponding BCM Struct With ID  */
		
		/* if the BCMmodule not initialized return error */
		
		if (g_BCM_EXcfg.FSM_State == OFF_State)
		{
			errorStatus = BCM_ERROR + NOT_INIT ;
		}else
		{
			*Tx_State = g_BCM_EXcfg.Lock_State;
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

/*
 * Input:  Pointer to a structure for the BCM Unit 
 * Output:
 * In/Out: Pointer to a state variable to save the result in
 * Return: The error status of the function.			
 * Description: Gets The Receive Buffer Lock State For the Basic Communication Module Unit
 * 							
 */

ERROR_STATUS BCM_Get_RxBuf_State(uint8_t* Rx_State, BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_OK;

	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL && Rx_State != NULL)
	{
		/* Search For The Corresponding BCM Struct With ID  */
		
		/* if the BCMmodule not initialized return error */
		
		if (g_BCM_EXcfg.FSM_State == OFF_State)
		{
			errorStatus = BCM_ERROR + NOT_INIT ;
		}else
		{
			*Rx_State = g_BCM_EXcfg.Lock_State;
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

/*
 * Input:  Pointer to a structure for the BCM Unit 
 * Output:
 * In/Out: 
 * Return: The error status of the function.			
 * Description: Unlock The Receive Buffer For the Basic Communication Module Unit
 * 							
 */

ERROR_STATUS BCM_RxUnlock(BCM_cfg_s* a_BCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_OK;

	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_BCM != NULL)
	{
		/* Search For The Corresponding BCM Structure With ID */
		if(g_BCM_EXcfg.Lock_State == Buffer_Locked)
		{
			g_BCM_EXcfg.Lock_State = Buffer_Unlocked;	
		}
		else
		{
			errorStatus = BCM_ERROR + BUFFER_ALREADY_UNLOCKED;
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

static ERROR_STATUS BCM_Buffer_Lock(BCM_EXcfg_s* a_exBCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_OK;

	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_exBCM != NULL)
	{
		a_exBCM->Lock_State = Buffer_Locked;
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		errorStatus = NULL_PTR + BCM_ERROR;
		return errorStatus;
	}
	return errorStatus;
}

static ERROR_STATUS BCM_TxUnlock(BCM_EXcfg_s* a_exBCM)
{
	ERROR_STATUS errorStatus = BCM_ERROR + E_OK;

	/*-------------[ Check BCM's Pointer Validity ]-------------*/
	if(a_exBCM != NULL)
	{
		a_exBCM->Lock_State = Buffer_Unlocked;
	}
	/*-------------[ In Case Of BCM's Null Pointer ]-------------*/
	else
	{
		errorStatus = NULL_PTR + BCM_ERROR;
		return errorStatus;
	}
	return errorStatus;	
}