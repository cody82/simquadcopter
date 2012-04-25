//same functionality as FlightControlTask.c

//#include "CopterHardwareConfig.h"
//#include "Axis.h"
#include "flightcontrol.h"

using namespace SimQuadCopter;

//#define CONTROLTASK_PERIOD_SEC 0.018

FlightControl flightcontrol;

unsigned char data[16];

void flightcontrol_init()
{
	flightcontrol.init();
	for(int i=0;i<sizeof(data);++i)data[i]=0;
}

// this code is normaly called in a loop in the original I4Copter code. thats not possible on the simulator.
void flightcontrol_update(const SimQuadCopter::Control &control, SimQuadCopter::QuadCopter &copter)
{
        //while(1){
                //receive Data
                //i4cos_msg_recv( &port, &data, (size_t)16, O_NONBLOCK );

		float x,z;
		copter.calcAnglesFromAcceleration(x,z);

		//flightcontrol.setSetpoint(control.throttle * 12.0f,control.roll,control.pitch);
 		//flightcontrol.control(  -x, -copter.gyroX.getValue() ,z, copter.gyroZ.getValue());

		flightcontrol.setSetpoint(control.throttle * 12.0f,control.roll,control.pitch);
 		flightcontrol.control(  z, copter.gyroZ.getValue() ,x, copter.gyroX.getValue());

                //waiting for next execution-cycle
                //i4cos_msleep(CONTROLTASK_PERIOD_SEC*1000);// *1000 because of millisecond
  	//}//while

}


