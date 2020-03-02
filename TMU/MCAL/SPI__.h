/*
 * SPI__.h
 *
 * Created: 2/10/2020 6:54:43 PM
 *  Author: MENA
 */ 


#ifndef SPI___H_
#define SPI___H_


#include "../util/std_types.h"
#include "../util/registers.h"
#include "../MCAL/DIO.h"
#include "../util/interrupt.h"

#define ZERO 0 

#define ENABLE 1
#define DISABLE 0


#define SCK_PORT  GPIOB
#define MISO_PORT GPIOB
#define MOSI_PORT GPIOB
#define SS_PORT   GPIOB

#define SPI_PORT GPIOB

#define SCK_PIN  BIT7
#define MISO_PIN BIT6
#define MOSI_PIN BIT5
#define SS_PIN   BIT4

#define  Enable_SPI 0x40

#define  Master_SPI 0x10

#define Slave_SPI 0x00

#define INTERRUPT_SPI 0x80

typedef enum SPI_clk{
	Fosc4 = 0x00 ,
	Fosc16 = 0x01 ,
	Fosc64 = 0x02 ,
	Fosc128 = 0x03 
	}SPI_clk;

typedef enum SPI_mode{
	mode0 = 0x00 ,
	mode1 = 0x04 ,
	mode2 = 0x08 ,
	mode3 = 0x0C 
}SPI_mode;

typedef enum SPI_DataOrder{
	MSB = 0x00 ,
	LSB = 0x20
	
	}SPI_DataOrder;


typedef struct SPI_cfg_s{
	
	SPI_clk clockSPI ; 
	SPI_mode phasePolarityMode ;
	SPI_DataOrder dataorder ;
	void(*ptr_call_back)(void);
	
	}SPI_cfg_s;


extern void _SPIInitMaster(SPI_cfg_s* );
extern void _SPIInitSlave(SPI_cfg_s*);


extern void _SPITrancevier(uint8_t* data);

extern ERROR_STATUS _SPISend(uint8_t data);
extern ERROR_STATUS _SPIRead(uint8_t* data);

void SPI_SetCBF(FunPtr isr_ptr);


#endif /* SPI___H_ */