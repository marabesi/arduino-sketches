#include <Wire.h>
#include "rgb_lcd.h"

#include <dht11.h>

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266_Simple.h>

#define ESP8266_SSID  "Marabesi"
#define ESP8266_PASS  "matheusmarabesi"

#define URL_SERVER "192.168.0.101:8000"
#define IP_SERVER "192.168.0.101"
#define PORT_SERVER 8000

#define DHT11PIN 7

rgb_lcd lcd;

dht11 DHT11;

// Create the ESP8266 device on pins 
//   3 for Arduino RX (TX on ESP connects to this pin) 
//   2 for Arduino TX (RX on ESP connects to this pin)

ESP8266_Simple wifi(3,2);


void setup()
{
  lcd.begin(16, 2);
    
  lcd.setRGB(255, 255, 0);
  lcd.write("Loading...");
  
  Serial.begin(115200); 

  wifi.begin(9600);
 
  lcd.clear();
  lcd.write("Connecting to "); 
  lcd.setCursor(0, 1);
  lcd.write(ESP8266_SSID);
  lcd.setCursor(0, 0);
  
  wifi.setupAsWifiStation(ESP8266_SSID, ESP8266_PASS, &Serial);
  
  // A blank line just for debug formatting 
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  lcd.clear();
  lcd.write("Connected on ");

  lcd.setCursor(0, 1);
  lcd.write(ESP8266_SSID);

  lcd.setCursor(0, 0);
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);
 

  Serial.println("Temp: ");
  Serial.println((float)DHT11.temperature, 2);
  
  char buffer[250];
  
  memcpy(buffer, 0, sizeof(buffer)); 
  strncpy_P(buffer, PSTR(""), sizeof(buffer)-1);
  
  //strncpy_P(buffer+strlen(buffer), PSTR("?Millis="), sizeof(buffer)-strlen(buffer)-1);
  
  //ltoa(millis(), buffer+strlen(buffer), 10); // Note "10" is Base10, not a length
    
  Serial.print("Requesting ");
  Serial.print(buffer);
  Serial.print(": ");
  
  unsigned int httpResponseCode = 
    wifi.GET
    (
      F(IP_SERVER),
      PORT_SERVER,
      buffer,
      sizeof(buffer),
      F(URL_SERVER),
      1
    );
  
  // Now let's interpret the response code
  if(httpResponseCode == 200 || httpResponseCode == ESP8266_OK)
  {
    lcd.clear();
    lcd.setRGB(255, 255, 255);
    lcd.write(buffer);
    
    Serial.println("OK");
    Serial.println(buffer);

    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    lcd.setRGB(255, 0, 0);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.print("HTTP Status ");
    Serial.println(httpResponseCode);

    lcd.clear();
    lcd.write(httpResponseCode);
  }
    
  
  delay(5000);  
}
