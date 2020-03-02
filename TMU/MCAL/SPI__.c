/*
 * SPI__.c
 *
 * Created: 2/10/2020 6:54:24 PM
 *  Author: MENA
 */ 

#include "SPI__.h"

volatile void (*PTR_CALL_BACK)(void);

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
	
	SPCR|=a_SPI_cfg-> clockSPI ;
	
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

	//while (!(SPSR&(1<<7)));

	*data = SPDR;
	
}

ISR(SPI_STC_vect){
	
	PTR_CALL_BACK() ;
}


