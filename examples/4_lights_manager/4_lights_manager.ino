/**
 * A simple example to show DimmableLightManager api's.
 */
#include "dimmable_light_manager.h"

const int N = 3;

#if defined(ARDUINO_ARCH_ESP8266)
const int syncPin = D7;
const int pins[N] = {D1, D2, D5};
#elif defined(ARDUINO_ARCH_ESP32)
const int syncPin = 23;
const int pins[N] = {4, 16, 17};
#elif defined(ARDUINO_ARCH_AVR)
const int syncPin = 2;
const int pins[N] = {3, 4, 5};
#elif defined(ARDUINO_ARCH_SAMD)
const int syncPin = 2;
const int pins[N] = {3, 4, 5};
#endif

DimmableLightManager dlm;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println();
  Serial.println("Dimmable Light for Arduino: fourth example");
  
  Serial.println("Init the dimmable light class... ");
  
  // Add all the lights you need
  for(int i=0; i<N; i++){
    if(dlm.add(String("light") + (i+1), pins[i])){
      Serial.println(String("   ") + (i+1) + "-th light added correctly");
    }else{
      Serial.println("   Light isn't added, probably this name was already used...");
    }
  }
  
  DimmableLight::setSyncPin(syncPin);
  DimmableLightManager::begin();
  
  Serial.println("Done!");
}

void loop() {
  for(int b=0; b<255; b+=10){
    for(int i=0; i<dlm.getCount(); i++){
      std::pair<String,DimmableLight*> p = dlm.get();
      String lightName = p.first;
      DimmableLight* dimLight = p.second;
      // Altervatively, you can require to the manager a specific light
      //DimmableLight* dimLight = dlm.get("light1");
      
      Serial.println(String("Setting --") + lightName + "-- to brightness: " + b);
      dimLight->setBrightness(b);
    }
    delay(500);
  }
  Serial.println();
}
