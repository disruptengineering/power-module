
int offset =3;// set the correction offset value
const int Vsensor_pin=34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  int volt = analogRead(Vsensor_pin);// read the input
  double voltage_scal = 0.0036*volt+2.1748;
  //double voltage = Vsensor_pin/4095;
  
  //double voltage_out = map(volt,0,4095, 0, 24); //- offset;// map 0-1023 to 0-2500 and add correction offset
  //voltage /=100;// divide by 100 to get the decimal values
  //Serial.print("Voltage: ");
  //Serial.print(voltage);//print the voltge
  //Serial.println("V");

  Serial.println(voltage_scal);
  


delay(500);
  
  
}
