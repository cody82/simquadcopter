#include "udpremote.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

namespace SimQuadCopter
{

void UdpCopter::init()
{
	// initialize SDL_net
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		exit(2);
	}

	// open udp server socket
	if(!(sock=SDLNet_UDP_Open(4000)))
	{
		printf("SDLNet_UDP_Open: %s\n",SDLNet_GetError());
		exit(4);
	}

	// allocate max packet
	if(!(out=SDLNet_AllocPacket(65535)))
	{
		printf("SDLNet_AllocPacket: %s\n",SDLNet_GetError());
		exit(5);
	}
	if(!(in=SDLNet_AllocPacket(65535)))
	{
		printf("SDLNet_AllocPacket: %s\n",SDLNet_GetError());
		exit(5);
	}

	if(SDLNet_ResolveHost(&ip,"192.168.0.100",4000)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		exit(4);
	}

	if(SDLNet_UDP_Bind(sock, 0, &ip)==-1)
	{
		printf("SDLNet_UDP_Bind: %s\n",SDLNet_GetError());
		exit(7);
	}

}

UdpCopter::UdpCopter(QuadCopter *copter)
{
	this->copter=copter;
	time=0;
}

void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void UdpCopter::update(float dtime)
{
	while(SDLNet_UDP_Recv(sock, in))
	{
		in->data[in->len]=0;
		//printf("recv: %s\n",in->data);

		vector<string> tokens;
		vector<string>::const_iterator i;

		string str((char*)in->data);

		Tokenize(str, tokens, "\n");

		for(i=tokens.begin(); i!=tokens.end(); ++i)
		{
			vector<string> tokens2;
			Tokenize(*i,tokens2);
			
			if(tokens2[0]=="throttle")
			{
				copter->control.throttle=atof(tokens2[1].c_str())*0.01f;
			}
			if(tokens2[0]=="yaw")
			{
				copter->control.yaw=atof(tokens2[1].c_str())*0.01f-0.5f;
			}
			if(tokens2[0]=="pitch")
			{
				copter->control.pitch=(atof(tokens2[1].c_str())*0.01f-0.5f);//*5.0f;
			}
			if(tokens2[0]=="roll")
			{
				copter->control.roll=atof(tokens2[1].c_str())*0.01f-0.5f;
			}
		}
	}

	time+=dtime;
	if(time>=0.02f)
	{
		time=0.0f;

		string s;
		stringstream ss;


		ss<<"gyroX "<<copter->gyroX.getValue()<<"\n";
		ss<<"gyroY "<<copter->gyroY.getValue()<<"\n";
		ss<<"gyroZ "<<copter->gyroZ.getValue()<<"\n";

		ss<<"gyroIntX "<<copter->gyroIntX<<"\n";
		ss<<"gyroIntY "<<copter->gyroIntY<<"\n";
		ss<<"gyroIntZ "<<copter->gyroIntZ<<"\n";

		ss<<"accelX "<<copter->accelX.getValue()<<"\n";
		ss<<"accelY "<<copter->accelY.getValue()<<"\n";
		ss<<"accelZ "<<copter->accelZ.getValue()<<"\n";

		ss<<"posX "<<copter->physics->getPosition().getX()<<"\n";
		ss<<"posY "<<copter->physics->getPosition().getY()<<"\n";
		ss<<"posZ "<<copter->physics->getPosition().getZ()<<"\n";
		
		ss<<"speedX "<<copter->physics->getSpeedVector().getX()<<"\n";
		ss<<"speedY "<<copter->physics->getSpeedVector().getY()<<"\n";
		ss<<"speedZ "<<copter->physics->getSpeedVector().getZ()<<"\n";

		ss<<"throttleXm "<<copter->physics->engineXm.getThrottle()<<"\n";
		ss<<"throttleXp "<<copter->physics->engineXp.getThrottle()<<"\n";
		ss<<"throttleZm "<<copter->physics->engineZm.getThrottle()<<"\n";
		ss<<"throttleZp "<<copter->physics->engineZp.getThrottle()<<"\n";

		ss<<"rpmXm "<<copter->physics->engineXm.getRPM()<<"\n";
		ss<<"rpmXp "<<copter->physics->engineXp.getRPM()<<"\n";
		ss<<"rpmZm "<<copter->physics->engineZm.getRPM()<<"\n";
		ss<<"rpmZp "<<copter->physics->engineZp.getRPM()<<"\n";

		ss<<"pitch "<<copter->control.pitch<<"\n";
		ss<<"yaw "<<copter->control.yaw<<"\n";
		ss<<"roll "<<copter->control.roll<<"\n";
		
		float x,z,rx,rz;
		//this is what the flightcontrol thinks (it is dependend on the acceleration of the copter)
		copter->calcAnglesFromAcceleration(x,z);
		//this is the actual angle
		copter->physics->calcRealAngles(rx,rz);
		ss<<"angleX "<<x<<"\n";
		ss<<"angleZ "<<z<<"\n";
 		ss<<"angleXreal "<<rx<<"\n";
		ss<<"angleZreal "<<rz<<"\n";


		ss<<"time "<<SDL_GetTicks()<<"\n";

		//ss<<"angleX "<<copter->physics->getAxisAngle(Vector3(0,0,1))<<"\n";
		//ss<<"angleZ "<<copter->physics->getAxisAngle(Vector3(1,0,0))<<"\n";

		s=ss.str();
		out->len=s.length();
		memcpy(out->data,s.c_str(),out->len);
		SDLNet_UDP_Send(sock, 0, out);
	}
}
/*
	// close the socket
	SDLNet_UDP_Close(sock);
	
	// free packet
	SDLNet_FreePacket(out);
	SDLNet_FreePacket(in);
	SDLNet_FreePacketV(packets);
	
	// shutdown SDL_net
	SDLNet_Quit();

	// shutdown SDL
	SDL_Quit();
*/

}
