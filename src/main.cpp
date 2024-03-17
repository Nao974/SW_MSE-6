#include <Arduino.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
int16_t btReceive[4] = {0, 0,  0, 0}; // Pilotage,  Trim, Joy X / Range Left, Joy Y / Range Right 

#include <L298N.h>
Moteur moteurGauche, moteurDroite;
const uint8_t VITESSE_MAX = 250; // Entre 0 et 255
int16_t vitesseBase;

#include <ESP32Servo.h>
Servo servoDir;
const uint8_t SERVO_MIDDLE = 90, SERVO_MAX = 120, SERVO_MIN = 60;
int16_t directionBase;


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
	moteurGauche.init(26, 25, 33, VITESSE_MAX); // IN1, IN2, ENA
	moteurDroite.init(14, 27, 12, VITESSE_MAX); // IN1, IN2, ENA

// Initialization Direction
	#if DEBUG== true 
		Serial.println("Servo init"); 
	#endif
	servoDir.attach(4, 1000, 2000);
	servoDir.write(90);

}

void loop() {
	// if reception of the 3 data sent by bluetooth
    if ( SerialBT.available() >= 4) {
        btReceive[0] = SerialBT.read(); // Pilotage
        btReceive[1] = SerialBT.read(); // Trim
        btReceive[2] = SerialBT.read(); // Joystick X Direction
        btReceive[3] = SerialBT.read(); // Joystick Y Vitesse

		// Recalculates data in motor range
        directionBase = map(btReceive[2],250, 0, SERVO_MIN, SERVO_MAX );
        vitesseBase = map(btReceive[3],0, 250, 255, -255 );

		#if DEBUG == true
			Serial.printf("Pilotage= %d // Trim= %d // Joy X= %d // Joy Y=%d\n", btReceive[0], btReceive[1], btReceive[2], btReceive[3]);
			Serial.printf("Vitesse base= %d, direction base=%d \n",vitesseBase,directionBase);
			//Serial.printf("Moteurs Gauche= %d // Moteurs Droit=%d\n", vitesseGauche, vitesseDroite);
		#endif

		if ( directionBase+btReceive[1]> SERVO_MAX )
			directionBase = SERVO_MAX;
		else if ( directionBase+btReceive[1]< SERVO_MIN )
			directionBase = SERVO_MIN;
		else directionBase = directionBase+btReceive[1];		
		servoDir.write(directionBase);
		
		moteurGauche.speed(vitesseBase);
		moteurDroite.speed(vitesseBase);
		
    }

}
