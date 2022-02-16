

//#include <Mouse.h>
#include <Keyboard.h>


// Switches are connected from ground to these defined pins
const int switchPin = 2;
const int buttonPin = 3;
const int xAxis = A0;
const int yAxis = A1;
const int ledPin = 12; 

int range = 12;
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value


bool keyboardIsActive = false;    // whether or not to control the mouse

int lastSwitchState = LOW;        // previous switch state
int lastButtonState = LOW;

void setup(){ 
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Keyboard.begin();
}



void loop()
{
//-------------------------------------------------
  int switchState = digitalRead(switchPin);

  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      keyboardIsActive = !keyboardIsActive;
      if (keyboardIsActive) {
        Keyboard.begin();
      } else{
        Keyboard.end();
      }
      digitalWrite(ledPin, switchState);
    }
  }
  lastSwitchState = switchState;
//----------------------------------
//  int switchState = digitalRead(switchPin);  
//  
//  if (keyboardIsActive) {
//    if (switchState != lastSwitchState){
//      if (digitalRead(switchPin) == HIGH){
//        //Keyboard.write(88);//X
//        Keyboard.write('c');//Z
//      } else {
//        Keyboard.release('c');
//      }
//    } 
//    else {
//    if (digitalRead(switchPin) == HIGH){
//      Keyboard.write('c');//Z
//      //delay(50);
//    }
// }
//   lastSwitchState = switchState;
//-----------------------------------------------------
  int buttonState = digitalRead(buttonPin);  
  
  if (keyboardIsActive) {
    if (buttonState != lastButtonState){
      if (digitalRead(buttonPin) == HIGH){
        Keyboard.write('Z');
      } else {
        Keyboard.release('Z');
      }
    } 
    else {
    if (digitalRead(buttonPin) == HIGH){
      Keyboard.write('Z');
    }
 }
   lastButtonState = buttonState;
//-----------------------------------------------------
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);
//-----------------------------------------------------
    if (xReading <= -threshold) {
      Keyboard.write(216); //KEY_LEFT_ARROW
      //delay(50);
    }else if (xReading >= threshold) {
      Keyboard.write(215); //KEY_RIGHT_ARROW
      //delay(50);
    }
    
    if (yReading <= -threshold) {
      Keyboard.write(218); //KEY_UP_ARROW
      //delay(50);
    }else if (yReading >= threshold) {
      Keyboard.write(217); //KEY_DOWN_ARROW
      //delay(50);
    }

//        if (xReading <= -threshold) {
//      Keyboard.write('w'); //KEY_LEFT_ARROW
//      delay(50);
//    }else if (xReading >= threshold) {
//      Keyboard.write('s'); //KEY_RIGHT_ARROW
//      delay(50);
//    }
//    
//    if (yReading <= -threshold) {
//      Keyboard.write('a'); //KEY_UP_ARROW
//      delay(50);
//    }else if (yReading >= threshold) {
//      Keyboard.write('d'); //KEY_DOWN_ARROW
//      delay(50);
//    }
//-----------------------------------------------------
  }
}


//------------------------------------
int readAxis(int thisAxis) {
  int reading = analogRead(thisAxis);
  reading = map(reading, 0, 1023, 0, range);
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }
  return distance;
}
//------------------------------------

//    if (xReading <= -threshold) {
//      Keyboard.write('KEY_LEFT_ARROW'); 
//    }else if (xReading >= threshold) {
//      Keyboard.write('KEY_RIGHT_ARROW');
//    }
//    if (yReading <= -threshold) {
//      Keyboard.write('KEY_UP_ARROW'); 
//    }else if (yReading >= threshold) {
//      Keyboard.write('KEY_DOWN_ARROW');
//    }
