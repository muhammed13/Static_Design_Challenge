/*
 ============================================================================
 Name        : Us.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */

#include "Us.h"

ERROR_STATUS Us_Init(void)
{
	uint8_t u8_status;
	DIO_Cfg_s trigger_init=
	{
			trigger_init.GPIO=GPIOA,
			trigger_init.pins=BIT0,
			trigger_init.dir=OUTPUT,
	};
	DIO_init (&trigger_init);
	DIO_Write (GPIOA, BIT0, LOW);

	Icu_cfg_s echo_init={ICU_CH2,ICU_TIMER_CH2};    /*INT2,TIMER2*/
	Icu_Init(&echo_init);
	u8_status=E_ok;
	return u8_status;

}

/*****************************************************************************************/

ERROR_STATUS Us_Trigger(void)
{
	uint8_t u8_status;
	softwareDelayMs(1);
	DIO_Write (GPIOA, BIT0, HIGH);
	softwareDelayMs(1);
	DIO_Write (GPIOA, BIT0, LOW);
	u8_status=E_ok;
	return u8_status;


}


/*****************************************************************************************/

ERROR_STATUS Us_GetDistance(uint16_t *Distance)
{
	uint8_t u8_status;
	uint32_t u32_time;
	Icu_ReadTime(Timer_2,ICU_RISE_TO_FALL,&u32_time);   /*high time*/
	*Distance=u32_time/58;
	u8_status=E_ok;
	return u8_status;
}
