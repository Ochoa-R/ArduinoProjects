// If you put callie and marie together, you get calamari :D
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// MACROS FOR SENSORS AND SPEAKER PINS
#define pirSig 4
#define soundSig 39
#define speak 5

// DETECTING WHICH METHOD TRIGGERED ALARM
byte whichDetect = 0;
const byte motion = 1;
const byte sound = 2;
const byte codeFail = 3;

// HANDLING ALARM STATE AND INCORRECT CODE INPUT
bool alarmState = true;
bool fail = false;
byte chances = 0;

// CODE STORAGE AND COMPARE STRINGS
String userIn;
String codeCheck;

// MISCELLANEOUS CONSTANTS
const byte codeLength = 4;
const byte green = 0x20;
const byte red = 0x10;
const byte ROWS = 4; 
const byte COLS = 4;

// KEYPAD LAYOUT
char hexaKeys[ROWS][COLS] = 
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// KEYPAD PINS
byte rowPins[ROWS] = {A8, A9, A10, A11}; 
byte colPins[COLS] = {A12, A13, A14, A15};

// LCD W/ I2C & KEYPAD INITIALIZATION
LiquidCrystal_I2C marie(0x27, 16, 2);
Keypad callie(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// FUNCTION DECLARATIONS
bool isRightCode();
bool isAlert();
void getKeys(String& calamari);
void askSet();
void setAlarm();
void tripAlarm();
void wrongCode();
void rightCode();
void beepPress();

void setup() 
{
  // INITIALIZE LCD DISPLAY
  marie.init();
  marie.backlight();
  marie.clear();
  marie.setCursor(0, 0);

  // SENSOR PINS INPUT AND PULLUP, LED AND BUZZER PINS OUTPUT
  DDRG = (0<<PG2)|(0<<PG5);
  DDRE = (1<<PE1)|(1<<PE3)|(1<<PE4)|(1<<PE5);
  PORTG = (1<<PG2)|(1<<PG5);
  PORTE = 0x00;

  // SET CODE TO RESET ALARM WHEN TRIGGERED
  marie.print("Set alarm code");
  delay(1000);
  getKeys(codeCheck);
  marie.clear();
  marie.print("Code Set");
  PORTE = green;
  delay(1000);
}

void loop()
{
  // CHECK IF ALARM IS NOT SET, ENABLE ALARM VIA * PRESS
  if(alarmState)
  {
    askSet();
    while(true)
    {
      getKeys(userIn);
      if(!userIn.equals(codeCheck))
      {
        wrongCode();
        chances = ++chances;
        if(chances >= 3)
        {
          alarmState = false;
          fail = true;
          break;
        }
        continue;
      }
      else
      {
        rightCode();
        setAlarm();
        break;
      }
    }
  }
  if(isAlert())
  {
    // SET ALARM AS TRIGGERED
    tripAlarm();
    askSet();

    // LOOP UNTIL THE CORRECT CODE IS INPUTTED, THEN DISARM ALARM
    while(true)
    {
      getKeys(userIn);
      if(!userIn.equals(codeCheck))
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

bool isAlert()
{
  if(!alarmState)
  {
    if(digitalRead(pirSig))
      whichDetect = motion;
    /*if(digitalRead(soundSig))
      whichDetect = sound;*/
    if(fail)
      whichDetect = codeFail;

    switch(whichDetect)
    {
    case motion:
      marie.clear();
      marie.print("MOTION DETECTED!");
      return true;
    case sound:
      marie.clear();
      marie.print("SOUND DETECTED");
      return true;
    case codeFail:
      marie.clear();
      marie.print("TOO WRONG!");
      return true;
    default:
      return false;
    }
  }
}


void getKeys(String& calamari)
{
  marie.clear();
  marie.setCursor(0,0);
  marie.print("Put 4-digit code");
  marie.setCursor(0,1);
  while(true)
  {
  char customKey = callie.getKey();
  marie.cursor();
    if(customKey && (customKey != '#') && (calamari.length() < codeLength))
    {
      beepPress(2000, 50);
      marie.print(customKey);
      calamari += customKey;
    }
    if((customKey == '#') && (calamari.length() == codeLength))
    {
      beepPress(1750, 250);
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
  if(alarmState)
    tone(speak, 500, 750);
  delay(1000);
}

void rightCode()
{
  noTone(speak);
  marie.clear();
  marie.setCursor(0,0);
  userIn.remove(0, codeLength);
  marie.print("Correct code");
  PORTE = green;
  alarmState = true;
  chances = 0;
  whichDetect = 0;
  tone(speak, 3000, 750);
  delay(1000);
  marie.clear();
}

void setAlarm()
{
  marie.clear();
  marie.print("Alarm set");
  PORTE = red;
  alarmState = false;
}

void tripAlarm()
{
  fail = false;
  PORTE = red;
  tone(speak, 4000);
  delay(1000);
}

void askSet()
{
  marie.setCursor(0,1);
  marie.print("Push * to");
  if(!alarmState)
    marie.print(" disarm");
  else
    marie.print(" arm"); 
  while(true)
  {
    if(callie.getKey() == '*')
    {
      beepPress(1500, 250);
      return;
    }
  }
}

void beepPress(uint32_t freq, uint32_t duration)
{
  if(alarmState)
  {
  tone(speak, freq, duration);
  }
}
