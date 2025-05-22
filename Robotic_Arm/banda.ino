// Control secvențial “ineficient” al trei motoare LEGO pe Arduino
// Toate motoarele în CCW, viteze și durate ușor de modificat

// Motor 1 (pini: 2 digital, 3 PWM)
// Motor 2 (pini: 4 digital, 5 PWM)
// Motor 3 (pini: 10 PWM, 11 digital)

const int M1_PIN_DIR = 3;
const int M1_PIN_PWM = 2;
const int M2_PIN_DIR = 5;
const int M2_PIN_PWM = 4;
const int M3_PIN_PWM = 10;
const int M3_PIN_DIR = 11;

void setup() {
  // Configurare pini ca OUTPUT
  pinMode(M1_PIN_DIR, OUTPUT);
  pinMode(M1_PIN_PWM, OUTPUT);
  pinMode(M2_PIN_DIR, OUTPUT);
  pinMode(M2_PIN_PWM, OUTPUT);
  pinMode(M3_PIN_DIR, OUTPUT);
  pinMode(M3_PIN_PWM, OUTPUT);

  // Repetă de 3 ori
  for (int cycle = 0; cycle < 3; cycle++) {
    // --- Secvența pas 1: Motorul 2 ---
    // Direcție CCW: PWM pe M2_PIN_PWM, DIR LOW pe M2_PIN_DIR
    analogWrite(M2_PIN_PWM, 50);  // viteză ~50%
    digitalWrite(M2_PIN_DIR, LOW);
    delay(500);                     // durată 0.5 s
    // Oprire Motor 2
    analogWrite(M2_PIN_PWM, 0);
    delay(100);

    // --- Secvența pas 2: Motorul 1 ---
    // Direcție CCW: PWM pe M1_PIN_PWM, DIR LOW pe M1_PIN_DIR
    analogWrite(M1_PIN_PWM, 100);  // viteză 100%
    digitalWrite(M1_PIN_DIR, LOW);
    delay(10000);                    // durată 2 s
    // Oprire Motor 1
    analogWrite(M1_PIN_PWM, 0);
    delay(100);

    // --- Secvența pas 3: Motorul 3 ---
    // Direcție CCW: PWM pe M3_PIN_PWM, DIR LOW pe M3_PIN_DIR
    analogWrite(M3_PIN_PWM, 50);  // viteză ~50%
    digitalWrite(M3_PIN_DIR, LOW);
    delay(600);                     // durată 0.5 s
    // Oprire Motor 3
    analogWrite(M3_PIN_PWM, 0);
    delay(100);

    // --- Secvența pas 4: Motorul 1 (din nou) ---
    // Direcție CCW: PWM pe M1_PIN_PWM, DIR LOW pe M1_PIN_DIR
    analogWrite(M1_PIN_PWM, 100);  // viteză 100%
    digitalWrite(M1_PIN_DIR, LOW);
    delay(10000);                    // durată 2 s
    // Oprire Motor 1
    analogWrite(M1_PIN_PWM, 0);
    delay(100);
  }
}

void loop() {
  // Nimic – secvența s-a terminat în setup()
}
