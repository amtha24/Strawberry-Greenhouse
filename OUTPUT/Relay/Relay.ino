const int relay1 = A1; //Pompa Soil
const int relay2 = A2; //Plant light
const int relay3 = A3; //Fan
const int relay4 = A4; //Water Tank
void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
}

void loop() {
  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  //digitalWrite(relay1, HIGH);
  //Serial.println("1 Current Flowing");
  //delay(1000); 
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  //digitalWrite(relay1, LOW);
  //Serial.println("1 Current not Flowing");
  //delay(1000);
  
  //digitalWrite(relay2, HIGH);
  //Serial.println("2 Current Flowing");
  //delay(1000); 
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  //digitalWrite(relay2, LOW);
  //Serial.println("2 Current not Flowing");
  //delay(1000);

  digitalWrite(relay3, HIGH);
  Serial.println("3 Current Flowing");
  delay(1000); 
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  digitalWrite(relay3, LOW);
  Serial.println("3 Current not Flowing");
  delay(1000);

  //digitalWrite(relay4, HIGH);
  //Serial.println("4 Current Flowing");
  //delay(1000); 
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  //digitalWrite(relay4, LOW);
  //Serial.println("4 Current not Flowing");
  //delay(1000);
}
