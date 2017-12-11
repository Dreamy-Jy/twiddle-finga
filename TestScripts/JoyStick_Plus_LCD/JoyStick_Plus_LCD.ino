#include <LiquidCrystal.h>

#define JYSTCK_X A0
#define JYSTCK_Y A1
#define SPEKR 13

unsigned int XMin, XCenter, XMax;
unsigned int YMin, YCenter, YMax;

int score = 0, gameRound = 1;
int command; boolean wasCorrect = false;

const int CMD_NUM = 5;
String commands[CMD_NUM] = {"left", "right", "up", "down", "center"};

unsigned long commandStartTime, commandChangeInterval = 1500;
unsigned long roundStartTime, roundChangeInterval = 30000;

const unsigned int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(SPEKR , OUTPUT);

  calibrate();

  startRound();
}

void loop() {
  int JYSTCK_currentXVal = analogRead(JYSTCK_X);
  int JYSTCK_currentYVal = analogRead(JYSTCK_Y);

  int response; // this seems redundant
  boolean correct = false;

  if (abs(millis() - roundStartTime) >= roundChangeInterval) {
    gameRound++;
    startRound();
  }

  if ( abs(millis() - commandStartTime) >= commandChangeInterval ) {
    command = random(0, (CMD_NUM - 1) );
    wasCorrect = false;
    clearLCDRow(0, 0, 0); lcd.print("Tilt "); lcd.print(commands[command]);
    updateScoreDisplay(0, 0);
    commandStartTime = millis();
  }

  response = interpetJKSTICKInputs(JYSTCK_currentXVal, JYSTCK_currentYVal);

  if ( response == command ) {
    correct = true;
    clearLCDRow(1, 0, 1); lcd.print("OK: "); lcd.print(commands[response]);
  } else {
    clearLCDRow(1, 0, 1); lcd.print("NO: "); lcd.print(commands[response]);
  }

  if (correct == true && wasCorrect == false) {
    score++;
    updateScoreDisplay(0, 0);
    wasCorrect = true;
  }

  delay(500);
}

void startRound() {
  score = 0;
  command = random(0, (CMD_NUM - 1) );
  lcd.clear(); lcd.print("Round "); lcd.print(gameRound);
  lcd.setCursor(0, 1); lcd.print("Get ready!!");
  if (gameRound > 1) {
    tone(SPEKR, 100, 1000);
    delay(1500);
    tone(SPEKR, 50, 1000);
    delay(1500);
    tone(SPEKR, 25, 1000);
  }
  delay(2000);
  lcd.clear(); lcd.print("Tilt "); lcd.print(commands[command]); updateScoreDisplay(0, 0);
  roundStartTime = millis();
  commandStartTime = millis();
}

void updateScoreDisplay(int prevCol, int prevRow) {
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

// code works heavly under the assumption that users will not try to
// tilt to the corners
int interpetJKSTICKInputs(int input_X, int input_Y ) {
  Serial.print("\n\nX input at: "); Serial.print(input_X); Serial.print("\n");
  Serial.print("Y input at: "); Serial.print(input_Y); Serial.print("\n");
  
  if (abs(input_X - XCenter) > abs(input_Y - YCenter) ) {
    if (input_X <= ((XCenter - XMin) / 2)) {
      Serial.print("I think this is a "); Serial.print(commands[0]);Serial.print(" tilt\n");
      return 0;
    } else if (input_X >= (XCenter + ((XMax - XCenter) / 2))) {
      Serial.print("I think this is a "); Serial.print(commands[1]);Serial.print(" tilt\n");
      return 1;
    } else {
      Serial.print("I think this is a "); Serial.print(commands[4]);Serial.print(" tilt\n");
      return 4;
    }
  } else if (abs(input_Y - YCenter) > abs(input_X - XCenter) ) {
    if (input_Y <= ((YCenter - YMin) / 2)) {
      Serial.print("I think this is a "); Serial.print(commands[2]);Serial.print(" tilt\n");
      return 2;
    } else if (input_Y >= (YCenter + ((YMax - YCenter) / 2))) {
      Serial.print("I think this is a "); Serial.print(commands[3]);Serial.print(" tilt\n");
      return 3;
    } else {
      Serial.print("I think this is a "); Serial.print(commands[4]);Serial.print(" tilt\n");
      return 4;
    }
  } else { // this else just catchs unexpected values
    return 4;
  }
}

void calibrate() {
  unsigned int centerP_X = 0;
  unsigned int centerP_Y = 0;
  unsigned int lowerP_X = 0;
  unsigned int lowerP_Y = 0;
  unsigned long maxP_X = 0;
  unsigned long maxP_Y = 0;

  lcd.print("Calibrating");

  // center calibration
  lcd.setCursor(0, 1); lcd.print("center the stick");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    centerP_X += analogRead(JYSTCK_X);
    centerP_Y += analogRead(JYSTCK_Y);
  }

  XCenter = (centerP_X / 100);
  YCenter = (centerP_Y / 100);
  clearLCDRow(1, 0, 1); lcd.print("center: "); lcd.print(XCenter); lcd.print(", "); lcd.print(YCenter);

  delay(2000);

  // left calibration
  clearLCDRow(1, 0, 1); lcd.print("Tilt stick left");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    lowerP_X += analogRead(JYSTCK_X);
  }

  XMin = (lowerP_X / 100);
  clearLCDRow(1, 0, 1); lcd.print("Left at: "); lcd.print(XMin);

  delay(2000);

  // right calibration

  clearLCDRow(1, 0, 1); lcd.print("Tilt stick right");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    maxP_X += analogRead(JYSTCK_X);
  }

  XMax = (maxP_X / 100);
  clearLCDRow(1, 0, 1); lcd.print("Right at: "); lcd.print(XMax);

  delay(2000);

  // up calibration
  clearLCDRow(1, 0, 1); lcd.print("Tilt stick Up");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    lowerP_Y += analogRead(JYSTCK_Y);
  }

  YMin = (lowerP_Y / 100);
  clearLCDRow(1, 0, 1); lcd.print("Up at: "); lcd.print(YMin);

  delay(2000);

  // down calibration

  clearLCDRow(1, 0, 1); lcd.print("Tilt stick Down");
  delay(2000);

  for (int i = 0; i < 100; i++) {
    maxP_Y += analogRead(JYSTCK_Y);
  }

  YMax = (maxP_Y / 100);
  clearLCDRow(1, 0, 1); lcd.print("Down at: "); lcd.print(YMax);

  delay(1000);
}
