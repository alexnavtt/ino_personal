#define Ain 5
#define Aout 6
#define Bin 9
#define Bout 10
#define Cin 11
#define Cout 13
#define senseA A0
#define senseB A1
#define senseC A2
#define ON 255
#define OFF 0

bool startup = true;
bool A_null_1 = true;
bool B_null_1 = false;
bool C_null_1 = false;
bool A_null_2 = false;
bool B_null_2 = false;
bool C_null_2 = false;
float DcV = 5;

void setup() 
{
  pinMode(Ain, OUTPUT);
  pinMode(Aout, OUTPUT);
  pinMode(Bin, OUTPUT);
  pinMode(Bout, OUTPUT);
  pinMode(Cin, OUTPUT);
  pinMode(Cout, OUTPUT);
  analogWrite(Ain, OFF);
  analogWrite(Aout, OFF);
  analogWrite(Bin, OFF);
  analogWrite(Bout, OFF);
  analogWrite(Cin, OFF);
  analogWrite(Cout, OFF);
}

void loop()
{
 while(startup)
 {
  analogWrite(Cin, ON);
  analogWrite(Bout, ON);
  delay(5);
  analogWrite(Cin, OFF);
  analogWrite(Ain, ON);
  delay(5);
  analogWrite(Bout, OFF);
  analogWrite(Cout, ON);
  delay(5);
  analogWrite(Ain, OFF);
  analogWrite(Bin, ON);
  delay(5);
  analogWrite(Cout, OFF);
  analogWrite(Aout, ON);
  delay(5);
  analogWrite(Bin, OFF);
  analogWrite(Cin, ON);
  delay(5);
  float A_BackEMF = 5*analogRead(senseA)/1024;
  if (A_BackEMF > 0.3*DcV)
  {
    analogWrite(Ain, OFF);
    analogWrite(Aout, OFF);
    analogWrite(Bin, OFF);
    analogWrite(Cout, OFF);
    delayMicroseconds(500);
    analogWrite(Bout, ON);
    analogWrite(Cin, ON);
    startup = false;
  }
 }

 if(A_null_1)
 {
  float Astate = 5*analogRead(senseA)/1024;
  if (Astate > 0.5*DcV)
  {
    A_null_1 = false;
    C_null_1 = true;
    analogWrite(Cin, OFF);
    analogWrite(Ain, ON);
  }
 }
 else if(C_null_1)
 {
  float Cstate = 5*analogRead(senseC)/1024;
  if (Cstate > 0.5*DcV)
  {
    B_null_1 = true;
    C_null_1 = false;
    analogWrite(Bout, OFF);
    analogWrite(Cout, ON);
  }
 }
 else if(B_null_1)
 {
  float Bstate = 5*analogRead(senseB)/1024;
  if (Bstate > 0.5*DcV)
  {
    B_null_1 = false;
    A_null_2 = true;
    analogWrite(Ain, OFF);
    analogWrite(Bin, ON);
  }
 }
 else if(A_null_2)
 {
  float Astate = 5*analogRead(senseA)/1024;
  if (Astate > 0.5*DcV)
  {
    A_null_2 = false;
    C_null_2 = true;
    analogWrite(Bout, OFF);
    analogWrite(Aout, ON);
  }
 }
 else if(C_null_2)
 {
  float Cstate = 5*analogRead(senseC)/1024;
  if (Cstate > 0.5*DcV)
  {
    C_null_2 = false;
    B_null_2 = true;
    analogWrite(Bin, OFF);
    analogWrite(Cin, ON);
  }
 }
 else if(B_null_2)
 {
  float Bstate = 5*analogRead(senseB)/1024;
  if (Bstate > 0.5*DcV)
  {
    B_null_2 = false;
    A_null_1 = true;
    analogWrite(Aout, OFF);
    analogWrite(Bout, ON);
  }
 }
}
