#include "math.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//BLE characteristics
BLECharacteristic *voltCharacteristic;
BLECharacteristic *currentCharacteristic;
BLECharacteristic *levelCharacteristic;

bool deviceConnected = false;

//Defining the service & characteristics
#define SERVICE_UUID "414D1C55-BF2C-4225-A229-548C5FD08B1C" 
#define CHARACTERISTIC_UUID_VOLTAGE "942CA185-47CF-47B7-91A0-C8493374D059"
#define CHARACTERISTIC_UUID_CURRENT "D8117FD9-FB40-43E8-A1D9-D75934CDAE36"
#define CHARACTERISTIC_UUID_LEVEL "970F94F1-14E5-421F-8870-A0E1E6F32A09"


//Class for servercallbacks (Wether the device is connected or not)
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


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
  Serial.begin(9600);
  // Create and name the BLE Device
  BLEDevice::init("Power Module");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  voltCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_VOLTAGE,
                      BLECharacteristic::PROPERTY_READ
                    );
  // Create a BLE Characteristic
  currentCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_CURRENT,
                      BLECharacteristic::PROPERTY_READ
                    );
 // Create a BLE Characteristic
  levelCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_LEVEL,
                      BLECharacteristic::PROPERTY_READ
                    );
                    
  //BLE2902 needed to notify
  voltCharacteristic->addDescriptor(new BLE2902());
  currentCharacteristic->addDescriptor(new BLE2902());
  levelCharacteristic->addDescriptor(new BLE2902());
  // Start the service
  pService->start();

  // Start advertising (showing your ble name to connect to)
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
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
  int16_t voltageScaled = (voltage * 100);
  int16_t currentScaled = (current * 100);

  Serial.print("Voltage scaled: ");
  Serial.print(voltageScaled);
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
  if (deviceConnected) {
    /* Set the value */
    char volts[8];
    char ble_current[8];
    char ble_level[8];
    dtostrf(voltageScaled, 1, 2, volts);
    dtostrf(currentScaled, 1, 2, ble_current);
    dtostrf(energyPercent, 1, 2, ble_level);
    voltCharacteristic->setValue(volts);  
    currentCharacteristic->setValue(ble_current);  
    levelCharacteristic->setValue(ble_level);  
  }

}
