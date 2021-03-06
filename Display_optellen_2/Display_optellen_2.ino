// 20-09-16 
// het optellen van getallen 2



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
} 

int i1 = 0;
int i2 = 0;
int i3 = 0;
int i4 = 0;

int i1t = 0;
int i2t = 0;
int i3t = 0;
int i4t = 0;

void loop() 
{
  if (digitalRead(button1) == HIGH)
  { 
    if (i1t = 1000)
    {
      i1++;
      i1t = 0;
    }
      
    i1t++;
  }
  if (digitalRead(button2) == HIGH)
  { 
    if (i2t = 1000)
    {
      i2++;
      i2t = 0;
    }
      
    i2t++;
  }
  if (digitalRead(button3) == HIGH)
  { 
    if (i3t = 1000)
    {
      i3++;
      i3t = 0;
    }
      
    i3t++;
  }
  if (digitalRead(button4) == HIGH)
  {
    if (i4t = 1000)
    {
      i4++;
      i4t = 0;
    }
      
    i4t++;
  }
    
  SevenSegDisplay(i1 * 1000 + i2 * 100 + i3 * 10 + i4);
  
  if (i1 > 9)
    i1 = 0;
  if (i2 > 9)
    i2 = 0;
  if (i3 > 9)
    i3 = 0;
  if (i4 > 9)
    i4 = 0;
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
