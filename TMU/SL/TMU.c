/*
 * TMU.c
 *
 * Created: 2/23/2020 9:36:01 PM
 *  Author: Khaled Magdy
 */ 
#include "TMU.h"
#include "TMU_cfg.h"


typedef struct Consumer_s{
	FunPtr Consumer_Ptr;
	uint8_t Periodicity;
	uint32_t Time;
	uint8_t State;
	uint32_t Count;
	uint16_t ConsumerID;
} Consumer_s;

static Timer_cfg_s g_TMU_TMR;
static TMU_cfg_s   g_TMU = {ZERO, ZERO, DISABLED};
static Consumer_s  g_RequestBuffer[REQUEST_BUFFER_LEN];

volatile static uint16_t g_ReqBuffer_Index   = ZERO;
volatile static uint8_t  g_TMR_Ticks_Changed = ZERO;
volatile static uint16_t g_TMU_TickTime      = ZERO;
volatile static uint16_t g_SysTicks          = ZERO;


static void TMU_ISR_cbf(void)
{
	/* Adjust The Tick Time According To The TMU's Time Configuration */
	g_SysTicks++;
	
	if(g_SysTicks == g_TMU_TickTime)
	{
		g_TMR_Ticks_Changed = TRUE;
		g_SysTicks = FALSE;	
	}
}

