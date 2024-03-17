#include <Arduino.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
uint8_t btReceive[4] = {0, 0,  0, 0}; // Pilotage,  Trim, Joy X / Range Left, Joy Y / Range Right 

#include <L298N.h>
Moteur moteurAvGauche, moteurAvDroite, moteurArGauche, moteurArDroite;
int16_t vitesseBase=0, directionBase=0, vitesseGauche= 0, vitesseDroite=0;
const uint8_t VITESSE_MAX = 125; // Entre 0 et 255, avec batterie 3S => 7,2V sur les moteurs 

#define DEBUG	true

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
	moteurAvGauche.init(18,  5, 19, VITESSE_MAX); // IN1, IN2, ENA
	moteurAvDroite.init(17, 16,  4, VITESSE_MAX); // IN1, IN2, ENA
	moteurArGauche.init(26, 25, 33, VITESSE_MAX); // IN1, IN2, ENA
	moteurArDroite.init(14, 27, 12, VITESSE_MAX); // IN1, IN2, ENA

}

void loop() {

	//btReceive[2] = 125;
	//btReceive[3] = 188;

	// Recalculates data in motor range
	directionBase = 0; // map(btReceive[2],0, 250, -255, 255 )+btReceive[1];
	vitesseBase = 255; //map(btReceive[3],0, 250, 255, -255 );



	#if DEBUG == true
		Serial.printf("Pilotage= %d // Trim= %d // Joy X= %d // Joy Y=%d\n", btReceive[0], btReceive[1], btReceive[2], btReceive[3]);
		Serial.printf("Vitesse base= %d, direction base=%d",vitesseBase,directionBase);
		Serial.printf("Moteurs Gauche= %d // Moteurs Droit=%d\n", vitesseGauche, vitesseDroite);
	#endif

	moteurAvDroite.speed(vitesseBase);
	moteurAvGauche.speed(-vitesseBase);
	moteurArDroite.speed(vitesseBase);
	moteurArGauche.speed(-vitesseBase);


}
