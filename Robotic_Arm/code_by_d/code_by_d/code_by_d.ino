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
int done = 0;

const int M1_PIN_DIR = 7;
//const int M1_PIN_PWM = 4;
const int M2_PIN_DIR = 4;
//const int M2_PIN_PWM = 8;
//const int M3_PIN_PWM = 13;
const int M3_PIN_DIR = 2;

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

  pinMode(M1_PIN_DIR, OUTPUT);
  //pinMode(M1_PIN_PWM, OUTPUT);
  pinMode(M2_PIN_DIR, OUTPUT);
  //pinMode(M2_PIN_PWM, OUTPUT);
  pinMode(M3_PIN_DIR, OUTPUT);
  //pinMode(M3_PIN_PWM, OUTPUT);
}

void loop() {
  BLEDevice central = BLE.central();

    if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

        //Run MOT2
   // analogWrite(M2_PIN_PWM, 50);  // viteză ~50%
    digitalWrite(M2_PIN_DIR, HIGH);
    delay(500);
    //Stop MOT2
    digitalWrite(M2_PIN_DIR, LOW);
    delay(100);

    //Run MOT1
    //analogWrite(M1_PIN_PWM, 100);  // viteză 100%
    digitalWrite(M1_PIN_DIR, HIGH);
    delay(10000); 

    while (central.connected()) {
      if(done == 1){
        if(motor_sel == false) {
        //Run MOT2
        //analogWrite(M2_PIN_PWM, 50);  // viteză ~50%
        digitalWrite(M2_PIN_DIR, HIGH);
        delay(500);
        //Stop MOT2
        digitalWrite(M2_PIN_DIR, LOW);
        delay(100);
        }
        if(motor_sel == true ) {
          //Run MOT3
          //analogWrite(M3_PIN_PWM, 50);  // viteză ~50%
          digitalWrite(M3_PIN_DIR, HIGH);
          delay(500); 
          //Stop MOT3
          digitalWrite(M3_PIN_DIR, LOW);
          delay(100);
          motor_sel = !motor_sel;
        }
        //Run MOT1
        //analogWrite(M1_PIN_PWM, 100);  // viteză 100%
        digitalWrite(M1_PIN_DIR, HIGH);
        delay(10000); 
        done = 0;
      }
       

      if (rxCharacteristic.written()) {
        //Stop MOT1
        digitalWrite(M1_PIN_DIR, LOW);
        delay(100);
        
        const uint8_t* data = rxCharacteristic.value();
        int len = rxCharacteristic.valueLength();
        char message = ' ';
        message = (char)data[0];
        Serial.print("Received via BLE: ");
        Serial.println(message);
        culoare = message;
        if (culoare == 'R') {
          // Pregătire (se duce la banda)
          Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 0);
          delay(1000);
          // Prindere (prinde cubu)
          Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 73);
          delay(1000);
          // Merge la Locatie
          Braccio.ServoMovement(100, 165, 180, 140, 120, 170, 73);
          delay(1000);
          // Da drumu la clesti
          Braccio.ServoMovement(100, 165, 180, 140, 120, 170, 0);
          delay(1000);
          //Merge in locatia safe
          Braccio.ServoMovement(100, 0, 45, 180, 0, 90, 10); 
          delay(1000);
          done = 1;
        } else if (culoare == 'G') {
            // Pregătire (se duce la banda)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 0);
            delay(1000);
            // Prindere (prinde cubu)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 73);
            delay(1000);
            // Merge la Locatie
            Braccio.ServoMovement(100, 155, 180, 120, 175, 170, 73);
            delay(1000);
            Braccio.ServoMovement(100, 155, 180, 120, 175, 170, 0);
            delay(1000);
            //Merge in locatia safe
            Braccio.ServoMovement(100, 0, 45, 180, 0, 90, 10); 
            delay(1000);
            done = 1;
        } else if (culoare == 'B') {
            // Pregătire (se duce la banda)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 0);
            delay(1000);
            // Prindere (prinde cubu)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 73);
            delay(1000);
            // Merge la Locatie
            Braccio.ServoMovement(100, 140, 130, 140, 180, 170, 73);
            delay(1000);
            // Da drumu la clesti
            Braccio.ServoMovement(100, 140, 130, 140, 180, 170, 0);
            delay(1000);
            //Merge in locatia safe
            Braccio.ServoMovement(100, 0, 45, 180, 0, 90, 10); 
            delay(1000);
            done = 1;
        } else if (culoare == 'Y') {
            // Pregătire (se duce la banda)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 0);
            delay(1000);
            // Prindere (prinde cubu)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 73);
            delay(1000);
            // Merge la Locatie
            Braccio.ServoMovement(100, 120, 110, 170, 180, 170, 73);
            delay(1000);
            // Da drumu la clesti
            Braccio.ServoMovement(100, 120, 110, 170, 180, 170, 0);
            delay(1000);
            //Merge in locatia safe
            Braccio.ServoMovement(100, 0, 45, 180, 0, 90, 10); 
            delay(1000);
            done = 1;
        } else if (culoare == 'O') {
            // Pregătire (se duce la banda)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 0);
            delay(1000);
            // Prindere (prinde cubu)
            Braccio.ServoMovement(100, 70, 45, 10, 130, 170, 73);
            delay(1000);
            // Merge la Locatie
            Braccio.ServoMovement(100, 95, 110, 170, 180, 170, 73);
            delay(1000);
            // Da drumu la clesti
            Braccio.ServoMovement(100, 95, 110, 170, 180, 170, 0);
            delay(1000);
            //Merge in locatia safe
            Braccio.ServoMovement(100, 0, 45, 180, 0, 90, 10); 
            delay(1000);
            done = 1;
        }
        Serial.print(done);
        BLE.central();
      }
    }

    Serial.println("Disconnected.");
  }
}
