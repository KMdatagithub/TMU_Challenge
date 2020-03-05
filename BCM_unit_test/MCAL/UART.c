/*
 * UART.c
 *
 * Created: 2/8/2020 5:44:07 PM
 *  Author: Khaled Magdy
 */

#include "UART.h"
#define DUMY_DATA 0x05

FunPtr Ptr_UART_TX = NULL;
FunPtr Ptr_UART_RX = NULL;



void UART_Init(UART_cfg* uart_cfg)
{

}

uint8_t UART_Read(void)
{
	return DUMY_DATA ;
}

void UART_Write(uint8_t data)
{

}

void UART_Write_String(uint8_t* str)
{

}

/*=======================================================*/
/*-----------[ Timers Set CallBack Functions ]-----------*/
/*=======================================================*/

void UART_SetTX(FunPtr Ptr_ISR)
{

}

void UART_SetRX(FunPtr Ptr_ISR)
{

}

/*=================================================*/
/*---------------[ UART ISR Handlers ]-------------*/
/*=================================================*/




