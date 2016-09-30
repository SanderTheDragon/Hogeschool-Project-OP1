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

const int button1 = 4;
const int button2 = 7;
const int button3 = A4;
const int button4 = A5;

const int button4m = A0;
const int button3m = A1;
const int button2m = A2;
const int button1m = A3;

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

int dot = 0;

void loop()
{
  ReadButtons();
  Led();
  DisplayNumber(i1 * 1000 + i2 * 100 + i3 * 10 + i4);
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

  if (dot & 1)
    DisplayDigit(disp1, byte(dotDigits[d1]));
  else
    DisplayDigit(disp1, byte(digits[d1]));

  if (dot & 2)
    DisplayDigit(disp2, byte(dotDigits[d2]));
  else
    DisplayDigit(disp2, byte(digits[d2]));

  if (dot & 4)
    DisplayDigit(disp3, byte(dotDigits[d3]));
  else
    DisplayDigit(disp3, byte(digits[d3]));

  if (dot & 8)
    DisplayDigit(disp4, byte(dotDigits[d4]));
  else
    DisplayDigit(disp4, byte(digits[d4]));
}

void ReadButtons()
{
  if (digitalRead(button1) == HIGH)
  {
    if (i1t == 10)
    {
      i1++;
      i1t = 0;

      Beep();
    }

    i1t++;
    dot |= 1;
  }

  if (digitalRead(button2) == HIGH)
  {
    if (i2t == 10)
    {
      i2++;
      i2t = 0;

      Beep();
    }

    i2t++;
    dot |= 2;
  }

  if (analogRead(button3) > 1000)
  {
    if (i3t == 10)
    {
      i3++;
      i3t = 0;

      Beep();
    }

    i3t++;
    dot |= 4;
  }

  if (analogRead(button4) > 1000)
  {
    if (i4t == 10)
    {
      i4++;
      i4t = 0;

      Beep();
    }

    i4t++;
    dot |= 8;
  }

  if (analogRead(button1m) > 1000)
  {
    if (i1tm == 10)
    {
      i1--;
      i1tm = 0;

      Beep();
    }

    i1tm++;
    dot |= 1;
  }

  if (analogRead(button2m) > 1000)
  {
    if (i2tm == 10)
    {
      i2--;
      i2tm = 0;

      Beep();
    }

    i2tm++;
    dot |= 2;
  }

  if (analogRead(button3m) > 1000)
  {
    if (i3tm == 10)
    {
      i3--;
      i3tm = 0;

      Beep();
    }

    i3tm++;
    dot |= 4;
  }

  if (analogRead(button4m) > 1000)
  {
    if (i4tm == 10)
    {
      i4--;
      i4tm = 0;

      Beep();
    }

    i4tm++;
    dot |= 8;
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
  {
    i1 = 0;
    dot &= ~1;
  }
  if (i2 <= 0)
  {
    i2 = 0;
    dot &= ~2;
  }
  if (i3 <= 0)
  {
    i3 = 0;
    dot &= ~4;
  }
  if (i4 <= 0)
  {
    i4 = 0;
    dot &= ~8;
  }
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
