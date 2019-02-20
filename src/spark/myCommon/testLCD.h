#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

void setup_lcd(){
    // (Col , row)
    // make some custom characters:
    byte fill[8] = {
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111
    };
    
    byte halffill[8] = {
        0b11100,
        0b11100,
        0b11100,
        0b11100,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
    };
    
    byte empty[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
    };

    byte rarrow[8] = {
        0b01100,
        0b00110,
        0b00011,
        0b11111,
        0b11111,
        0b00011,
        0b00110,
        0b01100,
    };    
    byte larrow[8] = {
        0b00110,
        0b01100,
        0b11000,
        0b11111,
        0b11111,
        0b11000,
        0b01100,
        0b00110,
    };    
   lcd.begin(16, 2);

   
    #if 1  
        // create a new character
        lcd.createChar(1, empty);
        // create a new character
        lcd.createChar(2, halffill);
        // create a new character
        lcd.createChar(3, fill);
        lcd.createChar(4, rarrow);
        lcd.createChar(5, larrow);
    #endif 
    lcd.setCursor(0, 0);
}



void plot_signal_bar( 
  const rgb_lcd& obj, 
  float sig=0., /// signal strength normalised to 0. to 1.
  int line=0,   /// LCD has two line ( or rows ) 
  const char * rowHead="G:"
){
   int shape;
    lcd.setCursor(0, line);
    lcd.print(rowHead);
    
    const char * signChar="-+";
    int sign = sig>=0;

    if (sign == 1){
        // right arrow
	shape= 4;
	}else{
        // left arrow
	shape= 5;
    }
    sig = max(min(abs(sig),1),0);    
    int barCount = sig * 10;

    for (int i=0; i + 1<=barCount;i=i+1){
	if ((i + 2 )<strlen(rowHead)){
		continue;
	}
	lcd.write(shape);    
      if (i+3 == barCount){
;
      }
    }
    if (sig!=0){
	    lcd.setCursor(12, line);
	    lcd.write(signChar[sign]);
	    lcd.print(min(99, int(sig*100)) );
	//    lcd.print(barCount);
	    lcd.setCursor(15, line);
	    lcd.write("%");
	}
}
