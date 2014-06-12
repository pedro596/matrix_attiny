#include <avr/interrupt.h>
#include <stdint.h>


#define DATA_0			PB4 // primeiro
#define LATCH_CK		PB3		//segundo
#define SHIFT_CK		PB0		//terceiro
#define BLINK			PB3

#define	_low(bit)		PORTB	&=	~(1<<bit)	
#define _high(bit)		PORTB	|=	(1<<bit)
#define _toogle(bit)	PINB	|=	(1<<bit)

/*
      |
      3º
      |
__ 1º_____ 2º__
	  |
	  4º
	  |
*/

uint32_t JoinData_out(uint8_t on_off, uint8_t r, uint8_t g , uint8_t b)
// adaptar dados para o hardware
{
	uint32_t data;
	uint8_t onoff_verde=0, verde_onoff=0;
	uint8_t i, g_temp=0;
	
	/*inverter o verde (reverse)*/
	for(i=0;i<8;i++)
		g_temp |= ((g>>i)&1)<<(7-i);
	

	//another cosmetics because the stupid rgb matrix pinout's...
	
	g = (g_temp>>4) | (g_temp<<4); 

	/*dividir (e juntar) bytes (verde e onoff)*/
	onoff_verde  = (on_off&0xf0)|(g&0x0f);
	verde_onoff  = (on_off&0x0f)|(g&0xf0);


	data = (uint32_t)(onoff_verde);				//[7:5] On/off 3º  [4:0] verde 2º invertido -> ncessário compensar
	data = (uint32_t)(data<<8)|verde_onoff; 	//[7:5] Verde 1º invertido  [4:0] on/off 4º
	data = (uint32_t)(data<<8)|b;			//Azul 
	data = (uint32_t)(data<<8)|r;				//Vermelho

	return data;
}

uint32_t write_red(uint8_t on_off, uint8_t red)
{
	uint32_t data;
	uint8_t onoff_verde=0, verde_onoff=0;
	uint8_t i, g=0;
	
	
	g=255;

	/*dividir (e juntar) bytes (verde e onoff)*/
	onoff_verde  = (on_off&0xf0)|(g&0x0f);
	verde_onoff  = (on_off&0x0f)|(g&0xf0);

	data = (uint32_t)(onoff_verde);			//[7:5] On/off 3º  [4:0] verde 2º invertido -> ncessário compensar
	data = (uint32_t)(data<<8)|verde_onoff; //[7:5] Verde 1º invertido  [4:0] on/off 4º
	data = (uint32_t)(data<<8)|(255);			//Azul 
	data = (uint32_t)(data<<8)|red;			//Vermelho

	return data;
}



void shift(uint32_t data)
{
	uint8_t i,temp;

	_low(LATCH_CK);
	_low(SHIFT_CK);

	for(i=0; i<32; i++){
		
		_low(SHIFT_CK);

		if((data>>i)&(0x1))
			_high(DATA_0);
		else
			_low(DATA_0);

		_high(SHIFT_CK);
	}
	
	_high(LATCH_CK);
	_low(LATCH_CK);
}


