/*
 * LCD.h
 *
 * Created: 3/11/2020 11:20:07 AM
 *  Author: Khaled Magdy
 */ 


#ifndef LCD_H_
#define LCD_H_



ERROR_STATUS LCD_init (void);

ERROR_STATUS LCD_clear(void);

ERROR_STATUS LCD_DATA(uint8_t a_Data);

ERROR_STATUS LCD_sendCommand(uint8_t a_CMD);

ERROR_STATUS LCD_displayChar(uint8_t a_CH);

ERROR_STATUS LCD_displayString(uint8_t* a_STR);

ERROR_STATUS LCD_gotoRowColumn(uint8_t a_ROW, uint8_t a_COL);

ERROR_STATUS LCD_displayStringRowColumn(uint8_t a_ROW, uint8_t a_COL, uint8_t* a_STR);

#endif /* LCD_H_ */