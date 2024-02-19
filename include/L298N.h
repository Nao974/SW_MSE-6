#include <arduino.h>

struct Moteur {
	uint8_t pinIN1;
	uint8_t pinIN2;
	uint8_t pinENA;
	uint16_t deadZone;

	void init( uint8_t _pinIN1, uint8_t _pinIN2, uint8_t _pinENA, uint8_t _deadZone) {
		pinIN1 = _pinIN1;
		pinMode(pinIN1, OUTPUT);
		pinIN2 = _pinIN2;
		pinMode(pinIN2, OUTPUT);
		pinENA = _pinENA;
		pinMode(pinENA, OUTPUT);
		deadZone = _deadZone;
	}

	void speed(int16_t vitesse) {
		if (vitesse > 0) {
			digitalWrite(pinIN1, HIGH);
			digitalWrite(pinIN2, LOW);
		}
		else {
			digitalWrite(pinIN1, LOW);
			digitalWrite(pinIN2, HIGH);	
			vitesse = - vitesse;	
		}
		vitesse = map(vitesse, 0,255, deadZone, 255);
		analogWrite(pinENA, vitesse);
		}				

	void stop() {
		digitalWrite(pinIN1, HIGH);
		digitalWrite(pinIN2, HIGH);
	}

};