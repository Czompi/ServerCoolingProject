
#include <Arduino.h>
#define DEGREE (byte)0
#define RPM (byte)1
#define INTAKE (byte)2
#define EXHAUST (byte)3
#define AMBIENT (byte)4
byte degChar[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte rpmChar[] = {
  B00000,
  B01100,
  B00101,
  B11011,
  B10100,
  B00110,
  B00000,
  B00000
};
byte intakeChar[] = {
  B01111,
  B00100,
  B10000,
  B01000,
  B10000,
  B00100,
  B01111,
  B00000
};
byte exhaustChar[] = {
  B11110,
  B01000,
  B00010,
  B00001,
  B00010,
  B01000,
  B11110,
  B00000
};

byte ambientChar[] = {
  B00000,
  B01000,
  B00100,
  B01000,
  B00000,
  B11110,
  B00100,
  B00000
};
