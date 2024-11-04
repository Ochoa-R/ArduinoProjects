//Uncomment the right file for your LCD
#include "i2cDisplayConfig.h"
//#include "RegDisplayConfig.h"


// Pins
const byte ledPin{2};
const byte inputPin{A1};
// Conversion constants
const float analogTo5V{0.00488};
const float analogTo8Bit{0.249};
// Tracking variables
volatile byte lastVolt{};
const byte RWDelay{100};
unsigned long lastRW{RWDelay};

void setup() 
{
  // Pin setup
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  // LCD initialization
  displayStart();
}

void loop() 
{
  if(millis() > lastRW)
  {
    float voltRead{analogRead(inputPin)};
    if(voltRead != lastVolt)
    {
      display.clear();
      display.print("Voltage: ");
      display.print(voltRead * analogTo5V);
      lastVolt = voltRead;
    }
    unsigned int voltWrite{voltRead * analogTo8Bit};
    analogWrite(ledPin, voltWrite);
    lastRW += RWDelay;
  }
}
