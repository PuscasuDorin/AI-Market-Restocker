#include <ArduinoBLE.h>
#include <Servo.h>
#include <Wire.h>
#include <Braccio.h>

BLEService customService("180C");
BLECharacteristic rxCharacteristic("2A56", BLEWrite, 20);

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

char culoare = ' ';
bool motor_sel = true;
int done = 1;
int s=0;

const int M1_PIN_DIR = 7;
const int M2_PIN_DIR = 4;
const int M3_PIN_DIR = 2;
const int trigPin = A0;
const int echoPin = A1;
float duration, distance;
bool motor_set = true;

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

  Braccio.begin();
  delay(500);

}

void loop() {
  BLEDevice central = BLE.central();

    if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());


    //delay(10000);

    while (central.connected()) {
      
      if(motor_set == true && s == 6){
        pinMode(M1_PIN_DIR, OUTPUT);
        pinMode(M2_PIN_DIR, OUTPUT);
        pinMode(M3_PIN_DIR, OUTPUT); 
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        motor_set = false;
      }
      if(done == 1 && motor_set == false){
        if(motor_sel == false) {
        //Run MOT2
        analogWrite(M2_PIN_DIR, 50);
        delay(1000);
        //Stop MOT2
        analogWrite(M2_PIN_DIR, 0);
        delay(100);
        }
        if(motor_sel == true ) {
          //Run MOT3
          analogWrite(M3_PIN_DIR, 50);
          delay(1000); 
          //Stop MOT3
          analogWrite(M3_PIN_DIR, 0);
          delay(100);
        }
        //Run MOT1
        digitalWrite(M1_PIN_DIR, HIGH);
        delay(5700); 
        done = 0;
        //Stop MOT1
        digitalWrite(M1_PIN_DIR, LOW);
        delay(100);
      }
       
      if (rxCharacteristic.written()) {
        
        const uint8_t* data = rxCharacteristic.value();
        int len = rxCharacteristic.valueLength();
        char message = ' ';
        message = (char)data[0];
        Serial.print("Received via BLE: ");
        Serial.println(message);
        culoare = message;
        
      if (culoare == 'R') {
        do{
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 10);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while(distance<10 || distance>11);
        delay(1000);
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);

        do{
        Braccio.ServoMovement(40, 125, 140, 140, 100, 170, 70);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while(distance<15.8 || distance>17);
        delay(500);
        Braccio.ServoMovement(40, 125, 140, 140, 100, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
      } else if (culoare == 'G') {
        do{
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 10);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while(distance<10 || distance>11);
        delay(1000);
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);
        do{
        Braccio.ServoMovement(40, 110, 140, 120, 175, 170, 70);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<14 || distance>15.5);
        delay(500);
        Braccio.ServoMovement(40, 110, 140, 120, 175, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
      } else if (culoare == 'B') {
        do{
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 0);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<10 || distance>11);
        delay(1000);
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);
        do{
        Braccio.ServoMovement(40, 90, 130, 140, 180, 170, 70);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while(distance<11 || distance>12.2);
        delay(500);
        Braccio.ServoMovement(40, 90, 130, 140, 180, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
      } else if (culoare == 'Y') {
        do{
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 10);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<10 || distance>11);
        delay(1000);
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);
        do{
        Braccio.ServoMovement(40, 70, 140, 120, 175, 170, 70);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<8.7 || distance>9.5);
        delay(500);
        Braccio.ServoMovement(40, 70, 140, 120, 175, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
      } else if (culoare == 'O') {
        do{
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 10);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<10 || distance>11);
        delay(1000);
        Braccio.ServoMovement(40, 60, 45, 10, 125, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);
        do{
        Braccio.ServoMovement(40, 30, 140, 120, 165, 170, 70);
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<6.5 || distance>8);
        delay(500);
        Braccio.ServoMovement(40, 30, 140, 120, 165, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
    }

        motor_sel = !motor_sel;
        BLE.central();
      }
        if(s<6){
        s++;
        Serial.print(s); 
        delay(3000);
      }
      
    }

    Serial.println("Disconnected.");
  }
}
