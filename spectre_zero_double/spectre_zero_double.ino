#include "AS726X.h"
AS726X sensor;

#include "myCommon/testLCD.h"
#include "myCommon/testTemp.h"


const int colorR = 127;
const int colorG = 127;
const int colorB = 127;

int led = 9;           // the PWM pin the LED is attached to
int brightness1 = 0;    // how bright the LED is
int fadeAmount1 = 15;    // how many points to fade the LED by

int tempSensorPin = A0;

// int getSensor(sensorPin) {
//   int sensorVal = analogRead(sensorPin);
//   Serial.print("Sensor Value: ");
//   Serial.print(sensorVal);
//   Serial.print("\n");
//   return (sensorVal);
// }

void test(){
    printf("Test works! \n");
}

//int useSensor = 0;
int useSensor    = 1;
int measure_mode = 1;
// the setup routine runs once when you press reset:
void setup() {
  delay(1000);
  Serial.begin(115200);
  
  setup_lcd();

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  if (useSensor){
   digitalWrite(8,HIGH);
   digitalWrite(9,LOW);
   sensor.begin();   
   sensor.setMeasurementMode(measure_mode);
//   sensor.softReset();
//   delay(1000);
   
   digitalWrite(9,HIGH);
   digitalWrite(8,LOW);
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

int readPin;
int stopPin;
void loop() {
  sig += 1;
//  sig += 1;
  
  sig = sig - 20 * (int(sig) / 20);    
  float sig1 = min(sig,20-sig);

  brightness1 = sig1/10. * 255;
  
  // LED brightness
  int cycle = 1;
  
  if ( sig % cycle == 0){
    if (  sig % (2*cycle) ==0){
      readPin=8;
      stopPin=9;
      Serial.print("changing to s1\n");
    }else{    
      readPin=9;
      stopPin=8;
        
    }
   digitalWrite(readPin,HIGH);
   digitalWrite(stopPin,LOW);
   
   sensor.softReset();
   delay(800);
//   digitalWrite(stopPin,HIGH);
//   digitalWrite(readPin,LOW);    
  }
//   delay(100);
   
   while (sensor.dataAvailable()==0){
    delay(50);
   }

   
//  analogWrite(9, brightness1 / 8);
  analogWrite(10,  (255 - brightness1));
  analogWrite(LED_BUILTIN,255- brightness1);
//  sensorVal = analogRead(sensorPin);
  
  
  if(useSensor){
    Serial.println("Measuring");
//    sensor.takeMeasurements();

    spV = sensor.getCalibratedViolet();
    spB = sensor.getCalibratedBlue();
    spG = sensor.getCalibratedGreen();
    spY = sensor.getCalibratedYellow();
//    spO=  sensor.getCalibratedOrange();
//    spR = sensor.getCalibratedRed();

//    spV = sensor.getCalibratedViolet();
//    spB = sensor.getBlue();
//    spG = sensor.getGreen();
//    spY = sensor.getCalibratedYellow();
//    
//    spO=  sensor.getCalibratedOrange();
//    spR = sensor.getCalibratedRed();


//   spAll = spV  + spB + spG + spO + spR;
   spAll = spV  + spB + spG;
    
  //  
  //  Serial.print("Green Value: ");
  //  Serial.print(yellow);
    Serial.print(spV);
    Serial.print(spB);
    Serial.print("\n");
  } else{
   float blue  = float(sig)/20;
   float green = 1 - sig1 ;
  }
  
//   blue  = sig1/10;
//   green = 1 - sig1/10 ;

    Serial.print("sig: ");
    Serial.print(sig);
    Serial.print(",");
  
    Serial.print("blue: ");
    Serial.print(spB);
    Serial.print(",");
    Serial.print("green: ");
    Serial.print(spG);
    Serial.print(",");
    Serial.print("\n");


  float temp = getTemp(tempSensorPin)/100;
  // Update LCD
  lcd.clear();
//  float bg_ratio = green/blue ;
  float raG = spG / spAll;

  plot_signal_bar(lcd, raG , 
    0, "G:" );
  plot_signal_bar(lcd, temp, 
    1, "T:" );


  // sensor.printMeasurements();//Prints out all measurements (calibrated)

  // wait for 30 milliseconds to see the dimming effect
  delay(100); 

}
