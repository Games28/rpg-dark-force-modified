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
		float rayAngle = player.rotationAngle + (x - NUM_RAYS / 2) / (float)(NUM_RAYS)*FOV_ANGLE;
		float fFoV = 60.0f;
		float fAngleStep = fFoV / (float)NUM_RAYS;
		float fViewAngle = (float)(x - (NUM_RAYS / 2)) * fAngleStep;
		float fCurAngle = (player.rotationAngle * 180.0f / PI) + fViewAngle;
		float fPlayerX = player.x;
		float fPlayerY = player.y;
		float fCosCurAngle = cos(fCurAngle * PI / 180.0f);
		float fSinCurAngle = sin(fCurAngle * PI / 180.0f);
		float fCosViewAngle = cos(fViewAngle * PI / 180.0f);

		float fx_hit, fy_hit, fBlockElevation = 1.0f, front_dist, back_dist;
		int nx_hit, ny_hit, block_level, WallTop, WallTop2, WallBottom, WallBottom2;
		
		std::vector<intersectInfo> vHitPointList;
		

		for (int k = 0; k < map.fMaps.size(); k++)
		{
			std::vector<intersectInfo> vCurLevelList;

			rays.Raycasting(rayAngle, k, vCurLevelList, player, map);

			for (int i = 0; i < (int)vCurLevelList.size(); i++)
			{
				vCurLevelList[i].frontdistance *= cos(fViewAngle * PI / 180.0f);
				calculateBottomAndTop(
					vCurLevelList[i].frontdistance,
					nHorizonHeight,
					vCurLevelList[i].maplevel,
					vCurLevelList[i].FHeight,
					vCurLevelList[i].ceil_front,
					vCurLevelList[i].bottom_front,
					player
				);
			}

			for (int i = 0; i < (int)vCurLevelList.size(); i++)
			{
				if (i == (int)vCurLevelList.size() - 1)
				{
					vCurLevelList[i].backdistance = vCurLevelList[i].frontdistance;
					vCurLevelList[i].ceil_back = vCurLevelList[i].ceil_front;
					vCurLevelList[i].bottom_back = vCurLevelList[i].bottom_front;
				}
				else
				{
					vCurLevelList[i].backdistance = vCurLevelList[i + 1].frontdistance;

					calculateBottomAndTop(
						vCurLevelList[i].backdistance,
						nHorizonHeight,
						vCurLevelList[i].maplevel,
						vCurLevelList[i].FHeight,
						vCurLevelList[i].ceil_back,
						vCurLevelList[i].bottom_back,
						player
					);
				}
			}

			vHitPointList.insert(vHitPointList.end(), vCurLevelList.begin(), vCurLevelList.end());
		}

		vHitPointList.erase(
			std::remove_if(
				vHitPointList.begin(),
				vHitPointList.end(),
				[](intersectInfo& a) {
					return a.FHeight == 0.0f;
				}
			),
			vHitPointList.end()
		);
		 
		std::sort(
			vHitPointList.begin(),
			vHitPointList.end(),
			[](intersectInfo& a, intersectInfo& b) {
				return (a.frontdistance > b.frontdistance) ||
					(a.frontdistance == b.frontdistance && a.maplevel < b.maplevel);
			}
		);

		


		for (int y = WINDOW_HEIGHT - 1; y >= 0; y--)
		{
			if (y < nHorizonHeight)
			{
				olc::Pixel skySample = olc::CYAN;
				PGEptr->Draw(x, y, skySample);
			}
			else
			{
				float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(y - nHorizonHeight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
				float fFloorProjX = player.x + fFloorProjDistance * fCosCurAngle;
				float fFloorProjY = player.y + fFloorProjDistance * fSinCurAngle;
				float fTILE_SIZE = TILE_SIZE;
				//while (fFloorProjX < 0.0f) { fFloorProjX += floor(fTILE_SIZE); }
				//while (fFloorProjY < 0.0f) { fFloorProjY += floor(fTILE_SIZE); }
				int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
				int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

				olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);
				//olc::Pixel p = SelectSceneryPixel(0, nSampleX, nSampleY, fDistnace, Side::Bottom);
				PGEptr->Draw(x, y, p);

			}
		}


		for (int i = 0; i < vHitPointList.size(); i++)
		{
			intersectInfo& hitRec = vHitPointList[i];
		
			if (hitRec.FHeight > 0.0f)
			{
				fx_hit = hitRec.wallHitX;
				fy_hit = hitRec.wallHitY;
				nx_hit = hitRec.mapX;
				ny_hit = hitRec.mapY;
				fBlockElevation = hitRec.FHeight;
				block_level = hitRec.maplevel;
				front_dist = hitRec.frontdistance;
				back_dist = hitRec.backdistance;
		
				WallTop = hitRec.ceil_front; WallTop < 0 ? 0 : WallTop;
				WallTop2 = hitRec.ceil_back; WallTop2 < 0 ? 0 : WallTop2;
				WallBottom = hitRec.bottom_front; WallBottom > WINDOW_HEIGHT ? WINDOW_HEIGHT : WallBottom;
				WallBottom2 = hitRec.bottom_back; WallBottom2 > WINDOW_HEIGHT ? WINDOW_HEIGHT : WallBottom2;
		
				for (int y = WallTop2; y < WallTop; y++)
				{
					float fheight = float(block_level) + fBlockElevation;
					float fRoofProjDistance = (((player.fPlayerH - float(fheight)) * TILE_SIZE / float(y - nHorizonHeight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
					
					float fRoofProjX = player.x + fRoofProjDistance * fCosCurAngle;
					float fRoofProjY = player.y + fRoofProjDistance * fSinCurAngle;
				
					int nSampleX = (int)(fRoofProjX) % TILE_SIZE;
					int nSampleY = (int)(fRoofProjY) % TILE_SIZE;
					olc::Pixel p = sprites[1].GetPixel(nSampleX, nSampleY);
					//olc::Pixel p = SelectSceneryPixel(1, nSampleX, nSampleY, front_dist, Side::Roof);
					PGEptr->Draw(x, y, p);
				}
		
				for (int y = WallTop; y < WallBottom; y++)
				{
					float fSampleY = 0;
					int nDisplayBlockHeight = 0;
					int textureid = 1;
					fSampleY = float(y - WallTop) / float(WallBottom - WallTop);
		
					float fSampleX;
		
					int indexX = hitRec.mapX;
					int indexY = hitRec.mapY;
					//textureid = getTexture(indexX, indexY, nDisplayBlockHeight, map);
					if (hitRec.wasHitVertical) {
						fSampleX = (int)hitRec.wallHitY % TILE_SIZE;
					}
					else
					{
						fSampleX = (int)hitRec.wallHitX % TILE_SIZE;
					}
		
					//fSampleX = int(fSampleX) % TILE_SIZE;
					fSampleY = fSampleY * float(TILE_SIZE);
					// having both sample coordinates, get the sample and draw the pixel
		
					olc::Pixel auxSample = sprites[textureid].GetPixel(fSampleX, fSampleY);
					//olc::Pixel auxSample = SelectSceneryPixel(textureid, (int)fSampleX, (int)fSampleY, fDistnace, Side::WalL);
					PGEptr->Draw(x, y, auxSample);
		
				}

				//for (int y = WallBottom + 1; y <= WallBottom2; y++)
				//{
				//	float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(nHorizonHeight - y)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
				//	float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
				//	float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
				//	int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
				//	int nSampleY = (int)(fFloorProjY) % TILE_SIZE;
				//
				//	olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);
				//	//olc::Pixel p = SelectSceneryPixel(0, nSampleX, nSampleY, fDistnace, Side::Top);
				//	PGEptr->Draw(x, y, p);
				//
				//}
		
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

