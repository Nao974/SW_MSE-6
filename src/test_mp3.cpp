#include <Arduino.h>
#include "DYPlayerArduino.h"

// Alternatively initialise on another serial port.
// https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/
const uint8_t MP3RX2 = 22;
const uint8_t MP3TX2 = 23;

// YPlayer Arduino: https://github.com/SnijderC/dyplayer
DY::Player player(&Serial2);

#define DEBUG   false

void setup() {
    // Init serial for comannd MP3 Player
    Serial2.begin(9600, SERIAL_8N1, MP3RX2, MP3TX2);

    // Init Player
    player.begin();

    #if DEBUG == true
        Serial.begin(115200);
    #endif

  player.setVolume(25); //Set the playback volume between 0 and 30. Default volume if not set: 20
  //player.setCycleMode(DY::PlayMode::Repeat); // Play all and repeat.
  player.setCycleMode(DY::PlayMode::Random); // Play all randomly and repeat.
  player.play();
}

void loop() {

    #if DEBUG== true
        Serial.print("Playing sound: ");
        Serial.println((int16_t)player.getPlayingSound());
        delay(2000);
    #endif    
  
}
