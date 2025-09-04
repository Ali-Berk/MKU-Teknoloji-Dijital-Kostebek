#include <TimerOne.h>

int randomNumber,oldNumber;
volatile bool interruptEnabled = false;
int skor = 0;

//led ve pin bağlantısını objelerle sağlama
class LED {
  private:
    int pin;
    int buttonPin;

  public:
    LED(int ledPin, int btnPin) { 
      pin = ledPin;
      buttonPin = btnPin;

      pinMode(pin, OUTPUT);
      pinMode(buttonPin, INPUT_PULLUP);

    }
};

// LED ve buton pinlerini tanımlama
const int numLEDs = 2;
int ledPins[numLEDs] = {0, 2};   // LED'ler için kullanılacak pinler
int buttonPins[numLEDs] = {1, 3}; // Butonlar için kullanılacak pinler
// LED nesne dizisini oluşturma
LED leds[numLEDs] = { LED(0, 1), LED(2,3) };

//ledleri açıp kapatacak olan fonksiyon
void turnOn_Off(){

  randomNumber = random(0,2);
  if (!(digitalRead(buttonPins[randomNumber]) == 0) & interruptEnabled == false) {
    digitalWrite(ledPins[randomNumber], LOW);
    delay(70);
    digitalWrite(ledPins[randomNumber], HIGH);
    activateInterrupt();
    oldNumber = randomNumber;
  }
  else if((digitalRead(buttonPins[randomNumber]) == 0)){
    interruptEnabled = false;
    digitalWrite(ledPins[randomNumber], LOW);
    skor++;
    Timer1.detachInterrupt();
    Timer1.initialize(1000000);
  }
  
}

//kesme kontrolü
void activateInterrupt(){
  if (!interruptEnabled){ 
  interruptEnabled = true;
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(LedInterrupt);
  }
  else {
    interruptEnabled = false;
  }
}

void setup() {
  randomSeed(analogRead(0));
  
  
}

void loop() {
  turnOn_Off();
}

// led açıldığında sayım yapacak ve ledi kapatıp devre dışı kalacak kesme
void LedInterrupt(){
  if (interruptEnabled) {
    digitalWrite(ledPins[oldNumber],LOW);
    activateInterrupt(); 
    Timer1.detachInterrupt();
    Timer1.initialize(1000000);
    skor--;
  }
}
