#include <ArduinoBLE.h>
#include <Servo.h>
#include <Wire.h>
#include <Braccio.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

BLEService customService("180C");
BLECharacteristic rxCharacteristic("2A56", BLEWrite, 20);

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

char color = ' ';
bool motor_sel = true;
int done = 1;
int s=0;
int m_reverse = 0;

const int M1_PIN_DIR = 7;
const int M2_PIN_DIR = 4;
const int M3_PIN_DIR = 2;
const int M2_PIN_REV_DIR = 8;
const int M3_PIN_REV_DIR = 13;
const int trigPin = A0;
const int echoPin = A1;
const int redLED = A3;
const int greenLED = A4;
float duration, distance;
bool motor_set = true;
const int startButton = A5;
int startButtonState = 0;

void setup() {
  pinMode(startButton, INPUT);
  startButtonState = digitalRead(startButton);

  if(startButtonState == LOW){
    Serial.begin(9600);
    while (!Serial);

    if (!BLE.begin()) {
      Serial.println("Failed to start BLE!");
      while (1);
    }
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("OLED screen was not detected."));
      while (true); // Stop execution
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
  else{
  setup();
  }
}

void loop() {
  BLEDevice central = BLE.central();

    if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());


    while (central.connected()) {
      
      if(motor_set == true && s == 6){
        pinMode(M1_PIN_DIR, OUTPUT);
        pinMode(M2_PIN_DIR, OUTPUT);
        pinMode(M3_PIN_DIR, OUTPUT); 
        pinMode(M2_PIN_REV_DIR, OUTPUT);
        pinMode(M3_PIN_REV_DIR, OUTPUT); 
        pinMode(redLED, OUTPUT);
        pinMode(greenLED, OUTPUT);

        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        motor_set = false;
      }
      if(done == 1 && motor_set == false){
        if(m_reverse == 4){
          analogWrite(M2_PIN_REV_DIR, 100);
          delay(500);
          analogWrite(M2_PIN_REV_DIR, 0);
          delay(100);
          analogWrite(M3_PIN_REV_DIR, 100);
          delay(500);
          analogWrite(M3_PIN_REV_DIR, 0);
          delay(100);
          m_reverse = 0;
          //delay(5000);
          startButtonState = digitalRead(startButton);
          while(startButtonState == HIGH){
            startButtonState = digitalRead(startButton);
            BLE.poll();
            delay(500);
          }
        }
        if(motor_sel == false) {
        BLE.poll();
        //Run MOT2
        digitalWrite(redLED, HIGH);
        analogWrite(M2_PIN_DIR, 50);
        delay(600);
        //Stop MOT2
        analogWrite(M2_PIN_DIR, 0);
        delay(100);
        digitalWrite(redLED, LOW);
        m_reverse++;
        }
        if(motor_sel == true ) {
          BLE.poll();
          //Run MOT3
          digitalWrite(redLED, HIGH);
          analogWrite(M3_PIN_DIR, 50);
          delay(600); 
          //Stop MOT3
          analogWrite(M3_PIN_DIR, 0);
          delay(100);
          digitalWrite(redLED, LOW);
          m_reverse++;
        }
        //Run MOT1
        digitalWrite(redLED, HIGH);
        digitalWrite(M1_PIN_DIR, HIGH);
        delay(5700); 
        done = 0;
        //Stop MOT1
        digitalWrite(M1_PIN_DIR, LOW);
        delay(100);
        digitalWrite(redLED, LOW);
        BLE.poll();
      }
       
      if (rxCharacteristic.written()) {
        
        const uint8_t* data = rxCharacteristic.value();
        int len = rxCharacteristic.valueLength();
        char message = ' ';
        message = (char)data[0];
        Serial.print("Received via BLE: ");
        Serial.println(message);
        color = message;
        
      if (color == 'R') {
        digitalWrite(greenLED, HIGH);
        display.clearDisplay();               
        display.setTextSize(2);               
        display.setTextColor(SSD1306_WHITE);  
        display.setCursor(0, 0);              
        display.println("RED");          
        display.display();   
        do{ BLE.poll();
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10); //CATCH
        delay(100);
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10);
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
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);
        
        Braccio.ServoMovement(40, 130, 140, 100, 160, 170, 70); //color 1
        delay(100);
        do{
        Braccio.ServoMovement(40, 130, 140, 100, 160, 170, 70); //color 1
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while(distance<14.8 || distance>17);
        delay(500);
        Braccio.ServoMovement(40, 130, 140, 100, 160, 170, 10);;
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
        digitalWrite(greenLED, LOW);
      } else if (color == 'G') {
        digitalWrite(greenLED, HIGH);
        display.clearDisplay();               
        display.setTextSize(2);               
        display.setTextColor(SSD1306_WHITE);  
        display.setCursor(0, 0);              
        display.println("GREEN");          
        display.display(); 
        do{ BLE.poll();
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10); //catch
        delay(100);
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10);
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
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);
        
        Braccio.ServoMovement(40, 110, 120, 120, 175, 170, 70); //color 2
        delay(100);
        do{
        Braccio.ServoMovement(40, 110, 120, 120, 175, 170, 70); //color 2
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<12.6 || distance>14.7);
        delay(500);
        Braccio.ServoMovement(40, 110, 120, 120, 175, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
        digitalWrite(greenLED, LOW);
      } else if (color == 'B') {
        digitalWrite(greenLED, HIGH);
        display.clearDisplay();               
        display.setTextSize(2);               
        display.setTextColor(SSD1306_WHITE);  
        display.setCursor(0, 0);              
        display.println("BLUE");          
        display.display(); 
        do{ BLE.poll();
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10); //catch
        delay(100);
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10);
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
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);

        Braccio.ServoMovement(40, 85, 120, 120, 180, 170, 70); //color 3
        delay(100);
        do{
        Braccio.ServoMovement(40, 85, 120, 120, 180, 170, 70); //color 3
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while(distance<10.5 || distance>12.5);
        delay(500);
        Braccio.ServoMovement(40, 85, 120, 120, 180, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
        digitalWrite(greenLED, LOW);
      } else if (color == 'Y') {
        digitalWrite(greenLED, HIGH);
        display.clearDisplay();               
        display.setTextSize(2);               
        display.setTextColor(SSD1306_WHITE);  
        display.setCursor(0, 0);              
        display.println("YELLOW");          
        display.display(); 
        do{ BLE.poll();
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10); //catch
        delay(100);
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10);
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
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);


        Braccio.ServoMovement(40, 60, 120, 120, 175, 170, 70); //color 4
        delay(100);
        do{
        Braccio.ServoMovement(40, 60, 120, 120, 175, 170, 70); //color 4
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<8 || distance>9.95);
        delay(500);
        Braccio.ServoMovement(40, 60, 120, 120, 175, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
        digitalWrite(greenLED, LOW);
      } else if (color == 'O') {
        digitalWrite(greenLED, HIGH);
        display.clearDisplay();               
        display.setTextSize(2);               
        display.setTextColor(SSD1306_WHITE);  
        display.setCursor(0, 0);              
        display.println("ORANGE");          
        display.display(); 
        do{ BLE.poll();
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10); //catch
        delay(100);
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 10);
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
        Braccio.ServoMovement(40, 80, 45, 0, 145, 170, 70);
        delay(1000);
        Braccio.ServoMovement(40, 60, 95, 60, 105, 170, 70);
        delay(500);

        Braccio.ServoMovement(40, 40, 140, 110, 150, 170, 70); //color 5
        delay(100);
        do{
        Braccio.ServoMovement(40, 40, 140, 110, 150, 170, 70); //color 5
        delay(2000);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;
        Serial.println(distance);
        } while (distance<6.7 || distance>7.8);
        delay(500);
        Braccio.ServoMovement(40, 40, 140, 110, 150, 170, 10);
        delay(500);
        Braccio.ServoMovement(40, 0, 45, 180, 0, 90, 10);
        delay(500);
        done = 1;
        BLE.poll();
        digitalWrite(greenLED, LOW);
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
