#include <Adafruit_BME680.h>

Adafruit_BME680 bmeSensor;

namespace Timing
{
  volatile unsigned long lastRead = millis();
  const unsigned long readInterval = 2000;
}

void setup() 
{
  Serial.begin(9600);
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

  Serial.println("All good!");
}

void loop() 
{
  using namespace Timing;
  if(millis() - lastRead > readInterval)
  {
    if(!bmeSensor.performReading())
    {
      Serial.println("reading failed.");
      return;
    } 
    printReading();
    lastRead = millis();
  }
}

void printReading()
{
  Serial.print("Temperature: ");
  Serial.print(bmeSensor.temperature);
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(bmeSensor.humidity);
  Serial.println(" %");
}
