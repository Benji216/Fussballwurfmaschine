//////////////////////////////////////////////////////////////////////////////
//
// Headerfile LCD.h zur LCD Ansteuerung am Port D f�r das HTL Demoboard
//
// Befehle: LCD_init(), LCD_cmd(char data), LCD_send(char data)
//          LCD_string(char *data)
//
// LCD_init();               initialisiert Port D
//                           und LCD im 4-Bit Mode, 2 Zeilen, 5x7 Dots
//                           Bsp.: LCD_init();
//
// LCD_cmd(char data);       schickt Befehl ans LCD
//                           Bsp.: LCD_cmd(0xC5);  //gehe zu 2. Zeile, 6. Position
//
// LCD_send(char data);      schickt Daten ans LCD
//                           Bsp.: LCD_send(0xEF); //sendet ein �
//
// LCD_string(char *data);   schickt eine Zeichenkette ans LCD
//                           Bsp.: LCD_string("Hallo");    //sendet Hallo
//
// Pinbelegung am Board:
// LCD | 32U4     | Bemerkung
// ----|----------|--------------------
// DB7 |    PD3   |
// DB6 |	PD2   |
// DB5 |	PD1   |
// DB4 |	PD0   |
// DB3 |	-     | wird nicht ben�tigt
// DB2 |	-     | wird nicht ben�tigt
// DB1 |	-     | wird nicht ben�tigt
// DB0 |	-     | wird nicht ben�tigt
// E   |	PD5   |
// R/W |	-     | per HW auf GND gelegt
// RS  |	PD4   |
//
////////////////////////////////////////////////////////////////////////////////



#include <avr/io.h>
#include "LCD1zeil.h"
#include <util/delay.h>		// _delay_ms() geht nur bis max. 262.14 ms / F_CPU !!!

void delay_ms (unsigned int ms)   //Hilfsfunktion: Zeitvernichtung
{
unsigned int i;
	for (i=0; i<ms; i++)
	{
		_delay_ms(1);
	}

}

void Enable(void)		//Hilfsfunktion: H=>L Flanke der Enable Leitung (E)
{
	PORTLCD = PORTLCD | (1<<E);	//E = 1
	delay_ms(5);
	PORTLCD = PORTLCD &~(1<<E);	//E = 0
	delay_ms(5);
}

void Enable2(void)		//Hilfsfunktion: H=>L Flanke der Enable Leitung (E)
{
	PORTLCD = PORTLCD | (1<<E);	//E = 1
	_delay_us(100);
	PORTLCD = PORTLCD &~(1<<E);	//E = 0
	_delay_us(100);
}

void Enable3(void)		//Hilfsfunktion: H=>L Flanke der Enable Leitung (E)
{
	PORTLCD = PORTLCD | (1<<E);	//E = 1
	delay_ms(1);
	PORTLCD = PORTLCD &~(1<<E);	//E = 0
	delay_ms(1);
}

////////////////////////////////////////////////////////////////////////////
//
// LCD_init(..) Initialisierung: Port D, 4-Bit Mode, 2 Zeilen, 5x7 Dots
//
////////////////////////////////////////////////////////////////////////////
void LCD_init(void)
{
	DDRLCD = DDRLCD |(1<<E) |(1<<RS);               			//E,RS als Ausgang
	DDRLCD = DDRLCD |(1<<DB7)|(1<<DB6)|(1<<DB5)|(1<<DB4);		//DB7..DB4 als Ausgang

	delay_ms(50);	//lt. Datenblatt min. 15ms nach Power ON warten
	PORTLCD = PORTLCD & (~(1<<RS) &  ~(1<<E)); //RS=0,E=0 (RW=0 per HW)

// Function Set
	//DB7..DB4 = 0011
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6));	//Interface auf 8 Bit
	PORTLCD = PORTLCD | (1<<DB5) | (1<<DB4);
	Enable();

	//DB7..DB4 = 0011
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6));	//Interface auf 8 Bit
	PORTLCD = PORTLCD | (1<<DB5) | (1<<DB4);
	Enable();

	//DB7..DB4 = 0011
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6));	//Interface auf 8 Bit
	PORTLCD = PORTLCD | (1<<DB5) | (1<<DB4);
	Enable();

	//DB7..DB4 = 0010
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) &~(1<<DB4));
	PORTLCD = PORTLCD | (1<<DB5);					//Interface auf 4 Bit
	Enable();

// 2-zeilig, 5x8 Matrix
	//DB7..DB4 = 0010
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) &~(1<<DB4));
	PORTLCD = PORTLCD | (1<<DB5);					//Upper Nibble
	Enable();

	//DB7..DB4 = 0000
	PORTLCD = PORTLCD | (1<<DB7);			//Lower Nibble
	PORTLCD = PORTLCD & (~(1<<DB6) & ~(1<<DB5) & ~(1<<DB4));
	Enable();

//Display Off //
	//DB7..DB4 = 0000
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) & ~(1<<DB5) & ~(1<<DB4)); //Upper Nibble
	Enable();

	//DB7..DB4 = 1000
	PORTLCD = PORTLCD | (1<<DB7);					//Lower Nibble
	PORTLCD = PORTLCD & (~(1<<DB6) & ~(1<<DB5) & ~(1<<DB4));
	Enable();

