#include "Relay.h"

#include <Arduino.h>

Relay::Relay(int pinId) : m_pinId(pinId)
{
	pinMode(pinId, OUTPUT);
}

Relay::~Relay()
{
}

void Relay::setEnabled(bool enable)
{
	if (enable)
	{
		writeEnableToBoard();
	}
	else
	{
		writeDisableToBoard();
	}
}

void Relay::writeEnableToBoard()
{
	digitalWrite(m_pinId, HIGH);
}

void Relay::writeDisableToBoard()
{
	digitalWrite(m_pinId, LOW);
}
