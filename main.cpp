#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int adcsensor = A3;
int adcsensorRead;
int lightLED = 10;
int touchSensorSignalPin = 7;
int lastState = LOW;
int currentState;
float fix;
int datafix;
int cacah;
float kalibrasi;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Glucose Meter ");
  lcd.setCursor(0, 5);
  lcd.print("*PROTOTYPE V.1.3");
  pinMode(lightLED, OUTPUT);
  pinMode(adcsensor, INPUT);
  pinMode(touchSensorSignalPin, INPUT_PULLUP);
  idleLight();
}

void loop() {
  idleLight();
  printLCD(1);

  currentState = digitalRead(touchSensorSignalPin);
  if (lastState == LOW && currentState == HIGH) {
    cacah = 0;
    datafix = 0;
    fix = 0;
    digitalWrite(lightLED, HIGH);
    proses();
  }
  lastState = currentState;
}

void proses() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Glucose Meter");
  lcd.setCursor(0, 5);
  lcd.print("ADC= ");

  for (int cacah = 0; cacah <= 43; cacah++) {
    currentState = digitalRead(touchSensorSignalPin);
    adcsensorRead = analogRead(adcsensor);
    lcd.setCursor(5, 5);
    lcd.print(adcsensorRead);
    datafix = adcsensorRead + datafix;
    delay(50);



    if (lastState == LOW && currentState == LOW) {
      return;
    }

    if (cacah == 43) {
      fix = datafix / 20;
      //        kalibrasi = (fix - 1022.1) / 0.0967;
      kalibrasi = fix - 10.21;
      //              kalibrasi = (fix - 10.21) * 1.8;

      Serial.println("Datafix, Fix dan Kalibrasi:      ");
      //            Serial.println(datafix);
      //            Serial.println(fix);
      Serial.println(kalibrasi);

      printLCD(2);

      delay(5000);

      printLCD(3);

      delay(5000);
      Serial.println("Proses selesai");
      Serial.println("        ");
      return;
    }
  }

  lastState = currentState;
}

void idleLight() {
  digitalWrite(lightLED, LOW);
  delay(500);
  digitalWrite(lightLED, HIGH);
  delay(500);
  digitalWrite(lightLED, LOW);
}

void printLCD(int text) {
  if (text == 1) {
    lcd.setCursor(0, 0);
    lcd.print("INSERT FINGER       ");
    lcd.setCursor(0, 5);
    lcd.print("TO START        ");
  } else if (text == 2) {
    // lcd.setCursor(0, 0);
    // lcd.print("KADAR Gula Darah");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ADC= ");
    lcd.print(fix);

    lcd.setCursor(0, 5);
    lcd.print("mg/dl= ");
    lcd.print(kalibrasi);
  } else if (text == 3) {
    if (kalibrasi < 80) {
      lcd.setCursor(0, 0);
      lcd.print("LOW LEVEL    ");
    }

    if ((kalibrasi >= 80) && (kalibrasi <= 200)) {
      lcd.setCursor(0, 0);
      lcd.print("NORMAL LEVEL    ");
    }

    if (kalibrasi > 200) {
      lcd.setCursor(0, 0);
      lcd.print("HIGH LEVEL    ");
    }
  }
}
