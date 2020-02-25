/*
 * Timer.h
 *
 *  Created on: Dec 21, 2019
 *      Author: Zayed & Saad
 */

#ifndef TIMER_H_
#define TIMER_H_

/************************************************************************/
/*									INCLUDES					        */
/************************************************************************/

#include "../util/std_types.h"
#include "../util/registers.h"
#include "../util/interrupt.h"

/************************************************************************/
/*								DEFINES							        */
/************************************************************************/


#define		TIMER_CH0					0
#define		TIMER_CH1					1
#define		TIMER_CH2					2

#define		TIMER_MODE					0
#define		COUNTER_UP_MODE				1
#define		COUNTER_DOWN_MODE			2

#define		TIMER_POLLING_MODE			0
#define		TIMER_INTERRUPT_MODE		1

#define		TIMER_NO_CLOCK				0x00
#define		TIMER_PRESCALER_NO			0x01
#define		TIMER_PRESCALER_8			0x02
#define		TIMER_PRESCALER_32			0x03
#define		TIMER_PRESCALER_64			0x04
#define		TIMER_PRESCALER_128			0x05
#define		TIMER_PRESCALER_256			0x06
#define		TIMER_PRESCALER_1024		0x07

#define 	Timer0_TIFR_OCBit			BIT1
#define 	Timer0_TIFR_OFBit			BIT0
#define 	Timer1_TIFR_OCBit			BIT4
#define 	Timer1_TIFR_OFBit			BIT2
#define 	Timer2_TIFR_OCBit			BIT7
#define 	Timer2_TIFR_OFBit			BIT6


/************************************************************************/
/*			  Structures Definitions		                            */
/************************************************************************/

typedef struct Timer_cfg_s
{
	uint8_t Timer_CH_NO;
	uint8_t Timer_Mode;
	uint8_t Timer_Polling_Or_Interrupt;
	uint8_t Timer_Prescaler;
	void (*Timer_Cbk_ptr)(void);
}Timer_cfg_s;

/************************************************************************/
/*				    TIMER FUNCTIONS' PROTOTYPES					        */
/************************************************************************/

/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
extern ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function strats the needed timer.
 * 							
 */
extern ERROR_STATUS Timer_Start(uint8_t Timer_CH_NO, uint16_t Timer_Count);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */
extern ERROR_STATUS Timer_Stop(uint8_t Timer_CH_NO);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 * 							
 */
extern ERROR_STATUS Timer_GetStatus(uint8_t Timer_CH_NO, bool* Data);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */
extern ERROR_STATUS Timer_GetValue(uint8_t Timer_CH_NO, uint16_t* Data);



void TIMER0_SetOV(FunPtr Ptr_ISR);
void TIMER0_SetCM(FunPtr Ptr_ISR);
void TIMER2_SetOV(FunPtr Ptr_ISR);
void TIMER2_SetCM(FunPtr Ptr_ISR);
void TIMER1_SetOV(FunPtr Ptr_ISR);
void TIMER1_SetCM(FunPtr Ptr_ISR);






#endif /* TIMER_H_ */