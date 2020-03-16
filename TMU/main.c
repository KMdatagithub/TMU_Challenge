/*
 * TMU.c
 *
 * Created: 2/23/2020 9:32:04 PM
 * Author : Khaled Magdy
 */ 

#include "SL/SOS.h"
#include "MCAL/DIO.h"
#include "ECUAL/LCD.h"
#include "ECUAL/KeyPad.h"
#include <avr/sleep.h>

/**=========================================================================*
 *								Defines & Globals							*
 *==========================================================================*/

#define GAME_INTRO_STATE  0
#define GAME_Q1_STATE     1
#define GAME_Q2_STATE     2
#define GAME_Q3_STATE     3
#define GAME_Q4_STATE     4
#define GAME_Q5_STATE     5
#define GAME_END_STATE    6
#define WIN_SCORE         5


DIO_Cfg_s g_LED0, g_LED1, g_LED2, g_LED3;
DIO_Cfg_s Test_Pin;
SOS_cfg_s MySOS;

uint8_t key_states[KEYS_NUM] = {ZERO};

/*==========================================================================*/

void CPU_Sleep(void)
{
	sleep_enable();
	sei();
	sleep_cpu();
}

/*==========================================================================*/
void Game_App(void);
void Dummy(void)
{
	DIO_Toggle(Test_Pin.GPIO, Test_Pin.pins);
}
void Scan_Keypad()
{
	Keypad_Scan(key_states);
}
void Update_LEDs()
{
	if((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED))
	{
		DIO_Write(g_LED0.GPIO, g_LED0.pins, HIGH);
		DIO_Write(g_LED1.GPIO, g_LED1.pins, HIGH);
		DIO_Write(g_LED2.GPIO, g_LED2.pins, HIGH);
		DIO_Write(g_LED3.GPIO, g_LED3.pins, HIGH);
	}	
	else if((key_states[0] == KEY_RELEASED) | (key_states[1] == KEY_RELEASED) | (key_states[2] == KEY_RELEASED))
	{
		DIO_Write(g_LED0.GPIO, g_LED0.pins, LOW);
		DIO_Write(g_LED1.GPIO, g_LED1.pins, LOW);
		DIO_Write(g_LED2.GPIO, g_LED2.pins, LOW);
		DIO_Write(g_LED3.GPIO, g_LED3.pins, LOW);
	}
}
void Init_LEDs(void)
{
	g_LED0.GPIO = GPIOB;
	g_LED0.dir  = OUTPUT;
	g_LED0.pins = BIT4;
	DIO_init(&g_LED0);
	g_LED1.GPIO = GPIOB;
	g_LED1.dir  = OUTPUT;
	g_LED1.pins = BIT5;
	DIO_init(&g_LED1);
	g_LED2.GPIO = GPIOB;
	g_LED2.dir  = OUTPUT;
	g_LED2.pins = BIT6;
	DIO_init(&g_LED2);
	g_LED3.GPIO = GPIOB;
	g_LED3.dir  = OUTPUT;
	g_LED3.pins = BIT7;
	DIO_init(&g_LED3);
	Test_Pin.dir = OUTPUT;
	Test_Pin.GPIO = GPIOD;
	Test_Pin.pins = BIT0;
	DIO_init(&Test_Pin);
}
/*==========================================================================*/

int main(void)
{
	/*---------------[ LCD Initialization ]--------------*/
	LCD_init();
	
	/*--------------[ LEDs Initialization ]--------------*/
	Init_LEDs();
	
	/*-------------[ Keypad Initialization ]-------------*/
	Keypad_Init();
	
	/*---------------[ TMU Initialization ]--------------*/
	MySOS.Tick_Time = 1;
	MySOS.Timer_ID = TIMER_CH0;
	SOS_Init(&MySOS);
	
	/*----------[ Start 3 Different Tasks ]---------*/
	/*---[[ Start_Task(Task_FunPtr, TaskID, Periodicity, Time_IN_ms, Priority, PreHook, PostHook); ]]---*/
	Start_Task(Scan_Keypad, 120, PERIODIC, 5,  2, Dummy, Dummy);
	Start_Task(Update_LEDs, 150, PERIODIC, 10, 5, Dummy, Dummy);
	Start_Task(Game_App,    110, PERIODIC, 50, 7, Dummy, Dummy);
	
	/*-------------[ SUPER LOOP ]-------------*/
	while (1)
	{
		SOS_Run();
	}
}

