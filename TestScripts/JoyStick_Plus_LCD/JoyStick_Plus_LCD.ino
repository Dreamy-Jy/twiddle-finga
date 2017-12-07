#include <LiquidCrystal.h>

#define JYSTCK_X A0

unsigned int XMin, XCenter, XMax;

int command;
int score = 0;
boolean wasCorrect = false;


String commands[3] = {"left", "center", "right"};

unsigned long startTime, commandDelay = 1500;

const unsigned int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  calibrate();

  clearLCDRow(0, 0, 0); lcd.print("Interpreting:");

  startTime = millis();
  command = random(0, 3);
  lcd.clear(); lcd.print("Tilt "); lcd.print(commands[command]);

}

void loop() {
  int reading = analogRead(JYSTCK_X);
  int response;
  boolean correct = false;

  if ( abs(millis() - startTime) >= commandDelay ) {
    command = random(0, 3);
    startTime = millis();
    wasCorrect = false;
    correct = false;
    clearLCDRow(0, 0, 0); lcd.print("Tilt "); lcd.print(commands[command]);
    setScore(0, 0);
  }

  // input interpetation and correctness Checking Code
  if (reading <= ((XCenter - XMin) / 2)) {
    response = 0;
  } else if (reading >= (XCenter + ((XMax - XCenter) / 2))) {
    response = 2;
  } else {
    response = 1;
  }
  if ( response == command ) {
    correct = true;
    clearLCDRow(1, 0, 1); lcd.print("OK: "); lcd.print(commands[response]);
  } else {
    clearLCDRow(1, 0, 1); lcd.print("NO: "); lcd.print(commands[response]);
  }

  if (correct == true && wasCorrect == false) {
    score++;
    setScore(0, 0);
    wasCorrect = true;
  }

  delay(500);
}

void setScore(int prevCol, int prevRow) {
  if (score < 10) {
    lcd.setCursor(15, 0); lcd.print(score);
  } else {
    lcd.setCursor(14, 0); lcd.print(score);
  }
  lcd.setCursor(prevCol, prevRow);
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
  }

  XCenter = (centerP / 100);
  clearLCDRow(1, 0, 1); lcd.print("center at: "); lcd.print(XCenter);

  delay(2000);

  // left calibration
  clearLCDRow(1, 0, 1); lcd.print("Tilt stick left");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    lowerP += analogRead(JYSTCK_X);
  }

  XMin = (lowerP / 100);
  clearLCDRow(1, 0, 1); lcd.print("Left at: "); lcd.print(XMin);

  delay(2000);

  // right calibration

  clearLCDRow(1, 0, 1); lcd.print("Tilt stick right");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    maxP += analogRead(JYSTCK_X);
  }

  XMax = (maxP / 100);
  clearLCDRow(1, 0, 1); lcd.print("Right at: "); lcd.print(XMax);

  delay(1000);
}
