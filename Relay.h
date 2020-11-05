#pragma once

class Relay
{
public:
	Relay(int pinId);
	~Relay();

	void setEnabled(bool enable);

private:

	void writeEnableToBoard();
	void writeDisableToBoard();

	int m_pinId = 0;
};

