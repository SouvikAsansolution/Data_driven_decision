
//Libraries
#include "Arduino.h"
#include "Wire.h"
#include "SPI.h"
#include "DHT.h"
#include "WiFi.h"
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
#define CS 16 			// Assignment of the CS pin
#define DHTPIN A7     	// what pin we're connected to
#define IRPIN 33 		// Pin for IR dectector
#define SDPIN A2 		// Pin for sound detector read analog
#define SDPIN2 39 		// Pin for sound detector read digital
#define DHTTYPE DHT11   // DHT 11  (AM2302)
#define MSG_BUFFER_SIZE	(50)

//Classes
DHT dht(DHTPIN, DHTTYPE); 		//// Initialize DHT sensor for normal 16mhz Arduino
Adafruit_MPU6050 mpu; 			//// Initialize IMU sensor module mpu6050
WiFiClient espClient; 			////MQTT Initialization
PubSubClient client(espClient); ////MQTT Initialization

//Variables
byte intensity = 0; //Stores sensor value for the light sensor
float hum = 0; 	 	//Stores humidity value
float temp = 0; 	//Stores temperature value
int detect = 0;	 	//1 or 0 depending on if something is in the way
int flag = 1; 		// Flag is used to check if something moved in front of irsensor and left again
int status = 0; 	//Status for linetracker 0 = object has moved, 1 = object detected, 2 = object is still there, 3 = no object detected
int asound = 0; 	//Analog value for the sound sensor
int dsound = 0; 	//Digital value for the sound sensor
int threshold = 2950; //Microphone threshold/bias
int unbiased = 0;


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
  SPI.begin(); // initialization of SPI port
  SPI.setDataMode(SPI_MODE0); // configuration of SPI communication in mode 0
  SPI.setClockDivider(SPI_CLOCK_DIV16); // configuration of clock at 1MHz
  pinMode(CS, OUTPUT); //configure pin connected to chip select as output
  pinMode(IRPIN, INPUT);
  pinMode(SDPIN, INPUT);
  pinMode(SDPIN2, INPUT);
  connectNetwork();
  client.setServer(mqtt_server, 1883);
  if (!mpu.begin()) {
  Serial.println("Sensor init failed");
  while (1)
    yield();
  }
  Serial.println("Found a MPU-6050 sensor");
}

//################################# SENSOR FUNCTIONS #######################################
void lightSensor(){
  digitalWrite(CS, LOW); // activation of CS line
  intensity = SPI.transfer(0) << 3; // Aquisition of first 5 bits of data without leading zeros
  intensity |= (SPI.transfer(0) >> 4); //Aquisition of last 3 bits of data and appending
  digitalWrite(CS, HIGH);
  client.publish("esp32/lightsensor", String(intensity).c_str());
}

void imu(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  String jsonString;
  DynamicJsonDocument doc(1024);

  doc["sensor"] = "IMU";
  doc["location"]   = "STPLC_01";

  doc["acceleration"][0] = a.acceleration.x;
  doc["acceleration"][1] = a.acceleration.y;
  doc["acceleration"][2] = a.acceleration.z;

  doc["gyroscope"][0] = g.gyro.x;
  doc["gyroscope"][1] = g.gyro.y;
  doc["gyroscope"][2] = g.gyro.z;

  serializeJson(doc, jsonString);
  Serial.println(jsonString);
  client.publish("esp32/imu", jsonString.c_str());
}

void humiditySensor(){
    hum = dht.readHumidity();
    temp= dht.readTemperature(); 
    client.publish("esp32/temperature",String(temp).c_str());
    client.publish("esp32/humidity",String(hum).c_str());
} 

void lineTracker(){
    detect = digitalRead(IRPIN);
    if (detect == 0 && flag == 0){
      flag = 1;
      status = 0;
      //Serial.println("Object has moved on!");
    }
    else if (detect == 1 && flag == 1){
      flag = 0;
      status = 1;
      //Serial.println("Object has been detected!");
    }
    else if (detect == 1 && flag == 0){
      status = 2;
      //Serial.println("Object is still here!");
    }
    else if (detect == 0 && flag == 1){
      status = 3;
      //Serial.println("There is no object!");
    }

    client.publish("esp32/detection", String(status).c_str());
}

void soundSensor(){
    asound = analogRead(SDPIN);
    dsound = digitalRead(SDPIN2);
    unbiased = abs(asound - threshold);
    client.publish("esp32/sound", String(unbiased).c_str());
}

//############################# LOOP SECTION ##############################################3

void loop()
{

    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    delay(10);
    humiditySensor();
    delay(10);
    lightSensor();
    delay(10);
    lineTracker();
    delay(10);
    soundSensor();
    delay(10);
    imu();
    delay(800);
}

   