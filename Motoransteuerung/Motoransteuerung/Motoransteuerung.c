/*
 * CFile1.c
 *
 * Created: 16.10.2019 09:07:10
 *  Author: benja
 */ 

#include <avr/io.h>
#include "LCD1zeil.h"
#include <util/delay.h>
#include "Motoransteuerung.h"


void allinit(void)
{
	CLKPR = 0x80;
	CLKPR = 0x01; //16MHz / 2 = 8MHz CLK_IO
	
	MCUCR = MCUCR|(1<<JTD); //JTAG disable
	MCUCR = MCUCR|(1<<JTD);
	
	 LCD_init();
	 LCD_cmd(0x0C);
	 
	 DDRB = DDRB | (1<<DDB7); //PB7 als Ausgang
	 DDRF = DDRF &~(1<<DDF0); //PF0 INPUT (ADC0)
	 
	 ADMUX = ADMUX &~(1<<REFS1);
	 ADMUX = ADMUX | (1<<REFS0); //Vref=AVCC
	 
	 ADMUX = ADMUX | (1<<ADLAR); //linksbündig
	 
	 ADMUX = ADMUX & 0b11100000; //MUX4:0 = 0
	 
	 ADCSRB = ADCSRB &~ (1<<MUX5); //MUX5 = 0 => MUX5:0=0 => //ADC0 (PF0) Single Ended
	 
	 ADCSRA = ADCSRA|(1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1); //Wandler ein, //Start Wandler,:64 Teiler
	 
	 
	 TCCR0A = TCCR0A | (1<<WGM00);
	 TCCR0A = TCCR0A | (1<<WGM01);
	 TCCR0B = TCCR0B &~(1<<WGM02); //WGM2:0=3 Fast PWM
	 
	 TCCR0A = TCCR0A | (1<<COM0A0);
	 TCCR0A = TCCR0A | (1<<COM0A1); //COM1:0=3 Inverting
	 
	 TCCR0B = TCCR0B | (1<<CS00);
	 TCCR0B = TCCR0B | (1<<CS01);
	 TCCR0B = TCCR0B &~(1<<CS02); //:64 Teiler, startet die PWM
	 
	 OCR0A = 100;
}