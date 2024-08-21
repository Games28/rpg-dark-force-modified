#define OLC_PGE_APPLICATION
#include "Application.h"



class darkforce : public olc::PixelGameEngine
{
public:
	darkforce()
	{
		sAppName = "dark force raycaster";
	}
public:
	Application App;
public:

	bool OnUserCreate() override
	{
		App.Setup();
		

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(RENDER_CEILING ? olc::BLACK : olc::BLUE);
        
		App.ProcessInput(this, fElapsedTime);

		App.Update(this,fElapsedTime);

		App.Render(this);

		

		return true;
	}
};

int main()
{
	darkforce game;
	if (game.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 2, 2))
	{
		game.Start();
	}

	return 0;
}
