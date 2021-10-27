
//Libraries
#include "Arduino.h"
#include "Wire.h"
#include "SPI.h"
#include "DHT.h"
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_Sensor.h"
#include <ArduinoJson.h>

//Network parameters
char ssid[] = "smart_production_WIFI"; //PolyBotNet
const char* password =  "aau smart production lab"; //SwarmBot
const char* mqtt_server = "172.20.66.5"; //192.168.1.191

//Constants
#define DHTPIN D5   // what pin we're connected to
#define SDPIN A0 // Pin for sound detector read analog
//#define SDPIN2 39 // Pin for sound detector read digital
#define DHTTYPE DHT11   // DHT 11  (AM2302)


//Classes
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
WiFiClient espClient; ////MQTT Initialization
PubSubClient client(espClient); ////MQTT Initialization

//Variables
float hum = 0;  //Stores humidity value
float temp = 0; //Stores temperature value
int asound = 0; //Analog value for the sound sensor
int dsound = 0; //Digital value for the sound sensor
int threshold = 800; //Microphone threshold/bias
int unbiased = 0;
String espName = "ESP02"; //Select the name of the ESP currently being programmed (even numbers ONLY 02, 04, 06, 08)



//#################################### SETUP SECTION ######################################
void connectNetwork(){
  delay(10);
  pinMode(13, OUTPUT); 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
  }

  Serial.println(WiFi.localIP());
  Serial.println("Connected to the WiFi network");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(){
  dht.begin();
  //pinMode(SDPIN2, INPUT);
  connectNetwork();
  client.setServer(mqtt_server, 1883);
}

//################################# SENSOR FUNCTIONS #######################################

void humiditySensor(){
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    Serial.print("This is the humdity: ");
    Serial.println(hum);
    Serial.print("This is the temperature: ");
    Serial.println(temp);
    String namePub = espName + "/temperature";
    client.publish(namePub.c_str(),String(temp).c_str());
    String namePub2 = espName + "/humidity";
    client.publish(namePub2.c_str(),String(hum).c_str());
} 


void soundSensor(){
    asound = analogRead(SDPIN);
    //dsound = digitalRead(SDPIN2);
    unbiased = abs(asound - threshold);
    String namePub = espName + "/sound";
    Serial.print("This is the sound level: ");
    Serial.println(unbiased);
    client.publish(namePub.c_str(), String(unbiased).c_str());
}


void loop()
{
 
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    humiditySensor();
    delay(10);
    soundSensor();
    delay(200);
}