const int Wlvltop = 52;
const int Wlvlbott = 53;
float statetop, statebott;
void setup() 
{ 
Serial.begin(9600); 
  pinMode(Wlvltop, INPUT_PULLUP); //Arduino Internal Resistor 10K
  pinMode(Wlvlbott, INPUT_PULLUP); //Arduino Internal Resistor 10K
} 
void loop() 
{
  statetop = digitalRead(Wlvltop);
  statebott = digitalRead(Wlvlbott);
  
  if (statebott == LOW) 
  { 
    Serial.println( "Valve Aktif");
  } 
  else if(statetop == HIGH )
  { 
    Serial.println( "Valve Mati" ); 
  }
 delay(5000); 
} 
