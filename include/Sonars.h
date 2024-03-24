#include <Arduino.h>
#include <NewPing.h>
#include "BluetoothSerial.h"

const uint16_t SONAR_MAX_DISTANCE = 1000; // Maximum distance (in cm) to ping.
const uint16_t SONAR_LIMIT_ORANGE = 40; // in cm
const uint16_t SONAR_LIMIT_RED = 25; // in cm
const uint8_t SONAR_NUMBER = 4;
const uint16_t SONAR_INTERVAL = 50; // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
uint16_t sonarDistance = 0;
uint8_t sonarID = 0;         
uint32_t sonarLastMillis = 0;  

extern uint8_t sonarStatus[SONAR_NUMBER];
extern NewPing sonar[SONAR_NUMBER];
extern BluetoothSerial SerialBT;

void UpdateSonars_SendBT(const uint8_t sendBT) {
    if ( millis() > sonarLastMillis + SONAR_INTERVAL) { 
        sonarDistance = sonar[sonarID].ping_cm();

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
        sonarID = ++sonarID % SONAR_NUMBER;
        sonarLastMillis = millis();
    }
}
