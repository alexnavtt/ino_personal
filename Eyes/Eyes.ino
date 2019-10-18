//www.elegoo.com
//2016.12.08
#include <Servo.h>
#include "SR04.h"
#define TRIG_PIN 51
#define ECHO_PIN 53
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
Servo myservo;

long a;
bool found = false;
bool following = false;
long found_time;
long trueFoundTime;
int surroundings[180];

int pos = 0;
int still_pos = 0;
int ref_distance = 0;

int threshold = -40;
int followThreshold = 30;
int Speed = 1;
int sensitivity = 5;

int count = 0;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
  delay(1000);
  scan(0,180);
}

void loop() {
  count = 0;
  for (pos = 0; pos <= 180; pos += Speed) 
  {
    myservo.write(pos);              
    a=sr04.Distance();
    Serial.print(a);
    Serial.print("cm\t Difference: ");
    Serial.println(a - surroundings[pos]);
    //delay(1000);
    delay(2);
    if (a - surroundings[pos]<threshold)
    {
      count += 1;
      if (count > sensitivity)
      {
        found = true;
        trueFoundTime = millis();
        found_time = millis();
        still_pos = pos;
        break; 
      }
    }else{
        count = 0;
    }                                            
  }
  count = 0;
  if (not found)
  {
    for (pos = 180; pos >= 0; pos -= Speed) 
    {
      myservo.write(pos);              
      a=sr04.Distance();
      Serial.print(a);
      Serial.print("cm\t Difference: ");
      Serial.println(a - surroundings[pos]);
      delay(2);
      if (a - surroundings[pos]<threshold)
      {
        count += 1;
        if (count > sensitivity)
        {
          found = true;
          trueFoundTime = millis();
          found_time = millis();
          still_pos = pos;
          break;
        }                       
      }else{
        count =0;
      }
    }
  }
  while (found)
  {
    myservo.write(still_pos);
    a=sr04.Distance();
    Serial.println(a);
    if ((a - surroundings[still_pos] > threshold) and (millis()-found_time > 300))
    {
      found = false;
      for (pos = still_pos; pos>=0; pos -= 3)
      {
        myservo.write(pos);
        delay(15);
      }
    }else if (a - surroundings[still_pos] < threshold)
    {
      found_time = millis();
    }
    if (millis() - trueFoundTime > 1000)
    {
      following = true;
      ref_distance = sr04.Distance();
      while (following)
      {
        follow(still_pos);
      }
    }
    /*if (millis() - trueFoundTime > 10000)
    {
      found = false;
      scan(still_pos - 15, still_pos + 15);
      Serial.println("Changed");
    }*/
  }
}

void scan(int theta1,int theta2)
{
  int sum = 0;
  theta1 = max(theta1, 0);
  theta2 = min(theta2, 180);
  myservo.write(theta1);
  delay(500); 
  for (pos = theta1; pos <= theta2; pos += 1)
  {
    myservo.write(pos);  
    for (int i=0; i<10; i++)
    {            
      a=sr04.Distance();
      sum += a;
      if (a > 1000)
      {
        sum -= a;
        i -= 1;
      }
    }
    Serial.print(sum/10);
    Serial.println("cm");
    surroundings[pos] = sum/10;
    //delay(1000);
    delay(2);  
    sum = 0;                                       
  }
  /*for (pos = theta2; pos >= theta1; pos -= 1)
  {
    myservo.write(pos);              
    a=sr04.Distance();
    Serial.print(a);
    Serial.println("cm");
    surroundings[pos] = (surroundings[pos]+a)/2;
    delay(2);                                         
  }*/
}

void follow(int startPoint)
{
  int iterationNo = 1;
  Serial.println("Following");
  while (following)
  {
    int offset = abs(sr04.Distance() - ref_distance);
    while (offset < followThreshold)
    {
      offset = abs(sr04.Distance() - ref_distance);
      Serial.print("Offset: "); Serial.println(offset);
      Serial.println("All good");
      delay(10);
    }
    Serial.println("Searching");
    myservo.write(startPoint + 10*iterationNo);
    int test_offset = abs(sr04.Distance() - ref_distance);
    if (test_offset < followThreshold)
     {
      break;
     }else{
      delay(10);
      myservo.write(startPoint - 10*iterationNo);
      test_offset = abs(sr04.Distance() - ref_distance);
      if (test_offset < followThreshold)
      {
        break;
      }else if (iterationNo == 3)
      {
        following = false;
        break;
      }
     }
     iterationNo++;
  }
}
