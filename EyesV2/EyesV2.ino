#include <Servo.h>
#include "SR04.h"
#define TRIG_PIN 51
#define ECHO_PIN 53
#define LED_PIN 32
SR04 cam = SR04(ECHO_PIN,TRIG_PIN);
Servo myservo;

int threshold = 130;
float surroundings[180];
int interval = 2;
int OldSensitivity = 15;
int NewSensitivity = 5;
bool following = false;
bool Dir1 = true;
int object;

void setup() 
{
  myservo.attach(9);
  Serial.begin(9600);
  delay(1000);
  scan();
}

void loop() 
{
  int myCount = 0;
  for (int pos = 0; pos < 180; pos+= interval)
  {
    myservo.write(pos);
    float distance = cam.Distance();
    //Serial.print("New Distance: "); Serial.print(distance);
    //Serial.print("\tSurroundings: "); Serial.println(surroundings[pos]);
    if (surroundings[pos] == 3000 and threshold - distance > NewSensitivity)
    {
      myCount ++;
      if (myCount >= 3)
      {
        if (check(pos) and check(pos+1))
        {
          digitalWrite(LED_PIN, HIGH);
          Serial.println("Object Detected");
          following = true;
          object = pos;
          while(following)
          {
            follow(object);
          }
        }
      }
    }else if(surroundings[pos] < 3000 and (surroundings[pos] - distance > OldSensitivity))
    {
      myCount ++;
      if (myCount >= 3)
      {
        if (check(pos) and check(pos+1))
        {
          digitalWrite(LED_PIN, HIGH);
          Serial.println("Object Detected");
          following = true;
          object = pos;
          while(following)
          {
            follow(object);
          }
        }
      }
    }else{
      myCount = 0;
      digitalWrite(LED_PIN, LOW);
      Serial.println("Nothing");
    }
  }
  myCount = 0;
  for (int pos = 180; pos > 0; pos-= interval)
  {
    myservo.write(pos);
    float distance = cam.Distance();
    //Serial.print("New Distance: "); Serial.print(distance);
    //Serial.print("\tSurroundings: "); Serial.println(surroundings[pos]);
    if (surroundings[pos] == 3000 and threshold - distance > NewSensitivity)
    {
      myCount ++;
      if (myCount >= 3)
      {
        if (check(pos) and check(pos+1))
        {
          digitalWrite(LED_PIN, HIGH);
          Serial.println("Object Detected");
          following = true;
          object = pos;
          while(following)
          {
            follow(object);
          }
        }
      }
    }else if(surroundings[pos] < 3000 and (surroundings[pos] - distance > OldSensitivity))
    {
      myCount ++;
      if (myCount >= 3)
      {
        if (check(pos) and check(pos+1))
        {
          digitalWrite(LED_PIN, HIGH);
          Serial.println("Object Detected");
          following = true;
          object = pos;
          while(following)
          {
            follow(object);
          }
        }
      }
    }else{
      myCount = 0;
      digitalWrite(LED_PIN, LOW);
      Serial.println("Nothing");
    }
  }
}

void scan()
{
  for (int pos = 0; pos < 180; pos++)
  {
    myservo.write(pos);
    float distance = 0;
    bool object  = false;
    for (int i = 0; i<5; i++)
    {
      distance = cam.Distance();
      surroundings[pos] = distance;
      if (distance - threshold < 10)
      {
        surroundings[pos] = min(surroundings[pos],distance);
        object = true;
        Serial.print(pos); Serial.print(" Degrees \t");
        Serial.print("object at "); Serial.print(distance); Serial.println(" cm");
      }
    }
    if (not object)
    {
      surroundings[pos] = 3000;
      Serial.print(pos); Serial.print(" Degrees \t");
      Serial.println("No object");
    }
  }
}

bool check(int pos)
{
  int goodCount = 0;
  myservo.write(pos);
  for (int i = 0; i<3; i++)
  {
    int distance = cam.Distance();
    if (surroundings[pos] == 3000 and threshold - distance > NewSensitivity)
    {
      goodCount ++;
    }else if(surroundings[pos] < 3000 and (surroundings[pos] - distance > OldSensitivity))
    {
      goodCount ++;
    }
  }
  if (goodCount == 3)
  {
    return true;
  }else{
    return false;
  }
}

void follow(int pos)
{
  following = false;
  while(check(pos))
  {
    delay(5);
  }
  if (Dir1)
  {
    Serial.println("Dir1");
    for (int i = 1; i<=5; i++)
    {
      if (not following)
      {
        if (check(pos + 10*i) and check(pos + 10*(i+1)))
        {
          object = pos + 10*(i+1);
          following = true;
          break;
        }
        delay(20);
       }
    }
    for (int i = 1; i<=5; i++)
    {
      if (not following)
      {
        if (check(pos - 10*i) and check(pos - 10*(i+1)))
        {
          object = pos - 10*(i+1);
          following = true;
          Dir1 = false;
          break;
        }
        delay(20);
      }
    }
  }else{
    Serial.println("Dir2");
    for (int i = 1; i<=5; i++)
    {
      if (not following)
      {
        if (check(pos - 10*i) and check(pos - 10*(i+1)))
        {
          object = pos - 10*(i+1);
          following = true;
          break;
        }
        delay(20);
       }
    }
    for (int i = 1; i<=5; i++)
    {
      if (not following)
      {
        if (check(pos + 10*i) and check(pos + 10*(i+1)))
        {
          object = pos + 10*(i+1);
          following = true;
          Dir1 = true;
          break;
        }
        delay(20);
      }
    }
  }
}
