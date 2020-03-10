/*
 * TMU.c
 *
 * Created: 2/23/2020 9:36:01 PM
 *  Author: Khaled Magdy
 */ 
#include "SOS.h"
#include "SOS_cfg.h"


typedef struct Task_s{
	FunPtr Task_Ptr;
	FunPtr Pre_Hook;
	FunPtr Post_Hook;
	uint8_t Periority;
	uint8_t Periodicity;
	uint32_t Time;
	uint8_t State;
	uint32_t Count;
	uint16_t TaskID;
} Task_s;

static Timer_cfg_s g_SOS_TMR;
static SOS_cfg_s   g_SOS = {ZERO, ZERO, DISABLED, NULL};
static Task_s  g_RequestBuffer[REQUEST_BUFFER_LEN];
static sint16_t g_ReadyTasks[REQUEST_BUFFER_LEN];

volatile static uint16_t g_ReqBuffer_Index   = ZERO;
volatile static uint8_t  g_TMR_Ticks_Changed = ZERO;
volatile static uint16_t g_SOS_TickTime      = ZERO;
volatile static uint16_t g_SysTicks          = ZERO;


static void SOS_ISR_cbf(void)
{
	/* Adjust The Tick Time According To The TMU's Time Configuration */
	g_SysTicks++;
	
	if(g_SysTicks == g_SOS_TickTime)
	{
		g_TMR_Ticks_Changed = TRUE;
		g_SysTicks = FALSE;	
	}
}

ERROR_STATUS SOS_Init(SOS_cfg_s* a_SOS_s)
{
	ERROR_STATUS errorStauts = E_OK;
	uint16_t index = 0;
	
	/*-------------[ Check TMU's Pointer Validity ]-------------*/
	if(a_SOS_s != NULL)
	{
		/*-------------[ TMU Initialization ]-------------*/
		g_SOS.Timer_ID  = a_SOS_s->Timer_ID;
		g_SOS.Tick_Time = a_SOS_s->Tick_Time;
		g_SOS.Timer_Cbk_ptr = SOS_ISR_cbf;
		g_SOS.State    = INACTIVE;
		g_SOS_TickTime = g_SOS.Tick_Time;
		
		/*-------------[ Timer Initialization ]-------------*/
		g_SOS_TMR.Timer_Mode = TIMER_MODE;
		g_SOS_TMR.Timer_Prescaler = TIMER_PRESCALER_64;
		g_SOS_TMR.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		g_SOS_TMR.Timer_Cbk_ptr = g_SOS.Timer_Cbk_ptr;
		
		/* Select The Timer Channel & Set It's CallBack Function */
		switch(a_SOS_s->Timer_ID)
		{
			case TIMER_CH0:
				g_SOS_TMR.Timer_CH_NO = TIMER_CH0;
				TIMER0_SetCM(SOS_ISR_cbf);
				break;
			case TIMER_CH1:
				g_SOS_TMR.Timer_CH_NO = TIMER_CH1;
				TIMER1_SetCM(SOS_ISR_cbf);
				break;
			case TIMER_CH2:
				g_SOS_TMR.Timer_CH_NO = TIMER_CH2;
				TIMER2_SetCM(SOS_ISR_cbf);
				break;
			default:
				errorStauts = TMU_ERROR + INVALID_IN;
				return errorStauts;
		}
		
		/* Apply The Settings & Start The TMU Timer Hardware Module */
		Timer_Init(&g_SOS_TMR);
		Timer_Start(g_SOS_TMR.Timer_CH_NO, TMR_Ticks);	
		
		/*  Initialize The Request Buffer  */
		for(index = 0; index < REQUEST_BUFFER_LEN; index++)
		{
			g_RequestBuffer[index].State     = INACTIVE;
			g_RequestBuffer[index].Task_Ptr  = NULL;
			g_RequestBuffer[index].Pre_Hook  = NULL;
			g_RequestBuffer[index].Post_Hook = NULL;
		}
	}
	/*-------------[ In Case Of TMU's Null Pointer ]-------------*/
	else
	{
		errorStauts = TMU_ERROR + NULL_PTR;
		return errorStauts;
	}
	return errorStauts;
}


