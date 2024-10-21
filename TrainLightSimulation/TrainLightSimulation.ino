#include <NewPing.h>

namespace Pins
{
const byte red = A15;
const byte green = A11;
const byte yellow = A8;
const byte echo = A1;
const byte trig = A0;
}

namespace Timing
{
  const byte yellowDuration = 200;
  const byte pingDuration = 20;
  unsigned long lastYellow = yellowDuration;
  unsigned long lastPing = pingDuration;
}

NewPing pingSense(Pins::trig, Pins::echo, 400);
unsigned int distance = 0;

void setup() 
{
  DDRK = 0x89;
  DDRF = 0x01;
  /*Utilizing writing to the port, here's the pinMode() equivalent
    using namespace Pins
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);*/
}

void loop() 
{
  static bool ledState = 0;

  using namespace Timing;
  if(millis() > lastYellow)
  {
    ledState = !ledState;
    digitalWrite(Pins::yellow, ledState);
    lastYellow += yellowDuration;
  }
  if(millis() > lastPing)
  {
    pingSense.ping_timer(pingCheck);
    lastPing += pingDuration;
  }
  if(distance >= 25)
  {
    digitalWrite(Pins::green, HIGH);
    digitalWrite(Pins::red, LOW);
  }
  else 
  {
    digitalWrite(Pins::green, LOW);
    digitalWrite(Pins::red, HIGH);
  }
}

void pingCheck()
{
  if(pingSense.check_timer())
  {
    distance = pingSense.ping_result / US_ROUNDTRIP_CM;
  }
}
