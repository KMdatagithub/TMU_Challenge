/*
 * TMU.h
 *
 * Created: 2/23/2020 9:36:28 PM
 *  Author: Khaled Magdy
 */ 


#ifndef TMU_H_
#define TMU_H_

#include "../MCAL/Timer.h"
#include "../util/std_types.h"
#include "../util/interrupt.h"
#include "../util/error_codes.h"


#define INACTIVE	0
#define ACTIVE		1
#define DISABLED	2

#define PERIODIC	1
#define ONESHOT		0

#define TMR_Ticks	250


/*-----[ TMU Configurations & Consumer Structures ]-----*/

typedef struct SOS_cfg_s{
	uint8_t Tick_Time;
	uint8_t Timer_ID;
	uint8_t State;
	void (*Timer_Cbk_ptr)(void);
}SOS_cfg_s;


/*-----[ TMU Functions' Prototypes ]-----*/

ERROR_STATUS SOS_Init(SOS_cfg_s* a_SOS_s);

ERROR_STATUS SOS_DeInit(SOS_cfg_s* a_SOS_s);

ERROR_STATUS Start_Task(FunPtr a_TaskFun, uint16_t a_TaskID, uint8_t a_Periodic_OneShot, uint32_t a_Time, uint8_t a_Priority, FunPtr PreHook, FunPtr PostHook);

ERROR_STATUS Delete_Task(uint16_t a_ConsumerID);

/* This Is The Scheduler (Dispatcher) Function For The SOS */
void SOS_Run(void);


#endif /* TMU_H_ */