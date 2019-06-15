
int offset =20;// set the correction offset value
const int Vsensor_pin=34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  int volt = analogRead(Vsensor_pin);// read the input

  //double voltage = Vsensor_pin/4095;
  
  //double voltage = map(volt,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  //voltage /=100;// divide by 100 to get the decimal values
  //Serial.print("Voltage: ");
  //Serial.print(voltage);//print the voltge
  //Serial.println("V");

  Serial.println(volt);
  


delay(500);
  
  
}
