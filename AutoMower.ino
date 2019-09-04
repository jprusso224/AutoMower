#define PPM_RECEIVE_PIN 2
#define PPM_NUM_CHANNELS 6
#define PPM_DETECTION_SPACE 5000
#define PPM_INTERRUPT_METHOD FALLING

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

#define FORWARD 1
#define REVERSE -1

#define DEADBAND 20

int ppmCurrentChannelIndex = 0; //Index 0 can be used to check the message spacing. Channel 1 is at index 1.
int ppmChannelPeakDeltas_us[PPM_NUM_CHANNELS + 1];

// PPM Time variables
unsigned long int ppmRisingTimeStampPrev_us = 0;
unsigned long int ppmPeakDelta_us = 0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(250000);
  pinMode(PPM_RECEIVE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PPM_RECEIVE_PIN), interruptProcessPPM, PPM_INTERRUPT_METHOD);

  //pinMode(13, OUTPUT);
  pinMode(MOTOR_DIR_1, OUTPUT);
  pinMode(MOTOR_DIR_2, OUTPUT);
  pinMode(MOTOR_SPEED_1, OUTPUT);
  pinMode(MOTOR_SPEED_2, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  Serial.print(ppmChannelPeakDeltas_us[PPM_CHANNEL_AILERON]);Serial.print("\t");
  Serial.print(ppmChannelPeakDeltas_us[PPM_CHANNEL_ELEVATOR]);Serial.print("\n");

  /*
  Serial.print(ppmChannelPeakDeltas_us[PPM_CHANNEL_THROTTLE]);Serial.print("\t");
  Serial.print(ppmChannelPeakDeltas_us[PPM_CHANNEL_RUDDER]);Serial.print("\t");
  Serial.print(ppmChannelPeakDeltas_us[PPM_CHANNEL_GEAR]);Serial.print("\t");
  Serial.print(ppmChannelPeakDeltas_us[PPM_CHANNEL_AUX1]); Serial.print("\n");
  */
  
 

    //unsigned long int throttle_us = ppmChannelPeakDeltas_us[PPM_CHANNEL_THROTTLE];
  
    unsigned long int steering_us = ppmChannelPeakDeltas_us[PPM_CHANNEL_AILERON];
    unsigned long int throttle_us = ppmChannelPeakDeltas_us[PPM_CHANNEL_ELEVATOR];

    if(throttle_us < 1000)
    {
      throttle_us = 1000;
    }
    if(throttle_us > 2000)
    {
      throttle_us = 2000;
    }
    
    if(steering_us < 1000)
    {
      steering_us = 1000;
    }
    if(steering_us > 2000)
    {
      steering_us = 2000;
    }

    int steeringPWM = map(steering_us, 1000, 2000, 0, 255);
    int throttlePWM = map(throttle_us, 1000, 2000, 0, 255);
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
        steering = map(steeringPWM, 128, 0, 0, throttle+); 
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

void interruptProcessPPM()
{
    unsigned long int ppmRisingTimeStampCurr_us = micros();

    ppmPeakDelta_us = ppmRisingTimeStampCurr_us - ppmRisingTimeStampPrev_us;

    ppmChannelPeakDeltas_us[ppmCurrentChannelIndex] = ppmPeakDelta_us;
    ppmCurrentChannelIndex++;

    if((ppmCurrentChannelIndex > PPM_NUM_CHANNELS) || (ppmPeakDelta_us > PPM_DETECTION_SPACE))
    {
       ppmCurrentChannelIndex = 0;
    }

    ppmRisingTimeStampPrev_us = ppmRisingTimeStampCurr_us;
}
