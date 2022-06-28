#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
BlynkTimer timer;

// ================= CONNECT TO WIFI ================
char auth[] = "cV_jE-9udMq8vMd5XrR0Sjgn1LT0VY7D";   // Auth Token from Blynk App.
char ssid[] = "DEAZ";                               // WiFi ssid 
char pass[] = "araaraara";                          // WiFi pass

static double pH, amonia, ta, wtrTemp, ard;

/* LCD */
WidgetLCD lcd(V6);

/* Relay */
#define RLY1 D7
#define RLY2 D8

/* Fuzzy */
double A, B;
double urendah[] = {0, 0, 7.75};
double ubaik[] = {6.5, 7.75, 9};
double utinggi[] = {7.75, 14, 14};
double xbaik[] = {0, 0, 0.1};
double xsedang[] = {0, 0.3, 0.6};
double xburuk[] = {0.1, 1.6, 1.6};
double nol = 0;
double tp = 0.5;
double lp = 1;
double minr[10];
double Rule[10];

/* Pompa */
int cond;
double full = 5;  //cm

/* Auto */
int snsr = 0;
int pump = 0;

SoftwareSerial Wemos(D5, D6);

void setup() {
  Serial.begin  (115200);
  Wemos.begin(9600);
  Blynk.begin(auth, ssid, pass);    //Local server

  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);

  digitalWrite(RLY1, HIGH);
  digitalWrite(RLY2, HIGH); 
}

void loop() {
  Blynk.run();
  timer.run();
  
  //timer.setInterval(400L, serial);
  //timer.setInterval(400L, isi);
  //timer.setInterval(400L, fr);

  pompa();
  timer.setInterval(1000L, lcd1);
  timer.setInterval(10000L, lcd2);
}

void lcd1(){
  lcd.clear();
  lcd.print(0, 0, "Sistem Perawatan");
  lcd.print(1, 1, "Lobster Air Tawar");
}

void lcd2(){
  lcd.clear();
  if (cond = 0){            
    lcd.print(0, 0, "Kondisi Air Baik");
    lcd.print(1, 1, ":)");             
  }
  if (cond = 30){          
    lcd.print(0, 0, "Air Buruk");
    lcd.print(1, 1, "Ganti Air");                  
  }
  if (cond = 50){          
    lcd.print(0, 0, "Air Buruk");
    lcd.print(1, 1, "Ganti Air");                   
  }
}

/*
void fr() {
  if (snsr = 0) {
    fuzzy();
  }
  else if (snsr = 1) {
    relay();
  }
}
*/

void serial() {
  if (Wemos.available() > 0) {
    ard = Wemos.parseFloat();
      if (Wemos.read() == '\n') {
      //Serial.println(ard);
      if (ard >= 2000 && ard <= 3023) {
        pH = ard;
        pH = pH - 2000;
        Serial.print(" pH : ");         Serial.println(pH, 2);
        Blynk.virtualWrite(V0, pH);
      }
      if (ard >= 4000 && ard <= 5023) {
        amonia = ard;
        amonia = amonia - 4000;
        Serial.print(" Amonia : ");         Serial.println(amonia);
        Blynk.virtualWrite(V1, amonia);
      }
      if (ard >= 6000 && ard <= 7023) {
        ta = ard;
        ta = ta - 6000;
        Serial.print(" Distance : ");         Serial.println(ta, 1);
        Blynk.virtualWrite(V2, ta);
      }
      if (ard >= 8000 && ard <= 9023) {
        wtrTemp = ard;
        wtrTemp = wtrTemp - 8000;
        Serial.print(" Temperature : ");         Serial.println(wtrTemp, 1);
        Blynk.virtualWrite(V3, wtrTemp);
      }
    }
  }
}

void fuzzy() {
  /*
  if (Serial.available() > 0)           //Wait for user input
  { 
     float in = Serial.parseFloat();
     if (in >= 0 && in <= 14) {
      pH = in;
     } 
     else if (in > 100){
      amonia = in - 100;
     }
  }
  Serial.print("pH : ");
  Serial.print(pH);
  Serial.print("\tamonia : ");
  Serial.println(amonia);
  */

  double keputusan = defuzzyfikasi();
  String msg;
  Serial.print("Hasil Keputusan Defuzzyfikasi : ");
  Serial.println(keputusan, 1);
  if (keputusan >= 0 && keputusan < 0.5) {
    msg = "Air Baik";
    cond = 0;
  } 
  else if (keputusan >= 0.5 && keputusan < 1) {
    msg = "Air mohon diganti 30%";
    cond = 30;
    snsr = 1;
  } 
  else if (keputusan == 1) {
    msg = "Air mohon diganti 50%";
    cond = 50;
    snsr = 1;
  }
  Serial.println(msg);
  delay(5000);
}
