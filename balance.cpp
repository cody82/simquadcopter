#include "balance.h"



namespace SimQuadCopter
{

BalanceHeight::BalanceHeight()
{
	e_int=0;
	e_prev=0;

	Kp=0.1f;
	Ki=0.03f;
	Kd=0.3f;
}	

float BalanceHeight::update(float dtime, float sensorvalue, float want)
{
	float e = want - sensorvalue;
	
	//P
	float p = Kp * e;

	//I
	e_int += e * dtime;
	float i = Ki * e_int;

	//D
	float diff = e-e_prev;
	float d = Kd * diff / dtime;

	float y = p + i + d;

	e_prev=e;
	return y+0.4f;
}

Balance::Balance()
{
	e_int=0;
	e_prev=0;

	Kp=0.3f;
	Ki=0.0f;
	Kd=0.0f;

	value=0.0f;
}	

float Balance::update(float dtime, float sensorvalue, float want)
{
	float e = want - sensorvalue;
	
	//P
	float p = Kp * e;

	//I
	e_int += e * dtime;
	float i = Ki * e_int;

	//D
	float diff = e-e_prev;
	float d = Kd * diff / dtime;

	float y = p + i + d;

	e_prev=e;
	value=y;
	return y;
}
}
