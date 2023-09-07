#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
}

void loop()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  float accelerationX = ax / 16384.0;  // Skalierungsfaktor für X-Achse
  float accelerationY = ay / 16384.0;  // Skalierungsfaktor für Y-Achse
  float accelerationZ = az / 16384.0;  // Skalierungsfaktor für Z-Achse

  float angleX = atan2(accelerationY, accelerationZ) * (180.0 / M_PI);  // Winkelberechnung für X-Achse
  float angleY = atan2(accelerationX, accelerationZ) * (180.0 / M_PI);  // Winkelberechnung für Y-Achse
  float angleZ = atan2(accelerationX, accelerationY) * (180.0 / M_PI);  // Winkelberechnung für Z-Achse

  Serial.print("Winkel X = ");
  Serial.print(angleX);
  Serial.print(" Grad, Winkel Y = ");
  Serial.print(angleY);
  Serial.print(" Grad, Winkel Z = ");
  Serial.print(angleZ);
  Serial.println(" Grad");

  delay(500);
}
