/*
 ============================================================================
 Name        : PWM.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */


/*********************************includes**********************************/
#include "PWM.h"
/***************************************************************************/


/*********************************definitions*******************************/
#define TIMER_ICR1_TOP_VALUE 2499
#define NULL_Ptr ((void*)0)
#define TIMER0_MAX_COUNT 256
#define TIMER2_MAX_COUNT 256
#define TIMER1_MAX_COUNT 65536
/***************************************************************************/


/******************************global variables*****************************/
uint8_t g8_TCNT0_value_Swpwm_ISR;
static uint16_t T0_PrescallerValue=0;
static uint16_t T1_PrescallerValue=0;
static uint16_t T2_PrescallerValue=0;
/***************************************************************************/



ERROR_STATUS Pwm_Init(Pwm_Cfg_s *Pwm_Cfg)
{
	uint8_t u8_status;
	if(Pwm_Cfg == NULL_Ptr)
	{
		u8_status = E_NOk;
	}
	else
	{
		TCNT0 = 0; /*timer initial value*/
		TIMSK = 0; /*disable interrupts*/
		//OCR0 = (u8_dutyCycle*TIMER0_NUMBER_OF_TICKS)/100; //output compare value
		switch(Pwm_Cfg->Channel)
		{
			case PWM_CH0:
				T0_PrescallerValue = Pwm_Cfg->Prescaler;
				TCCR0 =(1<<COM01)| (1<<WGM00);
				u8_status = E_ok;
				DIO_Cfg_s PWM0_Binsinit=
				{
						PWM0_Binsinit.GPIO=GPIOB,
						PWM0_Binsinit.pins=BIT3,
						PWM0_Binsinit.dir=OUTPUT,
				};
				DIO_init (&PWM0_Binsinit);
			break;

			case PWM_CH1A:
				T1_PrescallerValue = Pwm_Cfg->Prescaler;
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 3. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU
				 */
				TCCR1B =(1<<WGM13);
				u8_status = E_ok;
				DIO_Cfg_s PWM1AB_Binsinit=
				{
						PWM1AB_Binsinit.GPIO=GPIOD,
						PWM1AB_Binsinit.pins=BIT5,
						PWM1AB_Binsinit.dir=OUTPUT,
				};
				DIO_init (&PWM1AB_Binsinit);
			break;

			case PWM_CH1B:
				T1_PrescallerValue = Pwm_Cfg->Prescaler;
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				 * 2. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 3. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU
				 */
				TCCR1B =(1<<WGM13);
				u8_status = E_ok;
				DIO_Cfg_s PWM1B_Binsinit=
				{
						PWM1B_Binsinit.GPIO=GPIOD,
						PWM1B_Binsinit.pins=BIT4,
						PWM1B_Binsinit.dir=OUTPUT,
				};
				DIO_init (&PWM1B_Binsinit);
			break;

			case PWM_CH2:
				T2_PrescallerValue = Pwm_Cfg->Prescaler;
				TCCR2 =(1<<COM21)| (1<<WGM20);
				u8_status = E_ok;
				DIO_Cfg_s PWM2_Binsinit=
				{
						PWM2_Binsinit.GPIO=GPIOD,
						PWM2_Binsinit.pins=BIT7,
						PWM2_Binsinit.dir=OUTPUT,
				};
				DIO_init (&PWM2_Binsinit);
			break;

			case PWM_CH1A_CH1B:
				T1_PrescallerValue = Pwm_Cfg->Prescaler;
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 3. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU
				 */
				TCCR1B =(1<<WGM13);
				u8_status = E_ok;
				DIO_Cfg_s PWM1A_Binsinit=
				{
						PWM1A_Binsinit.GPIO=GPIOD,
						PWM1A_Binsinit.pins=BIT4 | BIT5,
						PWM1A_Binsinit.dir=OUTPUT,
				};
				DIO_init (&PWM1A_Binsinit);
			break;
			default:
				u8_status = E_NOk;
		}
	}
	return u8_status;
}
/**************************************************************************************/

