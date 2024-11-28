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
const byte winBlink{100};
const byte inBlink{150};
unsigned int patBlink{};

// Stick
const byte stickY{A8};
const byte stickX{A9};
const byte stickButt{A10};

// Game conditions
//volatile bool gameState{false};
enum Difficulty
{
  easy = 1,
  medium = 2,
  hard = 3
};
Difficulty difficulty[3] = {easy, medium, hard};
volatile bool winState{};
unsigned int timeLimit{};
const byte stageLimit{7};

// Game storage
volatile Directions pattern[stageLimit] = {};
volatile Directions playerInput[stageLimit] = {};

void setup() 
{
  DDRK = 0x00;
  PORTK = 0x04;
  DDRF = 0xFF;
  Serial.begin(115200);
}

void loop() 
{
  difficultySelect();
  winState = true;
  byte lossCount{0};
  for(byte stage{4};stage < stageLimit;)
  {
    Serial.println(stage);
    createPattern(stage);
    if(getPlayerIn(stage))
    {
      ++stage;
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
  {
    blinkLED(loseLight, 4, winBlink);
  }
  else
  {
    blinkLED(winLight, 4, winBlink);
  }
  delay(500);
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
    while(moveCheck() != none)
    {
      if(millis() > timer)
        return false;
    }
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
  {
    delay(10);
    return right;
  }
  if(analogRead(stickX) <= 60)
  {
    delay(10);
    return left;
  }
  if(analogRead(stickY) >= 600)
  {
    delay(10);
    return down;
  }
  if(analogRead(stickY) <= 60)
  {
    delay(10);
    return up;
  }
  return none; 
}

void difficultySelect()
{
  Difficulty selection{easy};
  byte count{0};
  while(true)
  {
    if(digitalRead(stickButt) == 0)
      break;
    if((moveCheck() == left) && (count > 0))
    {
      count -= 1;
      selection = difficulty[count];
      while(moveCheck() != none)
      ; 
    }
    if((moveCheck() == right) && (count < 2))
    {
      count += 1;
      selection = difficulty[count];
      while(moveCheck() != none)
      ; 
    }
    switch(selection)
    {
      case easy:
        digitalWrite(winLight, HIGH);
        digitalWrite(loseLight, LOW);
        break;
      case medium: 
        digitalWrite(winLight, LOW);
        digitalWrite(loseLight, HIGH);
        break;
      case hard:
        digitalWrite(winLight, HIGH);
        digitalWrite(loseLight, HIGH);
        break;
    }
  }
  clearLEDs();
  switch(selection)
  {
    case easy:
      patBlink = 300;
      timeLimit = 3000;
      return;
    case medium:
      patBlink = 250;
      timeLimit = 2500;
      return;
    case hard:
      patBlink = 200;
      timeLimit = 2000;
      return; 
  }
}

void clearLEDs() { PORTF &= 0x00; }
