//If you put callie and marie together, you get calamari :D

//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define pir 4

const uint8_t codeLength = 4;
static bool alarmState = true;
const byte green = 0x20;
const byte red = 0x10;
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
String userIn;
String codeCheck;

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
Keypad callie(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

bool isRightCode();
void getKeys(String& calamari, uint8_t cap);
void setAlarm();
void wrongCode();
void rightCode();

void setup() 
{
  //INITIALIZE LCD DISPLAY
  //marie.begin(16, 2);
  marie.init();
  marie.backlight();
  marie.clear();
  marie.setCursor(0, 0);

  //G INPUT AND PULLUP, E OUTPUT
  DDRG = (0<<PG5);
  DDRE = (1<<PE4)|(1<<PE5);
  PORTG = (1<<PG5);
  PORTE = 0x00;

  // SET CODE TO RESET ALARM WHEN TRIGGERED
  marie.print("Set alarm code");
  delay(1000);
  marie.clear();
  marie.setCursor(0,0);
  marie.print("Put 4-digit code");
  getKeys(codeCheck, codeLength);
  marie.clear();
  marie.print("Code Set");
  PORTE = green;
  delay(1000);
  
}

void loop()
{
  //CHECK IF ALARM IS NOT SET, ENABLE ALARM VIA * PRESS
  if(alarmState)
  {
    marie.clear();
    marie.print("Press * to set");
    while(true)
    {
      if(callie.getKey() == '*')
      {
        setAlarm();
        break;
      }
    }
  }
  // PIR SENSOR TO DETECT MOTION, CHECK IF ALARM IS SET
  if(digitalRead(pir) && !alarmState)
  {
    // SET ALARM AS TRIGGERED
    marie.clear();
    marie.print("ALARM TRIPPED!");
    alarmState = true;
    delay(1000);

    // LOOP UNTIL THE CORRECT CODE IS INPUTTED, THEN DISARM ALARM
    while(true)
    {
      marie.clear();
      marie.setCursor(0,0);
      marie.print("Put 4-digit code");
      getKeys(userIn, codeLength);
      if(!isRightCode())
      {
        wrongCode();
        continue;
      }
      else
      {
        rightCode();
        break;
      }
    }
  }
}

bool isRightCode()
{
  return userIn.equals(codeCheck);
}

void getKeys(String& calamari, uint8_t cap)
{
  marie.setCursor(0,1);
  while(true)
  {
  char customKey = callie.getKey();
  marie.cursor();
    if(customKey && (customKey != '#') && (calamari.length() < cap))
    {
      marie.print(customKey);
      calamari += customKey;
    }
    if((customKey == '#') && (calamari.length() == cap))
    {
      marie.clear();
      marie.setCursor(0,0);
      marie.noCursor();
      return;
    }
  }
}

void wrongCode()
{
  marie.clear();
  marie.setCursor(0,0);
  userIn.remove(0, 4);
  marie.print("Wrong code");
  delay(1000);
}

void rightCode()
{
  marie.clear();
  marie.setCursor(0,0);
  userIn.remove(0, 4);
  marie.print("Correct code");
  PORTE = green;
  alarmState = true;
  delay(1000);
}

void setAlarm()
{
  marie.clear();
  marie.print("Alarm set");
  PORTE = red;
  alarmState = false;
}
