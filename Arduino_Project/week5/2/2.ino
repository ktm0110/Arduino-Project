#include <Servo.h>

#define segA 2
#define segB 3
#define segC 5
#define segD 6
#define segE 7
#define segF 8
#define segG 9
int fndPort[7] = {segA, segB, segC, segD, segE, segF, segG};

#define swLeft   10   // 풍속 버튼
#define swCenter 12   // 온오프 버튼
#define swRight  13   // 회전 버튼

#define ledRed    A3
#define ledYellow A4
#define ledGreen  A5

// DC 모터
#define prop_EN   11
#define prop_IN1  A0
#define prop_IN2  A1

// 서보 모터
Servo headServo;
#define head_PIN 4    

int segMinus[7] = {0,0,0,0,0,0,1}; 
int segEqual[7] = {1,0,0,1,0,0,0}; 
int segTriple[7]= {1,0,0,1,0,0,1}; 
int segII[7]    = {0,1,1,0,1,1,0};  

bool isOn = false;
int windMode = 0; // 0 없음, 1 미풍, 2 강풍
bool isRotating = false;

int pos = 90;
int delta = 1;
const int minPos = 0;
const int maxPos = 180;
unsigned long previousMillis = 0;
const long servoInterval = 20;

const long debounceDelay = 50;

int buttonStateLeft = HIGH;
int lastButtonStateLeft = HIGH;
unsigned long lastDebounceTimeLeft = 0;

int buttonStateCenter = HIGH;
int lastButtonStateCenter = HIGH;
unsigned long lastDebounceTimeCenter = 0;

int buttonStateRight = HIGH;
int lastButtonStateRight = HIGH;
unsigned long lastDebounceTimeRight = 0;

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(fndPort[i], OUTPUT);
  }

  pinMode(swLeft, INPUT_PULLUP);
  pinMode(swCenter, INPUT_PULLUP);
  pinMode(swRight, INPUT_PULLUP);

  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  pinMode(prop_EN, OUTPUT);
  pinMode(prop_IN1, OUTPUT);
  pinMode(prop_IN2, OUTPUT);

  headServo.attach(head_PIN);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  analogWrite(prop_EN, 0);
  digitalWrite(prop_IN1, LOW);
  digitalWrite(prop_IN2, LOW);
  headServo.write(90);
  showSeg(segMinus);
}

void loop() {
  // 왼쪽 버튼 처리
  int readingLeft = digitalRead(swLeft);
  if (readingLeft != lastButtonStateLeft) {
    lastDebounceTimeLeft = millis();
  }
  if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
    if (readingLeft != buttonStateLeft) {
      buttonStateLeft = readingLeft;
      if (buttonStateLeft == LOW && isOn) {
        if (windMode == 0) {
          windMode = 1;
        } else {
          windMode = (windMode == 1) ? 2 : 1;
        }
      }
    }
  }
  lastButtonStateLeft = readingLeft;

  // 가운데 버튼 처리
  int readingCenter = digitalRead(swCenter);
  if (readingCenter != lastButtonStateCenter) {
    lastDebounceTimeCenter = millis();
  }
  if ((millis() - lastDebounceTimeCenter) > debounceDelay) {
    if (readingCenter != buttonStateCenter) {
      buttonStateCenter = readingCenter;
      if (buttonStateCenter == LOW) {
        isOn = !isOn;
        if (isOn) {
          windMode = 0;
          isRotating = false;
        }
      }
    }
  }
  lastButtonStateCenter = readingCenter;

  // 오른쪽 버튼 처리
  int readingRight = digitalRead(swRight);
  if (readingRight != lastButtonStateRight) {
    lastDebounceTimeRight = millis();
  }
  if ((millis() - lastDebounceTimeRight) > debounceDelay) {
    if (readingRight != buttonStateRight) {
      buttonStateRight = readingRight;
      if (buttonStateRight == LOW && isOn && windMode > 0) {
        isRotating = !isRotating;
      }
    }
  }
  lastButtonStateRight = readingRight;

  updateMotors();

  updateDisplay();
}

void updateMotors() {
  if (!isOn || windMode == 0) {
    analogWrite(prop_EN, 0);
    digitalWrite(prop_IN1, LOW);
    digitalWrite(prop_IN2, LOW);
    return;
  }

  // DC 모터
  digitalWrite(prop_IN1, HIGH);
  digitalWrite(prop_IN2, LOW);
  int speed = (windMode == 1) ? 128 : 255;
  analogWrite(prop_EN, speed);

  // 서보 모터
  if (isRotating) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= servoInterval) {
      previousMillis = currentMillis;
      pos += delta;
      if (pos >= maxPos) {
        pos = maxPos;
        delta = -1;
      } else if (pos <= minPos) {
        pos = minPos;
        delta = 1;
      }
      headServo.write(pos);
    }
  }
}

void updateDisplay() {
  if (!isOn || windMode == 0) {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    showSeg(segMinus);
    return;
  }

  // LED
  digitalWrite(ledYellow, (windMode == 1) ? HIGH : LOW);
  digitalWrite(ledGreen, (windMode == 2) ? HIGH : LOW);
  digitalWrite(ledRed, isRotating ? HIGH : LOW);

  // 7 세그먼트
  if (isRotating) {
    showSeg(segII);
  } else {
    if (windMode == 1) {
      showSeg(segEqual);
    } else {
      showSeg(segTriple);
    }
  }
}

void showSeg(int pattern[]) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(fndPort[i], pattern[i]);
  }
}