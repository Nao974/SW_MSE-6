#include <Arduino.h>
#include "DYPlayerArduino.h"

// Alternatively initialise on another serial port.
// https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/
// https://github.com/SnijderC/dyplayer
const uint8_t MP3RX2 = 22;
const uint8_t MP3TX2 = 23;
DY::Player player(&Serial2);


void setup() {
    Serial2.begin(9600, SERIAL_8N1, MP3RX2, MP3TX2);

  player.begin();
  // Also initiate the hardware serial port so we can use it for debug printing
  // to the console..
  Serial.begin(9600);
  player.setVolume(15); 
  // player.setCycleMode(DY::PlayMode::Sequence); // Play all and stop.
  player.setCycleMode(DY::PlayMode::Repeat); // Play all and repeat.
  //player.setCycleMode(DY::PlayMode::Random); // Play all randomly and repeat.
    //player.next();
 //player.previous (); 
 //player.volumeIncrease();
  //player.volumeDecrease();
  
 //player.playSpecified(4);
  player.play();
}

void loop() {

  // Print the number of the sound that is playing.
  Serial.print("Playing sound: ");
  Serial.println((int16_t)player.getPlayingSound());
  delay(3000);
    
  
}
/*
  // For some reason periods need to be stars for this module.
  // Future releases will do this automatically.
 /* char path[] = "/00001.MP3";
  player.playSpecifiedDevicePath(DY::Device::Flash, path);
  delay(500);
  Serial.print("Playing sound: ");
  Serial.println((int16_t)player.getPlayingSound());
  delay(4000);
*/
