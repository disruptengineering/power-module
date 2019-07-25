#include "math.h"


//Variables
double totalEnergyConsumed = 0;


//Conversion Values
const float vpp = 0.0012210012210012; 

//Battery Capacity in Amp Hours (Ah)
double batteryCap = 8.4;

//PINS

//Voltage sensor
const int Vsensor_pin=34;
//Current sensor
const int Csensor_pin=36;


void setup() {
  Serial.begin(115200);
}

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
    return voltage_scal;
  }

  //3293 is the analog voltage reading at which we will switch over to equation three to display scaled voltage
  else {
    double voltage_scal = 0.0023*volt+6.3361; // scaled voltage for 14-15v
    return voltage_scal;
  }
  
}

double getCurrent(int pin){
  double currentV = analogRead(pin);    // read the analog current input
  
  currentV = currentV - 1750; // Subtract offset for 1st current sensor (best one so far)
  //V = V - 1775; // Subtract offset for 3rd current sensor
  //V = V - 1768; // Subtract offset for 4th current sensor

  double voltage_Csensor = currentV * vpp;
  double current_convert = voltage_Csensor/0.100;
        
  if (current_convert < 2){
    double current_correct = current_convert - 0.15;
    return current_correct;
  }
  else if (current_convert >= 1.1 && current_convert <= 2.5){
    return current_convert;
  }
  else {
    double current_correct = current_convert + 0.2;
    return current_correct;
  }
}


double getEnergyConsumed(double delay_variable){
  //delay
  delay(delay_variable);
  //get current
  double current = getCurrent(Csensor_pin);
  //multiply delay by amps to get amp seconds, then convert to milliamp hours
  double energy = current *  delay_variable * 0.2777 / 1000;
  //1000 converts delay ms to s
  return energy;
}


void loop() {
  //SHOW VOLTAGE, CURRENT READINGS
  double voltage = getVoltage(Vsensor_pin);
  double current = getCurrent(Csensor_pin);
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print("V, ");
  Serial.print("Current: ");
  Serial.print(current);
  Serial.print("A");

  //POWER CONSUMPTION
  double energy = getEnergyConsumed(1000);
  totalEnergyConsumed += energy;
  Serial.println("");
  Serial.print("Total Energy Consumed: ");
  Serial.print(totalEnergyConsumed);
  Serial.print("mAh");
  Serial.println("");

  //BATTERY LEVEL
  double energyPercent = 100 - (totalEnergyConsumed/(10*batteryCap));
  Serial.println("");
  Serial.print("Estimated Battery Level: ");
  Serial.print(energyPercent);
  Serial.print("%");
  Serial.println("");
  Serial.println("---------------");
  
  //BLUETOOTH

}
