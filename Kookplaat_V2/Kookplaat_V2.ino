//Getallen en letters als bytes (zonder punt)
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

//Getallen en letters als bytes (met punt)
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

//Pins waar de shift registers op zijn aangesloten: clock, data, latch
const int clockPin = 8;
const int dataPin  = 12;
const int latchPin = 13;

//Bytes voor de selectie van de digit (via shift register)
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

//Pins waar de led lampjes op zijn aangesloten
const int ledPin[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

//Pin waar de piezo op is aangesloten
const int beeperPin = 11;

//Pins waar de knoppen op zijn aangesloten
const int buttonPitUp = A1;     //Pit omhoog
const int buttonTempUp = A0;    //Temperatuur omhoog
const int buttonOn = 9;         //Aan
const int buttonSafeOn = 10;    //Kinderslot aan

const int buttonPitDown = A5;   //Pit omlaag
const int buttonTempDown = A4;  //Temperatuur omlaag
const int buttonOff = A3;       //Uit
const int buttonSafeOff = A2;   //Kinderslot uit

//Zet de modus voor alle pins, INPUT voor de knoppen, OUTPUT voor de rest
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

//Temperatuur van de pitten
int temp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//Tellers voor de tijd dat de knoppen zijn ingedrukt, anders gaat het veel te snel
int bPitUp = 0;
int bPitDown = 0;
int bTempUp = 0;
int bTempDown = 0;
int bOn = 0;
int bOff = 0;
int bSafeOn = 0;
int bSafeOff = 0;

//De pit die op dit moment is geselecteerd
int pit = 0;

//Staat de kookplaat aan(true) of uit(false)
boolean on = false;

//Staat het kinderslot aan(true) of uit(false)
boolean lock = false;

void loop()
{
  if (on)              //Als de kookplaat aan staat
  {
    if (!lock)         //Als het kinderslot uit staat
    {
      CheckOn();       //Check of de uitknop is ingedrukt
      CheckPit();      //Check of de pit selectie is ingedrukt
      CheckLevel();    //Check of de temperatuur omhoog of omlaag moet
      CheckLock();     //Check of het kinderslot aan moet
    }
    else               //Als het kinderslot aan staat
    {
      CheckLock();     //Check of het kinderslot uit moet
      CheckBeep();     //Maak een geluid als een andere knop is ingedrukt
    }
    
    //Wordt altijd gedaan als de kookplaat aan staat
    DisplayNumber();   //Laat de nummers op de displays zien
    Leds();            //Laat de leds op verschillende felheid zien
  }
  else                 //Als de kookplaat uit staat
  {
    DisplayOff();      //Zet de display uit
    CheckOn();         //Check of de aanknop is ingedrukt
  }
}

//Functie om een getal op een bepaalde positie zien
void DisplayDigit(int displayIndex, byte digitL, byte digitR)
{
  DisplayOff();                                                        //Zet de display uit
  
  digitalWrite(latchPin, LOW);                                         //Zet de latch laag
  
  shiftOut(dataPin, clockPin, LSBFIRST, displayPin[displayIndex]);     //Schrijf naar de eerste shift register (positie)
  shiftOut(dataPin, clockPin, LSBFIRST, digitR);                       //Schrijf naar de tweede shift register (eerste display)
  shiftOut(dataPin, clockPin, LSBFIRST, digitL);                       //Schrijf naar de derde shift register  (tweede display)

  digitalWrite(latchPin, HIGH);                                        //Zet de latch hoog, dan komen de getallen tevoorschijn
}

//Functie om de display uit te zetten
void DisplayOff()
{
  digitalWrite(latchPin, LOW);                        //Zet de latch laag

  shiftOut(dataPin, clockPin, LSBFIRST, B11111111);   //Schrijf 1 naar de eerste shift register (selecteer alle posities)
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);   //Schrijf 0 naar de tweede shift register (zet alle segments uit)
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);   //Schrijf 0 naar de derde shift register  (zet alle segments uit)

  digitalWrite(latchPin, HIGH);                       //Zet de latch hoog, dan staan ze uit
}

