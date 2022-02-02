#include <Encoder.h>
#include <Arduino_JSON.h>
 

Encoder myEncoder(2, 3);
const int buttonPin=4;

int lastButtonState = HIGH;
int lastPosition  = 0;
int threshold = 3;

bool inputsChanged = false;
JSONVar outgoing;
 
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  outgoing["button"] = 0;
  outgoing["knob"] = 0;
}
 
void loop() {
  int buttonState = digitalRead(buttonPin);
//  Serial.print(buttonState);
//  Serial.print(",");
  if (buttonState != lastButtonState){
    delay(5);
    outgoing["button"] = buttonState;
    lastButtonState = buttonState;
    inputsChanged = true;
  }
  
  long newPosition = myEncoder.read();
  if (abs(newPosition - lastPosition)>threshold) {
    outgoing["knob"] = newPosition;
    lastPosition = newPosition;
    inputsChanged = true;
//    Serial.println(newPosition);
  }
    if (inputsChanged){
    Serial.println(outgoing);
    inputsChanged = false;
  }
}
