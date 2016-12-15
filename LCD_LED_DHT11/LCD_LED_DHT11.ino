// Libraries einbinden
#include <dht11.h>
#include <Wire.h>
#include "rgb_lcd.h"
 
dht11 DHT11;
#define DHT11PIN 7
#define LEDPIN 8

rgb_lcd lcd;

void setup()
{
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  lcd.begin(16, 2);
    
  lcd.setRGB(255, 255, 0);
  lcd.write("Loading...");
}
 
void loop()
{
  int chk = DHT11.read(DHT11PIN);
 
  Serial.println("Temp: ");
  Serial.println((float)DHT11.temperature, 2);

  lcd.clear();
  lcd.write(" Temp");
  lcd.print("     ");
  lcd.print("Humi");

  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print((float)DHT11.temperature, 2);
  lcd.print("   ");
  lcd.print((float)DHT11.humidity, 2);

  if ((float)DHT11.temperature >= 25) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }   
  
  delay(2000);
}
