#pragma once

static const int PPM_NUM_CHANNELS = 6;
static const int PPM_DETECTION_SPACE = 5000;
static const int PPM_MAX_US = 2000;
static const int PPM_MIN_US = 1000;
static const int PWM_MAX = 255;
static const int PWM_MIN = 0;

class PPM_Reader
{
public:
	PPM_Reader(int pinId, void(*interruptWrapper)(void));
	~PPM_Reader();

	int getChannelOutput_us(int channelId);
	int getChannelOutput_pwm(int channelId);

	void interruptProcessPPM();

private:

	int m_pinId = 0;	

	int ppmCurrentChannelIndex = 0; //Index 0 can be used to check the message spacing. Channel 1 is at index 1.
	int ppmChannelPeakDeltas_us[PPM_NUM_CHANNELS + 1]; 

	// PPM Time variables
	unsigned long int ppmRisingTimeStampPrev_us = 0;
	unsigned long int ppmPeakDelta_us = 0;
};
 