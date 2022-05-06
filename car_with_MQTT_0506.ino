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

//const char broker[] = "public.cloud.shiftr.io";
const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "cy2260project";

//-------------------------------------------
#include <Servo.h> 

Servo myservo; 

byte trig = 8;
byte echo = 7;
byte maxDist = 150;                               //Maximum sensing distance (Objects further than this distance are ignored)
//byte stopDist = 30;                               //Minimum distance from an object to stop in cm
float timeOut = 2*(maxDist+10)/100/340*1000000;   //Maximum time to wait for a return signal

const int switchPin = 2;
int lastSwitchState = 0;

const byte ENA=3;
const byte IN1=4;
const byte IN2=5;

const byte IN3=9;
const byte IN4=10;
const byte ENB=11;

int MAspeed = 55;
int MBspeed = 55;

int distance = 100;
int distanceR = 0;
int distanceL = 0;
int lastDistance = 0;

int autoStatus = 0;

//motor control-------------------------------------------------------------------------
void MAclock(){
  analogWrite(ENA, MAspeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void MAcounter(){
  analogWrite(ENA, MAspeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void MBclock(){
  analogWrite(ENB, MAspeed);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void MBcounter(){
  analogWrite(ENB, MAspeed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void backward(){
  MAcounter();
  MBclock();
}

void forward(){
  MAclock();
  MBcounter();
}

void turnR(){
  MAclock();
  MBclock();
}

void turnL(){
  MAcounter();
  MBcounter();
}

int getDistance()                                   //Measure the distance to an object
{
  unsigned long pulseTime;                          //Create a variable to store the pulse travel time
  int distance;                                     //Create a variable to store the calculated distance
  digitalWrite(trig, HIGH);                         //Generate a 10 microsecond pulse
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH, timeOut);         //Measure the time for the pulse to return
  distance = (float)pulseTime * 340 / 2 / 10000;    //Calculate the object distance based on the pulse time
  return distance;
}

void lookRight()
{
    myservo.write(30); 
    delay(500);
    int distanceR = getDistance();
    delay(100);
    myservo.write(90); 
}

void lookLeft()
{
    myservo.write(150); 
    delay(500);
    int distanceL = getDistance();
    delay(100);
    myservo.write(90); 
}


void setup() {
  myservo.attach(12);                           //Assign the servo pin
  pinMode(trig,OUTPUT);                           //Assign ultrasonic sensor pin modes
  pinMode(echo,INPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(switchPin, INPUT);

//  while (!Serial) {
//  }
  
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

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}
//-------------------------------------------------------
void loop() {
  myservo.write(90);                            
  delay(750);              
  distance = getDistance();
  if (abs(distance - lastDistance)>500){
    Serial.println(distance);
  }
  delay(100);
  lastDistance = distance;
      
  int switchState = digitalRead(switchPin);
  if (switchState != lastSwitchState){
    Serial.print("swith status: ");
    Serial.println(switchState);
  }
  delay(100);
  lastSwitchState = switchState;

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
  
      if (message == 1) {
        autoStatus = 0;
        forward();
      }else if (message == 2) {
        autoStatus = 0;
        backward();
      }else if (message == 3) {
        autoStatus = 0;
        turnL();
      }else if (message == 4) {
        autoStatus = 0;
        turnR();
      }else if (message == 5) {
        autoStatus = 1;
      }else if (message == 6) {
        autoStatus = 0;
        stop();
      }
    }
  Serial.println();
  }  

  if(autoStatus == 1 && switchState == 1){
    if(distance<=15){
      stop();
      delay(200);
      backward();
      delay(800);
      stop();
      delay(200);
      
      lookRight();
      delay(200);
      lookLeft();
      delay(200);
  
        if(distanceR>=distanceL){
          turnR();
          delay(800);
          stop();
          delay(200);
        }else{
          turnL();
          delay(800);
          stop();
          delay(200);
        } 
    }else{
      forward();
    }
  }

  if(switchState == 0){
   stop();
  }
}

//-------------------------------------------------------
//void autoMode(){
//  if(distance<=15){
//      stop();
//      delay(200);
//      
//      backward();
//      delay(800);
//      
//      stop();
//      delay(200);
//      
//      distanceR = lookRight();
//      delay(200);
//      distanceL = lookLeft();
//      delay(200);
//    
//      if(distanceR>=distanceL){
//        turnR();
//        delay(800);
//        stop();
//      }else{
//        turnL();
//        delay(800);
//        stop();
//      }
//   }else if (distance>15){
//      forward();
//   }else{
//      stop();
//   }
//     distance = getDistance();
//}
