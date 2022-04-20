//https://www.instructables.com/id/Trigger-Smartphone-Camera-With-Arduino/
#include 
byte address = 0x00;
int CS= 10;
 

void setup()
{  
  
 pinMode (CS, OUTPUT);
SPI.begin();
    
}
void loop()
{    
 digitalPotWrite(0); // no resistance here. Camera is triggered.
  delay(500);
  digitalPotWrite(128); //resistance here. Camera is not triggered.
delay(2000);
      }
int digitalPotWrite(int value)
{
digitalWrite(CS, LOW);
SPI.transfer(address);
SPI.transfer(value);
digitalWrite(CS, HIGH);
}
