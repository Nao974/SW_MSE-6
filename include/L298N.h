#include <arduino.h>

struct Moteur {
	uint8_t pinIN1;
	uint8_t pinIN2;
	uint8_t pinENA;

	void init( uint8_t PIN_IN1, uint8_t PIN_IN2, uint8_t PIN_ENA) {
		pinIN1 = PIN_IN1;
		pinMode(pinIN1, OUTPUT);
		pinIN2 = PIN_IN2;
		pinMode(pinIN2, OUTPUT);
		pinENA = PIN_ENA;
		pinMode(pinENA, OUTPUT);	
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
		analogWrite(pinENA, vitesse);
		}				

	void stop() {
		digitalWrite(pinIN1, HIGH);
		digitalWrite(pinIN2, HIGH);
	}

};