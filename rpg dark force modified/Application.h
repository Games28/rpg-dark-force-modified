#ifndef  APPLICATION_H
#define APPLICATION_H
#include "olcPixelGameEngine.h"

#include "defs.h"
#include "Map.h"
#include "Player.h"
#include "Raycast.h"
#include "Wall.h"

#include "Saber.h"
#include "ObjectManager.h"

class Application
{
public:
	Application() = default;

	void Setup();

	void ProcessInput(olc::PixelGameEngine* pge, float& fElapsedTime);
	
	void Update(olc::PixelGameEngine* pge,float & fElapsedTime);

	void Render(olc::PixelGameEngine* pge);

public:
	Map map;
	Player player;
	Raycast ray;
	Wall wall;
	
	Saber saber;
	
	ObjectManager OM;
};


#endif // ! APPLICATION_H


