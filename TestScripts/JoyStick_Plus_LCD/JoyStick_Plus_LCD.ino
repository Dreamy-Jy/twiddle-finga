#include <LiquidCrystal.h>

#define JYSTCK_X A0

unsigned int XMin, XCenter, XMax;

const unsigned int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  calibrate();

  clearLCDRow(0, 0, 0); lcd.print("Interpreting");
}

void loop() {
  //  Serial.print("maxP at: "); Serial.println(analogRead(A0)); Serial.println("");

  int reading = analogRead(JYSTCK_X);

  clearLCDRow(1, 0, 1);
  if (reading <= ((XCenter - XMin) / 2)) {
    lcd.print("Left");
  } else if (reading >= (XCenter + ((XMax - XCenter) / 2))) {
    lcd.print("Right");
  } else {
    lcd.print("Center");
  }

  delay(250);
}

void clearLCDRow(int row, int prevCol, int prevRow) {
  lcd.setCursor(0, row); lcd.print("                ");
  lcd.setCursor(prevCol, prevRow);
}

void calibrate() {
  unsigned int centerP = 0;
  unsigned int lowerP = 0;
  unsigned long maxP = 0;

  lcd.print("Calibrating");

  // center calibration
  lcd.setCursor(0, 1); lcd.print("center the stick");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    centerP += analogRead(JYSTCK_X);
    //    Serial.print("counter at: "); Serial.println(i); // @test
    //    Serial.print("centerP at: "); Serial.println(centerP); // @test
    //    Serial.println(""); // @test
  }

  XCenter = (centerP / 100);
  clearLCDRow(1, 0, 1); lcd.print("center at: "); lcd.print(XCenter);

  delay(2000);

  // left calibration
  clearLCDRow(1, 0, 1); lcd.print("Tilt stick left");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    lowerP += analogRead(JYSTCK_X);
    //    Serial.print("counter at: "); Serial.println(i); // @test
    //    Serial.print("lowerP at: "); Serial.println(lowerP); // @test
    //    Serial.println(""); // @test
  }

  XMin = (lowerP / 100);
  clearLCDRow(1, 0, 1); lcd.print("Left at: "); lcd.print(XMin);

  delay(2000);

  // right calibration

  clearLCDRow(1, 0, 1); lcd.print("Tilt stick right");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    maxP += analogRead(JYSTCK_X);
    //    Serial.print("Sensor Reading: "); Serial.println(analogRead(JYSTCK_X)); // @test
    //    Serial.print("counter at: "); Serial.println(i); // @test
    //    Serial.print("maxP at: "); Serial.println(maxP); // @test
    //    Serial.println(""); // @test
  }

  XMax = (maxP / 100);
  clearLCDRow(1, 0, 1); lcd.print("Right at: "); lcd.print(XMax);

  delay(1000);
}
