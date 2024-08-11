namespace Timing
{
  volatile unsigned long lastRead = 1; 
  volatile unsigned long lastMove = 1;
  volatile unsigned long lastPulse = 1;
  const unsigned int readInterval = 2000;
  const byte pulseInterval = 200;
  const byte motorInterval = 10;
}

namespace Pins
{
const byte trigger = A1;
const byte echo = A2;
const byte led = A6;
const byte motor = 6;
}
