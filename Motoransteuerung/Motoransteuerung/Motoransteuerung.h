/*
 * Motoransteuerung.h
 *
 * Created: 16.10.2019 09:09:29
 *  Author: benja
 */


#ifndef MOTORANSTEUERUNG_H_
#define MOTORANSTEUERUNG_H_

#define MAXDREHZAHL 2500
#define VCC 5
#define Taster1 1
#define Taster2 2
#define Taster3 3
#define Taster4 4

void allinit(void);
unsigned short int RechteDrehzahlmessung(void);
void LCDAusgabe(int *Wert);


#endif /* MOTORANSTEUERUNG_H_ */
