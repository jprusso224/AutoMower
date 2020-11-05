
#include "Relay.h"
#include "PPM_Reader.h"

#define PPM_RECEIVE_PIN 2
//#define PPM_NUM_CHANNELS 6
//#define PPM_DETECTION_SPACE 5000
//#define PPM_INTERRUPT_METHOD FALLING

// FUTABA CHANNEL MAP STD
#define PPM_CHANNEL_AILERON 0
#define PPM_CHANNEL_ELEVATOR 1
#define PPM_CHANNEL_THROTTLE 2
#define PPM_CHANNEL_RUDDER 3
#define PPM_CHANNEL_GEAR 4
#define PPM_CHANNEL_AUX1 5

// OUTPUT PINS
#define MOTOR_DIR_1 7
#define MOTOR_DIR_2 8
#define MOTOR_SPEED_1 10
#define MOTOR_SPEED_2 11
#define ALTERNATOR_ENABLE 12
#define MOTOR_ENABLE 13

#define FORWARD 1
#define REVERSE -1

#define DEADBAND 20

Relay * motorRelay;
PPM_Reader * ppmReader;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(250000);

  //pinMode(13, OUTPUT);
  pinMode(MOTOR_DIR_1, OUTPUT);
  pinMode(MOTOR_DIR_2, OUTPUT);
  pinMode(MOTOR_SPEED_1, OUTPUT);
  pinMode(MOTOR_SPEED_2, OUTPUT);
  pinMode(ALTERNATOR_ENABLE, OUTPUT);

  motorRelay = new Relay(static_cast<uint8_t>(MOTOR_ENABLE));
  ppmReader = new PPM_Reader(static_cast<uint8_t>(PPM_RECEIVE_PIN), interruptProcessPPM_Wrapper);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  Serial.print(ppmReader->getChannelOutput_us(PPM_CHANNEL_AILERON));Serial.print("\t");
  Serial.print(ppmReader->getChannelOutput_us(PPM_CHANNEL_ELEVATOR));Serial.print("\t");
  Serial.print(ppmReader->getChannelOutput_us(PPM_CHANNEL_THROTTLE)); Serial.print("\t");
  Serial.print(ppmReader->getChannelOutput_us(PPM_CHANNEL_RUDDER)); Serial.print("\t");
  Serial.print(ppmReader->getChannelOutput_us(PPM_CHANNEL_GEAR)); Serial.print("\t");
  Serial.print(ppmReader->getChannelOutput_us(PPM_CHANNEL_AUX1)); Serial.print("\n");
  
    //unsigned long int throttle_us = ppmChannelPeakDeltas_us[PPM_CHANNEL_THROTTLE];

	int motorInhibitPwm = ppmReader->getChannelOutput_us(PPM_CHANNEL_GEAR);
	if (motorInhibitPwm >= 1500)
	{
		motorRelay->setEnabled(false);
		Serial.print("DISABLE\n");
	}
	else
	{
		motorRelay->setEnabled(true);
		Serial.print("ENABLE\n");
	}

    int steeringPWM = ppmReader->getChannelOutput_pwm(PPM_CHANNEL_AILERON);
    int throttlePWM = ppmReader->getChannelOutput_pwm(PPM_CHANNEL_ELEVATOR);
    //int throttleLED = map(throttle_us, 1000, 2000, 0, 255);

    int throttle = 0;
    if(getMotorDirectionFromInputs(throttlePWM) == FORWARD)
    {
        digitalWrite(MOTOR_DIR_1, LOW);
        digitalWrite(MOTOR_DIR_2, HIGH);
        throttle = map(throttlePWM, 128, 255, 0, 255);  
    }
    else
    {
        digitalWrite(MOTOR_DIR_1, HIGH);
        digitalWrite(MOTOR_DIR_2, LOW);
        throttle = map(throttlePWM, 128, 0, 0, 255);
    }

    int t1 = 0;
    int t2 = 0;
    int steering = 0;
    if(steeringPWM > 128) //Right
    {
        Serial.print("RIGHT");Serial.print("\n");
        steering = map(steeringPWM, 128, 255, 0, throttle); 
        t2 = throttle - steering;
        t1 = throttle;
    }
    else
    {
        Serial.print("LEFT");Serial.print("\n");
        steering = map(steeringPWM, 128, 0, 0, throttle); 
        t2 = throttle;
        t1 = throttle - steering;
    }

    Serial.print(throttle);Serial.print("\n");

   // analogWrite(13, throttleLED);

    if(throttle >= DEADBAND)
    {
        analogWrite(MOTOR_SPEED_1, t1);
        analogWrite(MOTOR_SPEED_2, t2);
    }
    else
    {
        analogWrite(MOTOR_SPEED_1, 0);
        analogWrite(MOTOR_SPEED_2, 0);
    }
  
    delay(50);
  
 // Serial.println(pulseIn(PPM_RECEIVE_PIN, HIGH));

 // Serial.println(analogRead(A1));
  
}

int getMotorDirectionFromInputs(int throttlePWM)
{
    if(throttlePWM > 128)
    {
       return FORWARD;
    }
    else
    {
       return REVERSE;
    }
}

void interruptProcessPPM_Wrapper()
{
	if (ppmReader != nullptr)
	{
		ppmReader->interruptProcessPPM();
	}
}