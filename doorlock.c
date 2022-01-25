#include <Arduino.h>
#include <Keypad.h>
#include <string.h>
#include <SPI.h>
#include <MFRC522.h>

// definiramo broj redova i stupaca keypad-a
#define ROWS 4
#define COLS 4

// definiramo promjenjive pinove za RFID citac
#define RST_PIN 9
#define SS_PIN  10

// definiramo znakove na keypad-u kako bi arduino znao koju tipku smo stisnuli
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte redovi[ROWS] = { A0, A1, A2, A3 }; // definiramo pinove gdje spajamo redove keypad-a
byte stupci[COLS] = { A4, A5, 2, 3 }; // definiramo pinove gdje spajamo stupce keypad-a

// kreiramo varijablu "keypad" koja predstavlja nas key-pad
Keypad keypad = Keypad(makeKeymap(keys), redovi, stupci, ROWS, COLS);

// varijable potrebne za keypad
char unos[6] = {};
int brojac = 0;

// pinovi za statusne ledice
int red = 5;
int green = 7;
// pin za buzzer
int buzz = 4;

// kreiramo varijablu RFID citaca
MFRC522 mfrc522(SS_PIN, RST_PIN);
// varijabla u koju cemo citat UID kartice
String uid = "";
// varijabla za spremanje kartica
String kartice[6] = {};
int broj_kartica = 0;
// zastavica za dodavanje nove kartice
bool addCardFlag = false;

// inicijalizacija funkcija
// funkcije za RFID
void procitaj();
void dodaj_karticu();
int kartica_postoji(String uid);
// funkcije za keypad
void keypadEvent(KeypadEvent key);
void unesi(char key);
void provjera();
void otvori();

// SETUP
void setup() {
  Serial.begin(9600);
  SPI.begin();

  // Inicijaliziraj RFID citac
  mfrc522.PCD_Init();

  // definiraj modove pinova
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzz, OUTPUT);

  analogWrite(red, 30);
  keypad.addEventListener(keypadEvent);
}

// LOOP
void loop() {
  keypad.getKey();

  if(addCardFlag) {
    digitalWrite(red, !digitalRead(red));
    delay(200);
  }

  // ako nema kartice, ne idi dalje
  if(!mfrc522.PICC_IsNewCardPresent()) return;
  if(!mfrc522.PICC_ReadCardSerial()) return;
  // procitaj karticu
  procitaj();
}

// funkcija za parsiranje podataka kartice (izvlacimo UID)
void procitaj() {
  uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  if(!addCardFlag) {
    if(kartica_postoji(uid)) otvori();
  } else
    dodaj_karticu();
}

// funkcija za dodavanje nove kartice
void dodaj_karticu() {
  // ako kartica vec postoji, posalji zvucni signal da je vec unesena
  if(kartica_postoji(uid)){
    tone(buzz, 700, 25);
    delay(150);
    tone(buzz, 700, 25);
    return;
  }
  // dodaj UID kartice u niz
  kartice[broj_kartica] = uid;
  // povecaj broj kartica u nizu
  broj_kartica++;
  // oznaci uspjesno dodavanje kartice zvucnim i svjetlosnim signalom
  for(int i = 0; i < 4; i++) {
    tone(buzz, 700, 25);
    digitalWrite(green, HIGH);
    delay(75);
    digitalWrite(green, LOW);
    delay(75);
  }
}

// funkcija za provjeru kartice (je li kartica registrirana)
int kartica_postoji(String uid) {
  for(int i = 0; i < broj_kartica; i++)
    if(kartice[i] == uid)
      return 1;
  return 0;
}

// funkcija koja se pozove kad se pritisne botun na keypad-u
void keypadEvent(KeypadEvent key){
  switch(keypad.getState()) {
    // ako je botun samo pritisnut
    case PRESSED:
      unesi(key);
      break;
    // ako je botun pusten
    case RELEASED:
      if(key == '*') {
        addCardFlag = false;
        ocisti_unos();
        analogWrite(red, 30);
      }
      break;
    // ako se botun drzi prisnut (duze od sekunde)
    case HOLD:
      if(key == '*')
        addCardFlag = true;
      break;
    // inace nemoj napravit nista
    default:
      break;
  }
}

// funkcija za unos brojeva u niz
void unesi(char key) {
  // dodaj stisnuti broj u niz
  unos[brojac] = key;
  // odsviraj biiip
  tone(buzz, 700, 100);
  // ako je brojac sad na 4, to znaci da smo unijeli 5 brojeva
  // i onda treba provjeriti je li lozinka ispravna
  if(brojac == 4) {
    provjera();
    // nakon provjere postavimo niz ponovno na 0
    ocisti_unos();
    // izademo iz funkcije prije nego sta se izvrsi brojac++;
    return;
  }
  brojac++;
}

// funkcija za provjeru lozinke
void provjera() {
  if(strcmp(unos, "1234#") == 0) {
    otvori();
  } else {
    tone(buzz, 700, 100);
    delay(150);
    tone(buzz, 700, 100);
    delay(150);
    tone(buzz, 700, 100);
  }
}

// funkcija za otvaranje vrata (zvucni + svjetlosni signali)
void otvori() {
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  tone(buzz, 700, 400);
  delay(1000);
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
}

// funkcija za brisanje unesenih brojeva (ukoliko se pogresno stisne)
void ocisti_unos() {
  memset(unos, 0, sizeof unos);
  // vratimo brojac na 0
  brojac = 0;
}
