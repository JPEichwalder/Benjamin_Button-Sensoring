#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

const int ledPin = 13;  // Pin, an dem die LED angeschlossen ist

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  pinMode(ledPin, OUTPUT);  // Setze den Pin als Ausgang
}

void loop()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  float accelerationX = ax / 16384.0;  // Skalierungsfaktor für X-Achse
  float accelerationY = ay / 16384.0;  // Skalierungsfaktor für Y-Achse
  float accelerationZ = az / 16384.0;  // Skalierungsfaktor für Z-Achse

  float angleX = (atan2(accelerationY, accelerationZ) * (180.0 / M_PI))-99.13;  // Winkelberechnung für X-Achse
  float angleY = (atan2(accelerationX, accelerationZ) * (180.0 / M_PI))-158.6385;  // Winkelberechnung für Y-Achse
  float angleZ = (atan2(accelerationX, accelerationY) * (180.0 / M_PI))-0.5420;// Winkelberechnung für Z-Achse



  Serial.print("Winkel X = ");
  Serial.print(angleX);
  Serial.print(" Grad, Winkel Y = ");
  Serial.print(angleY);
  Serial.print(" Grad, Winkel Z = ");
  Serial.print(angleZ);
  Serial.println(" Grad");

  delay(100);
  
  if (angleZ >= 30.0) {
    digitalWrite(ledPin, HIGH);  // Schalte die LED ein
  } else {
    digitalWrite(ledPin, LOW);   // Schalte die LED aus
  } 
    // Sende die Daten an den PC über die serielle Schnittstelle
  Serial.print("Acceleration X = ");
  Serial.print(accelerationX);
  Serial.print(", Acceleration Y = ");
  Serial.print(accelerationY);
  Serial.print(", Acceleration Z = ");
  Serial.println(accelerationZ);
}
