//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define ledGreen 3
#define ledRed 2
#define pir 6

static uint8_t index = 0;
static bool codeState = false;
static bool alarmState = false;
const byte ROWS = 4; 
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A8, A9, A10, A11}; 
byte colPins[COLS] = {A12, A13, A14, A15}; 
char userIn[4] = {0, 0, 0, 0};
char codeCheck[4] = {'1', '2', '3', '4'};

//for Standard LCD:
/*const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;*/

//For LCD w/ I2C:
LiquidCrystal_I2C marie(0x27, 16, 2);
Keypad callie(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

bool isRightCode();
bool getKeys(char arr[]);

void setup() 
{
  //INITIALIZE LCD DISPLAY
  //marie.begin(16, 2);
  marie.init();
  marie.backlight();
  marie.clear();
  marie.setCursor(0, 0);

  // H INPUT AND PULLUP, E OUTPUT
  DDRH = (0<<PH3);
  DDRE = (1<<PE4)|(1<<PE5);
  PORTH = (1<<PH3);
  PORTE = 0x00;

  // SET CODE TO RESET ALARM WHEN TRIGGERED
  marie.print("Set alarm code");
  delay(1000);
  getKeys(codeCheck);
  alarmState = true;
  marie.clear();
  marie.print("Alarm set");
  PORTE = 0x20;
}

void loop()
{
  // PIR SENSOR TO DETECT MOTION, CHECK IF ALARM IS ALREADY TRIGGERED
  if(digitalRead(pir) && alarmState)
  {
    // SET ALARM AS TRIGGERED, TURN ON RED LED
    marie.clear();
    marie.print("ALARM TRIPPED!");
    alarmState = false;
    PORTE = 0x10;
    delay(1000);

    // LOOP UNTIL THE CORRECT CODE IS INPUTTED
    while(true)
    {
      getKeys(userIn);
      if(!isRightCode())
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

bool isRightCode()
{
  for(;index < 4;++index)
  {
    if(userIn[index] != codeCheck[index])
    {
      return false;
    }
    userIn[index] = 0;
  }
  index = 0;
  return true;
}

bool getKeys(char arr[])
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
