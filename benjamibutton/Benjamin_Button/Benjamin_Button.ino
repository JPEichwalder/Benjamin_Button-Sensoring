#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

const int ledPin = 13;    // Pin, an dem die LED angeschlossen ist
const int buttonPin = 2;  // Pin, an dem der Button angeschlossen ist
bool resetButtonPressed = false;
bool resetAngles = false;
float refAngleX = 0.0;
float refAngleY = 0.0;
float refAngleZ = 0.0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  pinMode(ledPin, OUTPUT);               // Setze den Pin als Ausgang
  pinMode(buttonPin, INPUT_PULLUP);     // Setze den Pin des Buttons als Eingang mit Pull-up Widerstand
}

void resetAnglesToZero()
{
  resetAngles = true;
}

void loop()
{
  
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accelerationX = ax / 16384.0;  // Skalierungsfaktor für X-Achse
  float accelerationY = ay / 16384.0;  // Skalierungsfaktor für Y-Achse
  float accelerationZ = az / 16384.0;  // Skalierungsfaktor für Z-Achse

  if (digitalRead(buttonPin) == HIGH)
  {
    refAngleX = atan2(accelerationY, accelerationZ) * (180.0 / M_PI);  // Aktuelle Winkel als neue Referenz speichern
    refAngleY = atan2(accelerationX, accelerationZ) * (180.0 / M_PI);
    refAngleZ = atan2(accelerationX, accelerationY) * (180.0 / M_PI);
    resetAnglesToZero();
    resetButtonPressed = true;
    delay(2000);  // Entprellung des Buttons
  }

  float angleX = (atan2(accelerationY, accelerationZ) * (180.0 / M_PI)) - refAngleX;  // Winkelberechnung für X-Achse relativ zur Referenz
  float angleY = (atan2(accelerationX, accelerationZ) * (180.0 / M_PI)) - refAngleY;  // Winkelberechnung für Y-Achse relativ zur Referenz
  float angleZ = (atan2(accelerationX, accelerationY) * (180.0 / M_PI)) - refAngleZ;  // Winkelberechnung für Z-Achse relativ zur Referenz

 

  Serial.print("Winkel X = ");
  Serial.print(angleX);
  Serial.print(" Grad, Winkel Y = ");
  Serial.print(angleY);
  Serial.print(" Grad, Winkel Z = ");
  Serial.print(angleZ);
  Serial.println(" Grad");

  delay(100);

  if (angleZ <= -60.0 || angleX <= -50.0)
  {
    digitalWrite(ledPin, HIGH);  // Schalte die LED ein
  }
  else
  {
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
