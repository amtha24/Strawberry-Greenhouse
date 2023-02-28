const int Wlvltop = 52;
const int Wlvlbott = 53;  
const int relay4 = A4;

float statetop, statebott;

void setup() 
{ 
  Serial.begin(9600); 
  pinMode(Wlvltop, INPUT_PULLUP); //Arduino Internal Resistor 10K
  pinMode(Wlvlbott, INPUT_PULLUP);
  pinMode(relay4, OUTPUT);
  
} 
void loop() 
{ 
  statetop = digitalRead(Wlvltop);
  statebott = digitalRead(Wlvlbott); 
  if (statetop == LOW && statebott == LOW) 
  { 
    Serial.println( "Valve Aktif");
    digitalWrite(relay4, HIGH); 
  } 
  else if(statetop == HIGH )
  { 
    Serial.println( "Valve Mati" );
    digitalWrite(relay4, LOW);
  }
  else
  {
    Serial.println( "Valve Mati" );
    digitalWrite(relay4, LOW); 
  } 
delay(1000); 
}
