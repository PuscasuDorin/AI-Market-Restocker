#include <Servo.h>
#include <Braccio.h>
#include <ArduinoBLE.h>

// Servo-uri Braccio
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

// Motoare bandă
const int M1_PWM = 12;
const int M1_DIR = 13;
const int M2_PWM = 7;
const int M2_DIR = 8;

BLEService service("180C");
BLECharacteristic rx("2A56", BLEWrite, 20);

char culoare = 0;

void asteaptaCuloare() {
  BLEDevice central = BLE.central();
  while (culoare == 0 && central.connected()) {
    if (rx.written()) {
      const uint8_t* data = rx.value();
      if (rx.valueLength() > 0) {
        culoare = (char)data[0];
        Serial.print("Culoare de la BLE: ");
        Serial.println(culoare);
      }
    }
    if (Serial.available()) {
      culoare = Serial.read();
      Serial.print("Culoare din Serial: ");
      Serial.println(culoare);
    }
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_DIR, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("BLE nu a pornit!");
    while (1);
  }

  BLE.setLocalName("ArduinoBLESortare");
  service.addCharacteristic(rx);
  BLE.setAdvertisedService(service);
  BLE.addService(service);
  BLE.advertise();

  Serial.println("Aștept conexiune BLE...");
  while (!BLE.central()) {
    if (Serial.available()) {
      char test = Serial.read();
      if (test == 'Z') {
        Serial.println("Test cu Z - se continuă fără BLE.");
        break;
      }
    }
  }

  Serial.println("Conectat. Pornesc banda...");

  // Motor 2
  digitalWrite(M2_DIR, HIGH);
  analogWrite(M2_PWM, 50);
  delay(500);
  analogWrite(M2_PWM, 0);
  digitalWrite(M2_DIR, LOW);

  // Motor 1
  digitalWrite(M1_DIR, HIGH);
  analogWrite(M1_PWM, 255);
  delay(2000);
  analogWrite(M1_PWM, 0);
  digitalWrite(M1_DIR, LOW);

  Serial.println("Aștept litera (R, G, B, Y, O) prin BLE sau Serial:");
  asteaptaCuloare();  // Aici se detectează culoarea
  delay(5000);
  // După detectare: motorul 1 continuă 1 sec
  digitalWrite(M1_DIR, HIGH);
  analogWrite(M1_PWM, 255);
  delay(2500);
  analogWrite(M1_PWM, 0);
  digitalWrite(M1_DIR, LOW);

  // Abia acum intrăm în Braccio
  delay(1000);
  Serial.println("=== INTRU IN BRACCIO ===");

  Braccio.begin();
  delay(500);

  // Pregătire (se duce la banda)
  Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 0);
  delay(1000);
  // Prindere (prinde cubu)
  Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 73);
  delay(1000);

  // Poziționare în funcție de culoare
  if (culoare == 'R') {
    Braccio.ServoMovement(100, 165, 180, 140, 120, 170, 73);
    delay(1000);
    Braccio.ServoMovement(100, 165, 180, 140, 120, 170, 0);
  } else if (culoare == 'G') {
    Braccio.ServoMovement(100, 155, 180, 120, 175, 170, 73);
    delay(1000);
    Braccio.ServoMovement(100, 155, 180, 120, 175, 170, 0);
  } else if (culoare == 'B') {
    Braccio.ServoMovement(100, 140, 130, 140, 180, 170, 73);
    delay(1000);
    Braccio.ServoMovement(100, 140, 130, 140, 180, 170, 0);
  } else if (culoare == 'Y') {
    Braccio.ServoMovement(100, 120, 110, 170, 180, 170, 73);
    delay(1000);
    Braccio.ServoMovement(100, 120, 110, 170, 180, 170, 0);
  } else {
    Braccio.ServoMovement(100, 95, 110, 170, 180, 170, 73);
    delay(1000);
    Braccio.ServoMovement(100, 95, 110, 170, 180, 170, 0);
  }

  delay(1000);
  //Merge in locatia safe
  Braccio.ServoMovement(100, 0, 45, 180, 0, 90, 10); 
  delay(1000);

  Serial.println("Sortare finalizată.");
}

void loop() {}