ERROR_STATUS Start_Task(FunPtr a_TaskFun, uint16_t a_TaskID, uint8_t a_Periodic_OneShot, uint32_t a_Time, uint8_t a_Priority, FunPtr PreHook, FunPtr PostHook)
{
	ERROR_STATUS a_errorStatus = E_OK;
	uint16_t index = 0;
	
	/*  Create New Consumer Instance & Initialize It  */
	Task_s a_NewTask;
	a_NewTask.Task_Ptr = a_TaskFun;
	a_NewTask.Post_Hook = PostHook;
	a_NewTask.Pre_Hook = PreHook;
	a_NewTask.TaskID = a_TaskID;
	a_NewTask.Time = a_Time;
	a_NewTask.Periodicity = a_Periodic_OneShot;
	a_NewTask.Periority = a_Priority;
	a_NewTask.Count = ZERO;
	a_NewTask.State = ACTIVE;
	
	if(g_SOS.State == INACTIVE || g_SOS.State == ACTIVE)
	{
		/*-------------[ Check All CBF Pointers Validity ]-------------*/
		if(a_TaskFun != NULL && PreHook != NULL && PostHook != NULL)
		{
			/*  In Case OF Full Request Buffer  */
			if(g_ReqBuffer_Index == REQUEST_BUFFER_LEN)
			{
				/*  Search For Inactive Task & Overwrite It */
				for(index = 0; index < REQUEST_BUFFER_LEN; index++)
				{
					if(g_RequestBuffer[index].State == INACTIVE)
					{
						g_RequestBuffer[index] = a_NewTask;
						break;
					}
				}
				/*  If All Tasks In The Buffer Are Active & No Space If Available */
				if(index == REQUEST_BUFFER_LEN)
				{
					a_errorStatus = TMU_ERROR + FULL_BUFFER;
					return a_errorStatus;
				}
			}
			/*  If There Still Space In The Request Buffer  */
			else
			{
				/*  Search The Buffer To Report Consumer ID Duplication IF Happened!  */
				for(index = 0; index < REQUEST_BUFFER_LEN; index++)
				{
					if(g_RequestBuffer[index].TaskID == a_TaskID)
					{
						a_errorStatus = TMU_ERROR + MULTI_START;
						return a_errorStatus;
					}
				}
				/* Add The New Consumer To The Request Buffer */
				g_RequestBuffer[g_ReqBuffer_Index++] = a_NewTask;				
			}
		}
		/*-------------[ In Case Of Consumer's Null Pointer CBF ]-------------*/
		else
		{
			a_errorStatus = TMU_ERROR + NULL_PTR;
			return a_errorStatus;
		}
	}
	/*-------------[ In Case The TMU IS Not Active (Not Initialized Yet) ]-------------*/
	else
	{
		a_errorStatus = TMU_ERROR + NOT_INIT;
		return a_errorStatus;
	}
	return a_errorStatus;	
}

ERROR_STATUS Delete_Task(uint16_t a_TaskID)
{
	ERROR_STATUS errorStatus = E_NOK;
	uint16_t a_u16_index;
	
	/*-------------[ Search For The Consumer ID In The Request Buffer ]-------------*/
	for(a_u16_index = ZERO; a_u16_index < REQUEST_BUFFER_LEN; a_u16_index++)
	{
		/*-------------[ When It's Found, Deactivate That Consumer ]-------------*/
		if(g_RequestBuffer[a_u16_index].TaskID == a_TaskID)
		{
			g_RequestBuffer[a_u16_index].State = INACTIVE;
			errorStatus = E_OK;
			return errorStatus;
		}
	}
	
	/*-------------[ Consumer Not Found In Request Buffer ]-------------*/
	return errorStatus;
}

