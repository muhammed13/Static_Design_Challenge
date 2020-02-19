/*
 ============================================================================
 Name        : external_interrupt.h
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */

#ifndef EXTERNAL_INTERRUPT_H_
#define EXTERNAL_INTERRUPT_H_


/**************************************ENUMs**********************************/
typedef enum EN_Edge_t{
	LOW_LEVEL=0,
	ANY_CHANGE=1,
	FALLING_EDGE = 2,
	RISIGING_EDGE = 3
}EN_Edge_t;



/*********************************global flags*********************************/



/************************************functions*******************************/
void INT0_Init(EN_Edge_t);
void INT1_Init(EN_Edge_t);
void INT2_Init(EN_Edge_t);
void INT0_DeInit(void);
void INT1_DeInit(void);
void INT2_DeInit(void);
void INT2_SetEdge(EN_Edge_t en_edge_selection);
#endif /* EXTERNAL_INTERRUPT_H_ */
