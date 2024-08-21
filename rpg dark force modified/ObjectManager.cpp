#include "ObjectManager.h"



void Object::Update(Powers& powers,Map& map,Player& player, float deltatime)
{
	if (!pickedup)
	{
		
		
		if (isthrown)
		{
			liftcount--;

			physics.Vertphysicssetup(liftup, 20.0f);
			float pixelpermeter = 50.0f;
			float gravity = physics.VertMass * 9.8f * pixelpermeter;
			//float uplift = -100.0f * pixelpermeter;;
              if (liftcount > 0)
              {
              	//physics.AddVertForce(uplift);
              }
              else
              {
              	physics.AddVertForce(gravity);
              }
			 
             liftup = physics.physicobjectlift(deltatime);
			//powers.TKthrow(*this, map, player, deltatime);
		}
		
		
		


	}
	
}


void Object::HorzMovement(float deltatime, Map& map, Player& player)
{
	

	float movestep = movedirection * movespeed * deltatime;

	float newObjectX = x + cos(player.rotationAngle) * movestep;
	float newObjectY = y + sin(player.rotationAngle) * movestep;

	if (!map.mapHasWallAt(newObjectX, newObjectY))
	{
		x = newObjectX;
		y = newObjectY;

	}
}

void Object::VertMovement(float deltatime, Player& player)
{
	




	
	
}



void ObjectManager::InitSprite()
{
	sprites[0] = new olc::Sprite("npc/probidle.png");
	sprites[1] = new olc::Sprite("npc/r2d2ground.png");
	sprites[2] = new olc::Sprite("npc/trooperT2.png");
	powers.initSprite();
}

void ObjectManager::InitObject()
{
	Object obj;
	obj.x = 300;
	obj.y = 400;
	obj.texture = 2;
	obj.scale = 1.0f;
	obj.stationary = false;
	obj.size = { 60,125 };
	obj.liftup = 0;
	obj.offset = 0;

	Object obj1;
	obj1.x = 200;
	obj1.y = 400;
	obj1.texture = 2;
	obj1.scale = 1.0f;
	obj1.stationary = false;
	obj1.size = { 60,125 };
	obj1.liftup = 0;
	obj1.offset = 0;


	objects.push_back(obj);
	objects.push_back(obj1);
}

void ObjectManager::Update(olc::PixelGameEngine* pge,float deltatime, Map& map, Player& player)
{
	for (auto& obj : objects)
	{
		obj.HorzMovement(deltatime, map, player);
	}

	float fObjPlyA;
	olc::vi2d indicatorPos = { WINDOW_WIDTH / 2, 30 };
	//pge->DrawSprite(indicatorPos, powers.indicatorsprite[0]);
	
	
		if (!ispickedup)
		{

			for (auto& obj : objects)
			{
				ptr = &obj;
				ptr->inSight = powers.isinsight(*ptr, player, 10.0f * (3.14159f / 180.0f), fObjPlyA);



				if (ptr->inSight)
				{


					//pge->DrawSprite(indicatorPos, powers.indicatorsprite[1]);

					if (pge->GetKey(olc::SPACE).bHeld)
					{
						ptr->VertMovement(deltatime, player);
						ptr->liftcount = 20;
						ptr->physics.push = Vec2(50 * PIXELS_PER_METER, 50 * PIXELS_PER_METER);
						ptr->isthrown = false;
						powers.throwcount = 0;
						ptr->pickedup = true;
						ispickedup = true;
						break;
					}



				}

			}



		}
		else
		{


			if (ptr->pickedup)
			{

				//if (player.controller == controlstyle::THROWN)
				//{
				//	powers.TKthrow(*ptr, map, player, deltatime);
				//}
				//else
				{


					ptr->HorzMovement(deltatime, map, player);
					
					//pge->DrawSprite(indicatorPos, powers.indicatorsprite[1]);
					powers.TKmove(pge,*ptr, player, map);
					powers.TKstrafe(*ptr, player);
					powers.TKrotation(pge, *ptr, player, map);
					//if (player.controller == controlstyle::THROWN)
					//{
					//	powers.TKpull(*ptr, player, 10);
					//}
					

				}
			}
			else
			{

			}

			if (pge->GetKey(olc::SPACE).bReleased)
			{
				ptr->ObjectHeight = 0;
				
				ptr->isthrown = true;
			}
			if (!pge->GetKey(olc::SPACE).bHeld)
			{



				ptr->islifting = false;
				ptr->pickedup = false;
				ispickedup = false;
				ptr = nullptr;

			}



		}
	

	for (auto& obj : objects)
	{
		
		obj.Update(powers, map,player,deltatime);
	}
}

