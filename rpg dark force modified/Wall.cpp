#include "Wall.h"

void Wall::wallTextures()
{
    //std::string sPath = "image/";
	sprites[0] = olc::Sprite("scenery/sand1.png");
	sprites[1] = olc::Sprite("scenery/stonewall.png");
	sprites[2] = olc::Sprite("scenery/Tatooinedoor1.png");
	sprites[3] = olc::Sprite("scenery/Tatooinedoor2.png");
	sprites[4] = olc::Sprite("scenery/tatooinewindow2.png");
	sprites[5] = olc::Sprite("scenery/Tatooinehousewall.png");
	
	

	std::cout << "All textures loaded." << std::endl;
}

void Wall::changeColorIntensity(olc::Pixel& p, float factor)
{
    // Joseph21 - assumed implementation
    p *= factor;
}

void Wall::calculateBottomAndTop(float wallDistance, int horizonheight,int maplevel, float wallheight, int& wallceil, int& wallfloor, Player &player)
{
	int nsliceHeight = ((TILE_SIZE / wallDistance) * DIST_TO_PROJ_PLANE);
	wallceil  = horizonheight - (float(nsliceHeight) * (1.0f - player.fPlayerH)) - (maplevel + wallheight - 1) * nsliceHeight;
	wallfloor = wallceil + nsliceHeight * wallheight;
}

olc::Pixel Wall::SelectSceneryPixel(int textureid, int samplex, int sampley, float distance, Side side)
{
	olc::Pixel p;

	int32_t isamplex = samplex;
	int32_t isampley = sampley;

	switch (side)
	{
	case Side::Top:
	{
		p = sprites[textureid].GetPixel(isamplex, isampley);
	}break;

	case Side::Roof:
	{
		p = sprites[textureid].GetPixel(isamplex, isampley);
	}break;

	case Side::Bottom:
	{
		p = sprites[textureid].GetPixel(isamplex, isampley);
		//float fdistance = ((3.0f / distance) * 100.0f);
		//
		//if (fdistance >= 1.0f) fdistance = 1.0f;
		//
		//
		//
		//p.r = uint8_t(float(p.r) * fdistance);
		//p.g = uint8_t(float(p.g) * fdistance);
		//p.b = uint8_t(float(p.b) * fdistance);
	}break;

	case Side::WalL:
	{
		p = sprites[textureid].GetPixel(isamplex, isampley);

		float brightness = 3.0;
		float scale = 0.0f;
		if (distance >= 10) scale = 10.0f * brightness;
		if (distance >= 100) scale = 100.0f * brightness;
		float fdistance = ((1.0 / distance) * scale);
		
		if (fdistance >= 1.0f) fdistance = 1.0f;
		


		p.r = uint8_t(float(p.r) * fdistance);
		p.g = uint8_t(float(p.g) * fdistance);
		p.b = uint8_t(float(p.b) * fdistance);
		
	}break;

	case Default:
	{
		std::cout << "error with texture" << std::endl;
	}break;
	}


	return p;
}



