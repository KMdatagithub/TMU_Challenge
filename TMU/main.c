/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 

#include "SL/SOS.h"
#include "MCAL/DIO.h"
#include <avr/sleep.h>

/**=========================================================================*
 *								Defines & Globals							*
 *==========================================================================*/

DIO_Cfg_s g_LED1;
DIO_Cfg_s g_LED2;
DIO_Cfg_s g_LED3;
DIO_Cfg_s Test_Pin;
SOS_cfg_s MySOS;

/*==========================================================================*/

void CPU_Sleep(void)
{
	sleep_enable();
	sei();
	sleep_cpu();
}

/*==========================================================================*/

void Dummy(void){}

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
	MySOS.Tick_Time = 1;
	MySOS.Timer_ID = TIMER_CH0;
	SOS_Init(&MySOS);
	
	/*----------[ Start 3 Different Tasks ]---------*/
	/*[[ TMU_Srart(Task_FunPtr, TaskID, Periodicity, Time_IN_ms); ]]*/
	Start_Task(ToggleLED1, 100, PERIODIC, 100, 5, Dummy, Dummy);
	Start_Task(ToggleLED2, 110, PERIODIC, 300, 15, Dummy, Dummy);
	Start_Task(ToggleLED3, 120, ONESHOT, 3000, 25, Dummy, Dummy);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1)
	{
		SOS_Run();
	}
}