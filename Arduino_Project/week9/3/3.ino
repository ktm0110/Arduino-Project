int x;
int y;
int speakerpin = 7;
int currentSong = 1;
bool playing = false;

// 3옥타브
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247

// 4옥타브
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

// 5옥타브
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

#define REST 0

// 비행기
int melody1[] = {
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4
};
int noteDurations1[] = {
  6, 8, 8, 8, 8, 8, 4,
  8, 8, 4, 8, 8, 4,
  6, 8, 8, 8, 8, 8, 4,
  8, 8, 6, 8, 4
};
int melody1_size = 25;

// super shy
int melody2[] = {
  NOTE_C5, NOTE_E5, NOTE_D5, REST, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E5, NOTE_D5, REST, NOTE_D5, NOTE_D5,
  NOTE_D5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_G5, NOTE_E5, NOTE_E5,
  NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, REST, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G4, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_G4, NOTE_G4, NOTE_D5, NOTE_C5, REST, REST, REST, REST, REST
};
int noteDurations2[] = {
  8, 8, 4, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 4, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 4, 2, 8, 8, 4, 10, 8
};
int melody2_size = 52;

// 캐논
int melody3[] = {
  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_A3, NOTE_B3};
int noteDurations3[] = {
  2, 2, 2, 2, 2, 2, 2, 2,
};
int melody3_size = 16;

void setup() {
  Serial.begin(9600);
  noTone(speakerpin);
}

void loop() {
  x = analogRead(A0);
  y = analogRead(A1);

  if (!playing) {
    if (x < 400) {  // 왼쪽
      int prev = currentSong;
      if (currentSong == 1) currentSong = 3;
      else if (currentSong == 2) currentSong = 1;
      else if (currentSong == 3) currentSong = 2;

      if (currentSong != prev) {
        printSongList();
        Serial.print("현재 선택된 곡: ");
        Serial.println(currentSong);
      }
    }
    else if (x > 600) {  // 오른쪽
      int prev = currentSong;
      if (currentSong == 1) currentSong = 2;
      else if (currentSong == 2) currentSong = 3;
      else if (currentSong == 3) currentSong = 1;

      if (currentSong != prev) {
        printSongList();
        Serial.print("현재 선택된 곡: ");
        Serial.println(currentSong);
      }
    }
  }

// 재생
  if (y > 600 && !playing) {
    playing = true;
    Serial.print("연주 시작: ");
    if (currentSong == 1) Serial.println("비행기");
    else if (currentSong == 2) Serial.println("Super Shy");
    else if (currentSong == 3) Serial.println("캐논 변주곡");

    playSong(currentSong);

    Serial.println("연주 종료");
    noTone(speakerpin);
    playing = false;
    delay(300);
  }

  delay(100);
}

// 목록
void printSongList() {
  Serial.println("1: 비행기");
  Serial.println("2: Super Shy");
  Serial.println("3: 캐논 변주곡");
}

// 연주
void playSong(int song) {
  int* melody;
  int* noteDurations;
  int size;

  if (song == 1) {
    melody = melody1;
    noteDurations = noteDurations1;
    size = melody1_size;
  }
  else if (song == 2) {
    melody = melody2;
    noteDurations = noteDurations2;
    size = melody2_size;
  }
  else if (song == 3) {
    melody = melody3;
    noteDurations = noteDurations3;
    size = melody3_size;
  }
  else return;

  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];

    if (melody[thisNote] == REST) {
      // 쉼표
      delay(noteDuration * 1.30);
    } else {
      // 음 재생
      tone(speakerpin, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(speakerpin);
    }
  }

  noTone(speakerpin);
}