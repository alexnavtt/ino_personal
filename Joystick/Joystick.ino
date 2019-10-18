int xVal = 512;
int yVal = 512;
int zVal = 0;
bool Stuck = false;

void setup() 
{
  Serial.begin(9600);
  pinMode(5, INPUT);
}

void loop() 
{
  if (not Stuck)
  {
    xVal = analogRead(A9);
    //yVal = analogRead(A8);
  }
  zVal = analogRead(A10);
  Serial.print("X:");
  Serial.print(xVal);
  //Serial.print(" | Y:");
  //Serial.print(yVal);
  Serial.print(" | Z:");
  Serial.print(zVal);
  Serial.print('\n');
  delay(30);
  analogWrite(5, xVal/4.0);
  if (zVal == 0)
  {
    if (Stuck)
    {
      Stuck = false;
    }
    else
    {
      Stuck = true;
    }
    delay(500);
  }
}
