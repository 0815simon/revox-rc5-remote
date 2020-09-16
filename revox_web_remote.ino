#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WebServer.h>

const char* ssid     = "my-SSID";
const char* password = "my-password";
ESP8266WebServer server(80);


int kRecvPin = 14; //Empfänger Pin D5 bei Wemos
int outPin = 15; // Digitalpin 8 bei wemos


IRrecv irrecv(kRecvPin);
decode_results results;

//Hier die IR-Codes
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



void handle_root() {

  String message = "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<title>TV Web Remote</title>\n"
  "<script>\n"
  "function pressButton(btn,value){\n"
  " var xmlhttp = new XMLHttpRequest();\n"
  " xmlhttp.onreadystatechange=function() {\n"
  "  if (xmlhttp.readyState==4 && xmlhttp.status==200) {\n"
  "   var response = xmlhttp.responseText;\n"
  "  }\n"
  " }\n"
  " xmlhttp.open('GET','/button?task=' + btn + '&value=' + value);\n"
  " xmlhttp.send();\n"
  " return false;\n"
  "}\n"
  "</script>\n"
  "</head>\n"
  "<body>\n"
  "<style type='text/css'>\n"
  "body {\n"
  "  padding-left:20px;\n"
  "  padding-right:20px;\n"
  "  margin: 0;\n"
  "  font-family: Geneva,'Lucida Sans','Lucida Grande',sans-serif;\n"
//  "  font-size: 30px;\n"
  "  font-size: 60px;\n"
  "  line-height: 40px;\n"
  "  color: #575757;\n"
  "  background-color: #fff;\n"
  "}\n"
  ".buttongroup {\n"
  " width:100%;\n"
  " max-width:1200px;\n"
  "}\n"
  ".span6 {\n"
  "  width:47%;\n"
  "}\n"
  ".span4 {\n"
  " width:22.0%;\n"
  "}\n"
  ".span12 {\n"
  " width:100%;\n"
  "}\n"
  ".span2 {\n"
  " width:100%;\n"
  "}\n"
  ".button {\n"
  "  border:2px solid #333;\n"
  "  padding-top:60px;\n"
  "  padding-bottom:60px;\n"
  "  margin:8px;\n"
  "  text-align:center;\n"
  "  font-weight:bold;\n"
  "  max-width:1200px;\n"
  "  float:left;\n"
  "  border-radius: 22px;\n"
  "  background-color: #eee;\n"
  "}\n"
  "a {\n"
  "  color: #333;\n"
  "}\n"
  ".buttonrow {\n"
  "  clear: both;\n"
  "}\n"
  ".power_btn {\n"
  "  background-color:#f88;\n"
  "}\n"
  ".select_btn {\n"
  "  background-color:#09f;\n"
  "}\n"
  ".number_btn {\n"
  " background-color:#aaa;\n"
  "}\n"
  ".channel_btn {\n"
  " background-color:#8f8;\n"
  "}\n"
  "</style>\n"
  "<div class='buttongroup span2'>\n"
  "<a href='#' onclick='pressButton(\"poweroff\");return false;'><div class='button power_btn span2'>OFF</div></a>\n"
  "</div>\n"
  
  
  "<div class='buttongroup span12'>\n"
  "<a href='#' onclick='pressButton(\"tv\");return false;'><div class='button select_btn span4'>TV</div></a>\n"
  "<a href='#' onclick='pressButton(\"cd\");return false;'><div class='button select_btn span4'>CD</div></a>\n"
  "<a href='#' onclick='pressButton(\"tape1\");return false;'><div class='button select_btn span4'>Tape1</div></a>\n"
  "<a href='#' onclick='pressButton(\"phono\");return false;'><div class='button select_btn span4'>Phono</div></a>\n"
  "</div>\n"
  "<div class='buttongroup span12'>\n"
  "<a href='#' onclick='pressButton(\"play\");return false;'><div class='button span6'>Play/Next</div></a>\n"
  "<a href='#' onclick='pressButton(\"stop\");return false;'><div class='button span6'>Stop</div></a>\n"
  "<a href='#' onclick='pressButton(\"open\");return false;'><div class='button span6'>Open</div></a>\n"
  "<a href='#' onclick='pressButton(\"back\");return false;'><div class='button span6'>Back</div></a>\n"
  "<a href='#' onclick='pressButton(\"voldn\");return false;'><div class='button span6'>Volume -</div></a>\n"
  "<a href='#' onclick='pressButton(\"volup\");return false;'><div class='button span6'>Volume +</div></a>\n"

  "</div>\n"

"<div class='buttongroup span2'>\n"
  "<a href='#' onclick='pressButton(\"mute\");return false;'><div class='button power_btn span2'>MUTE</div></a>\n"
  "</div>\n"
  "</body>\n"
  "</html>\n";
  server.send(200, "text/html", message);
  delay(100);
}

