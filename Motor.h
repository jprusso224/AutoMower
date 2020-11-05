#pragma once
class Motor
{
public:
	Motor(int directionPinId, int speedPinId);
	~Motor();

	int getDirectionPinId() { return m_directionPinId; }
	int getSpeedPinId();

	void writeSpeedToBoard(int speed_pwm);
	void writeDirectionToBoard(int direction);

private:
	int m_directionPinId = 0;
	int m_speedPinId = 0;
};

