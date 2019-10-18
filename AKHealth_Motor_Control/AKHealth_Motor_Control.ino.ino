#include <ArduinoJson.h>
#define Enable 7

//For Sleep_Schedule: {active, taper, intensity, duration, delay}
int Sleep_Schedule[] = {0,0,1,35,15}; 
//For Day_Schedule: {active, on_time, off_time, intensity, now_time}
int Day_Schedule[] = {0,0,0,0,0}; 
//For Custom Schedule: {{start_time, end_time, intensity}}
unsigned long Custom_Schedule[][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
//For Weekdays: {{Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday}}
bool Weekdays[5][7];

bool OVERRIDE = false;
float T_NOW = 0;
int week_index = 0;
unsigned long LAST_TIME = 0;
unsigned long T_NOW_MILLIS = 0;
unsigned long T_start_night = 0;
unsigned long T_start_day = 0;
bool KEY = false;
bool CHARGING = false;
int HIGHEST_INDEX = 0;
int ACTIVE_CUSTOM = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(Enable, OUTPUT);
  for (int i=0; i<7; i++) {
    for (int j=0; j<5; j++) {
      Weekdays[i][j] = false;
    }
  }
  Serial.println("SETUP");
}

void loop() 
{
  delay(100);
  checkCharging();
  if(not CHARGING)
  {
    readInput();
    if(not OVERRIDE)
    {
      updateSleep();
      updateDay();
      updateCustom();
    }
  }
  T_NOW_MILLIS += (millis() - LAST_TIME);
  LAST_TIME = millis();
  if (T_NOW_MILLIS > 86400000)
  {
    T_NOW_MILLIS += -86400000;
    week_index++;
    if (week_index == 7)
    {
      week_index = 0;
    }
  }
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
      String NAME = command["name"];
      //Serial.println(NAME);
    
      // ----------Daytime Schedule------------------------
      
      if (NAME == "D") //Command from Daytime Page
      {
        Day_Schedule[0] = command["A"];
        Day_Schedule[1] = command["D"];
        Day_Schedule[2] = command["I"];
        Day_Schedule[3] = command["L"];
        //int NOW = command["N"];
        //int Start_Time = T_NOW_MILLIS;
        T_start_day = T_NOW_MILLIS;
        if((not OVERRIDE) and (Day_Schedule[0]==0))
        {
          analogWrite(6,0);
          Serial.println("Turned off");
        }
      }
      else if (NAME == "DU")
      {
        Day_Schedule[3] = command["L"];
      }
      
      // ----------Front Page----------------------------
      
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
      
      // ----------Sleep Schedule-------------------------
      
      if (NAME == "S") //Command comes from Sleep page
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
       T_start_night = T_NOW_MILLIS;
       if (Sleep_Schedule[0]==0 and not OVERRIDE)
       {
        digitalWrite(6, 0);
        Serial.println("Sleep turned off.");
       }
      }
      
      if (NAME == "U")
      {
        Sleep_Schedule[2] = command["lvl"];
      }
      
      // ----------Custom Schedule------------------------
      
      if (NAME == "C") 
      {
        HIGHEST_INDEX = command["I"]; 
        HIGHEST_INDEX += -1; //-1 to account for Lua indexing from 1
        Custom_Schedule[HIGHEST_INDEX][0] = command["S"];
        Custom_Schedule[HIGHEST_INDEX][1] = command["E"];
        Custom_Schedule[HIGHEST_INDEX][2] = command["L"];
        Weekdays[HIGHEST_INDEX][0] = false;
        Weekdays[HIGHEST_INDEX][1] = false;
        Weekdays[HIGHEST_INDEX][2] = false;
        Weekdays[HIGHEST_INDEX][3] = false;
        Weekdays[HIGHEST_INDEX][4] = false;
        Weekdays[HIGHEST_INDEX][5] = false;
        Weekdays[HIGHEST_INDEX][6] = false;
      }
      else if (NAME == "W")
      {
        int Schedule_index = command["I"];
        Schedule_index--;
        Weekdays[Schedule_index][0] = command["Su"];
        Weekdays[Schedule_index][1] = command["M"];
        Weekdays[Schedule_index][2] = command["T"];
        Weekdays[Schedule_index][3] = command["W"];
        Weekdays[Schedule_index][4] = command["Th"];
        Weekdays[Schedule_index][5] = command["F"];
        Weekdays[Schedule_index][6] = command["S"];
      }
      else if (NAME == "R")
      {
        int Removed_Index = command["index"];// -1 accounted for in Lua code
        if(Weekdays[Removed_Index][week_index])
        {
          analogWrite(6,0);
          Serial.println("Custom Off");
        }
        HIGHEST_INDEX = command["Highest"];
        HIGHEST_INDEX--;
        if (ACTIVE_CUSTOM == Removed_Index)
        {
          analogWrite(6,0);
          Serial.println("Cancelled");
        }
        for (int i=Removed_Index; i<HIGHEST_INDEX+1; i++)
        {
          for (int j=0; j<7; j++)
          {
            Weekdays[i][j] = Weekdays[i+1][j];
          }
          for (int j=0; j<3; j++)
          {
            Custom_Schedule[i][j] = Custom_Schedule[i+1][j];
          }
        }
      }
  
      if (NAME == "T")
      {
        T_NOW = command["now"];
        T_NOW_MILLIS = T_NOW*1000;
        LAST_TIME = millis();
        week_index = command["day"];
        week_index += -1;
      }
    }
    else
    {
      Serial.println("Parsing failed");
    }
  }
}