/*==========================================================================*/

void Game_App(void)
{
	static uint8_t Game_FSM = ZERO, Ticks = ZERO, Score = ZERO, Key_Hold = ZERO;
	uint8_t score_ch[2] = {'\0'};
	
	switch(Game_FSM)
	{
		case GAME_INTRO_STATE:
		{
			if(Ticks == ZERO)
			{
				LCD_displayStringRowColumn(1, 4, (uint8_t*)"Welcome To");
				LCD_displayStringRowColumn(2, 5, (uint8_t*)"The Game");
				Ticks++;
			}
			else if(Ticks < 50)
			{
				Ticks++;
			}
			else if(Ticks == 50)
			{
				LCD_displayStringRowColumn(1, 3, (uint8_t*)"Press Any Key");
				LCD_displayStringRowColumn(2, 5, (uint8_t*)"To Start");
				Ticks++;
			}
			else
			{
				if(((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED)) && !Key_Hold)
				{
					LCD_displayStringRowColumn(1, 1, (uint8_t*)"Answer By Hiting");
					LCD_displayStringRowColumn(2, 1, (uint8_t*)"YES / NO / SKIP");
					Key_Hold = 1;
				}
				if((Key_Hold == 1) && ((key_states[0] == KEY_RELEASED)&&(key_states[1] == KEY_RELEASED)&&(key_states[2] == KEY_RELEASED)))
				{
					Key_Hold = ZERO;
					Ticks = ZERO;
					Game_FSM = GAME_Q1_STATE;
				}
			}
			break;
		}
		case GAME_Q1_STATE:
		{
			if(Ticks == 50)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 55)
			{
				Ticks++;
			}
			else if(Ticks == 55)
			{
				LCD_displayStringRowColumn(1, 1, (uint8_t*)"Mars Planet Has");
				LCD_displayStringRowColumn(2, 1, (uint8_t*)"3 Moons");
				Ticks++;
			}
			else
			{
				if(((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED)) && !Key_Hold)
				{
					if(key_states[1] == KEY_PRESSED)
					{ Score++; }
					Key_Hold = 1;
				}
				if((Key_Hold == 1) && ((key_states[0] == KEY_RELEASED)&&(key_states[1] == KEY_RELEASED)&&(key_states[2] == KEY_RELEASED)))
				{
					Key_Hold = ZERO;
					Ticks = ZERO;
					Game_FSM = GAME_Q2_STATE;
				}
			}
			break;
		}
		case GAME_Q2_STATE:
		{
			if(Ticks == ZERO)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 5)
			{
				Ticks++;
			}
			else if(Ticks == 5)
			{
				LCD_displayStringRowColumn(1, 1, (uint8_t*)"A Year On Mars");
				LCD_displayStringRowColumn(2, 1, (uint8_t*)"Is 687 Days");
				Ticks++;
			}
			else
			{
				if(((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED)) && !Key_Hold)
				{
					if(key_states[0] == KEY_PRESSED)
					{ Score++; }
					Key_Hold = 1;
				}
				if((Key_Hold == 1) && ((key_states[0] == KEY_RELEASED)&&(key_states[1] == KEY_RELEASED)&&(key_states[2] == KEY_RELEASED)))
				{
					Key_Hold = ZERO;
					Ticks = ZERO;
					Game_FSM = GAME_Q3_STATE;
				}
			}
			break;
		}
		case GAME_Q3_STATE:
		{
			if(Ticks == ZERO)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 5)
			{
				Ticks++;
			}
			else if(Ticks == 5)
			{
				LCD_displayStringRowColumn(1, 1, (uint8_t*)"60kg On Earth");
				LCD_displayStringRowColumn(2, 1, (uint8_t*)"Is 20kg on Moon");
				Ticks++;
			}
			else
			{
				if(((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED)) && !Key_Hold)
				{
					if(key_states[1] == KEY_PRESSED)
					{ Score++; }
					Key_Hold = 1;
				}
				if((Key_Hold == 1) && ((key_states[0] == KEY_RELEASED)&&(key_states[1] == KEY_RELEASED)&&(key_states[2] == KEY_RELEASED)))
				{
					Key_Hold = ZERO;
					Ticks = ZERO;
					Game_FSM = GAME_Q4_STATE;
				}
			}
			break;
		}
		case GAME_Q4_STATE:
		{
			if(Ticks == ZERO)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 5)
			{
				Ticks++;
			}
			else if(Ticks == 5)
			{
				LCD_displayStringRowColumn(1, 2, (uint8_t*)"1, 2, 4, 7, ??");
				LCD_displayStringRowColumn(2, 1, (uint8_t*)"Missing Num: 14");
				Ticks++;
			}
			else
			{
				if(((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED)) && !Key_Hold)
				{
					if(key_states[1] == KEY_PRESSED)
					{ Score++; }
					Key_Hold = 1;
				}
				if((Key_Hold == 1) && ((key_states[0] == KEY_RELEASED)&&(key_states[1] == KEY_RELEASED)&&(key_states[2] == KEY_RELEASED)))
				{
					Key_Hold = ZERO;
					Ticks = ZERO;
					Game_FSM = GAME_Q5_STATE;
				}
			}
			break;
		}
		case GAME_Q5_STATE:
		{
			if(Ticks == ZERO)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 5)
			{
				Ticks++;
			}
			else if(Ticks == 5)
			{
				LCD_displayStringRowColumn(1, 1, (uint8_t*)"Sound Travels In");
				LCD_displayStringRowColumn(2, 1, (uint8_t*)"Air Faster < H2O");
				Ticks++;
			}
			else
			{
				if(((key_states[0] == KEY_PRESSED) | (key_states[1] == KEY_PRESSED) | (key_states[2] == KEY_PRESSED)) && !Key_Hold)
				{
					if(key_states[1] == KEY_PRESSED)
					{ Score++; }
					Key_Hold = 1;
				}
				if((Key_Hold == 1) && ((key_states[0] == KEY_RELEASED)&&(key_states[1] == KEY_RELEASED)&&(key_states[2] == KEY_RELEASED)))
				{
					Key_Hold = ZERO;
					Ticks = ZERO;
					Game_FSM = GAME_END_STATE;
				}
			}
			break;
		}
		case GAME_END_STATE:
		{
			if(Ticks == ZERO)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 5)
			{
				Ticks++;
			}
			else if(Ticks == 5)
			{
				score_ch[ZERO] = Score+48;
				LCD_displayStringRowColumn(1, 2, (uint8_t*)"Your Score Is:");
				LCD_displayStringRowColumn(2, 9, (uint8_t*)&score_ch);
				Ticks++;
			}
			else if(Ticks < 50)
			{
				Ticks++;
			}
			else if(Ticks == 50)
			{
				LCD_clear();
				Ticks++;
			}
			else if(Ticks < 55)
			{
				Ticks++;
			}
			else if(Ticks == 55)
			{
				if(Score == WIN_SCORE)
				{
					LCD_displayStringRowColumn(1, 1, (uint8_t*)"Congratulations!");
					LCD_displayStringRowColumn(2, 5, (uint8_t*)"You Won!");	
				}
				else
				{
					LCD_displayStringRowColumn(1, 5, (uint8_t*)"Game Over");
					LCD_displayStringRowColumn(2, 5, (uint8_t*)"You Lose!");
				}
				Ticks++;
			}
			else if(Ticks < 100)
			{
				Ticks++;
			}
			else
			{
				LCD_clear();
				Score    = ZERO;
				Key_Hold = ZERO;
				Ticks    = ZERO;
				Game_FSM = GAME_INTRO_STATE;
			}
			break;
		}
		default:
			break;
	}
}