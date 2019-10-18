#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;
#define OUTPUT_READABLE_ACCELGYRO

long t_now = 0;
int i = 0;
void setup() 
{
  Serial.begin(9600);
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  t_now = millis();
}

void loop()
{
 int16_t accel = accelgyro.getAccelerationX();
 Serial.println(accel);
 i+=1;
 /*if(i > 1000)
 {
  Serial.print("Total time is "); Serial.print(millis()-t_now); Serial.println(" milliseconds"); 
 }*/
}
