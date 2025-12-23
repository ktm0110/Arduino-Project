#include <SoftwareSerial.h>

int Tx = 6; 
int Rx = 7;
SoftwareSerial BtSerial(Tx,Rx);
 void setup() {
  Serial.begin(9600);
  Serial.println("hello");
  BtSerial.begin(9600);
 }
 void loop() {
  if (BtSerial.available()) {       
    Serial.write(BtSerial.read());
  }
  if (Serial.available()) {         
    BtSerial.write(Serial.read());
  }
 }