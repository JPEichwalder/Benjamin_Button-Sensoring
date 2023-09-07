#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>
#include <SD.h>

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

const int ledPin = 13;  // Pin, an dem die LED angeschlossen ist

File dataFile;  // Dateiobjekt für die Daten

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  pinMode(ledPin, OUTPUT);  // Setze den Pin als Ausgang
  
  // Initialisierung der SD-Karte
  if (!SD.begin(4)) {
    Serial.println("Fehler beim Initialisieren der SD-Karte!");
    while (1);
  }
  
  // Öffnen der Daten-CSV-Datei zum Schreiben
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    // Header in die CSV-Datei schreiben
    dataFile.println("Winkel X (Grad), Winkel Y (Grad), Winkel Z (Grad)");
    dataFile.close();
  } else {
    Serial.println("Fehler beim Öffnen der Daten-CSV-Datei!");
  }
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

  // Daten in CSV-Datei speichern
  saveDataToCsv(angleX, angleY, angleZ);

  delay(100);
  
  if (angleZ >= 30.0) {
    digitalWrite(ledPin, HIGH);  // Schalte die LED ein
  } else {
    digitalWrite(ledPin, LOW);   // Schalte die LED aus
  }
}

void saveDataToCsv(float angleX, float angleY, float angleZ) {
  // Öffnen der Daten-CSV-Datei zum Anhängen
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    // Daten in die CSV-Datei schreiben
    dataFile.print(angleX);
    dataFile.print(", ");
    dataFile.print(angleY);
    dataFile.print(", ");
    dataFile.println(angleZ);
    dataFile.close();
  } else {
    Serial.println("Fehler beim Öffnen der Daten-CSV-Datei!");
  }
}