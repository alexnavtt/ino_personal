#define timeStep 1

int setPoint = 100;
int error_prior = 0;
float Kp = 0.05;
float Kd = 0.01;
float Ki = 0.01;
int integral = 0;
int derivative = 0;
long T1 = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop() 
{
  //---------------------PID Loop---------------------------//
  int LightLevel = analogRead(A0);
  int error = setPoint - LightLevel;
  
  integral += error*timeStep;
  derivative = (error - error_prior)/timeStep;
  float output = Kp*error + Kd*derivative + Ki*integral;
  
  if (output < 0)
  {
    output = 0;
  }else if(output > 1023)
  {
    output = 1023;
  }
  
  error_prior = error;
  analogWrite(9, 200-700.0*(output/1023.0));
  
  if (abs(setPoint - LightLevel) < 50)
  {
    Serial.println("All Good");
  }else{
    Serial.println(error);
  }
  //Serial.println(analogRead(A0));
  delay(timeStep);
  //--------------------------------------------------------//
}
