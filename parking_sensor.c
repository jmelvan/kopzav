int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int trigPin = 2;
int echoPin = 3;

long duration;
int distance;
int last_index = 0;
int new_index = 0;

int buzzer = 4;

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
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;

  if(distance <= 30){
    new_index = 8;
    bzzr(1);
  } else if (distance > 30 && distance <= 35){
    new_index = 7;
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

  if(last_index != new_index) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, arr[new_index]);
    digitalWrite(latchPin, HIGH); 
    last_index = new_index;
  }
  delay(50);
}

void bzzr(int speed){
  digitalWrite(buzzer, LOW);
  if(speed == 0) return;
  
  static unsigned long time = millis();
  
  if(millis() - time >= speed){
    time = millis();
    digitalWrite(buzzer, HIGH);
  }
}
