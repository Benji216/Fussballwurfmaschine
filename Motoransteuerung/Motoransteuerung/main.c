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
unsigned int drehzahl;

int main (void)
{
    char buffer[20];    //Wird ben�tigt um am LCD Ziffern auszugeben
	allinit();

	while(1)
	{
        drehzahl = RechteDrehzahlmessung();

        LCD_cmd(0x01); // LCD l�schen
        LCD_cmd(0x80); //gehe zu 2. Zeile, 6. Position
        itoa (drehzahl, buffer, 10);
        LCD_string(buffer);

		if(duty1 >= 0 && duty1 <= 100)
		{
			OCR4A = duty1 *  2;//PWM1 Tastverh�ltnis einstellen PC7
		}
		if(duty2 >= 0 && duty2 <= 100)
		{
			OCR4B = duty2 *  2;//PWM2 Tastverh�ltnis einstellen PB6
		}

		/*if(drehmessh)
        {
            unsigned int drehzahl;
            drehzahl = RechteDrehzahlmessung();
            LCD_cmd(0xC5); //gehe zu 2. Zeile, 6. Position
            itoa (drehzahl, buffer, 10);
            LCD_string(buffer);
            if(drehzahl < (MAXDREHZAHL/100 * LCDduty1)) // dreht sich zu langsam
            {
                if(duty1 <100)
                {
                    duty1++;
                }
            }
            else if(drehzahl > (MAXDREHZAHL/100 * LCDduty1)) // dreht sich zu schnell
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

ISR (INT0_vect) // Wenn Taster an PD0 gedr�ckt
{
	if(LCDduty1 < 100)
	{
		duty1++;
		LCDduty1++;
	}
	dutyh = 1;
	drehmessh = 1;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0zeit = 700; //700ms
}

ISR (INT1_vect) // Wenn Taster an PD1 gedr�ckt
{
	if(LCDduty1 > 0)
	{
		duty1--;
		LCDduty1--;
	}
	dutyh = 2;
	drehmessh = 1;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0zeit = 700; //700ms
}
ISR (INT2_vect) // Wenn Taster an PD2 gedr�ckt
{
	if(LCDduty2 < 100)
	{
		duty2++;
		LCDduty2++;
	}
	dutyh = 3;
	drehmessh = 1;
	TCCR0B = TCCR0B | (1<<CS01)|(1<<CS00);//:64 Teiler => dt=64/8MHz=8us startet timer0
	timer0h = 0;
	timer0zeit = 700; //700ms
}
ISR (INT3_vect) // Wenn Taster an PD3 gedr�ckt
{
	if(LCDduty2 > 0)
	{
		duty2--;
		LCDduty2--;
	}
	dutyh = 4;
	drehmessh = 1;
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
					if(LCDduty1 < 100)
					{
						duty1++;
						LCDduty1++;
					}
				}
				break;
			case 2: //Taster 2
				if ((PIND & (1<<PIND1)) == 0) //wenn PD1 = LOW
				{
					if(LCDduty1 > 0)
					{
						duty1--;
						LCDduty1--;
					}
				}
				break;
				case 3: //Taster 3
				if ((PIND & (1<<PIND2)) == 0) //wenn PD2 = LOW
				{
					if(LCDduty2 < 100)
					{
						duty2++;
						LCDduty2++;
					}
				}
				break;
				case 4:
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


	TCNT0 = 256-125; //Reinitialisierung
}

