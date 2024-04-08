//Created by Nevio Collin Zylla aka. Foxymedis for THM-Micro-Project
//Version 1
//Hardware used: Adafruit QT Py ESP32-S3 WiFi Dev Board with STEMMA QT - 8 MB Flash / No PSRAM

//Button Pins
#define BUTTON_1 18
#define BUTTON_2 17
#define BUTTON_3 9
#define BUTTON_4 8

//LED Pins
#define LED_BL 5
#define LED_GR 37
#define LED_RE 36
#define LED_YE 16

#define BUTTON_CODE 1
#define LED_CODE 0
#define ROUND_SIZE 10

#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
// Randomizer:
int random_count[2][ROUND_SIZE];
int simon_round = 0;

void setup() {
  // Button and LED in- and output
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);
  pinMode(LED_BL, OUTPUT);
  pinMode(LED_GR, OUTPUT);
  pinMode(LED_RE, OUTPUT);
  pinMode(LED_YE, OUTPUT);
  randomArray();
}

void loop() {
  // Project code made by Nevio Zylla:
  while(simon_round < ROUND_SIZE) {
    showLED(simon_round);
    gameReady();
    if(process_round(simon_round)) {
      simon_round++;
    }
    else {
      simon_round = 0;
    }
  }
  randomArray();
  game_win();
}

bool process_round(int current_round){
  int simon_index = 0;
  bool result = true;
  while(simon_index <= current_round) {
    int button = check_all_buttons();
    if(right_button(button, simon_index)){
      digitalWrite(random_count[LED_CODE][simon_index], HIGH);
      delay(200);
      digitalWrite(random_count[LED_CODE][simon_index], LOW);
      simon_index++;
      delay(200);
    }
    else{
      game_failed();
      result = false;
      break;
    }
  }
  return result;
}


void randomArray() {
  for (int i = 0; i < ROUND_SIZE; i++) {
    int temp = random(4);
    random_count[BUTTON_CODE][i] = get_button_pin(temp);
    random_count[LED_CODE][i] = get_led_pin(temp);
  }
}

int get_led_pin(int color) {
  switch(color) {
    case RED:
    return LED_RE;
    case BLUE:
    return LED_BL;
    case GREEN:
    return LED_GR;
    case YELLOW:
    return LED_YE;
  }
}

int get_button_pin(int color) {
  switch(color) {
    case YELLOW:
    return BUTTON_1;
    case RED:
    return BUTTON_2;
    case GREEN:
    return BUTTON_3;
    case BLUE:
    return BUTTON_4;
  }
}

void showLED(int rounds) {
  for (int i = 0; i < rounds + 1; i++) {
    int c = random_count[LED_CODE][i];
    digitalWrite(c, HIGH);
    delay(1000);
    digitalWrite(c, LOW);
    delay(200);
  }
}
void gameReady() {
  for(int i = 0; i < 2; i++){
    digitalWrite(LED_RE, HIGH);
    digitalWrite(LED_BL, HIGH);
    delay(200);
    digitalWrite(LED_RE, LOW);
    digitalWrite(LED_BL, LOW);
    delay(200);
    digitalWrite(LED_GR, HIGH);
    digitalWrite(LED_YE, HIGH);
    delay(200);
    digitalWrite(LED_GR, LOW);
    digitalWrite(LED_YE, LOW);
    delay(200);
  }
}
void game_failed() {
  for(int i = 0; i < 2; i++){
    digitalWrite(LED_RE, HIGH);
    digitalWrite(LED_BL, HIGH);
    digitalWrite(LED_GR, HIGH);
    digitalWrite(LED_YE, HIGH);
    delay(200);
    digitalWrite(LED_RE, LOW);
    digitalWrite(LED_BL, LOW);
    digitalWrite(LED_GR, LOW);
    digitalWrite(LED_YE, LOW);
    delay(200);
    digitalWrite(LED_RE, HIGH);
    digitalWrite(LED_YE, HIGH);
    delay(200);
    digitalWrite(LED_RE, LOW);
    digitalWrite(LED_YE, LOW);
    delay(200);
    digitalWrite(LED_BL, HIGH);
    digitalWrite(LED_GR, HIGH);
    delay(200);
    digitalWrite(LED_BL, LOW);
    digitalWrite(LED_GR, LOW);
    delay(200);
  }
}
void game_win() {
  for(int i = 0; i < 2; i++){
    digitalWrite(LED_RE, HIGH);
    delay(10);
    digitalWrite(LED_RE, LOW);
    delay(10);
    digitalWrite(LED_BL, HIGH);
    delay(10);
    digitalWrite(LED_BL, LOW);
    delay(10);
    digitalWrite(LED_GR, HIGH);
    delay(10);
    digitalWrite(LED_GR, LOW);
    delay(10);
    digitalWrite(LED_YE, HIGH);
    delay(10);
    digitalWrite(LED_YE, LOW);
    delay(10);
  }
}
int check_all_buttons() {
  bool button_pressed = true; 
  while(button_pressed) {
    if(!digitalRead(BUTTON_1)) {
      return BUTTON_1;
    }
    else if(!digitalRead(BUTTON_2)) {
      return BUTTON_2;
    }
    else if(!digitalRead(BUTTON_3)) {
      return BUTTON_3;
    }
    else if(!digitalRead(BUTTON_4)) {
      return BUTTON_4;
    }
  }
}
bool right_button(int button, int in) {
  if(button == random_count[BUTTON_CODE][in]) {
    return true;
  }
  else {
    return false;
  }
}

void test() {
  showLED(10);
  gameReady();
  int button = check_all_buttons();
  if(right_button(button, 0)){
    digitalWrite(random_count[LED_CODE][0], HIGH);
    delay(1000);
    digitalWrite(random_count[LED_CODE][0], LOW);
  }
  else{
    game_failed();
  }
  delay(2000);
}

