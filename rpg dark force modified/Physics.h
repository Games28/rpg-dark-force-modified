#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec2.h"
#include "defs.h"
class Object;

class Physics
{
public:
	Physics();
	void integrate(float& deltatime);
	void physicsconstants();
	float physicobjectlift(float& deltatime);

	//vertical physics
	void Vertphysicssetup(float lift, float mass);
	void Vertintegrate(float& deltatime);
	float VertIntegrate(float& deltatime);
	float VertIntegr(float& deltatime);
	void VertClearForces();
	void AddVertForce(const float& force);

	//horiziontal physics
	void Horzphysicssetup(float mass);
	void Horzintegrate(float& deltatime);
	Vec2 HorzIntegrate(float &rotationangle, float& deltatime);
	void AddHorzForces(const Vec2& force);
	void HorzClearForces();

public:
	bool isfalling;
	bool iscaught;
	
	int pixels_per_meter;
	
	float vel;
	float accelerate;
	float deltatime;

	//vertical physics
	float Vertpos;
	float VertAccelerate;
	float VertVel;
	float VertsumForces;
	float VertMass;
	float VertinvMass;
	int Vertradius;


	//horiziontal physics
	Vec2 Horzpos;
	Vec2 HorzVel;
	Vec2 HorzAccelerate;
	Vec2 HorzSumforces;
	float HorzMass;
	float HorzInvMass;
	float fallcount = 100.0f;
	
	Vec2 push = Vec2(50 * PIXELS_PER_METER, 50 * PIXELS_PER_METER);
};


class Force
{
public:
	Force() = default;
	static float clamp(float val, float min, float max);
	static Vec2 GenerateDragForce(const Object& object, float k);
	static Vec2 GenerateFrictionForce(const Object& object, float k);
};

#endif // !PARTICLE_H


