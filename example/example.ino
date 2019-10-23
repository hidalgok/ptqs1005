#include "PTQS1005.h"

PTQS1005 ptqs(Serial1);
PTQS1005::DATA data;

void setup()
{
  Serial.begin(9600);   
  Serial1.begin(9600);  
}

void loop()
{
  ptqs.requestRead();
  while(!(ptqs.read(data))){}
    Serial.print("PM2.5(PPM): ");
    Serial.println(data.PM_AE_UG_2_5);
    Serial.print("TVOC(PPM): ");
    Serial.println(data.TVOC_PPM);
    Serial.print("HCHO(Mg/m3): ");
    Serial.println(data.HCHO_MGM3);
    Serial.print("CO2(ppm): ");
    Serial.println(data.CO2_PPM);
    Serial.print("Temp: ");
    Serial.println(data.TEMP);
    Serial.print("Hum: ");
    Serial.println(data.HUMIDITY);
  Serial.println("------------------");
  Serial.println();
  delay(2000);
}
