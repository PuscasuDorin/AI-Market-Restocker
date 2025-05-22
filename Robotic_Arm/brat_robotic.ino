#include <Servo.h>
// DEFINIȚII servo-uri pentru Braccio (linker necesită aceste simboluri)
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Braccio.h>


// cele 8 poziții de sortare: {speed, base, shoulder, elbow, wristV, wristR, gripper}
const int positions[8][7] = {
  {100,165,180,140,120,170,73},
  {100,155,180,120,175,170,73},
  {100,140,130,140,180,170,73},
  {100,120,110,170,180,170,73},
  {100, 95,110,170,180,170,73},
  {100, 80,130,140,180,170,73},
  {100, 65,180,120,165,170,73},
  {100, 55,180,140,120,170,73}
};

// pozițiile fixe: pregătire-prindere, preindere, siguranță
const int prepPos[7] = {100, 90,  60,   20, 155, 170,   0};
const int gripPos[7] = {100, 90,  60,   20, 155, 170,   73};
const int safePos[7] = {100,  0,  45, 180,   0,  90,  10};

int selected = 0;
unsigned long lastPress = 0;
const unsigned long debounce = 200;

void setup() {
  // intrări butoane
  pinMode(UP_PIN,    INPUT_PULLUP);
  pinMode(DOWN_PIN,  INPUT_PULLUP);
  pinMode(LEFT_PIN,  INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(SET_PIN,   INPUT_PULLUP);
  pinMode(RST_PIN,   INPUT_PULLUP);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;) ;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);

  // Braccio
  Braccio.begin();
  delay(500);

  showSelection();
}

void loop() {
  unsigned long now = millis();

  // navigare ↑
  if (digitalRead(UP_PIN)==LOW && now - lastPress > debounce) {
    selected = (selected + 1) % 8;
    lastPress = now;
    showSelection();
    while (digitalRead(UP_PIN)==LOW);
  }
  // navigare ↓
  if (digitalRead(DOWN_PIN)==LOW && now - lastPress > debounce) {
    selected = (selected + 7) % 8;
    lastPress = now;
    showSelection();
    while (digitalRead(DOWN_PIN)==LOW);
  }
  // reset la forma 1
  if (digitalRead(RST_PIN)==LOW && now - lastPress > debounce) {
    selected = 0;
    lastPress = now;
    showSelection();
    while (digitalRead(RST_PIN)==LOW);
  }
  // confirmare
  if (digitalRead(SET_PIN)==LOW && now - lastPress > debounce) {
    lastPress = now;
    while (digitalRead(SET_PIN)==LOW);
    runPickAndPlace(selected);
    showSelection();
  }
}

// afișează pe OLED „Forma X”
void showSelection() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Selecteaza forma:");
  display.setTextSize(3);
  display.setCursor(30, 20);
  display.print(selected + 1);
  display.display();
}

// secvența de pick & place: prep → grip → sort → eliberează → safe
void runPickAndPlace(int idx) {
  // 1) pregătire
  Braccio.ServoMovement(prepPos[0], prepPos[1], prepPos[2],
                       prepPos[3], prepPos[4], prepPos[5], prepPos[6]);
  delay(500);

  // 2) poziție prindere
  Braccio.ServoMovement(gripPos[0], gripPos[1], gripPos[2],
                       gripPos[3], gripPos[4], gripPos[5], gripPos[6]);
  delay(500);

  // 3) deplasare la poziția selectată
  Braccio.ServoMovement( positions[idx][0],
                        positions[idx][1],
                        positions[idx][2],
                        positions[idx][3],
                        positions[idx][4],
                        positions[idx][5],
                        positions[idx][6] );
  delay(1000);

  // 4) eliberare (gripper = 0)
  Braccio.ServoMovement( positions[idx][0],
                        positions[idx][1],
                        positions[idx][2],
                        positions[idx][3],
                        positions[idx][4],
                        positions[idx][5],
                        0 );
  delay(500);

  // 5) întoarcere în poziția de siguranță
  Braccio.ServoMovement(safePos[0], safePos[1], safePos[2],
                       safePos[3], safePos[4], safePos[5], safePos[6]);
  delay(500);
}



