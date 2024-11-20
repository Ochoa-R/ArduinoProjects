// LEDs
enum Directions
{
  left = A0,
  up = A1,
  right = A2,
  down = A3,
  none = 0
};
const Directions LEDs[] = {left, up, right, down};
const byte winLight{A6};
const byte loseLight{A7};

// Stick
const byte stickY{A8};
const byte stickX{A9};

// Game conditions
//volatile bool gameState{false};
//volatile bool winState{false};
const unsigned int timeLimit{3000};

// Game storage
volatile Directions pattern[4] = {};
volatile Directions playerInput[4] = {};

void setup() 
{
  DDRK = 0x00;
  DDRF = 0xFF;
  Serial.begin(9600);
}

void loop() 
{
  createPattern();
  if(getPlayerIn())
    for(byte count{0};count < 3;++count)
      blinkLED(winLight);
  else
    for(byte count{0};count < 3;++count)
      blinkLED(loseLight);
}

void createPattern()
{
  for(byte count{0};count < 4;++count)
  {
    byte pick = LEDs[random(0,4)];
    blinkLED(pick);
    pattern[count] = pick;
  }
}

bool getPlayerIn()
{
  for(byte count{0};count < 4;++count)
  {
    byte tilt{};
    unsigned long timer{millis() + timeLimit};
    while(true)
    {
      if(millis() > timer)
        return false;
      if(moveCheck() != none)
      {
        tilt = moveCheck();
        break;
      }
      continue;
    }
    blinkLED(tilt);
    playerInput[count] = tilt;
    if(playerInput[count] != pattern[count])
      return false;
    while(moveCheck() != none)
      ;
  }
  return true; 
}

void blinkLED(Directions pin)
{
  digitalWrite(pin, HIGH);
  delay(200);
  digitalWrite(pin, LOW);
}

Directions moveCheck()
{
  if(analogRead(stickX) >= 600)
    return right;
  if(analogRead(stickX) <= 60)
    return left;
  if(analogRead(stickY) >= 600)
    return down;
  if(analogRead(stickY) <= 60)
    return up;
  return none; 
}

/*void printStickReading(Directions way)
{
  switch(way)
  {
    case left:
      Serial.println("left");
      return;
    case right:
      Serial.println("right");
      return;
    case up:
      Serial.println("up");
      return;
    case down:
      Serial.println("down");
      return;
    default:
      Serial.println("error: no read");
      return;
  }
}*/

// void clearGameLEDs() { PORTF &= 0xF0; }
