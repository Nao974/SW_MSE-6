#include <ESP32Servo.h>

const uint8_t SERVO_MIDDLE = 90, SERVO_MAX = 120, SERVO_MIN = 60;
extern Servo servoDir;
const uint8_t SERVO_GAUCHE = SERVO_MAX, SERVO_DROITE = SERVO_MIN;

void turnSecureMode(int16_t angle) {
    if ( angle > SERVO_MAX )
		angle = SERVO_MAX;
	else if ( angle < SERVO_MIN )
		angle = SERVO_MIN;
	servoDir.write(angle);
}