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

const int disp[8] = { 0, 1, 2, 3, 5, 6, 9, 10 };

const int button1 = 4;    //Pit omhoog
const int button2 = 7;    //Temperatuur omhoog
const int button3 = A4;   //Aan
const int button4 = A5;   //Kinderslot aan

const int button4m = A3;  //Pit omlaag
const int button3m = A2;  //Temperatuur omlaag
const int button2m = A1;  //Uit
const int button1m = A0;  //Kinderslot uit

const int beeper = 11;

/*const int led1 = 5;
const int led2 = 6;
const int led3 = 9;
const int led4 = 10;*/

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);

  pinMode(disp1, OUTPUT);
  pinMode(disp2, OUTPUT);
  pinMode(disp3, OUTPUT);
  pinMode(disp4, OUTPUT);
  pinMode(disp5, OUTPUT);
  pinMode(disp6, OUTPUT);
  pinMode(disp7, OUTPUT);
  pinMode(disp8, OUTPUT);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  pinMode(button1m, INPUT);
  pinMode(button2m, INPUT);
  pinMode(button3m, INPUT);
  pinMode(button4m, INPUT);

  pinMode(beeper, OUTPUT);

  /*pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);*/
}

int i[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

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
    
    DisplayNumber();
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
  delay(1);
}

void DisplayOff()
{
  digitalWrite(disp[0], HIGH);
  digitalWrite(disp[1], HIGH);
  digitalWrite(disp[2], HIGH);
  digitalWrite(disp[3], HIGH);
  digitalWrite(disp[4], HIGH);
  digitalWrite(disp[5], HIGH);
  digitalWrite(disp[6], HIGH);
  digitalWrite(disp[7], HIGH);
}

void DisplayNumber()
{
  if (pit == 0)
    DisplayDigit(disp[0], byte(dotDigits[i[0]]));
  else
    DisplayDigit(disp[0], byte(digits[i[0]]));

  if (pit == 1)
    DisplayDigit(disp2, byte(dotDigits[i2]));
  else
    DisplayDigit(disp2, byte(digits[i2]));

  if (pit == 2)
    DisplayDigit(disp3, byte(dotDigits[i3]));
  else
    DisplayDigit(disp3, byte(digits[i3]));

  if (pit == 3)
    DisplayDigit(disp4, byte(dotDigits[i4]));
  else
    DisplayDigit(disp4, byte(digits[i4]));
    
  if (pit == 4)
    DisplayDigit(disp5, byte(dotDigits[i5]));
  else
    DisplayDigit(disp5, byte(digits[i5]));

  if (pit == 5)
    DisplayDigit(disp6, byte(dotDigits[i6]));
  else
    DisplayDigit(disp6, byte(digits[i6]));

  if (pit == 6)
    DisplayDigit(disp7, byte(dotDigits[i7]));
  else
    DisplayDigit(disp7, byte(digits[i7]));

  if (pit == 7)
    DisplayDigit(disp8, byte(dotDigits[i8]));
  else
    DisplayDigit(disp8, byte(digits[i8]));
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
        Beep(i1 * 20 + 200);
        break;
      case 1:
        i2++;
        Beep(i2 * 20 + 200);
        break;
      case 2:
        i3++;
        Beep(i3 * 20 + 200);
        break;
      case 3:
        i4++;
        Beep(i4 * 20 + 200);
        break;
      case 4:
        i5++;
        Beep(i5 * 20 + 200);
        break;
      case 5:
        i6++;
        Beep(i6 * 20 + 200);
        break;
      case 6:
        i7++;
        Beep(i7 * 20 + 200);
        break;
      case 7:
        i8++;
        Beep(i8 * 20 + 200);
        break;
      }
      
      i2t = 0;
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
        Beep(i1 * 20 + 200);
        break;
      case 1:
        i2--;
        Beep(i2 * 20 + 200);
        break;
      case 2:
        i3--;
        Beep(i3 * 20 + 200);
        break;
      case 3:
        i4--;
        Beep(i4 * 20 + 200);
        break;
      case 4:
        i5--;
        Beep(i5 * 20 + 200);
        break;
      case 5:
        i6--;
        Beep(i6 * 20 + 200);
        break;
      case 6:
        i7--;
        Beep(i7 * 20 + 200);
        break;
      case 7:
        i8--;
        Beep(i8 * 20 + 200);
        break;
      }
      
      i2tm = 0;
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
  if (i5 > 9)
    i5 = 9;
  if (i6 > 9)
    i6 = 9;
  if (i7 > 9)
    i7 = 9;
  if (i8 > 9)
    i8 = 9;

  if (i1 <= 0)
    i1 = 0;
  if (i2 <= 0)
    i2 = 0;
  if (i3 <= 0)
    i3 = 0;
  if (i4 <= 0)
    i4 = 0;
  if (i5 <= 0)
    i5 = 0;
  if (i6 <= 0)
    i6 = 0;
  if (i7 <= 0)
    i7 = 0;
  if (i8 <= 0)
    i8 = 0;
}

void Beep(int freq)
{
  noTone(beeper);
  tone(beeper, freq, 250);
}

void Led()
{
  /*analogWrite(led1, (12 / 9 * i1));
  analogWrite(led2, (12 / 9 * i2));
  analogWrite(led3, (12 / 9 * i3));
  analogWrite(led4, (12 / 9 * i4));*/
}

void CheckOn()
{
  if (analogRead(button3) > 1000 && !on)
  {
    if (i3t == 5000)
    {
      on = true;
      
      Beep(220);
      delay(500);
      Beep(330);
      delay(500);
      Beep(440);
      
      for (int i = 0; i < 500; i++)
      {
        DisplayDigit(disp1, digits[0]);
        DisplayDigit(disp2, B11101100);
      }
      
      i3t = 0;
    }

    i3t++;
  }
  
  if (analogRead(button3m) > 1000 && on)
  {
    if (i3tm == 50)
    {
      on = false;
      
      for (int i = 0; i < 500; i++)
      {
        DisplayDigit(disp1, digits[0]);
        DisplayDigit(disp2, digits[15]);
        DisplayDigit(disp3, digits[15]);
      }
      
      DisplayOff();
      
      Beep(440);
      delay(500);
      Beep(330);
      delay(500);
      Beep(220);
      
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
      
      if (pit > 7)
        pit = 0;
      
      Beep(440);
      
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
        pit = 7;
      
      Beep(330);
      
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
      
      Beep(440);
      delay(300);
      Beep(440);
      
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
      
      Beep(330);
      delay(300);
      Beep(330);
      
      i4tm = 0;
    }

    i4tm++;
  }
}

void CheckBeep()
{
  if (digitalRead(button1) == HIGH || digitalRead(button2) == HIGH || analogRead(button3) > 1000 || analogRead(button4) > 1000 || analogRead(button1m) > 1000 || analogRead(button2m) > 1000 || analogRead(button3m) > 1000)
  {
    Beep(220);
    
    for (int i = 0; i < 50; i++)
    {
        DisplayDigit(disp1, dotDigits[5]);
        DisplayDigit(disp2, dotDigits[10]);
        DisplayDigit(disp3, dotDigits[15]);
        DisplayDigit(disp4, dotDigits[14]);
    }
  }
}

