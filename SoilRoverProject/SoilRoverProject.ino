/*Reads temperature and humidity every 2 seconds while a servo motor sweeps back and forth
  if an object gets too close to the ultrasonic ping sensor, the motor ceases sweeping
  once the object leaves the set distance, motor returns sweeping. proof of concept for
  the motors on the chassis of the rover

  TO DO:
    Connect and configure the soil moisture sensor
    Connect and configure the NPK Sensor*/


#include <Adafruit_BME680.h>
#include <NewPing.h>
#include <NewServo.h>
#include "roverVaribles.h"

Adafruit_BME680 bmeSensor;
NewServo servMotor(Pins::motor);
NewPing pingSensor(Pins::trigger, Pins::echo, 400); 

volatile unsigned int distance;

void setup() 
{
  using namespace Pins;
  Serial.begin(9600);
  while(!Serial)
    ;

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  servMotor.setMax(170);
  servMotor.setMin(10);
  servMotor.setInit(90);

  bmeSensor.begin();
  if(!bmeSensor.begin())
  {
    Serial.println("failed to connect. check wiring");
    while(true);
  }
  bmeSensor.setTemperatureOversampling(BME680_OS_8X);
  bmeSensor.setHumidityOversampling(BME680_OS_2X);
  bmeSensor.setPressureOversampling(BME680_OS_4X);
  bmeSensor.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bmeSensor.setGasHeater(320, 150);

  servMotor.begin();
  servMotor.goInit();
  Serial.println("All good!");
}

void loop() 
{
  using namespace Timing;

  if((millis() - lastMove > motorInterval) && (distance > 15))
  {
    moveMotor();
    lastMove = millis();
  }

  if(bmeSensor.beginReading() == 0)
  {
    Serial.println("reading failed to begin.");
    return;
  }

  if(millis() - lastRead > readInterval)
  {
    if(!bmeSensor.endReading())
    {
      Serial.println("reading failed to end.");
      return;
    } 
    printReading();
    lastRead = millis();
  }

  if(millis() - lastPulse > pulseInterval)
  {
    pingSensor.ping_timer(pulseCheck);
    lastPulse = millis();
  }
}

void printReading()
{
  Serial.print("Temperature: ");
  Serial.print(bmeSensor.temperature * 1.8 + 32);
  Serial.println(" *F");

  Serial.print("Humidity: ");
  Serial.print(bmeSensor.humidity);
  Serial.println(" %");
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
    distance = pingSensor.ping_result / US_ROUNDTRIP_CM;
  }
}
