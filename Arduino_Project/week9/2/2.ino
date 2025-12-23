int x;
int leftLED = 5;
int rightLED = 4;

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
}

void loop() {
  x = analogRead(A0);

  Serial.print(x);
  Serial.print(" ");
  Serial.println(digitalRead(8));

  if (x < 400) { // 왼쪽
    digitalWrite(leftLED, HIGH);
    digitalWrite(rightLED, LOW);
    Serial.println("Left direction detected");
  } else if (x > 600) { // 오른쪽
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, HIGH);
    Serial.println("Right direction detected");
  } else { // 중앙
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, LOW);
    Serial.println("Center position");
  }

  delay(100);
}