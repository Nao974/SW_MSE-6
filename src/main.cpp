#include <Arduino.h>

#include "DYPlayerArduino.h"
const uint8_t MP3RX2 = 22;
const uint8_t MP3TX2 = 21;
DY::Player player(&Serial2);

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
int16_t btReceive[4] = {0, 0,  0, 0}; // Pilotage,  Trim, Joy X / Range Left, Joy Y / Range Right 

#include <L298N.h>
Moteur moteurGauche, moteurDroite;
int16_t vitesseBase;
int16_t reculeDuree;
int32_t reculeLastMillis;

#include "Direction.h"
#include <ESP32Servo.h>
Servo servoDir;
int16_t directionBase;

#include "Sonars.h"
#include <NewPing.h>
uint8_t sonarStatus[SONAR_NUMBER]= {0, 0, 0, 0};
NewPing sonar[SONAR_NUMBER]= {
    NewPing( 32, 34, SONAR_MAX_DISTANCE),  // Left Front sensor (trigger pin, echo pin, and max distance to ping).
    NewPing( 17, 16, SONAR_MAX_DISTANCE),  // Left Back
    NewPing( 23, 35, SONAR_MAX_DISTANCE),  // Right Front
    NewPing( 19, 18, SONAR_MAX_DISTANCE)  // Right Back
};


// General
uint8_t drivingMode = true, autonomousState= 1;
uint32_t debugLastMillis, autonomousLastMillis, autonomousWaitLastMillis, autonomousWait;
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
	vitesseBase = 0;

// Initialization Direction
	#if DEBUG== true 
		Serial.println("Servo init"); 
	#endif
	servoDir.attach(4, 1000, 2000);
	directionBase = 90;
	servoDir.write(directionBase);

// Initialization Sonars
	#if DEBUG== true 
		Serial.println("Sonars init"); 
	#endif
    sonarID = 0;
    sonarLastMillis = millis();

// Initialization MP3 Player
  
  Serial2.begin(9600, SERIAL_8N1, MP3RX2, MP3TX2);
  player.begin();
  player.setVolume(25); //Set the playback volume between 0 and 30. Default volume if not set: 20
  player.setCycleMode(DY::PlayMode::Random); // Play all randomly and repeat.
  player.play();

// General
	drivingMode = true; // manual control at start-up
	autonomousState = 1; //1= forward, 2= back, 3= back wait because obstacle
}

void loop() {

	UpdateSonars_SendBT(drivingMode);

	// if reception of data sent by bluetooth
    if ( SerialBT.available() >= 4) {
        btReceive[0] = SerialBT.read(); // Pilotage
        btReceive[1] = SerialBT.read() - 10; // Trim original value -10 to 10, send BT 0 to 20, therefore -10 to find the original value
        btReceive[2] = SerialBT.read(); // Joystick X Direction
        btReceive[3] = SerialBT.read(); // Joystick Y Vitesse

		drivingMode = btReceive[0];
		if ( drivingMode) {
			// Recalculates data in motor range
			directionBase = map(btReceive[2],250, 0, SERVO_MIN, SERVO_MAX ) + btReceive[1];
			vitesseBase = map(btReceive[3],0, 250, 255, -255 );

			if ( ( sonarStatus[0] == 2 || sonarStatus[2] == 2) && vitesseBase > 0 )
				vitesseBase = 0;
			if ( ( sonarStatus[1] == 2 || sonarStatus[3] == 2) && vitesseBase < 0 )
				vitesseBase = 0;

			#if DEBUG == true
				Serial.printf("Pilotage= %d // Trim= %d // Joy X= %d // Joy Y=%d\n", btReceive[0], btReceive[1], btReceive[2], btReceive[3]);
			#endif
		}
	}

	if ( !drivingMode ) { // autonomous mode

		switch ( autonomousState) {
			case 1: { // Forward
				directionBase = SERVO_MIDDLE;
				vitesseBase = VITESSE_MIN;

				if ( sonarStatus[0] == 1 ) { // Left Front
					directionBase = SERVO_DROITE;
				}
				if ( sonarStatus[2] == 1 ) {// Right Front
					directionBase = SERVO_GAUCHE;
				}
				if ( sonarStatus[0] == 2 || sonarStatus[2] == 2 )  {
					autonomousState = 2;
					autonomousLastMillis = millis();
					autonomousWait = 0;
				}
				break;
			}

			case 2: { // go back
				if ( millis() > autonomousLastMillis+ autonomousWait + 2000)
					autonomousState = 1;
				else {
					directionBase = SERVO_DROITE;
					vitesseBase = -VITESSE_MIN;
					if (sonarStatus[1] == 2 || sonarStatus[3] == 2) { // obstacle at the back
						vitesseBase = 0;
						autonomousWaitLastMillis = millis();
						autonomousState = 3;
					}
				}
				break;
			}

			case 3: { // obstacle at the back, we wait
				if (sonarStatus[1] != 2 && sonarStatus[3] != 2) {
						autonomousWait = autonomousWait + ( millis() - autonomousWaitLastMillis );
						autonomousState = 2;
					}
				else if ( millis() > autonomousLastMillis + 7500 )
					autonomousState = 1;
			}
		}
	}
	#if DEBUG == true
		if ( millis() > debugLastMillis+ 500) {
			Serial.printf("Vitesse base= %d, direction base=%d \n",vitesseBase,directionBase);
			debugLastMillis = millis();
		}
	#endif
	turnSecureMode(directionBase);
	moteurGauche.speed(vitesseBase);
	moteurDroite.speed(vitesseBase);
}
