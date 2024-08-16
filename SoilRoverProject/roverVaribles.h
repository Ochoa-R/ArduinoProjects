namespace Timing
{
  volatile unsigned long lastRead = 0; 
  volatile unsigned long lastMove = 0;
  volatile unsigned long lastPulse = 0;
  volatile unsigned long lastWater = 0;
  const unsigned int readInterval = 2000;
  const byte pulseInterval = 200;
  const byte motorInterval = 10;
  const unsigned int waterInterval = 2000;
}

namespace Pins
{
const byte water = A7;
const byte trigger = A1;
const byte echo = A2;
const byte led = A6;
const byte motor = 6;
}