void Wall::renderWallProjection(olc::PixelGameEngine* PGEptr, Player& player, Raycast& rays, Map& map)
{



	int halfscreenwidth  = WINDOW_WIDTH / 2;
	int nHorizonHeight = WINDOW_HEIGHT * player.fPlayerH + (int)player.lookupordown;
	float anglestep = 60 / float(WINDOW_WIDTH);
	
	for (int x = 0; x < NUM_RAYS; x++) {     // iterate over all slices of the screen from left to right

        // work out angle from player perspective belonging to this slice
		float fViewangle = float(x - halfscreenwidth) * anglestep;

		int wallTopY, wallBottomY, nWallCeil, nWallCeil2, nWallFloor, nWallFloor2;
		int colheight, coordX, coordY;
		float Fcolheight, fDistnace;
		
		// calculated corrected distance as well as bottom and top of the wall projection - per hitpoint
		for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
		{
			rays.rays[x].listinfo[i].frontdistance *= cos(fViewangle * PI / 180.0f);
			calculateBottomAndTop(rays.rays[x].listinfo[i].frontdistance, nHorizonHeight, rays.rays[x].listinfo[i].maplevel, rays.rays[x].listinfo[i].FHeight, rays.rays[x].listinfo[i].ceil_front, rays.rays[x].listinfo[i].bottom_front, player);
		}

		for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
		{
			if (i == (int)rays.rays[x].listinfo.size() - 1)
			{
				rays.rays[x].listinfo[i].ceil_back = rays.rays[x].listinfo[i].ceil_front;
				rays.rays[x].listinfo[i].bottom_back = rays.rays[x].listinfo[i].bottom_front;
				rays.rays[x].listinfo[i].backdistance = rays.rays[x].listinfo[i].frontdistance;
			}
			else
			{
				rays.rays[x].listinfo[i].backdistance = rays.rays[x].listinfo[i + 1].frontdistance;
				calculateBottomAndTop(rays.rays[x].listinfo[i].backdistance, nHorizonHeight, rays.rays[x].listinfo[i].maplevel, rays.rays[x].listinfo[i].FHeight, rays.rays[x].listinfo[i].ceil_back, rays.rays[x].listinfo[i].bottom_back, player);
			}
		}

		float fFoV = 60.0f;
		float fAngleStep = fFoV / (float)NUM_RAYS;
		float fViewAngle = (float)(x - (NUM_RAYS / 2)) * fAngleStep;
		float fCurAngle = (player.rotationAngle * 180.0f / PI) + fViewAngle;
		float fPlayerX = player.x;
		float fPlayerY = player.y;
		float fCosCurAngle = cos(fCurAngle * PI / 180.0f);
		float fSinCurAngle = sin(fCurAngle * PI / 180.0f);
		float fCosViewAngle = cos(fViewAngle * PI / 180.0f);


		//sky and floor rendering
		
		for (int i = 0; i < rays.rays[x].listinfo.size(); i++)
		{
			intersectInfo& hitRec = rays.rays[x].listinfo[i];

			if (hitRec.FHeight > 0.0f)
			{

				Fcolheight = hitRec.FHeight;
				nWallCeil = hitRec.ceil_front;
				nWallCeil2 = hitRec.ceil_back;
				nWallFloor = hitRec.bottom_front;
				nWallFloor2 = hitRec.bottom_back;
				coordX = int(hitRec.wallHitX);
				coordY = int(hitRec.wallHitY);
				fDistnace = hitRec.frontdistance;

				for (int y = WINDOW_HEIGHT - 1; y >= 0; y--)
				{
					if (y < nHorizonHeight)
					{
						float fRoofProjDistance = (((player.fPlayerH - float(Fcolheight)) * TILE_SIZE / float(y - nHorizonHeight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
						float fRoofProjX = player.x + fRoofProjDistance * fCosCurAngle;
						float fRoofProjY = player.y + fRoofProjDistance * fSinCurAngle;

						int nSampleX = (int)(fRoofProjX) % TILE_SIZE;
						int nSampleY = (int)(fRoofProjY) % TILE_SIZE;
						olc::Pixel p = olc::CYAN;
						//olc::Pixel p = SelectSceneryPixel(1, nSampleX, nSampleY, fDistnace, Side::Roof);
						PGEptr->Draw(x, y, p);


					}
					else
					{
						float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(y - nHorizonHeight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
						float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
						float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
						float fTILE_SIZE = TILE_SIZE;
						//while (fFloorProjX < 0.0f) { fFloorProjX += floor(fTILE_SIZE); }
						//while (fFloorProjY < 0.0f) { fFloorProjY += floor(fTILE_SIZE); }
						int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
						int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

						//olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);
						olc::Pixel p = SelectSceneryPixel(0, nSampleX, nSampleY, fDistnace, Side::Bottom);
						PGEptr->Draw(x, y, p);
					}
				}


				for (int y = nWallCeil2; y < nWallCeil; y++)
				{
					float fRoofProjDistance = (((player.fPlayerH - float(Fcolheight)) * TILE_SIZE / float(y - nHorizonHeight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
					float fRoofProjX = player.x + fRoofProjDistance * fCosCurAngle;
					float fRoofProjY = player.y + fRoofProjDistance * fSinCurAngle;

					int nSampleX = (int)(fRoofProjX) % TILE_SIZE;
					int nSampleY = (int)(fRoofProjY) % TILE_SIZE;
					//olc::Pixel p = sprites[1].GetPixel(nSampleX, nSampleY);
					olc::Pixel p = SelectSceneryPixel(1, nSampleX, nSampleY, fDistnace, Side::Roof);
					PGEptr->Draw(x, y, p);
			
				}

				for (int y = nWallCeil; y <= nWallFloor; y++)
				{
					float fSampleY = 0;
					int nDisplayBlockHeight = 0;
					int textureid = 1;
					fSampleY = float(y - nWallCeil) / float(nWallFloor - nWallCeil);

					float fSampleX;

					int indexX = rays.rays[x].listinfo[i].mapX;
					int indexY = rays.rays[x].listinfo[i].mapY;
					//textureid = getTexture(indexX, indexY, nDisplayBlockHeight, map);
					if (rays.rays[x].listinfo[i].wasHitVertical) {
						fSampleX = (int)rays.rays[x].listinfo[i].wallHitY % TILE_SIZE;
					}
					else
					{
						fSampleX = (int)rays.rays[x].listinfo[i].wallHitX % TILE_SIZE;
					}

					//fSampleX = int(fSampleX) % TILE_SIZE;
					fSampleY = fSampleY * float(TILE_SIZE);
					// having both sample coordinates, get the sample and draw the pixel

				  olc::Pixel auxSample = sprites[textureid].GetPixel(fSampleX, fSampleY);
					//olc::Pixel auxSample = SelectSceneryPixel(textureid, (int)fSampleX, (int)fSampleY, fDistnace, Side::WalL);
					PGEptr->Draw(x, y, auxSample);
					//DepthDraw(x, y, rays.rays[x].listinfo[0].distance, auxSample);
				}

				for (int y = nWallFloor + 1; y <= nWallFloor2; y++)
				{
					float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(nHorizonHeight - y)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
					float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
					float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
					int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
					int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

					//olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);
					olc::Pixel p = SelectSceneryPixel(0, nSampleX, nSampleY, fDistnace, Side::Top);
					PGEptr->Draw(x, y, p);
				}


			}
		}


		
//	
	}
}

int Wall::getTexture(int x,int y, int& id, Map& map)
{
	
	int textureid[3];
	int selected = id - 1;
	
    return map.iTextures[selected][y * map.MapX + x];
	
}

