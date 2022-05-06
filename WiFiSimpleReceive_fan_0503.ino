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

//const int ledPin = 3;
//int intensity = 0;

const int transistorPin1 = 4; 
const int transistorPin2 = 5; 

void setup() {
  Serial.begin(9600);
  
  pinMode(transistorPin1, OUTPUT);
  pinMode(transistorPin2, OUTPUT);
  
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

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

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {
  int messageSize = mqttClient.parseMessage();

  if (messageSize) {
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    while (mqttClient.available()) {
      int message = mqttClient.parseInt();
      Serial.print("message is: ");
      Serial.println(message);
      if (message == 7 || message == 1) {
        digitalWrite(transistorPin1, HIGH);
        digitalWrite(transistorPin2, LOW);
      } else if (message == 8 || message ==  2) {
        digitalWrite(transistorPin1, LOW);
        digitalWrite(transistorPin2, HIGH);
      }else if (message == 9 || message == 3) {
        digitalWrite(transistorPin1, HIGH);
        digitalWrite(transistorPin2, HIGH);
      }else if (message == 10 || message == 4) {
        digitalWrite(transistorPin1, LOW);
        digitalWrite(transistorPin2, LOW);
      }
//      if (message == 1) {
//        digitalWrite(transistorPin1, HIGH);
//        digitalWrite(transistorPin2, LOW);
//      } else if (message == 2) {
//        digitalWrite(transistorPin1, LOW);
//        digitalWrite(transistorPin2, HIGH);
//      }else if (message == 3) {
//        digitalWrite(transistorPin1, HIGH);
//        digitalWrite(transistorPin2, HIGH);
//      }else if (message == 4) {
//        digitalWrite(transistorPin1, LOW);
//        digitalWrite(transistorPin2, LOW);
//      }
    }
    Serial.println();
  }
}
