


/////// Define array structure

struct specArray{
  float sig;
  float d[50];
  };
 
specArray signals;

void setup_signal(){
  signals.sig=5.;
//  signals = {0.5 , {}};
}

struct specArray getSignals(
  specArray arr
  ){

  /// forward the signals
  float sig = arr.sig;
  sig += 0.5;
  sig = sig - 20 * (int(sig) / 20);    
  arr.sig = sig;


  // Setting the fluorescence in the furture
  float sig1 = min(sig,20-sig); 
  arr.d[0] = sig1/10;
  arr.d[1] = 1. - arr.d[0] ;
  return arr;
}




void spec_print()  {
  //  sensor.takeMeasurements();
  //  int blue = sensor.getCalibratedBlue();
  //  int green = sensor.getCalibratedGreen();
  //  int yellow = sensor.getCalibratedYellow();
  
  // sensor.printMeasurements();//Prints out all measurements (calibrated)

  //  Serial.print("blue: ");
  //  Serial.print(blue);
  //  Serial.print(",");
  //  
  //  Serial.print("Green Value: ");
  //  Serial.print(yellow);
  //  Serial.print("\n");
//  return signals
}




//specArray signals = {10. , {}};


/////////////////////