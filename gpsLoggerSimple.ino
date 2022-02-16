
#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TinyGPSPlus.h>

// AltSoftSerial ss;
static TinyGPSPlus  gps;
const int chipSelect = 10;
void setup() {
  Serial.begin(9600); // Open COM Serial
  delay(200);
  //ss.begin(9600); // Open COM Serial

  pinMode(LED_BUILTIN, OUTPUT); // Use LED Pin

  while (!SD.begin(chipSelect)) {
    Serial.println("SD ERROR");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  digitalWrite(LED_BUILTIN, LOW);
  File dataFile = SD.open("datalog2.txt", FILE_WRITE);
  dataFile.println("RESTART_DETECTED");
  dataFile.println("TIMESTAMP,LAT,LON,NSAT,HDOP");
}


void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial.available() > 0)
    if (gps.encode(Serial.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  File dataFile = SD.open("datalog2.txt", FILE_WRITE);
  if (gps.date.isValid())
  {
    dataFile.print(gps.date.year());
    dataFile.print(F("/"));
    dataFile.print(gps.date.month());
    dataFile.print(F("/"));
    dataFile.print(gps.date.day());
  }
  else
  {
    dataFile.print(F("INVALID"));
  }

  dataFile.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    dataFile.print(gps.time.hour());
    dataFile.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    dataFile.print(gps.time.minute());
    dataFile.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    dataFile.print(gps.time.second());
    dataFile.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    dataFile.print(gps.time.centisecond());
  }
  else
  {
    dataFile.print(F("INVALID"));
  }
  
  dataFile.print(F(","));

  if (gps.location.isValid())
  {
    dataFile.print(gps.location.lat(), 6);
    dataFile.print(F(","));
    dataFile.print(gps.location.lng(), 6);
  }
  else
  {
    dataFile.print(F("INVALID"));
  }
  dataFile.print(F(","));

  dataFile.print(gps.satellites.value());
  dataFile.print(F(","));

  dataFile.print(gps.hdop.value());

  dataFile.println();
  dataFile.close();
}
