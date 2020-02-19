/*
 ============================================================================
 Name        : steering.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */
#include "Steering.h"

ERROR_STATUS Steering_Init(void)
{
	ERROR_STATUS u8_status;
	u8_status=Motor_Init(MOTOR_1);
	u8_status=Motor_Init(MOTOR_2);
	u8_status=Motor_Start(MOTOR_1,0);
	u8_status=Motor_Start(MOTOR_2,0);
	return u8_status;
}

/*****************************************************************************************/


ERROR_STATUS Steering_SteerCar(uint8_t Steering_CarCmd, uint8_t speed)
{
	ERROR_STATUS u8_status;
	switch(Steering_CarCmd)
	{
	case CAR_STOP:
		Motor_Stop(MOTOR_1);
		Motor_Stop(MOTOR_2);
		u8_status=E_ok;
		break;

	case CAR_FORWARD:
		Motor_Direction(MOTOR_1, MOTOR_FORWARD);
		Motor_Direction(MOTOR_2, MOTOR_FORWARD);
		Motor_SpeedUpdate(MOTOR_1_2,speed);
		u8_status=E_ok;
		break;

	case CAR_BACKWARD:
		Motor_Direction(MOTOR_1, MOTOR_BACKWARD);
		Motor_Direction(MOTOR_2, MOTOR_BACKWARD);
		Motor_SpeedUpdate(MOTOR_1_2,speed);
		u8_status=E_ok;
		break;

	case CAR_RIGHT:
		Motor_Direction(MOTOR_1, MOTOR_BACKWARD);
		Motor_Direction(MOTOR_2, MOTOR_FORWARD);
		Motor_SpeedUpdate(MOTOR_2,speed);
		u8_status=E_ok;
		break;

	case CAR_LEFT:
		Motor_Direction(MOTOR_1, MOTOR_FORWARD);
		Motor_Direction(MOTOR_2, MOTOR_BACKWARD);
		Motor_SpeedUpdate(MOTOR_1,speed);
		u8_status=E_ok;
		break;

	default:
		u8_status=E_NOk;
	}
	return u8_status;
}
