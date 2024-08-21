#ifndef PLAYER_H
#define PLAYER_H
#include "Map.h"
#include "olcPixelGameEngine.h"
#include "defs.h"
#include "MapEditor.h"
#include "Physics.h"

enum controlstyle
{
	STASIONARY,
	MOVEMENT,
	PULLED,
	THROWN
};

class Player
{
public:
	Player();
	~Player();
	void processInput(olc::PixelGameEngine* PGEptr,bool& pickedup,float deltatime, Map& map);
	void movePlayer(olc::PixelGameEngine* pge, float deltatime, Map& map);
	void renderMapPlayer(olc::PixelGameEngine* PGEptr);
	bool GetMouseSteering(olc::PixelGameEngine *pge, float& fHorPerc, float& fVerPerc);
	void Jumping(olc::PixelGameEngine* pge,Map& map, float deltatime);

public:
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for back, +1 for front
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	
	bool strafeLeft;
	bool strafeRight;
	int strafedirection;
	int frun;
	float lookupordown;
	float lookspeed;
	float vertlook;
	float strafeupspeed;
	float fPlayerH;
	bool movevert;
	bool lookvert;
	float strafePlayerX = 0;
	float strafePlayerY = 0;

	float rotatebefore;
	float rotateafter;
	float rotationdifference;

	olc::vf2d movementbefore;
	olc::vf2d movementafter;
	olc::vf2d movedifference;

	olc::vf2d strafebefore;
	olc::vf2d strafeafter;
	olc::vf2d strafedifference;
	bool bmousecontrol = false;
	bool jumping = false;
	float mass = 4.0f;
	float pixelpermeter = 50.0f;
	float gravity = mass * 9.8f * pixelpermeter;
	int liftcount = 10;
	Physics physics;
 	controlstyle controller = controlstyle::MOVEMENT;
};

#endif // !PLAYER_H


