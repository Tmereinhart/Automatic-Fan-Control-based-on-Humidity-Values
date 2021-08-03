/***************************************************
   Based on Adafruit MQTT Library ESP8266 Example Written by Tony DiCola for Adafruit Industries.

   Can be used with Various ESP 8266 boards

   This Example uses the NodeMCU 1.0 ESP 12-E

   Revised project code by T. Reinhart, R. Elliott, and J. McCubbin
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DHT.h>                          // Library for DHT 11 Temperature/Humidity Module

int indicator = D1;

float dhttemp = 0;                       // Variable for temperature value
float dhthumid = 0;                     // Variable for humidity value

float fanstart = 30;

float humidity_current = 0;
float humidity_previous = 0;
float low_pass = 0;
float differential = 0;
float delta = 2.5;

int fanstate = 0;
int motortimer = 0;

uint8_t DHTPIN = D2;               // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11              // DHT 11

//#define DHTTYPE DHT22       // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21       // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "SSID"
#define WLAN_PASS       "yourpassword"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Your Adafruit IO Username"
#define AIO_KEY         "Your Adafruit IO KEY"

/************ Global State (you don't need to change this!) ******************************************************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ******************************************************************************/
// Setup feeds for publishing temperature and humidity values

Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vanee-hvac.temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vanee-hvac.humidity");
Adafruit_MQTT_Publish relayonoff = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vanee-hvac.relay");

// Setup feed for the relay status. This will trigger the green LED indicator light
Adafruit_MQTT_Subscribe relay = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/vanee-hvac.relay");

/*************************** Sketch Code *****************************************************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D1, OUTPUT);
  pinMode (DHTPIN, INPUT);            // Initialize the DHT 11 Module
  dht.begin();

  Serial.println(F("Bathroom HVAC Sensor"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&relay);

  
  
}

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(30000))) {
     
     
     if (subscription == &relay) {                               
      Serial.print(F("Got: "));                               
      Serial.println((char *)relay.lastread);   
      delay(100);
      if (strcmp((char *)relay.lastread, "ON") == 0) {          // If its ON, Turn on the LED
       digitalWrite(indicator, HIGH);
       // Add code here for stoping the auto humid values if manually triggered
      }
      if (strcmp((char *)relay.lastread, "OFF") == 0) {         // If its OFF, Turn OFF the LED
        digitalWrite(indicator, LOW); 
      }
    
    }

  
    
  }

          


    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    
    dhthumid = dht.readHumidity(); // Read humidity
    delay(500);
    dhttemp = dht.readTemperature();  // Read temperature as Celsius (the default)
    delay(100);

      humidity_previous = humidity_current;
      humidity_current = dhthumid;
      low_pass = ((humidity_current *.05) + (humidity_previous * .95));
      Serial.println(low_pass);

      if (humidity_previous != 0){
      differential = (humidity_current - low_pass);
      Serial.println(differential);
      
   Serial.print(F("\nSending Temperature val "));               // This publishes the 'temp' value to the server
   Serial.println(dhttemp);
  
   
   Serial.print("...");
   if (! temperature.publish(dhttemp++)) {
     Serial.println(F("Failed"));
   } else {
     Serial.println(F("OK!"));
   }

    Serial.print(F("\nSending Humidity val "));                 // This publishes the 'humid' value to the server
   Serial.print(dhthumid);

   Serial.print("...");
   if (! humidity.publish(dhthumid++)) {
     Serial.println(F("Failed"));
   } else {
    Serial.println(F("OK!"));
   }

   
   Serial.print(F("\nFan State "));               
   Serial.println(fanstate);

if (differential >= delta) {                                    // Compares actual temp value to the setpoint
  if (fanstate == 0) {
    fanstate = 1;                                                //Manual Setpoint
    motortimer = 80;
  //Serial.print(F("\nTurning ON Exhaust Fan "));               // This publishes to the relay feed to turn ON the fan
  //Serial.print("...");
   if (! relayonoff.publish("ON")) {
   //  Serial.println(F("Failed"));
   } else {
    // Serial.println(F("OK!"));
   }
  }
}

if (fanstate == 1) {
  motortimer = motortimer -1;
}

if (fanstate == 1){
if (motortimer == 0) {
    fanstate = 0;
    motortimer = 0;
  //Serial.print(F("\nTurning OFF Exhaust Fan "));               // This publishes to the relay feed to turn ON the fan
  //Serial.print("...");
   if (! relayonoff.publish("OFF")) {
     //Serial.println(F("Failed"));
   } else {
    // Serial.println(F("OK!"));
   }
  
}
}      
  // ping the server to keep the mqtt connection alive
  //if(! mqtt.ping()) {
   // mqtt.disconnect();
  //}
 
}
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
