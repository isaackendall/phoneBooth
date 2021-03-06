/******************************************************************************
PIR_Motion_Detector.ino


The PIR motion sensor has a three-pin JST connector terminating it. Connect
the wire colors like this:
- Black: D2 - signal output (pulled up internally)
- White: GND
- Red: 5V


Whenever the PIR sensor detects movement, it'll write the alarm pin LOW.
Then it will define the Val value and push this to thingspeak.

******************************************************************************/

#include "ThingSpeak.h"                                 // Include ThingSpeak Library
#include <ESP8266WiFi.h>                                // Include ESP8266wifi Library

const int MOTION_PIN = 0;                               // Pin connected to motion detector
const int LED_PIN = 5;                                  // LED pin - active-high
int motionState = 0;                                    // We start, assuming no motion detected
int val = 0;                                            // Variable for reading the pin status


char ssid[] = "thecottage";                             // Enter your WiFi SSID (router name)
char pass[] = "BadRomance324";                          // Enter your WiFi router password
unsigned long myChannelNumber = 745770;                 // Enter your Thingspeak Channel Number
const char * myWriteAPIKey = "KGVF3I4PG1A37PFH";        // Enter your ThingSpeak API Key

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
unsigned long resetMillis = 0;
const long interval = 90000;                            // Time delay interval (90 seconds) to prevent false triggers when setting up device 
                                                        
int status = WL_IDLE_STATUS;
WiFiClient  client;


void setup() 
{
  Serial.begin(115200);

   WiFi.begin(ssid, pass);                               // Start WiFi connection
  ThingSpeak.begin(client);                             // Start ThingSpeak connection
  Serial.begin(115200);                                 // Serial Baud Rate
  pinMode(LED_PIN, OUTPUT);                              // Set the LED pin as an Output
  //pinMode(inputPin, INPUT);                             // Set the Motion Sensor pin as an Input

  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  Serial.println("Motion Sensor Alarm");
  Serial.println(val); 
  
  ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);           // Update ThingSpeak channel the PIR sensor value '0'
 
  
}

void loop() 
{

//********************* Read motion sensor and upload to ThingSpeak ***************************** 
  
currentMillis = millis();   

  int proximity = digitalRead(MOTION_PIN);
  if (proximity == LOW) // If the sensor's output goes low, motion is detected
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Motion detected!");
    val = 1;
    Serial.print("val value = ");
    Serial.println(val);                                                // set val to equal 1, which will be written to thingspeak channel
    ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);      // Update ThingSpeak channel the PIR sensor value '1'  
    Serial.println("Val Data Sent to ThingSpeak!");    
    delay(21000);                                                       // 21 sec delay as thingspeak will only take data every 20 secs
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Motion not detected!");
    val = 0;                                                            // set val to equal 0, which will be written to thingspeak channel
    Serial.print("val value = ");                                       // prints value of val
    Serial.println(val);                                                // prints value of val
    ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);      // Update ThingSpeak channel with the value of val'  
    Serial.println("Val Data Sent to ThingSpeak!");                     // Update ThingSpeak channel the PIR sensor value '0'  
    delay(21000);                                                       // 21 sec delay as thingspeak will only take data every 20 secs
  }
}
