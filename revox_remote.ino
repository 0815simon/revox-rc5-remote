#include <Arduino.h>
#include <IRremoteESP8266.h> //using IRremote.h when using Arduino
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WebServer.h>


int kRecvPin = 14; //Empfänger Pin D5 bei Wemos
int outPin = 15; // Digitalpin 8 bei Wemos


IRrecv irrecv(kRecvPin);
decode_results results;

//Hier die IR-Codes, bei mir Billig-Fernbedienung, Codes wechselten im laufenden Betrieb
const int RC5_POWER = 0xBAD;
const int RC5_POWER1 = 0x3AD;

const int RC5_STOP = 0xB99;
const int RC5_STOP1 = 0x399;

const int RC5_PLAY = 0xB8B;
const int RC5_PLAY1 = 0x38B;

const int RC5_VOLUP = 0x392;
const int RC5_VOLUP1 = 0xB92;

const int RC5_VOLDOWN = 0xB93;
const int RC5_VOLDOWN1 = 0x393;

const int RC5_CD = 0xBB2;
const int RC5_CD1 = 0x3B2;

const int RC5_TV = 0x3B1;
const int RC5_TV1 = 0xBB1;

const int RC5_AUX = 0x3B9;
const int RC5_AUX1 = 0xBB9;

const int RC5_PHONO = 0xBBA;
const int RC5_PHONO1 = 0x3BA;

const int RC5_MUTE = 0xBB0;
const int RC5_MUTE1 = 0x3B0;

const int RC5_BACK = 0x394;
const int RC5_BACK1 = 0xB94;

const int RC5_OPEN = 0x38D;
const int RC5_OPEN1 = 0xB8D;

void setup() {
  // put your setup code here, to run once:
pinMode(outPin, OUTPUT);
Serial.begin(115200);
 irrecv.enableIRIn();  // Start the receiver
while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);

 // Disable WIFI-Mode
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  Serial.print("Revox-Empfänger ist bereit");

}

void loop() {
  if (irrecv.decode(&results)) {
    //Anzeigen der Fernbedienungs-Codes
    serialPrintUint64(results.value, HEX); 
    Serial.println("");

    switch(results.value) {
      case RC5_POWER:
      case RC5_POWER1:
        Serial.println("Power");
        senden("1000", "111111");
      break;
    }

    switch(results.value) {
      case RC5_PLAY:
      case RC5_PLAY1:
        Serial.println("Play");
        senden("0000", "000100");
      break;
    }
    
    switch(results.value) {
      case RC5_STOP:
      case RC5_STOP1:
        Serial.println("Stop");
        senden("0000", "000110");
      break;
    }

        switch(results.value) {
      case RC5_VOLUP:
      case RC5_VOLUP1:
        Serial.println("VOLUP");
        senden("1000", "100010");
      break;
    }

        switch(results.value) {
      case RC5_VOLDOWN:
      case RC5_VOLDOWN1:
        Serial.println("VOLDOWN");
        senden("1000", "000010");
      break;
    }

        switch(results.value) {
      case RC5_CD:
      case RC5_CD1:
        Serial.println("CD");
        senden("1000", "001010");
      break;
    }

        switch(results.value) {
      case RC5_AUX:
      case RC5_AUX1:
        Serial.println("Aux");
        senden("1000", "001100");
      break;
    }

        switch(results.value) {
      case RC5_TV:
      case RC5_TV1:
        Serial.println("TV");
        senden("1000", "111100");
      break;
    }

        switch(results.value) {
      case RC5_PHONO:
      case RC5_PHONO1:
        Serial.println("Phono");
        senden("1000", "101100");
      break;
    }

        switch(results.value) {
      case RC5_MUTE:
      case RC5_MUTE1:
        Serial.println("Mute");
        senden("1000", "110100");
      break;
    }

        switch(results.value) {
      case RC5_BACK:
      case RC5_BACK1:
        Serial.println("Back");
        senden("0000", "100011");
      break;
    }

            switch(results.value) {
      case RC5_OPEN:
      case RC5_OPEN1:
        Serial.println("Open");
        senden("0000", "011110");
      break;
    }
    irrecv.resume();  // Receive the next value
  }
  delay(100);
} 



//Verarbeitung als String, ist leider als Hex-Code nicht sauber gelaufen :-(
void senden(String adresse, String ITTcode){ //Adresse entweder 1000 (primär) oder 0000 (sekundär)
digitalWrite(outPin, LOW);  // Pin auf LOW setzen
 digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden Impuls
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
  delayMicroseconds(430);      // Startpuls

digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
  delayMicroseconds(135);      // ebenfalls Startpuls

//////////////////////Adresse --> normal bei 1. 135 ms, 2. Adresse alles bei 285 ms

char Zeichen[10]; //Arrayvariable vorbereiten zum einzeln ablesen
adresse.toCharArray(Zeichen, 8);   

for (byte i = 0; i < 4; i = i + 1) {
  //Serial.println(Zeichen[i]);
  
 digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen

switch (Zeichen[i]){
  case '0':
  delayMicroseconds(280);      
  break;

  case '1':
  delayMicroseconds(130);      
  break;
}
}  // Ende Adressierung 1000=Bank1, oder 0000=Bank2 möglich

ITTcode.toCharArray(Zeichen, 8);
for (byte i = 0; i < 6; i = i + 1) {
  digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen

switch (Zeichen[i]){
  case '0':
  delayMicroseconds(280);      // 280 Mikrosekunden warten
  break;
  case '1':
  delayMicroseconds(130);      // 130 Mikrosekunden warten
  break;
}
} //ende ITTcode-Ausführung

//Abschlusscode
digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
  delayMicroseconds(435);      // 435 Mikrosekunden warten
digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
}
