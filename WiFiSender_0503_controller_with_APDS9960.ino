#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "cy2260project";

const long interval = 1000;
unsigned long previousMillis = 0;

const int buttonPin = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;
const int buttonPin5 = 6;
const int buttonPin6 = 7;
const int buttonPin7 = 8;
const int buttonPin8 = 9;

const int debounceDelay = 5;

int lastButtonState = 0;
int lastButtonState2 = 0;
int lastButtonState3 = 0;
int lastButtonState4 = 0;
int lastButtonState5 = 0;
int lastButtonState6 = 0;
int lastButtonState7 = 0;
int lastButtonState8 = 0;


int count = 0;
int count2 = 0;

int mg1 = 1;
int mg2 = 2; 
int mg3 = 3; 
int mg4 = 4; 
int mg5 = 5; 
int mg6 = 6; 
int mg7 = 7; 
int mg8 = 8;
int mg9 = 9;
int mg10 = 10; 
//-------------------------------------
#include <Arduino_APDS9960.h>



void setup() {
  Serial.begin(9600);

  APDS.begin();
  APDS.setGestureSensitivity(80);
  
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(buttonPin8, INPUT_PULLUP);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();


  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  mqttClient.poll();
  
  int buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    delay(debounceDelay);
    
    if (buttonState == LOW) {
      mqttClient.beginMessage(topic);
      mqttClient.print(mg1);
      mqttClient.endMessage();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      Serial.print("Button1 Clicked ");
      Serial.println(mg1);
    } 
    lastButtonState = buttonState;
  }
//------------------------------------------------------
  int buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 != lastButtonState2) {
    delay(debounceDelay);
    
    if (buttonState2 == LOW) {
      mqttClient.beginMessage(topic);
      mqttClient.print(mg2);
      mqttClient.endMessage();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      Serial.print("Button2 Clicked ");
      Serial.println(mg2);
    }
    lastButtonState2 = buttonState2;
  }
//------------------------------------------------------
  int buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 != lastButtonState3) {
    delay(debounceDelay);
    
    if (buttonState3 == LOW) {
      mqttClient.beginMessage(topic);
      mqttClient.print(mg3);
      mqttClient.endMessage();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      Serial.print("Button3 Clicked ");
      Serial.println(mg3);
    }
    lastButtonState3 = buttonState3;
  }
//------------------------------------------------------
  int buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 != lastButtonState4) {
    delay(debounceDelay);
    
    if (buttonState4 == LOW) {
      mqttClient.beginMessage(topic);
      mqttClient.print(mg4);
      mqttClient.endMessage();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      Serial.print("Button5 Clicked ");
      Serial.println(mg4);
    }
    lastButtonState4 = buttonState4;
  }
//------------------------------------------------------
  int buttonState5 = digitalRead(buttonPin5);
  if (buttonState5 != lastButtonState5) {
    delay(debounceDelay);
    
    if (buttonState5 == LOW) {
      mqttClient.beginMessage(topic);
      mqttClient.print(mg5);
      mqttClient.endMessage();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      Serial.print("Button5 Clicked ");
      Serial.println(mg5);
    }
    lastButtonState5 = buttonState5;
  }
//------------------------------------------------------
  int buttonState6 = digitalRead(buttonPin6);
  if (buttonState6 != lastButtonState6) {
    delay(debounceDelay);
    
    if (buttonState6 == LOW) {
      mqttClient.beginMessage(topic);
      mqttClient.print(mg6);
      mqttClient.endMessage();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      Serial.print("Button6 Clicked ");
      Serial.println(mg6);
    }
    lastButtonState6 = buttonState6;
  }

  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_LEFT:   // 0
        mqttClient.beginMessage(topic);
        mqttClient.print(mg7);
        mqttClient.endMessage();
        Serial.print("Sending message to topic: ");
        Serial.println(topic);
        Serial.print("Left ");
        Serial.println(mg7);
        break;
      case GESTURE_RIGHT: // 1
        mqttClient.beginMessage(topic);
        mqttClient.print(mg8);
        mqttClient.endMessage();
        Serial.print("Sending message to topic: ");
        Serial.println(topic);
        Serial.print("Right ");
        Serial.println(mg8);
        break;
      case GESTURE_UP: // 2
        mqttClient.beginMessage(topic);
        mqttClient.print(mg9);
        mqttClient.endMessage();
        Serial.print("Sending message to topic: ");
        Serial.println(topic);
        Serial.print("Up ");
        Serial.println(mg9);
        break;
      case GESTURE_DOWN: // 3
        mqttClient.beginMessage(topic);
        mqttClient.print(mg10);
        mqttClient.endMessage();
        Serial.print("Sending message to topic: ");
        Serial.println(topic);
        Serial.print("Down ");
        Serial.println(mg10);
        break;
    }
  }
}
