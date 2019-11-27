#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "LCD1zeil.h"
#include <util/delay.h>
#include "Motoransteuerung.h"

unsigned char duty1 = 0;
unsigned char duty2 = 0;
unsigned char dutyh = 0;
unsigned short int timer0h = 0;
unsigned short int timer0zeit = 0;

int main (void)
{
	 char buffer[20];    //Wird benötigt um am LCD Ziffern auszugeben
	 
	allinit();
	
	Fault_Protection();
	
	
	while(1)
	{
		LCD_cmd(0xC5); //gehe zu 2. Zeile, 6. Position
		LCD_send('a');
		if(duty1 >= 0 && duty1 <= 100)
		{
			OCR4A = duty1 *  2;//PWM1 Tastverhältnis einstellen
		}
		if(duty2 >= 0 && duty2 <= 100)
		{
			OCR4B = duty2 *  2;//PWM2 Tastverhältnis einstellen
		}
	}
	return 0;
}

ISR (TIMER4_FPF_vect)
{
	PORTC = PORTC &~(1<<PORTC6);
	PORTC = PORTC &~(1<<PORTC7);
	while(1)
	{
		PORTB = PORTB ^(1<<PORTB7);
		_delay_ms(500);
	}
}

ISR (INT0_vect) // Wenn Taster an PD0 gedrückt
{
	if(duty1 < 100)
	{
		duty1++;
	}
	dutyh = 1;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0zeit = 700; //700ms
}

ISR (INT1_vect) // Wenn Taster an PD1 gedrückt
{
	if(duty1 > 0)
	{
		duty1--;
	}
	dutyh = 2;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0zeit = 700; //700ms
}
ISR (INT2_vect) // Wenn Taster an PD2 gedrückt
{
	if(duty2 < 100)
	{
		duty2++;
	}
	dutyh = 3;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0h = 0;
	timer0zeit = 700; //700ms
}
ISR (INT3_vect) // Wenn Taster an PD3 gedrückt
{
	if(duty2 > 0)
	{
		duty2--;
	}
	dutyh = 4;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0zeit = 700; //700ms
}

ISR (TIMER0_OVF_vect)
{
	timer0h++;
	if(((PIND & (1<<PIND0)) == 1) && ((PIND & (1<<PIND1)) == 1) && ((PIND & (1<<PIND2)) == 1) && ((PIND & (1<<PIND3)) == 1))
	{
		TCCR0B = TCCR0B &~ (1<<CS01) &~ (1<<CS00);//stoppt timer0
		timer0h = 0;
	}
	if(timer0h > timer0zeit)
	{
		switch (dutyh)
		{
			case 1: // Taster 1
				if ((PIND & (1<<PIND0)) == 0) //wenn PD0 = LOW
				{
					if(duty1 < 100)
					{
						duty1++;
					}
				}
				break;
			case 2: //Taster 2
				if ((PIND & (1<<PIND1)) == 0) //wenn PD1 = LOW
				{
					if(duty1 > 0)
					{
						duty1--;
					}
				}
				break;
				case 3: //Taster 3
				if ((PIND & (1<<PIND2)) == 0) //wenn PD2 = LOW
				{
					if(duty2 < 100)
					{
						duty2++;
					}
				}
				break;
				case 4:
				if ((PIND & (1<<PIND3)) == 0) //wenn PD3 = LOW
				{
					if(duty2 > 0)
					{
						duty2--;
					}
				}
				break;
			default:
				{
					TCCR0B = TCCR0B &~ (1<<CS01) &~ (1<<CS00);//stoppt timer0
					timer0h = 0;
					dutyh = 0;
					break;
				}
		}
		timer0zeit = 50; // 50 ms
		timer0h = 0;
	}
	
	
	TCNT0 = 256-125; //Reinitialisierung
}

