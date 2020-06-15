#include "AS726X.h"
AS726X sensor;

#include "myCommon/testLCD.h"
#include "myCommon/testTemp.h"
#include <math.h>   
double Log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}  

void softReset(){
  asm volatile ("  jmp 0");
}
const int colorR = 127;
const int colorG = 127;
const int colorB = 127;

int led = 9;           // the PWM pin the LED is attached to
int brightness1 = 0;    // how bright the LED is
int fadeAmount1 = 15;    // how many points to fade the LED by

int sensorPin = A0;
int buttonPin = A1;


int useSensor = 1;
//int useSensor = 1;
int measure_mode=0;

int cycle = 2; /// set to 1 if 

//int controlPin = 8;
//int treatPin = 9;

int controlPin = 9;
int treatPin = 8;

int readPin;
int stopPin;

struct util {
  float control_spG = 1.0;
  float treat_spG  = 1.0;
  float temp = 0.0;
  int cycle = 2;
  char line2[256];
} mem ;

void updateLine2(struct util& mem, const char * sep) {
  sprintf( mem.line2,"T:%2dC %s %3d,%3d",int(mem.temp),
    sep,
    int(10*mem.control_spG),
    int(10*mem.treat_spG)
    );
}
// the setup routine runs once when you press reset:
void setup() {
  delay(1000);
  Serial.begin(115200);
  
  setup_lcd();
  plot_signal_bar(lcd, 0, 
    0, "Resetting:" );

  pinMode(8, OUTPUT);
//  pinMode(9, OUTPUT); 
//  while (1==1){
//     Serial.println(digitalRead(buttonPin));
//     delay(500);
//  }
  if (digitalRead(buttonPin)==1){
    mem.cycle = 2;
  }
  int pin;
  for (pin = 3; pin <= 5; pin += 1){
    pinMode(pin,OUTPUT);
    digitalWrite(pin,HIGH);
  }
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  if (useSensor){
   Serial.println("initialising");
   digitalWrite(controlPin, HIGH);
   digitalWrite(treatPin, LOW);
   sensor.begin();   
   sensor.setMeasurementMode(measure_mode);
//   sensor.softReset();
//   delay(1000);
   
   digitalWrite(treatPin,HIGH);
   digitalWrite(controlPin,LOW);
   sensor.begin();   
   sensor.setMeasurementMode(measure_mode);
//   sensor.softReset();
//   delay(1000);
   Serial.println("Using spectral sensor");
  }
  
  
  Serial.println("Intialised!");
}





int sig = 0;
int sensorVal = 0;

float spV,spB,spG,spY,spO,spR,spAll;
//float green, blue,yellow;
  float val;

void loop() {
  sig += 1;
//  sig += 1;
  sig = sig %20;
//  sig = sig - 20 * (int(sig) / 20);    
  float sig1 = min(sig,20-sig);

  brightness1 = sig1/10. * 255;
  
  // LED brightness
  int cycle = mem.cycle;
  if ( sig % cycle ==1){
    char header[128];
    sprintf(header,"R%1d", sig % (2*cycle) / 2);
    updateLine2(mem, header);
    plot_signal_bar(lcd,0,1,mem.line2);

    Serial.print("changing sensor...\n");
    
    if (  sig % (2*cycle) ==1){
      readPin=controlPin;
      stopPin=treatPin;
      Serial.print("measuring control\n");
    }else{    
      readPin=treatPin;
      stopPin=controlPin;
      Serial.print("measuring treatment\n");
    }
   digitalWrite(readPin,HIGH);
   digitalWrite(stopPin,LOW);
   
   sensor.softReset();
   delay(1400);
  }
//   delay(100);

  if (digitalRead(buttonPin)==1){
    softReset();
  }

   
//   Serial.print("Waiting for data\n");
   while (sensor.dataAvailable()==0){
      delay(50);
   }  
  
  if(useSensor){
//    Serial.println("Measuring");
//    sensor.takeMeasurements();

    spV = sensor.getCalibratedViolet();
    spB = sensor.getCalibratedBlue();
    spG = sensor.getCalibratedGreen();
    spY = sensor.getCalibratedYellow();
//    spO=  sensor.getCalibratedOrange();
//    spR = sensor.getCalibratedRed();

//   spAll = spV  + spB + spG + spO + spR;
   spAll = spV  + spB + spG;

  } else{
   float blue  = float(sig)/20;
   float green = 1 - sig1 ;
  }

  if (readPin == controlPin){
    mem.control_spG = spG;
  }else if (readPin == treatPin){
    mem.treat_spG = spG;
  }
  
//   blue  = sig1/10;
//   green = 1 - sig1/10 ;

  mem.temp = getTemp(A0);
  // Update LCD
  lcd.clear();
//  float bg_ratio = green/blue ;
//  float raG = spG / spAll;
//  float ratio = mem.treat_spG/mem.control_spG;
  float pcount= 0.1;
//  float ratio = (pcount + mem.treat_spG)/ ( pcount + mem.control_spG );
  float ratio = (mem.treat_spG - mem.control_spG )/\
    ( mem.treat_spG + mem.control_spG + pcount );
    
  float log2Ratio = Log2(ratio);
  if (mem.cycle==1){
    val = spG/spAll;
    
  }else if (mem.cycle==2){
//    val = (ratio + 1 )/2;
    val = ratio;
    
  }

  plot_signal_bar(lcd, val , 
    0, "G:" );
  updateLine2(mem,"  ");
  plot_signal_bar(lcd, 0, 
    1, mem.line2 );

    Serial.print("sig: ");
    Serial.print(val);
    Serial.print(",");
  
    Serial.print("blue: ");
    Serial.print(spB);
    Serial.print(",");
    Serial.print("green: ");
    Serial.print(spG);
    Serial.print(",");
    Serial.print("ratio: ");
//    Serial.print(spG/spAll*1000);
    Serial.print(ratio);
    Serial.print(",");
    Serial.print("\n");



  // sensor.printMeasurements();//Prints out all measurements (calibrated)

  // wait for 30 milliseconds to see the dimming effect
  delay(100); 

}
