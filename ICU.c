/*
 ============================================================================
 Name        : SwICU.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */

#include "ICU.h"

#define NULL_Ptr ((void*)0)

/* Global variables to hold the address of the call back function in the application */
volatile void (*g_callBackPtr)(void) = NULL_Ptr;


volatile uint8_t g8_SwICU_value;
volatile uint8_t g8_edgeCount;
volatile uint16_t g8_timeHigh;
volatile uint16_t g8_timePeriod;
volatile uint16_t g8_timePeriodPlusHigh;
volatile uint8_t g8_echo_flag;



volatile uint8_t g8_T2_StopFlag;

ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)
{
	ERROR_STATUS u8_status;
	St_TimerCfg timer0_stru_init=
		{
			timer0_stru_init.Timer_CH_NO=Timer_0,
			timer0_stru_init.Timer_Mode=TIMER_MODE,
			timer0_stru_init.Timer_Polling_Or_Interrupt=TIMER_POLLING_MODE,
			timer0_stru_init.Timer_Prescaler=T0_PRESCALER_64,
		};

	St_TimerCfg timer2_stru_init=
		{
			timer2_stru_init.Timer_CH_NO=Timer_2,
			timer2_stru_init.Timer_Mode=TIMER_MODE,
			timer2_stru_init.Timer_Polling_Or_Interrupt=TIMER_POLLING_MODE, //T2_INTERRUPT_NORMAL
			timer2_stru_init.Timer_Prescaler=T2_PRESCALER_1024,
		};

	switch(Icu_Cfg->ICU_Ch_No)
	{
	case ICU_CH0:
		//MCUCR |= (1<<ISC10);   /* Trigger INT0 with the any change*/
		INT0_Init(RISIGING_EDGE);
		u8_status=E_ok;
		break;

	case ICU_CH1:
		//MCUCR |= (1<<ISC10);   /* Trigger INT0 with the any change*/
		INT1_Init(RISIGING_EDGE);
		u8_status=E_ok;
		break;

	case ICU_CH2:
		//MCUCSR |= (1<<ISC2);   /* Trigger INT0 with the rising change*/
		INT2_Init(RISIGING_EDGE);
		u8_status=E_ok;
		break;

	default:
		u8_status=E_NOk;
		break;
	}

	switch(Icu_Cfg->ICU_Ch_Timer)
	{
	case Timer_0:
		Timer_Init(&timer0_stru_init);
		u8_status=E_ok;
		break;

	case Timer_1:
		/*cauldn't use timer1 since it used to perform other functionality*/
		u8_status=E_NOk;
		break;

	case Timer_2:
		Timer_Init(&timer2_stru_init);
		u8_status=E_ok;
		break;

	default:
		u8_status=E_NOk;
		break;
	}
	return u8_status;
}
/************************************************************************************/

ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel, uint8_t Icu_EdgeToEdge, uint32_t * Icu_Time)
{
	ERROR_STATUS u8_status;
	switch(Icu_Channel)
	{
	case Timer_0:
		switch(Icu_EdgeToEdge)
		{
		case ICU_RISE_TO_RISE:
			INT2_Init(RISIGING_EDGE);
			//MCUCSR |= (1<<ISC2);
			Icu_setCallBack(Rising_EdgeFunc);
			*Icu_Time=(g8_timePeriod)*4;
			u8_status=E_ok;
			break;

		case ICU_RISE_TO_FALL:
			INT2_Init(RISIGING_EDGE);
			//MCUCSR |= (1<<ISC2);
			Icu_setCallBack(Rising_EdgeFunc);
			*Icu_Time=(g8_timeHigh)*4;
			u8_status=E_ok;
			break;

		case ICU_FALE_TO_RISE:
			INT2_Init(RISIGING_EDGE);
			//MCUCSR |= (1<<ISC2);
			Icu_setCallBack(Rising_EdgeFunc);
			*Icu_Time=(g8_timePeriod-g8_timeHigh)*4;
			u8_status=E_ok;
			break;

		default:
			u8_status=E_NOk;
			break;
		}
	break;

	case Timer_1:
		/*no need to handle timer1 for ICU since it is used to perform another functionality*/
		u8_status=E_NOk;
	break;

	case Timer_2:
		switch(Icu_EdgeToEdge)
		{
		case ICU_RISE_TO_RISE:
			INT2_Init(RISIGING_EDGE);
			//MCUCSR |= (1<<ISC2);
			Icu_setCallBack(Rising_EdgeFunc);
			*Icu_Time=(g8_timePeriod)*64;
			u8_status=E_ok;
			break;

		case ICU_RISE_TO_FALL:
			//INT2_Init(RISIGING_EDGE);
			//MCUCSR |= (1<<ISC2);

			Icu_setCallBack(Rising_EdgeFunc);
			*Icu_Time=(g8_timeHigh)*64;   //+(T2_OVF_counter)*255
			//T2_OVF_counter=0;
			u8_status=E_ok;
			break;

		case ICU_FALE_TO_RISE:
			//INT2_Init(RISIGING_EDGE);
			MCUCSR |= (1<<ISC2);
			Icu_setCallBack(Rising_EdgeFunc);
			*Icu_Time=(g8_timePeriod-g8_timeHigh)*64;
			u8_status=E_ok;
			break;

		default:
			u8_status=E_NOk;
			break;
		}
	break;

	default:
		u8_status=E_NOk;
	}
	return u8_status;
}
/**********************************callback function*************************************/

void Icu_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/**************************************************************************************/


void Rising_EdgeFunc(void)
{
	g8_edgeCount++;
	if(g8_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Timer_Clear(Timer_2);
		Timer_Start(Timer_2,TIMER0_NUMBER_OF_TICKS);
		/* Detect falling edge */
		INT2_SetEdge(FALLING_EDGE);
		//T2_OVF_counter=0;
	}
	else if(g8_edgeCount == 2)
	{
		/* Store the High time value */
		Timer_GetValue(Timer_2,&g8_timeHigh);
		/* Detect rising edge */
		INT2_SetEdge(RISIGING_EDGE);
		g8_echo_flag=1;
	}
	else if(g8_edgeCount == 3)
	{
		/* Store the Period time value */
		Timer_GetValue(Timer_2,&g8_timePeriod);
		/* Detect falling edge */
		INT2_SetEdge(FALLING_EDGE);
		//T2_OVF_counter=0;
	}
	else if(g8_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		//g8_timePeriodPlusHigh = TCNT0;
		Timer_GetValue(Timer_2,&g8_timePeriodPlusHigh);
		/* Clear the timer counter register to start measurements again */
		Timer_Clear(Timer_2);
		/* Detect rising edge */
		INT2_SetEdge(RISIGING_EDGE);
		g8_edgeCount=0;
		g8_echo_flag=1;
	}
}

/*
void Rising_EdgeFunc1(void)
{
	if(g8_INT2_edge==RISIGING_EDGE)
	{

		Timer_Start(Timer_2,TIMER0_NUMBER_OF_TICKS);
		T2_OVF_counter=0;
		TCNT2=0;
	}
	else
	{

		Timer_Stop(Timer_2);
		g8_T2_StopFlag=1;
	}
}
*/
