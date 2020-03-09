/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 

#include "SL/SOS.h"
#include "SL/BCM.h"
#include "MCAL/DIO.h"
#include "MCAL/UART.h"
#include <avr/sleep.h>

/**=========================================================================*
 *								Defines & Globals							*
 *==========================================================================*/

#define TRANSMIT_MAX 500
#define ENTER_KEY	 0x0D

BCM_cfg_s BCM1;
UART_cfg UART1;
DIO_Cfg_s g_LED1;
DIO_Cfg_s g_LED2;
DIO_Cfg_s g_LED3;
DIO_Cfg_s Test_Pin;
TMU_cfg_s TMU1;

uint8_t txBuffer[TRANSMIT_MAX];
uint8_t msg_len = ZERO;

volatile uint8_t a_index        = ZERO;
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
}

/*==========================================================================*/

void CPU_Sleep(void)
{
	sleep_enable();
	sei();
	sleep_cpu();
}

/*==========================================================================*/

void ToggleLED1()
{
	DIO_Toggle(g_LED1.GPIO, g_LED1.pins);
}
void ToggleLED2()
{
	DIO_Toggle(g_LED2.GPIO, g_LED2.pins);
}
void ToggleLED3()
{
	DIO_Toggle(g_LED3.GPIO, g_LED3.pins);
}
;
void Init_LEDs(void)
{
	g_LED1.GPIO = GPIOD;
	g_LED1.dir  = OUTPUT;
	g_LED1.pins = BIT4;
	DIO_init(&g_LED1);
	g_LED2.GPIO = GPIOD;
	g_LED2.dir  = OUTPUT;
	g_LED2.pins = BIT5;
	DIO_init(&g_LED2);
	g_LED3.GPIO = GPIOD;
	g_LED3.dir  = OUTPUT;
	g_LED3.pins = BIT6;
	DIO_init(&g_LED3);
	DIO_Write(g_LED3.GPIO, g_LED3.pins, HIGH);
	Test_Pin.dir = OUTPUT;
	Test_Pin.GPIO = GPIOD;
	Test_Pin.pins = BIT0;
	DIO_init(&Test_Pin);
}

/*==========================================================================*/

int main(void)
{
	/*-------------[ LEDs Initialization ]-------------*/
	Init_LEDs();
		
	/*-------------[ TMU Initialization ]-------------*/
	TMU1.Tick_Time = 1;
	TMU1.Timer_ID = TIMER_CH0;
	TMU_Init(&TMU1);
	
	/*----------[ Start 3 Different Consumers ]---------*/
	/*[[ TMU_Srart(Consumer_FunPtr, ConsumerID, Periodicity, Time_IN_ms); ]]*/
	TMU_Start(ToggleLED1, 100, PERIODIC, 100);
	TMU_Start(ToggleLED2, 110, PERIODIC, 300);
	TMU_Start(ToggleLED3, 120, ONESHOT, 3000);
	
	/*-------------[ BCM Initialization ]-------------*/
	BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);
	BCM_Send(txBuffer, TRANSMIT_MAX, &BCM1, txnotify);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1)
	{
		DIO_Write(Test_Pin.GPIO, Test_Pin.pins, HIGH);
		BCM_Tx_Dispatcher();
		TMU_Dispatcher();
		DIO_Write(Test_Pin.GPIO, Test_Pin.pins, LOW);
		CPU_Sleep();
		// CPU Load Is Found To Be 3% At The First Few Seconds And It Drops Down To 2% Thereafter
	}
}