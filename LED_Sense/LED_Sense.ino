# include <IRremote.h> 
int RECV_PIN = 11; // define input pin on Arduino  
IRrecv irrecv (RECV_PIN); 
decode_results results; 
int testValue = 0;
long tNow;
String mode = "controller";
bool Continue = false;
int timeOut = 50;
int fanLevel = 128;
int fanState = 0;
int ZERO = 26775;
int ONE = 12495;
int TWO = 6375;
int THREE = 31365;
int FOUR = 4335;
int FIVE = 14535;
int SIX = 23205;
int SEVEN = 17085;
int EIGHT = 19125;
int NINE = 21165;
int PLAY = 765;
int VOL_UP = 25245;
int VOL_DN = -22441;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  irrecv.enableIRIn (); // Start the receiver 
}

void loop()
{
  if (irrecv.decode (& results))
  {
    int Signal = results.value;
    if (Signal == PLAY)
    {
      if (fanState == 0)
      {
        analogWrite(5, fanLevel);
      }
      else
      {
        analogWrite(5, 0);
      }
    fanState = 1-fanState;
    }else if (Signal == VOL_UP)
    {
      fanLevel += 32;
      fanLevel = min(fanLevel, 255);
      analogWrite(5, fanLevel);
    }else if (Signal == VOL_DN)
    {
      fanLevel -= 32;
      fanLevel = max(fanLevel, 0);
      analogWrite(5, fanLevel);
    }
  }
  if ((irrecv.decode (& results)) & (mode == "controller"))
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
      timeOut = 100;
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
    }else if(Signal == EIGHT)
    {
      mode = "lights";
    }
    irrecv.resume (); // Receive the next value  
  }
  if (mode == "lights")
  {
    if (irrecv.decode (& results)) 
    {
      int Signal = results.value;
      Serial.println(Signal);
      if (Signal == NINE)
      {
        mode = "controller";
      }
      irrecv.resume (); // Receive the next value
    }
      
    int sum = 0;
    for (int i=0; i < 10; i++)
    {
      testValue = analogRead(A1);
      sum += testValue;
    }
    float finalTestValue = sum/10.0;
    //Serial.println(finalTestValue);
    if (finalTestValue > 750)
      {
         Lights(1,1,1,1);
      }
    else
      {
        Lights(0,0,0,0);
      }  
  }
}

int Lights(int a,int b,int c,int d)
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
