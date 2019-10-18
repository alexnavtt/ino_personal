// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include <Servo.h>

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

Servo myservo;  // create servo object to control a servo

int Max7219_pinCLK = 10; int Max7219_pinCS = 11; int Max7219_pinDIN = 8;

int16_t ax, ay, az;
int16_t gx, gy, gz;
bool firstTime = true;
long T_then = 0;
long sum_aX = 0;
long sum_aY = 0;
long sum_aZ = 0;
long sumX = 0;
long sumY = 0;
long sumZ = 0;
int offset_aX;
int offset_aY;
int offset_aZ;
int offsetX;
int offsetY;
int offsetZ;
long vX = 0;
long X = 0;
long Y = 0;
long Z = 0;
long PHIx = 0;
long PHIy = 0;
long PHIz = 0;
int pos = 0;
int i = 0;
long T_update = millis();

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


#define LED_PIN 13
bool blinkState = false;

void Write_Max7219_byte(unsigned char DATA)
{
  unsigned char i;
  digitalWrite(Max7219_pinCS,LOW);
  for(i=8;i>=1;i--)
  {
    digitalWrite(Max7219_pinCLK,LOW);
    digitalWrite(Max7219_pinDIN,DATA&0x80);// Extracting a bit data
    DATA = DATA<<1;
    digitalWrite(Max7219_pinCLK,HIGH);                                            
  }
}

void Write_Max7219(unsigned char address,unsigned char dat)
{
  digitalWrite(Max7219_pinCS,LOW);
  Write_Max7219_byte(address);
  //address，code of LED
  Write_Max7219_byte(dat);
  //data，figure on LED
  digitalWrite(Max7219_pinCS,HIGH); 
}

void Init_MAX7219(void) 
{
  Write_Max7219(0x09, 0x00);
  //decoding ：BCD
  Write_Max7219(0x0a, 0x03);
  //brightness 
  Write_Max7219(0x0b, 0x07); 
  //scanlimit；8 LEDs 
  Write_Max7219(0x0c, 0x01);
  //power-down mode：0，normal mode：1 
  Write_Max7219(0x0f, 0x00);
  //test display：1；EOT，display：0
}

void setup() {
    pinMode(Max7219_pinCLK,OUTPUT);
    pinMode(Max7219_pinCS,OUTPUT);
    pinMode(Max7219_pinDIN,OUTPUT);
    delay(50);
    Init_MAX7219();
    unsigned char block[8] = {0x0, 0x0, 0x0, B00011000, B00011000, 0x0, 0x0, 0x0};
    for(int j=1;j<9;j++) 
    {
      Write_Max7219(j,block[j-1]); 
    } 
    
    myservo.attach(9);
    pinMode(12, INPUT);
    
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    while (i < 300)
    {
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      sum_aX += ax;
      sum_aY += ay;
      sum_aZ += az;
      sumX += gx;
      sumY += gy;
      sumZ += gz;
      i++;
    }
    offset_aX = sum_aX/300;
    offset_aY = sum_aY/300;
    offset_aZ = sum_aZ/300;
    offsetX = sumX/300;
    offsetY = sumY/300;
    offsetZ = sumZ/300;
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    long T_now = millis();
    int moveX;
    int moveY;
    if (T_now - T_update > 20)
    {
      myservo.write(PHIz/1000+90);
      moveY = min(-PHIx/15000,4);
      moveY = max(moveY, -4);
      moveX = min(-PHIy/15000,3);
      moveX = max(moveX, -3);
      
      T_update = T_now;
      
      unsigned char block[8] = {0x0, 0x0, 0x0, B00011000, B00011000, 0x0, 0x0, 0x0};
      for(int j=1;j<9;j++) 
      {
        if (moveX > 0)
        {
          for (int k = 0; k<moveX; k++)
          { 
            block[j-1] += block[j-1];
          }
        }else if (moveX < 0)
        {
          for (int k = 0; k<-moveX; k++)
          {
            block[j-1] -= 0.5*block[j-1];
          }
        }

        Write_Max7219(min(j-moveY,8),block[j-1]); 
      } 
      
      if(digitalRead(12) == 1)
      { 
        //X = 0;
        //Y = 0;
        //Z = 0;
        PHIx = 0;
        PHIy = 0;
        PHIz = 0;
      }
    }
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    if (firstTime)
    {
      T_then = millis();
      firstTime = false;
    }

    //Steady state error offset
    //ax -= offset_aX;
    //ay -= offset_aY;
    //az -= offset_aZ;
    gx -= offsetX; //660;
    gy -= offsetY; //10;
    gz -= offsetZ; //75;

    //Serial.print(ax);
    
    long deltaT = T_now - T_then;
    
    if (abs(gx) > 50)
    {
      PHIx += deltaT*(gx)/33.0;
    }
    if (abs(gy) > 50)
    {
      PHIy += deltaT*(gy)/33.0;
    }
    if (abs(gz) > 50)
    {
      PHIz += deltaT*(gz+75)/33.0;
    }
    //Serial.print("\t X:"); Serial.println(X);
    

    /*
    Serial.print("x: "); Serial.print(ax-550); Serial.print("\t");
    Serial.print("y: "); Serial.print(ay+420); Serial.print("\t");
    Serial.print("z: "); Serial.print(az-15700); Serial.print("\t");
    */
    
    /*
    Serial.print("xg: "); Serial.print(gx); Serial.print("\t");
    Serial.print("yg: "); Serial.print(gy); Serial.print("\t");
    Serial.print("zg: "); Serial.print(gz);
    */
    
    Serial.print("PHIx: "); Serial.print(PHIx/1000.0);
    Serial.print("\tPHIy: "); Serial.print(PHIy/1000.0); 
    Serial.print("\tPHIz: "); Serial.print(PHIz/1000.0);
    Serial.print("\tOffX: "); Serial.print(moveX);
    Serial.print("  \tOffY: "); Serial.println(moveY);
        
    T_then = T_now;
}