void ObjectManager::Input(olc::PixelGameEngine* pge, Player& player, float deltatime)
{
	pge->DrawString(200, 30, "liftup: " + std::to_string(player.fPlayerH));
	for (auto& obj : objects)
	{
		
		
			if (obj.pickedup)
			{

				
				{
					pge->DrawString(200, 20, "liftup: " + std::to_string(obj.liftup));
					
					if (pge->GetKey(olc::J).bHeld) obj.offset += -0.5;
					if (pge->GetKey(olc::L).bHeld) obj.offset += 0.5;
					if (pge->GetKey(olc::I).bHeld) obj.movedirection = +1;
					if (pge->GetKey(olc::K).bHeld) obj.movedirection = -1;

					//test vertical look and move
					//player.strafeupspeed * player.frun* deltatime;
					//if (pge->GetKey(olc::PGDN).bHeld ) obj.liftup += player.strafeupspeed * player.frun * deltatime;
					//if (pge->GetKey(olc::PGUP).bHeld ) obj.liftup -= player.strafeupspeed * player.frun * deltatime;
					
					//if (pge->GetKey(olc::PGDN).bReleased) obj.liftup -= 0;
					//if (pge->GetKey(olc::PGUP).bReleased) obj.liftup += 0;
					
					if (pge->GetKey(olc::UP).bHeld) obj.liftup -= player.lookspeed * deltatime;
					if (pge->GetKey(olc::DOWN).bHeld) obj.liftup += player.lookspeed * deltatime;
					
					if (pge->GetKey(olc::UP).bReleased) obj.liftup -= 0;
					if (pge->GetKey(olc::DOWN).bReleased) obj.liftup += 0;

				
					

				}

				
			}
			else
			{
				if (pge->GetKey(olc::W).bHeld) obj.movedirection = 0;
				if (pge->GetKey(olc::S).bHeld) obj.movedirection = 0;
				if (pge->GetKey(olc::J).bHeld) obj.offset += 0;
				if (pge->GetKey(olc::L).bHeld) obj.offset += 0;
				if (pge->GetKey(olc::LEFT).bHeld) obj.ObjectHeight = 0;
				if (pge->GetKey(olc::RIGHT).bHeld) obj.ObjectHeight = 0;
			}
		
		
	}

}

void ObjectManager::Render(olc::PixelGameEngine* pge, Player& player, Raycast& ray)
{
	int halfscreenheight = WINDOW_HEIGHT * player.fPlayerH;

	auto normalizeAngle = [=](float* angle)
		{
			*angle = remainder(*angle, TWO_PI);
			if (*angle < 0) {
				*angle = TWO_PI + *angle;
			}
		};

	auto distanceBetweenPoints = [=](float x1, float y1, float x2, float y2)
		{
			return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		};


	for (auto& obj : objects)
	{
		obj.visible = false;

		float angleSpritePlayer = atan2(obj.y - player.y, obj.x - player.x) - atan2(sin(player.rotationAngle), cos(player.rotationAngle));

		if (angleSpritePlayer > PI)
			angleSpritePlayer -= TWO_PI;
		if (angleSpritePlayer < -PI)
			angleSpritePlayer += TWO_PI;





		const float ESPSILON = 0.2f;

		const float EPSILON = 0.2f;
		if (angleSpritePlayer < (FOV_ANGLE / 2) + EPSILON) {
			obj.visible = true;
			obj.angle = angleSpritePlayer;

			obj.distance = distanceBetweenPoints(obj.x, obj.y, player.x, player.y);


		}
		else {
			obj.visible = false;
		}
	}



	objects.sort([=](const Object& a, const Object& b)
		{
			return a.distance > b.distance;
		}
	);

	int nHorizonHeight = WINDOW_HEIGHT * player.fPlayerH + (int)player.lookupordown;

	
	

	for (auto& obj : objects)
	{

		if (obj.visible)
		{



			//float newheight = lookdividedhundred - heightminuslook;




			//object_t sprite = visibleSprites[i];

			float fVecX = obj.x - player.x;
			float fVecY = obj.y - player.y;
			float distancefromplayer = sqrtf(fVecX * fVecX + fVecY * fVecY);




			float fPlayerA_rad = deg2rad(player.rotationAngle);
			float fEyeX = cosf(fPlayerA_rad);
			float fEyeY = sinf(fPlayerA_rad);
			obj.rotationangle = atan2f(fEyeY, fEyeX) - atan2f(fVecY, fVecX);




			if (obj.offset < -3.14159f)
				obj.offset += 2.0f * 3.14159f;

			if (obj.offset > 3.14159f)
				obj.offset -= 2.0f * 3.14159f;

			obj.rotationangle += obj.offset;

			if (obj.rotationangle < -3.14159f)
				obj.rotationangle += 2.0f * 3.14159f;

			if (obj.rotationangle > 3.14159f)
				obj.rotationangle -= 2.0f * 3.14159f;



			float fPlayerFOV_rad = deg2rad(60.0f);
			float fCompensatePlayerHeight = (player.fPlayerH - 0.5f) * 64;

			float  fObjHlveSliceHeight = float(WINDOW_HEIGHT / obj.distance);
			float  spriteheight = (TILE_SIZE / obj.distance) * DIST_TO_PROJ_PLANE;
			float  fObjHlveSliceHeightScld = float((WINDOW_HEIGHT * 0.5f) / obj.distance);

			float fObjCeilingNormalized = float(nHorizonHeight) - fObjHlveSliceHeight;
			float fObjCeilingScaled = float(nHorizonHeight) - obj.distance;
			// and adapt all the scaling into the ceiling value
			float fScalingDifference = fObjCeilingNormalized - fObjCeilingScaled;
			float fObjCeiling = float( nHorizonHeight) - (spriteheight / 2) * obj.scale;
			float fObjFloor = float( nHorizonHeight) + (spriteheight / 2);

			fObjCeiling += fCompensatePlayerHeight * fObjHlveSliceHeight * 2.0f;
			fObjFloor += fCompensatePlayerHeight * fObjHlveSliceHeight * 2.0f;
			

			if (obj.pickedup)
			{
				fObjCeiling += obj.liftup;
				fObjFloor += obj.liftup;
			}
			
			
		

			float fObjHeight = fObjFloor - fObjCeiling;
			float fObjAR = float(sprites[obj.texture]->width) / float(sprites[obj.texture]->height);
			float fObjWidth = fObjHeight / fObjAR;

			float fMidOfObj = (0.5f * (obj.angle / (fPlayerFOV_rad / 2.0f)) + 0.5f) * float(WINDOW_WIDTH);







			// render the sprite
			for (float fx = 0.0f; fx < (int)fObjWidth; fx++) {
				// get distance across the screen to render
				int nObjColumn = int(fMidOfObj + fx - (fObjWidth / 2.0f));
				// only render this column if it's on the screen
				if (nObjColumn >= 0 && nObjColumn < WINDOW_WIDTH) {
					for (float fy = 0.0f; fy < (int)fObjHeight; fy++) {
						// calculate sample coordinates as a percentage of object width and height
						float fSampleX = fx / fObjWidth;
						float fSampleY = fy / fObjHeight;
						// sample the pixel and draw it
						//if (fx > 0 && fx < WINDOW_WIDTH && fy > 0 && fy < WINDOW_HEIGHT)
						{
							olc::Pixel pSample;
							//new
							if (obj.stationary)
							{
								pSample = sprites[obj.texture]->Sample(fSampleX, fSampleY);
							}
							else
							{
								pSample = SelectedPixel(pge, &obj, sprites[obj.texture], obj.size, fSampleX, fSampleY, obj.rotationangle);
							}

							float dist = ray.rays[nObjColumn].listinfo[0].frontdistance;
							if (obj.distance < dist)
							{
								if (pSample != olc::MAGENTA && ray.Depthbuffer[nObjColumn] >= obj.distance)
								{
									pge->SetPixelMode(olc::Pixel::ALPHA);
									pge->SetPixelBlend(0.50f);
									pge->Draw(nObjColumn, fObjCeiling + fy, pSample);
									pge->SetPixelMode(olc::Pixel::NORMAL);
									ray.Depthbuffer[nObjColumn] = obj.distance;

								}
							}
						}
					}
				}

			}

		}
	}

	
}

