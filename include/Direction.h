#include <ESP32Servo.h>

const uint8_t SERVO_MIDDLE = 95, SERVO_MAX = 160, SERVO_MIN = 20;
extern Servo servoDir;
const uint8_t SERVO_GAUCHE = SERVO_MIN, SERVO_DROITE = SERVO_MAX;

void turnSecureMode(int16_t angle) {
    if ( angle > SERVO_MAX )
		angle = SERVO_MAX;
	else if ( angle < SERVO_MIN )
		angle = SERVO_MIN;
	servoDir.write(angle);
}
