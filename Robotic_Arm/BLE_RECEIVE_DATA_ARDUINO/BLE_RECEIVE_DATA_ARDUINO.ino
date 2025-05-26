#include <ArduinoBLE.h>

BLEService customService("180C");
BLECharacteristic rxCharacteristic("2A56", BLEWrite, 20);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Failed to start BLE!");
    while (1);
  }

  BLE.setLocalName("ArduinoR4BLE");
  BLE.setAdvertisedService(customService);
  customService.addCharacteristic(rxCharacteristic);
  BLE.addService(customService);

  BLE.advertise();
  Serial.println("BLE service is advertising...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (rxCharacteristic.written()) {
        const uint8_t* data = rxCharacteristic.value();
        int len = rxCharacteristic.valueLength();
        String msg = "";
        for (int i = 0; i < len; i++) {
          msg += (char)data[i];
        }
        Serial.print("Received via BLE: ");
        Serial.println(msg);
        BLE.central();
      }
    }

    Serial.println("Disconnected.");
  }
}
