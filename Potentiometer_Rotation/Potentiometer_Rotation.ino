float vmin = 94;
float vmax = 1021;
float vcheck = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() 
{
  float vdif = analogRead(A0);
  if (abs(vdif - vcheck)> 50)
  {
    Serial.print(vdif/1024.0); Serial.print("volts \t");
    Serial.print(250.0*(vdif-vmin)/(vmax-vmin));
    Serial.println(" degrees");
    delay(200);
  }
  vcheck = vdif;
  delay(1000);
}
