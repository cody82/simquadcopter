#include "quadcopter.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include "flightcontrol.h"
#include "hardware/CopterHardwareConfig.h"

namespace SimQuadCopter
{

bool OdeEngine::simulatePropellerRotation=true;
bool OdeEngine::simulatePropellerAirFriction=true;

dWorldID OdeCopter::world=0;
dSpaceID OdeCopter::space=0;
dJointGroupID OdeCopter::contactgroup=0;

Control::Control()
{
	throttle=yaw=pitch=roll=0;
}

void OdeCopter::init()
{
	if(world==0)
	{
		world=dWorldCreate();
		dWorldSetGravity(world,0,-9.81f,0);

		space=dSimpleSpaceCreate(0);

		contactgroup=dJointGroupCreate(10);

		dWorldSetERP(world,0.5);

		std::cout << "ODE float size: " << sizeof(dReal) << std::endl;
	}
}

void OdeCopter::close()
{
	dJointGroupDestroy(contactgroup);
	dWorldDestroy(world);
	dSpaceDestroy(space);

	dCloseODE();
}

OdeCopter::OdeCopter(QuadCopter *copter,float size):
	engineXp(Vector3(size*0.5f,0,0)),
	engineXm(Vector3(-size*0.5f,0,0)),
	engineZp(Vector3(0,0,size*0.5f)),
	engineZm(Vector3(0,0,-size*0.5f))
{
	init();
	this->copter=copter;
	
	dMass mass2;

	//mass of the copter, not including motors and propellers!
	//mass of frame (300g)
	body=dBodyCreate(world);
	dMassSetBoxTotal(&mass,0.15f,copter->size,copter->size*0.1f,copter->size*0.1f);
	dMassSetBoxTotal(&mass2,0.15f,copter->size*0.1f,copter->size*0.1f,copter->size);
	dMassAdd(&mass,&mass2);
	dBodySetMass(body,&mass);
	
	//mass of boards (300g)
	boards=dBodyCreate(world);
	dMassSetBoxTotal(&mass2,0.3f,0.08f,0.052f,0.11f);//units in meters
	dBodySetMass(boards,&mass2);
	dBodySetPosition(boards,0,0.035f,0);//center of boards package is 3.5cm over frame center

	//mass of battery (300g)
	battery=dBodyCreate(world);
	dMassSetBoxTotal(&mass2,0.3f,0.142f,0.0234f,0.0425f);//units in meters
	dBodySetMass(battery,&mass2);
	dBodySetPosition(boards,0,-0.03f,0);//center of battery is 3.0cm under frame center

	//attach battery and boards to the frame with fixed joints
	dJointID fixed;
	fixed=dJointCreateFixed(world,0);
	dJointAttach(fixed,battery,body);
	dJointSetFixed(fixed);
	fixed=dJointCreateFixed(world,0);
	dJointAttach(fixed,boards,body);
	dJointSetFixed(fixed);


	dGeomID geom;
	//geom=dCreateBox(space,copter->size,copter->size*0.05f,copter->size*0.05f);
	geom=dCreateBox(space,copter->size,0.082f,0.04f);
	dGeomSetBody(geom,body);
	//geom=dCreateBox(space,copter->size*0.05f,copter->size*0.05f,copter->size);
	geom=dCreateBox(space,0.04f,0.082f,copter->size);
	dGeomSetBody(geom,body);
	
	engineXp.init(this,Vector3(size*0.5f,0,0),1);
	engineXm.init(this,Vector3(-size*0.5f,0,0),1);
	engineZp.init(this,Vector3(0,0,size*0.5f),-1);
	engineZm.init(this,Vector3(0,0,-size*0.5f),-1);

	mountJoint=NULL;

	//select mount type
	switch(0)
	{
	case 1:
		mountUniversal();
		break;
	case 2:
		mountBall();
		break;
	case 3:
		mountHingeX();
		break;
	case 4:
		mountHingeZ();
		break;
	case 0:
		break;
	}

	if(mountJoint==NULL)
	{
		dBodySetPosition(body,0,1,0);
		dGeomID plane=dCreatePlane(space,0,1,0,0);
	}

	currentAirFriction=0;
}

float OdeCopter::getSpeed() const
{
	const dReal *v = dBodyGetLinearVel(body);
	Vector3 v2(v[0],v[1],v[2]);
	return length(v2);
}

Vector3 OdeCopter::getSpeedVector() const
{
	const dReal *v = dBodyGetLinearVel(body);
	Vector3 v2(v[0],v[1],v[2]);
	return v2;
}

void OdeCopter::mountUniversal()
{
	mountJoint=dJointCreateUniversal(OdeCopter::world,0);
	dJointAttach(mountJoint,body,NULL);
	dJointSetUniversalAnchor (mountJoint, 0, 0, 0);
	dJointSetUniversalAxis1 (mountJoint, 1, 0, 0);
	dJointSetUniversalAxis2 (mountJoint, 0, 0, 1);
}

void OdeCopter::mountBall()
{
	mountJoint=dJointCreateBall(OdeCopter::world,0);
	dJointAttach(mountJoint,body,NULL);
	dJointSetBallAnchor(mountJoint,0,0,0);
}

void OdeCopter::mountHingeX()
{
	mountJoint=dJointCreateHinge(OdeCopter::world,0);
	dJointAttach(mountJoint,body,NULL);
	dJointSetHingeAnchor(mountJoint,0,0,0);
	dJointSetHingeAxis(mountJoint,1,0,0);
}

void OdeCopter::mountHingeZ()
{
	mountJoint=dJointCreateHinge(OdeCopter::world,0);
	dJointAttach(mountJoint,body,NULL);
	dJointSetHingeAnchor(mountJoint,0,0,0);
	dJointSetHingeAxis(mountJoint,0,0,1);
}


void OdeCopter::addEngineForce(const OdeEngine& engine)
{
	dBodyAddRelForceAtRelPos(body,0,engine.currentForce(),0,engine.position.getX(),engine.position.getY(),engine.position.getZ());
}

void OdeCopter::nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	if (dGeomIsSpace (o1) || dGeomIsSpace (o2))
	{
		// colliding a space with something
		dSpaceCollide2 (o1,o2,data,&nearCallback);
		// collide all geoms internal to the space(s)
		//if (dGeomIsSpace (o1)) dSpaceCollide (o1,data,&nearCallback);
		//if (dGeomIsSpace (o2)) dSpaceCollide (o2,data,&nearCallback);
	}
	else
	{
		// colliding two non-space geoms, so generate contact
		// points between o1 and o2
		dContact contact[10];
		int num_contact = dCollide (o1,o2,10,&contact[0].geom,sizeof(dContact));
		// add these contact points to the simulation
		if(num_contact>0)
		{
			dBodyID b1=dGeomGetBody(o1);
			dBodyID b2=dGeomGetBody(o2);
			if(b1!=b2 && !dAreConnectedExcluding (b1,b2,dJointTypeContact))
			{

				for (int i=0; i<num_contact; i++)
				{
					contact[i].surface.mode = dContactBounce;
					contact[i].surface.mu = 0.8; // ???
					contact[i].surface.bounce = 0.05; // ???
					dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
					dJointAttach (c, dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));
				}
			}
		}
    	}
}

