# include <IRremote.h> 
int RECV_PIN = 11; // define input pin on Arduino  
IRrecv irrecv (RECV_PIN); 
decode_results results; 
int testValue = 0;
long tNow;
bool Continue = false;
int timeOut = 50;
int ZERO = 26775;
int ONE = 12495;
int TWO = 6375;
int THREE = 31365;
int FOUR = 4335;
int FIVE = 14535;
int SIX = 23205;
int SEVEN = 17085;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  irrecv.enableIRIn (); // Start the receiver 
}

void loop()
{
  /*int longString = 0;
  testValue = analogRead(A0);
  if (testValue < 800)
  { 
    while(testValue < 800)
    {
      testValue = analogRead(A0);
    }
    Continue = true;
    while (Continue)
    {
      testValue = analogRead(A0);
      Serial.print((testValue+200)/1024);
      if (testValue > 800)
      {
        longString++;
      }
      else
      {
        longString = 0;
      }
      
      if (longString > 30)
      {
        Continue = false;
        Serial.println('\n');
      }
    }
  }*/
  if (irrecv.decode (& results))  
  { 
    int Signal = results.value;
    Serial.println(Signal);
    //Serial.println (results.value), HEX); 
    if (Signal == ONE)
    {
      Lights(1,0,0,0);
    }else if (Signal == TWO)
    {
      Lights(0,1,0,0);
    }else if (Signal == THREE)
    {
      Lights(0,0,1,0);
    }else if (Signal == FOUR)
    {
      Lights(0,0,0,1);
    }else if (Signal == ZERO)
    {
      Lights(0,0,0,0);
    }else if (Signal == FIVE)
    {
      Lights(1,1,1,1);
    }else if (Signal == SIX)
    {
      timeOut = 50;
      Lights(1,0,0,0);
      Lights(0,1,0,0);
      Lights(0,0,1,0);
      Lights(0,0,0,1);
      Lights(0,0,1,0);
      Lights(0,1,0,0);
      Lights(1,0,0,0);
      digitalWrite(44, LOW);
    }else if(Signal == SEVEN)
    {
      timeOut = 200;
      Lights(1,0,0,0);
      Lights(0,1,0,0);
      Lights(1,0,0,0);
      Lights(0,0,1,0);
      Lights(1,0,0,0);
      Lights(0,0,0,1);
      Lights(1,0,0,0);
      Lights(0,0,1,0);
      Lights(1,0,0,0);
      Lights(0,1,0,0);
      Lights(1,0,0,0);
      Lights(0,0,0,0);
      Lights(0,1,1,0);
      Lights(1,0,0,1);
      Lights(0,0,0,0);
      Lights(1,1,1,1);
    }
    irrecv.resume (); // Receive the next value 
  } 
}

void Lights(int a,int b,int c,int d)
{
  if (a==1)
  {
  digitalWrite(44,HIGH);
  }else{
    digitalWrite(44, LOW);
  }
  if (b==1)
  {
    digitalWrite(45, HIGH);
  }
  else{
    digitalWrite(45, LOW);
  }
  if (c==1)
  {
    digitalWrite(46, HIGH);
  }
  else{
    digitalWrite(46, LOW);
  }
  if (d==1)
  {
    digitalWrite(47, HIGH);
  }
  else{
    digitalWrite(47, LOW);
  }
  delay(timeOut);
}
