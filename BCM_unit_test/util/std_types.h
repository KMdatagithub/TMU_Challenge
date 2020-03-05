/*
 * std_types.h
 *
 *  Created on: Oct 21, 2019
 *      Author: Sprints
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


/************************************************************************/
/*                   typedefs for standard types                        */
/************************************************************************/

#define		NULL						((void *)0)
typedef		unsigned char				uint8_t;
typedef		unsigned int				uint16_t;
typedef		unsigned long int			uint32_t;
typedef		unsigned long long			uint64_t;
typedef		signed char					sint8_t;
typedef		signed int					sint16_t;
typedef		signed long int				sint32_t;
typedef		signed long long			sint64_t;
typedef		volatile uint8_t* const		reg_type8_t;
typedef		volatile uint16_t* const	reg_type16_t;
typedef		void (*FunPtr)(void);


/************************************************************************/
/*				 		defining boolean values                         */
/************************************************************************/

typedef uint8_t bool;
#define FALSE 	0	// defines boolean false
#define TRUE 	1	// defines boolean true


/************************************************************************/
/*						 LOW/HIGH defines                               */
/************************************************************************/
#define LOW  0x00	// defines LOW value for a bit
#define HIGH 0xFF	// defines HIGH value for a bit
#define ZERO 0


/************************************************************************/
/*	            	 Bit Manipulation Macro defines                     */
/************************************************************************/

#define SET_BIT(r, b)     (r |=  (1<<b))
#define CLEAR_BIT(r, b)   (r &= ~(1<<b))
#define TOGGLE_BIT(r, b)  (r)^=(b)
#define GET_BIT(r, b)     (((r)>>(b))&1)

/************************************************************************/
/*			 			 defining error status             		        */
/************************************************************************/

#define ERROR_STATUS	uint16_t
#define E_OK			0
#define E_NOK			1



/************************************************************************/
/*			 				Bit Definitions             		        */
/************************************************************************/

#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80


#endif /* STD_TYPES_H_ */