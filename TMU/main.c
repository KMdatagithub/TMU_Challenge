/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 
#include "SL/TMU.h"
#include "MCAL/DIO.h"

DIO_Cfg_s g_LED1;
DIO_Cfg_s g_LED2;
DIO_Cfg_s g_LED3;

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

void StopTasks()
{
	TMU_Stop(100);
	//TMU_Stop(110);
}

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
}


int main(void)
{
	/*-------------[ TMU Initialization ]-------------*/
    TMU_cfg_s TMU1;
	TMU1.Tick_Time = 1;
	TMU1.Timer_ID = TIMER_CH0;
	TMU_Init(&TMU1);
    
	/*-------------[ LEDs Initialization ]-------------*/
	Init_LEDs();
	
	/*----------[ Start 3 Different Consumers ]---------*/
	/*[[ TMU_Srart(Consumer_FunPtr, ConsumerID, Periodicity, Time_IN_ms); ]]*/
	TMU_Start(ToggleLED1, 100, PERIODIC, 100);
	TMU_Start(ToggleLED2, 110, PERIODIC, 300);
	TMU_Start(ToggleLED3, 120, ONESHOT, 3000);
	TMU_Start(StopTasks, 130, ONESHOT, 5000);
	
	while (1) 
    {
		TMU_Dispatcher();
    }
}

