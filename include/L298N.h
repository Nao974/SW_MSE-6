#include <arduino.h>
const uint8_t VITESSE_MAX = 250; // between 0 and 255

struct Moteur {
	uint8_t pinIN1;
	uint8_t pinIN2;
	uint8_t pinENA;
	uint8_t vitesseMax;

	void init( uint8_t _pinIN1, uint8_t _pinIN2, uint8_t _pinENA, uint8_t _vitesseMax) {
		pinIN1 = _pinIN1;
		pinMode(pinIN1, OUTPUT);
		pinIN2 = _pinIN2;
		pinMode(pinIN2, OUTPUT);
		pinENA = _pinENA;
		pinMode(pinENA, OUTPUT);
		vitesseMax = _vitesseMax;
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
		if (vitesse > vitesseMax)
			vitesse = vitesseMax;
		analogWrite(pinENA, vitesse>vitesseMax?vitesseMax:vitesse);
		}				

	void stop() {
		digitalWrite(pinIN1, HIGH);
		digitalWrite(pinIN2, HIGH);
	}

};