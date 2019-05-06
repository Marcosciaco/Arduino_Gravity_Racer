#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SD.h>
#include <SPI.h>
#include "dht.h"

//===================================Screen=================================
static const int RXPin = 0;
static const int TXPin = 1;
//=================================GPS-Speed=================================
TinyGPSPlus gps;
double Lat = 0;
double Long = 0;
int num_sat = 0;
double gps_speed = 0;
static const uint32_t GPSBaud = 9600;
//=================================TMP=======================================
dht DHTR;
dht DHTL;
static const int sensorPinR = A1;
static const int sensorPinL = A2;
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
//==================================SD=======================================
File myFile;
static const int RXPing = 10;
static const int TXPing = 13;
static const int pinCS = 53;
SoftwareSerial ss(RXPing, TXPing);


//=================================Setup=====================================
void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  pinMode(53, OUTPUT);
  if (SD.begin()) {
  }
}

//=================================Loop======================================
void loop() {
  //setup Sensors
  DHTR.read11(A0);
  DHTL.read11(A1);
  //get the GPS Data
  getGPS();
  //send all the collected Data to the Screen
  sendScreen();
  //send all the collected Data to the SD
  sendtoSD();
}

//makes a smart delay which only activates if needed
static void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

//gets the Date and Time, the location and Speed from the GPS
void getGPS() {
  //number of satellites
  num_sat = gps.satellites.value();
  if (gps.location.isValid() == 1) {
    //get Location and Speed
    Lat = gps.location.lat();
    Long = gps.location.lng();
    gps_speed = gps.speed.kmph();
  }
  if (gps.date.isValid()) {
    //get Date
    days = gps.date.day();
    months = gps.date.month();
    years = gps.date.year();
  }
  if (gps.time.isValid()) {
    //get Time
    hours = gps.time.hour();
    minutes = gps.time.minute();
    seconds = gps.time.second();
  }
  //Delay to avoid unnecessary waiting
  smartDelay(1000);
}

//Send all the collected Data to the Sreen
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

  String s1 = ".";
  String s = (hours+2) + s1 + minutes + s1 + seconds;
  Serial.print("time.txt=");
  Serial.print("\"");
  Serial.print(s);
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
//send all the collected Data to tthe SD
void sendtoSD() {
    myFile = SD.open("test.txt",FILE_WRITE);
    if (myFile) {
      myFile.print(days);
      myFile.print(".");
      myFile.print(months);
      myFile.print(".");
      myFile.print(years);
      myFile.print(",");
      myFile.print(hours+2);
      myFile.print(".");
      myFile.print(minutes);
      myFile.print(".");
      myFile.print(seconds);
      myFile.print(";");
      myFile.print(tmpr);
      myFile.print(";");
      myFile.print(tmpl);
      myFile.print(";");
      myFile.print(hml);
      myFile.print(";");
      myFile.print(hmr);
      myFile.print(",");
      myFile.print(gps_speed);
      myFile.print(",");
      myFile.print(Lat);
      myFile.print(",");
      myFile.print(Long);
      myFile.print(",");
      myFile.println(";");
      myFile.close();
    }
}
