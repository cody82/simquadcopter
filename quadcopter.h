#ifndef QUADCOPTER_H
#define QUADCOPTER_H

#include <ode/ode.h>
#include "vectormath/vectormath_aos.h"
#include "vectormath/mat_aos.h"
#include "vectormath/vec_aos.h"
#include "vectormath/quat_aos.h"
#include <GL/gl.h>

#include "udpremote.h"

//old balancer
#include "balance.h"

//I4Copter flightcontrol
//#include "flightcontrol.h"

using namespace Vectormath::Aos;

namespace SimQuadCopter
{

class QuadCopter;
class OpenGL1;
class OdeCopter;
class UdpCopter;

class Object
{
public:
	Object(const Vector3& position,const Quat& orientation);
	//virtual Vector3& position();
	//virtual Quat& orientation;
};

class Sensor
{
public:
	Sensor();

	void setValue(float v);
	float getValue() const;

	float noise;
protected:
	float value;
};

class OdeEngine
{
public:
	OdeEngine(const Vector3& position);
	void init(OdeCopter *copter,Vector3 position, float dir);
	
	void setThrottle(float throttle);
	void setRPM(float rpm);
	float getThrottle() const;
	
	float currentForce() const;
	float getTorque() const;
	void update(float dtime);
	float getRPM() const;

	dBodyID motor;
	dBodyID propeller;
	dJointID hinge;
	dJointID fixed;//connects the engine to the quadcopter body
	float direction;
	Vector3 position;

	//float maxRPM;
	/*float errorRPM;
	float timerRPM;
	float currentErrorRPM;
*/
	static bool simulatePropellerRotation;
	static bool simulatePropellerAirFriction;


protected:
	float calcRPM(float throttle) const;

	float throttle;
	float maxForce;
	//float wantedThrottle;
	float pwmThrottle;
	float pwmTimer;
	float currentRPM;
	float acceleration;

	float maxRPM;
	float errorRPM;
	float timerRPM;
	float currentErrorRPM;
};

class OdeCopter
{
public:
	OdeCopter(QuadCopter *copter, float size);
	void update(float dtime);

	Vector3 getPosition();
	Quat getOrientation();

	void setPosition(Vector3 v);

	float getTotalThrust() const;

	void calcRealAngles(float &x,float &z) const;
	
	float getSpeed() const;
	Vector3 getSpeedVector() const;

	void addAirFrictionForce();

	dBodyID body;//frame body
	dMass mass;//frame mass
	dBodyID battery;
	dBodyID boards;

	float currentAirFriction;

	QuadCopter *copter;
	
	OdeEngine engineXp;
	OdeEngine engineXm;
	OdeEngine engineZp;
	OdeEngine engineZm;

	// this joint mounts the copter to the static environment for testing
	dJointID mountJoint;

	static void init();
	static void close();
	static dWorldID world;
	static dSpaceID space;
	static dJointGroupID contactgroup;
protected:
	void mountUniversal();
	void mountBall();
	void mountHingeX();
	void mountHingeZ();

	void addEngineForce(const OdeEngine& engine);
	static void nearCallback (void *data, dGeomID o1, dGeomID o2);
};

class Control
{
public:
	Control();
	float throttle;
	float yaw;
	float pitch;
	float roll;
};

class QuadCopter
{
public:
	QuadCopter(float size);

	Control control;

	void update(float dtime);

	void calcAnglesFromAcceleration(float &x, float &z);

	Sensor gyroX;
	Sensor gyroY;
	Sensor gyroZ;
	float gyroIntX;
	float gyroIntY;
	float gyroIntZ;

	Sensor accelX;
	Sensor accelY;
	Sensor accelZ;


	OdeCopter *physics;
	UdpCopter *remote;

	Balance balanceX;
	Balance balanceZ;
	BalanceHeight balanceY;

	float size;
	float flightControlTimer;
};

class OpenGL1
{
public:
	static void draw(const QuadCopter& copter);
	static void draw(OdeEngine *e);
	static void drawFloor();

	static void getMatrix(dReal *matrix,const dReal* Position1,const dReal* Rotation1);
	static void getMatrix(dReal *matrix,dBodyID body);

};



}

#endif
