//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define ledGreen 3
#define ledRed 2
#define button 6

//for Standard LCD:
/*const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
LiquidCrystal marie(rs, en, d4, d5, d6, d7);*/

//For LCD w/ I2C:
LiquidCrystal_I2C marie(0x27, 16, 2);

const byte ROWS = 4; 
const byte COLS = 4; 
char userIn[4] = {0, 0, 0, 0};
char codeCheck[4] = {'1', '2', '3', '4'};
static uint8_t index = 0;
static bool codeState = false;
static bool alarmState = false;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {A8, A9, A10, A11}; 
byte colPins[COLS] = {A12, A13, A14, A15}; 

Keypad callie = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

bool isRightCode(uint8_t index);

bool getKeys(uint8_t index);

const char bald[] = {'B', 'A', 'L', 'D', '!'};

void setup() 
{
  //marie.begin(16, 2);
  marie.init();
  marie.backlight();
  marie.clear();
  marie.setCursor(0, 0);
  //marie.print("Time passed:");
  
  DDRH = (0<<PH3);
  DDRE = (1<<PE4)|(1<<PE5);
  PORTH = (1<<PH3);
  PORTE = 0x00;

  marie.print("Set alarm code");
  delay(1000);
  alarmState = getKeys(index, codeCheck);
  marie.clear();
  marie.print("Alarm set");
  PORTE = 0x20;
}

void loop()
{
  if(!digitalRead(button) && alarmState)
  {
    marie.clear();
    marie.print("ALARM TRIPPED!");
    alarmState = false;
    PORTE = 0x10;
    delay(1000);
    while(true)
    {
      getKeys(index, userIn);
      if(!isRightCode(index))
      {
        marie.clear();
        marie.setCursor(0,0);
        marie.print("Wrong code");
        delay(1000);
        continue;
      }
      else
      {
        marie.clear();
        marie.setCursor(0,0);
        marie.print("Correct Code");
        PORTE = 0x20;
        alarmState = true;
        delay(1000);
        marie.clear();
        marie.print("Alarm set");
        break;
      }
    }
  }
}

bool isRightCode(uint8_t index)
{
  for(;index < 4;++index)
  {
    if(userIn[index] != codeCheck[index])
    {
      return false;
    }
    userIn[index] = 0;
  }
  return true;
}

bool getKeys(uint8_t index, char arr[])
{
  marie.clear();
  marie.setCursor(0,0);
  marie.print("Enter Code:");
  marie.setCursor(0,1);
  while(true)
  {
  char customKey = callie.getKey();
  marie.cursor();
    if(customKey && (customKey != '#') && (index < 4))
    {
      marie.print(customKey);
      arr[index] = customKey;
      index = ++index;
    }
    if(customKey == '#')
    {
      marie.clear();
      marie.setCursor(0,0);
      marie.noCursor();
      index = 0;
      return true;
    }
  }
}