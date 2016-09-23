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

const int latchPin = 10;
const int clockPin = 11;
const int dataPin  = 12;

const int disp1 = 2;
const int disp2 = 3;
const int disp3 = 4;
const int disp4 = 5;

const int button1 = 6;
const int button2 = 7;
const int button3 = 8;
const int button4 = 9;

const int button4m = A0;
const int button3m = A1;
const int button2m = A2;
const int button1m = A3;

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

void loop() 
{
  if (digitalRead(button1) == HIGH)
  { 
    if (i1t == 10)
    {
      i1++;
      i1t = 0;
    }
      
    i1t++;
  }
  if (digitalRead(button2) == HIGH)
  { 
    if (i2t == 10)
    {
      i2++;
      i2t = 0;
    }
      
    i2t++;
  }
  if (digitalRead(button3) == HIGH)
  { 
    if (i3t == 10)
    {
      i3++;
      i3t = 0;
    }
      
    i3t++;
  }
  if (digitalRead(button4) == HIGH)
  {
    if (i4t == 10)
    {
      i4++;
      i4t = 0;
    }
      
    i4t++;
  }
   if (digitalRead(button1m) == HIGH)
  {
    if (i1tm == 10)
    {
      i1--;
      i1tm = 0;
    }
      
    i1tm++;
  }

   if (digitalRead(button2m) == HIGH)
  {
    if (i2tm == 10)
    {
      i2--;
      i2tm = 0;
    }
      
    i2tm++;
  }

   if (digitalRead(button3m) == HIGH)
  {
    if (i3tm == 10)
    {
      i3--;
      i3tm = 0;
    }
      
    i3tm++;
  }

   if (digitalRead(button4m) == HIGH)
  {
    if (i4tm == 10)
    {
      i4--;
      i4tm = 0;
    }
      
    i4tm++;
  }
     
  if (i1 > 9)
    i1 = 9;
  if (i2 > 9)
    i2 = 9;
  if (i3 > 9)
    i3 = 9;
  if (i4 > 9)
    i4 = 9;
   
  if (i1 < 0)
    i1 = 0;
  if (i2 < 0)
    i2 = 0;
  if (i3 < 0)
    i3 = 0;
  if (i4 < 0)
    i4 = 0;
    
  SevenSegDisplay(i1 * 1000 + i2 * 100 + i3 * 10 + i4);
}

void DisplayADigit(int disp, byte digit)
{
  digitalWrite(latchPin, LOW);
  AllDispOff();
  
  shiftOut(dataPin, clockPin, LSBFIRST, digit);
  
  digitalWrite(latchPin, HIGH);
  digitalWrite(disp, LOW);
  delay(5);
}

void AllDispOff()
{
  digitalWrite(disp1, HIGH);
  digitalWrite(disp2, HIGH);
  digitalWrite(disp3, HIGH);
  digitalWrite(disp4, HIGH);
}

void SevenSegDisplay(int number)
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
  
  DisplayADigit(disp1, byte(digits[d1]));
  DisplayADigit(disp2, byte(digits[d2]));
  DisplayADigit(disp3, byte(digits[d3]));
  DisplayADigit(disp4, byte(digits[d4]));
}
