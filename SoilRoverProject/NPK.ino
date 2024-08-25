/*Functions made using code from Puneeth Kumar
  https://www.makerguides.com/arduino-uno-and-npk-sensor-project/#Links*/

#include "NPK.h"

NPK::NPK(byte pinDE, byte pinRE, byte pinRO, byte pinDI)
{
  DE = pinDE;
  RE = pinRE;
  RO = pinRO;
  DI = pinDI;
}

void NPK::begin()
{
  mod = SoftwareSerial(RO, DI);
  mod.begin(9600);

  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
}

void NPK::nitrogen() 
{
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(nitro, sizeof(nitro)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
  }
  nitroRead = values[4];
}
 
void NPK::phosphorous() 
{
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(phos, sizeof(phos)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
  }
  phosRead = values[4];
}
 
void NPK::potassium()
{
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(pota, sizeof(pota)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
  }
  potasRead = values[4];
}
