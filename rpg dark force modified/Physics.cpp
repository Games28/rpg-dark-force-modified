#include "Physics.h"
#include "ObjectManager.h"

Physics::Physics()
{
	//gravity = 9.8f;
    //pixels_per_meter = 50;
    //accelerate = gravity * pixels_per_meter;
}

void Physics::integrate(float& deltatime)
{
	vel += accelerate * deltatime;
	//pos += vel * deltatime;
}

void Physics::physicsconstants()
{
	//gravity = 9.8f;
	//pixels_per_meter = 50;
	//accelerate = gravity * pixels_per_meter;
}

float Physics::physicobjectlift(float& deltatime)
{
	
	

	Vertintegrate(deltatime);

	

	if (Vertpos >= 0)
	{
		Vertpos = 0;

		VertVel *= -0.2f;
	}

	return Vertpos;
}

void Physics::Vertphysicssetup(float lift, float mass)
{
	Vertpos = lift;
	VertMass = mass;
	if (VertMass != 0.0f)
	{
		VertinvMass = 1.0f / VertMass;
	}
	else
	{
		VertinvMass = 0.0f;
	}
}

void Physics::Vertintegrate(float& deltatime)
{
	VertAccelerate = VertsumForces * VertinvMass;

	VertVel += VertAccelerate * deltatime;
	Vertpos += VertVel * deltatime;
	
	VertClearForces();
}

float Physics::VertIntegrate(float& deltatime)
{
	VertAccelerate = VertsumForces * VertinvMass;

	VertVel += VertAccelerate * deltatime;
	Vertpos += VertVel * deltatime;

	
	VertClearForces();
	return Vertpos;
}

float Physics::VertIntegr(float& deltatime)
{
	VertAccelerate = VertsumForces * VertinvMass;

	VertVel += VertAccelerate * deltatime;
	VertClearForces();
	return VertVel;
}



void Physics::VertClearForces()
{
	VertsumForces = 0;
}

void Physics::AddVertForce(const float& force)
{
	VertsumForces += force;
}

void Physics::Horzphysicssetup( float mass)
{
	

	HorzMass = mass;
	if (HorzMass != 0.0f)
	{
		HorzInvMass = 1.0f / HorzMass;
	}
	else
	{
		HorzInvMass = 0.0f;
	}
}

void Physics::Horzintegrate(float& deltatime)
{
	HorzAccelerate = HorzSumforces * HorzInvMass;

	HorzVel += HorzAccelerate * deltatime;

	Horzpos += HorzVel * deltatime;
	HorzClearForces();
}

Vec2 Physics::HorzIntegrate(float &rotationangle,float& deltatime)
{
	Vec2 vel = Vec2(0,0);
	HorzAccelerate = HorzSumforces * HorzInvMass;

	HorzVel += HorzAccelerate * deltatime;

	vel = HorzVel  * deltatime;



	HorzClearForces();

	return vel;
}

void Physics::AddHorzForces(const Vec2& force)
{
	HorzSumforces += force;
}

void Physics::HorzClearForces()
{
	HorzSumforces = Vec2(0, 0);
	
}


float Force::clamp(float val, float min, float max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

Vec2 Force::GenerateDragForce(const Object& object, float k)
{
	Vec2 dragForce = Vec2(0, 0);
	
	if (object.physics.HorzVel.MagnitudeSquared() > 0)
	{
		Vec2 dragDirection = object.physics.HorzVel.UnitVector() * -1.0f;
		float dragMagnitude = k * object.physics.HorzVel.MagnitudeSquared();
	
		dragForce = dragDirection * dragMagnitude;
	
	}

	return dragForce;
}
