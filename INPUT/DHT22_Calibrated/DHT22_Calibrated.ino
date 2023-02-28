#include "DHT.h"
#define DHTPIN 22     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}
void Dht22()
{
    
  delay(2000);
  int hum = dht.readHumidity();
  int temp = dht.readTemperature();
  int h = hum + 16;
  int t = temp - 6;
  
  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.println("%");
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
}
void loop() 
{
 Dht22();
}

//void hum()
//{
  
//}
