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

// Poziții sortare + prindere + siguranță
const int positions[5][7] = {
  {100,165,180,140,120,170,73},  // R - 1
  {100,155,180,120,175,170,73},  // G - 2
  {100,140,130,140,180,170,73},  // B - 3
  {100,120,110,170,180,170,73},  // Y - 4
  {100, 95,110,170,180,170,73}   // O - 5
};
const int prepPos[7] = {100, 90,  60,   20, 155, 170,   0};
const int gripPos[7] = {100, 90,  60,   20, 155, 170,  73};
const int safePos[7] = {100,  0,  45, 180,   0,  90,  10};

// Motoare
const int M1_PWM = 12;
const int M1_DIR = 13;
const int M2_PWM = 7;
const int M2_DIR = 8;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Braccio.begin();

  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_DIR, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("BLE nu a pornit!");
    while (1);
  }

  BLE.setLocalName("ArduinoBLESortare");
  BLEService service("180C");
  BLECharacteristic rx("2A56", BLEWrite, 20);
  service.addCharacteristic(rx);
  BLE.setAdvertisedService(service);
  BLE.addService(service);
  BLE.advertise();

  Serial.println("Aștept conexiune BLE...");
  while (!BLE.central()) {}

  Serial.println("Conectat. Pornesc banda...");

  // Motor 2: 0.5 sec
  digitalWrite(M2_DIR, HIGH);
  analogWrite(M2_PWM, 50);
  delay(500);
  analogWrite(M2_PWM, 0);
  digitalWrite(M2_DIR, LOW);



  // Motor 1: 5 sec
  digitalWrite(M1_DIR, HIGH);
  analogWrite(M1_PWM, 255);
  delay(5000);
  analogWrite(M1_PWM, 0);
  digitalWrite(M1_DIR, LOW);

 

  

  // Se caută culoarea
  char culoare = 0;
  BLEDevice central = BLE.central();
  while (culoare == 0 && central.connected()) {
    if (rx.written()) {
      const uint8_t* data = rx.value();
      if (rx.valueLength() > 0) {
        culoare = (char)data[0];
        Serial.print("Culoare detectată: ");
        Serial.println(culoare);
      }
    }
  }

  // După detecție: motor 1 din nou 5 sec
  digitalWrite(M1_DIR, HIGH);
  analogWrite(M1_PWM, 255);
  delay(5000);
  analogWrite(M1_PWM, 0);
  digitalWrite(M1_DIR, LOW);
  delay(500); // timp pentru stabilizar
  Serial.println("Pornesc Braccio...");

  // Apucă și plasează
  Braccio.ServoMovement(prepPos[0], prepPos[1], prepPos[2],
                        prepPos[3], prepPos[4], prepPos[5], prepPos[6]);
  delay(500);
  Braccio.ServoMovement(gripPos[0], gripPos[1], gripPos[2],
                        gripPos[3], gripPos[4], gripPos[5], gripPos[6]);
  delay(500);

  int idx = 4;
  if (culoare == 'R') idx = 0;
  else if (culoare == 'G') idx = 1;
  else if (culoare == 'B') idx = 2;
  else if (culoare == 'Y') idx = 3;

  Braccio.ServoMovement(positions[idx][0], positions[idx][1], positions[idx][2],
                        positions[idx][3], positions[idx][4], positions[idx][5], positions[idx][6]);
  delay(1000);
  Braccio.ServoMovement(positions[idx][0], positions[idx][1], positions[idx][2],
                        positions[idx][3], positions[idx][4], positions[idx][5], 0);
  delay(500);
  Braccio.ServoMovement(safePos[0], safePos[1], safePos[2],
                        safePos[3], safePos[4], safePos[5], safePos[6]);
  delay(500);

  Serial.println("Sortare finalizată.");
}

void loop() {
  // Nu face nimic
}
