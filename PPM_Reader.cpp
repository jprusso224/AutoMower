#include "PPM_Reader.h"

#include <Arduino.h>

PPM_Reader::PPM_Reader(int pinId, void(*interruptWrapper)(void))
	: m_pinId(pinId)
{
	pinMode(pinId, INPUT);
	attachInterrupt(digitalPinToInterrupt(pinId), interruptWrapper , FALLING);
}

PPM_Reader::~PPM_Reader()
{
}

int PPM_Reader::getChannelOutput_us(int channelId)
{
	int ppmReading_us = ppmChannelPeakDeltas_us[channelId];

	if (ppmReading_us < PPM_MIN_US)
	{
		ppmReading_us = PPM_MIN_US;
	}
	if (ppmReading_us > PPM_MAX_US)
	{
		ppmReading_us = PPM_MAX_US;
	}

	return ppmReading_us;
}

int PPM_Reader::getChannelOutput_pwm(int channelId)
{
	int ppmReading_us = getChannelOutput_us(channelId);

	int pwmConversion = map(ppmReading_us, PPM_MIN_US, PPM_MAX_US, PWM_MIN, PWM_MAX);

	return pwmConversion;
}

void PPM_Reader::interruptProcessPPM()
{
	unsigned long int ppmRisingTimeStampCurr_us = micros();

	ppmPeakDelta_us = ppmRisingTimeStampCurr_us - ppmRisingTimeStampPrev_us;

	ppmChannelPeakDeltas_us[ppmCurrentChannelIndex] = ppmPeakDelta_us;
	ppmCurrentChannelIndex++;

	if ((ppmCurrentChannelIndex > PPM_NUM_CHANNELS) || (ppmPeakDelta_us > PPM_DETECTION_SPACE))
	{
		ppmCurrentChannelIndex = 0;
	}

	ppmRisingTimeStampPrev_us = ppmRisingTimeStampCurr_us;
}
