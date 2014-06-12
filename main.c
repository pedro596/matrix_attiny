#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include "matrix.h"
#include "BasicSerial3.h"


extern void TxTimedByte(char, char);
extern char RxTimedByte(char, char); /* 1.5 bit delay, 1 bit delay */

volatile uint8_t text[50];
volatile uint32_t data;
volatile unsigned char timer=0;
volatile uint8_t linha =0;
volatile uint16_t change_letter=0;
volatile uint8_t letter_number=0;
volatile uint8_t c;
volatile uint8_t flag_comm=0;


inline void HdInit()
{
	/* Configure  my "soft-step" timer for the app */
	/* 0.5ms shoud be ok  4000 clock operations between ticks  */ 
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS01);
	OCR0A = 249;
	TIMSK |= (1<<OCIE0A);

	/*set outputs*/
	DDRB |= (1<<BLINK) |(1<<LATCH_CK)|(1<<SHIFT_CK)|(1<<DATA_0);

	sei();
}

void serOut(const char* str)		/**/
{
   while (*str) TxByte (*str++);
}



inline void Set8Mhz()
{
	CLKPR = 0b10000000;
	CLKPR = 0b00000000;
}

int main()
{
	Set8Mhz();
	uint8_t request_count=0;
	HdInit();	


	for(;;)
	{   
	    if(flag_comm>0){
			
		    TIMSK &= ~(1<<OCIE0A);						//desliga interrupção timer
		    sprintf(text, "-%d%d-\n\r", request_count/8, request_count%8);
		    serOut(text);
		    
		    if(request_count<8)
		    	red[request_count%8] = RxByte(); 	
		    else if(request_count<16)
		    	green[request_count%8] = RxByte();
		    else
		    	blue[request_count%8] = RxByte();

   			if(request_count>22)
		    	request_count=0;
		    else
		    	request_count++;
   			
       		TIMSK |= (1<<OCIE0A);						//liga interrupções timer					
       		flag_comm =0;
		}
    }
   		
}


ISR(TIMER0_COMPA_vect){


	timer++;
	linha++;
	change_letter++;

	if(change_letter>5000)
	{
		letter_number++;
		if(letter_number>20)
			letter_number=0;

		change_letter=0;
	}



	if(timer>23)		//12 ms 
		timer = 0;
	
	if(linha>7)
		linha = 0;
	
	if(timer<7)
		data = JoinData_out((1<<linha), red[linha], 255, 255);
	else if(timer<15)
		data = JoinData_out((1<<linha), 255,255, blue[linha]);
	else if(timer<23)
		data = JoinData_out((1<<linha), 255, green[linha], 255);
	else
		flag_comm=1;
	
	if(timer<23)
		shift(data);

}