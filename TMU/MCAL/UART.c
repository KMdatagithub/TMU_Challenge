/*
 * UART.c
 *
 * Created: 2/8/2020 5:44:07 PM
 *  Author: Khaled Magdy
 */ 

#include "UART.h"


FunPtr Ptr_UART_TX = NULL;
FunPtr Ptr_UART_RX = NULL;


void UART_Init(UART_cfg* uart_cfg)
{
	uint8_t uart_control = 0x80;
	// Set The UART Baud Rate
	UBRRL = ((F_cpu/(16*uart_cfg->baudrate)) - 1);
	UBRRH = ((F_cpu/(16*uart_cfg->baudrate)) - 1)>>8;
	// Set The UART Mode OF Operation
	UCSRB |= (uart_cfg->mode | uart_cfg->uartInterrupts);
	// Set The UART Synchronization
	// Set The UART Data Sampling Edge
	// Set The UART Data Bits
	// Set The UART Parity Option
	// Set The UART Stop Bits
	// Set The UART Interrupt Mode
	uart_control |= (uart_cfg->uartSync | uart_cfg->sampleEdge | uart_cfg->dataBits | uart_cfg->parity | uart_cfg->stopBits);
	UCSRC |= uart_control;
	SREG  |= (0x80);
}

uint8_t UART_Read(void)
{
	return UDR;
}

void UART_Write(uint8_t data)
{
	// Wait For Empty Transmit Buffer
	while(!(UCSRA & (1<<5)));
	// Write The Data To The Buffer & Send!
	UDR = data;
}

void UART_Write_String(uint8_t* str)
{
	uint16_t i=0;
	while(str[i] != '\0')
		UART_Write(str[i++]);
}

/*=======================================================*/
/*-----------[ Timers Set CallBack Functions ]-----------*/
/*=======================================================*/

void UART_SetTX(FunPtr Ptr_ISR)
{
	Ptr_UART_TX = Ptr_ISR;
}

void UART_SetRX(FunPtr Ptr_ISR)
{
	Ptr_UART_RX = Ptr_ISR;
}

/*=================================================*/
/*---------------[ UART ISR Handlers ]-------------*/
/*=================================================*/

ISR(USART_RXC_vect)
{
	if(Ptr_UART_RX != NULL)
	{
		Ptr_UART_RX();
	}
}

ISR(USART_TXC_vect)
{
	if(Ptr_UART_TX != NULL)
	{
		Ptr_UART_TX();
	}
}