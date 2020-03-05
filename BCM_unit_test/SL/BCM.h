/*
 * BCM.h
 *
 * Created: 3/1/2020 9:30:20 PM
 *  Author: Khaled Magdy
 */ 


#ifndef BCM_H_
#define BCM_H_

#include "../util/error_codes.h"
#include "../util/std_types.h"


/*======================================*/
/*-----[ BCM External Definitions ]-----*/
/*======================================*/

/* Buffer Lock States*/

#define Buffer_Locked			1
#define Buffer_Unlocked			0

/* BCM HW Comm. Protocols */

#define UART_Protocol			0
#define SPI_Protocol			1

/* BCM Operating Mode */

#define BCM_Tx_Mode				0
#define BCM_Rx_Mode				1

typedef uint8_t enum_BcmStatus;
typedef	void (*Notify_FunPtr)(enum_BcmStatus);

/*==============================================*/

/*---------[ BCM User Configurations ]----------*/

typedef struct BCM_cfg_s{
	uint8_t Mode;
	uint8_t BCM_CH_ID;
	uint8_t Protocol;
}BCM_cfg_s;

/**=========================================================================*
 *							BCM Functions Prototypes						*
 *==========================================================================*/


/**
 * Input: Pointer to a structure contains the information needed to initialize the BCM 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the Basic Communication Module
 * 							
 */
ERROR_STATUS BCM_Init(BCM_cfg_s*);
/**
 * Input: Pointer to a structure contains the information needed to De-initialize the BCM Unit 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: De-Initialize the Basic Communication Module
 * 							
 */
ERROR_STATUS BCM_DeInit(BCM_cfg_s*);
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
ERROR_STATUS BCM_Send(uint8_t* Buffer, uint16_t Buf_Len, BCM_cfg_s* My_BCM, Notify_FunPtr);
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
ERROR_STATUS BCM_Setup_RxBuffer(BCM_cfg_s* a_BCM, uint16_t a_Buffer_Len, uint8_t* buf, Notify_FunPtr);
/**
 * Input:  Pointer to a structure for the BCM Unit 
 * Output:
 * In/Out: Pointer to a state variable to save the result in
 * Return: The error status of the function.			
 * Description: Gets The Transmit Buffer Lock State For the Basic Communication Module Unit
 * 							
 */
ERROR_STATUS BCM_Get_TxBuf_State(uint8_t* Tx_State, BCM_cfg_s*);
/**
 * Input:  Pointer to a structure for the BCM Unit 
 * Output:
 * In/Out: Pointer to a state variable to save the result in
 * Return: The error status of the function.			
 * Description: Gets The Receive Buffer Lock State For the Basic Communication Module Unit
 * 							
 */
ERROR_STATUS BCM_Get_RxBuf_State(uint8_t* Rx_State, BCM_cfg_s*);
/**
 * Input:  Pointer to a structure for the BCM Unit 
 * Output:
 * In/Out: 
 * Return: The error status of the function.			
 * Description: Unlock The Receive Buffer For the Basic Communication Module Unit
 * 							
 */
ERROR_STATUS BCM_RxUnlock(BCM_cfg_s*);
/**
 * Input:   
 * Output:
 * In/Out: 
 * Return: The error status of the function.			
 * Description: The Transmit Dispatcher For the Basic Communication Module Unit
 * 							
 */
void BCM_Tx_Dispatcher(void);
/**
 * Input:   
 * Output:
 * In/Out: 
 * Return: The error status of the function.			
 * Description: The Receive Dispatcher For the Basic Communication Module Unit
 * 							
 */
void BCM_Rx_Dispatcher(void);
/**
 * Input:   
 * Output:
 * In/Out: 
 * Return: The Received Message Length			
 * Description: Gets The Message Length From The BCM Receiver
 * 			
 */
ERROR_STATUS BCM_Get_msgLEN(uint8_t* msg_Lentgh);

#endif /* BCM_H_ */