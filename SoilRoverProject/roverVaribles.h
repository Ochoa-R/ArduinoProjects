#ifndef ROVER_VAR_H
#define ROVER_VAR_H

namespace Timing
{
  volatile unsigned long lastMove = 0;
  volatile unsigned long lastPulse = 0;
  const byte pulseInterval = 200;
  const byte motorInterval = 10;
}

namespace Pins
{
const byte water = A7;
const byte trigger = A1;
const byte echo = A2;
const byte led = A6;
const byte motor = 6;
const byte toggle = 9;
const byte RE = 8;
const byte DE = 7;
const byte RO = 10;
const byte DI = 11; 
}

void moving();
void stopped();
void moveMotor();
void pulseCheck();
void checkNPK();
void printReading(unsigned int& water);
unsigned int checkWater();

#endif
