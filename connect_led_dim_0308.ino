#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//-----------------------------------------------------------------
int pin         =  6; 
int numPixels   = 8;
bool SW = false;

Adafruit_NeoPixel pixels(numPixels, pin, NEO_GRB + NEO_KHZ800);
//-----------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  
  if (!BLE.begin()) {
  Serial.println("starting BLE failed!");
  while (1);
  }

  BLE.setLocalName("cube_LED");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(switchCharacteristic);

  BLE.addService(ledService);

  switchCharacteristic.writeValue(0);

  BLE.advertise();
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
}

//-----------------------------------------------------------------
void loop() {
  pixels.clear();
  BLEDevice central = BLE.central();

   if (SW == true){
    dim();
  }else{
    pixels.clear();
  }

  if (central.connected()) {
    if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {
          SW = true;
          //dim();
        }else{
          SW = false;
        }
    }
  }


}

//-----------------------------------------
void dim(){
    for(int j=0; j<255; j+=1){
    for(int i=0; i<numPixels; i++) {
      pixels.setPixelColor(i, j, j, j);
      pixels.show();
      delay(1);
    }
  }
  for(int j=255; j>=0; j-=1){
    for(int i=0; i<numPixels; i++) {
            pixels.setPixelColor(i, j, j, j);
            pixels.show();
            delay(1);
          }
        }
  delay(350);
}
