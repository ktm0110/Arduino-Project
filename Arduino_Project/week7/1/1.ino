#include <U8glib.h>
#include <IRremote.h>
#include <Servo.h>

U8GLIB_SSD1306_128X64
u8g(U8G_I2C_OPT_NONE);

int TOUCH_PIN = 7;
int IR_PIN    = A0;
int GREEN_LED = 2;
int RED_LED   = 3;
int SERVO_PIN = 9;

IRrecv irrecv(IR_PIN);
decode_results results;
Servo lockServo;

int input[4] = {0};
int pos = 0;
int tap = 0;
unsigned long lastTap = 0;
#define TAP_TIMEOUT 800
#define DEBOUNCE    150

unsigned long unlockTime = 0;
#define UNLOCK_TIME 5000

// 비밀번호
const int PROGMEM correct[4] = {1, 2, 3, 4};

// 입력 화면
void drawInput() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_fub17);
    u8g.setPrintPos(0, 20);
    u8g.print(F("PW: "));
    
    u8g.setPrintPos(0, 60);
    for (int i = 0; i < 4; i++) {
      if (i < pos) {
        u8g.print(input[i]);
      } else {
        u8g.print('*');
      }
      u8g.print(' ');
    }
    // 현재 터치 입력
    if (tap > 0 && pos < 4) {
      u8g.setPrintPos(100, 60);
      u8g.print(tap);
    }
  } while (u8g.nextPage());
}

// 상태메시지
void showStatus(int code) {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_fub17);
    u8g.setPrintPos(50, 50);
    u8g.print(code);
  } while (u8g.nextPage());
}

// 카운트다운
void showCountdown() {
  unsigned long elapsed = millis() - unlockTime;
  if (elapsed >= UNLOCK_TIME) return;
  int remain = (UNLOCK_TIME - elapsed) / 1000;
  showStatus(remain);  // 5,4,3,2,1
}

// 문 열림
void openDoor() {
  digitalWrite(GREEN_LED, HIGH);
  lockServo.write(90);
  unlockTime = millis();
  showStatus(1);  // OPEN
  delay(800);
}

// 자동 잠금
void lockDoor() {
  lockServo.write(0);
  digitalWrite(GREEN_LED, LOW);
  unlockTime = 0;
  showStatus(2);  // LOCK
  delay(800);
  showStatus(0);  // 입력 대기
  drawInput();   // 입력 화면 복귀
}

// 틀렸을때
void wrong() {
  digitalWrite(RED_LED, HIGH);
  showStatus(3);
  delay(800);
  showStatus(2);
  delay(800);
  showStatus(1);
  delay(800);
  digitalWrite(RED_LED, LOW);
  showStatus(0);
  delay(800);
  drawInput();
}

// 입력 초기화
void resetInput() {
  pos = 0;
  tap = 0;
  lastTap = 0;
  for (int i = 0; i < 4; i++) input[i] = 0;
  drawInput();
}

// 비밀번호 확인
bool checkPassword() {
  for (int i = 0; i < 4; i++) {
    if (input[i] != pgm_read_byte(&correct[i])) return false;
  }
  return true;
}

// 터치 입력
void handleTouch() {
  static bool last = false;
  bool now = digitalRead(TOUCH_PIN);

  if (now && !last) {
    unsigned long t = millis();
    if (t - lastTap < DEBOUNCE) return;
    if (tap == 0 || t - lastTap <= TAP_TIMEOUT) {
      tap++;
      lastTap = t;
      drawInput();
    }
  }
  last = now;

  if (tap > 0 && millis() - lastTap > TAP_TIMEOUT) {
    if (tap >= 1 && tap <= 9 && pos < 4) {
      input[pos++] = tap;
      drawInput();
    }
    tap = 0;
  }
}

// IR 입력
void handleIR() {
  if (irrecv.decode(&results)) {
    uint32_t code = results.value;
    if (code >= 0x100000 && code <= 0xFFFFFF) {
      int digit = 0;
      switch (code) {
        case 0xFF30CF: digit = 1; break;
        case 0xFF18E7: digit = 2; break;
        case 0xFF7A85: digit = 3; break;
        case 0xFF10EF: digit = 4; break;
        case 0xFF38C7: digit = 5; break;
        case 0xFF5AA5: digit = 6; break;
        case 0xFF42BD: digit = 7; break;
        case 0xFF4AB5: digit = 8; break;
        case 0xFF52AD: digit = 9; break;
      }
      if (digit >= 1 && digit <= 9 && pos < 4) {
        input[pos++] = digit;
        drawInput();
      }
    }
    irrecv.resume();
  }
}

void setup() {
  pinMode(TOUCH_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  lockServo.attach(SERVO_PIN);
  lockServo.write(0);

  irrecv.enableIRIn();

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  showStatus(2);  // 잠금
  delay(1000);
  showStatus(0);  // 입력 대기
  drawInput();    // 입력 화면 초기화
}

void loop() {
  handleTouch();
  handleIR();

  // 4자리 입력 완료
  if (pos == 4) {
    if (checkPassword()) {
      openDoor();
    } else {
      wrong();
    }
    resetInput();
  }

  // 카운트다운 잠금
  if (unlockTime > 0) {
    unsigned long elapsed = millis() - unlockTime;
    if (elapsed < UNLOCK_TIME) {
      showCountdown();
    } else {
      lockDoor();
    }
  }

  delay(50);
}