#include <TimerOne.h>

// --- Oyun değişkenleri ---
int randomNumber, oldNumber;
int skor = 0;
bool gameOver = false;
unsigned long gameStartTime;
unsigned long lastMillis = 0;
unsigned long changeInterval = 1000; // varsayılan

// --- Zorluk Butonu ---
const int difficultyButtonPin = 22; // ayrı seçim butonu

// --- LED ve Buton pinleri ---
const int numLEDs = 9;
int ledPins[numLEDs]    = {2, 3, 4, 5, 6, 7, 8, 9, 10};   // LED pinleri
int buttonPins[numLEDs] = {23,24,25,26,27,28,29,30,31};   // Buton pinleri

// --- Zorluk ayarları ---
void setDifficulty(int diff) {
  if (diff == 1) changeInterval = 1500;
  else if (diff == 2) changeInterval = 1000;
  else if (diff == 3) changeInterval = 500;
}

// --- Spiral efekti ---
void spiralEffect() {
  for (int round = 0; round < 3; round++) {
    for (int i = 0; i < numLEDs; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(70);
      digitalWrite(ledPins[i], LOW);
    }
  }
}

// --- Skor göstergesi ---
void showScore(int score) {
  int showCount = score;
  if (showCount > 9) showCount = 9;
  if (showCount < 0) showCount = 0;

  for (int i = 0; i < showCount; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
    digitalWrite(ledPins[i], LOW);
  }
}

// --- Zorluk seçimi millis ile (revize) ---
void chooseDifficulty() {
  int currentChoice = 1;
  unsigned long lastPress = 0;
  bool buttonHeld = false;

  // Tüm LED’leri kapat ve ilk LED’i yak
  for (int i = 0; i < numLEDs; i++) digitalWrite(ledPins[i], LOW);
  digitalWrite(ledPins[currentChoice - 1], HIGH);

  while (true) {
    unsigned long now = millis();

    // Butona basılmış mı?
    if (digitalRead(difficultyButtonPin) == LOW) {
      if (!buttonHeld) {
        buttonHeld = true;
        lastPress = now;

        // Zorluğu artır
        currentChoice++;
        if (currentChoice > 3) currentChoice = 1;

        // LED göstergesini güncelle
        for (int i = 0; i < numLEDs; i++) digitalWrite(ledPins[i], LOW);
        digitalWrite(ledPins[currentChoice - 1], HIGH);

        // Zorluğu set et
        setDifficulty(currentChoice);
      }
    } else {
      buttonHeld = false;
    }

    // Başlangıç için LED ve skor sıfırlama
    if (now - lastPress > 100) { // 100 ms debounce
      skor = 0;
      gameStartTime = millis();
      gameOver = false;
      lastMillis = millis();
      break; // oyun başlasın
    }
  }
}

// --- LED değiştirme ---
void nextLED() {
  digitalWrite(ledPins[randomNumber], LOW);
  randomNumber = random(0, numLEDs);
  digitalWrite(ledPins[randomNumber], HIGH);
  oldNumber = randomNumber;
}

// --- Setup ---
void setup() {
  randomSeed(analogRead(0));
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(difficultyButtonPin, INPUT_PULLUP);

  chooseDifficulty();
}

// --- Loop ---
void loop() {
  unsigned long now = millis();

  if (!gameOver) {
    if (now - gameStartTime >= 30000) {
      // oyun bitti
      gameOver = true;
      for (int i = 0; i < numLEDs; i++) digitalWrite(ledPins[i], LOW);

      // efekt ve skor göster
      spiralEffect();
      showScore(skor);
      delay(1000);

      // tekrar zorluk seçimi
      chooseDifficulty();
    }

    // led değiştirme
    if (now - lastMillis >= changeInterval) {
      nextLED();
      lastMillis = now;
    }

    // doğru butona basma kontrolü
    if (digitalRead(buttonPins[randomNumber]) == LOW) {
      digitalWrite(ledPins[randomNumber], LOW);
      skor++;
      delay(150); // tuş debouncing
      nextLED();
      lastMillis = now;
    }
  }
}
