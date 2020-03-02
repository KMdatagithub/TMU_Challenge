/*
 * BCM.h
 *
 * Created: 3/1/2020 9:30:20 PM
 *  Author: Khaled Magdy
 */ 


#ifndef BCM_H_
#define BCM_H_

#include "../MCAL/UART.h"
#include "../util/error_codes.h"
#include "../MCAL/SPI__.h"
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

/*==============================================*/

typedef uint8_t enum_BcmStatus;
typedef	void (*Notify_FunPtr)(enum_BcmStatus);

/*-----[ BCM Configurations ]-----*/

typedef struct BCM_cfg_s{
	uint8_t Mode;
	uint8_t BCM_CH_ID;
	uint8_t Protocol;
}BCM_cfg_s;



ERROR_STATUS BCM_Init(BCM_cfg_s*);

ERROR_STATUS BCM_DeInit(BCM_cfg_s*);

ERROR_STATUS BCM_Send(uint8_t* Buffer, uint16_t Buf_Len, BCM_cfg_s* My_BCM, Notify_FunPtr);

uint8_t* BCM_Setup_RxBuffer(BCM_cfg_s* a_BCM, uint16_t a_Buffer_Len);

ERROR_STATUS BCM_Get_TxBuf_State(uint8_t* Tx_State, BCM_cfg_s*);

ERROR_STATUS BCM_Get_RxBuf_State(uint8_t* Rx_State, BCM_cfg_s*);

ERROR_STATUS BCM_TxUnlock(BCM_cfg_s*);

ERROR_STATUS BCM_RxUnlock(BCM_cfg_s*);

void BCM_Tx_Dispatcher(void);

void BCM_Rx_Dispatcher(void);


#endif /* BCM_H_ */