void OdeCopter::setPosition(Vector3 v)
{
	dBodySetPosition(body,v.getX(),v.getY(),v.getZ());
}

void OdeCopter::update(float dtime)
{
	static Vector3 last_speed(0,0,0);

	dSpaceCollide(space,NULL,&nearCallback);

	dWorldStep(world,dtime);
	dJointGroupEmpty(contactgroup);

	addEngineForce(engineXp);
	addEngineForce(engineXm);
	addEngineForce(engineZp);
	addEngineForce(engineZm);

	addAirFrictionForce();

	engineXm.update(dtime);
	engineXp.update(dtime);
	engineZp.update(dtime);
	engineZm.update(dtime);
	
	dVector3 dv;

	const dReal *v=dBodyGetAngularVel(body);
	dBodyVectorFromWorld(body,v[0],v[1],v[2],dv);
	//minus sign: positive rotation in I4Copter is counter-clockwise when looking in positive direction of the axis.
	copter->gyroX.setValue(-dv[0]);
	copter->gyroY.setValue(-dv[1]);
	copter->gyroZ.setValue(-dv[2]);

	v=dBodyGetLinearVel(body);
	dBodyVectorFromWorld(body,v[0],v[1],v[2],dv);

	Vector3 speed(dv[0],dv[1],dv[2]);
	Vector3 accel=(speed-last_speed)/dtime;

	// gravity
	dBodyVectorFromWorld(body,0,9.81f,0,dv);
	accel+=Vector3(dv[0],dv[1],dv[2]);


	copter->accelX.setValue(accel.getX());
	copter->accelY.setValue(accel.getY());
	copter->accelZ.setValue(accel.getZ());

	last_speed=speed;
}

