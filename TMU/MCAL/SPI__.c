/*
 * SPI__.c
 *
 * Created: 2/10/2020 6:54:24 PM
 *  Author: MENA
 */ 

#include "SPI__.h"

 void (*PTR_CALL_BACK)(void);
 volatile uint8_t g_u8_RxFlag = TRUE;

extern void _SPIInitMaster(SPI_cfg_s * a_SPI_cfg){
	
		DIO_Cfg_s SPI_input_pins ;
		DIO_Cfg_s SPI_output_pins ;
		
	
	
		/* clear status register */
		SPSR = ZERO ;
		
		/* clear control mode */
		
		SPCR = ZERO ;
		
		/* master or slave bit 4 */
		SPCR |= Master_SPI ; 
		
		/*set clock*/
		 
		SPCR|= a_SPI_cfg-> clockSPI ;
		
		/* data order update */
		
		SPCR|= a_SPI_cfg-> dataorder ;
		
		/* set phase bit2 and polarity bit3 */
		
		 SPCR |= a_SPI_cfg-> phasePolarityMode;
		
		/* set interrupt  */
		
		SPCR |= INTERRUPT_SPI ;
		
		/*set interrupt call back function */
		
		PTR_CALL_BACK = a_SPI_cfg->ptr_call_back ;
		 
////// GPIO set output and input 

	//SS   bit4 OUTPUT
	//MOSI bit5 OUTPUT
	//MISO bit6 INPUT
	//SCK  bit7 OUTPUT

	SPI_input_pins.GPIO = SPI_PORT ;
	SPI_input_pins.pins = MISO_PIN ;
	SPI_input_pins.dir = INPUT ;
	
	SPI_output_pins.GPIO = SPI_PORT ;
	SPI_output_pins.pins =  SCK_PIN|MOSI_PIN|SS_PIN ;
	SPI_output_pins.dir = OUTPUT ;
	
	DIO_init(&SPI_output_pins);
	DIO_init(&SPI_input_pins);
	
		
	/// enable SPI bit 6
	
	sei();
	SPCR |= Enable_SPI ;
	
}

extern void _SPIInitSlave(SPI_cfg_s * a_SPI_cfg ){
	
	DIO_Cfg_s SPI_input_pins ;
	DIO_Cfg_s SPI_output_pins ;
			
			
	
	//// clear status register
	SPSR = ZERO ;
	
	/// clear control mode
	
	SPCR = ZERO ;
	
	
	// master or slave bit 4
	SPCR |= Slave_SPI ;
	
	/// data order update
	
	SPCR|=a_SPI_cfg-> dataorder ;
	
	//set clock
	
	SPCR |= a_SPI_cfg-> clockSPI ;
	
	// set phase and polarity
	SPCR |=a_SPI_cfg -> phasePolarityMode ;
	
	/* set interrupt  */
	SPCR |= INTERRUPT_SPI ;
	
	/*set interrupt call back function */
		
	PTR_CALL_BACK = a_SPI_cfg->ptr_call_back ;
	
	////// GPIO set output and input


	//SS   bit4 INPUT
	//MOSI bit5 INPUT
	//MISO bit6 OUTPUT
	//SCK  bit7 INPUT

	SPI_input_pins.GPIO = SPI_PORT ;
	SPI_input_pins.pins =  SCK_PIN|MOSI_PIN|SS_PIN ;
	SPI_input_pins.dir = INPUT ;

	SPI_output_pins.GPIO = SPI_PORT ;
	SPI_output_pins.pins = MISO_PIN ;
	SPI_output_pins.dir = OUTPUT ;


	DIO_init(&SPI_output_pins);
	DIO_init(&SPI_input_pins);
	

	/// enable SPI bit 6
	sei();
	
	SPCR |= Enable_SPI ;
	
		
	
}

extern void _SPITrancevier(uint8_t* data){
	
	if(SPCR&Master_SPI){
		
		/*Select the slave By pull down the SS pin */
		
		DIO_Write(SPI_PORT,BIT4,ZERO );
		
	}
	
	
	SPDR = *data;
	while (!g_u8_RxFlag);
	*data = SPDR;	
	
	g_u8_RxFlag = FALSE ;
}

extern ERROR_STATUS _SPISend(uint8_t data)
{
	uint8_t state = E_NOK;	
	if(g_u8_RxFlag == TRUE)
	{
		SPDR = data;
		g_u8_RxFlag = FALSE;	
		state = E_OK;
	}
	return state;
}

extern ERROR_STATUS _SPIRead(uint8_t* data)
{
	uint8_t state = E_NOK;
	if(g_u8_RxFlag == TRUE)
	{
		*data = SPDR;
		g_u8_RxFlag = FALSE;
		state = E_OK;
	}
	return state;
}


void SPI_SetCBF(FunPtr isr_ptr)
{
	PTR_CALL_BACK = isr_ptr;
}

ISR(SPI_STC_vect)
{
	g_u8_RxFlag = TRUE;
	PTR_CALL_BACK();
}


