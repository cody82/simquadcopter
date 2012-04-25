#ifndef SIMACTUATOR_H
#define SIMACTUATOR_H

#include "../quadcopter.h"

namespace SimQuadCopter
{

class SimActuator
{
private:
	OdeEngine *engine;
	
public:
	SimActuator();
	void init(OdeEngine *engine);

	void setVoltage(float voltage);

};

}

#endif

