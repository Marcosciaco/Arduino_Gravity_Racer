#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SD.h>
#include <SPI.h>
#include "dht.h"

static const int RXPin = 0;
static const int TXPin = 1;
//=================================GPS-Speed=================================
double Lat = 0;
double Long = 0;
int num_sat = 0;
double gps_speed = 0;
//=================================TMP=======================================
dht DHTR;
dht DHTL;

float hml = 0;
float hmr = 0;
float tmpl = 0;
float tmpr = 0;
//=================================Time======================================
int days;
int months;
int years;
int hours;
int minutes;
int seconds;

SoftwareSerial ss(RXPin, TXPin);
File file;
TinyGPSPlus gps;

static const int pinCS = 53;
static const uint32_t GPSBaud = 9600;
static const int potPin = A0;
static const int sensorPinR = A1;
static const int sensorPinL = A2;
static const int togglePinSP = 8;
static const int togglePinSD = 11;

void setup() {
  ss.begin(GPSBaud);
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  pinMode(togglePinSP, INPUT);
  pinMode(togglePinSD, INPUT);
  if (SD.begin()) {
    file = SD.open("test.txt", FILE_WRITE);
  }
}

void loop() {
  DHTR.read11(A0);
  DHTL.read11(A1);
  getGPS();
  sendtoSD();
  sendScreen();
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void getGPS() {
  num_sat = gps.satellites.value();
  if (gps.location.isValid() == 1) {
    Lat = gps.location.lat();
    Long = gps.location.lng();
    gps_speed = gps.speed.kmph();
  }
  if (gps.date.isValid()) {
    days = gps.date.day();
    months = gps.date.month();
    years = gps.date.year();
  }
  if (gps.time.isValid()) {
    hours = gps.time.hour();
    minutes = gps.time.minute();
    seconds = gps.time.second();
  }
  smartDelay(1000);
}

void sendScreen() {
  //========================================TMP-to-Screen========================================
  tmpl = DHTL.temperature;
  tmpr = DHTR.temperature;
  hmr = DHTR.humidity;
  hml = DHTL.humidity;

  Serial.print("tmplt.txt=");
  Serial.print("\"");
  Serial.print(tmpl);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("tmprt.txt=");
  Serial.print("\"");
  Serial.print(tmpr);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("hml.txt=");
  Serial.print("\"");
  Serial.print(hml);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("hmr.txt=");
  Serial.print("\"");
  Serial.print(hmr);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  //========================================Speed========================================
  Serial.print("sat.txt=");
  Serial.print("\"");
  Serial.print(num_sat);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("spdt.txt=");
  Serial.print("\"");
  Serial.print(gps_speed);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("spd.val=");
  Serial.print(gps_speed);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void sendtoSD() {
  if (digitalRead(togglePinSD) == HIGH) {
    if (file) {
      file.print(days);
      file.print(".");
      file.print(months);
      file.print(".");
      file.print(years);
      file.print(".");
      file.print(hours);
      file.print(".");
      file.print(minutes);
      file.print(".");
      file.print(seconds);
      file.print(",");
      file.print(tmpr);
      file.print(",");
      file.print(tmpl);
      file.print(",");
      file.print(hml);
      file.print(",");
      file.print(hmr);
      file.print(",");
      file.print(gps_speed);
      file.print(",");
    }
  } else {
    if (digitalRead(togglePinSD) == LOW) {
      file.close();
    }
  }
}
