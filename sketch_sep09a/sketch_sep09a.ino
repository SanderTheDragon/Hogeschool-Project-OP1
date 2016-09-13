const int numbers[11][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },    //0
  { 0, 1, 1, 0, 0, 0, 0 },    //1
  { 1, 1, 0, 1, 1, 0, 1 },    //2
  { 1, 1, 1, 1, 0, 0, 1 },    //3
  { 0, 1, 1, 0, 0, 1, 1 },    //4
  { 1, 0, 1, 1, 0, 1, 1 },    //5
  { 1, 0, 1, 1, 1, 1, 1 },    //6
  { 1, 1, 1, 0, 0, 0, 0 },    //7
  { 1, 1, 1, 1, 1, 1, 1 },    //8
  { 1, 1, 1, 1, 0, 1, 1 },    //9
  { 0, 0, 0, 0, 0, 0, 0 }     //Uit
};

const int segments[7] = { 9, 2, 3, 5, 6, 8, 7 };

const int digits[4] = { 10, 11, 12 ,13 };

void setup()
{
  for (int i = 2; i <= 13; i++)
    pinMode(i, OUTPUT);
    
  digitalWrite(digits[0], HIGH);
  digitalWrite(digits[1], HIGH);
  digitalWrite(digits[2], HIGH);
  digitalWrite(digits[3], HIGH);
}

void loop()
{
  const int n[4] = { 1, 2, 3, 4 };
  showDigits(n);
}

void showDigits(const int* num)
{
  for (int i = 0; i < 4; i++)
  {
    showDigit(numbers[num[i]], digits[i]);
  }
}

void showDigit(const int* num, const int dig)
{
  digitalWrite(dig, LOW);
  
  for (int i = 0; i <= 7; i++)
  {
    digitalWrite(segments[i], num[i]);
  }
  
  digitalWrite(dig, HIGH);
}
