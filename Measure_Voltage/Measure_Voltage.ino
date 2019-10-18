int Num_Samples = 1;
#define V_ref 5

unsigned long sum = 0;
int sample_count = 0;
float voltage = 0.0;
float t_init =0.0;
float t_now = 0.0;
int key = 0;
int i = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  //analogWrite(9, 80);
}

/*void loop()
{
  while(key == 0)
  {
    if(Serial.available() > i)
    {
      key = 1;
      t_init = millis();
    }
  }

  while(millis() - t_init < 3000)
  {
    t_now = millis();
    sample_count = 0;
    sum = 0;
    
    while (sample_count < Num_Samples) 
    {
      sum += analogRead(A0);
      sample_count++;
      delay(1);
    }
    voltage = ((float)sum/float(Num_Samples)*V_ref)/1024;
    Serial.print(voltage);
    Serial.print('\n');
  }
  key = 0;
  i=i+2;
}*/

void loop()
{
  sample_count = 0;
  sum = 0;
  
  while (sample_count < Num_Samples) 
  {
    sum += analogRead(A3);
    sample_count++;
    delayMicroseconds(1000);
  }
  voltage = ((float)sum/float(Num_Samples)*V_ref)/1024;
  Serial.print(voltage);
  Serial.print('\n');
  key = 0;
  i=i+2;
}
