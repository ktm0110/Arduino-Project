int touch_pin = 7;
int led_pin = 4;
int state = 0;
bool touch_flag = false;
void setup(){
  Serial.begin(9600);
  pinMode(touch_pin,INPUT);
  pinMode(led_pin, OUTPUT);
}

void loop(){
  state = digitalRead(touch_pin);
  if(state == HIGH){ // HIGH 면터치센서가작동한것
    if(touch_flag == false){ //print를한번만하기위해
      touch_flag = true;
      Serial.println("on");
      digitalWrite(led_pin, HIGH);
      delay(500);
    }
  }
  else{
    if(touch_flag == true){
      touch_flag = false;
      Serial.println("off");
      digitalWrite(led_pin, LOW);
      delay(500);
    }
  }
}
