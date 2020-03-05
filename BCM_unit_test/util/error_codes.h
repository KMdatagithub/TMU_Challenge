/*
 * error_codes.h
 *
 * Created: 2/26/2020 11:41:39 AM
 *  Author: Khaled Magdy
 */ 


#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

/*-----[ Module Error Codes ]-----*/

#define TMR_ERROR   100
#define TMU_ERROR	200
#define DIO_ERROR	300
#define PWM_ERROR   400
#define UART_ERROR  500
#define BCM_ERROR   600

/*-----[ Special Error Codes ]-----*/

#define E_OK					0
#define NULL_PTR				1
#define NOT_INIT				2
#define ALREADY_INIT			3
#define INVALID_IN				4
#define MULTI_STOP				5
#define MULTI_START				6
#define FULL_BUFFER				7
#define BUFFER_ALREADY_UNLOCKED 8
#define BUFFER_ALREADY_LOCKED   9
#define WRONG_ID				10
#define WRONG_CHECKSUM			11



#endif /* ERROR_CODES_H_ */