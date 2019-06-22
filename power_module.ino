#include "math.h"
int offset =3;// set the correction offset value
const int Vsensor_pin=34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  int volt = analogRead(Vsensor_pin);// read the input
if (volt < 2429){
  double voltage_scal = 0.0043*volt+0.4894; //scaled voltage for 0-10.99v
  //Serial.print("Voltage: ");
  Serial.println(voltage_scal);
}

//2429 is the analog voltage at which we will switch over to equation two to display scaled voltage
 else if (volt < 3293 && volt > 2429){
  double voltage_scal = 0.0036*volt+2.1748; //scaled voltage for 11-13.99v
  //Serial.print("Voltage: ");
  Serial.println(voltage_scal);
 }
   //3293 is the analog voltage reading at which we will switch over to equation three to display scaled voltage
 else {
  double voltage_scal = 0.0023*volt+6.3361; // scaled voltage for 14-15v
    //Serial.print("Voltage: ");
  Serial.println(voltage_scal);
 }
  
  
  delay(1000);
  
  
}
