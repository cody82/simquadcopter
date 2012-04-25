#ifndef __UDPREMOTE_H
#define __UDPREMOTE_H

#include "quadcopter.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_net.h"

namespace SimQuadCopter
{
class QuadCopter;
class UdpCopter
{
public:
	UdpCopter(QuadCopter *copter);
	
	void init();
	void update(float dtime);

protected:
	UDPsocket sock;
	UDPpacket *out, *in;
	QuadCopter *copter;
	float time;
	IPaddress ip;

};
}

#endif
