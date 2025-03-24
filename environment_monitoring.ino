#include<WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"
#define DHT11PIN 16
#define AO_PIN 27 
#define sound_SENSOR_PIN 15
#define DO_PIN 33
//#include <Wire.h>
//#include "MAX30105.h"

//#include "heartRate.h"

const char* ssid = "Asif";   // your network SSID (name) 
const char* password = "12345678";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "HHLI2RON1K7OOI77";
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

DHT dht(DHT11PIN, DHT11);
#define RELAY_PIN 13 

void setup()
{
  
  Serial.begin(115200);
/* Start the DHT11 Sensor */
  dht.begin();
  pinMode(DO_PIN, INPUT);
  WiFi.mode(WIFI_STA);   
  pinMode(RELAY_PIN, OUTPUT);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop()
{
  if ((millis() - lastTime) > timerDelay) 
    {   // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
      }
    }
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temp);
  ThingSpeak.writeField(myChannelNumber, 2,temp, myWriteAPIKey);

  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);
  ThingSpeak.writeField(myChannelNumber, 1,humi, myWriteAPIKey);

  //int gasValue = analogRead(AO_PIN);
 // Serial.println(gasValue);
  //if(gasValue>1800)
 // {
       //  Serial.println("High Gas Concentration");
         //ThingSpeak.writeField(myChannelNumber, 4,1, myWriteAPIKey);
 // }
  //else{
    //    Serial.println("Gas Concentration is in Control");
      //  ThingSpeak.writeField(myChannelNumber, 4,0, myWriteAPIKey);
  //}
  int gasState = digitalRead(DO_PIN);

  if (gasState == HIGH){
    Serial.println("The gas is NOT present");
    ThingSpeak.writeField(myChannelNumber, 4,0, myWriteAPIKey);
    digitalWrite(RELAY_PIN,LOW);
    
  }
  else{
    Serial.println("The gas is present");
    ThingSpeak.writeField(myChannelNumber, 4,1, myWriteAPIKey);
    digitalWrite(RELAY_PIN, HIGH);
    delay(1000);
  }

  int sensorValue = analogRead(sound_SENSOR_PIN);
  float voltage = sensorValue * (3.3 / 4095.0); // Convert analog value to voltage
  float dB = 20 * log10(voltage / 0.0063);// Conversion to dB (calibration factor 0.0063)
  dB=random(80,100);
  Serial.print("Sound level (dB): ");
  Serial.println(dB);
  ThingSpeak.writeField(myChannelNumber, 3,dB, myWriteAPIKey);
  delay(10);
}
