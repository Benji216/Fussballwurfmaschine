/*
 * CFile1.c
 *
 * Created: 16.10.2019 09:07:10
 *  Author: benja
 */

#include <avr/io.h>
#include <stdlib.h>
#include "LCD1zeil.h"
#include <avr/interrupt.h>
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

	DDRB = DDRB | (1<<DDB7); //PB7 OUTPUT (LED)
	PORTB = PORTB |(1<<PORTB7);//LED ausschalten

	DDRC = DDRC | (1<<DDC7); //OC4A -Pin (PC7) als OUTPUT Timer4
	DDRB = DDRB | (1<<DDB6);//OC4B -Pin (PB6) als Output Timer4

	DDRE = DDRE &~(1<<DDE6);//PE6 Input (AIN+ F�r Komperator)
	DDRF = DDRF &~(1<<DDF4); //PF4 INPUT (AIN- ADC4)

	DDRF = DDRF &~(1<<DDF0); //PF0 INPUT (ADC0 f�r Poti)

	DDRD = DDRD &~(1<<DDD0) &~(1<<DDD1) &~(1<<DDD2) &~(1<<DDD3); //PD0-3 Input (External Interrupt0-3)
	PORTD = PORTD |(1<<PORTD0) |(1<<PORTD1) |(1<<PORTD2) |(1<<PORTD3);//PD0-3 Pull up ein

	DIDR1 = DIDR1 | (1<<AIN0D); //dig. Input Buffer am AIN+ (PE6) disable
	DIDR0 = DIDR0 | (1<<ADC4D); //dig. Input Buffer am ADC4 (PF4) disable


	TCCR4A = TCCR4A | (1<<PWM4A);
	TCCR4A = TCCR4A | (1<<PWM4B);
	TCCR4D = TCCR4D &~(1<<WGM41);
	TCCR4D = TCCR4D &~(1<<WGM40); //Fast PWM Mode

	TCCR4A = TCCR4A | (1<<COM4A1);
	TCCR4A = TCCR4A | (1<<COM4A0); //COM4A1:0=3, invert OC4A(PC7) Pin ein

	TCCR4A = TCCR4A | (1<<COM4B1);
	TCCR4A = TCCR4A | (1<<COM4B0); //COM4B1:0=3, invert OC4B(PB6) Pin ein


	TCCR4D = TCCR4D | (1<<FPEN4);//Fault Protection Enable
	TCCR4D = TCCR4D | (1<<FPIE4); //Fault Protection Interrupt Enable
	ADCSRA = ADCSRA &~(1<<ADEN); //ADC aus Komparator ein
	TCCR4D = TCCR4D | (1<<FPAC4);// Analog comparator f�r AIN+ ausw�hlen
	ADCSRB = ADCSRB | (1<<ACME); //AIN- h�ngt am ADC Multiplexer
	ADMUX = ADMUX | (1<<MUX2); //ADC4 als AIN- Input w�hlen

	OCR4C = 200; //TOP f�r f_PWM = 10 kHz, f_PWM = f_CLK_T4/OCR4C


	TCCR4B = TCCR4B &~(1<<CS43);
	TCCR4B = TCCR4B &~(1<<CS42);
	TCCR4B = TCCR4B | (1<<CS41);
	TCCR4B = TCCR4B | (1<<CS40); //Timer4 Prescaler 4, Start PWM

	TIMSK0 = TIMSK0 | (1<<TOIE0); //Timer_0 Overflow Interrupt freigeben
	TCNT0 = 256-125; //Startwert f�r 1ms (125*8us=1ms)


	EICRA = EICRA | (1<<ISC01); //INT0 fallende Flanke
	EIMSK = EIMSK | (1<<INT0); //INT0 freigeben

	EICRA = EICRA | (1<<ISC11); //INT1 fallende Flanke
	EIMSK = EIMSK | (1<<INT1); //INT1 freigeben

	EICRA = EICRA | (1<<ISC21); //INT2 fallende Flanke
	EIMSK = EIMSK | (1<<INT2); //INT2 freigeben

	EICRA = EICRA | (1<<ISC31); //INT3 fallende Flanke
	EIMSK = EIMSK | (1<<INT3); //INT3 freigeben


	sei();
}
unsigned short int RechteDrehzahlmessung(void)
{
	unsigned char adc_h;
	unsigned short int drehzahl;


	TCCR4B = TCCR4B &~(1<<CS43);
	TCCR4B = TCCR4B &~(1<<CS42);
	TCCR4B = TCCR4B &~(1<<CS41);
	TCCR4B = TCCR4B &~(1<<CS40); //Timer4 Prescaler 0, Stoppet Timer4

	ADMUX = ADMUX &~(1<<REFS1);
	ADMUX = ADMUX | (1<<REFS0); //Vref=AVCC

	ADMUX = ADMUX & 0b11100000; //MUX4:0 = 0
	ADCSRB = ADCSRB &~ (1<<MUX5); //MUX5 = 0 => MUX5:0=0 => //ADC0 (PF0) Single Ended

	ADCSRA = ADCSRA|(1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1); //Wandler ein, //Start Wandler,:64 Teiler

	while(ADCSRA & (1<<ADSC)); //warten auf Wandlungsende



    adc_h = ADCL;
    drehzahl = ADCH & 0b00000011;
    drehzahl = drehzahl <<8;
    drehzahl = drehzahl | adc_h; // Wert 0 bis 1023

    drehzahl = (float)MAXDREHZAHL/1023 * drehzahl;  //Drehzahl ausrechnen



    ADCSRA = ADCSRA &~(1<<ADEN); //ADC aus Komparator ein
    ADMUX = ADMUX | (1<<MUX2); //ADC4 als AIN- Input w�hlen

    TCCR4B = TCCR4B &~(1<<CS43);
	TCCR4B = TCCR4B &~(1<<CS42);
	TCCR4B = TCCR4B | (1<<CS41);
	TCCR4B = TCCR4B | (1<<CS40); //Timer4 Prescaler 4, Start PWM


	return(drehzahl);
}
void LCDAusgabe(int *Wert)
{
    char buffer[20];
    int hilf;
    char ausg;
    char stelle;
    int i;

    stelle = 0x80;
    hilf = *Wert;
    for(i=100;i>0;i= i/10)
    {
        ausg = hilf /i;
        LCD_cmd(stelle);
        itoa(ausg, buffer,10);
        LCD_string(buffer);
        hilf = hilf - ausg * i;
        stelle++;
    }

    Wert++;
    stelle = 0x8d;
    hilf = *Wert;
    for(i=100;i>0;i= i/10)
    {
        ausg = hilf /i;
        LCD_cmd(stelle);
        itoa(ausg, buffer,10);
        LCD_string(buffer);
        hilf = hilf - ausg * i;
        stelle++;
    }
}

