#include "math.h"


//Variables

//Battery Capacity in Amp Hours (Ah)
double batteryCap = 8.4;

//PINS

//Voltage sensor
const int Vsensor_pin=34;
//Current sensor
const int Csensor_pin=35;


//FUNCTIONS

double getVoltage(int pin){

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

double getCurrent(int pin){
  //Code here
}

bool isBatteryFull(){
  //Code here
}

double getBatteryLevel(double capacity){
  //Run isBatteryFull 
  if (isBatteryFull() == TRUE){

  } else {
    
  }
}

double getEnergyConsumed(double current, double delay){
  //delay
  delay(delay);
  //get current
  double current = getCurrent();
  //multiply delay by amps to get amp seconds, then convert to milliamp hours
  double energy = current *  delay * 0.2777;
  return energy;
}


void setup() {
  Serial.begin(115200);
}

void loop() {
  //SHOW VOLTAGE, CURRENT READINGS
  double voltage = getVoltage(Vsensor_pin);
  double current = getCurrent(Csensor_pin);
  Serial.println("Voltage: ");
  Serial.print(voltage);
  Serial.print("V, ");
  Serial.print("Current: ")
  Serial.print(current);
  Serial.print("A");

  //POWER CONSUMPTION
  double energy = getEnergyConsumed(current, 1000);
  totalEnergyConsumed += energy;
  Serial.println("");
  Serial.print("Total Energy Consumed: ");
  Serial.print(totalEnergyConsumed);
  Serial.print("mAh");
  
}

