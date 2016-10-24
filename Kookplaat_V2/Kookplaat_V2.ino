const byte digits[16] =
{
  B11111100,  //0
  B01100000,  //1
  B11011010,  //2
  B11110010,  //3
  B01100110,  //4
  B10110110,  //5
  B10111110,  //6
  B11100000,  //7
  B11111110,  //8
  B11110110,  //9
  B11101110,  //A
  B00111110,  //B
  B00011010,  //C
  B01111010,  //D
  B10011110,  //E
  B10001110,  //F
};

const byte dotDigits[16] =
{
  B11111101,  //0
  B01100001,  //1
  B11011011,  //2
  B11110011,  //3
  B01100111,  //4
  B10110111,  //5
  B10111111,  //6
  B11100001,  //7
  B11111111,  //8
  B11110111,  //9
  B11101111,  //A
  B00111111,  //B
  B00011011,  //C
  B01111011,  //D
  B10011111,  //E
  B10001111,  //F
};

const int clockPin = 8;
const int dataPin  = 12;
const int latchPin = 13;

const int displayPin[8] = { 
  B01110111,
  B10111011,
  B11011101,
  B11101110,
  B01110111,
  B10111011,
  B11011101,
  B11101110
};

const int ledPin[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

const int beeperPin = 11;

const int buttonPitUp = A1;     //Pit omhoog
const int buttonTempUp = A0;    //Temperatuur omhoog
const int buttonOn = 9;         //Aan
const int buttonSafeOn = 10;    //Kinderslot aan

const int buttonPitDown = A5;  //Pit omlaag
const int buttonTempDown = A4; //Temperatuur omlaag
const int buttonOff = A3;      //Uit
const int buttonSafeOff = A2;  //Kinderslot uit

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  
  for (int l = 0; l < 8; l++)
    pinMode(ledPin[l], OUTPUT);

  pinMode(buttonPitUp, INPUT);
  pinMode(buttonTempUp, INPUT);
  pinMode(buttonOn, INPUT);
  pinMode(buttonSafeOn, INPUT);

  pinMode(buttonPitDown, INPUT);
  pinMode(buttonTempDown, INPUT);
  pinMode(buttonOff, INPUT);
  pinMode(buttonSafeOff, INPUT);

  pinMode(beeperPin, OUTPUT);
}

int temp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int bPitUp = 0;
int bPitDown = 0;
int bTempUp = 0;
int bTempDown = 0;
int bOn = 0;
int bOff = 0;
int bSafeOn = 0;
int bSafeOff = 0;

int pit = 0;

boolean on = false;
boolean lock = false;

void loop()
{
  if (on)
  {
    if (!lock)
    {
      CheckOn();
      CheckPit();
      CheckLevel();
      CheckLock();
    }
    else
    {
      CheckLock();
      CheckBeep();
    }
    
    DisplayNumber();
    Leds();
  }
  else
  {
    DisplayOff();
    CheckOn();
  }
}

void DisplayDigit(int displayIndex, byte digitL, byte digitR)
{
  DisplayOff();
  
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, LSBFIRST, displayPin[displayIndex]);
  shiftOut(dataPin, clockPin, LSBFIRST, digitR);
  shiftOut(dataPin, clockPin, LSBFIRST, digitL);

  digitalWrite(latchPin, HIGH);
}

void DisplayOff()
{
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, LSBFIRST, B11111111);
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);

  digitalWrite(latchPin, HIGH);
}

void DisplayNumber()
{
  if (!lock)
  {
    DisplayDigit(0, byte(digits[temp[0]]), byte(digits[temp[4]]));
    DisplayDigit(1, byte(digits[temp[1]]), byte(digits[temp[5]]));
    DisplayDigit(2, byte(digits[temp[2]]), byte(digits[temp[6]]));
    DisplayDigit(3, byte(digits[temp[3]]), byte(digits[temp[7]]));
    
    if (pit < 4)
      DisplayDigit(pit, byte(dotDigits[temp[pit]]), byte(digits[temp[pit + 4]]));
    else
      DisplayDigit(pit, byte(digits[temp[pit - 4]]), byte(dotDigits[temp[pit]]));
  }
  else
  {
    DisplayDigit(0, byte(dotDigits[temp[0]]), byte(dotDigits[temp[4]]));
    DisplayDigit(1, byte(dotDigits[temp[1]]), byte(dotDigits[temp[5]]));
    DisplayDigit(2, byte(dotDigits[temp[2]]), byte(dotDigits[temp[6]]));
    DisplayDigit(3, byte(dotDigits[temp[3]]), byte(dotDigits[temp[7]]));
  }
}

