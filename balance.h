#ifndef __BALANCE_H
#define __BALANCE_H


namespace SimQuadCopter
{

class BalanceHeight
{
public:
	BalanceHeight();
	float update(float dtime, float sensorvalue, float want);

	float Kp;
	float Ki;
	float Kd;

protected:
	float e_int;
	float e_prev;
};

class Balance
{
public:
	Balance();
	float update(float dtime, float sensorvalue, float want);

	float Kp;
	float Ki;
	float Kd;

	float value;
protected:
	float e_int;
	float e_prev;
};

}


#endif
