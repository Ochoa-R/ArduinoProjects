#include "rover.h"
#include "NPK.h"
#include <Adafruit_BME680.h>

void stopped()
{
  unsigned int water = checkWater();

  checkNPK();

  if(!bmeSensor.performReading())
  {
    Serial.println("Failed to perform reading.");
  }

  printReading(water);
}

unsigned int checkWater()
{
  delay(1000);
  unsigned int waterLevel = analogRead(Pins::water);
  return waterLevel;
}

void checkNPK()
{
  npkSense.nitrogen();
  delay(100);
  npkSense.phosphorous();
  delay(100);
  npkSense.potassium();
}

void printReading(unsigned int& water)
{
  Serial.print("Water level: ");
  Serial.println(water);

  Serial.print("Temperature: ");
  Serial.print(bmeSensor.temperature * 1.8 + 32);
  Serial.println(" *F");
  Serial.print("Humidity: ");
  Serial.print(bmeSensor.humidity);
  Serial.println(" %");

  Serial.print("Nitrogen: ");
  Serial.println(npkSense.nitroRead);
  Serial.print("Phosphorous: ");
  Serial.println(npkSense.phosRead);
  Serial.print("Potassium: ");
  Serial.println(npkSense.potasRead);
}
