#define LEFT_BTN   4
#define CENTER_BTN 3
#define RIGHT_BTN  2

void setup() {
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(CENTER_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(LEFT_BTN) == LOW) {
    Serial.println("left");
    delay(300);
  }
  else if (digitalRead(CENTER_BTN) == LOW) {
    Serial.println("center");
    delay(300);
  }
  else if (digitalRead(RIGHT_BTN) == LOW) {
    Serial.println("right");
    delay(300);
  }
}
