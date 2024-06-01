#include <Keypad.h>
#include <LiquidCrystal.h>

#define green 1
#define red 2
#define buzzer 5
#define pir 3

String kcode;
String kstore;

bool ARMED = false;
const byte codeLength = 4;
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// KEYPAD
byte rowPins[ROWS] = {A15, A14, A13, A12}; 
byte colPins[COLS] = {A11, A10, A9, A8};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// LCD DISPLAY
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
LiquidCrystal marie(rs, en, d4, d5, d6, d7);

void setup(){
  marie.begin(16, 2);
  marie.clear();
  marie.setCursor(5,0);
  marie.print("READY");
  marie.setCursor(3, 1);
  marie.print("ENTER CODE");
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(green, HIGH);
}
  
void loop(){
  getCode();
  if(!kcode.equals(kstore))
  {
    marie.setCursor(3, 0);
    marie.print("WRONG CODE");
    marie.setCursor(3, 1);
    marie.print("TRY AGAIN");
    kcode.remove(0, codeLength);
  }
  else
  {
    marie.setCursor(3, 0);
    marie.print("RIGHT CODE");
    marie.setCursor(4, 1);
    kcode.remove(0, codeLength);
    changeArmState();
  }
  
}

void getCode()
{
  while(true)
  {
    char key = keypad.getKey();
    if(key && kcode.length() < codeLength && key != '#')
    {
      marie.cursor();
      marie.clear();
      tone(buzzer, 3000, 100);
      kcode += key;
      marie.print(kcode);
    }
    if(key == '#' && kcode.length() == codeLength)
    {
      if(kstore.length() == 0)
        kstore = kcode;
      marie.clear();
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
  if(ARMED)
  {
    if(digitalRead(pir))
    {
      tone(buzzer, 4000, 2000);
      marie.clear();
      marie.setCursor(3,0);
      marie.print("TRIGGER ALERT!");
    }
  }
}
