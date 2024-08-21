#include "Application.h"



void Application::Setup()
{
	wall.wallTextures();
	//sprite.initSpriteinfo();
	//sprite.initsprites();
	saber.initSaber();
	map.InitMap(MAP_NUM_COLS_X, MAP_NUM_ROWS_Y);
	map.addMapLayer(map.Map_levelOne);
	map.addMapLayer(map.Map_levelTwo);
	map.addMapLayer(map.Map_levelThree);
	map.addTextures(map.Texture_levelOne);
	map.addTextures(map.Texture_levelTwo);
	map.addTextures(map.Texture_levelThree);
	//powers.initSprite();
	OM.InitSprite();
	OM.InitObject();
}

void Application::ProcessInput(olc::PixelGameEngine* pge, float& fElapsedTime)
{
	//player input
	player.processInput(pge,OM.ispickedup, fElapsedTime, map);
	//player.Jumping(pge,map, fElapsedTime);
	OM.Input(pge, player, fElapsedTime);
	
	//object movement input

}

void Application::Update(olc::PixelGameEngine* pge, float& fElapsedTime)
{
	

	player.movePlayer(pge,fElapsedTime, map);




	ray.castAllRays(player, map);

	OM.Update(pge, fElapsedTime, map, player);
	
	
	//powers.TKUpdate(pge, player, map, sprite, fElapsedTime);

	
}

void Application::Render(olc::PixelGameEngine* pge)
{
	wall.renderWallProjection(pge, player, ray, map);
	//sprite.SpriteProjection(pge, ray, player);



	map.renderMapGrid(pge);           // little map
	player.renderMapPlayer(pge);// player in the map
	//sprite.mapSprites(pge,sprite);
	OM.Render(pge, player, ray);
	OM.RenderMapObjects(pge);
	ray.renderMapRays(pge, player, wall.nTestRay);   // rays in the map
}
