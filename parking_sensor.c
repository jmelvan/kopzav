#include <Arduino.h>

// pinovi za pomacni registar (koristi se kako bi smanjili broj pinova za ledice sa 8 na 3)
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

// pinovi za ultrasonicki senzor
int trigPin = 2;
int echoPin = 3;

// varijable koje se koriste za logiku programa
long duration;
int distance;
int last_index = 0;
int new_index = 0;

// pin za buzzer (zvucnik)
int buzzer = 4;

// bitovima prikazan broj upaljenih ledica (0 do 8 ledica) i njihov redoslijed
int arr[9] = {
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00111111,
  0b01111111,
  0b11111111,
};

void setup() {
  // definiranje tipa pinova za pomacni registar
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // definiranje tipa pinova za senzor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // definiranje tipa pina za buzzer
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // logika za senzor
  // postavimo "trigger pin" na stanje bez impulsa
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  // salje se impuls na "trigger pin", taj impuls traje 5 mikrosekundi
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  // ponovno vratimo "trigger pin" na stanje prije impulsa, nakon  sta je proslo 10 mikrosekundi
  digitalWrite(trigPin, LOW);

  // pulseIn je funkcija koja ceka na "echo pin" odnosno ceka da se zvuk odbije i vrati te mjeri vrijeme koje je proslo
  duration = pulseIn(echoPin, HIGH);
  // udaljenost se dobije na nacin da se vrijeme trajanja (od senzora do objekta i nazad) pomnozi sa brzinom zvuka te onda podijeli sa 2 (jer nas zanima udaljenost do objekta a ne i natrag)
  distance = duration * 0.0343 / 2;

  // uvjeti za udaljenosti (npr. ako je udaljenost manja od 30cm, itd.)
  if(distance <= 30){
    // new index oznacava broj upaljenih ledica
    new_index = 8;
    // upalimo buzzer (broj 1 oznacava stalno sviranje, a ne bip bip)
    bzzr(1);
  } else if (distance > 30 && distance <= 35){
    new_index = 7;
    // broj 150 oznacava pauzu izmedu 2 bip bip u milisekundama
    bzzr(150);
  } else if (distance > 35 && distance <= 40){
    new_index = 6;
    bzzr(250);
  } else if (distance > 40 && distance <= 45){
    new_index = 5;
    bzzr(350);
  } else if (distance > 45 && distance <= 50){
    new_index = 4;
    bzzr(450);
  } else if (distance > 50 && distance <= 55){
    new_index = 3;
    bzzr(550);
  } else if (distance > 55 && distance <= 60){
    new_index = 2;
    bzzr(700);
  } else if (distance > 60 && distance <= 65){
    new_index = 1;
    bzzr(900);
  } else {
    new_index = 0;
    bzzr(0);
  }

  // ako postoji promjena u broj ledica koje tribaju svitlit, prikazi novi broj ledica priko pomacnog registra
  if(last_index != new_index) {
    // treba postaviti "latch pin" na LOW kako bi mogli poslati novu vrijednost na pomacni registar
    digitalWrite(latchPin, LOW);
    // saljemo nove podatke na pomacni registar
    shiftOut(dataPin, clockPin, MSBFIRST, arr[new_index]);
    // ponovno zakljucamo unos u pomacni registar tako sta stavimo "latch pin" na HIGH
    digitalWrite(latchPin, HIGH); 
    // stavimo last index da je trenutni index kako bi ponovno mogli provjeravati postoji li promjena
    last_index = new_index;
  }

  // napravimo kratki delay od 50 milisekundi
  delay(50);
}

// funkcija za bip bip
void bzzr(int speed){
  digitalWrite(buzzer, LOW);
  if(speed == 0) return;
  
  static unsigned long time = millis();
  
  if(millis() - time >= speed){
    time = millis();
    digitalWrite(buzzer, HIGH);
  }
}