void SOS_Run(void)
{
	sint16_t a_s16_index = ZERO, a_temp = ZERO;
	uint8_t a_u8_ReadyIndex = ZERO, a_counter = ZERO;
	
	/*-------------[ Every TMU Tick, Go Through The Request Buffer ]-------------*/
	if(g_TMR_Ticks_Changed)
	{
		/* Initialize The Ready Tasks Buffer */
		for(a_s16_index = ZERO; a_s16_index < REQUEST_BUFFER_LEN; a_s16_index++)
		{
			g_ReadyTasks[a_s16_index] = -1;
		}
		
		/* Go Through The Tasks Request Buffer To Add All Ready Tasks To Be Executed */
		for(a_s16_index = ZERO; a_s16_index < REQUEST_BUFFER_LEN; a_s16_index++)
		{
			g_RequestBuffer[a_s16_index].Count += g_SOS_TickTime;
			
			/*-------------[ IF Consumer's Due Time Is Met!  ]-------------*/
			if(g_RequestBuffer[a_s16_index].Count >= g_RequestBuffer[a_s16_index].Time)
			{	
				/* IF The Consumer Function IS Periodic */
				if(g_RequestBuffer[a_s16_index].Periodicity == PERIODIC && g_RequestBuffer[a_s16_index].State == ACTIVE)
				{
					g_RequestBuffer[a_s16_index].Count = ZERO;
					g_ReadyTasks[a_u8_ReadyIndex++] = a_s16_index;
				}
				
				/* Else IF The Consumer Function IS OneShot (Non-Periodic) */
				else if (g_RequestBuffer[a_s16_index].Periodicity == ONESHOT && g_RequestBuffer[a_s16_index].State == ACTIVE)
				{
					g_RequestBuffer[a_s16_index].State = INACTIVE;
					g_ReadyTasks[a_u8_ReadyIndex++] = a_s16_index;
				}
			}
		}
		/* Sort The Ready Tasks Based On Priority */
		for(a_s16_index = ZERO; g_ReadyTasks[a_s16_index] != -1; a_s16_index++)
		{
			for (a_counter = a_s16_index + 1; g_ReadyTasks[a_counter] != -1; a_counter++)
			{
				if(g_RequestBuffer[g_ReadyTasks[a_s16_index]].Periority > g_RequestBuffer[g_ReadyTasks[a_counter]].Periority)
				{
					a_temp = g_ReadyTasks[a_s16_index];
					g_ReadyTasks[a_s16_index] = g_ReadyTasks[a_counter];
					g_ReadyTasks[a_counter] = a_temp;
				}					
			}
		}
		/* Execute The Ready Tasks One By One (In Priority Order) */
		for(a_s16_index = ZERO; g_ReadyTasks[a_s16_index] != -1; a_s16_index++)
		{
			g_RequestBuffer[g_ReadyTasks[a_s16_index]].Pre_Hook();
			g_RequestBuffer[g_ReadyTasks[a_s16_index]].Task_Ptr();
			g_RequestBuffer[g_ReadyTasks[a_s16_index]].Post_Hook();
		}		
		
		g_TMR_Ticks_Changed = FALSE;
	}
}

ERROR_STATUS TMU_DeInit(SOS_cfg_s* a_SOS_s)
{
	ERROR_STATUS errorStauts = E_OK;
	
	/*-------------[ Check TMU's Pointer Validity ]-------------*/
	if(a_SOS_s != NULL)
	{
		if(g_SOS.State == ACTIVE || g_SOS.State == INACTIVE)
		{
			/*-------------[ TMU DeInitialization ]-------------*/
			Timer_Stop(g_SOS.Timer_ID);
			g_SOS.Timer_ID  = a_SOS_s->Timer_ID = ZERO;
			g_SOS.Tick_Time = a_SOS_s->Tick_Time = ZERO;
			g_SOS.Timer_Cbk_ptr = NULL;
			g_SOS.State    = DISABLED;
			g_SOS_TickTime = g_SOS.Tick_Time = ZERO;
		}
		else
		{
			errorStauts = TMU_ERROR + NOT_INIT;
			return errorStauts;
		}
	}
	/*-------------[ In Case Of TMU's Null Pointer ]-------------*/
	else
	{
		errorStauts = TMU_ERROR + NULL_PTR;
		return errorStauts;
	}
	return errorStauts;
}