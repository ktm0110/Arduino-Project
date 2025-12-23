#define trigPin 13
#define echoPin 12

#define RED   2
#define YELLOW 3
#define GREEN 4

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 60000);
  distance = ((float)(340 * duration) / 10000) / 2; 

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);

  if (distance > 0 && distance < 5) {
    digitalWrite(RED, HIGH);
  }
  else if (distance >= 5 && distance < 20) {
    digitalWrite(YELLOW, HIGH);
  }
  else if (distance >= 20 && distance <= 1000) {
    digitalWrite(GREEN, HIGH);
  }

  delay(100);
}
