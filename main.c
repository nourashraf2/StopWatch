/*
 * main.c
 *
 *  Created on: Sep 15, 2023
 *      Author: nouri
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


unsigned char s1,s2,m1,m2,h1,h2;
//                        h2  h1  m2  m1  s2  s1
//enable of each segment  PA0 PA1 PA2 PA3 PA4 PA5
 void displaySegments(){
	PORTA=(1<<PA5)|(PORTA & 0xC0);
	PORTC=(PORTC & 0xF0) | (s1 & 0x0F);
	_delay_us(1);
	PORTA=(1<<PA4)|(PORTA & 0xC0);
	PORTC=(PORTC & 0xF0) | (s2 & 0x0F);
	_delay_us(1);
	PORTA=(1<<PA3)|(PORTA & 0xC0);
	PORTC=(PORTC & 0xF0) | (m1 & 0x0F);
	_delay_us(1);
	PORTA=(1<<PA2)|(PORTA & 0xC0);
	PORTC=(PORTC & 0xF0) | (m2 & 0x0F);
	_delay_us(1);
	PORTA=(1<<PA1)|(PORTA & 0xC0);
	PORTC=(PORTC & 0xF0) | (h1 & 0x0F);
	_delay_us(1);
	PORTA=(1<<PA0)|(PORTA & 0xC0);
	PORTC=(PORTC & 0xF0) | (h2 & 0x0F);
	_delay_us(1);


}

/*
 pres=64
 1M/64=15625
 15625*(6.4*10^-5)=1sec
 */
 void timer_init(){
	 TIMSK |=(1<<OCIE1A);
	 TCCR1A=(1<<FOC1A);
	 TCCR1B =(1<<WGM12)|(1<<CS11) |(1<<CS10);
	 TCNT1=0;
	 OCR1A = 15624;
	 SREG |= (1<<7);


 }

 void interrupt0_init(){
	 GICR |=(1<<INT0);
	 MCUCR |=(1<<ISC01);

 }

 void interrupt1_init(){
	 GICR |=(1<<INT1);
	 MCUCR |=(1<<ISC11) |(1<<ISC10);

 }


 void interrupt2_init(){
	 GICR |=(1<<INT2);
	 MCUCSR &= ~(1<<ISC2);


 }

 ISR(INT0_vect){
	 s1=0;
	 s2=0;
	 m1=0;
	 m2=0;
	 h1=0;
	 h2=0;
	 TCNT1=0;
	/* if(!(TCCR1A)){
	 		TCCR1A = (1<<FOC1A);
	 		TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);
	 	}
*/


 }

 ISR(INT1_vect){
	 TCCR1A = 0;
	 TCCR1B = 0;

 }

 ISR(INT2_vect){
	 TCCR1A=(1<<FOC1A);
	 TCCR1B =(1<<WGM12)|(1<<CS11) |(1<<CS10);

 }



 ISR(TIMER1_COMPA_vect){
	 if(s1==9){
		 if(s2==5){
			 if(m1==9){
				 if(m2==5){
					 if(h1==9){
						 if(h2==5){
							 s1=0;
							 s2=0;
							 m1=0;
							 m2=0;
							 h1=0;
							 h2=0;

						 }
						 else{
							 h2++;
							 h1=0;
							 m2=0;
							 m1=0;
							 s2=0;
							 s1=0;
						 }

					 }
					 else{
						 h1++;
						 m2=0;
						 m1=0;
						 s1=0;
						 s2=0;
					 }
				 }
				 else{
					 m2++;
					 m1=0;
					 s2=0;
					 s1=0;


				 }
			 }
			 else{
				 m1++;
				 s1=0;
				 s2=0;

			 }

		 }
		 else{
			 s1=0;
			 s2++;
		 }
	 }
	 else{
		 s1++;
	 }

 }



 int main(){
	 DDRA |=0x3F;
	 DDRC |=0x0F;
	 PORTC &= (0xF0);
	 timer_init();

	 //for int0
	 interrupt0_init();
	 DDRD &=~(1<<PD2);
	 PORTD|=(1<<PD2);

	 //for int1
	 interrupt1_init();
	 DDRD &=~(1<<PD3);

	 //for int2
	 interrupt2_init();
	 DDRB &=~(1<<PB2);
	 PORTB |=(1<<PB2);





	 while(1){
		 displaySegments();
	 }










 }























