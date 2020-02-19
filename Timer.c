/*
 ============================================================================
 Name        : AGILE_REQ2.h
 Author      : Muhammed Gamal
 Description :this file contains the timer driver
 ============================================================================
 */


/*********************************includes**********************************/
#include "Timers.h"
/***************************************************************************/


/*********************************definitions*******************************/
#define TIMER_ICR1_TOP_VALUE 2499
#define NULL_Ptr ((void*)0)
#define TIMER0_MAX_COUNT 256
#define TIMER2_MAX_COUNT 256
#define TIMER1_MAX_COUNT 65536
#define CLEAR 0
/***************************************************************************/


/******************************global variables*****************************/
uint8_t g8_TCNT0_value_Swpwm_ISR;
static uint16_t T0_PrescallerValue;
static uint16_t T1_PrescallerValue;
static uint16_t T2_PrescallerValue;
volatile uint16_t T2_OVF_counter=0;
/***************************************************************************/




/**
 * Input: Pointer to a structure contains the information needed to initialize the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */
ERROR_STATUS Timer_Init(St_TimerCfg* Timer_cfg)
{
	uint8_t u8_status;
	if(Timer_cfg == NULL_Ptr)
	{
		u8_status=E_NOk;
	}
	else
	{
		TIMSK = Timer_cfg->Timer_Polling_Or_Interrupt;
		switch(Timer_cfg->Timer_CH_NO)
		{
			case Timer_0:
				TCCR0 = Timer_cfg->Timer_Mode | (1<<FOC0);
				T0_PrescallerValue=Timer_cfg->Timer_Prescaler;
				TIMSK=Timer_cfg->Timer_Polling_Or_Interrupt;
				u8_status=E_ok;
				break;
			case Timer_1:
				TCCR1A =Timer_cfg->Timer_Mode | (1<<FOC1A) | (1<<FOC1B);
				T1_PrescallerValue=Timer_cfg->Timer_Prescaler;
				TIMSK=Timer_cfg->Timer_Polling_Or_Interrupt;
				u8_status=E_ok;
				break;
			case Timer_2:
				TCCR2 =Timer_cfg->Timer_Mode | (1<<FOC0);
				T2_PrescallerValue=Timer_cfg->Timer_Prescaler;
				TIMSK=Timer_cfg->Timer_Polling_Or_Interrupt;
				u8_status=E_ok;
				break;
			default:
				u8_status=E_NOk;
		}
	}
	return u8_status;
}





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
ERROR_STATUS Timer_Start(uint8_t Timer_CH_NO, uint16_t Timer_Count)
{
	uint8_t u8_status;
	switch(Timer_CH_NO)
	{
		case Timer_0:
			if(Timer_Count<=TIMER0_MAX_COUNT)
			{
				TCNT0 = (TIMER0_MAX_COUNT-Timer_Count);
				TCCR0 |= T0_PrescallerValue;
				u8_status=E_ok;
			}
			else
			{
				u8_status=E_NOk;
			}
			break;

		case Timer_1:
			if(Timer_Count<=TIMER1_MAX_COUNT)
			{
				TCNT1H = (((TIMER1_MAX_COUNT-Timer_Count) & 0xFF00)>>8);
				TCNT1L = ((TIMER1_MAX_COUNT-Timer_Count) & 0x00FF);
				TCCR1B = T1_PrescallerValue;
				u8_status=E_ok;
			}
			else
			{
				u8_status=E_NOk;
			}
			break;

		case Timer_2:
			if(Timer_Count<=TIMER0_MAX_COUNT)
			{
				TCNT2 = (TIMER2_MAX_COUNT-Timer_Count);
				TCCR2 |= T2_PrescallerValue;
				u8_status=E_ok;
			}
			else
			{
				u8_status=E_NOk;
			}

			break;

		default:
			u8_status=E_NOk;
	}
	return u8_status;
}



/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the needed timer.
 *
 */
ERROR_STATUS Timer_Stop(uint8_t Timer_CH_NO)
{
	uint8_t u8_status;
	switch(Timer_CH_NO)
	{
		case Timer_0:
			TCCR0 = T0_NO_CLOCK;
			u8_status=E_ok;
			break;
		case Timer_1:
			TCCR1 = T1_NO_CLOCK;
			u8_status=E_ok;
			break;
		case Timer_2:
			TCCR2 = T2_NO_CLOCK;
			u8_status=E_ok;
			break;
		default:
			u8_status=E_NOk;
			break;
	}
	return u8_status;
}




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
ERROR_STATUS Timer_GetStatus(uint8_t Timer_CH_NO, bool_t* Data)
{
	uint8_t u8_status;
	switch(Timer_CH_NO)
	{
		case Timer_0:
			if(TIFR & (1<<TOV0))
			{
				*Data=FLAG_RAISED;
			}
			else
			{
				*Data=FLAG_DOWN;
			}
			u8_status=E_ok;
			break;

		case Timer_1:
			if(TIFR & (1<<TOV1))
			{
				*Data=FLAG_RAISED;
			}
			else
			{
				*Data=FLAG_DOWN;
			}
			u8_status=E_ok;
			break;

		case Timer_2:
			if(TIFR & (1<<TOV2))
			{
				*Data=FLAG_RAISED;
			}
			else
			{
				*Data=FLAG_DOWN;
			}
			u8_status=E_ok;
			break;

		default:
			u8_status=E_NOk;
	}
	return u8_status;
}



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
ERROR_STATUS Timer_GetValue(uint8_t Timer_CH_NO, uint16_t* Data)
{
	uint8_t u8_status;
	switch(Timer_CH_NO)
	{
		case Timer_0:
			*Data = TCNT0;
			u8_status=E_ok;
			break;
		case Timer_1:
			*Data = TCNT1;
			u8_status=E_ok;
			break;
		case Timer_2:
			*Data = TCNT2;
			u8_status=E_ok;
			break;
		default:
			u8_status=E_NOk;
	}
	return u8_status;
}



ERROR_STATUS Timer_Clear(uint8_t Timer_CH_NO)
{
	uint8_t u8_status;
	switch(Timer_CH_NO)
	{
		case Timer_0:
			TCNT0= CLEAR;
			u8_status=E_ok;
			break;
		case Timer_1:
			TCNT1 = CLEAR;
			u8_status=E_ok;
			break;
		case Timer_2:
			TCNT2 = CLEAR;
			u8_status=E_ok;
			break;
		default:
			u8_status=E_NOk;
			break;
	}
	return u8_status;
}
/***************************************************************************************/


/**************************************************************************************
                                 TIMERS ISRs
**************************************************************************************/

ISR(TIMER2_OVF_vect)
{
	//TCNT0=T2_OVF_counter;
	T2_OVF_counter++;
}
