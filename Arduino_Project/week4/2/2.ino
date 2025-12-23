#include <IRremote.h>

// 초음파
#define trigPin 13
#define echoPin 12

// 스위치
#define BUTTON 2

// led
#define YELLOW 3
#define GREEN 4

// ir
#define RECV_PIN A0
IRrecv irrecv(RECV_PIN);
decode_results results;

// 기본 10
int mode = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value != 0xFFFFFFFF) {
      handle(results.value);
    }
    irrecv.resume();
  }

  // 스위치 x
  if (digitalRead(BUTTON) == HIGH) {
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);
  } 
  // 스위치 o
  else {
    long distance = getDistance();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 0 && distance < mode) {
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
    } 
    else {
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, HIGH);
    }
  }

  delay(200);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  return distance;
}

void handle(unsigned long code) {
  switch (code) {
    case 0xFF30CF:  // 1번
      mode = 10;
      Serial.println("10cm Mode");
      break;

    case 0xFF18E7:  // 2번
      mode = 20;
      Serial.println("20cm Mode");
      break;
  }
}
