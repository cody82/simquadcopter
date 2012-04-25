#ifndef FLIGHTCONTROL_H
#define FLIGHTCONTROL_H

#include "FlightControl/FlightControl.h"
#include "quadcopter.h"

extern FlightControl flightcontrol;
void flightcontrol_init();
void flightcontrol_update(const SimQuadCopter::Control &control, SimQuadCopter::QuadCopter &copter);


#endif