void OdeCopter::addAirFrictionForce()
{
	double speed=getSpeed();

	const float cw=0.8f;//like a truck
	const float area=0.5f*0.2f;//just a guess
	const float density=1.2f;//air at 20 degrees celsius is 1.2kg/m^3

	float force=0.5*density*cw*area*speed*speed;
	currentAirFriction=force;

	//calculate force vector
	const dReal *v = dBodyGetLinearVel(body);
	Vector3 v2(v[0],v[1],v[2]);
	v2=-normalize(v2)*force;
	
	dBodyAddForce(body,v2.getX(),v2.getY(),v2.getZ());
}

void OdeCopter::calcRealAngles(float &x,float &z) const
{
	//this is like QuadCopter::calcAnglesFromAcceleration
	dVector3 dv;
	dBodyVectorToWorld(body,0,1,0,dv);
	
	float ax=-dv[0],ay=dv[1],az=-dv[2];
	
	//rotation about x axis
	float gx=atan2(az,ay);
	
	//rotation about z axis
	float gz=-atan2(ax,ay);

	x=gx;
	z=gz;
}

Vector3 OdeCopter::getPosition()
{
	const dReal *v=dBodyGetPosition(body);
	return Vector3(v[0],v[1],v[2]);
}

Quat OdeCopter::getOrientation()
{
	const dReal *v=dBodyGetQuaternion(body);
	return Quat(v[3],v[0],v[1],v[2]);
}

float OdeCopter::getTotalThrust() const
{
	return engineXp.currentForce()+
		engineXm.currentForce()+
		engineZp.currentForce()+
		engineZm.currentForce();
}

QuadCopter::QuadCopter(float size)
{
	this->size=size;
	physics=new OdeCopter(this,size);
	remote=new UdpCopter(this);
	gyroIntX = gyroIntY = gyroIntZ = 0.0f;

	//hardware and flightcontrol init
	actuatorForward.init(&physics->engineZp);
	actuatorBackward.init(&physics->engineZm);
	actuatorLeft.init(&physics->engineXp);
	actuatorRight.init(&physics->engineXm);

	flightControlTimer=0;
	flightcontrol_init();
}

void QuadCopter::calcAnglesFromAcceleration(float &x, float &z)
{
	float ax=accelX.getValue(),ay=accelY.getValue(),az=accelZ.getValue();
	
	//rotation about x axis
	float gx=atan2(az,ay);
	
	//rotation about z axis
	float gz=-atan2(ax,ay);

	x=gx;
	z=gz;
}

void QuadCopter::update(float dtime)
{
	gyroIntX += gyroX.getValue() * dtime;
	gyroIntY += gyroY.getValue() * dtime;
	gyroIntZ += gyroZ.getValue() * dtime;

	flightControlTimer+=dtime;
	if(flightControlTimer>=0.022f)
	{
		flightControlTimer=fmod(flightControlTimer,0.022f);
	//select here
	switch(2)
	{
	case 1:
	{
		//old code
		float throttle=control.throttle;// balanceY.update(dtime, physics->getPosition().getY(), control.throttle*10.0f);

		float roll=balanceZ.update(dtime, gyroZ.getValue(), control.roll);
		float pitch=balanceX.update(dtime, gyroX.getValue(), control.pitch);

		physics->engineXp.setThrottle(throttle+roll+control.yaw);
		physics->engineXm.setThrottle(throttle-roll+control.yaw);
	
		physics->engineZp.setThrottle(throttle-pitch-control.yaw);
		physics->engineZm.setThrottle(throttle+pitch-control.yaw);
		break;
	}
	case 2:
		//I4Copter flightcontrol
		flightcontrol_update(control,*this);
		break;
	case 3:
		//no balancer, direct control
		physics->engineXp.setThrottle(control.throttle+control.roll+control.yaw);
		physics->engineXm.setThrottle(control.throttle-control.roll+control.yaw);
	
		physics->engineZp.setThrottle(control.throttle-control.pitch-control.yaw);
		physics->engineZm.setThrottle(control.throttle+control.pitch-control.yaw);
	}
	}

	physics->update(dtime);
	remote->update(dtime);
}

