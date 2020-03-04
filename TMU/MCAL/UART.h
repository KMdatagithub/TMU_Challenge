/*
 * UART.h
 *
 * Created: 2/8/2020 5:43:18 PM
 *  Author: Khaled Magdy
 */ 


#ifndef UART_H_
#define UART_H_

#include "../util/registers.h"
#include "../util/interrupt.h"
#include "UART_cfg.h"


#define F_cpu	16000000UL

typedef enum En_uartMode_t{
	UART_TX = 0x08, UART_RX = 0x10, UART_Transceiver = 0x18
}En_uartMode_t;

typedef enum En_uartSyncMode_t{
	UART_Async = 0, UART_Sync = 0x40
}En_uartSyncMode_t;

typedef enum En_uartDataBits_t{
	UART_Bits_5 = 0, UART_Bits_6 = 0x02, UART_Bits_7 = 0x04, UART_Bits_8 = 0x06
}En_uartDataBits_t;

typedef enum En_uartParity_t{
	No_Parity = 0, Even_Parity = 0x20, Odd_Parity = 0x30
}En_uartParity_t;

typedef enum En_uartStopBits_t{
	StopBits_1 = 0, StopBits_2 = 0x08
}En_uartStopBits_t;

typedef enum En_uartSampleEdg_t{
	RisingEdg = 0, FallingEdg = 0x01
}En_uartSampleEdg_t;

typedef enum En_uartInterrupts_t{
	OnTx = 0x40, OnRx = 0x80, All_Interrupts = 0xC0,
	UDR_Empty_Interrupt = 0x20, UART_No_Interrupts = 0x00
}En_uartInterrupts_t;

typedef struct UART_cfg{
	En_uartMode_t mode;
	En_uartDataBits_t dataBits;
	En_uartParity_t parity;
	En_uartSyncMode_t uartSync;
	En_uartStopBits_t stopBits;
	En_uartSampleEdg_t sampleEdge;
	En_uartInterrupts_t uartInterrupts;
	uint32_t baudrate;
}UART_cfg;


//----------------------------------------------------------

void UART_Init(UART_cfg*);
uint8_t UART_Read(void);
void UART_Write(uint8_t);
void UART_Write_String(uint8_t*);

void UART_SetTX(FunPtr Ptr_ISR);
void UART_SetRX(FunPtr Ptr_ISR);

#endif /* UART_H_ */