#include <SoftwareSerial.h>

const int soilsen = A0;
float soilanalog,soilval;

SoftwareSerial arduino(19,18); //RX,TX

void setup() 
{ 
  Serial.begin(115200);
  arduino.begin(9600);
  pinMode(soilsen,INPUT);
  
}

void soilmoisture()
{
  soilanalog = analogRead(soilsen);
  soilval = map(soilanalog,526,205,0.5,10);
  delay(5000);
}

void loop() 
{
  soilmoisture();
  soilval = soilval + 20;
  arduino.print(soilval);
  Serial.println(soilval);
  
  delay(1000);
} 
