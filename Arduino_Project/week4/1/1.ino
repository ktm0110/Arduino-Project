#include <IRremote.h>

int RECV_PIN = A0;
IRrecv irrecv(RECV_PIN);
decode_results results;

long num1 = 0, num2 = 0;
char op = 0;
bool enterNum1 = true;

void setup() {
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
}

void handle(unsigned long IR_num) {
  int digit = -1;

  switch (IR_num) {
    case 0xFF6897: digit = 0; break;
    case 0xFF30CF: digit = 1; break;
    case 0xFF18E7: digit = 2; break;
    case 0xFF7A85: digit = 3; break;
    case 0xFF10EF: digit = 4; break;
    case 0xFF38C7: digit = 5; break;
    case 0xFF5AA5: digit = 6; break;
    case 0xFF42BD: digit = 7; break;
    case 0xFF4AB5: digit = 8; break;
    case 0xFF52AD: digit = 9; break;

    case 0xFFA857:
      op = '+';
      enterNum1 = false;
      Serial.print(" + ");
      return;

    case 0xFFE01F:
      op = '-';
      enterNum1 = false;
      Serial.print(" - ");
      return;

    case 0xFF906F:
      if (op == '+') {
        Serial.print(" = ");
        Serial.print(num1 + num2);
        Serial.println();
      }
      else if (op == '-') {
        Serial.print(" = ");
        Serial.print(num1 - num2);
        Serial.println();
      }
      else Serial.println("Err");
      num1 = num2 = 0;
      op = 0;
      enterNum1 = true;
      return;
  }

  if (digit >= 0) {
    if (enterNum1) {
      num1 = num1 * 10 + digit;
      Serial.print(digit);
    } else {
      num2 = num2 * 10 + digit;
      Serial.print(digit);
    }
  }
}
