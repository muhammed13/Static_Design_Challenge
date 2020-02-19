/*
 ============================================================================
 Name        : SwICU.h
 Author      : Muhammed Gamal
 Description :
 ============================================================================
 */

#ifndef ICU_H_
#define ICU_H_

/************************************************************************/
/*				               Includes                                 */
/************************************************************************/
#include "DIO.h"
#include "registers.h"
#include "std_types.h"
#include "external_interrupt.h"
#include "Timers.h"
/************************************************************************/
/*				               Constants                                */
/************************************************************************/
#define ICU_CH0			(0)			/* defines External Interrupt 0 */
#define ICU_CH1			(1)       	/* defines External Interrupt 1 */
#define ICU_CH2			(2)			/* defines External Interrupt 2 */

#define ICU_TIMER_CH0	(0)			/* defines Timer 0 */
#define ICU_TIMER_CH1	(1)			/* defines Timer 1 */
#define ICU_TIMER_CH2	(2)			/* defines Timer 2 */

#define ICU_RISE_TO_RISE		(0)
#define ICU_RISE_TO_FALL		(1)
#define ICU_FALE_TO_RISE		(2)

/************************************************************************/
/*				               Structures                               */
/************************************************************************/
typedef struct Icu_cfg_s{
	uint8_t ICU_Ch_No;		/* To choose the External Interrupt number */
	uint8_t ICU_Ch_Timer;     /* To choose the Timer number */
}Icu_cfg_s;


/************************************************************************/
/*				               Global variables                               */
/************************************************************************/
extern volatile uint8_t g8_SwICU_value;
extern volatile void (*g_callBackPtr)(void);


extern volatile uint8_t g8_edgeCount;
extern volatile uint16_t g8_timeHigh;
extern volatile uint16_t g8_timePeriod;
extern volatile uint16_t g8_timePeriodPlusHigh;
extern volatile uint8_t g8_echo_flag;
/************************************************************************/
/*				          Functions' Prototypes                         */
/************************************************************************/

/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/
ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg);

/***************************************************************************
 * Function		: Icu_ReadTime
 * Input		:
 *				Icu_Channel --> ICU timer channel
 *									- ICU_TIMER_CH0
 *									- ICU_TIMER_CH1
 *									- ICU_TIMER_CH2
 *				Icu_EdgeToEdge -- > edges to calculate pusle time between:
 *									- ICU_RISE_TO_RISE
 *									- ICU_RISE_TO_FALL
 *									- ICU_FALE_TO_RISE					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from falling edge to rising edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between 2 edges				       *
 ***************************************************************************/
ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel, uint8_t Icu_EdgeToEdge, uint32_t * Icu_Time);




void Icu_setCallBack(void(*a_ptr)(void));

void Rising_EdgeFunc(void);

#endif /* ICU_H_ */
