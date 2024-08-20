#include "rover.h"
#include <NewServo.h>
#include <NewPing.h>

void moving()
{
  using namespace Timing;
  if((millis() - lastMove > motorInterval) && (distance > 15))
  {
    moveMotor();
    lastMove = millis();
  }

  if(millis() - lastPulse > pulseInterval)
  {
    pingSensor.ping_timer(pulseCheck);
    lastPulse = millis();
  }
}

void moveMotor()
{
  static bool sweepFlag = true; 
  static byte motorPos = 90;

  if(motorPos >= 170)
    sweepFlag = false;
  if(motorPos <= 10)
    sweepFlag = true;
  
  sweepFlag ? (++motorPos) : (--motorPos);
  
  servMotor.move(motorPos);
}

void pulseCheck()
{
  if(pingSensor.check_timer())
  {
    if(pingSensor.ping_result == 0)
    {
      distance = 400;
    }
    else
    {
      distance = pingSensor.ping_result / US_ROUNDTRIP_CM;
    }
  }
}