//Functie om de getallen te laten zien
void DisplayNumber()
{
  if (!lock)                                                                        //Als het kinderslot uit staat
  {
    //Schrijf alle getallen op de juiste posities
    DisplayDigit(0, byte(digits[temp[0]]), byte(digits[temp[4]]));
    DisplayDigit(1, byte(digits[temp[1]]), byte(digits[temp[5]]));
    DisplayDigit(2, byte(digits[temp[2]]), byte(digits[temp[6]]));
    DisplayDigit(3, byte(digits[temp[3]]), byte(digits[temp[7]]));
    
    //Laat een punt zien op de locatie van de geslecteerde pin
    if (pit < 4)
      DisplayDigit(pit, byte(dotDigits[temp[pit]]), byte(digits[temp[pit + 4]]));   //Laat op de eerste display een punt zien
    else
      DisplayDigit(pit, byte(digits[temp[pit - 4]]), byte(dotDigits[temp[pit]]));   //Laat op de tweede display een punt zien
  }
  else                                                                              //Als het kinderslot aan staat
  {
    //Schrijf alle getallen op de juiste posities, allemaal met een punt
    DisplayDigit(0, byte(dotDigits[temp[0]]), byte(dotDigits[temp[4]]));
    DisplayDigit(1, byte(dotDigits[temp[1]]), byte(dotDigits[temp[5]]));
    DisplayDigit(2, byte(dotDigits[temp[2]]), byte(dotDigits[temp[6]]));
    DisplayDigit(3, byte(dotDigits[temp[3]]), byte(dotDigits[temp[7]]));
  }
}

//Functie om de temperatuur knoppen te lezen
void CheckLevel()
{
  if (analogRead(buttonTempUp) > 1000)      //Als de knop is ingedrukt
  {
    if (bTempUp == 25)                      //Als de knop voor 25 loops{1} is ingedrukt
    {
      temp[pit]++;                          //Zet de temperatuur van de geselecteerde pit 1 omhoog
      
      if (temp[pit] > 9)                    //Als de temperatuur hoger is dan 9
        temp[pit] = 9;                      //Dan is de temperatuur 9(temperatuur kan niet >10 zien)
      else
        Beep(temp[pit] * 20 + 200);         //Maak een geluidje als de temperatuur hoger wordt
      
      bTempUp = 0;                          //Zet het aantal loops{1} dat de knop is ingedrukt weer op 0
    }

    bTempUp++;                              //Zet het aantal loops{1} 1 hoger
  }

  if (analogRead(buttonTempDown) > 1000)    //Als de knop is ingedrukt
  {
    if (bTempDown == 25)                    //Als de knop voor 25 loops{1} is ingedrukt
    {
      temp[pit]--;                          //Zet de temperatuur van de geselecteerde pit 1 omlaag
      
      if (temp[pit] < 0)                    //Als de temperatuur lager is dan 0
        temp[pit] = 0;                      //Dan is de temperatuur 0(temperatuur kan niet <0 zijn)
      else
        Beep(temp[pit] * 20 + 200);         //Maak een geluidje als de temperatuur lager wordt
      
      bTempDown = 0;                        //Zet het aantal loops{1} dat de knop is ingedrukt weer op 0
    }

    bTempDown++;                            //Zet het aantal loops{1} 1 hoger
  }
}

//Functie om een beep geluidje te maken op een bepaalde frequentie
void Beep(int freq)
{
  noTone(beeperPin);            //Zet het geluid uit als er een aanstond
  tone(beeperPin, freq, 250);   //Maak het geluid voor 250 milliseconden op de gegeven frequentie
}

//Functie om de aan- en uitknop te lezen
void CheckOn()
{
  if (digitalRead(buttonOn) == HIGH && !on)        //Als de knop is ingedrukt en de kookplaat uit staat
  {
    if (bOn == 100)                                //Als de knop voor 100 loops{1} is ingedrukt
    {
      on = true;                                   //Zet de kookplaat aan
      
      //Maak 3 geluiden, oplopend in frequentie
      Beep(220);
      delay(500);
      Beep(330);
      delay(500);
      Beep(440);
      
      //Laat het woord 'ON' zien
      for (int i = 0; i < 250; i++)
      {
        DisplayDigit(0, digits[0], B00000000);
        DisplayDigit(1, B11101100, B00000000);
        DisplayDigit(2, B00000000, B00000000);
        DisplayDigit(3, B00000000, B00000000);
      }
      
      bOn = 0;                                     //Zet het aantal loops{1} weer op 0
    }

    bOn++;                                         //Zet het aantal loops{1} 1 hoger
  }
  
  if (analogRead(buttonOff) > 1000 && on)          //Als de knop is ingedrukt en de kookplaat aan staat
  {
    if (bOff == 100)                               //Als de knop voor 100 loops{1} is ingedrukt
    {
      on = false;                                  //Zet de kookplaat uit
      
      //Laat het woord 'OFF' zien
      for (int i = 0; i < 250; i++)
      {
        DisplayDigit(0, digits[0], B00000000);
        DisplayDigit(1, digits[15], B00000000);
        DisplayDigit(2, digits[15], B00000000);
        DisplayDigit(3, B00000000, B00000000);
      }
      
      DisplayOff();                                //Zet de display uit
      
      //Maak 3 geluiden, afloped in frequentie
      Beep(440);
      delay(500);
      Beep(330);
      delay(500);
      Beep(220);
      
      pit = 0;                                     //Zet de geselecteerde pit op 0
      lock = 0;                                    //Zet het kinderslot uit(staat al uit, als die aan staat, dan kan deze knop niet zijn ingedrukt)
      
      for (int i = 0; i < 8; i++)                  //Zet de temperatuur van elke pit op 0
        temp[i] = 0;
      
      bOff = 0;                                    //Zet het aantal loops{1} weer op 0
    }

    bOff++;                                        //Zet het aantal loops{1} 1 hoger
  }
}

