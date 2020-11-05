#include "Motor.h"

#include <Arduino.h>

Motor::Motor(int directionPinId, int speedPinId)
	: m_directionPinId(directionPinId),
	  m_speedPinId(speedPinId)
{
	pinMode(directionPinId, OUTPUT);
	pinMode(speedPinId, OUTPUT);
}

Motor::~Motor()
{
}

void Motor::writeSpeedToBoard(int speed_pwm)
{
	analogWrite(m_speedPinId, speed_pwm);
}

void Motor::writeDirectionToBoard(int direction)
{
	digitalWrite(m_directionPinId, direction);
}
