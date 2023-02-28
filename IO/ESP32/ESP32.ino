#define BLYNK_TEMPLATE_ID "TMPLTka5b8wN"
#define BLYNK_DEVICE_NAME "Greenhouse Strawberry"
#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial

#include "SoftwareSerial.h"
#define APP_DEBUG
#include "BlynkEdgent.h"

SoftwareSerial Esp(35,34);
int Soil,Temp,Hum,Lamp,Valve,Fan;

void setup()
{
  Serial.begin(115200);
  Esp.begin(9600);
  pinMode(35,INPUT);
  pinMode(34,OUTPUT);
  delay(100);
  BlynkEdgent.begin();
}

void Greenhouse()
{
  if(Esp.available() > 0)
    {
      int serial = Esp.parseInt();
      if (serial >=50 && serial < 100)
        {
          Soil = serial - 50;
          Serial.print("moisture"); Serial.println(Soil);
          Blynk.virtualWrite(V0,Soil);
        }
      else if(serial >=100 && serial < 200)
        {
          Temp = serial - 100;
          Serial.print("Temp"); Serial.println(Temp);
          Blynk.virtualWrite(V1,Temp);
        }
      else if(serial >=200 && serial < 300)
        {
          Hum = serial - 200;
          Serial.print("Hum"); Serial.println(Hum);
          Blynk.virtualWrite(V2,Hum);
        }
      else if(serial >=300 && serial < 350)
        {
          Lamp = serial - 300;
          Serial.print("Lamp"); Serial.println(Lamp);
          Blynk.virtualWrite(V3,Lamp);
        }
      else if(serial >=350 && serial < 400)
        {
          Valve = serial - 350;
          Serial.print("Valve"); Serial.println(Valve);
          Blynk.virtualWrite(V4,Valve);
        }
      else if(serial >=400 && serial < 450)
        {
          Fan = serial - 400;
          Serial.print("Fan"); Serial.println(Fan);
          Blynk.virtualWrite(V5,Fan);
        }
    }
  else
    {
    }
} 


void loop() 
{
  BlynkEdgent.run();
  Greenhouse();
}
