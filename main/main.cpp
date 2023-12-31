#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE AM2301  // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


const char* ssid = "xxxxx";   // your network SSID (name) 
const char* password = "xxxxxxx";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = xxxxx;
const char * myWriteAPIKey = "xxxxxxxx";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float temperatureC;
float Humidity;


void setup() {
  Serial.begin(115200);  //Initialize serial
  //initBME();
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  dht.begin();

}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }


      delay(2000);
    
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f = dht.readTemperature(true);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) 
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      else
      {

        ThingSpeak.setField(1, t);
        ThingSpeak.setField(2, h);
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        if(x == 200){
          Serial.println("Channel update successful.");
        }
        else{
          Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
        lastTime = millis();
      }
    

    
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.print(F("°C "));
      Serial.print(f);
          Serial.println(F("°F"));

  
    
    
  }
}
