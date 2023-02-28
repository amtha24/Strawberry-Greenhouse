int soilsen = A0;
float soilanalog,soilval;

void setup() 
{ 
  Serial.begin(115200);
  
}

void loop() 
{
  soil();
  delay(1000);
} 

float soil()
{ 
  soilanalog = analogRead(soilsen);
  soilval = map(soilanalog,350,228,1,9);//250<99
  Serial.print("Analog :");Serial.println(soilanalog);
  //Serial.print("Calibrated :");Serial.println(soilval);
  delay(2500);
}
