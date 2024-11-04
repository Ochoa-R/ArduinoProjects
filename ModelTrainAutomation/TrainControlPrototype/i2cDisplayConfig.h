#ifndef I2C_DISPLAY
#define I2C_DISPLAY
//Config for Liquid Crystal Display w/ I2C Module
#include <LiquidCrystal_I2C.h>
// LCD
LiquidCrystal_I2C display(0x27, 16, 2);
// LCD Initialization
void displayStart()
{
  display.init();
  display.backlight();
  display.clear();
  display.setCursor(0, 0);
}
#endif