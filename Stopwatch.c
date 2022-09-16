/*
 * Stopwatch.c
 *
 *  Created on: 12 Sept 2022
 *      Author: Lenovo
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char seconds=0;
unsigned char mins=0;
unsigned char hours=0;
void timer2_compare_ini(void){
	TCCR1A|=(1<<FOC1A);
	TCCR1B|=(1<<CS10)|(1<<WGM12)|(1<< CS12); //prescale 1024
	TCNT1=0;
	OCR1A=1000;
	TIMSK|=(1<<OCIE1A);
}
void int0_ini(void){
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01); //falling edge
	MCUCR&=~(1<<ISC00);
	PORTD|=(1<<PD2); //internal pull up
}
void int1_ini(void){
	MCUCR|=(1<<ISC11)|(1<<ISC10); //rising edge
	GICR |= (1<<INT1);
}
void int2_ini(void){

	MCUCR&=~(1<<ISC2); //falling edge
	GICR|=(1<<INT2);

	PORTB|=(1<<PB2); //internal pull up
}
ISR(TIMER1_COMPA_vect){
	seconds++;
	if(seconds==60){
		mins++;
		seconds=0;
	}
	if (mins==60){
		hours++;
		mins=0;
	}
}
ISR (INT0_vect){
	seconds=0;
	mins=0;
	hours=0;
}
ISR(INT1_vect){
	TCCR1B &=~(1<<CS10)&~(1<<CS11)&~(1<<CS12); //TURNING OFF THE CLOCK
}
ISR(INT2_vect){
	TCCR1B|=(1<<CS10)|(1<< CS12);
}

int main(void){
	DDRC|=0x0F;
	//PORTC&=~0xF0;
	DDRA|=0x3F;
	DDRD&=~(1<<2)&~(1<<3);
	DDRB&=~(1<<2);
	timer2_compare_ini();
	int0_ini();
	int1_ini();
	int2_ini();
	SREG|=(1<<7);
	while(1){
		/*PORTA|=(1<<PA0);
		PORTA&=~(1<<PA1)&~(1<<PA2)&~(1<<PA3)&~(1<<PA4)&~(1<<PA5);
		PORTC=(seconds%10) | (PORTC &0xF0); // using % to get the first unit of seconds
		_delay_ms(3);
		PORTA|=(1<<PA1);
		PORTA&=~(1<<PA0)&~(1<<PA2)&~(1<<PA3)&~(1<<PA4)&~(1<<PA5);
		PORTC=(seconds/10) | (PORTC &0xF0); //second unit
		_delay_ms(3);
		PORTA|=(1<<PA2);
		PORTA&=~(1<<PA0)&~(1<<PA1)&~(1<<PA3)&~(1<<PA4)&~(1<<PA5);
		PORTC=(mins%10) | (PORTC &0xF0);
		_delay_ms(3);
		PORTA|=(1<<PA3);
		PORTA&=~(1<<PA0)&~(1<<PA2)&~(1<<PA1)&~(1<<PA4)&~(1<<PA5);
		PORTC=(mins/10) | (PORTC &0xF0);
		_delay_ms(3);
		PORTA|=(1<<PA4);
		PORTA&=~(1<<PA0)&~(1<<PA2)&~(1<<PA3)&~(1<<PA1)&~(1<<PA5);
		PORTC=(hours%10) | (PORTC &0xF0);
		_delay_ms(3);
		PORTA|=(1<<PA5);
		PORTA&=~(1<<PA0)&~(1<<PA2)&~(1<<PA3)&~(1<<PA1)&~(1<<PA1);
		PORTC=(hours/10) | (PORTC &0xF0);
		_delay_ms(3);*/
		PORTA = (1 << 0);
		PORTC=(seconds%10) | (PORTC &0xF0); // take units of the number
		_delay_ms(3);

		PORTA = (1 << 1);
		PORTC=(seconds/10) | (PORTC &0xF0); //take tenth of the number
		_delay_ms(3);

		PORTA = (1 << 2);
		PORTC=(mins%10) | (PORTC &0xF0); //take units of the number
		_delay_ms(3);

		PORTA = (1 << 3);
		PORTC=(mins/10) | (PORTC &0xF0); //take tenth of the number
		_delay_ms(3);

		PORTA = (1 << 4);
		PORTC=(hours%10) | (PORTC &0xF0); //take units of the number
		_delay_ms(3);

		PORTA = (1 << 5);
		PORTC=(hours/10) | (PORTC &0xF0);; //take tenth of the number
		_delay_ms(3);
	}

}
