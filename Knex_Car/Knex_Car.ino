#define LED1 45
#define LightSense A0

void setup()
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  //pinMode(LED2, OUTPUT);
}

void loop() 
{
  int LightReading = analogRead(LightSense);
  Serial.println(LightReading);
  if (LightReading > 500)
  {
    digitalWrite(LED1, HIGH);
  }else{
    digitalWrite(LED1, LOW);
  }
}
