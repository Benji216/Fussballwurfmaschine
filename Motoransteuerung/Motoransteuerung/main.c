#include <avr/io.h>
#include <stdlib.h>
#include "LCD1zeil.h"
#include <util/delay.h>
#include "Motoransteuerung.h"


int main (void)
{
	CLKPR = 0x80;
	CLKPR = 0x01; //16MHz / 2 = 8MHz CLK_IO
	
	MCUCR = MCUCR|(1<<JTD); //JTAG disable
	MCUCR = MCUCR|(1<<JTD);
	
	 char buffer[20];    //Wird benötigt um am LCD Ziffern auszugeben
	 unsigned char adc_h;
	 unsigned char Vin;
	 unsigned char h = 0;
	 
	allinit();
	
	
	while(1)
	{
		while(ADCSRA & (1<<ADSC)); //warten auf Wandlungsende
		adc_h = ADCL;
		adc_h = ADCH;
		adc_h = adc_h>>1;
		
		if(adc_h != h)
		{
			Vin = adc_h;
			if(Vin >=0 && Vin <= 27)
			{
				Vin = 0;
			}
			else
			{
				Vin = Vin - 27;
			}
			
			OCR4A = Vin * 2; //PWM Tastverhältnis einstellen
			
			itoa (Vin,buffer,10);
			LCD_cmd(0x01);
			LCD_string(buffer);
			h = adc_h;
			_delay_ms(200);

			
		}
		ADCSRA = ADCSRA|(1<<ADSC);  //Wandler neu starten
		
		
		
	}
	return 0;
}