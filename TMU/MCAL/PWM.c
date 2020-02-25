/*
 * PWM.c
 *
 * Created: 2/17/2020 11:32:24 AM
 *  Author: Khaled Magdy
 */ 
#include "PWM.h"



extern ERROR_STATUS Pwm_Init(Pwm_Cfg_s *Pwm_Cfg)
{
	DIO_Cfg_s a_PWM1A;
	DIO_Cfg_s a_PWM1B;
	ERROR_STATUS errorStatus = E_NOK;
	if(Pwm_Cfg != NULL)
	{
		TCCR1 = 0x0000;
		// Set For inverting or non-inverting
		TCCR1 |= 0xA000;
		// Set the Mode
		TCCR1 |= 0x0010;
		// Stop The Timer
		TCCR1 |= TIMER_NO_CLOCK;
		// Set the Initial Values
		TCNT1 = 0x00;
		switch(Pwm_Cfg->Channel)
		{
			case PWM_CH1A:
				a_PWM1A.dir  = OUTPUT;
				a_PWM1A.GPIO = PWM1A_GPIO;
				a_PWM1A.pins = PWM1A_BIT;
				DIO_init(&a_PWM1A);
				errorStatus = E_OK;
				break;
			case PWM_CH1B:
				a_PWM1B.dir  = OUTPUT;
				a_PWM1B.GPIO = PWM1B_GPIO;
				a_PWM1B.pins = PWM1B_BIT;
				DIO_init(&a_PWM1B);
				errorStatus = E_OK;
				break;
			default:
				errorStatus = E_NOK;
				break;
		}	
	}
	else
	{
		errorStatus = E_NOK;
	}
	return errorStatus;
}

extern ERROR_STATUS Pwm_Start(uint8_t Channel, uint8_t a_u8_Duty, uint32_t a_u32_Frequency)
{
	ERROR_STATUS errorStatus = E_NOK;
	// Set The Period To (1/Fpwm)
	ICR1 = (2000000 / a_u32_Frequency) >> 1;
	switch(Channel)
	{
		case PWM_CH1A:
			OCR1A = (a_u8_Duty / 100.0) * ICR1;
			errorStatus = E_OK;
			break;
		case PWM_CH1B:
			OCR1B = (a_u8_Duty / 100.0) * ICR1;
			errorStatus = E_OK;
			break;
		default:
			break;
	}
	TCCR1 |= TIMER_PRESCALER_8;
	
	return errorStatus;
}

extern ERROR_STATUS Pwm_Update(uint8_t a_u8_Channel, uint8_t a_u8_Duty, uint32_t a_u32_Frequency)
{
	ERROR_STATUS errorStatus = E_NOK;
	// Set The Period To (1/Fpwm)
	ICR1 = (2000000 / a_u32_Frequency) >> 1;
	switch(a_u8_Channel)
	{
		case PWM_CH1A:
			OCR1A = (a_u8_Duty / 100.0) * ICR1;
			errorStatus = E_OK;
			break;
		case PWM_CH1B:
			OCR1B = (a_u8_Duty / 100.0) * ICR1;
			errorStatus = E_OK;
			break;
		default:
			break;
	}
	return errorStatus;	
}

extern ERROR_STATUS Pwm_Stop(uint8_t Channel)
{
	ERROR_STATUS errorStatus = E_NOK;
	switch(Channel)
	{
		case PWM_CH1A:
			OCR1A = 0;
			errorStatus = E_OK;
			break;
		case PWM_CH1B:
			OCR1B = 0;
			errorStatus = E_OK;
			break;
		default:
			break;
	}
	return errorStatus;
}