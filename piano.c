#include <Arduino.h>
#include "pitches.h"

#define E4  2
#define DS4 3
#define D4  4
#define C4  5
#define B3  6
#define A3  7
#define E3  8
#define C3  9

int buzz = 11;

void setup() {
  Serial.begin(9600);
  pinMode(E4, INPUT_PULLUP);
  pinMode(DS4, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(C4, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(E3, INPUT_PULLUP);
  pinMode(C3, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
}

void loop() {
  if(!digitalRead(E4))
    tone(buzz, NOTE_E4, 10);

  if(!digitalRead(DS4))
    tone(buzz, NOTE_DS4, 10);

  if(!digitalRead(D4))
    tone(buzz, NOTE_D4, 10);

  if(!digitalRead(C4))
    tone(buzz, NOTE_C4, 10);

  if(!digitalRead(B3))
    tone(buzz, NOTE_B3, 10);

  if(!digitalRead(A3))
    tone(buzz, NOTE_A3, 10);

  if(!digitalRead(E3))
    tone(buzz, NOTE_E3, 10);

  if(!digitalRead(C3))
    tone(buzz, NOTE_C3, 10);

}