ERROR_STATUS TMU_Init(TMU_cfg_s* a_TMU_s)
{
	ERROR_STATUS errorStauts = E_OK;
	uint16_t index = 0;
	
	/*-------------[ Check TMU's Pointer Validity ]-------------*/
	if(a_TMU_s != NULL)
	{
		/*-------------[ TMU Initialization ]-------------*/
		g_TMU.Timer_ID  = a_TMU_s->Timer_ID;
		g_TMU.Tick_Time = a_TMU_s->Tick_Time;
		g_TMU.Timer_Cbk_ptr = TMU_ISR_cbf;
		g_TMU.State    = INACTIVE;
		g_TMU_TickTime = g_TMU.Tick_Time;
		
		/*-------------[ Timer Initialization ]-------------*/
		g_TMU_TMR.Timer_Mode = TIMER_MODE;
		g_TMU_TMR.Timer_Prescaler = TIMER_PRESCALER_64;
		g_TMU_TMR.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		g_TMU_TMR.Timer_Cbk_ptr = g_TMU.Timer_Cbk_ptr;
		
		/* Select The Timer Channel & Set It's CallBack Function */
		switch(a_TMU_s->Timer_ID)
		{
			case TIMER_CH0:
				g_TMU_TMR.Timer_CH_NO = TIMER_CH0;
				TIMER0_SetCM(TMU_ISR_cbf);
				break;
			case TIMER_CH1:
				g_TMU_TMR.Timer_CH_NO = TIMER_CH1;
				TIMER1_SetCM(TMU_ISR_cbf);
				break;
			case TIMER_CH2:
				g_TMU_TMR.Timer_CH_NO = TIMER_CH2;
				TIMER2_SetCM(TMU_ISR_cbf);
				break;
			default:
				errorStauts = TMU_ERROR + INVALID_IN;
				return errorStauts;
		}
		
		/* Apply The Settings & Start The TMU Timer Hardware Module */
		Timer_Init(&g_TMU_TMR);
		Timer_Start(g_TMU_TMR.Timer_CH_NO, TMR_Ticks);	
		
		/*  Initialize The Request Buffer  */
		for(index = 0; index < REQUEST_BUFFER_LEN; index++)
		{
			g_RequestBuffer[index].State = INACTIVE;
			g_RequestBuffer[index].Consumer_Ptr = NULL;
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


ERROR_STATUS TMU_Start(FunPtr a_ConsumerFun, uint16_t a_ConsumerID, uint8_t a_Periodic_OneShot, uint32_t a_Time)
{
	ERROR_STATUS a_errorStatus = E_OK;
	uint16_t index = 0;
	
	/*  Create New Consumer Instance & Initialize It  */
	Consumer_s a_NewConsumer;
	a_NewConsumer.Consumer_Ptr = a_ConsumerFun;
	a_NewConsumer.ConsumerID = a_ConsumerID;
	a_NewConsumer.Time = a_Time;
	a_NewConsumer.Periodicity = a_Periodic_OneShot;
	a_NewConsumer.Count = ZERO;
	a_NewConsumer.State = ACTIVE;
	
	if(g_TMU.State == INACTIVE || g_TMU.State == ACTIVE)
	{
		/*-------------[ Check Consumer's CBF Pointer Validity ]-------------*/
		if(a_ConsumerFun != NULL)
		{
			/*  In Case OF Full Request Buffer  */
			if(g_ReqBuffer_Index == REQUEST_BUFFER_LEN)
			{
				/*  Search For Inactive Consumer & Overwrite It */
				for(index = 0; index < REQUEST_BUFFER_LEN; index++)
				{
					if(g_RequestBuffer[index].State == INACTIVE)
					{
						g_RequestBuffer[index] = a_NewConsumer;
						break;
					}
				}
				/*  If All Consumers In The Buffer Are Active & No Space If Available */
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
					if(g_RequestBuffer[index].ConsumerID == a_ConsumerID)
					{
						a_errorStatus = TMU_ERROR + MULTI_START;
						return a_errorStatus;
					}
				}
				/* Add The New Consumer To The Request Buffer */
				g_RequestBuffer[g_ReqBuffer_Index++] = a_NewConsumer;				
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

ERROR_STATUS TMU_Stop(uint16_t a_ConsumerID)
{
	ERROR_STATUS errorStatus = E_NOK;
	uint16_t a_u16_index;
	
	/*-------------[ Search For The Consumer ID In The Request Buffer ]-------------*/
	for(a_u16_index = ZERO; a_u16_index < REQUEST_BUFFER_LEN; a_u16_index++)
	{
		/*-------------[ When It's Found, Deactivate That Consumer ]-------------*/
		if(g_RequestBuffer[a_u16_index].ConsumerID == a_ConsumerID)
		{
			g_RequestBuffer[a_u16_index].State = INACTIVE;
			errorStatus = E_OK;
			return errorStatus;
		}
	}
	
	/*-------------[ Consumer Not Found In Request Buffer ]-------------*/
	return errorStatus;
}


void TMU_Dispatcher(void)
{
	uint16_t a_u16_index;
	FunPtr a_ConsumerFunction;
	
	/*-------------[ Every TMU Tick, Go Through The Request Buffer ]-------------*/
	if(g_TMR_Ticks_Changed)
	{
		for(a_u16_index = ZERO; a_u16_index < REQUEST_BUFFER_LEN; a_u16_index++)
		{
			g_RequestBuffer[a_u16_index].Count += g_TMU_TickTime;
			
			/*-------------[ IF Consumer's Due Time Is Met!  ]-------------*/
			if(g_RequestBuffer[a_u16_index].Count >= g_RequestBuffer[a_u16_index].Time)
			{
				a_ConsumerFunction = g_RequestBuffer[a_u16_index].Consumer_Ptr;
				
				/* IF The Consumer Function IS Periodic */
				if(g_RequestBuffer[a_u16_index].Periodicity == PERIODIC && g_RequestBuffer[a_u16_index].State == ACTIVE)
				{
					g_RequestBuffer[a_u16_index].Count = ZERO;
					a_ConsumerFunction();
				}
				
				/* Else IF The Consumer Function IS OneShot (Non-Periodic) */
				else if (g_RequestBuffer[a_u16_index].Periodicity == ONESHOT && g_RequestBuffer[a_u16_index].State == ACTIVE)
				{
					g_RequestBuffer[a_u16_index].State = INACTIVE;
					a_ConsumerFunction();
				}
			}
		}
		g_TMR_Ticks_Changed = FALSE;
	}
}

ERROR_STATUS TMU_DeInit(TMU_cfg_s* a_TMU_s)
{
	ERROR_STATUS errorStauts = E_OK;
	
	/*-------------[ Check TMU's Pointer Validity ]-------------*/
	if(a_TMU_s != NULL)
	{
		if(g_TMU.State == ACTIVE || g_TMU.State == INACTIVE)
		{
			/*-------------[ TMU DeInitialization ]-------------*/
			Timer_Stop(g_TMU.Timer_ID);
			g_TMU.Timer_ID  = a_TMU_s->Timer_ID = ZERO;
			g_TMU.Tick_Time = a_TMU_s->Tick_Time = ZERO;
			g_TMU.Timer_Cbk_ptr = NULL;
			g_TMU.State    = DISABLED;
			g_TMU_TickTime = g_TMU.Tick_Time = ZERO;
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