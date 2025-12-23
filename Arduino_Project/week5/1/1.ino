#define segA 2
#define segB 3
#define segC 5
#define segD 6
#define segE 7
#define segF 8
#define segG 9
#define segDP 4
#define buttonPin 10

int fndPort[8] = {segA, segB, segC, segD, segE, segF, segG, segDP};

int digitData[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

int studentID[] = {2,0,2,2,0,4,2,0,4,7};
int phoneNum[]  = {9,9,3,4};  

bool mode = 0;

void setup(){
  for(int i=0;i<8;i++) pinMode(fndPort[i], OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void displayDigit(int num){
  for(int i=0;i<8;i++){
    digitalWrite(fndPort[i], digitData[num][i]);
  }
}

void flashEight(){
  for(int k=0;k<2;k++){
    displayDigit(8);
    delay(300);
    clearDisplay();
    delay(300);
  }
}

void clearDisplay(){
  for(int i=0;i<8;i++) digitalWrite(fndPort[i], 0);
}

bool buttonPressed(){
  if(digitalRead(buttonPin) == LOW){
    while(digitalRead(buttonPin) == LOW);
    return true;
  }
  return false;
}

void loop(){
  if(mode == 0){
    for(int i=0;i<sizeof(studentID)/sizeof(int);i++){
      displayDigit(studentID[i]);
      unsigned long t0 = millis();
      while(millis()-t0 < 500){
        if(digitalRead(buttonPin) == LOW){
          flashEight();
          mode = 1;
          return;
        }
      }
    }
    flashEight();
    mode = 1;
  } 
  else {
    for(int i=0;i<sizeof(phoneNum)/sizeof(int);i++){
      displayDigit(phoneNum[i]);
      unsigned long t0 = millis();
      while(millis()-t0 < 500){
        if(digitalRead(buttonPin) == LOW){
          flashEight();
          mode = 0;
          return;
        }
      }
    }
    flashEight();
    mode = 0;
  }
}
