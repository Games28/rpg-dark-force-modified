#ifndef RAYCAST_H
#define RAYCAST_H

#include <vector>

#include "olcPixelGameEngine.h"
#include "defs.h"
#include "Player.h"
#include "Map.h"

struct intersectInfo
{
	float wallHitX;
	float wallHitY;
	int mapX;
	int mapY;
	float frontdistance;
	float backdistance;
	int height;
	bool wasHitVertical;
	float rayAngle;
	int texture;
	

	//multilevel flying and crouching
	int bottom_front;
	int bottom_back;
	int ceil_front;
	int ceil_back;

	// for debugging purposes
	bool rayUp, rayDn, rayLt, rayRt;

	int maplevel;
	float FHeight;
	//new
	float fDistanceFront;
};


struct Ray {
	std::vector<intersectInfo> listinfo;
	
};



class Raycast
{
public:
	Raycast() = default;
	void castAllRays(Player& player, Map& map);
	void castRay(float rayAngle, int stripID,int maplevel, Player& player, Map& map);
	void renderMapRays(olc::PixelGameEngine* PGEptr, Player& player, int testray); // Joseph21 - added testray for debugging

public:
	Ray rays[NUM_RAYS];
	float* Depthbuffer = new float[WINDOW_WIDTH];
	float fMaxDistnace = (float)sqrt(MAP_NUM_COLS_X * MAP_NUM_COLS_X + MAP_NUM_ROWS_Y * MAP_NUM_ROWS_Y);

	
};

#endif // !RAYCAST_H


