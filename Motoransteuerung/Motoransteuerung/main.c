#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "LCD1zeil.h"
#include <util/delay.h>
#include "Motoransteuerung.h"

unsigned char LCDduty1 = 0;
unsigned char duty1 = 0;
unsigned char LCDduty2 = 0;
unsigned char duty2 = 0;
unsigned char dutyh = 0;
unsigned short int timer0h = 0;
unsigned short int timer0zeit = 0;
unsigned char drehmessh = 0;
unsigned int drehzahlr = 0;
unsigned int drehzahll = 0;

int main (void)
{
    int Wert[4];
	allinit();

	while(1)
	{
		if(duty1 >= 0 && duty1 <= 100)
		{
			OCR4A = duty1 *  2;//PWM1 Tastverhältnis einstellen PC7
		}
		if(duty2 >= 0 && duty2 <= 100)
		{
			OCR4B = duty2 *  2;//PWM2 Tastverhältnis einstellen PB6
		}

		/*if(drehmessh)
        {
            drehzahlr = RechteDrehzahlmessung();
            if(drehzahlr < (MAXDREHZAHL/100 * LCDduty1)) // dreht sich zu langsam
            {
                if(duty1 <100)
                {
                    duty1++;
                }
            }
            else if(drehzahlr > (MAXDREHZAHL/100 * LCDduty1)) // dreht sich zu schnell
            {
                if(duty1 >0)
                {
                    duty1--;
                }
            }
            else
            {
                drehmessh = 0; //drehmessung beenden
            }
        }*/
       Wert[0] = duty1;
       Wert[1] = duty2;
       Wert[2] = drehzahlr;
       Wert[3] = drehzahll;

       LCDAusgabe(Wert);
	}
	return 0;
}

ISR (TIMER4_FPF_vect)
{
    LCD_cmd(0x01);
	PORTC = PORTC &~(1<<PORTC6);
	PORTC = PORTC &~(1<<PORTC7);
	LCD_cmd(0x80);
	LCD_string("Fehler");
	while(1)
	{
		PORTB = PORTB ^(1<<PORTB7);
		_delay_ms(500);
	}
}

ISR (INT0_vect) // Wenn Taster an PD0 gedrückt
{
	if(LCDduty1 < 100)
	{
		duty1++;
		LCDduty1++;
	}
	dutyh = Taster1;
	drehmessh = 1;
	timer0h = 0;
	timer0zeit = 700; //700ms
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
}

ISR (INT1_vect) // Wenn Taster an PD1 gedrückt
{
	if(LCDduty1 > 0)
	{
		duty1--;
		LCDduty1--;
	}
	dutyh = Taster2;
	drehmessh = 1;
	timer0h = 0;
	timer0zeit = 700; //700ms
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
}
ISR (INT2_vect) // Wenn Taster an PD2 gedrückt
{
	if(LCDduty2 < 100)
	{
		duty2++;
		LCDduty2++;
	}
	dutyh = Taster3;
	drehmessh = 1;
	timer0h = 0;
	timer0zeit = 700; //700ms
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
}
ISR (INT3_vect) // Wenn Taster an PD3 gedrückt
{
	if(LCDduty2 > 0)
	{
		duty2--;
		LCDduty2--;
	}
	dutyh = Taster4;
	drehmessh = 1;
	timer0h = 0;
	timer0zeit = 700; //700ms
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
}

ISR (TIMER0_OVF_vect) //1ms
{
	timer0h++;
	if(timer0h > timer0zeit)
	{
		switch (dutyh)
		{
			case Taster1:
				if ((PIND & (1<<PIND0)) == 0) //wenn PD0 = LOW
				{
					if(LCDduty1 < 100)
					{
						duty1++;
						LCDduty1++;
					}
				}
				break;
			case Taster2:
				if ((PIND & (1<<PIND1)) == 0) //wenn PD1 = LOW
				{
					if(LCDduty1 > 0)
					{
						duty1--;
						LCDduty1--;
					}
				}
				break;
				case Taster3:
				if ((PIND & (1<<PIND2)) == 0) //wenn PD2 = LOW
				{
					if(LCDduty2 < 100)
					{
						duty2++;
						LCDduty2++;
					}
				}
				break;
				case Taster4:
				if ((PIND & (1<<PIND3)) == 0) //wenn PD3 = LOW
				{
					if(LCDduty2 > 0)
					{
						duty2--;
						LCDduty2--;
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

    if(((PIND & (1<<PIND0)) == 1) && ((PIND & (1<<PIND1)) == 1) && ((PIND & (1<<PIND2)) == 1) && ((PIND & (1<<PIND3)) == 1))
	{
		TCCR0B = TCCR0B &~ (1<<CS01) &~ (1<<CS00);//stoppt timer0
		timer0h = 0;
	}


	TCNT0 = 256-125; //Reinitialisierung
}

