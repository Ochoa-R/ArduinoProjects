/*While in a moving state, motor sweeps back and forth unless Ultrasonic Ping Sensor detects an object getting
too close. While in stopped state, reads a temperature and humidity, water, and NPK Sensor. 3-Pin Toggle switch 
is used to switch between the two states in the prototype stage*/

#include "Adafruit_BME680.h"
#include <NewServo.h>
#include <NewPing.h>
#include "rover.h"
#include "NPK.h"

Adafruit_BME680 bmeSensor;
NewServo servMotor(Pins::motor);
NewPing pingSensor(Pins::trigger, Pins::echo, 400); 
NPK npkSense(Pins::RE, Pins::DE, Pins::RO, Pins::DI);

volatile unsigned int distance;

void setup() 
{
  using namespace Pins;
  Serial.begin(9600);
  while(!Serial)
    ;

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(water, INPUT);
  pinMode(toggle, INPUT_PULLUP);

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

  npkSense.begin();

  servMotor.begin();
  servMotor.goInit();
  Serial.println("All good!");
}

void loop() 
{
  if(digitalRead(Pins::toggle) == 0)
  {
    pinMode(Pins::motor, OUTPUT);
    moving();
  }
  else
  {
    pinMode(Pins::motor, INPUT_PULLUP);
    stopped();
  }
}
