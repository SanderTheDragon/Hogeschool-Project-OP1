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

const int disp1 = 2;
const int disp2 = 3;
const int disp3 = 0;
const int disp4 = 1;

const int button1 = 4;    //Pit omhoog
const int button2 = 7;    //Temperatuur omhoog
const int button3 = A4;   //Aan
const int button4 = A5;   //Kinderslot aan

const int button4m = A3;  //Pit omlaag
const int button3m = A2;  //Temperatuur omlaag
const int button2m = A1;  //Uit
const int button1m = A0;  //Kinderslot uit

const int beeper = 11;

const int led1 = 5;
const int led2 = 6;
const int led3 = 9;
const int led4 = 10;

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);

  pinMode(disp1, OUTPUT);
  pinMode(disp2, OUTPUT);
  pinMode(disp3, OUTPUT);
  pinMode(disp4, OUTPUT);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  pinMode(button1m, INPUT);
  pinMode(button2m, INPUT);
  pinMode(button3m, INPUT);
  pinMode(button4m, INPUT);

  pinMode(beeper, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}

int i1 = 0;
int i2 = 0;
int i3 = 0;
int i4 = 0;

int i1t = 0;
int i2t = 0;
int i3t = 0;
int i4t = 0;
int i1tm = 0;
int i2tm = 0;
int i3tm = 0;
int i4tm = 0;

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
    
    DisplayNumber(i1 * 1000 + i2 * 100 + i3 * 10 + i4);
    Led();
  }
  else
  {
    DisplayOff();
    CheckOn();
  }
}

void DisplayDigit(int disp, byte digit)
{
  digitalWrite(latchPin, LOW);
  DisplayOff();

  shiftOut(dataPin, clockPin, LSBFIRST, digit);

  digitalWrite(latchPin, HIGH);
  digitalWrite(disp, LOW);
  delay(5);
}

void DisplayOff()
{
  digitalWrite(disp1, HIGH);
  digitalWrite(disp2, HIGH);
  digitalWrite(disp3, HIGH);
  digitalWrite(disp4, HIGH);
}

void DisplayNumber(int number)
{
  int d1, d2, d3, d4;

  if (number > 9999)
    number = 9999;
  if (number < 0)
    number = 0;

  d1 = (int) (number / 1000);
  number = number - (d1 * 1000);
  d2 = (int) (number / 100);
  number = number - (d2 * 100);
  d3 = (int) (number / 10);
  d4 = number - (d3 * 10);

  if (pit == 0)
    DisplayDigit(disp1, byte(dotDigits[d1]));
  else
    DisplayDigit(disp1, byte(digits[d1]));

  if (pit == 1)
    DisplayDigit(disp2, byte(dotDigits[d2]));
  else
    DisplayDigit(disp2, byte(digits[d2]));

  if (pit == 2)
    DisplayDigit(disp3, byte(dotDigits[d3]));
  else
    DisplayDigit(disp3, byte(digits[d3]));

  if (pit == 3)
    DisplayDigit(disp4, byte(dotDigits[d4]));
  else
    DisplayDigit(disp4, byte(digits[d4]));
}

void CheckLevel()
{
  if (digitalRead(button2) == HIGH)
  {
    if (i2t == 10)
    {
      switch (pit)
      {
      case 0:
        i1++;
        break;
      case 1:
        i2++;
        break;
      case 2:
        i3++;
        break;
      case 3:
        i4++;
        break;
      }
      
      i2t = 0;

      Beep();
    }

    i2t++;
  }

  if (analogRead(button2m) > 1000)
  {
    if (i2tm == 10)
    {
      switch (pit)
      {
      case 0:
        i1--;
        break;
      case 1:
        i2--;
        break;
      case 2:
        i3--;
        break;
      case 3:
        i4--;
        break;
      }
      
      i2tm = 0;

      Beep();
    }

    i2tm++;
  }

  if (i1 > 9)
    i1 = 9;
  if (i2 > 9)
    i2 = 9;
  if (i3 > 9)
    i3 = 9;
  if (i4 > 9)
    i4 = 9;

  if (i1 <= 0)
    i1 = 0;
  if (i2 <= 0)
    i2 = 0;
  if (i3 <= 0)
    i3 = 0;
  if (i4 <= 0)
    i4 = 0;
}

void Beep()
{
  analogWrite(beeper, 100);
  delay(50);
  analogWrite(beeper, 0);
}

void Led()
{
  analogWrite(led1, (12 / 9 * i1));
  analogWrite(led2, (12 / 9 * i2));
  analogWrite(led3, (12 / 9 * i3));
  analogWrite(led4, (12 / 9 * i4));
}

void CheckOn()
{
  if (analogRead(button3) > 1000 && !on)
  {
    if (i3t == 5000)
    {
      on = true;
      
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(disp1, digits[0]);
        DisplayDigit(disp2, B11101100);
      }
      
      Beep();
      delay(100);
      Beep();
      delay(100);
      Beep();
      
      i3t = 0;
    }

    i3t++;
  }
  
  if (analogRead(button3m) > 1000 && on)
  {
    if (i3tm == 50)
    {
      on = false;
      
      Beep();
      delay(100);
      Beep();
      delay(100);
      Beep();
      
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(disp1, digits[0]);
        DisplayDigit(disp2, digits[15]);
        DisplayDigit(disp3, digits[15]);
      }
      
      pit = 0;
      lock = 0;
      i1 = 0;
      i2 = 0;
      i3 = 0;
      i4 = 0;
      
      i3tm = 0;
    }

    i3tm++;
  }
}

void CheckPit()
{
  if (digitalRead(button1) == HIGH)
  {
    if (i1t == 30)
    {
      pit++;
      
      if (pit > 3)
        pit = 0;
      
      Beep();
      
      i1t = 0;
    }

    i1t++;
  }
  
  if (analogRead(button1m) > 1000)
  {
    if (i1tm == 30)
    {
      pit--;
      
      if (pit < 0)
        pit = 3;
      
      Beep();
      
      i1tm = 0;
    }

    i1tm++;
  }
}

int t = 0;

void CheckLock()
{
  if (analogRead(button4) > 1000 && !lock)
  {
    if (i4t == 50)
    {
      lock = true;
      
      Beep();
      delay(100);
      Beep();
      
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(disp1, dotDigits[5]);
        DisplayDigit(disp2, dotDigits[10]);
        DisplayDigit(disp3, dotDigits[15]);
        DisplayDigit(disp4, dotDigits[14]);
      }
      
      i4t = 0;
    }

    i4t++;
  }
  
  if (analogRead(button4m) > 1000 && lock)
  {
    if (i4tm == 100)
    {
      lock = false;
      
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(disp1, digits[5]);
        DisplayDigit(disp2, digits[10]);
        DisplayDigit(disp3, digits[15]);
        DisplayDigit(disp4, digits[14]);
      }
      
      Beep();
      delay(100);
      Beep();
      
      i4tm = 0;
    }

    i4tm++;
  }
}

void CheckBeep()
{
  if (digitalRead(button1) == HIGH || digitalRead(button2) == HIGH || analogRead(button3) > 1000 || analogRead(button4) > 1000 || analogRead(button1m) > 1000 || analogRead(button2m) > 1000 || analogRead(button3m) > 1000)
  {
    Beep();
    
    for (int i = 0; i < 10; i++)
    {
        DisplayDigit(disp1, dotDigits[5]);
        DisplayDigit(disp2, dotDigits[10]);
        DisplayDigit(disp3, dotDigits[15]);
        DisplayDigit(disp4, dotDigits[14]);
    }
  }
}

