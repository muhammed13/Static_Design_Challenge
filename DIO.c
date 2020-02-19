#include "DIO.h"

#define NULL_Ptr ((void*)0)


ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	uint8_t u8_error;
	if(DIO_info == NULL_Ptr)
	{
		u8_error = E_NOk;
	}
	else
	{
		switch(DIO_info->GPIO)
		{
		case GPIOA:
			if(DIO_info->dir == INPUT)
			{
				PORTA_DIR &= ~(DIO_info->pins);
			}
			else
			{
				PORTA_DIR |= DIO_info->pins;
			}
			u8_error = E_ok;
			break;

		case GPIOB:
			if(DIO_info->dir == INPUT)
			{
				PORTB_DIR &= ~(DIO_info->pins);
			}
			else
			{
				PORTB_DIR |= DIO_info->pins;
			}
			u8_error = E_ok;
			break;
		case GPIOC:
			if(DIO_info->dir == INPUT)
			{
				PORTC_DIR &= ~(DIO_info->pins);
			}
			else
			{
				PORTC_DIR |= DIO_info->pins;
			}
			u8_error = E_ok;
			break;

		case GPIOD:
			if(DIO_info->dir == INPUT)
			{
				PORTD_DIR &= ~(DIO_info->pins);
			}
			else
			{
				PORTD_DIR |= DIO_info->pins;
			}
			u8_error = E_ok;
			break;

		default:
			u8_error = E_NOk;
			break;
		}
	}
	return u8_error;
}

/**************************************************************************/
ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)
{
	uint8_t u8_error;
	switch(GPIO)
	{
	case GPIOA:

		if(value == LOW)
		{
			PORTA_DATA &= ~(pins);
		}
		else
		{
			PORTA_DATA |= (pins&value);
		}
		u8_error = E_ok;
		break;

	case GPIOB:
		if(value == LOW)
		{
			PORTB_DATA &= ~(pins);
		}
		else
		{
			PORTB_DATA |= (pins&value);
		}
		u8_error = E_ok;
		break;
	case GPIOC:
		if(value == LOW)
		{
			PORTC_DATA &= ~(pins);
		}
		else
		{
			PORTC_DATA |= (pins&value);
		}
		u8_error = E_ok;
		break;

	case GPIOD:
		if(value == LOW)
		{
			PORTD_DATA &= ~(pins);
		}
		else
		{
			PORTD_DATA |= (pins&value);
		}
		u8_error = E_ok;
		break;

	default:
		u8_error = E_NOk;
		break;
	}
	return u8_error;
}

/***********************************************************************************/

ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data)
{
	uint8_t u8_error;
	switch(GPIO)
	{
	case 0:
		*data= (PORTA_PIN & pins);
		u8_error = E_ok;
		break;

	case 1:
		*data= (PORTB_PIN & pins);
		u8_error = E_ok;
		break;

	case 2:
		*data= (PORTC_PIN & pins);
		u8_error = E_ok;
		break;
	case 3:
		*data= (PORTD_PIN & pins);
		u8_error = E_ok;
		break;
	default:
		u8_error = E_NOk;
		break;
	}
	if(*data!=LOW)
	{
		*data=HIGH;
	}
	return u8_error;
}
/***********************************************************************************/

ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins)
{
	uint8_t u8_error;
	switch(GPIO)
	{
	case 0:
			PORTA_DATA ^= pins;
			u8_error = E_ok;
		break;

	case 1:
			PORTB_DATA ^= (pins);
			u8_error = E_ok;
		break;
	case 2:
			PORTC_DATA ^= (pins);
			u8_error = E_ok;
		break;

	case 3:
			PORTD_DATA ^= (pins);
			u8_error = E_ok;
		break;

	default:
		u8_error = E_NOk;
		break;
	}
	return u8_error;
}
/************************************************************************************/
