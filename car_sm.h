/*
 ============================================================================
 Name        : car_sm.h
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */

#ifndef CAR_SM_H_
#define CAR_SM_H_


/*************************************************************/
/*                     INCLUDES                              */
/*************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "common_macros.h"
#include "Steering.h"
#include "Us.h"



/*************************************************************/
/*                     DEFINITIONS                              */
/*************************************************************/
#define NEAR 0
#define INTERMEDIATE 1
#define FAR 2



/*************************************************************/
/*                     FUNCTIONS DECLARATION                 */
/*************************************************************/

/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
*/
ERROR_STATUS Car_SM_Init(void);

/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
*/
ERROR_STATUS Car_SM_Update(void);



#endif /* CAR_SM_H_ */
