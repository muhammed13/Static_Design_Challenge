/*
 ============================================================================
 Name        : interrupt.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */
#include "interrupt.h"
#include "ICU.h"

#define NULL_Ptr ((void*)0)


volatile uint8_t g8_state;
volatile uint8_t g8_interrupt_zero_flag=0;
volatile uint8_t g8_echo_flag=0;

/************************INT0*****************************************/
void INT0_Init(EN_Edge_t en_edge_selection)
{
	cli();                              /*Disable interrupts by clearing I-bit*/
	GICR  |= (1<<INT0);                 /* Enable external interrupt pin INT0*/
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOD,
			DIO_stru_init.pins=BIT2,
			DIO_stru_init.dir=INPUT,
	};
	DIO_init (&DIO_stru_init);
	switch(en_edge_selection)
	{
	case LOW_LEVEL:
								/* Trigger INT0 with the low level*/
		break;

	case ANY_CHANGE:
		MCUCR |= (1<<ISC10);   /* Trigger INT0 with the any change*/
		break;

	case FALLING_EDGE:
		MCUCR |= (1<<ISC00);   /* Trigger INT0 with the falling edge*/
		break;

	case RISIGING_EDGE:
		MCUCR |= (1<<ISC00) | (1<<ISC01);   /* Trigger INT0 with the raising edge*/
		break;
	}
	sei();                              /*Enable interrupts by setting I-bit*/
}


void INT0_DeInit(void)
{
	cli();                              /*Disable interrupts by clearing I-bit*/
	GICR  &= ~(1<<INT0);                 /* Disable external interrupt pin INT0*/
	MCUCR &= ~(1<<ISC00) & ~(1<<ISC01);
}

/****************************INT1*******************************************/
void INT1_Init(EN_Edge_t en_edge_selection)
{
	cli();                              /*Disable interrupts by clearing I-bit*/
	GICR  |= (1<<INT1);                 /* Enable external interrupt pin INT1*/
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOD,
			DIO_stru_init.pins=BIT3,
			DIO_stru_init.dir=INPUT,
	};
	DIO_init (&DIO_stru_init);
	switch(en_edge_selection)
	{
	case LOW_LEVEL:
								            /* Trigger INT0 with the low level*/
		break;

	case ANY_CHANGE:
		MCUCR |= (1<<ISC10);                /* Trigger INT0 with the any change*/
		break;

	case FALLING_EDGE:
		MCUCR |= (1<<ISC11);                /* Trigger INT0 with the falling edge*/
		break;

	case RISIGING_EDGE:
		MCUCR |= (1<<ISC10) | (1<<ISC11);   /* Trigger INT0 with the raising edge*/
		break;
	}
	sei();                              /*Enable interrupts by setting I-bit*/
}

void INT1_DeInit(void)
{
	cli();                              /*Disable interrupts by clearing I-bit*/
	GICR  &= ~(1<<INT1);                 /* Disable external interrupt pin INT0*/
	MCUCR &= ~(1<<ISC10) & ~(1<<ISC11);
}

/****************************INT2*******************************************/
void INT2_Init(EN_Edge_t en_edge_selection)
{
	cli();                              /*Disable interrupts by clearing I-bit*/
	GICR  |= (1<<INT2);                 /* Enable external interrupt pin INT1*/
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOB,
			DIO_stru_init.pins=BIT2,
			DIO_stru_init.dir=INPUT,
	};
	DIO_init (&DIO_stru_init);
	switch(en_edge_selection)
	{
	case LOW_LEVEL:
		                                /*nothing to do*/
		break;

	case ANY_CHANGE:
		                                /*nothing to do*/
		break;

	case FALLING_EDGE:
		MCUCSR &= ~(1<<ISC2);                            /*nothing to do*/
		break;

	case RISIGING_EDGE:
		MCUCSR |= (1<<ISC2);            /* Trigger INT0 with the raising edge*/
		break;
	}
	sei();                              /*Enable interrupts by setting I-bit*/
}

void INT2_DeInit(void)
{
	cli();                               /*Disable interrupts by clearing I-bit*/
	GICR  &= ~(1<<INT2);                 /* Disable external interrupt pin INT0*/
	MCUCSR &= ~(1<<ISC2);
}

void INT2_SetEdge(EN_Edge_t en_edge_selection)
{
	/*I have to check first if it is enabled or not*/
	switch(en_edge_selection)
	{
	case LOW_LEVEL:
		                                /*nothing to do*/
		break;

	case ANY_CHANGE:
		                                /*nothing to do*/
		break;

	case FALLING_EDGE:
		MCUCSR &= ~(1<<ISC2);                            /*nothing to do*/
		break;

	case RISIGING_EDGE:
		MCUCSR |= (1<<ISC2);            /* Trigger INT0 with the raising edge*/
		break;
	}
}
/***************************************************************************/


/********************************ISRS***************************************/

ISR(INT2_vect)
{
	if(g_callBackPtr != NULL_Ptr)
	{
		// Call the Call Back function in the application after the edge is detected
		(*g_callBackPtr)(); // another method to call the function using pointer to function g_callBackPtr();
	}

}

/******************************************************************************/