ERROR_STATUS Pwm_Start(uint8_t Channel,uint8_t Duty,uint32_t Frequncy)
{
	uint8_t u8_status;
	switch(Channel)
	{
	case PWM_CH0:
		OCR0 = (Duty*TIMER0_MAX_COUNT)/100; /*output compare value*/
		switch(Frequncy)
		{
		case(T0_PHASE_CORRECT_FREQUENCY_31KHZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM01=0 & WGM00=1
				 * 3. clear OC0 on compare match COM00=0 & COM01=1
				 * 4. clock = F_CPU CS00=1 CS01=0 CS02=0
				 */
				TCCR0 =PWM_PRESCALER_NO | (1<<COM01)| (1<<WGM00);
				u8_status = E_ok;
				break;

		case(T0_PHASE_CORRECT_FREQUENCY_4KHZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM01=0 & WGM00=1
				 * 3. clear OC0 on compare match COM00=0 & COM01=1
				 * 4. clock = F_CPU/8 CS00=1 CS01=0 CS02=0
				 */
				TCCR0 =PWM_PRESCALER_8 | (1<<COM01)| (1<<WGM00);
				u8_status = E_ok;
				break;

		case(T0_PHASE_CORRECT_FREQUENCY_500HZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM01=0 & WGM00=1
				 * 3. clear OC0 on compare match COM00=0 & COM01=1
				 * 4. clock = F_CPU/64 CS00=1 CS01=1 CS02=0
				 */
				TCCR0 =PWM_PRESCALER_64 | (1<<COM01)| (1<<WGM00);
				u8_status = E_ok;
				break;

		case(T0_PHASE_CORRECT_FREQUENCY_122HZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM01=0 & WGM00=1
				 * 3. clear OC0 on compare match COM00=0 & COM01=1
				 * 4. clock = F_CPU/256 CS00=0 CS01=0 CS02=1
				 */
				TCCR0 =PWM_PRESCALER_256 | (1<<COM01)| (1<<WGM00);
				u8_status = E_ok;
				break;

		case(T0_PHASE_CORRECT_FREQUENCY_31HZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM01=0 & WGM00=1
				 * 3. clear OC0 on compare match COM00=0 & COM01=1
				 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
				 */
				TCCR0 =PWM_PRESCALER_1024 | (1<<COM01)| (1<<WGM00);
				u8_status = E_ok;
				break;

		default:
			u8_status = E_NOk;
			break;
		}
	break;

	case PWM_CH1A:
		OCR1A = (Duty)*(TIMER_ICR1_TOP_VALUE/100);   /*output compare value*/
		switch(Frequncy)
			{

			case(T1_PHASE_CORRECT_FREQUENCY_3KHZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1A1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU
					 */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_NO;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_400HZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1A1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/8
					 */
					TCCR1B = (1<<WGM13) | PWM_PRESCALER_8;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_50HZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1A1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/64
					 */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_64;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_13HZ):  //90hz
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
				     * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				     * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				     * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				     * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1A1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/256
				     */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_256;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_3HZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1A1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/1024
					 */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_1024;
					u8_status = E_ok;
					break;

			default:
				u8_status = E_NOk;
				break;
			}
	break;

	case PWM_CH1B:
		OCR1B = (Duty)*(TIMER_ICR1_TOP_VALUE/100);   /*output compare value*/
		switch(Frequncy)
			{
			case(T1_PHASE_CORRECT_FREQUENCY_3KHZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1B1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU
					 */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_NO;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_400HZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1B1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/8
					 */
					TCCR1B = (1<<WGM13) | PWM_PRESCALER_8;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_50HZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1B1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/64
					 */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_64;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_13HZ):  //90hz
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
				     * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				     * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				     * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				     * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1B1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/256
				     */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_256;
					u8_status = E_ok;
					break;

			case(T1_PHASE_CORRECT_FREQUENCY_3HZ):
					ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
					/* Configure timer control register TCCR1A
					 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
					 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
					 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
					 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
					 */
					TCCR1A = (1<<WGM11) | (1<<COM1B1);

					/* Configure timer control register TCCR1A
					 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
					 * 2. Prescaller = F_CPU/1024
					 */
					TCCR1B =(1<<WGM13) | PWM_PRESCALER_1024;
					u8_status = E_ok;
					break;

				default:
					u8_status = E_NOk;
					break;
			}
	break;

	case PWM_CH2:
		OCR2 = (Duty*TIMER2_MAX_COUNT)/100; /*output compare value*/
		switch(Frequncy)
		{
		case(T2_PHASE_CORRECT_FREQUENCY_31KHZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM21=0 & WGM20=1
				 * 3. clear OC0 on compare match COM20=0 & COM21=1
				 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
				 */
				TCCR2 =PWM_PRESCALER_NO | (1<<COM21)| (1<<WGM20);
				break;

		case(T2_PHASE_CORRECT_FREQUENCY_4KHZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM21=0 & WGM20=1
				 * 3. clear OC0 on compare match COM20=0 & COM21=1
				 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
				 */
				TCCR2 =PWM_PRESCALER_8 | (1<<COM21)| (1<<WGM20);
				break;

		case(T2_PHASE_CORRECT_FREQUENCY_500HZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM21=0 & WGM20=1
				 * 3. clear OC0 on compare match COM20=0 & COM21=1
				 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
				 */
				TCCR2 =PWM_PRESCALER_64 | (1<<COM21)| (1<<WGM20);
				break;

		case(T2_PHASE_CORRECT_FREQUENCY_122HZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM21=0 & WGM20=1
				 * 3. clear OC0 on compare match COM20=0 & COM21=1
				 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
				 */
				TCCR2 =PWM_PRESCALER_256 | (1<<COM21)| (1<<WGM20);
				break;

		case(T2_PHASE_CORRECT_FREQUENCY_31HZ):
				/* Configure the timer control register
				 * 1. PWM mode FOC0=0
				 * 2. phase correct PWM mode WGM21=0 & WGM20=1
				 * 3. clear OC0 on compare match COM20=0 & COM21=1
				 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
				 */
				TCCR2 =PWM_PRESCALER_1024 | (1<<COM21)| (1<<WGM20);
				break;

		default:
			u8_status = E_NOk;
			break;
		}
	break;

	case PWM_CH1A_CH1B:
		OCR1A = (Duty)*(TIMER_ICR1_TOP_VALUE/100);   /*output compare value*/
		OCR1B = (Duty)*(TIMER_ICR1_TOP_VALUE/100);   /*output compare value*/
		switch(Frequncy)
		{
		case(T1_PHASE_CORRECT_FREQUENCY_3KHZ):
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU
				 */
				TCCR1B =(1<<WGM13) | PWM_PRESCALER_NO;
				u8_status = E_ok;
				break;

		case(T1_PHASE_CORRECT_FREQUENCY_400HZ):
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU/8
				 */
				TCCR1B = (1<<WGM13) | PWM_PRESCALER_8;
				u8_status = E_ok;
				break;

		case(T1_PHASE_CORRECT_FREQUENCY_50HZ):
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU/64
				 */
				TCCR1B =(1<<WGM13) | PWM_PRESCALER_64;
				u8_status = E_ok;
				break;

		case(T1_PHASE_CORRECT_FREQUENCY_13HZ):  //90hz
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU/256
				 */
				TCCR1B =(1<<WGM13) | PWM_PRESCALER_256;
				u8_status = E_ok;
				break;

		case(T1_PHASE_CORRECT_FREQUENCY_3HZ):
				ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
				/* Configure timer control register TCCR1A
				 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
				 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
				 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
				 * 4. phase correct Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 10)
				 */
				TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);

				/* Configure timer control register TCCR1A
				 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 10)
				 * 2. Prescaller = F_CPU/1024
				 */
				TCCR1B =(1<<WGM13) | PWM_PRESCALER_1024;
				u8_status = E_ok;
				break;

			default:
				u8_status = E_NOk;
				break;
		}

	break;

	default:
		u8_status = E_NOk;
	}
	return u8_status;
}
/**************************************************************************************/
ERROR_STATUS Pwm_Update(uint8_t Channel,uint8_t Duty,uint32_t Frequncy)
{
	uint8_t u8_status;
	switch (Channel)
	{
		case PWM_CH0:
			if(T0_PrescallerValue==0)
			{
				u8_status = E_NOk;
			}
			else
			{
				OCR0 = (Duty*TIMER0_MAX_COUNT)/100; /*output compare value*/
				switch(Frequncy)
				{
				case(T0_PHASE_CORRECT_FREQUENCY_31KHZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM01=0 & WGM00=1
						 * 3. clear OC0 on compare match COM00=0 & COM01=1
						 * 4. clock = F_CPU CS00=1 CS01=0 CS02=0
						 */
						TCCR0 =PWM_PRESCALER_NO | (1<<COM01)| (1<<WGM00);
						u8_status = E_ok;
						break;

				case(T0_PHASE_CORRECT_FREQUENCY_4KHZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM01=0 & WGM00=1
						 * 3. clear OC0 on compare match COM00=0 & COM01=1
						 * 4. clock = F_CPU/8 CS00=1 CS01=0 CS02=0
						 */
						TCCR0 =PWM_PRESCALER_8 | (1<<COM01)| (1<<WGM00);
						u8_status = E_ok;
						break;

				case(T0_PHASE_CORRECT_FREQUENCY_500HZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM01=0 & WGM00=1
						 * 3. clear OC0 on compare match COM00=0 & COM01=1
						 * 4. clock = F_CPU/64 CS00=1 CS01=1 CS02=0
						 */
						TCCR0 =PWM_PRESCALER_64 | (1<<COM01)| (1<<WGM00);
						u8_status = E_ok;
						break;

				case(T0_PHASE_CORRECT_FREQUENCY_122HZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM01=0 & WGM00=1
						 * 3. clear OC0 on compare match COM00=0 & COM01=1
						 * 4. clock = F_CPU/256 CS00=0 CS01=0 CS02=1
						 */
						TCCR0 =PWM_PRESCALER_256 | (1<<COM01)| (1<<WGM00);
						u8_status = E_ok;
						break;

				case(T0_PHASE_CORRECT_FREQUENCY_31HZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM01=0 & WGM00=1
						 * 3. clear OC0 on compare match COM00=0 & COM01=1
						 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
						 */
						TCCR0 =PWM_PRESCALER_1024 | (1<<COM01)| (1<<WGM00);
						u8_status = E_ok;
						break;

				default:
					u8_status = E_NOk;
					break;
				}
			}
		break;

		case PWM_CH1A:
			if(T1_PrescallerValue==0)
			{
				u8_status = E_NOk;
			}
			else
			{
				OCR1A = (Duty)*(TIMER_ICR1_TOP_VALUE/100);   /*output compare value*/
				switch(Frequncy)
				{

				case(T1_PHASE_CORRECT_FREQUENCY_3KHZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1A1);

						/* Configure timer control register TCCR1A
						 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
						 * 2. Prescaller = F_CPU
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_NO;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_400HZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1A1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/8
						 */
						TCCR1B = (1<<WGM13) | PWM_PRESCALER_8;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_50HZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1A1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/64
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_64;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_13HZ):  //90hz
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1A1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/256
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_256;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_3HZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1A1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/1024
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_1024;
						u8_status = E_ok;
						break;

				default:
					u8_status = E_NOk;
					break;
				}
			}
		break;

		case PWM_CH1B:
			if(T1_PrescallerValue==0)
			{
				u8_status = E_NOk;
			}
			else
			{
				OCR1B = (Duty)*(TIMER_ICR1_TOP_VALUE/100);   /*output compare value*/
				switch(Frequncy)
				{
				case(T1_PHASE_CORRECT_FREQUENCY_3KHZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1B1);

						/* Configure timer control register TCCR1A
						 * 1. phase correct Pwm Mode with the TOP in ICR1 WGM12=0 WGM13=1 (Mode Number 10)
						 * 2. Prescaller = F_CPU
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_NO;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_400HZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1B1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/8
						 */
						TCCR1B = (1<<WGM13) | PWM_PRESCALER_8;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_50HZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1B1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/64
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_64;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_13HZ):  //90hz
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1B1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/256
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_256;
						u8_status = E_ok;
						break;

				case(T1_PHASE_CORRECT_FREQUENCY_3HZ):
						ICR1 = TIMER_ICR1_TOP_VALUE;	/* Set TOP count for timer1 in ICR1 register */
						/* Configure timer control register TCCR1A
						 * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
						 * 2. Clear OC1B on compare match (non inverting mode) COM1B1=1 COM1B0=0
						 * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
						 * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
						 */
						TCCR1A = (1<<WGM11) | (1<<COM1B1);

						/* Configure timer control register TCCR1A
						 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
						 * 2. Prescaller = F_CPU/1024
						 */
						TCCR1B =(1<<WGM13) | PWM_PRESCALER_1024;
						u8_status = E_ok;
						break;

					default:
						u8_status = E_NOk;
						break;
				}
			}
		break;

		case PWM_CH2:
			if(T2_PrescallerValue==0)
			{
				u8_status = E_NOk;
			}
			else
			{
				OCR2 = (Duty*TIMER2_MAX_COUNT)/100; /*output compare value*/
				switch(Frequncy)
				{
				case(T2_PHASE_CORRECT_FREQUENCY_31KHZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM21=0 & WGM20=1
						 * 3. clear OC0 on compare match COM20=0 & COM21=1
						 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
						 */
						TCCR2 =PWM_PRESCALER_NO | (1<<COM21)| (1<<WGM20);
						u8_status = E_ok;
						break;

				case(T2_PHASE_CORRECT_FREQUENCY_4KHZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM21=0 & WGM20=1
						 * 3. clear OC0 on compare match COM20=0 & COM21=1
						 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
						 */
						TCCR2 =PWM_PRESCALER_8 | (1<<COM21)| (1<<WGM20);
						u8_status = E_ok;
						break;

				case(T2_PHASE_CORRECT_FREQUENCY_500HZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM21=0 & WGM20=1
						 * 3. clear OC0 on compare match COM20=0 & COM21=1
						 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
						 */
						TCCR2 =PWM_PRESCALER_64 | (1<<COM21)| (1<<WGM20);
						u8_status = E_ok;
						break;

				case(T2_PHASE_CORRECT_FREQUENCY_122HZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM21=0 & WGM20=1
						 * 3. clear OC0 on compare match COM20=0 & COM21=1
						 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
						 */
						TCCR2 =PWM_PRESCALER_256 | (1<<COM21)| (1<<WGM20);
						u8_status = E_ok;
						break;

				case(T2_PHASE_CORRECT_FREQUENCY_31HZ):
						/* Configure the timer control register
						 * 1. PWM mode FOC0=0
						 * 2. phase correct PWM mode WGM21=0 & WGM20=1
						 * 3. clear OC0 on compare match COM20=0 & COM21=1
						 * 4. clock = F_CPU/1024 CS20=1 CS21=0 CS22=1
						 */
						TCCR2 =PWM_PRESCALER_1024 | (1<<COM21)| (1<<WGM20);
						u8_status = E_ok;
						break;

				default:
					u8_status = E_NOk;
				}
			}
		break;

		default:
			u8_status = E_NOk;
	}
	return u8_status;
}
/**************************************************************************************/
extern ERROR_STATUS Pwm_Stop(uint8_t Channel)
{
	uint8_t u8_status;
	switch(Channel)
	{
	case PWM_CH0:
		TCCR0=0;
		u8_status = E_ok;
		break;

	case PWM_CH1A:
		TCCR1A=0;    /*you can remove this line*/
		TCCR1B=0;
		u8_status = E_ok;
		break;
	case PWM_CH1B:
		TCCR1A=0;   /*you can remove this line*/
		TCCR1B=0;
		u8_status = E_ok;
		break;
	case PWM_CH2:
		TCCR2=0;
		u8_status = E_ok;
		break;

	default:
		u8_status = E_NOk;
	}
	return u8_status;
}
