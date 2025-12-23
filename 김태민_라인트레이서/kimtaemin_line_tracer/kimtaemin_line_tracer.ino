#include <Servo.h>
Servo myservo;

int trigPin = 13;
int echoPin = 12;

int RightMotor_E_pin = 5;
int LeftMotor_E_pin = 6;
int RightMotor_1_pin = 8;
int RightMotor_2_pin = 9;
int LeftMotor_3_pin = 10;
int LeftMotor_4_pin = 11;

int L_Line = A5;
int C_Line = A4;
int R_Line = A3;

int L_MAX_MotorSpeed = 180;
int R_MAX_MotorSpeed = 180;
int L_MotorSpeed = 0;
int R_MotorSpeed = 0;

const int TARGET_WALL_DISTANCE = 13;   // 오른쪽 벽 13cm 유지
const int WALL_SWITCH_THRESHOLD = 40;
const int BASE_SPEED = 130;
const int TURN_ADJUST = 70;

const int SERVO_RIGHT_WALL = 0;        // 오른쪽 벽 보기

void setup() {
  pinMode(RightMotor_E_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);

  pinMode(L_Line, INPUT);
  pinMode(C_Line, INPUT);
  pinMode(R_Line, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myservo.attach(2);
  myservo.write(SERVO_RIGHT_WALL);

  Serial.begin(9600);
  Serial.println("10cm보다 가까우면 왼쪽, 멀면 오른쪽, 기본 직진");
  delay(1500);
}

void loop() {
  myservo.write(SERVO_RIGHT_WALL);
  delay(50);
  int wall_dist = getDistance_cm();

  if (wall_dist >= WALL_SWITCH_THRESHOLD || wall_dist < 0) {
    Serial.print("벽 멀음: "); Serial.print(wall_dist); Serial.println("cm 라인 추종");
    follow_line();
    return;
  }

  Serial.print("벽: "); Serial.print(wall_dist); Serial.print("cm ");
  maintain_10cm_wall(wall_dist);
  delay(20);
}

void maintain_10cm_wall(int dist) {
  if (dist < TARGET_WALL_DISTANCE) {
    L_MotorSpeed = BASE_SPEED - TURN_ADJUST;
    R_MotorSpeed = BASE_SPEED + TURN_ADJUST;
    motor_role(HIGH, HIGH);
    Serial.println("10cm보다 가까움 왼쪽 회전");
  }
  else if (dist > TARGET_WALL_DISTANCE) {
    L_MotorSpeed = BASE_SPEED + TURN_ADJUST;
    R_MotorSpeed = BASE_SPEED - TURN_ADJUST;
    motor_role(HIGH, HIGH);
    Serial.println("10cm보다 멀음 오른쪽 회전");
  }
  else {
    L_MotorSpeed = BASE_SPEED;
    R_MotorSpeed = BASE_SPEED;
    motor_role(HIGH, HIGH);
    Serial.println("10cm 직진");
  }
}

void follow_line() {
  int L = digitalRead(L_Line);
  int C = digitalRead(C_Line);
  int R = digitalRead(R_Line);

  if (L == HIGH && C == HIGH && R == LOW) {
    turn_right();
  }
  else if (L == LOW && C == HIGH && R == HIGH) {
    turn_left();
  }
  else if (L == HIGH && C == LOW && R == HIGH) {
    go_forward();
  }
  else {
    go_forward();
  }
}

void go_forward() {
  L_MotorSpeed = L_MAX_MotorSpeed;
  R_MotorSpeed = R_MAX_MotorSpeed;
  motor_role(HIGH, HIGH);
}

void turn_left() {
  L_MotorSpeed = 40;
  R_MotorSpeed = R_MAX_MotorSpeed;
  motor_role(HIGH, HIGH);
}

void turn_right() {
  L_MotorSpeed = L_MAX_MotorSpeed;
  R_MotorSpeed = 60;
  motor_role(LOW, HIGH);
}

void motor_role(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);
  analogWrite(RightMotor_E_pin, R_MotorSpeed);
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);
}

int getDistance_cm() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;

  int distance = ((float)(340 * duration) / 10000) / 2;
  if (distance < 2 || distance > 400) return -1;

  return distance;
}