int soilsen = A0;
float soilanalog,soilval;

void setup() 
{ 
  Serial.begin(9600);
  
}

void loop() 
{
  soil();
  Serial.println(soilanalog);
  Serial.print(soilval);
  Serial.println("%");
  delay(1000);
  
} 

void soil()
{ 
  soilanalog = analogRead(soilsen);
  soilval = map(soilanalog,526,205,0.5,10);

}
