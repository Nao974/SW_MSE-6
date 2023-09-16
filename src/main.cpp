#include <Arduino.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
//                      Pilotage,  Trim,     Joystick X, Joystick Y                        
uint8_t btReceive[4] = {0,         0,        0,         0};

#include <L298N.h>
Moteur moteurAvGauche, moteurAvDroite, moteurArGauche, moteurArDroite;
int16_t vitesseBase=0, directionBase=0, vitesseGauche= 0, vitesseDroite=0;

#define DEBUG	false

void setup() {
// Initialization of Serial debug
    #if DEBUG== true 
		Serial.begin(115200);
	#endif

// Initilization of the Bluetooth server
	#if DEBUG== true 
		Serial.println("SerialBT init"); 
	#endif
    SerialBT.begin("MSE-6");

// Initialization of motors
	#if DEBUG== true 
		Serial.println("Motors init"); 
	#endif
	moteurAvGauche.init(18,  5, 19, 70); // IN1, IN2, ENA
	moteurAvDroite.init(17, 16,  4, 140); // IN1, IN2, ENA
	moteurArGauche.init(26, 25, 33, 100); // IN1, IN2, ENA
	moteurArDroite.init(14, 27, 12, 105); // IN1, IN2, ENA

}

void loop() {
	// if reception of the 3 data sent by bluetooth
    if ( SerialBT.available() >= 4) {
        btReceive[0] = SerialBT.read(); // Pilotage
        btReceive[1] = SerialBT.read(); // Trim
        btReceive[2] = SerialBT.read(); // Joystick X Direction
        btReceive[3] = SerialBT.read(); // Joystick Y Vitesse

		// Recalculates data in motor range
        directionBase = map(btReceive[2],0, 250, -255, 255 )+btReceive[1];
        vitesseBase = map(btReceive[3],0, 250, 255, -255 );

		vitesseGauche= vitesseBase + directionBase;
		vitesseDroite= vitesseBase - directionBase;

		if ( vitesseGauche > 255 ) vitesseGauche = 255;
		if ( vitesseGauche < -255 ) vitesseGauche = -255;
		if ( vitesseDroite > 255 ) vitesseDroite = 255;
		if ( vitesseDroite < -255 ) vitesseDroite = -255;

		#if DEBUG == true
			Serial.printf("Pilotage= %d // Trim= %d // Joy X= %d // Joy Y=%d\n", btReceive[0], btReceive[1], btReceive[2], btReceive[3]);
			Serial.printf("Vitesse base= %d, direction base=%d",vitesseBase,directionBase);
			Serial.printf("Moteurs Gauche= %d // Moteurs Droit=%d\n", vitesseGauche, vitesseDroite);
		#endif

		moteurAvGauche.speed(vitesseGauche);
		moteurAvDroite.speed(vitesseDroite);
		moteurArGauche.speed(vitesseGauche);
		moteurArDroite.speed(vitesseDroite);
    }

}
