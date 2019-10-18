#include <ArduinoJson.h>

int Sleep_Schedule[] = {0,0,1,35,15};
int Day_Schedule[] = {0,0,0,0,0}
bool OVERRIDE = false;
long T_start_night = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(6,OUTPUT);
}

void loop() 
{
  readInput();
  delay(200);
  updateSleep();
}

void readInput()
{
  if (Serial.available())
  {
    String test = Serial.readString();
    Serial.println(test);
    
    const int capacity = JSON_OBJECT_SIZE(20);
    StaticJsonBuffer<capacity+100> jb;
    JsonObject& command = jb.parseObject(test);
    
    if (command.success())
    {
      Serial.println("Success!");
    }
    else
    {
      Serial.println("Parsing failed");
    }
    
    String NAME = command["name"];
    //Serial.println(NAME);

    if (NAME == "F") //Command comes from the front page
    {
      int STATE = command["State"];
      int LEVEL = command["Level"];
      if(STATE == 1)
      {
        int PWM_level =51*LEVEL;
        Serial.println("Turned on at intesity " + String(LEVEL));
        Serial.println(PWM_level);
        analogWrite(6, PWM_level);
        OVERRIDE = true;
      }
      else if(STATE == 0)
      {
        Serial.println("Turned off");
        analogWrite(6, 0);
        OVERRIDE = false;
      }
    }
    else if (NAME == "S") //Command comes from Sleep page
    {
      if( command["act"])
      {
        Sleep_Schedule[0]=1;
      }else{
        Sleep_Schedule[0]=0;
      }
      if( command["tpr"])
      {
        Sleep_Schedule[1]=1;
      }else{
        Sleep_Schedule[1]=0;
      }
     Sleep_Schedule[2] = command["lvl"];
     Sleep_Schedule[3] = command["dur"];
     Sleep_Schedule[4] = command["Dlay"];
     T_start_night = millis();
    }
    else if (NAME == "U")
    {
      Sleep_Schedule[2] = command["lvl"];
    }
  }
}

void updateSleep()
{
  if((Sleep_Schedule[0] == 1) and not OVERRIDE)
  {
    Day_Schedule[0] = 0;
    int level = Sleep_Schedule[2];
    int dur = Sleep_Schedule[3];
    int Delay = Sleep_Schedule[4];
    long T_end_night = T_start_night + dur*60000;
    if (Sleep_Schedule[1] == 0)
    {
      analogWrite(6, level*51);
    }
    else if(Sleep_Schedule[1] == 1)
    {
      long taper_time = T_start_night + Delay*60000;
      long T_now = millis();
      if(T_now < taper_time)
      {
        analogWrite(6, level*51);
        Serial.println(level);
      }
      else
      {
        float prop = 1-(T_now-taper_time)/(1.0*(T_end_night-taper_time));
        float tmp_level = level*prop;
        analogWrite(6, max(51*tmp_level,0));
        Serial.println(tmp_level);
      }
    }
    if(millis()>T_end_night)
    {
      analogWrite(6, 0);
      Sleep_Schedule[0] = 0;
    }
  }
}