Sensor::Sensor()
{
	value=0;
	noise=0;//0.1f;
}

void Sensor::setValue(float v)
{
	value=v;
}

float Sensor::getValue() const
{
	return value + (((float)rand())/((float)RAND_MAX)-0.5f) * noise;
}

OdeEngine::OdeEngine(const Vector3& position)
{
	throttle=0;
	this->position=position;
	pwmThrottle=0;
	pwmTimer=0;
	acceleration=0;

	maxRPM=5000 + (((float)rand())/((float)RAND_MAX)-0.5f) * 100;
	errorRPM=0;
	timerRPM=0;
	currentErrorRPM=0;
}

float OdeEngine::calcRPM(float throttle) const
{
	//TODO: fitting function?
	return std::max(0.0f,throttle * maxRPM + currentErrorRPM);
}

float OdeEngine::getTorque() const
{
	return getRPM()/6000.0f*0.15f;
}
float OdeEngine::getThrottle() const
{
	return throttle;
}

void OdeEngine::setThrottle(float throttle)
{
	if(throttle>1.0f)
		this->throttle=1.0f;
	else if(throttle<0.0f)
		this->throttle=0.0f;
	else
		//motor controller has 256 steps(?)
		this->throttle=((float)((int)(throttle*255.0f)))/255.0f;
		
}

float OdeEngine::getRPM() const
{
	return currentRPM;
	//return dJointGetHingeAngleRate(hinge) * 60.0f / 2.0f / 3.14f * direction;
}

void OdeEngine::update(float dtime)
{
	pwmTimer+=dtime;
	if(pwmTimer>=(0.022f + 0.0f))//pwm signal length is 22ms
	{
		pwmThrottle=throttle;
		pwmTimer=0.0f;
	}

	timerRPM+=dtime;
	if(timerRPM>=1.0f)
	{
		currentErrorRPM=(((float)rand())/((float)RAND_MAX)-0.5f) * errorRPM;
		timerRPM=0.0f;
	}

	float rpm=calcRPM(throttle);//pwmThrottle * maxRPM + currentErrorRPM;
	float current=getRPM();
	
	//calculate propeller speed (trial and error, i dont know how that works)
	float diff=rpm-current;
	float maxspeed=4000.0f*dtime;
	float speed=std::min(maxspeed,diff*2.0f*dtime);
	float minspeed=30.0f*dtime;
	float result;
	if(diff>0.0f)
		result=std::min(speed+minspeed,diff);
	else
		result=std::max(speed-minspeed,diff);
	diff=result-acceleration;
	acceleration+=diff*dtime*2.0f;
	if(acceleration>0.0f)
		acceleration=std::min(acceleration,maxspeed);
	else
		acceleration=std::max(acceleration,-maxspeed);
	
	if((current<rpm && current+acceleration> rpm)||(current>rpm && current+acceleration< rpm))
	{
		current=rpm;
		acceleration=0;
	}
	else
		current+=acceleration;


	setRPM(current);

	if(simulatePropellerAirFriction)
		dBodyAddRelTorque(motor,0,direction*getTorque(),0);

	//needed for high speed rotation
	dVector3 v;
	dJointGetHingeAxis(hinge,v);
	dBodySetFiniteRotationAxis(propeller,v[0],v[1],v[2]);
}

float OdeEngine::currentForce() const
{
	//return throttle*5.0f;
	//float force = std::max(0.0f,(9.9f * (throttle*100.0f) - 88.0f) * 0.01f);
	float x=getRPM();
	
	//fitting functions for propellers
	float epp=sqrt(34795*34795+x*x)-34816;
	float apc=sqrt(15366*15366+x*x)-15420;
	
	float force=std::max(0.0f,apc*0.01f);
	return force;
}

