#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup()
{
 lcdSetup();
}

void loop()
{
  // Do nothing here...
}

/**********************************************************/
void lcdSetup()
{
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Please enter Code to");
  lcd.setCursor(0, 1);
  lcd.print("------Activate------");
  lcd.setCursor(8, 3);
  lcd.print("----"); 
}
