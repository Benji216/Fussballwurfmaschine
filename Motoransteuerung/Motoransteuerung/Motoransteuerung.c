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
	 DDRC = DDRC | (1<<DDC7); //OC4A -Pin (PC7) als OUTPUT Timer4
	 DDRC = DDRC | (1<<DDC6); //OC4A#-Pin (PC6) als OUTPUT
	 DDRF = DDRF &~(1<<DDF0); //PF0 INPUT (ADC0)
	 
	 ADMUX = ADMUX &~(1<<REFS1);
	 ADMUX = ADMUX | (1<<REFS0); //Vref=AVCC
	 ADMUX = ADMUX | (1<<ADLAR); //linksbündig
	 ADMUX = ADMUX & 0b11100000; //MUX4:0 = 0
	 ADCSRB = ADCSRB &~ (1<<MUX5); //MUX5 = 0 => MUX5:0=0 => //ADC0 (PF0) Single Ended
	 
	 ADCSRA = ADCSRA|(1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1); //Wandler ein, //Start Wandler,:64 Teiler
	 
	 
	 TCCR4A = TCCR4A | (1<<PWM4A);
	 TCCR4D = TCCR4D &~(1<<WGM41);
	 TCCR4D = TCCR4D &~(1<<WGM40); //Fast PWM Mode
	 
	 TCCR4A = TCCR4A &~(1<<COM4A1);
	 TCCR4A = TCCR4A | (1<<COM4A0); //COM4A1:0=1, OC4A(PC7) und OC4A#(PC6) Pins ein 
	 
	 OCR4C = 200; //TOP für f_PWM = 10 kHz, f_PWM = f_CLK_T4/OCR4C
	 
	 OCR4A = 100; //1:1 Tastverhältnis (ohne Dead Time)
	 
	 TCCR4B = TCCR4B &~(1<<CS43);
	 TCCR4B = TCCR4B &~(1<<CS42);
	 TCCR4B = TCCR4B | (1<<CS41);
	 TCCR4B = TCCR4B | (1<<CS40); //Timer4 Prescaler 4, Start PWM
}