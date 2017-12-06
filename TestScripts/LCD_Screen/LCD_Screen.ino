#include <LiquidCrystal.h>

#define VY_POT_PIN A0

const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
const  commands[4] = {};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  lcd.print("twiddle fingga");

  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0, 1);
  //  respond(random(0, 4));
  //  delay(1000);
  currentPotReading = analogRead(POT_PIN);
  
  if (abs(currentPotReading - lastPotReading) >= 5) {
    Serial.println("I have changed");
    Serial.println(currentPotReading);
    potCharacteristic.setValue(currentPotReading);
  }

  lastPotReading = currentPotReading;
  delay(2);


}

void respond(int num) {
  switch (num) {
    case 0:
      lcd.print(1);
      break;
    case 1:
      lcd.print(2);
      break;
    case 2:
      lcd.print(3);
      break;
    case 3:
      lcd.print(4);
      break;
  }
}

/*
  issue #1:
  numbers change to fas
*/
