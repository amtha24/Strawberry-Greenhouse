#include "RTClib.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

int Hours;

const int LDRsen = A5;
const int relay2 = A2;


float L;

void setup()
 {
  Serial.begin (9600);
  
  pinMode (LDRsen, INPUT);
  pinMode(relay2, OUTPUT);
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) 
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

 }

void LDR()
 {
  int LDRanalog = analogRead(LDRsen);
  L = map(LDRanalog, 163, 84, 600, 1500);
  Serial.println(L);
  delay(5000);
  
 }

void lamp()
 {
  DateTime now = rtc.now();
  Hours = now.hour();
  LDR();
  if( L >= 1450 && Hours >= 18 )
  {
    digitalWrite(relay2, LOW);
  }
  else
  {
    digitalWrite(relay2, HIGH);
  }

 }
void LCD()
{ lamp();
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(Hours);
  lcd.setCursor(3,0);
  lcd.print("Hour");
  lcd.setCursor(3,1);
  lcd.print("Amtha Setio P");
  lcd.setCursor(5,2);
  lcd.print("4D D4TE 2");
  lcd.setCursor(6,3);
  lcd.print("TEST LCD"); 
}


void loop()
 {
  lamp();
  LCD();

 }
