#include <Keypad.h>
#include <LiquidCrystal.h>

#define green 1
#define red 2
#define buzzer 5
#define pir 3

String kcode;
String kstore;

volatile bool ARMED = false;
const byte codeLength = 4;
const byte ROWS = 4; 
const byte COLS = 4; 

// KEYPAD
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A15, A14, A13, A12}; 
byte colPins[COLS] = {A11, A10, A9, A8};
Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD DISPLAY
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
LiquidCrystal marie(rs, en, d4, d5, d6, d7);

void setup(){
  // INITIALIZE DISPLAY
  marie.begin(16, 2);
  marie.clear();

  // LED AND BUZZER PINS OUTPUT
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // SET CODE FOR ARM/DISARM
  marie.setCursor(0,0);
  marie.print("READY");
  marie.setCursor(0, 1);
  marie.print("SET CODE");
  getCode(kstore);
  tone(buzzer, 3500, 350);
  marie.print("CODE SET");
  marie.setCursor(0, 1);
  marie.print("ENTER TO ARM");
  digitalWrite(green, HIGH);
}
  
void loop()
{
  getCode(kcode);
  if(!kcode.equals(kstore))
  {
    tone(buzzer, 2500, 350);
    marie.print("WRONG CODE");
    marie.setCursor(0, 1);
    marie.print("TRY AGAIN");
    kcode.remove(0, codeLength);
  }
  else
  {
    tone(buzzer, 3500, 350);
    marie.print("RIGHT CODE");
    marie.setCursor(0, 1);
    kcode.remove(0, codeLength);
    changeArmState();
  }
  
}

void getCode(String& code)
{
  while(true)
  {
    char key = keypad.getKey();
    if(key && code.length() < codeLength && key != '#')
    {
      tone(buzzer, 3000, 100);
      marie.cursor();
      marie.clear();
      code += key;
      marie.print(code);
    }
    if(key == '#' && code.length() == codeLength)
    {
      marie.clear();
      marie.setCursor(0,0);
      marie.noCursor();
      return;
    }
  }
}

void changeArmState()
{
  ARMED = !ARMED;
  if(ARMED)
  {
    marie.print("ARMED");
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    attachInterrupt(digitalPinToInterrupt(pir), motion, RISING);
  }
  else
  {
    marie.print("DISARMED");
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    detachInterrupt(digitalPinToInterrupt(pir));
  }
}

void motion()
{
  if(digitalRead(pir))
  {
    tone(buzzer, 4000, 1000);
    marie.clear();
    marie.setCursor(0,0);
    marie.print("MOTION DETECTED");
  }
}