void CheckLevel()
{
  if (analogRead(buttonTempUp) > 1000)
  {
    if (bTempUp == 25)
    {
      temp[pit]++;
      
      if (temp[pit] > 9)
        temp[pit] = 9;
      else
        Beep(temp[pit] * 20 + 200);
      
      bTempUp = 0;
    }

    bTempUp++;
  }

  if (analogRead(buttonTempDown) > 1000)
  {
    if (bTempDown == 25)
    {
      temp[pit]--;
      
      if (temp[pit] < 0)
        temp[pit] = 0;
      else
        Beep(temp[pit] * 20 + 200);
      
      bTempDown = 0;
    }

    bTempDown++;
  }
}

void Beep(int freq)
{
  noTone(beeperPin);
  tone(beeperPin, freq, 250);
}

void CheckOn()
{
  if (digitalRead(buttonOn) == HIGH && !on)
  {
    if (bOn == 100)
    {
      on = true;
      
      Beep(220);
      delay(500);
      Beep(330);
      delay(500);
      Beep(440);
      
      for (int i = 0; i < 250; i++)
      {
        DisplayDigit(0, digits[0], B00000000);
        DisplayDigit(1, B11101100, B00000000);
        DisplayDigit(2, B00000000, B00000000);
        DisplayDigit(3, B00000000, B00000000);
      }
      
      bOn = 0;
    }

    bOn++;
  }
  
  if (analogRead(buttonOff) > 1000 && on)
  {
    if (bOff == 100)
    {
      on = false;
      
      for (int i = 0; i < 250; i++)
      {
        DisplayDigit(0, digits[0], B00000000);
        DisplayDigit(1, digits[15], B00000000);
        DisplayDigit(2, digits[15], B00000000);
        DisplayDigit(3, B00000000, B00000000);
      }
      
      DisplayOff();
      
      Beep(440);
      delay(500);
      Beep(330);
      delay(500);
      Beep(220);
      
      pit = 0;
      lock = 0;
      
      for (int i = 0; i < 8; i++)
        temp[i] = 0;
      
      bOff = 0;
    }

    bOff++;
  }
}

void CheckPit()
{
  if (analogRead(buttonPitUp) > 1000)
  {
    if (bPitUp == 50)
    {
      pit++;
      
      if (pit > 7)
        pit = 0;
      
      Beep(440);
      
      bPitUp = 0;
    }

    bPitUp++;
  }
  
  if (analogRead(buttonPitDown) > 1000)
  {
    if (bPitDown == 50)
    {
      pit--;
      
      if (pit < 0)
        pit = 7;
      
      Beep(330);
      
      bPitDown = 0;
    }
    
    bPitDown++;
  }
}

int t = 0;

void CheckLock()
{
  if (digitalRead(buttonSafeOn) == HIGH && !lock)
  {
    if (bSafeOn == 100)
    {
      lock = true;
      
      Beep(440);
      delay(300);
      Beep(440);
      
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(0, digits[5], digits[0]);
        DisplayDigit(1, digits[10], B11101100);
        DisplayDigit(2, digits[15], B00000000);
        DisplayDigit(3, digits[14], B00000000);
      }
      
      bSafeOn = 0;
    }

    bSafeOn++;
  }
  
  if (analogRead(buttonSafeOff) > 1000 && lock)
  {
    if (bSafeOff == 50)
    {
      lock = false;
      
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(0, digits[5], digits[0]);
        DisplayDigit(1, digits[10], digits[15]);
        DisplayDigit(2, digits[15], digits[15]);
        DisplayDigit(3, digits[14], B00000000);
      }
      
      Beep(330);
      delay(300);
      Beep(330);
      
      bSafeOff = 0;
    }

    bSafeOff++;
  }
}

void CheckBeep()
{
  if (analogRead(buttonPitUp) > 1000 || analogRead(buttonTempUp) > 1000 || digitalRead(buttonOn) == HIGH || digitalRead(buttonSafeOn) == HIGH || analogRead(buttonTempDown) > 1000 || analogRead(buttonPitDown) > 1000 || analogRead(buttonOff) > 1000)
  {
    Beep(220);
  }
}

void Leds()
{  
  for (int l = 0; l < 8; l++)
  {
    if (temp[l] > 0)
      AnalogWrite(ledPin[l], temp[l]);
  }
}

void AnalogWrite(int pin, int level)
{
  digitalWrite(pin, HIGH);
  delayMicroseconds(level * 8);
  digitalWrite(pin, LOW);
}

