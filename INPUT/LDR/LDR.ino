const int LDRsen = A5;
 
void setup()
 {
  pinMode (LDRsen, INPUT);
  Serial.begin (115200);
 }
 
void loop()
 {
  
  int LDRanalog = analogRead(LDRsen);
  int y = map(LDRanalog, 163, 94, 600, 1500);
  Serial.print("Sensor value:");
  Serial.println(y);
  Serial.print("Analog value:");
  Serial.println(LDRanalog);
  delay(5000);
 }
