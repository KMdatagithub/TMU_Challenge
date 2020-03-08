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

typedef struct TMU_cfg_s{
	uint8_t Tick_Time;
	uint8_t Timer_ID;
	uint8_t State;
	void (*Timer_Cbk_ptr)(void);
}TMU_cfg_s;


/*-----[ TMU Functions' Prototypes ]-----*/

ERROR_STATUS TMU_Init(TMU_cfg_s* a_TMU_s);

ERROR_STATUS TMU_DeInit(TMU_cfg_s* a_TMU_s);

ERROR_STATUS TMU_Start(FunPtr a_ConsumerFun, uint16_t a_ConsumerID, uint8_t a_Periodic_OneShot, uint32_t a_Time);

ERROR_STATUS TMU_Stop(uint16_t a_ConsumerID);

void TMU_Dispatcher(void);


#endif /* TMU_H_ */