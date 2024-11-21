// LEDs
enum Directions
{
  left = A0,
  up = A1,
  right = A2,
  down = A3,
  none = 0
};
const Directions LEDs[4] = {left, up, right, down};
const byte winLight{A6};
const byte loseLight{A7};

//  LED Delays
const unsigned long winBlink{100};
const unsigned long inBlink{150};
const unsigned long patBlink{300};

// Stick
const byte stickY{A8};
const byte stickX{A9};

// Game conditions
//volatile bool gameState{false};
volatile bool winState{};
const unsigned int timeLimit{3000};
const byte stageLimit{8};

// Game storage
volatile Directions pattern[stageLimit] = {};
volatile Directions playerInput[stageLimit] = {};

void setup() 
{
  DDRK = 0x00;
  DDRF = 0xFF;
  Serial.begin(9600);
}

void loop() 
{
  winState = true;
  byte lossCount{0};
  for(byte stage{4};stage <= stageLimit;)
  {
    createPattern(stage);
    if(getPlayerIn(stage))
    {
      stage = ++stage;
      blinkLED(winLight, 2, winBlink);
    }
    else
    {
      blinkLED(loseLight, 2, winBlink);
      lossCount += 1;
    }
    clearPattern(stage);
    if(lossCount >= 3)
    {
      winState = false;
      break;
    }
  }
  delay(500);
  if(!winState)
    blinkLED(loseLight, 4, winBlink);
  else
    blinkLED(winLight, 4, winBlink);
}

void createPattern(byte currentStage)
{
  for(byte count{0};count < currentStage;++count)
  {
    byte pick = LEDs[random(0,4)];
    blinkLED(pick, 1, patBlink);
    pattern[count] = pick;
  }
}

void clearPattern(byte currentStage)
{
  for(byte count{0};count < currentStage;++count)
  {
    pattern[count] = 0;
    playerInput[count] = 0;
  }
}

bool getPlayerIn(byte currentStage)
{
  for(byte count{0};count < currentStage;++count)
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
    blinkLED(tilt, 1, inBlink);
    playerInput[count] = tilt;
    if(playerInput[count] != pattern[count])
      return false;
    while(moveCheck() != none)
      ;
  }
  return true; 
}

void blinkLED(Directions pin, byte blinkTimes, unsigned long time)
{
  for(byte count{0};count < blinkTimes;++count)
  {
    digitalWrite(pin, HIGH);
    delay(time);
    digitalWrite(pin, LOW);
    delay(time);
  }
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