//Clear Display //
	//DB7..DB4 = 0000
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) & ~(1<<DB5) & ~(1<<DB4)); //Upper Nibble
	Enable();

	//DB7..DB4 = 0001
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) & ~(1<<DB5)); //Lower Nibble
	PORTLCD = PORTLCD | (1<<DB4);
	Enable();

//No Display Shift //
	//DB7..DB4 = 0000
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) & ~(1<<DB5) & ~(1<<DB4)); //Upper Nibble
	Enable();

	//DB7..DB4 = 0011
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6));	//Lower Nibble
	PORTLCD = PORTLCD | (1<<DB5) | (1<<DB4);
	Enable();

// Display ON , Cursor ON, Blinken ON //
	//DB7..DB4 = 0000
	PORTLCD = PORTLCD & (~(1<<DB7) & ~(1<<DB6) & ~(1<<DB5) & ~(1<<DB4)); //Upper Nibble
	Enable();

	//DB7..DB4 = 1111
	PORTLCD = PORTLCD | (1<<DB7) | (1<<DB6) | (1<<DB5) | (1<<DB4);	//Lower Nibble
	Enable();
}

////////////////////////////////////////////////////////////////////////////
//
// LCD_send(..) sendet 1 Byte im 4-Bit Mode
//
////////////////////////////////////////////////////////////////////////////
void LCD_send(char data)
{
	char temp = data;

		PORTLCD = PORTLCD | (1<<RS);	//SFR vom LCD mit RS auf Daten umschlten

		//Upper Nibble senden
		if (temp & 0b10000000) {PORTLCD = PORTLCD | (1<<DB7);}
		else {PORTLCD = PORTLCD & ~(1<<DB7);}

		if (temp & 0b01000000) {PORTLCD = PORTLCD | (1<<DB6);}
		else {PORTLCD = PORTLCD & ~(1<<DB6);}

		if (temp & 0b00100000) {PORTLCD = PORTLCD | (1<<DB5);}
		else {PORTLCD = PORTLCD & ~(1<<DB5);}

		if (temp & 0b00010000) {PORTLCD = PORTLCD | (1<<DB4);}
		else {PORTLCD = PORTLCD & ~(1<<DB4);}

		Enable2();
		//delay_ms(1);

		//Lower Nibble senden
		if (temp & 0b00001000) {PORTLCD = PORTLCD | (1<<DB7);}
		else {PORTLCD = PORTLCD & ~(1<<DB7);}

		if (temp & 0b00000100) {PORTLCD = PORTLCD | (1<<DB6);}
		else {PORTLCD = PORTLCD & ~(1<<DB6);}

		if (temp & 0b00000010) {PORTLCD = PORTLCD | (1<<DB5);}
		else {PORTLCD = PORTLCD & ~(1<<DB5);}

		if (temp & 0b00000001) {PORTLCD = PORTLCD | (1<<DB4);}
		else {PORTLCD = PORTLCD & ~(1<<DB4);}

		Enable2();
		//delay_ms(1);

}

////////////////////////////////////////////////////////////////////////////
//
// LCD_cmd(..) Befehl senden im 4-Bit Mode
//
////////////////////////////////////////////////////////////////////////////
void LCD_cmd(char data)
{
	char temp = data;

		PORTLCD = PORTLCD & ~(1<<RS);	//SFR vom LCD mit RS auf Befehle umschalten

		//Upper Nibble senden
		if (temp & 0b10000000) {PORTLCD = PORTLCD | (1<<DB7);}
		else {PORTLCD = PORTLCD & ~(1<<DB7);}

		if (temp & 0b01000000) {PORTLCD = PORTLCD | (1<<DB6);}
		else {PORTLCD = PORTLCD & ~(1<<DB6);}

		if (temp & 0b00100000) {PORTLCD = PORTLCD | (1<<DB5);}
		else {PORTLCD = PORTLCD & ~(1<<DB5);}

		if (temp & 0b00010000) {PORTLCD = PORTLCD | (1<<DB4);}
		else {PORTLCD = PORTLCD & ~(1<<DB4);}

		Enable3();
		//delay_ms(1);

		//Lower Nibble senden
		if (temp & 0b00001000) {PORTLCD = PORTLCD | (1<<DB7);}
		else {PORTLCD = PORTLCD & ~(1<<DB7);}

		if (temp & 0b00000100) {PORTLCD = PORTLCD | (1<<DB6);}
		else {PORTLCD = PORTLCD & ~(1<<DB6);}

		if (temp & 0b00000010) {PORTLCD = PORTLCD | (1<<DB5);}
		else {PORTLCD = PORTLCD & ~(1<<DB5);}

		if (temp & 0b00000001) {PORTLCD = PORTLCD | (1<<DB4);}
		else {PORTLCD = PORTLCD & ~(1<<DB4);}

		Enable3();
		//delay_ms(1);

}

////////////////////////////////////////////////////////////////////////////
//
// LCD_string(..) sendet ganzen String im 4-Bit Mode
//
////////////////////////////////////////////////////////////////////////////
void LCD_string(char *data)
{
	    while (*data != '\0')	//bis zum letzten Zeichen
        	{LCD_send(*data++);}
}



void LCD_secCursor(char n)
{
        LCD_cmd(0x80+n);
 }
