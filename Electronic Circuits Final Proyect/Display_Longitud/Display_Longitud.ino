#include <LiquidCrystal.h> // include the LCD library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int potPin = A0; 
int Longitud = 0; 
void setup() {
lcd.begin(16, 2); // lcd rows and columns
lcd.print("Longitud"); 
}
void loop() {
Longitud = analogRead(potPin);     
Longitud = map(Longitud, 0, 1023, 635, 762);

// set cursor to second row, first column
lcd.setCursor(0, 1);
lcd.print(Longitud);
delay(100);
//wipe the extra characters
lcd.print(" ");
delay(1);

  
}
