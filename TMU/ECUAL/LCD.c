/*
 * LCD.c
 *
 * Created: 3/11/2020 11:19:55 AM
 *  Author: Khaled Magdy
 */ 
#include "../MCAL/DIO.h"
#include "LCD.h"
#include "LCD_cfg.h"

# define F_CPU 16000000UL
#include <util/delay.h> // for development only

ERROR_STATUS LCD_init (void)
{
	/* DIO Pins Initialization */
	DIO_Cfg_s LCD_Pins, LCD_EN;
	LCD_EN.GPIO   = LCD_EN_GPIO;
	LCD_EN.pins   = LCD_EN_BIT;
	LCD_EN.dir    = OUTPUT;
	LCD_Pins.GPIO = LCD_GPIO;
	LCD_Pins.dir  = OUTPUT;
	LCD_Pins.pins = (RS | RW | D4 | D5 | D6 | D7);
	DIO_init(&LCD_EN);
	DIO_init(&LCD_Pins);
	
	/* LCD Initialization Procedure (As Stated In Datasheet) */
	DIO_Write(LCD_GPIO, RS, LOW);
	DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
	LCD_DATA(0x00);
	_delay_ms(130);
	LCD_sendCommand(0x03);
	_delay_ms(5);
	LCD_sendCommand(0x03);
	_delay_us(150);
	LCD_sendCommand(0x03);
	LCD_sendCommand(0x02);
	LCD_sendCommand(0x02);
	LCD_sendCommand(0x08);
	LCD_sendCommand(0x00);
	LCD_sendCommand(0x0C);
	LCD_sendCommand(0x00);
	LCD_sendCommand(0x06);
	
	return 0;
}

ERROR_STATUS LCD_DATA(uint8_t a_Data)
{
	if(a_Data & 1)
		DIO_Write(LCD_GPIO, D4, HIGH);
	else
		DIO_Write(LCD_GPIO, D4, LOW);
	if(a_Data & 2)
		DIO_Write(LCD_GPIO, D5, HIGH);
	else
		DIO_Write(LCD_GPIO, D5, LOW);
	if(a_Data & 4)
		DIO_Write(LCD_GPIO, D6, HIGH);
	else
		DIO_Write(LCD_GPIO, D6, LOW);
	if(a_Data & 8)
		DIO_Write(LCD_GPIO, D7, HIGH);
	else
		DIO_Write(LCD_GPIO, D7, LOW);
		
	return 0;
}

ERROR_STATUS LCD_sendCommand(uint8_t a_CMD)
{
	// Select Command Register
	DIO_Write(LCD_GPIO, RS, LOW);
	// Move The Command Data To LCD
	LCD_DATA(a_CMD);
	// Send The EN Clock Signal
	DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, HIGH);
	_delay_us(15);
	DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
	_delay_us(15);
	
	return 0;
}

ERROR_STATUS LCD_displayChar(uint8_t a_CH)
{
	  uint8_t Low4,High4;
	  
	  Low4 = a_CH & 0x0F;
	  High4 = a_CH & 0xF0;
	  
	  DIO_Write(LCD_GPIO, RS, HIGH);
	  
	  LCD_DATA(High4>>4);
	  DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, HIGH);
	  _delay_us(15);
	  DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
	  _delay_us(15);
	  
	  LCD_DATA(Low4);
	  DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, HIGH);
	  _delay_us(15);
	  DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
	  _delay_us(15);
	  
	  return 0;
}

ERROR_STATUS LCD_gotoRowColumn(uint8_t a_ROW, uint8_t a_COL)
{
	uint8_t Temp,Low4,High4;
	if(a_ROW == 1)
	{
		Temp = 0x80 + a_COL - 1;
		High4 = Temp >> 4;
		Low4 = Temp & 0x0F;
		LCD_sendCommand(High4);
		LCD_sendCommand(Low4);
	}
	if(a_ROW == 2)
	{
		Temp = 0xC0 + a_COL - 1;
		High4 = Temp >> 4;
		Low4 = Temp & 0x0F;
		LCD_sendCommand(High4);
		LCD_sendCommand(Low4);
	}
	
	return 0;
}

ERROR_STATUS LCD_displayString(uint8_t* a_STR)
{
	uint8_t i;
	for(i=0; a_STR[i] != '\0'; i++)
	{
		LCD_displayChar(a_STR[i]);
	}
	return 0;
}

ERROR_STATUS LCD_displayStringRowColumn(uint8_t a_ROW, uint8_t a_COL, uint8_t* a_STR)
{
	LCD_gotoRowColumn(a_ROW, a_COL);
	LCD_displayString(a_STR);
	
	return 0;
}

ERROR_STATUS LCD_clear(void)
{
	LCD_sendCommand(0x00);
	LCD_sendCommand(0x01);
	
	return 0;
}