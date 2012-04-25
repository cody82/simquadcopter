#include "SimActuator.h"

#include <iostream>
using namespace std;

namespace SimQuadCopter
{

SimActuator::SimActuator()
{
	engine = NULL;
}

void SimActuator::init(OdeEngine *engine)
{
	this->engine=engine;
}


void SimActuator::setVoltage(float voltage)
{
	if(engine==NULL)
	{
		cout << "SimActuator not initialized." << endl;
	}
	float throttle =  voltage / 12.0f;

	engine->setThrottle(throttle);
}

}