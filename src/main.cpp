/******************************************************************************
*   Author       -  Görföl Zoltán
*   Date         -  2020.01.27.
*   Chip         -  Ardiono Nano, LED & KEY Board (TM1638)
*   Compiler     -  PlatformIO & Vs Code
*
*   Szamlalo (fel/le/0-99999) tekercselogephez
*   
*******************************************************************************
*
*   +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
*   !   ! !   ! !   ! !   ! !   ! !   ! !   ! !   !    
*   ! 0 ! ! 1 ! ! 2 ! ! 3 ! ! 4 ! ! 5 ! ! 6 ! ! 7 !
*   !   ! !   ! !   ! !   ! !   ! !   ! !   ! !   !
*   +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
*
*   +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
*   ! O ! ! O ! ! O ! ! O ! ! O ! ! O ! ! O ! ! O !    
*   +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
*    Fel  Le    Reset 10000 1000  100   10    1
*
*******************************************************************************/

#include "TM1638.h"

int i;                                     // munka valtozo
long n= 0;                                 // szamlalo
boolean irany = true;                      // irany (true-fel, false-le)
unsigned char kapcsolo_allapot= 0;         // kapcsolo elozo allapot
unsigned char key;                         // tárolt bill.

TM1638 k( 5, 6, 7);                        // data_io, clock, strobe

void kijelzes(boolean irany, long num) {
     String work = String(num, DEC);
     while (work.length() < 5) work = " " + work;
     if( irany ) work = "FEL" + work;
     else        work = "LE " + work; 
     k.setDisplayToString(work, 0);

     byte LedByte = 0;
     long q = num;                         // LED kiiras
     if ( q % 10) LedByte = 128;           // 1
     q = q / 10;
     if ( q % 10) LedByte = 128+64;        // 10
     q = q / 10;
     if ( q % 10) LedByte = 128+64+32;     // 100
     q = q / 10;
     if ( q % 10) LedByte = 128+64+32+16;  // 1000
     q = q / 10;
     if ( q % 10) LedByte = 128+64+32+16+8;// 10000
     k.setLEDs(LedByte);
}

void incr_decr(boolean irany, long num) {
    if( irany ) n = min(n+num, 99999);
    else        n = max(n-num, 0);  
}

void setup()
{
   pinMode(2, INPUT_PULLUP); // erintkezok felhuzasa
   pinMode(3, INPUT_PULLUP);
   kijelzes(irany, n);
}

void loop()
{   
   if ( ( digitalRead( 2) == 0) && ( kapcsolo_allapot == 0)) { // kapcsolo be
      kapcsolo_allapot= 1;                                     // 2-es 3-as csere felfutó v. lefutó élre számoljon.
      incr_decr( irany, 1 );                                   // 2-es a kapcs. középső vez.-ről ---> nyomáskor számol.
   }                                                           // ell. esetben elengedéskor számolna.                                           
   if ( digitalRead( 3) == 0) kapcsolo_allapot= 0;             // kapcsolo ki
   
   i= k.getButtons();                   // bill. kezeles
   if (i > 0) {
       key= i;                          // bill. eltarolasa
   }
   else {                               // bill. elengedése utáni feldolg.
     if ( key == 1)   irany= true;      // fel
     if ( key == 2)   irany= false;     // le
     if ( key == 4)   n= 0;             // reset
     if ( key == 8)   incr_decr(irany, 10000);
     if ( key == 16)  incr_decr(irany, 1000);
     if ( key == 32)  incr_decr(irany, 100);
     if ( key == 64)  incr_decr(irany, 10);
     if ( key == 128) incr_decr(irany, 1);
     kijelzes(irany, n);
     key = 0;
   }
}
