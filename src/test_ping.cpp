#include <Arduino.h>
#include <NewPing.h>

const uint8_t SONAR_MAX_DISTANCE=1000; // Maximum distance (in cm) to ping.

NewPing sonarLeft( 32, 34, SONAR_MAX_DISTANCE);  // Left sensor (trigger pin, echo pin, and max distance to ping).
NewPing sonarRight(33, 35, SONAR_MAX_DISTANCE);  // Right sensor


void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.print("sonarLeft="); Serial.print(sonarLeft.ping_cm()); Serial.print("cm ");
    Serial.print("sonarRight="); Serial.print(sonarRight.ping_cm()); Serial.print("cm ");
    delay(250); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
}
