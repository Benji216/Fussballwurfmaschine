#define F_CPU 8000000UL

#include <avr/io.h>
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
	 int adc_h;
	 float Vin;
	 int h = 0;
	 
	allinit();
	
	
	while(1)
	{
		while(ADCSRA & (1<<ADSC)); //warten auf Wandlungsende
		adc_h = ADCL;
		adc_h = ADCH;
		
		if(adc_h != h)
		{
			Vin = (adc_h*5)/256;
			dtostrf(Vin, 3, 2, buffer);
			LCD_cmd(0x01);
			LCD_string(buffer);
			h = adc_h;
			_delay_ms(500);
		}
		ADCSRA = ADCSRA|(1<<ADSC);  //Wandler neu starten
		
		/*OCR0A++;
		if(OCR0A>=256)
		{
			OCR0A = 0;
		}
		_delay_ms(50);*/
		
	}
	return 0;
}