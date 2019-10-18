void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int reading = analogRead(A1);
  Serial.println(reading);
  if (reading > 150)
  {
    digitalWrite(4, HIGH);
  }else{
    digitalWrite(4, LOW);
  }
}
