#ifndef NPK_H
#define NPK_H

#include <SoftwareSerial.h>
#include "Wire.h"

SoftwareSerial mod(0, 0);

class NPK
{
  private:
  byte DE = 0;
  byte RE = 0;
  byte RO = 0;
  byte DI = 0;

  const byte nitro[8] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
  const byte phos[8] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
  const byte pota[8] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
  
  byte values[11];

  public:
  byte potasRead = 0;
  byte nitroRead = 0;
  byte phosRead = 0; 

  NPK(byte pinDE, byte pinRE, byte pinRO, byte pinDI);

  void begin();

  void nitrogen();
 
  void phosphorous();
 
  void potassium();
};

#endif