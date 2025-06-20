#include "LiquidCrystal_I2C.h"

#define SPEAKER_PIN A0
#define BUTTON_PIN 2
#define LED_PIN 3

#define PRESS_ROUNDS 3

LiquidCrystal_I2C lcd(0x27,  16, 2);

uint32_t loops = 0;
uint32_t score = 0;
uint32_t pressRoundsLeft = PRESS_ROUNDS;
bool started = false;
bool flash = false;
unsigned long lastFlash = 0;
unsigned long lastPress = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  reset();
}

void reset() {
  score = 0;
  loops = 0;
  pressRoundsLeft = PRESS_ROUNDS;
  started = false;

  lcd.clear();
  lcd.print("Press to start");
}

void printScore() {
  char textBuf[16];
  snprintf(textBuf, sizeof(textBuf), "Score: %i", score);
  lcd.print(textBuf);
}

void updateScore() {
  lcd.clear();
  printScore();
}

void start() {
  lcd.clear();
  lcd.print("Good luck!");

  for(int i = 0; i < 4; i++) {
    tone(SPEAKER_PIN, 200 * i);
    delay(100);
  }

  delay(100);
  noTone(SPEAKER_PIN);
  
  delay(1000);

  lcd.clear();
  digitalWrite(LED_PIN, LOW);
  updateScore();
  started = true;
}

bool isButtonPressed() {
  return !digitalRead(BUTTON_PIN);
}

void delayAndProcess(unsigned long ms) {
  for(int i = 0; i < ms; i++) {
    if(isButtonPressed()) {
      noTone(SPEAKER_PIN);
      delay(10);
      
      int rmPoints = 20;

      lcd.clear();
      lcd.print("Not yet!");
      
      if(score > rmPoints) {
        lcd.setCursor(0, 1);
        lcd.print("-20 points");
        score -= rmPoints;
      }

      for(int i = 1; i <= 3; i++) {
        tone(SPEAKER_PIN, 500 / i);
        delay(200);
      }

      noTone(SPEAKER_PIN);
      
      delay(1000);
      updateScore();
    }

    if(random(0, 10000) == 0 && (millis() > lastPress + 3000)) {
      beginPress();
    }

    if(!started) {
      return;
    }

    delay(1);
  }
}

void beginPress() {
  pressRoundsLeft -= 1;

  lastPress = millis();
  unsigned long duration = 1000 * random(1, 3);

  digitalWrite(LED_PIN, HIGH);

  tone(SPEAKER_PIN, 1000);
  delay(10);
  noTone(SPEAKER_PIN);

  while(millis() < lastPress + duration) {
    if(isButtonPressed()) {
      uint32_t points = (duration - (millis() - lastPress)) / 10;
      score += points;

      lcd.clear();
      lcd.print("Nice!");

      char textBuf[16];
      lcd.setCursor(0, 1);
      snprintf(textBuf, sizeof(textBuf), "+%i points!", points);
      lcd.print(textBuf);      

      for(size_t i = 0; i < 3; i++) {
        tone(SPEAKER_PIN, 500 * i);
        delay(100);
      }

      noTone(SPEAKER_PIN);
    }

    delay(1);
  }

  digitalWrite(LED_PIN, LOW);
  updateScore();
  delay(100);

  if(pressRoundsLeft == 0) {
    lcd.clear();
    lcd.print("Time's up!");    
    lcd.setCursor(0, 1);
    printScore();

    unsigned long del = 200;

    for(size_t i = 0; i < 4; i++) {
      tone(SPEAKER_PIN, 300 * i);
      delay(del);
    }
    noTone(SPEAKER_PIN);
    delay(del);
    tone(SPEAKER_PIN, 300);
    delay(del);
    tone(SPEAKER_PIN, 1200);
    delay(del * 2);
    noTone(SPEAKER_PIN);

    delay(3000);
    reset();
  }
}

void loop() {
  unsigned long time = millis();

  if(!started) {
    if(time > lastFlash + 500) {
      flash = !flash;
      lastFlash = time;
      digitalWrite(LED_PIN, flash);
    }

    if(isButtonPressed()) {
      start();
    }

    return;
  };

  loops++;

  int toneDelay = 200;
  size_t iterations = 1;

  if(loops % 4 == 0) {
    toneDelay = 50;
    iterations = 2;
  }

  for(int i = 0; i < iterations; i++) {
      tone(SPEAKER_PIN, 300);
      delayAndProcess(toneDelay);

      noTone(SPEAKER_PIN);
      delayAndProcess(toneDelay);
      
      tone(SPEAKER_PIN, 200);
      delayAndProcess(toneDelay);
      
      noTone(SPEAKER_PIN);
      delayAndProcess(toneDelay);
  }
}
