/**
 * @file main.cpp
 * @version 0.1.0
 * @author Braidan Duffy (braidan.duffy@gmail.com)
 * @brief 
 * @date May 10, 2025
 *       May 10, 2025 (last edit)
 * 
 * CHANGELOG:
 * Version 0.1.0 - Initial baseline release
**/
#include <Arduino.h>
#include "microros__test.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Hello World!");
    Serial.println(TEST_PIN);
}

void loop() {
}