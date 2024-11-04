#ifndef REG_DISPLAY
#define REG_DISPLAY
//Config for Liquid Crystal Display w/o I2C Module
#include <LiquidCrystal.h>
// LCD Pins
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
// LCD
LiquidCrystal display(rs, en, d4, d5, d6, d7);
// LCD Initialization
void displayStart()
{
  display.begin();
  display.clear();
}
#endif