void ObjectManager::RenderMapObjects(olc::PixelGameEngine* pge)
{
	for (auto& obj : objects) {

		olc::Pixel p;
		if (obj.visible)
		{
			p = olc::WHITE;
		}
		else
		{
			p = olc::GREY;
		}
		pge->FillRect(
			obj.x * MINIMAP_SCALE_FACTOR,
			obj.y * MINIMAP_SCALE_FACTOR,
			2,
			2,
			p
		);

		pge->DrawLine(
			obj.x * MINIMAP_SCALE_FACTOR,
			obj.y * MINIMAP_SCALE_FACTOR,
			(obj.x + cos(obj.offset) * 40) * MINIMAP_SCALE_FACTOR,
			(obj.y + sin(obj.offset) * 40) * MINIMAP_SCALE_FACTOR
			);
	}
}

olc::Pixel ObjectManager::SelectedPixel(olc::PixelGameEngine* ptr, Object* obj, olc::Sprite* sprite, olc::vf2d size, float samplex, float sampley, float Angle)
{

	auto is_in_range = [=](float a, float low, float high) {
		return (low <= a && a < high);
		};

	// "bodge" angle in the range [ -PI, PI )

	olc::vf2d TileSize = { 60,125 };
	olc::vf2d vOffset;
	if (is_in_range(Angle, -0.75f * 3.14159265f, -0.25f * 3.14159265f))
		vOffset = { 1.0f, 0.0f }; else   // sprite from the left
		if (is_in_range(Angle, -0.25f * 3.14159265f, +0.25f * 3.14159265f))
			vOffset = { 0.0f, 0.0f }; else   //                 back
			if (is_in_range(Angle, +0.25f * 3.14159265f, +0.75f * 3.14159265f))
				vOffset = { 3.0f, 0.0f };
			else   //                 right
				vOffset = { 2.0f, 0.0f };        //                 front




	// the subsprites are 100 x 100 pixels
	olc::vf2d vSample = ((vOffset + olc::vf2d(samplex, sampley)) * size) / olc::vf2d(float(sprite->width), float(sprite->height));




	//test
	olc::Pixel p = sprite->Sample(vSample.x, vSample.y);



	return p;
}




