#include <SPI.h>
#include <BLEPeripheral.h>

#define POT_PIN A0

#define BLE_REQ 10
#define BLE_RDY 2
#define BLE_RST 9

int currentPotReading;
int lastPotReading;

//int potHigh;
//int potLow;

BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);
BLEService slideSender = BLEService("FF10");

BLEUnsignedIntCharacteristic potCharacteristic = BLEUnsignedIntCharacteristic("FF11", BLERead|BLENotify );
BLEDescriptor potDescriptor = BLEDescriptor("2901", "Slider Value");

void setup() {
  Serial.begin(9600);

  blePeripheral.setLocalName("Swing Sender");
  blePeripheral.setDeviceName("Swings Sender");
  blePeripheral.setAdvertisedServiceUuid(slideSender.uuid());

  blePeripheral.addAttribute(slideSender);
  blePeripheral.addAttribute(potCharacteristic);
  blePeripheral.addAttribute(potDescriptor);

  blePeripheral.begin();

  potCharacteristic.setValue(analogRead(POT_PIN));

}

void loop() {
  blePeripheral.poll();
  
  currentPotReading = analogRead(POT_PIN);
  if (abs(currentPotReading - lastPotReading) >= 5) {
    Serial.println("I have changed");
    Serial.println(currentPotReading);
    potCharacteristic.setValue(currentPotReading);
  }

  lastPotReading = currentPotReading;
  delay(2);
}
