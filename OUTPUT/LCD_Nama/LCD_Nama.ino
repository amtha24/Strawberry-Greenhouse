#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
}
void LCD()
{
  lcd.init();
  
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(6,0);
  lcd.print("TEST LCD");
  lcd.setCursor(3,1);
  lcd.print("Amtha Setio P");
  lcd.setCursor(5,2);
  lcd.print("4D D4TE 2");
  lcd.setCursor(6,3);
  lcd.print("TEST LCD"); 
}

void loop()
{
  LCD();
}
