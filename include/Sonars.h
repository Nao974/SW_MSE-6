#include <Arduino.h>
#include <NewPing.h>
#include "BluetoothSerial.h"

const uint16_t SONAR_MAX_DISTANCE = 2000; // Maximum distance (in cm) to ping.
const uint16_t SONAR_LIMIT_ORANGE = 60; // in cm
const uint16_t SONAR_LIMIT_RED = 30; // in cm
const uint8_t SONAR_NUMBER = 4;
const uint16_t SONAR_INTERVAL = 100; // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
uint16_t sonarDistance = 0;
uint8_t sonarID = 0;         
uint32_t sonarLastMillis = 0;  

extern uint8_t sonarStatus[SONAR_NUMBER];
extern NewPing sonar[SONAR_NUMBER];
extern BluetoothSerial SerialBT;

void UpdateSonars_SendBT(const uint8_t sendBT) {
    if ( millis() > sonarLastMillis + SONAR_INTERVAL) { 
        sonarDistance = sonar[sonarID].ping_cm();
        if ( sonarDistance < 2) sonarDistance = 255;

        if ( sonarDistance <= SONAR_LIMIT_RED ) {
            if ( sonarStatus[sonarID] != 2 ){
                sonarStatus[sonarID] = 2;
                if (sendBT) {
                    SerialBT.print(sonarID);
                    SerialBT.println("2");
                }
            }
        }
        else if ( sonarDistance <= SONAR_LIMIT_ORANGE ){
            if ( sonarStatus[sonarID] != 1 ) {
                sonarStatus[sonarID] = 1;
                if (sendBT) {
                    SerialBT.print(sonarID);
                    SerialBT.println("1"); 
                }
            }
        }
        else if ( sonarStatus[sonarID] != 0) {
                sonarStatus[sonarID] = 0;
                if (sendBT) {
                    SerialBT.print(sonarID);
                    SerialBT.println("0");   
                }           
        }

        #if DEBUG == true
            {Serial.print(sonarID);Serial.print(": ");Serial.println(sonarDistance);}
        #endif

        sonarID = ++sonarID % SONAR_NUMBER;
        sonarLastMillis = millis();
    }
}
