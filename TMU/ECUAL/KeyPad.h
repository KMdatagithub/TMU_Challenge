/*
 * KeyPad.h
 *
 * Created: 3/11/2020 11:20:48 AM
 *  Author: Khaled Magdy
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../MCAL/DIO.h"


#define KEY_PRESSED	 1
#define KEY_RELEASED 0
#define KEY_FLOATING 2

#define KEYS_NUM     9
#define ROWS	  	 3
#define COLS		 3

#define FILTER_ORDER 8


ERROR_STATUS Keypad_Init(void);

ERROR_STATUS Keypad_Scan(uint8_t* a_Keys_Status);


#endif /* KEYPAD_H_ */