void updateSleep()
{
  if((Sleep_Schedule[0] == 1))
  {
    Day_Schedule[0] = 0;    //Turning off daytime schedule
    int level = Sleep_Schedule[2];
    int dur = Sleep_Schedule[3];
    int Delay = Sleep_Schedule[4];
    long T_end_night = T_start_night + (dur+Delay)*60000;
    if (Sleep_Schedule[1] == 0)
    {
      analogWrite(6, level*51);
    }
    else if(Sleep_Schedule[1] == 1)
    {
      long taper_time = T_start_night + Delay*60000;
      if(T_NOW_MILLIS < taper_time)
      {
        analogWrite(6, level*51);
        //Serial.println(level);
      }
      else
      {
        float prop = 1-(T_NOW_MILLIS-taper_time)/(1.0*(T_end_night-taper_time));
        float tmp_level = level*prop;
        analogWrite(6, max(51*tmp_level,0));
        //Serial.println(tmp_level);
      }
    }
    if(T_NOW_MILLIS>T_end_night)
    {
      analogWrite(6, 0);
      Sleep_Schedule[0] = 0;
      Serial.println("Sleep Schedule Finished");
    }
  }
}

void updateDay()
{
  if (Day_Schedule[0] == 1)
  {
    int duration = Day_Schedule[1];
    int interval = Day_Schedule[2];
    int level = Day_Schedule[3];
    long Duration_Start = T_start_day;
    long Duration_End = T_start_day + interval*60000;
    int i = 0;
    bool In_Section = false;
    
    while ((Duration_Start - T_start_day) < 12*60*60000)
    {
      Duration_Start = T_start_day + i*(interval + duration)*60000;
      Duration_End = Duration_Start + duration*60000;
      if (Duration_Start > 86400000)
      {
        Duration_Start += -86400000;
      }
      if (Duration_End > 86400000)
      {
        Duration_End += -86400000;
      }
      
      if ((T_NOW_MILLIS > Duration_Start) and (T_NOW_MILLIS < Duration_End))
      {
        In_Section = true;
        analogWrite(6, level*51);
        //Serial.println(level);
        KEY = true;
        break;
      }
      i++;
    }
    if ( not In_Section )
    {
      analogWrite(6, 0);
      Serial.println("0");
    }
    if ((T_NOW_MILLIS - T_start_day > 12*60*60000) and KEY)
    {
      Day_Schedule[0] = 0;
      Serial.println("Schedule Finished");
      KEY = false;
    }
  }
}

void updateCustom()
{
  for (int i=0; i<(HIGHEST_INDEX+1); i++)
  {
    unsigned long customStart = Custom_Schedule[i][0] * 60000;
    unsigned long customEnd = Custom_Schedule[i][1] * 60000;
    int customLevel = Custom_Schedule[i][2];
    
    if(Weekdays[i][week_index] and (T_NOW_MILLIS > customStart) and (T_NOW_MILLIS < customEnd))
    {
      analogWrite(6, customLevel*51);
      //Serial.println(customLevel);
      ACTIVE_CUSTOM = i;
    }
    else if(Weekdays[i][week_index] and (T_NOW_MILLIS - customEnd < 3000))
    {
      analogWrite(6, 0);
      Serial.println("Custom Finished");
    }
  }
}

void checkCharging()
{
  int test_value = analogRead(A5);
  if(test_value > 800)
  {
    digitalWrite(6, LOW);
    CHARGING = true;
  }
  else
  {
    CHARGING = false;
  }
}
