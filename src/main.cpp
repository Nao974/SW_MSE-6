#include <Arduino.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
//                      Pilotage,  Trim,     Joystick X, Joystick Y                        
uint8_t btReceive[4] = {0,         0,        0,         0};

#include <L298N.h>
Moteur moteurAvGauche, moteurAvDroite, moteurArGauche, moteurArDroite;
const uint16_t vitesse = 255;

// Formated data
int16_t stickX= 0, stickY=0;

void setup() {
// Initilization of the Bluetooth server
    SerialBT.begin("MSE-6");

// Initialization of motors
	moteurAvGauche.init(2,   4,  3); // IN1, IN2, ENA
	moteurAvDroite.init(5,   7,  6); // IN1, IN2, ENA
	moteurArGauche.init(8,   9, 10); // IN1, IN2, ENA
	moteurArDroite.init(12, 13, 11); // IN1, IN2, ENA    

// Initialization of Serial debug
    Serial.begin(9600);
}

void loop() {
// if reception of the 3 data sent by bluetooth
    if ( SerialBT.available() >= 4) {
        btReceive[0] = SerialBT.read(); // Pilotage
        btReceive[1] = SerialBT.read(); // Trim
        btReceive[2] = SerialBT.read(); // Joystick X
        btReceive[3] = SerialBT.read(); // Joystick Y
        Serial.printf("Pilotage= %d // Trim= %d // Joy X= %d // Joy Y=%d\n", btReceive[0], btReceive[1], btReceive[2], btReceive[3]);

/*        // Recalculates data in servo range
        stickX = map(btReceive[1],0, 248, -225, 225 );
        stickY = map(btReceive[2],0, 248, 110, -110 );
        //Serial.printf("Direction= %d // Stick X= %d // Stick Y=%d\n", btReceive[0], stickX, stickY); */
    }

/*
	//Avant
	moteurAvGauche.speed(vitesse);
	moteurAvDroite.speed(vitesse);
	moteurArGauche.speed(vitesse);
	moteurArDroite.speed(vitesse);
	delay(3000);
	moteurAvGauche.stop();
	moteurAvDroite.stop();	
	moteurArGauche.stop();
	moteurArDroite.stop();	

	//Arriere
	moteurAvGauche.speed(-vitesse);
	moteurAvDroite.speed(-vitesse);
	moteurArGauche.speed(-vitesse);
	moteurArDroite.speed(-vitesse);
	delay(3000);
	moteurAvGauche.stop();
	moteurAvDroite.stop();	
	moteurAvGauche.stop();
	moteurAvDroite.stop();	
*/
}
