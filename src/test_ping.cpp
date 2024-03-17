#include <Arduino.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#include <NewPing.h>
const uint16_t SONAR_MAX_DISTANCE = 1000; // Maximum distance (in cm) to ping.
const uint16_t SONAR_LIMIT_ORANGE = 40; // in cm
const uint16_t SONAR_LIMIT_RED = 25; // in cm
const uint8_t SONAR_NUMBER = 2;
uint16_t sonarDistance = 0;
uint8_t sonarID = 0;         
uint8_t sonarStatus[]= {0,    // Left Front
                        0,    // Left Back
                           0, // Right Front
                           0};// Right Back
NewPing sonar[SONAR_NUMBER]= {
    NewPing( 32, 34, SONAR_MAX_DISTANCE),  // Left Front sensor (trigger pin, echo pin, and max distance to ping).
    // Left Back Sensr
    NewPing( 23, 35, SONAR_MAX_DISTANCE)  // Right Front sensor
    // Right Back Senser
};

uint32_t sonarLastMillis = 0;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("MSE-6");
    sonarID = 0;
    sonarLastMillis = millis();
}

void loop() {
    if ( millis() > sonarLastMillis+50) { // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
        sonarDistance = sonar[sonarID].ping_cm();
        //Serial.print("Lecture sonar N"); Serial.println(sonarID);

        if ( sonarDistance <= SONAR_LIMIT_RED ) {
            if ( sonarStatus[sonarID] != 2 ){
                Serial.print("Sonar N"); Serial.print(sonarID);Serial.println(": Red");
                sonarStatus[sonarID] = 2;
                SerialBT.print(sonarID);
                SerialBT.println("2");
            }
        }
        else if ( sonarDistance <= SONAR_LIMIT_ORANGE ){
            if ( sonarStatus[sonarID] != 1 ) {
                Serial.print("Sonar N"); Serial.print(sonarID);Serial.println(": Orange");
                sonarStatus[sonarID] = 1;
                SerialBT.print(sonarID);
                SerialBT.println("1");            }
        }
        else if ( sonarStatus[sonarID] != 0) {
                Serial.print("Sonar N"); Serial.print(sonarID);Serial.println(": Green");
                sonarStatus[sonarID] = 0;
                SerialBT.print(sonarID);
                SerialBT.println("0");              
        }
        sonarID = ++sonarID % SONAR_NUMBER;
        sonarLastMillis = millis();
    }
}