//Functie om de pit selectie knoppen te lezen
void CheckPit()
{
  if (analogRead(buttonPitUp) > 1000)
  {
    if (bPitUp == 50)
    {
      pit++;                              //Zet de pit 1 hoger
      
      if (pit > 7)                        //Als je bij de laatste pit bent
        pit = 0;                          //Dan ben je weer bij het begin
      
      Beep(440);                          //Maak een geluidje
      
      bPitUp = 0;
    }

    bPitUp++;
  }
  
  if (analogRead(buttonPitDown) > 1000)
  {
    if (bPitDown == 50)
    {
      pit--;                              //Zet de pit 1 lager
      
      if (pit < 0)                        //Als je bij de eerste pit bent
        pit = 7;                          //Dan ben je weer aan het einde
      
      Beep(330);                          //Maak een geluidje
      
      bPitDown = 0;
    }
    
    bPitDown++;
  }
}

//Functie om de kinderslot knoppen te lezen
void CheckLock()
{
  if (digitalRead(buttonSafeOn) == HIGH && !lock)    //Als de knop is ingedrukt en het kinderslot uit staat
  {
    if (bSafeOn == 100)
    {
      lock = true;                                   //Zet het kinderslot aan
      
      //Maak twee geluidjes
      Beep(440);
      delay(300);
      Beep(440);
      
      //Laat de woorden 'SAFE ON' zien
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
  
  if (analogRead(buttonSafeOff) > 1000 && lock)      //Als de knop is ingedrukt en het kinderslot aan staat
  {
    if (bSafeOff == 50)
    {
      lock = false;                                  //Zet het kinderslot uit
      
      //Laat de woorden 'SAFE OFF' zien
      for (int i = 0; i < 100; i++)
      {
        DisplayDigit(0, digits[5], digits[0]);
        DisplayDigit(1, digits[10], digits[15]);
        DisplayDigit(2, digits[15], digits[15]);
        DisplayDigit(3, digits[14], B00000000);
      }
      
      //Maak twee geluidjes
      Beep(330);
      delay(300);
      Beep(330);
      
      bSafeOff = 0;
    }

    bSafeOff++;
  }
}

//Functie om een geluidje te maken
void CheckBeep()
{
  //Als een knop die niet de kinderslot uit knop is is ingedrukt
  if (analogRead(buttonPitUp) > 1000 || analogRead(buttonTempUp) > 1000 || digitalRead(buttonOn) == HIGH || digitalRead(buttonSafeOn) == HIGH || analogRead(buttonTempDown) > 1000 || analogRead(buttonPitDown) > 1000 || analogRead(buttonOff) > 1000)
  {
    Beep(220);  //Dan maakt die een geluidje
  }
}

//Functie om de led lampjes te laten branden
void Leds()
{  
  for (int l = 0; l < 8; l++)            //Voor elke pit, laat de led branden
  {
    if (temp[l] > 0)                     //Als de temperatuur hoger is dan 0
      AnalogWrite(ledPin[l], temp[l]);   //Dan brandt de led
  }
}

//Functie om de led lampjes te dimmen zonder analog output
void AnalogWrite(int pin, int level)
{
  digitalWrite(pin, HIGH);       //Zet de pin hoog
  delayMicroseconds(level * 8);  //Wacht een aantal microseconden
  digitalWrite(pin, LOW);        //Zet de pin laag
}

/*
  {1} loops: Het aantal keer dat 'void loop()' is uitgevoerd
*/