void OdeEngine::init(OdeCopter *copter,Vector3 position, float dir)
{
	dMass mass;
	
	direction=dir;
	
	// motor
	motor=dBodyCreate(OdeCopter::world);
	dMassSetCylinderTotal(&mass,0.070f,2,0.015f,0.04f);
	dBodySetMass(motor,&mass);
	dBodySetPosition(motor,position.getX(),position.getY(),position.getZ());
	
	//motor=copter->body;
	
	// propeller
	propeller=dBodyCreate(OdeCopter::world);
	dMassSetBoxTotal(&mass,0.020f,0.02f,0.005f,0.2f);
	dBodySetMass(propeller,&mass);
	dBodySetPosition(propeller,position.getX(),position.getY()+0.02f,position.getZ());
	//this is needed for high speed rotations
	dBodySetFiniteRotationMode(propeller,1);

	// connection propeller <> motor
	hinge=dJointCreateHinge(OdeCopter::world,0);
	dJointAttach(hinge,propeller,motor);
	dJointSetHingeAnchor(hinge,position.getX(),position.getY()+0.01f,position.getZ());
	dJointSetHingeAxis(hinge,0,1,0);
	
	//dJointSetHingeParam(hinge,dParamVel,3.14);
	dJointSetHingeParam(hinge,dParamFMax,0.1f);
	
	if(copter!=NULL)
	{
		fixed=dJointCreateFixed(OdeCopter::world,0);
		dJointAttach(fixed,motor,copter->body);
		dJointSetFixed(fixed);
	}
	else
		fixed=NULL;

	//dJointSetHingeParam(hinge,dParamVel,2000*direction);
	
}

void OdeEngine::setRPM(float rpm)
{
	if(rpm<0.0f)
		rpm=0.0f;
	
	currentRPM=rpm;
	
	if(simulatePropellerRotation)
		dJointSetHingeParam(hinge,dParamVel,rpm / 60.0f * 2.0f * 3.14f * direction);
	else
		dJointSetHingeParam(hinge,dParamVel,0);
	

}

void OpenGL1::getMatrix(dReal *matrix,const dReal* Position1,const dReal* Rotation1)
{
	matrix[0]=Rotation1[0];
	matrix[1]=Rotation1[4];
	matrix[2]=Rotation1[8];
	matrix[3]=0;
	matrix[4]=Rotation1[1];
	matrix[5]=Rotation1[5];
	matrix[6]=Rotation1[9];
	matrix[7]=0;
	matrix[8]=Rotation1[2];
	matrix[9]=Rotation1[6];
	matrix[10]=Rotation1[10];
	matrix[11]=0;
	matrix[12]=Position1[0];
	matrix[13]=Position1[1];
	matrix[14]=Position1[2];
	matrix[15]=1;
}

void OpenGL1::getMatrix(dReal *matrix,dBodyID body)
{
	const dReal* Position1 = dBodyGetPosition(body);
	const dReal* Rotation1 = dBodyGetRotation(body);
	getMatrix(matrix,Position1,Rotation1);
}

void OpenGL1::draw(const QuadCopter& copter)
{
	//Vector3 pos=copter.physics->getPosition();
	//glTranslatef(pos.getX(),pos.getY(),pos.getZ());

	//Matrix4 m(copter.physics->getOrientation(),copter.physics->getPosition());
	//m=transpose(m);
	
	GLfloat matrix[16];
	getMatrix(matrix,copter.physics->body);
	glPushMatrix();
	glMultMatrixf (matrix);

	//glMultMatrixf((float*)&m);

	float s=copter.size*0.5f;

	glColor4f(1,1,1,1);
	glBegin(GL_LINES);
	glVertex3f(s,0,0);
	glVertex3f(-s,0,0);
	glVertex3f(0,0,s);
	glVertex3f(0,0,-s);
	glEnd();

	glColor4f(1,0,0,1);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,copter.gyroX.getValue());
	glEnd();

	glColor4f(0,1,0,1);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(copter.gyroZ.getValue(),0,0);
	glEnd();
	glPopMatrix();
	

	draw(&copter.physics->engineXp);
	draw(&copter.physics->engineXm);
	draw(&copter.physics->engineZp);
	draw(&copter.physics->engineZm);
	
}

void OpenGL1::draw(OdeEngine *e)
{
	GLfloat matrix[16];
	
	getMatrix(matrix,e->propeller);
	glPushMatrix();
	glMultMatrixf (matrix);
	glColor4f(1,1,1,1);
	glBegin(GL_LINES);
	glVertex3f(0,0,-0.1f);
	glVertex3f(0,0,0.1f);
	glEnd();
	glPopMatrix();
}

void OpenGL1::drawFloor()
{
	int x,z;
	
	glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_LINES);
	for(x=-10;x<=10;++x)
	{
		glVertex3f(x,0,-10);
		glVertex3f(x,0,10);
		
	}

	for(z=-10;z<=10;++z)
	{
		glVertex3f(-10,0,z);
		glVertex3f(10,0,z);
	}

	glEnd();
}

}
