int PWM_PIN = 2;
int pwmval;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN,OUTPUT);
  Serial.println("Send a value between 1 and 255");
}

void loop() {
  if (Serial.available() >0) {
      pwmval =  Serial.parseInt();
      Serial.print("Set Speed to: ");
      Serial.println(pwmval);
      analogWrite(PWM_PIN, pwmval);
      Serial.println("done!");
      delay(2000);
  }
}
