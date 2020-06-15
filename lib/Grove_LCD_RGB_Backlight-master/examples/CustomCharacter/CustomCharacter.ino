/*
  CustomCharacter.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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
    
   lcd.begin(16, 2);

   
    #if 1  
        // create a new character
        lcd.createChar(1, empty);
        // create a new character
        lcd.createChar(2, halffill);
        // create a new character
        lcd.createChar(3, fill);
    #endif 
    lcd.setCursor(0, 0);
}



void plot_signal_bar( 
  const rgb_lcd& obj, 
  float sig=0., /// signal strength normalised to 0. to 1.
  int line=0,   /// LCD has two line ( or rows ) 
  const char * rowHead="G:"){
 
    lcd.setCursor(0, line);
    lcd.print(rowHead);

    int barCount = sig * 20;
    lcd.setCursor(2, line);
    for (int i=0;i<barCount;i=i+2){
      lcd.write(3);    
      if (i+1 == barCount){
        lcd.write(2);
      }
    }
    
    lcd.setCursor(12, line);
    lcd.print(int(sig*100));
    lcd.setCursor(15, line);
    lcd.write("%");
}
  

void setup() 
{
 setup_lcd();
}


float sig = 0.;

//void loop() 
//{
//
//    lcd.clear();
//
//    int delayTime = 100;
//    
//    plot_signal_bar(lcd, sig/10. , 0, "G:");
//    plot_signal_bar(lcd, 1 - sig/10. , 1, "B:" );
//   
//    sig += 1.5;
//    sig = sig - 10 * (int(sig) / 10);    
//    
//    
//    delay(delayTime);
//    
//}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