// button web page
void handle_button() {
  String message = "button send: ";
  String task = "";
  String value = "";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    if(server.argName(i) == "task")
    {
      task = server.arg(i);
    }
    if(server.argName(i) == "value")
    {
      value = server.arg(i);
    }
  }
  if(task == "mute")
  {
     message += "{mute}";
  }
  
  else if(task == "poweroff")
  {
     message += "{poweroff}";
     senden("1000", "111111");
     delay(100);
  }
  else if(task == "tv")
  {
     message += "{tv}";
     Serial.print("button pressed: TV Hurra!");
  }
   else if(task == "tape1")
  {
     message += "{tape1}";
  }
  else if(task == "volup")
  {
     message += "{volup}";
  
  }
  else if(task == "voldn")
  {
     message += "{voldn:}";
       delay(100);
     
  }
  Serial.print("button pressed: ");
  Serial.println(task);
  server.send(200, "text/plain", message);
  delay(100);
}

void setup() {
  // put your setup code here, to run once:
pinMode(outPin, OUTPUT);
Serial.begin(115200);
 irrecv.enableIRIn();  // Start the receiver
while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);

 // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rConnecting");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(0, HIGH);
    delay(500);
    digitalWrite(0, LOW);
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handle_root);
  server.on("/button", handle_button);
  server.begin();
  Serial.println("HTTP server started");
    
  Serial.println();
  Serial.print("Revox-Remote ist bereit");

}

void loop() {
server.handleClient();
  
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
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
        senden("0000", "100110");
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


void senden(String adresse, String ITTcode){
// Funktionsenden("1000", "111111"); //Adressierung 1000 oder 0000=sekundäre Adresse bei Revox

digitalWrite(outPin, LOW);  // Pin auf LOW setzen
 digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden Impuls
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
  delayMicroseconds(430);      // Startpuls

digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 20 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
  delayMicroseconds(135);      // ebenfalls Startpuls


char Zeichen[10]; //Arrayvariable vorbereiten zum einzeln ablesen
adresse.toCharArray(Zeichen, 8);   

for (byte i = 0; i < 4; i = i + 1) {
  //Serial.println(Zeichen[i]);
  
 digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 50 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen

switch (Zeichen[i]){
  case '0':
  delayMicroseconds(280);      // 280 Mikrosekunden warten
  break;

  case '1':
  delayMicroseconds(130);      // 130 Mikrosekunden warten
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
Serial.println("Code 0");
  break;
  case '1':
  delayMicroseconds(130);      // 130 Mikrosekunden warten
  break;
}
} //ende ITTcode-Ausführung

//Abschlusscode
digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 50 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
  delayMicroseconds(435);      // 50 Mikrosekunden warten
digitalWrite(outPin, HIGH); // Pin auf HIGH setzen
  delayMicroseconds(20);      // 50 Mikrosekunden warten
  digitalWrite(outPin, LOW);  // Pin auf LOW setzen
}
