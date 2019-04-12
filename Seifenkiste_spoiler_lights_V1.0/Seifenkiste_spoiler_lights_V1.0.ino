#include <Servo.h>

int potVal = 0;
int angle = 0;
static const int potPin = A0;
==========================================RGB======================================
int rVal = 255;
int gVal = 0;
int bVal = 127;

int rDir = -1;
int gDir = 1;
int bDir = -1;

const int rPin = 11;
const int gPin = 10;
const int bPin = 9;
==========================================SPR======================================
Servo servo;

void setup() {
 servo.attach(9);
 pinMode(,OUTPUT);
 pinMode(,OUTPUT);
 pinMode(,OUTPUT);
}

void loop() {
  potspoiler();
  rgb();
}

void potspoiler() {
  potVal = analogRead(potPin);
  angle = map(potVal, 0,1023,0,61);
  servo.write(angle);
  delay(15);
}

void rgb(){
  
  analogWrite(rPin, rVal);
  analogWrite(gPin, gVal);
  analogWrite(bPin, bVal);

  rVal = rVal + rDir;
  gVal = gVal + gDir;
  bVal = bVal + bDir;

  if (rVal >= 255 || rVal <= 0) {
    rDir = rDir * -1;
  }

  if (gVal >= 255 || gVal <= 0) {
    gDir = gDir * -1;
  }

  if (bVal >= 255 || bVal <= 0) {
    bDir = bDir * -1;
  }
  delay(10);
}
