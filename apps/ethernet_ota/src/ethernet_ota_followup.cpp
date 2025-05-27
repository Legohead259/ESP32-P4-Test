#include <Arduino.h>
#include <ETH.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <Controller.h>

//variabls for blinking an LED with Millis
const int led = 2; // ESP32 Pin to which onboard LED is connected
unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 1000;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED

void setup() {

    pinMode(led, OUTPUT);
    
    Serial.begin(115200);
    Serial.println("Booting");
    controller.begin();
}

void loop() {    
    //loop to blink without delay
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        Serial.println("Working...");   
    }
}