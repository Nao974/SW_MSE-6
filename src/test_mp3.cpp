#include <Arduino.h>
#include "DYPlayerArduino.h"

// Alternatively initialise on another serial port.
// https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/
const uint8_t MP3RX2 = 10;
const uint8_t MP3TX2 = 11;
DY::Player player(&Serial2);


void setup() {
    Serial2.begin(9600, SERIAL_8N1, MP3RX2, MP3TX2);

    player.begin();
    player.setVolume(20);
    player.setCycleMode(DY::PlayMode::Random); // Play all randomly and repeat.
    player.play();
}

void loop() {
    delay(5000);
}
