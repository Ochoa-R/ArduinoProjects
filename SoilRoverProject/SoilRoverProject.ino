/*Reads temperature and humidity every 2 seconds while a servo motor sweeps back and forth
  if an object gets too close to the ultrasonic ping sensor, the motor ceases sweeping
  once the object leaves the set distance, motor returns sweeping. proof of concept for
  the motors on the chassis of the rover
  
  TO DO: Figure out how to pulse the Ultrasonic Sensor asychronously, otherwise it messes
  with timing of the motor and temp/hum sensor*/


#include <Adafruit_BME680.h>
#include <NewServo.h>
#include "roverVaribles.h"

Adafruit_BME680 bmeSensor;
NewServo servMotor(Pins::motor);

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
  //static bool safeDistance;
  using namespace Timing;

  if((millis() - lastMove > motorInterval))// && safeDistance == true)
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

  /*if(millis() - lastPulse > pulseInterval)
  {
    unsigned int distanceCm = pulseRead() / 29 / 2;
    (distanceCm < 15) ? (safeDistance = false) : (safeDistance = true);
  }*/
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

unsigned long pulseRead()
{
  digitalWrite(Pins::trigger, HIGH);
  delayMicroseconds(1000);
  digitalWrite(Pins::trigger, LOW);

  return pulseIn(Pins::echo, HIGH);
}
