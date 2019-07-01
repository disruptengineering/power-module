#include "math.h"


//Variables

//Battery Capacity in Amp Hours (Ah)
const double BatteryCap=8.4


//PINS

//Voltage sensor
const int Vsensor_pin=34;
//Current sensor


void setup() {
  Serial.begin(115200);
}

void loop() {
  
  voltage = getVoltage(Vsensor_pin);
  Serial.println(voltage);
  delay(1000);
  
}

//FUNCTIONS

double getVoltage(pin){

  int volt = analogRead(pin);// read the input

  if (volt < 2429){
    double voltage_scal = 0.0043*volt+0.4894; //scaled voltage for 0-10.99v
    return voltage_scal;
  }

  //2429 is the analog voltage at which we will switch over to equation two to display scaled voltage
  else if (volt < 3293 && volt > 2429){
    double voltage_scal = 0.0036*volt+2.1748; //scaled voltage for 11-13.99v
    return voltage_scal
  }

  //3293 is the analog voltage reading at which we will switch over to equation three to display scaled voltage
  else {
    double voltage_scal = 0.0023*volt+6.3361; // scaled voltage for 14-15v
    return voltage_scal
  }
  
}

double getCurrent(){
  //Code here
}

bool isBatteryFull(){
  //Code here
}

double getBatteryLevel(capacity){
  //Run isBatteryFull 
  if (isBatteryFull() == TRUE){

  } else {
    
  }
}