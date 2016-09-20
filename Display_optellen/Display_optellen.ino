// 16-09-16
// het optellen van getallen



// Define the bit-patterns for the 7-segment displays 
const byte SevenSeg[16] = 
{
     // Hex digits
     B11111100, B01100000, B11011010, B11110010,   // 0123
     B01100110, B10110110, B10111110, B11100000,   // 4567
     B11111110, B11110110, B11101110, B00111110,   // 89AB
     B00011010, B01111010, B10011110, B10001110,   // CDEF
  
};

// Pin connected to latch pin (RCLK,12) of 74HC595
const int latchPin = 10;
// Pin connected to clock pin (SRCLK,11) of 74HC595
const int clockPin = 11;
// Pin connected to Data in (SER,14) of 74HC595
const int dataPin  = 12;
// Pin connected to cathodes
const int disp1 = 0;  // Displays are numbered from left to right
const int disp2 = 1;  // as disp1 to disp4.
const int disp3 = 2;  // Scan displays quickly and use POV to display
const int disp4 = 3;  // 4 digits with only 1 shift register

//
void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  pinMode(disp1, OUTPUT);
  pinMode(disp2, OUTPUT);
  pinMode(disp3, OUTPUT);
  pinMode(disp4, OUTPUT);
}  
//
void loop() 
{
for (int i=0; i < 10000; ++i)
  {
    // Display i as 4 digits on the display.
    // No leading zero blanking.
    for (int j=0; j<20; j++) // Draw the display several times to slow it down
      SevenSegDisplay(i);
  }
}

void DisplayADigit(int dispno, byte digit2disp)
{
  
  // Turn off the shift register pins
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);
  
  AllDispOff();  // Turn off all cathode drivers.
  
  // shift the bits out:
  shiftOut(dataPin, clockPin, LSBFIRST, digit2disp);
  
  digitalWrite(latchPin, HIGH);  // Set latch high to set segments.
  
  digitalWrite(dispno, LOW);  // Drive one cathode low to turn on display.
  
  delay(5);  // Wait a bit for POV

}

void AllDispOff()
{
  // Turn all cathode drivers off
  digitalWrite(disp1, HIGH);
  digitalWrite(disp2, HIGH);
  digitalWrite(disp3, HIGH);
  digitalWrite(disp4, HIGH);
}

void SevenSegDisplay(int number)
{
  int d1,d2,d3,d4; // Temporary values for thousands, hundreds, tens and units
  
  if (number > 9999)
     number = 9999;  // Do some bounds checks to avoid strangeness
  if (number < 0)
     number = 0;
  
  d1 = (int) (number / 1000);    // Get thousands
  number = number - (d1 * 1000);
  d2 = (int) (number / 100);     // Get hundreds
  number = number - (d2 * 100);
  d3 = (int) (number / 10);      // Get tens
  d4 = number - (d3* 10);        // Get units
  
  DisplayADigit(disp1,byte(SevenSeg[d1]));  // Show thousands
  DisplayADigit(disp2,byte(SevenSeg[d2]));  // Show hundreds
  DisplayADigit(disp3,byte(SevenSeg[d3]));  // Show tens
  DisplayADigit(disp4,byte(SevenSeg[d4]));  // Show units

}
