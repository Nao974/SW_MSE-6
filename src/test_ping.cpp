#include <Arduino.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#include "Sonars.h"
#include <NewPing.h>
uint8_t sonarStatus[SONAR_NUMBER]= {0, 0, 0, 0};
NewPing sonar[SONAR_NUMBER]= {
    NewPing( 32, 34, SONAR_MAX_DISTANCE),  // Left Front sensor (trigger pin, echo pin, and max distance to ping).
    NewPing( 17, 16, SONAR_MAX_DISTANCE),  // Left Back
    NewPing( 23, 35, SONAR_MAX_DISTANCE),  // Right Front
    NewPing( 19, 18, SONAR_MAX_DISTANCE)  // Right Back

};

void setup() {
    Serial.begin(115200);
    SerialBT.begin("MSE-6");
    sonarID = 0;
    sonarLastMillis = millis();
}

void loop() {
    UpdateSonars_SendBT(true);
}
