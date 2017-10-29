
const int STICK_SWITCH = 3;

void setup() {
  pinMode(STICK_SWITCH, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print("y value: ");
  Serial.println(analogRead(A0));
  Serial.print("x value: ");
  Serial.println(analogRead(A1));
  if (digitalRead(STICK_SWITCH) == LOW) {
    Serial.println("\n\nI'm pressed fam\n\n");
  }

  delay(100);
}
